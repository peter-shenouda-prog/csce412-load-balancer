/** @file RequestGenerator.h @brief Seeded random traffic generation. */
#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H
#include "Request.h"
#include "Config.h"
#include <random>
/** @brief Owns the random engine used for request fields and arrivals. */
class RequestGenerator {
    std::mt19937 engine;
    Config config;
    std::string address();
public:
    /** @brief Initialize repeatable random traffic. @param c Settings including seed. */
    explicit RequestGenerator(const Config& c);
    /** @brief Generate both IPs, duration and type. @return New random request. */
    Request next();
    /** @brief Sample whether an arrival burst occurs. @return True on arrival. */
    bool arrives();
    /** @brief Sample burst size. @return Count within configured inclusive bounds. */
    int burst();
};
#endif
