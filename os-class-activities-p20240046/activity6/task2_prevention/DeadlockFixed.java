import java.util.concurrent.Semaphore;

class AccountSafe {
    String name;
    int balance;

    AccountSafe(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class SafeTransfer {
    // ONE shared mutex protecting the entire transfer operation.
    // Only one thread can enter the critical section at a time,
    // which eliminates circular wait between account-level locks.
    static Semaphore mutex = new Semaphore(1);

    static void transfer(AccountSafe from, AccountSafe to, int amount) {
        String tName = Thread.currentThread().getName();
        try {
            System.out.println("[" + tName + "] Requesting mutex to transfer $"
                    + amount + " from " + from.name + " to " + to.name + "...");

            mutex.acquire();   // acquire the global mutex — only one transfer at a time

            try {
                System.out.println("[" + tName + "] Mutex acquired. Executing transfer...");

                // Simulate some processing time (no deadlock risk — mutex is already held)
                Thread.sleep(100);

                if (from.balance < amount) {
                    System.out.println("[" + tName + "] Insufficient funds in "
                            + from.name + ". Transfer aborted.");
                    return;
                }

                from.balance -= amount;
                to.balance   += amount;

                System.out.println("[" + tName + "] SUCCESS: Transferred $" + amount
                        + " from " + from.name + " to " + to.name);
            } finally {
                mutex.release();   // always release, even if an exception occurs
                System.out.println("[" + tName + "] Mutex released.");
            }

        } catch (InterruptedException e) {
            System.out.println("[" + tName + "] Interrupted.");
            Thread.currentThread().interrupt();
        }
    }
}

public class DeadlockFixed {
    public static void main(String[] args) throws InterruptedException {

        AccountSafe accountA = new AccountSafe("Account-A", 1000);
        AccountSafe accountB = new AccountSafe("Account-B", 1000);

        int startingTotal = accountA.balance + accountB.balance;

        System.out.println("=== Bank Transfer - Deadlock Prevention (Mutex Solution) ===");
        System.out.println("Starting Balance - Account-A: $" + accountA.balance
                + "  |  Account-B: $" + accountB.balance);
        System.out.println("Starting Total: $" + startingTotal);
        System.out.println("Prevention Strategy: Single semaphore mutex (initialized to 1)");
        System.out.println("------------------------------------------------------------");
        System.out.println("Transaction 1 (Thread-1): Transfer $100 from Account-A -> Account-B");
        System.out.println("Transaction 2 (Thread-2): Transfer $200 from Account-B -> Account-A");
        System.out.println("------------------------------------------------------------\n");

        Thread t1 = new Thread(() ->
                SafeTransfer.transfer(accountA, accountB, 100), "Thread-1");

        Thread t2 = new Thread(() ->
                SafeTransfer.transfer(accountB, accountA, 200), "Thread-2");

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        int finalTotal = accountA.balance + accountB.balance;

        System.out.println("\n------------------------------------------------------------");
        System.out.println("Final Balance - Account-A: $" + accountA.balance
                + "  |  Account-B: $" + accountB.balance);
        System.out.println("Final Total  : $" + finalTotal);
        System.out.println("Starting Total was: $" + startingTotal);

        if (finalTotal == startingTotal) {
            System.out.println("Balance check PASSED: Total is unchanged. ($"
                    + startingTotal + " -> $" + finalTotal + ")");
        } else {
            System.out.println("Balance check FAILED: Money was created or destroyed!");
        }

        System.out.println("\nNo deadlock occurred");
        System.out.println("Both transfers completed successfully using the mutex semaphore.");
        System.out.println("The single mutex ensured mutual exclusion over the full transfer,");
        System.out.println("preventing any circular wait between account-level locks.");
    }
}