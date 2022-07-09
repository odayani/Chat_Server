Name: Or Dayani
ChatServer.c


 --Functions--
 
:black_circle: int init_pool(conn_pool_t *pool)

:black_circle: int add_conn(int sd, conn_pool_t *pool)

:black_circle: int remove_conn(int sd, conn_pool_t *pool)

:black_circle: int push_message(conn_t* node, int len, char* buffer)

:black_circle: int add_msg(int sd, char *buffer, int len, conn_pool_t *pool)

:black_circle: int write_to_client(int sd, conn_pool_t *pool)

:black_circle: void intHandler(int SIG_INT)


Compile:   gcc chatServer.c -o <program_name>
Run: ./<program_name>
