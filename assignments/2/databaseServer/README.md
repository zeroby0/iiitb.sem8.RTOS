# Socket::Echo server

## Code layout

- [Commons.h](./commons.h) implements the common modules used by both client and server.
- [Server.h](./server.h) implements the modules used by server.
- [Server.c](./server.c) implements the behaviour of server.
- [Client.h](./client.h) implements the modules used by client.
- [Client.c](./client.c) implements the behaviour of client and some tooling for benchmarks.


## Benchmarks

Benchmarks can be taken using the `zsh` script [benchmark.sh](./benchmark.sh).

Measurements are made upto **16 simultaneous clients**. Each n-simultaneous-clients simulation is run `10000` times and Min, Max, and Mean are calculated. These limits can be changed from scripts [benchmark.sh](./benchmark.sh). Change the gnuplot file [bm.gp](./bm.gp) to change how the benchmarks are plotted.

Measured data from the benchmarks is available in the [results](./results) folder. When `i` clients are simultaneously running, `client-i.csv` has the measurements from clients. `{client}-{min/max/mean}.csv` has the min/max/mean of client.


#### Client
Time in microseconds from the moment message is sent to the moment reply is completely recieved.  Click on the pictures to enlarge. 

|Mean|Maximum|Minimum|
|--|--|--|
|![](./results/images/client-mean.png)|![](./results/images/client-max.png)|![](./results/images/client-min.png)|



##### Client delay with N clients

`N` clients are run at a time, and this is repeated `10000` times. 
All clients measure the time taken and log to a file, 
so there are `Nx10000` samples in the image for n-clients.
Click on the pictures to enlarge.


|||||
|--|--|--|--|
|![](./results/images/client-1.png)|![](./results/images/client-2.png)|![](./results/images/client-3.png)|![](./results/images/client-4.png)|
|Number of clients = 1|Number of clients = 2|Number of clients = 3|Number of clients = 4|
|![](./results/images/client-5.png)|![](./results/images/client-6.png)|![](./results/images/client-7.png)|![](./results/images/client-8.png)|
|Number of clients = 5|Number of clients = 6|Number of clients = 7|Number of clients = 8|
|![](./results/images/client-9.png)|![](./results/images/client-10.png)|![](./results/images/client-11.png)|![](./results/images/client-12.png)|
|Number of clients = 9|Number of clients = 10|Number of clients = 11|Number of clients = 12|
|![](./results/images/client-13.png)|![](./results/images/client-14.png)|![](./results/images/client-15.png)|![](./results/images/client-16.png)|
|Number of clients = 13|Number of clients = 14|Number of clients = 15|Number of clients = 16|
