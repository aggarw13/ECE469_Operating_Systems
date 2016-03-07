################################Lab 3 - ECE469 README#######################

-> All questions have been completed and are present in their folders (q2 for Radeon Chemical Reaction Problem) within the apps sub-directory, implementation of MailBoxes IPC structure completed in mbox.c && mbox.h  and Lottery Scheduling Features along with Sleep, Yield and Autowake implemented in process.c && process.h files 

-> The updated DLXOS and application programs can be compiled with 'make' command in each of the corresponding folders. The dlx.obj files for any applications test programs after compilation be executed with Makefile (already provided for  q2)

-> OS SCHEDULING EXECUTION NOTE :
    1) The process.c file contains both the Round-Robin (RR) and Lottery Scheduling (LT) code defined under the RR_SCHED and LT_SCHED macro sections, respectively. By default, the process.h file defines the LT_SCHED macro which compiles and enables Lottery scheduling in DLXOS kernel. When testing mailbox implementation and q2, the macro in process.h should be changed to RR_SCHED for compiling the Round-Robin shceduling implementation.
    2) Also, process.h includes macro for enabling Dynamic Scheduling (defined with DYNAMIC_SCHED macro) for Lotter Scheduling. If Static ticketing version Lottery scheduling has to be tested, this macro should be commented out in process.h  

-> APPLICATION (q2) EXECUTION NOTE : After successfully executing the Radeon chhemical reaction program on either of the scheduling forms (RR and LT), the terminal on my ECN account freezes (probably due to limited availability of memory). This may be faced by the grader while testing. Please OPEN a NEW TERMINAL TAB after testing and observing the output of the q2 application program. REMINDER- If using a grading script, please consider if the terminal window freezes.



