# Concurrent System for Book Loan
 The objective of the project is to develop a book loan system. The components of system are described below:

->Requesting Processes (PS): are the processes that will enter the system the operations on the books requested by users. 
  The requesting processes will be invoked from one or more terminals as follows:
              $ requester [–i file] –p pipeReceiver

->Request Receiver (RP): this process receives the requests from the PS and, depending on the type of
request, perform different actions.

  The Receiver Process will be invoked from a terminal as follows:
     $ ./receiver –p pipeReceiver –f filedata [ –s filesoutput ]
 
 Buffer    
 
![image](https://user-images.githubusercontent.com/75964273/174708996-e9fd4edb-49a5-4173-9f22-995325961736.png)

 Processes Communication

![image](https://user-images.githubusercontent.com/75964273/174709018-7971f0c4-1d56-481c-8bf3-95526fa89f7e.png)

Report

![image](https://user-images.githubusercontent.com/75964273/174709064-bb9daf82-6fed-449c-b7ff-5b2da215a78e.png)
