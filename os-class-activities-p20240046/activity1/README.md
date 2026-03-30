Class Activity 1 — System Calls in Practice

    Student Name: Song Phengroth
    Student ID: P20240046
    Date: 27/03/2026

Warm-Up: Hello System Call

Screenshot of running hello_syscall.c on Linux:

Hello syscall

Screenshot of running hello_winapi.c on Windows (CMD/PowerShell/VS Code):

Hello WinAPI

Screenshot of running copyfilesyscall.c on Linux:

Copy file syscall
Task 1: File Creator & Reader
Part A — File Creator

Describe your implementation: [What differences did you notice between the library version and the system call version?]

In the library version, I used fopen() and fprintf() to create and write to the file, which was simpler because the C library handled many low-level details automatically. In the system call version, I used open(), write(), and close(), so I had to manually specify flags, permissions, and manage the file descriptor myself.

Version A — Library Functions (file_creator_lib.c):
![TASK1A](Images/task1_creator_lib.png)

Task 1A - Library

Version B — POSIX System Calls (file_creator_sys.c):
![Images/task1_creator_sys.png](Images/task1_creator_sys.png)

Task 1A - Syscall

Questions:

    What flags did you pass to open()? What does each flag mean?

I passed flags such as O_WRONLY | O_CREAT | O_TRUNC. O_WRONLY opens the file for write-only access, O_CREAT creates the file if it does not already exist, and O_TRUNC clears the file contents if the file already exists.

    What is 0644? What does each digit represent?

0644 is the file permission mode in octal. The first digit 0 shows that the number is written in octal. The owner gets 6, which means read and write. The group gets 4, which means read only. Others also get 4, which means read only.

    What does fopen("output.txt", "w") do internally that you had to do manually?

    fopen("output.txt", "w") internally opens or creates the file, sets the correct mode, prepares buffering, and returns a FILE * stream. In the system call version, I had to manually call open(), provide the flags and permissions, use write(), and close the file with close().

Part B — File Reader & Display

Describe your implementation: [Your notes]

In the library version, I used fgets() to read text line by line from the file. In the system call version, I used open() and read() with a buffer, then displayed the contents using write() or printf(). The system call version required more control over how many bytes were read each time.

Version A — Library Functions (file_reader_lib.c):
![task1_reader_lib.png](Images/task1_reader_lib.png)
Task 1B - Library

Version B — POSIX System Calls (file_reader_sys.c):
![Task1B](<Images/take1_reader_sys.c .png>)
Task 1B - Syscall

Questions:

    What does read() return? How is this different from fgets()?

read() returns the number of bytes actually read. It returns 0 when the end of file is reached, and -1 if an error happens. This is different from fgets(), which returns a pointer to the string if successful and NULL on end of file or error. Also, fgets() reads text as a string, while read() works with raw bytes.

    Why do you need a loop when using read()? When does it stop?

    You need a loop because read() may not return the entire file in one call. It only reads up to the buffer size each time. The loop stops when read() returns 0, which means end of file, or -1, which means an error occurred.

Task 2: Directory Listing & File Info

Describe your implementation: [Your notes] In the library version, I used functions like opendir(), readdir(), and stat() to access directory entries and file information more easily. In the system call version, the process was lower-level and required more manual handling of output and formatting.
Version A — Library Functions (dir_list_lib.c)

Task 2 - Version A
Version B — System Calls (dir_list_sys.c)
![task_2](Images/task2_sys.png)
Task 2 - Version B
Questions

    What struct does readdir() return? What fields does it contain?

    readdir() returns a pointer to a struct dirent. Common fields include d_name, which stores the file name, d_ino, which stores the inode number, and sometimes d_type, which indicates the file type.

    What information does stat() provide beyond file size?

    stat() provides much more than file size. It can give the file type, permissions, inode number, number of links, owner ID, group ID, last access time, last modification time, and last status change time.

    Why can't you write() a number directly — why do you need snprintf() first?

write() only sends raw bytes to a file descriptor. A number in memory is stored in binary form, not as readable text characters. snprintf() converts the number into a string first, so write() can output it in a human-readable format.
Optional Bonus: Windows API (file_creator_win.c)

Screenshot of running on Windows:

Task 1 - Windows
Bonus Questions

    Why does Windows use HANDLE instead of integer file descriptors?

Windows uses HANDLE because it represents a more general reference to many kinds of system objects, not just files. A handle is managed by the Windows kernel and gives more flexibility for different resources such as files, processes, events, and pipes.

    What is the Windows equivalent of POSIX fork()? Why is it different?

    The Windows equivalent is CreateProcess(). It is different because Windows does not duplicate the current process the way fork() does. Instead, it creates a new process from a specified executable, which gives more explicit control but works differently from Unix process creation.

    Can you use POSIX calls on Windows?

    Not directly in normal Windows programs. Windows mainly uses the WinAPI. Some POSIX-like calls can be used through compatibility layers such as Cygwin, MinGW, or WSL, but they are not native Windows system calls.

Task 3: strace Analysis

Describe what you observed: [What surprised you about the strace output? How many more system calls did the library version make?]

I observed that the library version usually makes more system calls than the direct system call version. This is because library functions often perform extra setup work such as buffering, memory allocation, file status checks, and cleanup before calling the actual kernel service. What surprised me most was that a simple high-level function can result in several lower-level system calls behind the scenes.
strace Output — Library Version (File Creator)

strace - Library version File Creator
strace Output — System Call Version (File Creator)

strace - System call version File Creator
strace Output — Library Version (File Reader or Dir Listing)

strace - Library version
strace Output — System Call Version (File Reader or Dir Listing)

strace - System call version
strace -c Summary Comparison

strace summary - Library strace summary - Syscall
Questions

    How many system calls does the library version make compared to the system call version?

The library version makes more system calls than the direct system call version. The exact number should be taken from the strace -c output in my screenshots, but in general the system call version is smaller and more direct.

    What extra system calls appear in the library version? What do they do?

Extra system calls often include brk and mmap for memory management, fstat for getting file information, and sometimes openat for opening files. These support higher-level library behavior such as buffering, stream setup, and internal management. 3. How many write() calls does fprintf() actually produce?

fprintf() may produce one or more write() calls depending on buffering. If the output is flushed at once, it may only produce one write(). If buffering or multiple flushes happen, it can produce more than one.

    In your own words, what is the real difference between a library function and a system call?

A system call is the direct request made to the kernel to perform an operating system service, such as reading a file or creating a process. A library function is a higher-level function provided by user-space libraries that may prepare data, manage buffering, and then call one or more system calls internally. In simple terms, the library function is more convenient, while the system call is the actual interface to the kernel.
Task 4: Exploring OS Structure
System Information

    📸 Screenshot of uname -a, /proc/cpuinfo, /proc/meminfo, /proc/version, /proc/uptime:
    ![TASK4](Images/task4_system_info.png)

System Info
Process Information

    📸 Screenshot of /proc/self/status, /proc/self/maps, ps aux:
    ![Task4a](Images/task4_system_info.png)

Process Info
Kernel Modules

    📸 Screenshot of lsmod and modinfo:
    ![lsmod](Images/task4_modinfo_output.png)

Kernel Modules
OS Layers Diagram

    📸 Your diagram of the OS layers, labeled with real data from your system:
    ![DIagram](Images/Diagram.jpg)

OS Layers Diagram
Questions

    What is /proc? Is it a real filesystem on disk?

    /proc is a virtual filesystem created by the Linux kernel to expose information about processes and the system. It is not a real filesystem stored on disk. Its contents are generated dynamically while the system is running.

    Monolithic kernel vs. microkernel — which type does Linux use?

    Linux uses a monolithic kernel. This means most core operating system services, such as device drivers, memory management, and process management, run inside kernel space.

    What memory regions do you see in /proc/self/maps?

    Typical memory regions include the program code section, data section, heap, stack, shared libraries, and mapped memory regions. You may also see special kernel-related virtual memory areas.

    Break down the kernel version string from uname -a.

    The uname -a output usually includes the kernel name, host name, kernel release, kernel version, machine hardware name, processor type, hardware platform, and operating system. Each part gives information about the running Linux system and hardware architecture.

    How does /proc show that the OS is an intermediary between programs and hardware?

/proc shows that the OS is an intermediary because it gives programs a structured way to access system and hardware-related information without directly touching the hardware. The kernel collects and manages this information, then exposes it through files in /proc.
Reflection

What did you learn from this activity? What was the most surprising difference between library functions and system calls?

From this activity, I learned that library functions and system calls may seem similar, but they operate at different levels. Library functions are easier to use because they provide abstraction, buffering, and convenience, while system calls interact directly with the kernel and require more manual control. The most surprising difference was seeing through strace that one simple library function can trigger several system calls behind the scenes. This helped me understand more clearly how user programs communicate with the operating system.