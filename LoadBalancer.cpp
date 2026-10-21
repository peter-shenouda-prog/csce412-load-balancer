/** @file LoadBalancer.cpp @brief Discrete-clock load balancing implementation. */
#include "LoadBalancer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
LoadBalancer::LoadBalancer(const Config& c):config(c),generator(c),
    blockedLow(ipv4(c.blockedStart)),blockedHigh(ipv4(c.blockedEnd)) {
    config.validate();
    log.open(config.logFile);
    if(!log) throw std::runtime_error("Cannot write log: "+config.logFile);
    for(int i=0;i<config.servers;++i) servers.push_back(std::make_unique<WebServer>(nextId++));
    minServers=peakServers=config.servers;
    const auto target=static_cast<std::size_t>(config.servers)*config.initialPerServer;
    // Refill after firewall rejection so the initial accepted queue is exactly the target.
    std::size_t attempts=0;
    while(requests.size()<target) {
        if(++attempts>target*100+1000) throw std::runtime_error("Firewall prevents initial queue fill");
        accept(generator.next());
    }
    queuePeak=requests.size();
}
int LoadBalancer::activeCount() const {
    int count=0; for(const auto& s:servers) if(!s->retiring()) ++count; return count;
}
int LoadBalancer::busyCount() const {
    int count=0; for(const auto& s:servers) if(s->busy()) ++count; return count;
}
void LoadBalancer::emit(const std::string& text,const std::string& color) {
    if(config.color && !color.empty()) std::cout<<color;
    std::cout<<text<<'\n';
    if(config.color && !color.empty()) std::cout<<"\033[0m";
    log<<text<<'\n';
}
void LoadBalancer::event(const std::string& text,const std::string& color) {
    if(config.verbose) emit("[cycle "+std::to_string(cycle)+"] "+text,color);
}
void LoadBalancer::accept(const Request& request) {
    ++generated;
    auto source=ipv4(request.ipIn);
    if(source>=blockedLow && source<=blockedHigh) {
        ++rejected; event("BLOCK source="+request.ipIn,"\033[31m");
    } else { requests.push(request); queuePeak=std::max(queuePeak,requests.size()); }
}
void LoadBalancer::scale() {
    if(cycle-lastScale<config.cooldown) return;
    int count=activeCount();
    if(requests.size()>static_cast<std::size_t>(80)*count) {
        if(static_cast<int>(servers.size())<config.maxServers) {
            servers.push_back(std::make_unique<WebServer>(nextId++));
            ++additions; lastScale=cycle;
            event("SCALE UP active="+std::to_string(count+1)+" queue="+std::to_string(requests.size()),"\033[32m");
        } else { ++capacityLimited; lastScale=cycle; event("CAPACITY LIMIT reached","\033[31m"); }
    } else if(count>1 && requests.size()<static_cast<std::size_t>(50)*count) {
        // Prefer an idle server. A busy server drains before its allocation is freed.
        auto it=std::find_if(servers.begin(),servers.end(),[](const auto& s){return !s->retiring()&&!s->busy();});
        if(it==servers.end()) it=std::find_if(servers.begin(),servers.end(),[](const auto& s){return !s->retiring();});
        (*it)->retire(); ++removals; lastScale=cycle;
        event("SCALE DOWN retire server="+std::to_string((*it)->id())+" active="+std::to_string(count-1)+" queue="+std::to_string(requests.size()),"\033[33m");
    }
    minServers=std::min(minServers,activeCount()); peakServers=std::max(peakServers,activeCount());
}
void LoadBalancer::removeDrained() {
    servers.erase(std::remove_if(servers.begin(),servers.end(),[this](const auto& s){
        if(s->retiring()&&!s->busy()) { ++deallocated; event("DEALLOCATE server="+std::to_string(s->id())); return true; }
        return false;
    }),servers.end());
}
void LoadBalancer::checkConservation() const {
    auto inFlight=busyCount();
    if(generated!=rejected+completed+static_cast<long long>(requests.size())+inFlight ||
       assigned!=completed+inFlight || completed!=processing+streaming)
        throw std::logic_error("Request conservation failure");
}
void LoadBalancer::summary(const std::string& phase) {
    std::ostringstream s; s<<std::fixed<<std::setprecision(2);
    s<<"=== "<<phase<<" SUMMARY ===\n"
     <<"Initial servers: "<<config.servers<<"\nConfigured clock cycles: "<<config.cycles
     <<"\nElapsed clock cycles: "<<cycle<<"\nQueue size: "<<requests.size()
     <<"\nTask time range: "<<config.minTime<<".."<<config.maxTime<<" cycles (inclusive)"
     <<"\nSeed: "<<config.seed<<"\nArrival probability per cycle: "<<config.arrivalProbability
     <<"\nArrival burst range: "<<config.burstMin<<".."<<config.burstMax
     <<"\nScaling thresholds: below 50 * active servers / above 80 * active servers"
     <<"\nScaling cooldown: "<<config.cooldown<<" cycles"
     <<"\nBlocked source range: "<<config.blockedStart<<".."<<config.blockedEnd
     <<"\nGenerated requests (including rejected): "<<generated
     <<"\nAccepted requests: "<<generated-rejected<<"\nRejected requests: "<<rejected
     <<"\nDiscarded accepted requests: 0\nAssigned requests: "<<assigned
     <<"\nCompleted requests: "<<completed<<"\nCompleted processing: "<<processing
     <<"\nCompleted streaming: "<<streaming<<"\nArrival events: "<<arrivalEvents
     <<"\nNew requests after initialization: "<<arrivingRequests
     <<"\nActive servers (accepting work): "<<activeCount()
     <<"\nBusy servers (including draining): "<<busyCount()
     <<"\nIdle active servers: "<<activeCount()-(busyCount()-(static_cast<int>(servers.size())-activeCount()))
     <<"\nDraining servers: "<<static_cast<int>(servers.size())-activeCount()
     <<"\nAllocated servers: "<<servers.size()<<"\nDeallocated inactive servers: "<<deallocated
     <<"\nServer additions: "<<additions<<"\nServer retirements: "<<removals
     <<"\nMinimum / maximum active servers: "<<minServers<<" / "<<peakServers
     <<"\nCapacity-limit events: "<<capacityLimited<<"\nPeak queue size: "<<queuePeak
     <<"\nAverage end-of-cycle queue: "<<(cycle?static_cast<double>(queueTotal)/cycle:0)
     <<"\nServer utilization (% of allocated server-cycles): "<<(availableCycles?100.0*busyCycles/availableCycles:0)
     <<"\nConservation: generated = rejected + completed + queued + in-flight"
     <<"\nConservation check: PASS\n";
    emit(s.str(),"\033[36m");
}
void LoadBalancer::run() {
    checkConservation(); summary("START");
    for(cycle=1;cycle<=config.cycles;++cycle) {
        if(generator.arrives()) {
            ++arrivalEvents; int n=generator.burst(); arrivingRequests+=n;
            event("ARRIVAL count="+std::to_string(n),"\033[34m");
            for(int i=0;i<n;++i) accept(generator.next());
        }
        scale(); removeDrained();
        for(auto& server:servers) {
            if(!server->busy()&&!server->retiring()&&!requests.empty()) {
                Request r=requests.front(); requests.pop(); server->assign(r); ++assigned;
                event("ASSIGN server="+std::to_string(server->id())+" type="+r.jobType+" time="+std::to_string(r.time));
            }
            ++availableCycles;
            if(server->busy()) ++busyCycles;
            auto done=server->tick();
            if(done) { ++completed; if(done->jobType=='P') ++processing; else ++streaming;
                event("COMPLETE server="+std::to_string(server->id())); }
        }
        removeDrained(); queueTotal+=static_cast<long long>(requests.size()); checkConservation();
    }
    cycle=config.cycles; summary("END");
}
