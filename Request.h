/** @file Request.h @brief Data carried by one simulated request. */
#ifndef REQUEST_H
#define REQUEST_H
#include <string>
/** @brief A request with source/destination IPv4 text, duration and job category. */
struct Request {
    std::string ipIn; ///< Requester's IPv4 address.
    std::string ipOut; ///< Result destination's IPv4 address.
    int time = 1; ///< Positive processing duration in simulated cycles.
    char jobType = 'P'; ///< P: processing; S: streaming.
};
#endif
