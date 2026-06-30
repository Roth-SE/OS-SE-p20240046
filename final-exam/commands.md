# commands.md — exact commands I ran, per part

> Paste the **real** commands you ran, in order, in the fenced blocks below. Graded for
> command competency and is your defence if any output is questioned. One block per part.
> Delete the hint comments and replace with your actual commands.

## Part A — Threads, Mapping & Signals

```bash
# compile the threaded program (mind the threading flag), run it
# capture the 1:1 user→kernel (LWP) mapping into thread_map.txt while it runs
# compile/run signal_demo and demonstrate catching the interactive interrupt
# Navigate to directory
cd ~/OS-SE-p20240046/final-exam/partA_threads/

# Compile and run the threaded program (mind the threading flag)
gcc -pthread thread_demo.c -o thread_demo

# Run in background to capture the mapping
./thread_demo &

# Capture the 1:1 user→kernel (LWP) mapping into thread_map.txt while it runs
ps -eLf | grep thread_demo > ~/OS-SE-p20240046/final-exam/partA_threads/thread_map.txt

# Compile/run signal_demo and demonstrate catching the interactive interrupt
gcc signal_demo.c -o signal_demo
./signal_demo
# (Pressed Ctrl+C to trigger SIGINT)
```

## Part B — Permissions, Special Bits & ACLs

```bash
# build the tree (shared dir + private file); set octal + symbolic modes
# demonstrate setgid + sticky on a dir you own; build/set the setuid binary
# add and read back an ACL entry; save reports
# Navigate to directory
cd ~/OS-SE-p20240046/final-exam/partB_security/

# Build the tree (shared dir + private file); set octal + symbolic modes
mkdir shared_dir
touch private_file.txt
chmod 600 private_file.txt
chmod o=x shared_dir

# Save reports
ls -ld shared_dir > perm_report.txt
ls -l private_file.txt >> perm_report.txt
stat shared_dir >> perm_report.txt
stat private_file.txt >> perm_report.txt

# Demonstrate setgid + sticky on a dir you own; build/set the setuid binary
chmod g+s shared_dir
chmod +t shared_dir
gcc setuid_demo.c -o setuid_demo
chmod u+s setuid_demo

# Execute to capture evidence
ls -ld shared_dir
ls -l setuid_demo
./setuid_demo
```

## Part C — Bash Scripting, PATH & Safe Scanning

```bash
# make greeter runnable by name via PATH; record PATH + resolved location
# run collector over your dirs; show it skips unreadable/missing files safely
# Navigate to directory
cd ~/OS-SE-p20240046/final-exam/partC_scripting
mkdir -p scripts

# Make greeter runnable by name via PATH; record PATH + resolved location
mkdir -p ~/bin
nano ~/bin/greeter
chmod +x ~/bin/greeter
export PATH="$HOME/bin:$PATH"
greeter
echo $PATH > path_report.txt
which greeter >> path_report.txt
cp ~/bin/greeter scripts/

# Setup test directories for collector
mkdir -p scan_dir_1 scan_dir_2
echo "Beacon system config ok" > scan_dir_1/config.txt
echo "Launch telemetry nominal" > scan_dir_2/data.txt
touch scan_dir_1/secret_keys
chmod 000 scan_dir_1/secret_keys

# Run collector over your dirs; show it skips unreadable/missing files safely
nano ~/bin/collector
chmod +x ~/bin/collector
collector
cp ~/bin/collector scripts/

# Restored permissions to allow git commit
chmod 600 scan_dir_1/secret_keys
```

## Part D — Race Condition & flock

```bash
# init stock; run swarm several times unpatched and record final stock each time
# add the exclusive advisory lock around the read-modify-write; re-run swarm
# Navigate to directory
mkdir -p ~/OS-SE-p20240046/final-exam/partD_secure/scripts
cd ~/OS-SE-p20240046/final-exam/partD_secure

# Init scripts
nano ~/bin/buy_beacon
nano ~/bin/swarm
chmod +x ~/bin/buy_beacon ~/bin/swarm

# Run swarm several times unpatched and record final stock each time
swarm
swarm
swarm
echo "Run 1: 199, Run 2: 199, Run 3: 199. Unpatched runs failed to reach 150." > observations.txt

# Add the exclusive advisory lock around the read-modify-write; re-run swarm
nano ~/bin/buy_beacon
swarm

# Copy deliverables
cp ~/bin/buy_beacon scripts/
cp ~/bin/swarm scripts/
```

## Part E — Backups & cron

```bash
# E1: run backup_project enough times that pruning happens (keep newest RETAIN_N)
# E2: per-user crontab, two entries (absolute paths):
#     recurring (CRON_INTERVAL) -> partE_automation/logs/cron_recurring.log
#     one-shot at TIMED        -> partE_automation/logs/cron_oneshot.log
# E3: backup_exam -> tar the final-exam folder to ~/exam-backups/final-exam-<ts>.tar.gz
#     crontab: run backup_exam on a short interval AND once at exactly 16:00 today
#     then: ls ~/exam-backups
# capture crontab -l + both logs + the ~/exam-backups listing into cron_report.txt
# Setup directories
mkdir -p ~/OS-SE-p20240046/final-exam/partE_automation/{scripts,logs,sample_project,project_backups}
cd ~/OS-SE-p20240046/final-exam/partE_automation
echo "dummy code" > sample_project/main.c
echo "dummy config" > sample_project/config.txt

# E1: run backup_project enough times that pruning happens
nano ~/bin/backup_project
chmod +x ~/bin/backup_project
backup_project; sleep 1; backup_project; sleep 1; backup_project; sleep 1; backup_project; sleep 1; backup_project
cp ~/bin/backup_project scripts/

# E2 & E3 Script Setup
nano ~/bin/timed_job
nano ~/bin/backup_exam
chmod +x ~/bin/timed_job ~/bin/backup_exam
cp ~/bin/timed_job scripts/
cp ~/bin/backup_exam scripts/
mkdir -p ~/exam-backups

# Edit per-user crontab
crontab -e

# Capture crontab -l + both logs + the ~/exam-backups listing into cron_report.txt
echo "--- Crontab Configuration ---" > cron_report.txt
crontab -l >> cron_report.txt
echo -e "\n--- Recurring Log (E2) ---" >> cron_report.txt
cat logs/cron_recurring.log >> cron_report.txt
echo -e "\n--- One-Shot Log (14:35) (E2) ---" >> cron_report.txt
cat logs/cron_oneshot.log >> cron_report.txt
echo -e "\n--- Exam Backups Directory (E3) ---" >> cron_report.txt
ls -l ~/exam-backups >> cron_report.txt
```
