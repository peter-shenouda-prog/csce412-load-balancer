# Project 4 deliverable review — October 25, 2026

Reviewed the six Canvas assignment pages individually through the signed-in
browser and compared them with the TA follow-up supplied in this chat. This is
an evidence check, not a prediction or guarantee of a grade.

| Deliverable | Canvas requirements | Evidence and status |
|---|---|---|
| Pre-deliverable (10 points) | Diagram/UML, explanation, HTML/CSS, people.tamu.edu, branding | Canvas shows the October 18 submission. Original Relay design preserved at /project4/. Implementation changes are recorded separately. No separate rubric displayed. |
| Log (80 points) | Starting/ending queue and task range (30); dynamic/random execution information (30); end status (20) | required-run.txt is the actual 20-starting-server/100000-cycle quiet run. load-balancer-log.txt adds all 169 rejection events, 7 additions, 6 retirements, and deallocations, with initial/final summaries. Assignment/completion chatter omitted. End queue 1528, completed 35831, in-flight 20, active servers 21. |
| Documentation (100 points) | Request/WebServer/LoadBalancer with good descriptions (80); people.tamu.edu hosting (10); descriptive main page (10) | Live Doxygen main page opened successfully and links all required classes. Header contracts, file comments, algorithm and ownership descriptions are included. |
| Repository (20 points) | Git repository URL | Repository made public at the user's request. Descriptive commit history is available. No separate rubric displayed. |
| Code ZIP (50 points) | Makefile and necessary .cpp/.h files; no executables | source-code.zip contains 6 .cpp files including the test driver, 5 .h files, Makefile, settings and project notes. Core source compiled and tests passed. Archive excludes executables, objects and generated dependency files. No separate rubric displayed. |
| Video (80 points) | Design choice (20); classes/structures (20); input/execution/status (20); dynamic server count (20) | Replacement is a real timed terminal-session recording, rendered as a 1:55 silent MP4 for the student's voice-over. Shows code structures, interactive 20/100000 input, results, server assignments, additions/retirements and a separate IP rejection scenario. Audio must still be supplied and synchronized before final submission. |

## TA follow-up checks

- **Concise log:** all scaling/blocking/deallocation events retained; routine
  per-request chatter removed. Required quiet log supplied separately.
- **Intro / initial configuration / end status:** clearly labeled in the log.
- **Actual operation:** terminal recording executes the compiled binary and
  displays genuine program output; it is not the old slide-based walkthrough.
- **Normal and stressed conditions:** the required run and high initial backlog
  show distribution and scaling in both directions; the firewall scenario uses
  20 arrivals per cycle, starting with two servers.
- **Special scenario:** firewall-demo.txt generates 1000 requests in 50 cycles;
  997 are accepted and 3 source addresses inside 10.0.0.0–10.255.255.255 are rejected.
- **Results:** actual quiet/verbose end summaries were compared and matched.
- **Duration:** silent replacement is 1:55; final narrated export must stay below 3:00.
- **Introduction and conclusion:** included in DEMO-VOICEOVER.md; still require
  the student's actual voice recording.

## Canvas sources reviewed

- [Log](https://canvas.tamu.edu/courses/473182/assignments/3244827)
- [Video](https://canvas.tamu.edu/courses/473182/assignments/3244826)
- [Documentation](https://canvas.tamu.edu/courses/473182/assignments/3244824)
- [Code ZIP](https://canvas.tamu.edu/courses/473182/assignments/3244823)
- [Repository](https://canvas.tamu.edu/courses/473182/assignments/3244825)
- [Pre-deliverable](https://canvas.tamu.edu/courses/473182/assignments/3244828)

The five final assignment pages showed no submission during this review; the
pre-deliverable showed Submitted. No assignments were submitted by this assistant.
The optional bonus switch/concurrent specialized pools are not implemented.
The old 5:11 synthesized slideshow is superseded and should not be submitted.
