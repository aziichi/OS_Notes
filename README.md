# Operating Systems Notes

## Chapters 

1. [Introduction](#introduction)
2. [Operating System Structures](#operating-system-structures)
3. [File System Structure of Linux](#file-system-structure-of-linux)
4. [Processes](#processes)
5. [The Process API](#the-process-api)
6. [Limited Direct Execution](#limited-direct-execution)
7. [Inter-Process Communication](#inter-process-communication)
8. [Inter-Process Communication API](#inter-process-communication-api)
9. [Threads](#threads)
10. [Thread API](#thread-api)
11. [CPU Scheduling](#cpu-scheduling)
12. [Multi-level Feedback Queue](#multi-level-feedback-queue)
13. [Lottery Scheduling](#lottery-scheduling)
14. [Concurrency](#concurrency)
15. [Solutions for the Race Condition](#solutions-for-the-race-condition)
16. [Conditional Variables and Semaphores](#conditional-variables-and-semaphores)
17. [Deadlock](#deadlock)
18. [Deadlock Prevention](#deadlock-prevention)
20. [Deadlock Avoidance](#deadlock-avoidance)
19. [Deadlock Detection and Recovery](#deadlock-detection-and-recovery)
20. [Memory Management and Contiguous Memory Allocation](#memory-management-and-contiguous-memory-allocation)
22. [Free Space Management](#free-space-management)
21. [Paging](#paging)
22. [Segmentation](#segmentation)
23. [Virtual Memory](#virtual-memory)
24. [Page Replacement Algorithms](#page-replacement-algorithms)
25. [Thrashing](#thrashing)
26. [Files and Directories](#files-and-directories)
    
## Introduction

- Operating system acts as an interface between user and the hardware. It acts as a resource manager because it provides the virtualization of CPU and main memory. It also handles various tricky concurrency issues and stores files persistently.
- One goal of OS is to provide **abstractions** in order to make the system easy to use and also to provide **high performance** by minimizing the overhead. Another goal is to provide **protection** between applications by assigning them their own address space in memory, thus using the principle of **isolation**.
- Types of OS include:
	- Single User OS
	- Batch OS
	- Multi-Programming OS (Multi-User, Multi-Tasking) 
	- Multi-Processing OS
	- Time Sharing OS
	- Distributed OS
	- Real Time OS

## Operating System Structures

- Simple/Monolithic Structure
- Micro-Kernel Structure
- Layered Structure
- Virtual Machines

## File System Structure of Linux

- It has a **hierarchical structure** with root at the top and other directories and files hanging from it.	```
```
	|-- Root
		|-- Page daemon
		|-- Swapper
		`-- Init
			|-- User 1		
			|-- User 2
			|-- User 3
```
- There's a command interpreter called the **shell** that allows users to issue commands and navigate the system.


## Processes

- The process is a **running program**. It can be described by its state, the contents of memory in its address space, its CPU registers, the program counter and the stack pointer and information about I/O, accounting information, etc.
- The **process API** consists of calls programs can make related to processes. Typically, this includes creation, destruction, and other useful calls.
- Process states are **new, running, ready, waiting, terminated**.
- A process list contains information about all processes in the system. Each entry is found in what is sometimes called a **process control block (PCB)**, which is really just a structure that contains information about a specific process.

## The Process API

- Each process is identified by a **Process ID (PID)**.
- Process creation is done using the f**ork()** syscall which creates a child process when called from the parent process.
- **wait()** syscall waits for the child process to finish first.
- The **exec()** family of syscalls allow us to execute an entirely new program in the child process.
- **exit()** syscall is used to terminate a process with an exit return code passed as an argument. 
- **Signals** give us the ability to control processes. They can stop, continue, and even terminate the processes.
- As there are multiple users on the OS, a **user** can control only their own processes.
- A **superuser** can control all the processes.
- The first process that starts when the OS boots is the **init** process which is a user level process and all the other processes are created from it.
- A **zombie process** is the one whose parent has terminated before it. Such process is given to the init process in Unix/Linux and then it cleans it up.

## The Producer and Consumer Problem

- There is a **fixed size buffer**. A producer produces some data and places it inside that buffer while a consumer consumes the data from it.
- There is a pointer that points to the current empty slot in the buffer where the data can be placed and another pointer that points to the slot where the data can be taken from.
- The buffer is **empty** if both pointers are at the same position. Same can be said in the case if the buffer is **full**.
- For empty buffer, the consumer sits idle while for full buffer the producer sits idle.

## Limited Direct Execution

 - Restricted Operations are achieved through two modes of execution in the CPU, **user** mode and **kernel** mode.
 - A system call is used to **trap** into kernel mode from user mode.
 - The trap instruction saves register state, changes the status to kernel mode and jumps to the predefined destination in the OS pointed to by the address in the **trap table**.
 - A **return-from-trap** instruction returns it back to user mode.
 - The trap table is set by OS at **boot time** (in kernel mode).
 - The process gives control of the CPU back to the OS using two approaches, **cooperative approach** and **non-cooperative approach**.
 - In cooperative approach, it makes a system call or generate a trap so that OS can get the control back. But if it get stuck in an infinite loop and does not make a system call or generate trap, then the only solution will be a **reboot**.
 - In non-cooperative approach, there is a **timer interrupt** that interrupts the CPU every x milliseconds so that it can give control back to the OS.
 - During the timer interrupt or system call, the OS might want to switch from one process to another. This is known as a **context switch**.

## Inter-Process Communication

- Processes can communicate with each other using two methods, **Direct** communication or **Indirect** communication.
- Direct communication: In this, one process sends the message directly to the other process using its name or identifier. Similarly, the other process receives the message using the name or identifier of the sender.
- Indirect communication: In this, one process sends the message to some mailbox (port), which is a channel and then the other process receives the message from that mailbox.
- The channels used in Unix/Linux for inter-process communication are **Pipe**, **FIFO**, **Sockets**.

## Inter-Process Communication API

- We can use **pipe()** system call in C for inter-process communication.
- FIFO is a special file type with which a file can be created to allow inter-process communication.
- **mknod()** or **mkfifo()** system calls are used to create this special file.
- Unlike pipe, FIFO needs to **explicitly** opened first before data can be written to it.

## Threads

- It is **lightweight** process. A process can have multiple threads which share the **same address space** of the process and can run concurrently.
- Threads share the code, data and file descriptors, PCB, etc. of the process but each thread has its own registers, program counter and stack pointer. 
- Threads are **less resource intensive** and easier to create. They result in more **responsiveness**. 
- There can be three types of thread models:
	- **One-to-One**: Each user level thread has a corresponding kernel level thread.
	- **Many-to-One**: Many user level threads have one kernel level thread.
	- **Many-to-Many**: Many user level threads have many kernel level threads.

## Thread API

- In Unix/Linux we have **PThreads** which can be used in C using the **pthread** library.
- **pthread_create()** is used to create a thread.
- **pthread_join()** is used to join a thread to the calling thread.
- Joining a thread means that the calling thread waits for the joined thread to finish its execution first before executing itself (i.e it blocks itself until then)

## CPU Scheduling

- **Operating system schedulers** and the importance of **context switching** understanding.
- **Assumptions** about running processes and their impact on **scheduling policies**.
- Introduction to **scheduling metrics**, primarily focusing on **turnaround time**.
- Basic algorithm executing the **first arrived job** - **First-Come, First-Served (FCFS)**. **Simplicity** but potential performance issues, e.g., **convoy effect**.
- Prioritizes the **shortest job** based on remaining execution time - **Shortest Job First (SJF)**. Optimal under certain conditions, potential for **starvation**.
- Preemptive SJF allowing job switches before completion. Addresses **convoy problem** by prioritizing **shorter jobs** - **Preemptive Shortest Job First (PSJF)**.
- Introduction of **response time** as a metric. SJF not optimal for response time due to **turnaround time focus**.
- **Time-slicing algorithm** for fair CPU time distribution - **Round Robin (RR)**. Good for response time but can have higher **turnaround time**.
- Importance of handling **I/O** in scheduling. Treating each **CPU burst**, including I/O, as an independent job for better **overlap**.
- Difficulty of accurately predicting job lengths. Need for schedulers performing well without prior knowledge of **job lengths**.
- Two scheduler families: SJF/STCF for **turnaround time optimization**, RR for **response time**. Balancing trade-offs between **turnaround time** and **response time**. Addresses I/O considerations and challenges in predicting job lengths.

## Multi-level Feedback Queue

- The **Multi-Level Feedback Queue (MLFQ)** scheduler is a well-known approach to scheduling. It aims to optimize **turnaround time** and make the system responsive to **interactive users**.
- MLFQ addresses the challenge of scheduling without prior knowledge of job lengths, striving to minimize **turnaround time** and **response time** simultaneously.
- MLFQ learns from the past to predict the future, which is common in **operating systems** and other **CS areas**. It works well when jobs have predictable phases of behavior.
- MLFQ has distinct queues with different **priority levels**. A job on a higher-priority queue is chosen to run first. Basic rules include **priority-based decision-making** and **round-robin scheduling** among jobs with the same priority.
- Introduces the concept of **job allotment**, representing the time a job can spend at a given priority. Priority adjustments are based on **job behavior**, using rules for entering the system, using up allotment, and giving up the CPU.
- It addresses the problem of **starvation** by periodically boosting the priority of all jobs to the **topmost queue**. This ensures progress for **CPU-bound jobs** and proper treatment for **interactive ones**.
- Improves accounting of **CPU time** to prevent gaming of the scheduler. Redefines rules for using up time allotment and demoting jobs based on better accounting.
- There are challenges in parameterizing the scheduler, including the number of queues, **time slice length**, and priority boost frequency. 
- The MLFQ approach as a scheduling method that learns from **job behavior**. List of MLFQ rules:
   1. **Rule 1:** If Priority(A) > Priority(B), A runs (B doesn’t).
   2. **Rule 2:** If Priority(A) = Priority(B), A & B run in round-robin fashion using the time slice (quantum length) of the given queue.
   3. **Rule 3:** When a job enters the system, it is placed at the highest priority (the topmost queue).
   4. **Rule 4:** Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).
   5. **Rule 5:** After some time period S, move all the jobs in the system to the topmost queue.

## Lottery Scheduling

- **Proportional-Share Scheduling Concepts**
  - Guarantees each job a certain **percentage** of CPU time
  - Introduces the concept of "**tickets**" representing a process's share of resources
  - **Lottery scheduling** is an early example where processes hold tickets, and a lottery determines which process runs next

- **Lottery Scheduling**
  - Involves periodically holding a lottery to select the next process to run
  - Processes with more **tickets** have a higher chance of winning the lottery
  - Utilizes **randomness** for decision-making to achieve probabilistic correctness
  - Provides **flexibility**, **simplicity**, and **lightweight operation**

- **Ticket Mechanisms**
  - Introduces mechanisms like **ticket currency**, **ticket transfer**, and **ticket inflation** to manipulate tickets
  - **Ticket currency** allows users to allocate tickets among their jobs
  - **Ticket transfer** enables temporary transfer of tickets between processes
  - **Ticket inflation** allows processes to temporarily adjust their ticket count
  
- **Implementation of Lottery Scheduling**
  - Implements a simple decision-making process using a **random number generator** and a list of processes with tickets
  - The winning **ticket** determines the next process to run
  
- **Stride Scheduling**
  - A deterministic **fair-share scheduler**, providing precise CPU time proportions
  - Assigns a "**stride**" value to each process based on its **ticket count**
  - Utilizes a **pass counter** to determine which process to run next
  - Provides **exact proportions** at the end of each scheduling cycle
  
- **Linux Completely Fair Scheduler (CFS)**
  - Focuses on **fairness**, **scalability**, and **efficiency** in scheduling
  - Utilizes **virtual runtime (vruntime)** to evenly distribute CPU time among processes
  - Employs **red-black trees** to efficiently organize and access running processes
  - Incorporates controls for process priority through the "**nice**" value and **weight computation**
  - Handles **sleep/wake scenarios** to avoid starvation without wasting CPU cycles
  
- **Efficiency in CFS**
  - Aims to minimize **scheduling overhead** and spends very little time making scheduling decisions
  - Uses parameters like **sched latency** and **min granularity** to balance fairness and performance
  - Efficiently manages process priority using **weights** derived from the **nice** value


## Concurrency

- **Concurrency**
    - Execution of multiple instruction sequences simultaneously.
    - Occurs in the operating system when multiple process threads run in parallel.
- **Critical Section:
    - Piece of code accessing a shared resource, like a variable or data structure.
- **Race Condition**
    - Arises when multiple threads enter the critical section simultaneously.
    - Concurrent updates to the shared data structure may lead to unexpected outcomes.
- **Indeterminate Program**
    - Program with one or more race conditions.
    - Output varies between runs based on the timing of thread executions.
    - Results in non-deterministic program behavior.
- **Avoiding Problems**
    - Threads should use mutual exclusion primitives.
    - Ensures only one thread enters a critical section at a time.
    - Eliminates race conditions, leading to deterministic program outputs.
      
      
## Solutions for the Race Condition

- **Atomic Operations**
	- An atomic operation is an indivisible and uninterruptible operation that occurs in a single, instantaneous step. In the context of critical sections and race conditions, making a critical code section an atomic operation means that the entire section is executed without interruption.
-  **Mutual Exclusion using Locks**
	- Mutual exclusion is a synchronization technique that ensures only one thread or process can access a critical section at a time.
-  **Semaphores**
	- Semaphores are synchronization objects used to control access to a shared resource by multiple processes or threads. They maintain a count, and operations like wait () and signal () are performed on them.
- **Use of Flag Variable:**
    - Not a good solution because it does not fulfill the condition of progress.
- **Peterson’s Solution:**
	- It uses two variables, 'turn' and an array of flags, to allow processes to enter their critical sections in a mutually exclusive manner.
    - Effective for avoiding race conditions but limited to two processes/threads.
- **Mutex/Locks:**
    - Used to implement mutual exclusion and prevent race conditions.
    - Only one thread/process allowed to access the critical section.
    - **Disadvantages:**
        -  **Contention**
            - If one thread acquires the lock, others may be busy waiting.
            - Risk of infinite waiting if the acquiring thread dies.
        - **Deadlocks**
        - **Debugging challenges**

## Conditional Variables and Semaphores

- **Conditional Variable:**
  - Synchronization primitive for thread synchronization based on a certain condition.
  - Works in conjunction with a lock.
  - Threads enter a wait state only when they acquire a lock, releasing it during the wait.
  - When another thread signals the occurrence of the event, the waiting thread transitions to the running state, reacquires the lock, and resumes execution.
  - **Use of Conditional Variable**
    - To avoid busy waiting.
  - Contention is not present.
  - In POSIX standard, it has two calls **pthread_cond_wait()** and **pthread_cond_signal()**

- **Semaphores:**
  - Synchronization method using an integer representing the number of resources.
  - Allows multiple threads to execute critical sections concurrently.
  - Different from mutex, which allows only one thread to access a shared resource at a time.
  - **Binary Semaphore:**
    - Can have values 0 or 1.
    - Also known as mutex locks.
  - **Counting Semaphore:**
    - Can range over an unrestricted domain.
    - Controls access to a resource with a finite number of instances.
- In POSIX standard, it has two calls **sem_wait()** and **sem_post()**

- **Modifying Semaphore Operations:**
  - To overcome busy waiting, modify wait() and signal() semaphore operations.
  - When a process executing wait() finds the semaphore value not positive, it blocks itself instead of busy waiting.
  - The block operation places the process in a waiting queue associated with the semaphore, and the CPU scheduler selects another process to execute.

- **Restarting Blocked Processes:**
  - A blocked process waiting on semaphore S restarts when another process executes a signal() operation.
  - Restarting is done through a wakeup() operation, changing the process from the waiting state to the ready state and placing it in the ready queue.

## Deadlock
- In a **multi-programming environment**, processes **compete** for **finite resources**.
- Process **requests** a resource, entering a **waiting state** if it's **unavailable**, potentially leading to **deadlock**.
- **Deadlock** occurs when two or more processes are **waiting** for resources that are held by each other.
- **Deadlock** is a **synchronization bug** where processes remain **indefinitely stuck**, tying up system resources.
- Examples of resources include **memory space**, **CPU cycles**, **files**, **locks**, **sockets**, and **I/O devices**.
- Resources can have **multiple instances**, like multiple **CPUs** in a system.
- Process/resource **utilization** involves **requesting**, **using**, and **releasing** resources.
- **Deadlock necessary conditions** include **mutual exclusion**, **hold and wait**, **no preemption**, and **circular wait**.
- Methods for handling deadlocks include **prevention** and **avoidance** protocols, **deadlock detection** and **recovery**, or **ignoring** the problem.

## Deadlock Prevention
- Ensures at least one necessary condition cannot hold.
  
- **Mutual Exclusion**:
  - Use locks only for non-sharable resources.
  - Some resources are intrinsically non-sharable, so denial of mutual exclusion doesn't prevent deadlock.
  
- **Hold & Wait**:
  - Guarantee a process requesting a resource doesn't hold any other resource.
  - Protocols:
    - (A): Request and allocate all resources before execution.
    - (B): Allow process to request resources only when it holds none, and release all before requesting more.
    
- **No Preemption**:
  - If a process can't immediately get a resource, preempt all current resources and restart when they're available.
  - Alternatively, preempt desired resource from waiting process if it's allocated to a process waiting for more resources.
  
- **Circular Wait**:
  - Impose proper ordering of resource allocation.
  - Ensure processes acquire resources in a predefined order to avoid circular waits.

## Deadlock Avoidance
- Kernel given advance info on resources processes will use.
- System decides whether process should wait based on available resources, current allocations, and future requests.
- Schedule processes and resources to prevent deadlock.
- **Safe state**: System can allocate resources to each process and avoid deadlock.
- **Unsafe state**: System cannot prevent processes from requesting resources that may lead to deadlock.

- **Banker Algorithm**:
  - Determines if allocating resources to a process will leave system in a safe state.
  - Process may wait if allocation would result in an unsafe state.

## Deadlock Detection and Recovery
- Used when prevention or avoidance techniques are not implemented.
  
- **Single Instance of Each Resource Type** (Wait-for Graph Method):
  - Deadlock exists if there's a cycle in the wait-for graph.
  - System maintains wait-for graph and periodically checks for cycles.
  
- **Multiple Instances for Each Resource Type**:
  - Banker Algorithm can be employed for deadlock detection and avoidance.

- **Recovery from Deadlock**:
  - **Process Termination**:
    - Abort all deadlock processes.
    - Abort one process at a time until deadlock cycle is eliminated.
  - **Resource Preemption**:
    - Successively preempt resources from processes and allocate to others until deadlock cycle is broken.
    - 
## Memory Management and Contiguous Memory Allocation
- **Logical versus Physical Address Space**:
  - **Logical Address**:
    - Generated by CPU.
    - Address of instruction or data used by a process.
    - User can access.
    - Indirect access to physical address.
    - Doesn't exist physically, **virtual address**.
    - Set of all logical addresses generated by any program, **Logical Address Space**.
  - **Physical Address**:
    - Loaded into memory-address register.
    - User cannot access.
    - Location in main memory.
    - Accessed indirectly.
    - Set of all physical addresses corresponding to logical addresses, **Physical Address Space**.
    - Computed by **MMU**.

- **Memory Mapping and Protection**:
  - OS provides **Virtual Address Space (VAS)**.
  - **Relocation register** (Base address) and **limit register** determine legal address range.
  - **MMU** maps logical address dynamically.
  - Dispatcher loads relocation and limit registers during **context switch**.
  - Protection from modification ensured by checking addresses against registers.
  - Accessing OS or other users' memory results in OS **trap**, treated as **fatal error**.

- **Allocation Method on Physical Memory**:
  - **Contiguous Allocation**:
    - **Fixed Partitioning**:
      - Main memory divided into equal or different sized partitions.
      - Limitations: **internal** and **external fragmentation**, **process size limitation**, **low degree of multi-programming**.
    - **Dynamic Partitioning**:
      - Partition size not declared initially.
      - Advantages: **no internal fragmentation**, **no process size limit**, **better degree of multi-programming**.
      - Limitation: **external fragmentation**.

## Free Space Management
- **Defragmentation/Compaction**:
  - **Dynamic partitioning** suffers from **external fragmentation**.
  - **Compaction** minimizes external fragmentation by making all free partitions contiguous and bringing all loaded partitions together.
  - **Defragmentation** merges free partitions to store larger processes, making free space contiguous.

- **Representation of Free Space in OS**:
  - Free holes in memory are represented by a **free list** (Linked-List data structure).

- **Satisfying Request of Size n from Free Holes List**:
  - OS implements various algorithms to find and allocate holes:
    - **First Fit**:
      - Allocate first hole big enough.
      - Simple and fast implementation.
    - **Next Fit**:
      - Starts search from last allocated hole, enhancing First Fit.
    - **Best Fit**:
      - Allocate smallest hole big enough.
      - Reduces internal fragmentation but may cause major external fragmentation.
    - **Worst Fit**:
      - Allocate largest hole big enough.
      - Leaves larger holes, potentially accommodating other processes.

## Paging
- **Disadvantage of Dynamic Partitioning**:
  - **External Fragmentation**.
    - Can be removed by **Compaction**, but with overhead.
    - Need more dynamic/flexible/optimal mechanism for loading processes.

- **Idea behind Paging**:
  - **Paging**: Allows physical address space of a process to be non-contiguous.
  - Avoids external fragmentation and the need for compaction.
  - Divides physical memory into fixed-sized blocks called **Frames**, and logical memory into blocks of the same size called **Pages**.

- **Page Table**:
  - Data structure storing page-to-frame mappings.
  - Contains base address of each page in physical memory.
  - Divides CPU-generated addresses into page number (p) and page offset (d) to access frame in physical memory.

- **Page Table Management**:
  - Stored in main memory at process creation, base address stored in **Process Control Block (PCB)**.
  - **Page Table Base Register (PTBR)** points to current page table, facilitates context switching.

- **Avoiding External Fragmentation**:
  - **Paging** allows non-contiguous allocation of process pages in random free frames of physical memory.

- **Performance Enhancement**:
  - **Paging** can be slow due to many memory references.
  - **Translation Look-aside Buffer (TLB)**:
    - Hardware support to speed up paging process.
    - Acts as a hardware cache for high-speed memory.
    - Stores page-to-frame mappings to avoid referencing the page table frequently.
    - **TLB hit** occurs when requested logical address mapping is in TLB.
    - **Address Space Identifier (ASID)** uniquely identifies each process for address space protection and TLB management.
   
## Segmentation
- **Memory Management Aspect with Paging**:
  - **Separation** of user's view of memory from actual physical memory becomes unavoidable.

- **Segmentation**:
  - Memory management technique supporting user's view of memory.
  - Logical address space is a collection of segments, based on user's view.
  - Each segment identified by segment number and offset.

- **Process Segmentation**:
  - Process divided into variable segments based on user's view.

- **Paging and Operating System**:
  - **Paging** primarily managed by the operating system, dividing processes into pages.
  - Operating system may load parts of the same function into different pages, affecting efficiency.

- **Advantages of Segmentation**:
  - **No internal fragmentation**.
  - Contiguous allocation within segment.
  - Segment table size generally smaller than page table.
  - Compiler optimization keeps similar functions within segments.

- **Disadvantages of Segmentation**:
  - **External fragmentation**.
  - Different segment sizes impact swapping efficiency.

- **Hybrid Approach**:
  - Modern system architecture integrates both segmentation and paging in hybrid approach.

 ## Virtual Memory
- Technique allowing execution of processes partially in memory, using secondary memory as swap space.
- Provides illusion of large main memory.
- Programs can exceed physical memory size.
- Enhances CPU utilization and throughput.

- **Demand Paging**:
  - Popular virtual memory management method.
  - Stores least used process pages in secondary memory.
  - Pages brought into main memory on demand or page fault.
  - Utilizes valid-invalid bit scheme in page table.

- **Lazy Swapper**:
  - Swaps pages into memory only when needed.
  - Considers process as sequence of pages, not contiguous address space.

- **Handling Page Fault**:
  - Process faults if accessing page not in memory.
  - OS handles fault:
    - Checks internal table for validity.
    - Swaps in page from disk to memory.
    - Updates page table and restarts interrupted instruction.

- **Pure Demand Paging**:
  - Extreme form where process starts with no pages in memory.
  - Pages brought into memory only when required.

- **Advantages of Virtual Memory**:
  - Increased degree of multi-programming.
  - Large apps runnable with less physical memory.

- **Disadvantages of Virtual Memory**:
  - Slower system due to swapping.
  - Potential for thrashing.

## Page Replacement Algorithms
- **Page Fault Handling**:
  - Occurs when process tries to access page not in memory.
  - OS brings page from swap-space to frame.
  - Page replacement may be necessary if no free frames available.

- **Page Replacement Algorithms**:
  - Aim: Minimize page faults.
  - Types:
    - **FIFO**:
      - Oldest page replaced.
      - Simple but may not perform well.
      - Vulnerable to Belady’s anomaly.
    - **Optimal**:
      - Replaces page not referenced in future.
      - Lowest page fault rate but difficult to implement.
    - **Least Recently Used (LRU)**:
      - Replaces page not used for longest period.
      - Implemented using counters or stack.
    - **Counting-based**:
      - Tracks number of references to each page.
      - Includes **Least Frequently Used (LFU)** and **Most Frequently Used (MFU)** strategies.

- **Belady’s Anomaly**:
  - FIFO may increase page faults with more frames, unlike other algorithms.

- **Challenges**:
  - Optimal algorithm requires future knowledge.
  - LFU and MFU replacements uncommon due to complexity.

- **Objectives**:
  - Ensure efficient use of memory.
  - Minimize page faults and overhead.

## Thrashing 
- Occurs when a process lacks the necessary frames, leading to frequent page faults.
- The process repeatedly replaces pages needed immediately, causing continuous faults.
- Results in the system spending more time on page fault handling than executing processes.

- **Handling Thrashing**:
  - **Working Set Model**:
    - Based on the Locality Model.
    - Sufficient frames are allocated to the process to accommodate the current locality.
    - Insufficient frames lead to thrashing.
  - **Page Fault Frequency**:
    - Thrashing is linked to a high page-fault rate.
    - Aim to control the page-fault rate:
      - Establish upper and lower bounds.
      - Allocate an additional frame if the rate is too high.
      - Remove a frame if the rate is too low.
    - Control the page-fault rate to prevent thrashing.

## Files and Directories
- **File**: 
  - Array of bytes.
  - **Created**, **read**, **written**, and **deleted**.
  - Has a low-level name (i-number).
  
- **Directory**: 
  - Collection of tuples containing human-readable names and low-level names.
  - Each entry maps to another directory or a file.
  - Special entries: **"."** (refers to itself) and **".."** (refers to parent).

- **Directory Tree/Hierarchy**: 
  - Organizes files and directories into a tree starting at the **root**.

- **Accessing Files**: 
  - Process requests permission via system call (usually **open()**).
  - OS grants permission and returns a **file descriptor**.
  - File descriptor is a per-process entity in the **open file table**.

- **File Descriptor**: 
  - Tracks file access, current offset, and other information.
  - Updated by **read()** and **write()**, or via **lseek()** for random access.

- **Persisting Updates**: 
  - Use **fsync()** or related calls to update **persistent media**.
  - Balancing **performance** and **correctness** is crucial.

- **Linking Files**: 
  - Use **hard links** or **symbolic links** to have multiple names refer to the same file.
  - Consider strengths and weaknesses of each.

- **Deleting Files**: 
  - Deleting a file is **unlinking** it from the directory hierarchy.

- **Sharing Mechanisms**: 
  - **Permissions bits** and **access control lists** enable sharing control.
  - Access control lists offer more precise control over access and manipulation.
