## Voice over IP

Voice is recorded via pulse audio and is transmitted over tcp stream sockets.

Client records and sends audio while server recieves and plays it. For two people to communicate in a full-duplex manner, both of them should be running both the files. When a connection is made to the server, the corresponding client initiates a connection back to the caller. Both the clients send and both the servers play back.

