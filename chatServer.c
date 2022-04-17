#include "chatServer.h"


static int end_server = 0;

void intHandler(int SIG_INT) {
	/* use a flag to end_server to break the main loop */
}

int main (int argc, char *argv[])
{
	
	signal(SIGINT, intHandler);
   
	conn_pool_t* pool = malloc(sizeof(conn_pool_t));
	init_pool(pool);
   
	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming      */
	/* connections on                                            */
	/*************************************************************/
	socket(...);
   
   
	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	ioctl(...);
	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	bind(...);

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	listen(...);

	/*************************************************************/
	/* Initialize fd_sets  			                             */
	/*************************************************************/
	
	/*************************************************************/
	/* Loop waiting for incoming connects, for incoming data or  */
	/* to write data, on any of the connected sockets.           */
	/*************************************************************/
	do
	{
		/**********************************************************/
		/* Copy the master fd_set over to the working fd_set.     */
		/**********************************************************/
		
		/**********************************************************/
		/* Call select() 										  */
		/**********************************************************/
		select(...);

		
		/**********************************************************/
		/* One or more descriptors are readable or writable.      */
		/* Need to determine which ones they are.                 */
		/**********************************************************/
		
		for (/* check all descriptors, stop when checked all valid fds */)
		{
			/* Each time a ready descriptor is found, one less has  */
			/* to be looked for.  This is being done so that we     */
			/* can stop looking at the working set once we have     */
			/* found all of the descriptors that were ready         */
				
			/*******************************************************/
			/* Check to see if this descriptor is ready for read   */
			/*******************************************************/
			if (FD_ISSET(...)
			{
				/***************************************************/
				/* A descriptor was found that was readable		   */
				/* if this is the listening socket, accept one      */
				/* incoming connection that is queued up on the     */
				/*  listening socket before we loop back and call   */
				/* select again. 						            */
				/****************************************************/
                accept(...)
				/****************************************************/
				/* If this is not the listening socket, an 			*/
				/* existing connection must be readable				*/
				/* Receive incoming data his socket             */
				/****************************************************/
				read(...)
				/* If the connection has been closed by client 		*/
                /* remove the connection (remove_conn(...))    		*/
							
				/**********************************************/
				/* Data was received, add msg to all other    */
				/* connectios					  			  */
				/**********************************************/
                add_msg(...);
		                  
				
			} /* End of if (FD_ISSET()) */
			/*******************************************************/
			/* Check to see if this descriptor is ready for write  */
			/*******************************************************/
			if (FD_ISSET(...) {
				/* try to write all msgs in queue to sd */
				write_to_client(...);
		 	}
		 /*******************************************************/
		 
		 
      } /* End of loop through selectable descriptors */

   } while (end_server == FALSE);

	/*************************************************************/
	/* If we are here, Control-C was typed,						 */
	/* clean up all open connections					         */
	/*************************************************************/
	
	return 0;
}


int init_pool(conn_pool_t* pool) {
	//initialized all fields
	return 0;
}

int add_conn(int sd, conn_pool_t* pool) {
	/*
	 * 1. allocate connection and init fields
	 * 2. add connection to pool
	 * */
	return 0;
}


int remove_conn(int sd, conn_pool_t* pool) {
	/*
	* 1. remove connection from pool 
	* 2. deallocate connection 
	* 3. remove from sets 
	* 4. update max_fd if needed 
	*/
	
	return 0;
}

int add_msg(int sd,char* buffer,int len,conn_pool_t* pool) {
	
	/*
	 * 1. add msg_t to write queue of all other connections 
	 * 2. set each fd to check if ready to write 
	 */
	
	return 0;
}

int write_to_client(int sd,conn_pool_t* pool) {
	
	/*
	 * 1. write all msgs in queue 
	 * 2. deallocate each writen msg 
	 * 3. if all msgs were writen successfully, there is nothing else to write to this fd... */
	
	return 0;
}

