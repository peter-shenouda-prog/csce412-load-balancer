/** @file RequestGenerator.cpp @brief Random request and arrival implementation. */
#include "RequestGenerator.h"
RequestGenerator::RequestGenerator(const Config& c):engine(c.seed),config(c) {}
std::string RequestGenerator::address() {
    std::uniform_int_distribution<int> octet(0,255);
    std::string result;
    for(int i=0;i<4;++i) { if(i) result+='.'; result+=std::to_string(octet(engine)); }
    return result;
}
Request RequestGenerator::next() {
    std::uniform_int_distribution<int> duration(config.minTime,config.maxTime), type(0,1);
    return {address(),address(),duration(engine),type(engine)==0?'P':'S'};
}
bool RequestGenerator::arrives() { return std::bernoulli_distribution(config.arrivalProbability)(engine); }
int RequestGenerator::burst() { return std::uniform_int_distribution<int>(config.burstMin,config.burstMax)(engine); }
