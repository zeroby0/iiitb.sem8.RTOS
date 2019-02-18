## Database server benchmarks

Benchmarks can be taken using the `zsh` script [benchmark.sh](./benchmark.sh). This folder has a lot of tooling for benchmarks. The [EchoServer](../echo) is cleaner and might be easier to understand.

Measurements are made upto **9 simultaneous clients**. Each n-simultaneous-clients simulation is run 500 times and Min, Max, and Mean are calculated. These limits can be changed from scripts [benchmark.sh](./benchmark.sh) and [runner.sh](./runner.sh) respectively. Change the gnuplot file [bm.gp](./bm.gp) to change how the benchmarks are plotted.

Measured data from the benchmarks is available in the [results](./results) folder. When `i` clients are simultaneously running, `client-i.csv` has the measurements from clients and `server-i.csv` has the server measurements. `{client/server}-{min/max/mean}.csv` has the min/max/mean of client/server.

## Server
Time in microseconds from the moment message is received to the moment message send completes. Click on the pictures to enlarge.

|Maximum|Minimum|Mean|
|--|--|--|
|![](./results/images/server-max.png)|![](./results/images/server-min.png)|![](./results/images/server-mean.png)|

## Client
Time in microseconds from the moment message is sent to the moment is completely recieved.  Click on the pictures to enlarge. 

|Maximum|Minimum|Mean|
|--|--|--|
|![](./results/images/client-max.png)|![](./results/images/client-min.png)|![](./results/images/client-mean.png)|

## Server delay with N clients
`N` clients are run at a time, and this is repeated `500` times. 
For every request from client, server logs the time taken,
so there are `Nx500` samples in the image for n-clients.
Click on the pictures to enlarge.

||||
|--|--|--|
|Number of clients = 1|Number of clients = 2|Number of clients = 3|
|![](./results/images/server-1.png)|![](./results/images/server-2.png)|![](./results/images/server-3.png)|
|Number of clients = 4|Number of clients = 5|Number of clients = 6|
|![](./results/images/server-4.png)|![](./results/images/server-5.png)|![](./results/images/server-6.png)|
|Number of clients = 7|Number of clients = 8|Number of clients = 9|
|![](./results/images/server-7.png)|![](./results/images/server-8.png)|![](./results/images/server-9.png)|

## Client delay with N clients

`N` clients are run at a time, and this is repeated `500` times. 
All clients measure the time taken and log to a file, 
so there are `Nx500` samples in the image for n-clients.
Click on the pictures to enlarge.

||||
|--|--|--|
|Number of clients = 1|Number of clients = 2|Number of clients = 3|
|![](./results/images/client-1.png)|![](./results/images/client-2.png)|![](./results/images/client-3.png)|
|Number of clients = 4|Number of clients = 5|Number of clients = 6|
|![](./results/images/client-4.png)|![](./results/images/client-5.png)|![](./results/images/client-6.png)|
|Number of clients = 7|Number of clients = 8|Number of clients = 9|
|![](./results/images/client-7.png)|![](./results/images/client-8.png)|![](./results/images/client-9.png)|