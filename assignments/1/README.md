# Message Queue
 In this assignment you are expected to design and implement a client server system using message queues, running on a single machine.

## Road Map: 

In the subsequent assignments, we will modify this program  to work across machines. Using this as a foundation we will  build a webserver that will run on small embedded computers, which can respond to real time requests.  Then, we will measure the performance of the webserver for various requests. Today's emebdded Systems do not have any Display or keyboard interfaces. Instead they have webserver running within them. We use our smart phone to connect to that webserver and do various configurations do operations etc. (Eg: Wifi Router at home, Jio 4G dongle all have a small webserver running inside them today, in future, Refrigerators, Washing machines, Microwave Oven etc will come with web interface, so that we can control them through our smartphone instead of using push button which do suffer from loose connections , wear & tear etc). We are using this concept to have case study for our assignment.

**However In this exercise our focus is with implementation of Client server system using message queues.**

## Echo Engine

1. We have a Process called Server (a separate executable) running on a computer. The folder where it has started has collection of text files (all ending with .txt extenstion). 
2. We have a five separate process (Called Clients) ClientA, ClientB, ClientC, ClientD, ClientE.  Each of them are running in separate folders.
3. Each client can communicate with Server through a message queue.  Similarly server can send messages back to the client through another message queue.
4. As the first step you should implement a server and five clients that can communicate through message queue. For example. The Client A sends an alphabet say.... 'd' to server, server receives it converts it into upper case and send it client. Like this every client will send an alphabet to server and it respond back with the corresponding upper case. In case if you send an upper case alphabet, the server responds with lower case. If you send any other character like \*, $ % or numbers the server does not do anything it just respond back with the same character.

Once you get such a simple client server working with message queue,we can go to the next level, which is implementing a data base engine.
In this exercise you are expected to measure the time taken by a server to respond to a request under various conditions. Like when all the five clients are sending request at the same time, when only one client is sending when two clients are sending request  like that and tabulate the results.

## Data base Engine

Here we need to modify the functionality of both client and server
As it was mentioned earlier the server has a collection of files (hundreds of files) in its folder.   Client send a request as string to the server the request will of the format 
GET <filename>  and server will respond to the request by sending the contents of the file back to the client as another string.
Eg  Client send GET marks.txt  then the server will respond back with contents of the file.
In case if the client send a filename which does not exist in server, the  server will respond with the string "Data not found"
You are expected to test the client server properly with all combinations. Like one client sending a request, multiple clients ( 5 max) sending request still the server responds.
In this exercise you are expected to measure the time taken by a server to respond to a request under various conditions. Like when all the five clients are sending request at the same time, when only one client is sending when two clients are sending request  like that and tabulate the results.

## How to do the assignment

1. Create folder called ASSIGNMENT-1 in your GITHUB repository (repository name RTOS)
2. Create folder called EchoEngine inside ASSIGNMENT-1
3. Implement the Echo Engine and upload it into RTOS/ASSIGNMENT-1/EchoEngine
4. Capture the performance data and enter it into a .md (markdown file) and have it in the same folder
5. Implement the Echo Engine and upload it into RTOS/ASSIGNMENT-1/DatabaseEngine
6. Capture the performance data and enter it into a .md (markdown file) and have it in the same folder