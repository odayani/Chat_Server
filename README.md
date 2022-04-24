Name: Or Dayani
ID: 315710715

ex4 - Chat Server.

subbmitted files:
ChatServer.c - the c file of the program.

 --Functions--
int init_pool(conn_pool_t *pool)

int add_conn(int sd, conn_pool_t *pool)

int remove_conn(int sd, conn_pool_t *pool)

int push_message(conn_t* node, int len, char* buffer)

int add_msg(int sd, char *buffer, int len, conn_pool_t *pool)

int write_to_client(int sd, conn_pool_t *pool)

void intHandler(int SIG_INT)

Compile: 
gcc chatServer.c -o <program_name>
Run:
./<program_name>
