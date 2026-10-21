/** @file Config.h @brief Configuration values and strict file loading. */
#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <cstdint>
/** @brief Simulation settings loaded from a key=value file. */
struct Config {
    int servers=20, cycles=100000, initialPerServer=100;
    int minTime=10, maxTime=100, cooldown=100, maxServers=200;
    int burstMin=1, burstMax=5;
    double arrivalProbability=0.12;
    unsigned seed=412;
    bool verbose=false, color=false;
    std::string blockedStart="10.0.0.0", blockedEnd="10.255.255.255";
    std::string logFile="simulation.log";
    /** @brief Load a configuration file; reject unknown or malformed entries.
     * @param path File to load. @return Parsed, validated settings.
     * @throws std::runtime_error Invalid file or configuration. */
    static Config load(const std::string& path);
    /** @brief Reject inconsistent settings. @throws std::runtime_error Invalid settings. */
    void validate() const;
};
/** @brief Convert dotted IPv4 to its unsigned numeric value.
 * @param text IPv4 string. @return Numeric address. @throws std::runtime_error Malformed IPv4. */
std::uint32_t ipv4(const std::string& text);
#endif
