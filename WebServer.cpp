/** @file WebServer.cpp @brief Request countdown and graceful retirement. */
#include "WebServer.h"
#include <stdexcept>
WebServer::WebServer(int id):identifier(id) {}
void WebServer::assign(const Request& request) {
    if(current || draining || request.time<1 || (request.jobType!='P' && request.jobType!='S'))
        throw std::logic_error("Invalid server assignment");
    current=request; remaining=request.time;
}
std::optional<Request> WebServer::tick() {
    if(!current) return std::nullopt;
    if(--remaining==0) { auto done=current; current.reset(); return done; }
    return std::nullopt;
}
void WebServer::retire() { draining=true; }
bool WebServer::busy() const { return current.has_value(); }
bool WebServer::retiring() const { return draining; }
int WebServer::id() const { return identifier; }
