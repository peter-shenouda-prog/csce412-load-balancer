/** @file Config.cpp @brief Strict configuration and IPv4 parsing. */
#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <set>
namespace {
std::string trim(const std::string& s) {
    auto a=s.find_first_not_of(" \t\r\n");
    return a==std::string::npos ? "" : s.substr(a,s.find_last_not_of(" \t\r\n")-a+1);
}
int integer(const std::string& s) {
    std::size_t n=0; int v=std::stoi(s,&n);
    if(n!=s.size()) throw std::runtime_error("Invalid integer: "+s);
    return v;
}
bool boolean(const std::string& s) {
    if(s=="true") return true;
    if(s=="false") return false;
    throw std::runtime_error("Boolean must be true or false");
}
}
std::uint32_t ipv4(const std::string& text) {
    std::istringstream in(text); std::string part; std::uint32_t result=0;
    for(int i=0;i<4;++i) {
        if(!std::getline(in,part,'.') || part.empty() || part.size()>3 ||
           part.find_first_not_of("0123456789")!=std::string::npos)
            throw std::runtime_error("Invalid IPv4: "+text);
        int octet=integer(part);
        if(octet>255) throw std::runtime_error("Invalid IPv4: "+text);
        result=(result<<8)|static_cast<std::uint32_t>(octet);
    }
    if(in.peek()!=std::char_traits<char>::eof() || text.back()=='.')
        throw std::runtime_error("Invalid IPv4: "+text);
    return result;
}
Config Config::load(const std::string& path) {
    std::ifstream in(path);
    if(!in) throw std::runtime_error("Cannot open config: "+path);
    Config c; std::string line; std::set<std::string> seen;
    while(std::getline(in,line)) {
        line=trim(line.substr(0,line.find('#'))); if(line.empty()) continue;
        auto eq=line.find('=');
        if(eq==std::string::npos) throw std::runtime_error("Expected key=value: "+line);
        auto k=trim(line.substr(0,eq)), v=trim(line.substr(eq+1));
        if(!seen.insert(k).second) throw std::runtime_error("Duplicate key: "+k);
        if(k=="servers") c.servers=integer(v);
        else if(k=="cycles") c.cycles=integer(v);
        else if(k=="initial_per_server") c.initialPerServer=integer(v);
        else if(k=="min_time") c.minTime=integer(v);
        else if(k=="max_time") c.maxTime=integer(v);
        else if(k=="cooldown") c.cooldown=integer(v);
        else if(k=="max_servers") c.maxServers=integer(v);
        else if(k=="burst_min") c.burstMin=integer(v);
        else if(k=="burst_max") c.burstMax=integer(v);
        else if(k=="seed") { int s=integer(v); if(s<0) throw std::runtime_error("Negative seed"); c.seed=static_cast<unsigned>(s); }
        else if(k=="arrival_probability") {
            std::size_t n=0; c.arrivalProbability=std::stod(v,&n);
            if(n!=v.size()) throw std::runtime_error("Invalid probability");
        }
        else if(k=="verbose") c.verbose=boolean(v);
        else if(k=="color") c.color=boolean(v);
        else if(k=="blocked_start") c.blockedStart=v;
        else if(k=="blocked_end") c.blockedEnd=v;
        else if(k=="log_file") c.logFile=v;
        else throw std::runtime_error("Unknown config key: "+k);
    }
    c.validate(); return c;
}
void Config::validate() const {
    if(servers<1 || maxServers<servers || maxServers>10000 || cycles<1 || cycles>10000000 ||
       initialPerServer<0 || initialPerServer>10000 || minTime<1 || maxTime<minTime ||
       maxTime>1000000 || cooldown<1 || burstMin<1 || burstMax<burstMin || burstMax>10000 ||
       !std::isfinite(arrivalProbability) || arrivalProbability<0 || arrivalProbability>1 || logFile.empty())
        throw std::runtime_error("Invalid configuration bounds");
    if(ipv4(blockedStart)>ipv4(blockedEnd)) throw std::runtime_error("Reversed blocked IP range");
}
