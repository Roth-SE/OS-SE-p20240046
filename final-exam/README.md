# Final Exam — <Song Phengroth>

<!-- ===== COVER SHEET — required first section. Fill EVERY line. ===== -->
```
Student name: Song Phengroth
Student ID: p20240046
Server username: se-song-phengroth 
Exam scenario value (COMPANY / PRODUCT): OrbitWorks / Beacon
Date & start time: 6/30/2026 1:00PM
AI assistant used (name/none): GEMINI
```

> Exact commands per part are in `commands.md`. Live-curveball answers are in `live_mods.md`.
> Replace every `<...>` below. Keep answers tied to **your own** scenario numbers.

---

## Part A — Threads, Kernel Mapping & Signals

**Screenshots**

![A1 — all workers + summary](partA_threads/images/a1_thread_run.png)
![A2 — signal_demo: commands + interaction (signal sent, cleanup msg, clean exit)](partA_threads/images/a2_signal_catch.png)

**Written (one short answer)**

- **Why does a worker thread's joined result reach the main thread, but a forked
  child's value would not?* Threads share the same address space, heap, and data segment within a single process, allowing a worker thread to pass a pointer directly back to the main thread via pthread_join. 
In contrast, fork() creates an entirely separate child process with an isolated copy of the parent's memory; 
therefore, changes made or values computed in the child do not affect the parent's memory and cannot be passed back without explicit Inter-Process Communication (IPC).
  <threads share one address space (joined value read from shared memory); a forked
  child runs in a copied address space, so its changes never reach the parent>

**Anything not completed:** <none / ...>

---

## Part B — Files, Permissions & Special Bits

**Screenshot**

![B1 — setuid / setgid / sticky bits + id output](partB_security/images/b1_special_bits.png)

**Written (one short answer)**

- **Translate your private file's final octal mode into the 9-char symbolic string**
  (e.g. `600` → `rw-------`).
  octal `<NNN>` → `<rwx-style>`

**Anything not completed:** <none / ...>

---

## Part C — Bash Scripting, PATH & Safe File Scanning

**Screenshot**

![C1 — collector run + consolidated report](partC_scripting/images/c1_collector_run.png)

**Written (one short answer)**

- **Why did `greeter` fail to run by name before you added your `bin` directory to
  PATH?**
  <the shell only searches directories listed in $PATH; adding ~/bin let it resolve the
  bare name `greeter`>

**Anything not completed:** <none / ...>

---

## Part D — Concurrency, a Race Condition & File Locking

**Screenshot**

![D2 — swarm lands at the correct stock after the lock](partD_secure/images/d2_patched.png)

**Written (one short answer)**

- **Why did the unpatched `swarm` sometimes leave more stock than the correct final
  value (with `<INITIAL_STOCK>` stock and `<SWARM_SIZE>` concurrent buyers)?**
  <concurrent buyers read the same stale stock (lost update), so some decrements
  overwrote each other — fewer than expected applied>

**Anything not completed:** <note here if the race was hard to reproduce — D3's lock is
what's graded>

---

## Part E — Backups, Archiving & cron Automation

**Screenshot**

![E1 — only RETAIN_N archives remain + log](partE_automation/images/e1_backup_retention.png)

**Written (one short answer)**

- **Archiving vs compression — which one actually shrank the bytes, and why?**
  <tar archives (bundles) many files into one; gzip/compression shrinks bytes —
  compression reduced the size>

**Anything not completed:** <none / ...>
