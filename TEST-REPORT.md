# Verification report

- Built on Ubuntu with g++ and C++17, -Wall -Wextra -Wpedantic: passed with no warnings.
- make test: server/configuration unit checks and binary integration checks passed.
- Required run: 20 starting servers, 100000 cycles, initial queue 2000, final queue
  1528, task times 10–100, 35831 completions, 20 in flight, 169 rejections.
- Capacity changes: 7 additions, 6 retirements, active count range 19–25.
- Request conservation assertions passed on every cycle, not just at the end.
- Deterministic repeat run matched on the same C++ standard library.
- Quiet output matched its plain-text log; verbose output showed arrivals,
  assignments, completions, IP blocks, additions, retirements and deallocations.
- Empty input, minimum server count, capacity cap, total source blocking, and
  malformed settings were checked by integration tests.
- AddressSanitizer + UndefinedBehaviorSanitizer full required run passed with
  ASAN_OPTIONS=detect_leaks=0. The initial default sanitizer launch crashed in WSL
  without a diagnostic; the rerun with leak detection disabled completed. Leak
  detection is therefore not claimed as verified.
- Doxygen 1.9.1 generated HTML with no warnings.

The prepared reference log is from the actual compiled C++ program. No simulated
results were fabricated. Cross-library random distributions may differ.

## October 25 follow-up verification

The build and make test passed again. Quiet and verbose required-run summaries
matched exactly. All 169 blocks, 7 additions and 6 retirements were retained in
the concise execution-evidence log. The separate 50-cycle, 1000-arrival firewall
scenario rejected 3 matching 10.* source addresses and accepted 997 others.
The 1:55 PTY recording was rendered to H.264 MP4 and visually checked at the
input, results, scaling and rejection sections. Narration remains pending.
