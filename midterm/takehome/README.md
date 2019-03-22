The timing on server is unreliable. 
Instead, the server watches for changes in files and sends events to client.
For the filewatch, we use coroutines from the other folder.
For server push of events, we use sockets used in the last assignment.