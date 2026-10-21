/** @file LoadBalancer.h @brief FIFO scheduling, firewall, scaling and statistics. */
#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H
#include "Config.h"
#include "RequestGenerator.h"
#include "WebServer.h"
#include <queue>
#include <vector>
#include <memory>
#include <fstream>
/** @brief Independent simulation instance owning its queue, servers, RNG and log. */
class LoadBalancer {
    Config config;
    RequestGenerator generator;
    std::queue<Request> requests;
    std::vector<std::unique_ptr<WebServer>> servers;
    std::ofstream log;
    std::uint32_t blockedLow, blockedHigh;
    long long generated=0, rejected=0, assigned=0, completed=0, processing=0, streaming=0;
    long long additions=0, removals=0, deallocated=0, arrivalEvents=0, arrivingRequests=0;
    long long busyCycles=0, availableCycles=0, queueTotal=0, capacityLimited=0;
    std::size_t queuePeak=0;
    int nextId=1, cycle=0, lastScale=0, minServers=0, peakServers=0;
    int activeCount() const;
    int busyCount() const;
    void emit(const std::string& text, const std::string& color="");
    void event(const std::string& text, const std::string& color="");
    void accept(const Request& request);
    void scale();
    void removeDrained();
    void summary(const std::string& phase);
    void checkConservation() const;
public:
    /** @brief Initialize servers, firewall, logger and full accepted queue.
     * @param c Validated simulation settings. @throws std::runtime_error Invalid config/log path. */
    explicit LoadBalancer(const Config& c);
    /** @brief Run exactly the configured cycles, then write final statistics.
     * @throws std::logic_error Request accounting is inconsistent. */
    void run();
};
#endif
