/** @file WebServer.h @brief One dynamically owned simulated web server. */
#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include "Request.h"
#include <optional>
/** @brief Processes at most one request; retirement drains current work safely. */
class WebServer {
    int identifier, remaining=0;
    bool draining=false;
    std::optional<Request> current;
public:
    /** @brief Construct an idle server. @param id Unique server identifier. */
    explicit WebServer(int id);
    /** @brief Assign work to an idle, non-retiring server.
     * @param request Work item. @throws std::logic_error Server unavailable or request invalid. */
    void assign(const Request& request);
    /** @brief Advance one clock cycle. @return Completed request, or empty if none completed. */
    std::optional<Request> tick();
    /** @brief Stop accepting new work; finish current work before deletion. */
    void retire();
    /** @brief Whether this server is processing. @return Busy status. */
    bool busy() const;
    /** @brief Whether retirement has been requested. @return Retirement status. */
    bool retiring() const;
    /** @brief Read the stable server ID. @return Identifier. */
    int id() const;
};
#endif
