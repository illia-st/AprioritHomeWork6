# Apriorit homework 6

Here is a client/server project impolemented using "socket.h".
I have tried to make an async server which can communicate with a large number of clients.

Client is really simple and can connect to the only server.

Server structure:

    Server operates with Session objects.
    Class Session is derived class from CommunicationUnit. Session object is craeted after a new client connection.
    I tried to use OOP affectivly and minimize using c-style programming.

    New connections are being accepted in the different thread. Tasks requested
    by the clients are also queued and performed in a different thread.

Client structure:

    Client is simpler that the Server. Client is synchronous and wait for the server's
    respond only after the request being sent. Client operate wich Host object.
    Host is also derived from CommunicationUnit. The difference between Host
    and Session is that LoadData processes received data differently.


Also there are also other common files such as Buffer.h and Core.h.

Server usage: ./Server_Runner <port>
Client usage: ./Client_Runner <host> <port>

I wanted to implement more features such as async Client and the possibility to create Server and Client
with keep alive status. Unfortunately I didn't have enough time for that.


I will be really glad to receive informative feedback with poiting out every issue
I have made.

Thank you in advance.
