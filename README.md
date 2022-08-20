Chat server - C language

Submitted files :

chatServer.c - the chat server simulator code.
READMY - exercise readme.

I created the var : iterator to check that i do not run more then needed iterations , means i checked at each iteratin if iterator is equals to the nReady that select function returned in each iteration.

This program uses select function.

This project is simulates a chat server that handles connections of clients and manage the messeges between the clients.

to run the chat server use the command : 
gcc chatServer.c -o chatServer ./chatServer
and then start telnets (enter localhost from browser) and connect to the port-number.
