# Final demo preparation

Use demo-terminal-silent.mp4: the replacement is a real terminal-session
recording lasting 1 minute 55 seconds. Read DEMO-VOICEOVER.md and record your
voice. Supply the audio so it can be synchronized to the footage. Keep the
completed export below three minutes. The old five-minute slideshow is superseded.

The recording shows Request/WebServer/LoadBalancer structures, interactive input
of 20 servers and 100000 cycles, quiet results, actual assignments to multiple
servers, scale-up and scale-down events, and the high-arrival firewall scenario.
It ends with passing tests and a conclusion. The terminal cast preserves the
actual captured session and original event timestamps.

For a new live demonstration, run from the project directory:

```sh
make
./loadbalancer config.txt --interactive --quiet
# Enter 20, then 100000.
./loadbalancer demo.txt --verbose > demo-output.txt
grep 'ASSIGN server=' demo.log | head -5
grep 'SCALE' demo.log
./loadbalancer firewall-demo.txt --verbose > firewall-output.txt
grep 'BLOCK source=' firewall-demo.log
make test
```

RUBRIC-AUDIT.md maps the footage and artifacts to the actual Canvas criteria
and the TA follow-up. No final video with student narration is claimed yet.
