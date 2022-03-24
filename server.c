#include<stdio.h>
#include<string.h>	
#include<stdlib.h>	
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>			//write
void  play(int socket) ;
int connessione_server(int porta);

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

int main(int argc , char *argv[])
{
	int socket_desc;
	int porta;
	if  (argc != 2) {
		perror ("specificare la porta di esecuzione 1025-32000 ...Esempio  $./server 8888");
		return -1;
	}
	porta = atoi(argv[1]);
	socket_desc=connessione_server(porta);
	play(socket_desc);
	close(socket_desc);
	return 0;
}


void  play(int socket) {
	char buffer[2000];
	char message[] = "PING";
	int tocca_a_me=1;
	int lenb = 0;
	for (int i =0; i<10; i++) {
		if (tocca_a_me) {
			sleep(1);
			printf("Invio %s \n", message);
			write(socket , message , strlen(message));
			tocca_a_me = 0;
		}else {
			lenb =read(socket,buffer,100); //Bloccante, attende di ricevere i dati
			buffer[lenb]='\0';
			printf("Letti %d caratteri ---> ricevuto %s ",lenb, buffer);
			tocca_a_me = 1;
		}
	}
}
