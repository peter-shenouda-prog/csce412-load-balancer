# Relay | Load Balancer
Peter Shenouda · CSCE 412 · Project 4

A C++17 discrete-clock simulator with random request arrivals, a FIFO queue,
dynamic web servers, inclusive source-IP blocking, and event/summary logs.

## Build and run
Use Linux, macOS, WSL Ubuntu, or a university Linux environment with g++, GNU
Make, and Python 3 (Python is needed only for integration tests).

```sh
make
./loadbalancer config.txt --quiet
./loadbalancer config.txt --interactive --quiet
./loadbalancer demo.txt --verbose --color
make test
make docs
```

`--interactive` prompts for starting server count and clock cycles. A configuration
file is still loaded for the other settings. `--help` prints usage. Run commands
from this directory. Quiet and verbose options override the file; the last mode
option wins. Color affects the console only. Output also goes to `log_file`.
Running again overwrites that log. The required reference run is saved separately
in `required-run.txt`; it was generated from the actual C++ binary on Ubuntu.

## Files
- `Request.h`: the four request fields.
- `Config.h/.cpp`: strict key=value parsing and IPv4 range validation.
- `RequestGenerator.h/.cpp`: random IPs, times, job types and arrival bursts.
- `WebServer.h/.cpp`: idle/busy state, exact-cycle processing, safe retirement.
- `LoadBalancer.h/.cpp`: request queue, firewall, server ownership, scaling, logs.
- `main.cpp`: configuration, command-line options and interactive driver.
- `Makefile`: build, test, documentation and clean targets.
- `config.txt`: required 20-server, 100000-cycle quiet run.
- `demo.txt`: 20000-cycle event demo, showing growth and retirement.
- `site/index.html`: branded design, diagram and explanation.
- `site/docs/html/index.html`: generated Doxygen documentation (in website package).

## Configuration guide
Every setting can be changed without editing C++. Unknown/duplicate keys and
invalid values cause an error. Full-line and trailing `#` comments are supported.

| Setting | Meaning |
|---|---|
| servers / cycles | Starting server count / exact simulation duration |
| initial_per_server | Initial accepted queue multiplier, normally 100 |
| min_time / max_time | Inclusive task duration range, positive integers |
| cooldown | Minimum cycles between scaling decisions that change capacity or hit its cap |
| max_servers | Safety cap on allocated servers, including draining servers |
| arrival_probability | Probability of a new burst each cycle, from 0 to 1 |
| burst_min / burst_max | Inclusive request count per arriving burst |
| seed | Seed for repeatable random generation |
| blocked_start / blocked_end | Inclusive IPv4 source range to reject |
| verbose / color | true or false |
| log_file | Writable destination for plain-text logs |

Bounds protect accidental input: at most 10000 servers, 10000000 cycles,
10000 initial requests per starting server, 10000 requests per burst, and
1000000 cycles per task. These are safety bounds, not performance guarantees.
The initial queue is refilled after firewall rejection to reach servers ×
initial_per_server accepted requests. A finite attempt limit reports a firewall
configuration that makes initialization impossible or impractical.

## Timing and design
At cycle 0 the initial queue exists and every server is idle. Each numbered cycle:
1. Sample an arrival burst and filter every incoming request by source IP.
2. If the cooldown has elapsed, compare queued requests with active server count.
3. Add one server above 80 × active servers, or retire one below 50 × active servers.
4. Assign FIFO requests to idle non-retiring servers.
5. Advance every busy server by one cycle and count any completions.
6. Free retired servers once idle, then check accounting and sample queue size.

A time-1 request assigned during a cycle finishes at that cycle's end. A server
does not take a second request in the same cycle. The first scaling check occurs
after one cooldown. Values equal to 50 or 80 times active servers cause no scaling.
At least one server remains active. Idle servers retire first; a busy retiring
server finishes its request before its unique_ptr allocation is deleted. No
accepted requests are dropped. The desired queue band is a control goal; cooldowns,
burst arrivals and the server cap can temporarily keep the queue outside it.

The run stops exactly at the requested duration. It does not finish queued work
afterward. End summaries distinguish queued requests from in-flight requests.
Deallocated servers are historical inactive servers, not a reusable idle pool.
Both job types share the standard pool; specialized concurrent load balancers
and a switch are optional bonus work and are not included.

## Statistics and verification
Each cycle enforces:
`generated = rejected + completed + queued + in-flight`,
`assigned = completed + in-flight`, and
`completed = completed processing + completed streaming`.

Utilization counts busy allocated-server cycles divided by all allocated-server
cycles, including draining servers. Average queue samples the end of each cycle;
peak queue also includes arrivals before dispatch. Seeded results repeat on the
same C++ standard library; different libraries may implement distributions
differently. A displayed 100.00% utilization can be rounded.

Unit tests cover processing timing, assignment guards, retirement and IPv4
validation. Integration tests execute the C++ binary for required-run accounting,
determinism, quiet/verbose modes, scaling both ways, minimum/maximum capacity,
total firewall rejection, and invalid settings.

## Documentation
Install Doxygen if needed, then run `make docs`. The generated main page describes
the project and links to the design. Headers and implementation files contain
Doxygen file comments; public types and methods describe their contracts. The
prepared website package already contains real Doxygen-generated output.

## AI assistance
The initial Request structure and the completed implementation, documentation,
tests and packaging were produced with ChatGPT/Codex assistance at the student's
request, based on the student's report that the instructor permitted full AI
assistance. The implementation and test results can be inspected in this repository.

## Video walkthrough

The hosted video uses actual source and captured C++ program output with
synthesized narration, rather than a live terminal screen recording.
See VIDEO-TRANSCRIPT.md or DEMO-GUIDE.md for a student-recorded alternative.

https://people.tamu.edu/~peter.shenouda/project4/final/project-demo.mp4
