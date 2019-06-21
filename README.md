UDP Client
----------------------------------------------------
The UDP client side looks similar to that of TCP except that it does not call connect() and also it only needs to d a single receive because UDP sockets preserve message boundaries.
I initialized "fromSize" to contain the size of the address buffer (fromAddr) and then
pass its address as the last parameter, recvfrom() blocks until a UDP datagram addressed to this socket's port arrives. It then copies the data from the first arriving datagram into echoBuffer and copies the Internet address and (UDP) port number of its source from the packet's headers into the structure fromAddr. Note that the data
buffer is actually one byte bigger than ECHOMAX, to allow us to add a null byte to terminate the string.
it will work correctly most of the time. However, it would not be suitable for production use, because if a message is lost going to or from the server, the call to recvfrom() blocks forever, and the program does not terminate. Clients generally deal with this problem through the use of timeouts.

UDP Server
-----------------------------------------------------------------
The server is very simple. It loops forever, receiving a message and then sending the same message back to wherever it came from. Actually, the server only receives and sends back the first 255 characters of the message; any excess is silently discarded by the sockets implementation.
