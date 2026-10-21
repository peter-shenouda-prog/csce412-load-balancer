# Run the finished project on this laptop

Open Ubuntu/WSL, then paste these commands:

```sh
cd /mnt/c/Users/peter/Documents/Codex/2026-10-19/institution-logo-ps-bettercampus-dashboard-courses/outputs/load-balancer
make
./loadbalancer config.txt --interactive --quiet
```

Enter `20` for starting servers and `100000` for cycles. The summaries print to
the terminal and are written to simulation.log. This is the required run.

For the shorter event demonstration:

```sh
./loadbalancer demo.txt --verbose --color
grep 'SCALE' demo.log
```

For verification:

```sh
make test
```

The actual reference output is required-run.txt. Follow DEMO-GUIDE.md to record
your video. SUBMISSION-CHECKLIST.md contains the live links and upload filenames.
