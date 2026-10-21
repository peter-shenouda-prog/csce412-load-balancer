# Video demonstration guide

Record your screen and your own explanation for about 3–5 minutes. This guide
is not a recorded video. Practice the commands first in a Linux/WSL terminal.

## 1. Explain the design (about 40 seconds)
Open `site/index.html`. Explain: requests pass through an IP blocker, wait in a
FIFO queue, then go to idle servers. Every simulated cycle advances processing.
The controller adds or retires servers based on queued requests per active server.
The 50/80 threshold gap and cooldown reduce rapid repeated capacity changes.

## 2. Show classes and structures (about 50 seconds)
Open Request.h, WebServer.h and LoadBalancer.h. Point out the four request fields,
the server's current request and remaining time, the queue<Request>, and the
vector<unique_ptr<WebServer>> that dynamically owns the servers. Explain that a
busy retiring server finishes before deletion. Show Config.h and config.txt.

## 3. Build, input and quiet run (about 50 seconds)
```sh
make
./loadbalancer config.txt --interactive --quiet
```
Enter `20` then `100000`. Show the start and end summaries: initial queue 2000,
time range 10–100, completed/queued/in-flight/rejected counts, active and retired
servers, additions and retirements. Explain that 20 is the starting count and
dynamic scaling intentionally changes it. Show that the accounting totals match.

## 4. Show event mode and capacity changes (about 50 seconds)
```sh
./loadbalancer demo.txt --verbose --color
grep -E 'SCALE UP|SCALE DOWN|DEALLOCATE|BLOCK' demo.log | head -25
```
Point out both SCALE UP and SCALE DOWN. If the first 25 lines contain too many
BLOCK messages, use `grep 'SCALE' demo.log`. Show ARRIVAL, ASSIGN and COMPLETE
events. Explain that the log records useful changes rather than every idle tick.
Color is optional, and the log remains readable plain text.

## 5. Finish (about 30 seconds)
```sh
make test
```
Show the passing tests, Doxygen website, and your Git repository after uploading.
Mention that job types are generated and reported; the optional bonus switch is
not implemented. End with what you learned about queues, scaling and AI-assisted
verification. Explain the code in your own words and disclose AI assistance.
