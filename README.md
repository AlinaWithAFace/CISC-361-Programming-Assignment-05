Programming Assignment 5
========================

**User-Level Thread Library**  
Develop this project on cisc361.acad.cis.udel.edu (Linux)

### Objective

In this project, you will design and implement a user-level thread library which supports thread creation, thread scheduling, thread synchronization, and inter-thread communication (message passing and mailbox) functions.

### Assignment

You will design and implement, in 4 phases, a user-level thread library.

#### Phase 1

*   Thread library initialization and shutdown, and thread creation and termination:

*   `void t\_init();` /\* Initialize the thread library by setting up the "running" and the "ready" queues, creating TCB of the "main" thread, and inserting it into the running queue. \*/
*   `void t\_shutdown();` /\* Shut down the thread library by freeing all the dynamically allocated memory. \*/
*   `int t\_create(void (\*func)(int), int thr\_id, int pri);` /\* Create a thread with priority pri, start function func with argument thr\_id as the thread id. Function func should be of type void func(int). TCB of the newly created thread is added to the end of the "ready" queue; the parent thread calling t\_create() continues its execution upon returning from t\_create(). \*/
*   `void t\_terminate();` /\* Terminate the calling thread by removing (and freeing) its TCB from the "running" queue, and resuming execution of the thread in the head of the "ready" queue via setcontext(). \*/
*   An initial thread control block (TCB) may look like the following.
    ```
     struct tcb {
    	  int         thread\_id;
              int         thread\_priority;
    	  ucontext\_t  thread\_context;
    	  struct tcb \*next;
           };
    
           typedef struct tcb tcb; 
    ```

*   Yield the CPU:

*   `void t\_yield();` /\* The calling thread volunarily relinquishes the CPU, and is placed at the end of the ready queue. The first thread (if there is one) in the ready queue resumes execution. \*/

*   Test cases: T1 and T1x

#### Phase 2

*   CPU scheduling and time slicing:
    * 2-Level Queue (2LQ) with level 0 (high priority) and 1 (low priority). By default, the `main` thread is with level 1 (low priority).
    * Round-Robin (RR) scheduler within each level with time quantum of 1 microseconds. Use `ualarm()` to "simulate" timer interrupt so that when `SIGALRM` is caught by the signal handler, context switching is performed. Notice that right before context switching, another `SIGALRM` is scheduled as the next timer interrupt. Also, when a thread yields before its time quantum expires, the scheduled `SIGALRM` should be canceled via `ualarm(0,0)`.
*   Test cases: T1a, T2, T4, and T7

#### Phase 3

*   Thread synchronization with Semaphore:

*   int sem\_init(sem\_t \*\*sp, int sem\_count); /\* Create a new semaphore pointed to by sp with a count value of sem\_count. \*/
*   void sem\_wait(sem\_t \*sp); /\* Current thread does a wait (P) on the specified semaphore. \*/
*   void sem\_signal(sem\_t \*sp); /\* Current thread does a signal (V) on the specified semaphore. Follow the _Mesa_ semantics where the thread that signals continues, and the first waiting (blocked) thread (if there is any) becomes ready. \*/
*   void sem\_destroy(sem\_t \*\*sp); /\* Destroy (free) any state related to specified semaphore. \*/

*   [sample thread library where applications include ud\_thread.h](UD_Thread_Sem.tar)
*   Test cases: T3 and [Dining Philosophers](philosophers.c) \[[4 sample outputs](p) (where all philosophers ate in the 4th run)\]

#### Phase 4

*   Inter-thread communications:

*   mailbox:

*   int mbox\_create(mbox \*\*mb); /\* Create a mailbox pointed to by mb. \*/
*   void mbox\_destroy(mbox \*\*mb); /\* Destroy any state related to the mailbox pointed to by mb. \*/
*   void mbox\_deposit(mbox \*mb, char \*msg, int len); /\* Deposit message msg of length len into the mailbox pointed to by mb. \*/
*   void mbox\_withdraw(mbox \*mb, char \*msg, int \*len); /\* Withdraw the first message from the mailbox pointed to by mb into msg and set the message's length in len accordingly. The caller of mbox\_withdraw() is responsible for allocating the space in which the received message is stored. If there is no message in the mailbox, len is set to 0. mbox\_withdraw() is not blocking. Even if more than one message awaits the caller, only one message is returned per call to mbox\_withdraw(). Messages are withdrew in the order in which they were deposited. \*/
*   An initial mailbox may look like the following.
    
     struct messageNode {
             char \*message;     // copy of the message 
             int  len;          // length of the message 
             int  sender;       // TID of sender thread 
             int  receiver;     // TID of receiver thread 
             struct messageNode \*next; // pointer to next node 
           };
    	 
           typedef struct {
    	  struct messageNode  \*msg;       // message queue
    	  sem\_t               \*mbox\_sem;
           } mbox; 
    
*   Test cases: T6

*   message passing:

*   void send(int tid, char \*msg, int len); /\* Send a message to the thread whose tid is tid. msg is the pointer to the start of the message, and len specifies the length of the message in bytes. In your implementation, all messages are character strings. \*/
*   void receive(int \*tid, char \*msg, int \*len); /\* Wait for and receive a message from another thread. The caller has to specify the sender's tid in tid, or sets tid to 0 if it intends to receive a message sent by any thread. If there is no "matching" message to receive, the calling thread waits (i.e., blocks itself). \[A sending thread is responsible for waking up a waiting, receiving thread.\] Upon returning, the message is stored starting at msg. The tid of the thread that sent the message is stored in tid, and the length of the message is stored in len. The caller of receive() is responsible for allocating the space in which the message is stored. Even if more than one message awaits the caller, only one message is returned per call to receive(). Messages are received in the order in which they were sent. The caller will not resume execution until it has received a message (blocking receive). \*/
*   Test cases: T5 and T8

### How to get started

Start the project from the [sample thread library (a tar file).](UD_Thread.tar)

*   Context: Use getcontext() and setcontext() within the same thread ([let's loop](loop.c))
*   Switching between threads : Use getcontext()/setcontext()/swapcontext() system calls.  
    ([sample swapcontext code](swapcontext.c) and its [execution trace](swapcontext.pdf))  
    ([sample swapcontext via yield code](swapcontext-yield.c))
*   Creating a new thread : Use makecontext() system call.
*   Scheduling : Use ualarm()and a signal handler that catches SIGALRM and schedules the next thread in accordance with the scheduling policy. Use sigrelse(3c)/sighold(3c) to make thread library APIs atomic. ([sample alarm code](test_alarm.c)) ([example critical section code](interrupts.c) by Sean Krail)
*   Semaphore : Atomic execution of wait and signal must be enforced. This can be achieved by disabling and enabling "interrupts" via `sighold(3c)` and `sigrelse(3c)`, respectively.

### Test Runs

The TA will use the test programs [T1](test01.c), [T1x](test01x.c), [T1a](test01a.c), [T2](test02.c), [T2a](test02a.c), [T3](test03.c), [T4](test04.c), [T5](test05.c), [T6](test06.c), [T7](test07.c), [T8](test08.c), [T9](test09.c), [T10](test10.c), and [T11](test11.c), to test your code against outputs [1](1), [1x](1x), [1a](1a.Chris_Meyer), [2](2), [2a](2a), [3](3), [4](4), [5](5), [6](6), [7](7), [8](8), [9](9), [10](10), and [11](11), and may use other test programs as well.  
Blocking send/receive: \[[9 with active V](9.active)\], \[[9 with passive V](9.passive)\], \[[11 with active V](11.active)\], \[[11 with passive V](11.passive)\]

### Contributed Test Runs from Students

Phase 1:

*   [Shone-Hoffman](mytest.c) ([output](mytest_output.txt))
*   [Sullivan](Tfib.c) ([makefile](Makefile))

Phase 2:

Phase 3:

*   [Senzer-Spicer](TSS.c) ([output](TSSOutput.txt) and [README](README.TSS))
*   [Shone-Hoffman](3test.c) ([output](3test_output.txt))

Phase 4:

*   [Senzer-Spicer](SenzerSpicerMailbox.c) ([output](SSP4Output.txt) and [README](README.SSmbox))

* * *

### Grading

*   50% correctly working thread library initialization, thread creation/termination, 2LQ/RR and voluntary scheduling.
*   15% correctly working thread synchronization via semaphore.
*   25% correctly working inter-thread communication.
*   10% documentation and code structure.