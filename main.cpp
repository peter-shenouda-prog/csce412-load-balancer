/** @file main.cpp @brief Command-line and interactive simulation driver. */
#include "LoadBalancer.h"
#include <iostream>
#include <stdexcept>
#include <string>
/** @brief Parse settings and run the simulation.
 * @param argc Argument count. @param argv Options: config path, --quiet, --verbose, --color, --interactive.
 * @return Zero on success, one on invalid input or runtime failure. */
int main(int argc,char* argv[]) {
    try {
        std::string path="config.txt"; bool interactive=false;
        for(int i=1;i<argc;++i) {
            std::string arg=argv[i];
            if(arg=="--help") {
                std::cout<<"Usage: loadbalancer [config.txt] [--quiet|--verbose] [--color] [--interactive]\n";
                return 0;
            }
            if(arg=="--interactive") interactive=true;
            else if(arg=="--quiet" || arg=="--verbose" || arg=="--color") {}
            else if(arg.rfind("--",0)==0) throw std::runtime_error("Unknown option: "+arg);
            else path=arg;
        }
        Config c=Config::load(path);
        for(int i=1;i<argc;++i) {
            std::string arg=argv[i];
            if(arg=="--quiet") c.verbose=false;
            if(arg=="--verbose") c.verbose=true;
            if(arg=="--color") c.color=true;
        }
        if(interactive) {
            std::string line; std::size_t used=0;
            std::cout<<"Starting servers: "; if(!std::getline(std::cin,line)) throw std::runtime_error("Missing server input");
            c.servers=std::stoi(line,&used); if(used!=line.size()) throw std::runtime_error("Invalid server input");
            std::cout<<"Clock cycles: "; if(!std::getline(std::cin,line)) throw std::runtime_error("Missing cycle input");
            c.cycles=std::stoi(line,&used); if(used!=line.size()) throw std::runtime_error("Invalid cycle input");
        }
        c.validate(); LoadBalancer balancer(c); balancer.run(); return 0;
    } catch(const std::exception& e) { std::cerr<<"Error: "<<e.what()<<'\n'; return 1; }
}
