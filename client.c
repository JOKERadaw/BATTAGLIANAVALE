#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#define SUNK -1
#define HIT 1
#define MISS 0
#define SQUARE_SIZE 50
#define SPESSORECROCE 4
// disegna una croce dati le coordinate logice (0-20,0-10)=(x,y) rossa=1 bianca=0
void drawcross(int x,int y,int color){
    int z;
    gfx_color(255,255,255);
    if(color)gfx_color(198,3,3);
    for(z=0;z<SPESSORECROCE;z++){
        gfx_line(x*SQUARE_SIZE+z,y*SQUARE_SIZE,x*SQUARE_SIZE+SQUARE_SIZE,y*SQUARE_SIZE+SQUARE_SIZE-z);
        gfx_line(x*SQUARE_SIZE+z,y*SQUARE_SIZE+SQUARE_SIZE,x*SQUARE_SIZE+SQUARE_SIZE,y*SQUARE_SIZE+z);
    }
    for(z=1;z<SPESSORECROCE;z++){
        gfx_line(x*SQUARE_SIZE,y*SQUARE_SIZE+z,x*SQUARE_SIZE+SQUARE_SIZE-z,y*SQUARE_SIZE+SQUARE_SIZE);
        gfx_line(x*SQUARE_SIZE,y*SQUARE_SIZE+SQUARE_SIZE-z,x*SQUARE_SIZE+SQUARE_SIZE-z,y*SQUARE_SIZE);
    }
}
//mattack ritorna se è stata affondata,mancata,beccata
//e aggiorna la nostra matrice in base a questo
//poichè anche noi vediamo quello che l'avversario ha colpito
int mattack(int mat[][20],int barche[],int xy[]){
    if(--barche[mat[xy[0]][xy[1]]]==0){
        drawcross(xy[0],xy[1],1);
        mat[xy[0]][xy[1]]=0;
        return SUNK;
    }else if(barche[mat[xy[0]][xy[1]]]>0){
        drawcross(xy[0],xy[1],1);
        mat[xy[0]][xy[1]]=0;
        return HIT;
    }
    drawcross(xy[0],xy[1],0);
    return MISS;
}
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
	int boats[]={1,1,1,1,2,2,2,3,3,4},mat[11][11]={0};
	posboat(mat,boats);
	int buffer[2];
	int message;
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
