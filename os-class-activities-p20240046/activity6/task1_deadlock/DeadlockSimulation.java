import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {
    // Tracks whether each thread is currently waiting for a second lock
    static volatile String thread1WaitingFor = null;
    static volatile String thread2WaitingFor = null;
    static AtomicBoolean transferCompleted = new AtomicBoolean(false);

    static void transfer(Account from, Account to, int amount) {
        String tName = Thread.currentThread().getName();
        try {
            System.out.println("[" + tName + "] Attempting to lock FROM: " + from.name);
            from.lock.acquire();
            System.out.println("[" + tName + "] LOCKED: " + from.name + "  (holds this lock)");

            // Brief sleep so the other thread has time to grab its source lock
            Thread.sleep(150);

            // Record that this thread is now waiting for the second lock
            if (tName.equals("Thread-1")) {
                thread1WaitingFor = to.name;
            } else {
                thread2WaitingFor = to.name;
            }

            System.out.println("[" + tName + "] Trying to lock TO: " + to.name + "  <-- will block if deadlock");
            to.lock.acquire();
            System.out.println("[" + tName + "] LOCKED: " + to.name);

            // Critical section
            from.balance -= amount;
            to.balance += amount;
            System.out.println("[" + tName + "] Transfer of $" + amount
                    + " from " + from.name + " to " + to.name + " COMPLETED");
            transferCompleted.set(true);

            to.lock.release();
            from.lock.release();

            // Clear waiting flag
            if (tName.equals("Thread-1")) thread1WaitingFor = null;
            else thread2WaitingFor = null;

        } catch (InterruptedException e) {
            System.out.println("[" + tName + "] Interrupted while waiting.");
            Thread.currentThread().interrupt();
        }
    }
}

public class DeadlockSimulation {
    public static void main(String[] args) throws InterruptedException {

        Account accountA = new Account("Account-A", 1000);
        Account accountB = new Account("Account-B", 1000);

        System.out.println("=== Bank Transfer Deadlock Simulation ===");
        System.out.println("Starting Balance - Account-A: $" + accountA.balance
                + "  |  Account-B: $" + accountB.balance);
        System.out.println("Starting Total: $" + (accountA.balance + accountB.balance));
        System.out.println("-----------------------------------------");
        System.out.println("Transaction 1 (Thread-1): Transfer $100 from Account-A -> Account-B");
        System.out.println("Transaction 2 (Thread-2): Transfer $200 from Account-B -> Account-A");
        System.out.println("-----------------------------------------\n");

        Thread t1 = new Thread(() ->
                Transfer.transfer(accountA, accountB, 100), "Thread-1");

        Thread t2 = new Thread(() ->
                Transfer.transfer(accountB, accountA, 200), "Thread-2");

        long startTime = System.currentTimeMillis();

        t1.start();
        t2.start();

        // Watchdog: monitor for deadlock after a grace period
        Thread watchdog = new Thread(() -> {
            try {
                // Give threads 1 second to complete normally
                Thread.sleep(1000);

                if (t1.isAlive() || t2.isAlive()) {
                    System.out.println("\n[WATCHDOG] Threads still running after 1 second...");
                    System.out.println("[WATCHDOG] Checking for deadlock...\n");
                    Thread.sleep(500);

                    // If both threads are still alive and each is waiting, it's deadlock
                    if (t1.isAlive() && t2.isAlive()
                            && Transfer.thread1WaitingFor != null
                            && Transfer.thread2WaitingFor != null) {
                        System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                        System.out.println("  Deadlock detected: transactions are stuck");
                        System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                        System.out.println();
                        System.out.println("  Thread-1 holds Account-A  and is waiting for "
                                + Transfer.thread1WaitingFor);
                        System.out.println("  Thread-2 holds Account-B  and is waiting for "
                                + Transfer.thread2WaitingFor);
                        System.out.println();
                        System.out.println("  Circular wait: Thread-1 -> Account-B -> Thread-2 -> Account-A -> Thread-1");
                        System.out.println();
                        System.out.println("Current balances (no transfer completed):");
                        System.out.println("  Account-A: $" + accountA.balance);
                        System.out.println("  Account-B: $" + accountB.balance);
                        System.out.println("  Total    : $" + (accountA.balance + accountB.balance));
                        System.out.println();
                        System.out.println("[WATCHDOG] Interrupting stuck threads to terminate...");
                        t1.interrupt();
                        t2.interrupt();
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        watchdog.setDaemon(true);
        watchdog.start();

        t1.join(4000);
        t2.join(4000);

        if (!Transfer.transferCompleted.get()) {
            System.out.println("\n[MAIN] No transfer completed. Deadlock confirmed.");
        } else {
            System.out.println("\nFinal Balance - Account-A: $" + accountA.balance
                    + "  |  Account-B: $" + accountB.balance);
            System.out.println("Final Total: $" + (accountA.balance + accountB.balance));
        }

        long elapsed = System.currentTimeMillis() - startTime;
        System.out.println("[MAIN] Program exiting after " + elapsed + " ms.");
        System.exit(0);
    }
}