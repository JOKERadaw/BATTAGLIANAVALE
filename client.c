#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int connessione_client(char *ip, int porta);
void play (int socket);

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

int main(int argc , char *argv[])
{
	int socket_desc;
	int porta;
	if  (argc !=3 ) {
		perror ("specificare indirizzo e porta del server ...Esempio  $./server 127.0.0.1 8888");
		return -1;
	}
	porta = atoi(argv[2]);
	socket_desc=connessione_client(argv[1],porta);
	play(socket_desc);
	close(socket_desc);
	return 0;
}


void  play(int socket) {
	char buffer[2000];
	char message[] = "PONG";
	int tocca_a_me=0;
	int lenb=0;
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
