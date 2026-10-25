# Voice-over for the 1 minute 55 second terminal recording

Read only the quoted text. The times refer to demo-terminal-silent.mp4. Leave
the indicated pauses; we can adjust the timing when your audio is available.

## 0:00–0:20 — Introduction and structures

“Hi, I'm Peter Shenouda. This is Relay, my load balancer simulation. It filters
requests, queues accepted work, and distributes it across web servers. Request
stores the two IP addresses, processing time, and job type. Each WebServer handles
one request at a time.”

**Pause until 0:20.**

## 0:20–0:32 — Architecture and scaling

“LoadBalancer owns the FIFO queue and dynamically allocated servers. Above eighty
queued requests per server, it adds one. Below fifty, it retires one. A cooldown
separates changes, and busy retiring servers finish their work.”

**Pause until 0:32.**

## 0:32–1:00 — User input, execution and results

“I'm running quiet mode with twenty starting servers and one hundred thousand
clock cycles. The starting queue is two thousand requests, with task times from
ten to one hundred cycles. The run completes thirty-five thousand eight hundred
thirty-one requests, with fifteen hundred twenty-eight queued and twenty still
processing. It records seven additions, six retirements, and one hundred
sixty-nine rejected requests.”

**Pause until 1:00.**

## 1:00–1:23 — Actual distribution and scaling

“This verbose run shows requests assigned to different server IDs. The large
initial queue stresses capacity and triggers scaling up. As the queue shrinks,
the controller scales down. These are actual events from the program's log.”

**Pause until 1:23.**

## 1:23–1:43 — Rejection scenario

“This high-arrival scenario blocks source addresses in the ten-dot range.
The program rejects these three matching requests while accepting other traffic.
The summary confirms one thousand generated, nine hundred ninety-seven accepted,
and three rejected. Blocked requests never enter the processing queue.”

**Pause until 1:43.**

## 1:43–1:55 — Verification and conclusion

“The tests pass, and request accounting is checked every cycle. Relay demonstrates
request distribution, dynamic scaling, and IP filtering. The code, logs and
documentation are prepared for submission. Thank you.”
