## Chapters 

1. [Introduction](#Introduction)
2. [Operating System Structures](<#Operating System Structures>)
3. [File System Structure of Unix/Linux](<#File System Structure of Unix/Linux>)
4. [Processes](#Processes)
5. [The Process API](<#The Process API>)
6. [Limited Direct Execution](<#Limited Direct Execution>)
7. [Inter-Process Communication](<#Inter-Process Communication>)
8. [Inter-Process Communication API](<#Inter-Process Communication API>)
9. [Threads](#Threads)
10. [Thread API](<#Thread API>)

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

## File System Structure of Unix/Linux

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