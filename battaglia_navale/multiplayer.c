#include<stdio.h>
#include<string.h>	
#include<stdlib.h>	
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "gfx.h"
#define MARGIN 50

int connessione_server(int porta) {
	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( porta);
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
	}
	puts("bind done");
	//Listen
	listen(socket_desc , 3);
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);  //Bloccante, attende di ricevere una connessione
	if (new_socket<0)
	{
		perror("accept failed");
	}
	puts("Connection accepted");
	return new_socket;

}

int connessione_client(char *ip, int porta) {
	int socket_desc;
	struct sockaddr_in server;
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons( porta);
	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) 
	{
		puts("connect error");
		return 1;
	}
	puts("Connected\n");
	return socket_desc;
}