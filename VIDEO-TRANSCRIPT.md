# Relay — demonstration transcript

Synthesized narration over actual source and captured program output. This is a narrated walkthrough, not a live terminal screen recording.

## The design

This is Relay, a C plus plus load balancing simulation for C S C E four twelve. The diagram shows the main design. Random requests first pass through a source I P address filter. Accepted requests enter a first in, first out queue. Idle web servers take the oldest waiting request and process it for a specified number of clock cycles. A controller adds or retires servers as the queue changes. This walkthrough uses the actual source and captured output from the compiled program, with synthesized narration.

## Requests and servers

The Request structure stores the requester address, result destination address, processing duration, and job category. P means processing, and S means streaming. The web server holds an optional current request, a remaining time counter, and a retirement flag. Assign accepts work only when the server is idle and not retiring. Tick advances processing by one cycle and returns a completed request when the counter reaches zero. Retire stops new assignments while allowing current work to finish.

## Ownership and configuration

The load balancer owns a standard queue of requests and a vector of unique pointers to web servers. Creating a server allocates an object, and erasing its unique pointer releases it automatically. Configuration is separate from source code. These are the settings for the required run: twenty initial servers, one hundred thousand clock cycles, task times from ten to one hundred cycles, and a one hundred cycle scaling cooldown. The random seed makes runs repeatable on the same standard library. Arrival probability and burst size are configurable too.

## Input and execution

The program was built with the Makefile and run in Ubuntu. Interactive mode asks for the starting server count and the number of clock cycles. For this captured run, the supplied inputs were twenty and one hundred thousand. The starting summary reports two thousand queued requests, because the default initial multiplier is one hundred requests per server. The initial queue is filled with accepted requests after applying the firewall. Quiet mode prints only the beginning and ending summaries, and also saves them to the configured plain text log.

## The required run

Here is the actual ending summary. The run stops at exactly one hundred thousand cycles. It completes thirty five thousand eight hundred thirty one requests, with one thousand five hundred twenty eight still queued and twenty in progress. One hundred sixty nine requests were rejected by the source address filter. Twenty one servers are accepting work at the end. Starting with twenty servers does not mean keeping twenty throughout the run: dynamic scaling is an essential part of this project. The conservation check passes, accounting for every generated request.

## Dynamic server changes

Verbose mode records meaningful events instead of printing every idle clock tick. These lines come from the shorter demonstration log. They show the controller adding capacity while the queue is large, then retiring servers as the queue shrinks. Above eighty queued requests per active server, one server is added. Below fifty, one is retired, with at least one server kept active. The controller waits for the configured cooldown between changes. If the chosen server is busy, it finishes its current request before deallocation, so accepted work is not lost.

## Firewall and event visibility

The firewall compares numeric I P v four addresses against an inclusive configured range. It applies the same check to the initial queue and later arrivals. Matching requests are counted as rejected and never enter the queue. Verbose output also records arrival bursts, assignments, completions, and physical server deallocations. Optional colors distinguish events in the console, while the saved log remains plain text. Processing and streaming requests share the standard server pool; the optional bonus switch and specialized concurrent pools are not implemented.

## Verification and deliverables

The project includes server and configuration unit checks, plus integration tests that execute the real C plus plus binary. The tests cover quiet and verbose output, repeatability, scaling in both directions, the minimum and maximum server limits, blocked traffic, and invalid settings. Every cycle also checks request conservation. The required run passed address and undefined behavior checks with leak detection disabled in this W S L environment. The source archive was extracted into a fresh folder and built and tested successfully. Deliverables include the code and Makefile, actual run log, live documentation website, Git repository, and this narrated walkthrough.