#include <sys/select.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "chatServer.h"

static int end_server = 0;

void intHandler(int SIG_INT) {
    end_server = 1;
}

int main(int argc, char* argv[]) {

    if(argc < 2){
        printf("Usage: <port>\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);

    conn_pool_t* pool = malloc(sizeof(conn_pool_t));
    init_pool(pool);

    char buffer[300];
    memset(buffer, 0, 300);
    int port = atoi(argv[1]);

    /*********************/
    /* Create an AF_INET stream socket to receive incoming      */
    /* connections on                                            */
    /*********************/
    //    socket(...);
    struct sockaddr_in main_socket;

    int main_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    main_socket.sin_family = AF_INET;
    main_socket.sin_port = htons(port);
    main_socket.sin_addr.s_addr = htonl(INADDR_ANY);


    //    ioctl(...);
    int on = 1;
    int rc = ioctl(main_socket_fd, (int)FIONBIO, (char*)&on);
    if(rc < 0) {
        perror("ioctl");
    }

    /*******/
    /* Bind the socket */
    /*******/
    if (bind(main_socket_fd, (struct sockaddr *) &main_socket, sizeof(main_socket)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    /*********/
    /* Set the listen backlog */
    /*********/
    if (listen(main_socket_fd, 10) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }
    /* Initialize fd_sets */
    FD_SET(main_socket_fd, &(pool->read_set));

    pool->maxfd = main_socket_fd;
    struct sockaddr_in client;
    unsigned int client_len = sizeof(client);
    /* Loop waiting for incoming connects, for incoming data or  */
    /* to write data, on any of the connected sockets.           */

    add_conn(main_socket_fd, pool);

    do
    {
        /* Copy the master fd_set over to the working fd_set. */
        pool->ready_read_set = pool->read_set;
        pool->ready_write_set = pool->write_set;

        /* Call select() */
        printf("Waiting on select()...\nMaxFd %d\n", pool->maxfd);
        select(pool->maxfd + 1, &(pool->ready_read_set), &(pool->ready_write_set), NULL, NULL);

        conn_t* iter = pool->conn_head;
        while(iter != NULL){
            /* Check to see if this descriptor is ready for read */
            if (FD_ISSET(iter->fd ,&pool->ready_read_set)) {

                if(iter->fd == main_socket_fd){
                    /* Read from listening socket */
                    int new_fd = accept(main_socket_fd, (struct sockaddr *)&client, &client_len);
                    add_conn(new_fd, pool);
                    printf("New incoming connection on sd %d\n", new_fd);
                }
                    /* Receive incoming data from socket */
                else if(read(iter->fd, buffer, 1200) <= 0) {
                    close(iter->fd);
                    remove_conn(iter->fd, pool); /* remove the connection (remove_conn(...)) */
                    printf("removing connection with sd %d \n", iter->fd);
                }
                    /* Data was received */
                else{
                    printf("Descriptor %d is readable\n", iter->fd);
                    add_msg(iter->fd, buffer, (int) strlen(buffer), pool);
                    printf("%d bytes received from sd %d\n", (int)strlen(buffer), iter->fd);
                    memset(buffer,'\0',300);
                }

            } /* End of if (FD_ISSET()) */

            /* Check to see if this descriptor is ready for write  */
            if (FD_ISSET(iter->fd ,&pool->write_set) && iter->fd != main_socket_fd){
                /* try to write all msgs in queue to sd */
                write_to_client(iter->fd, pool);
            }
            /*******************/
            /* End of loop through selectable descriptors */
            iter = iter->next;
        }

    }
    while (end_server == 0);

    /*********************/
    /* If we are here, Control-C was typed,						 */
    /* clean up all open connections					         */
    /*********************/

    return 0;
}


int init_pool(conn_pool_t *pool) {
    if (!pool) {
        return -1;
    }
    FD_ZERO(&pool->read_set);
    FD_ZERO(&pool->write_set);
    FD_ZERO(&pool->ready_write_set);
    FD_ZERO(&pool->ready_read_set);
    pool->conn_head = NULL;
    pool->maxfd = 0;
    pool->nready = 0;
    pool->nr_conns=0;
    return 0;
}

int add_conn(int sd, conn_pool_t *pool) {

    conn_t *insert_node = malloc(sizeof(conn_t));
    if (insert_node == NULL) {
        return -1;
    }

    insert_node->fd = sd;
    insert_node->write_msg_head = NULL;
    insert_node->write_msg_tail = NULL;

    //update pool info//
    pool->nr_conns++;
    FD_SET(sd, &pool->read_set);

    if (sd > pool->maxfd) {
        pool->maxfd = sd;
    }

    //point to head of list
    if (pool->conn_head == NULL) {
        pool->conn_head = insert_node;
        insert_node->next = NULL;
        return 0;
    }

    //add to linked list
    conn_t *pointer = pool->conn_head;

    while (pointer->next != NULL) {
        pointer = pointer->next;
    }

    insert_node->prev = pointer;
    pointer->next = insert_node;

    /*
     * 1. allocate connection and init fields VV
     * 2. add connection to pool VV
     * */
    return 0;
}

int remove_conn(int sd, conn_pool_t *pool) {
    conn_t *del_node = pool->conn_head;

    if (pool->conn_head == NULL) {
        perror("empty pool");
        return -1;
    }

    while (del_node->fd != sd) {
        del_node = del_node->next;
    }

    del_node->prev->next = del_node->next; // disconnect it from the connection list

    if(del_node->next != NULL) {
        del_node->next->prev = del_node->prev;
    }

    close(sd);
    printf("Connection closed for sd %d\n",sd);
    free(del_node);
    FD_CLR(sd, &pool->read_set);
    FD_CLR(sd, &pool->write_set);

    /*
    * 1. remove connection from pool VV
    * 2. deallocate connection
    * 3. remove from sets VV
    * 4. update max_fd if needed VV
    */

    return 0;
}

int push_message(conn_t* node, int len, char* buffer){
    msg_t *msg = malloc(sizeof(msg_t));
    msg->message = malloc(len * sizeof(char));
    strncpy(msg->message, buffer, len);
    msg->message[len] = '\0';

    if(node->write_msg_head == NULL){
        node->write_msg_head = msg;
        node->write_msg_tail = node->write_msg_head;
        return 1;
    }
    node->write_msg_tail->next = msg;
    node->write_msg_tail = node->write_msg_tail->next;
    return 1;
}

int add_msg(int sd, char *buffer, int len, conn_pool_t *pool) {
    if (FD_ISSET(sd, &pool->write_set) == -1) return -1;
    conn_t *curr_node = pool->conn_head;

    while (curr_node != NULL) {
        if (curr_node->fd != sd) {
            push_message(curr_node, len, buffer);
            FD_SET(curr_node->fd, &pool->write_set);
        }
        curr_node = curr_node->next;
    }

    /*
     * 1. add msg_t to write queue of all other connections VV
     * 2. set each fd to check if ready to write VV
     */
    return 0;
}

int write_to_client(int sd, conn_pool_t *pool) {
    conn_t *temp = pool->conn_head;

    while(temp != NULL && temp->fd != sd){
        temp = temp->next;
    }

    while (temp->write_msg_head != NULL) {
        write(temp->fd, temp->write_msg_head->message, strlen(temp->write_msg_head->message));
        msg_t* to_del = temp->write_msg_head;
        temp->write_msg_head = temp->write_msg_head->next;
        free(to_del->message);
        free(to_del);
    }

    FD_CLR(temp->fd, &pool->write_set);

    return 0;
}