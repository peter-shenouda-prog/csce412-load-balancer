# Relay | Load Balancer

**Peter Shenouda — CSCE 412 — Project 4**

This C++17 discrete-clock simulation models a FIFO request queue served by a
dynamically sized collection of web servers. Each Request contains source and
destination IPv4 addresses, a positive duration in cycles, and a P/S job category.
RequestGenerator uses a seeded random engine for request fields and arrival bursts.

LoadBalancer owns the queue and WebServer objects using unique pointers. It checks
the source IP against an inclusive configured firewall range before enqueueing.
Every cycle it generates arrivals, considers scaling, assigns queued work to idle
servers, advances processing, and frees retired servers after they finish.

Scaling adds one server above 80 queued requests per active server and retires one
below 50 per active server. Changes are separated by a configurable cooldown;
at least one server stays active. A safety limit caps allocated servers. Busy
retiring servers drain existing work and do not accept another request.

Quiet mode emits starting and ending summaries. Verbose mode additionally records
arrivals, blocks, assignments, completions and scaling events, without idle-cycle
messages. Console colors are optional and logs remain plain text.

Build with `make`; run `./loadbalancer config.txt --quiet` for the required
20-server, 100000-cycle starting configuration. Use `--interactive` to enter the
starting server count and simulation duration. The simulation stops exactly at
the configured duration and reports queued and unfinished work rather than
silently draining it afterward.

The standard implementation handles both job categories in one pool. The optional
bonus switch and concurrent specialized load balancers are not implemented.
See the [design page](../../index.html) for workflow and design rationale.
