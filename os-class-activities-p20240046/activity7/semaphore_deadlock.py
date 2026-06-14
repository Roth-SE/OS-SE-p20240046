import threading
import time

# Each process is a list of operations:
#   ('w', s) = wait(s)    ('s', s) = signal(s)    ('p', d) = print digit d
def run_case(title, inits, programs, timeout=2.0):
    sems = {name: threading.Semaphore(v) for name, v in inits.items()}
    finished = set()

    def worker(name, ops):
        for kind, arg in ops:
            if kind == 'w':
                sems[arg].acquire()
                time.sleep(0.02)        # widen the window so races actually occur
            elif kind == 's':
                sems[arg].release()
            elif kind == 'p':
                pass                    # this is where the process would print(arg)
        finished.add(name)

    threads = [threading.Thread(target=worker, args=(n, ops), name=n, daemon=True)
               for n, ops in programs.items()]
    for t in threads:
        t.start()
    for t in threads:
        t.join(timeout)

    if len(finished) == len(programs):
        print(f"{title}: NO deadlock — all finished {sorted(finished)}")
    else:
        stuck = sorted(t.name for t in threads if t.is_alive())
        print(f"{title}: DEADLOCK — stuck: {stuck}; finished: {sorted(finished)}")

# P3 differs between the cases; P1 and P2 are the same throughout.
P1 = [('w','s1'),('w','s2'),('p','1'),('s','s2'),('s','s1')]
P2 = [('w','s2'),('w','s3'),('p','2'),('s','s3'),('s','s2')]
P3_case1 = [('w','s1'),('w','s2'),('w','s3'),('p','3'),('s','s3'),('s','s2'),('s','s1')]
P3_case2 = [('w','s2'),('w','s3'),('w','s1'),('p','3'),('s','s1'),('s','s3'),('s','s2')]

run_case("Case 1 (s1=s2=s3=1)",
         {'s1':1,'s2':1,'s3':1},
         {'P1':P1, 'P2':P2, 'P3':P3_case1})

run_case("Case 2 (s1=s2=s3=1)",
         {'s1':1,'s2':1,'s3':1},
         {'P1':P1, 'P2':P2, 'P3':P3_case2})

run_case("Case 3 (s1=2)",
         {'s1':2,'s2':1,'s3':1},
         {'P1':P1, 'P2':P2, 'P3':P3_case2})