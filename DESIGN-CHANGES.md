# Implementation notes relative to the pre-deliverable

The original Relay design was found on the connected university web drive after
implementation began. It is preserved in its original folder. The final design
page describes the actual implementation, with the following simplifications:

- The FIFO uses std::queue<Request> directly instead of a RequestQueue wrapper.
- Request stores IPv4 addresses as strings; firewall comparisons parse them into
  uint32_t values. One inclusive blocked range is configured instead of a list.
- Firewall and logging methods belong to LoadBalancer rather than separate
  IpBlocker and Logger classes. Class and method names differ from the proposal.
- The loop samples arrivals, scales, dispatches, then advances processing. A new
  time-1 request finishes at the end of its assignment cycle. The proposal advanced
  existing work before dispatch and began new processing on the next iteration.
- The first scaling decision occurs after one cooldown. Further checks occur
  after cooldown; multiple draining retirements are allowed if cooldown permits.
  A retiring server never takes new work and is freed only after finishing.
- Defaults were tuned to arrival probability 0.12, bursts 1–5, task times 10–100,
  and cooldown 100. These values produced both growth and retirement in the
  required run. They remain configurable. A maximum allocation cap was added.

The core plan remains: random requests, source-IP filtering, FIFO dispatch,
dynamically allocated servers, cooldown-based 50/80 scaling and event/summary logs.
The optional bonus switch is not implemented.
