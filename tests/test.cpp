/** @file test.cpp @brief Boundary checks for server state and configuration. */
#include "WebServer.h"
#include "Config.h"
#include <cassert>
#include <functional>
#include <iostream>
bool fails(const std::function<void()>& f) { try { f(); } catch(...) { return true; } return false; }
int main() {
    assert(ipv4("0.0.0.0")==0); assert(ipv4("255.255.255.255")==0xffffffffU);
    for(auto s:{"1.2.3","1.2.3.256","1.2.3.4.","1.2.3.4.5","-1.2.3.4","a.2.3.4"})
        assert(fails([&]{ipv4(s);}));
    WebServer s(1); assert(!s.busy()); assert(!s.tick());
    Request r{"1.2.3.4","4.3.2.1",2,'S'}; s.assign(r);
    assert(fails([&]{s.assign(r);})); assert(!s.tick());
    s.retire(); auto done=s.tick(); assert(done && done->jobType=='S');
    assert(!s.busy() && s.retiring()); assert(fails([&]{s.assign(r);}));
    WebServer one(2); r.time=1; one.assign(r); assert(one.tick());
    Config c; c.validate(); c.cooldown=0; assert(fails([&]{c.validate();}));
    c=Config{}; c.blockedStart="11.0.0.0"; assert(fails([&]{c.validate();}));
    std::cout<<"Unit checks passed\n";
}
