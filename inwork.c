#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#define WINDOW_WIDTH 1005
#define WINDOW_HEIGHT 550
int mouse_pos_x;
int mouse_pos_y;
void draw_grid(int row,int column,int margin_top);

void main(){
    int c = 0;
    int size = 4;
    gfx_open(WINDOW_WIDTH,WINDOW_HEIGHT,"Battaglia Navale");
    draw_grid(10,20,50);
    gfx_draw_text(0,25,"Ciao",4);
    
    while (c != 'q'){
        c = gfx_wait();
        if((int)c==1){
            mouse_pos_x = gfx_xpos();
            mouse_pos_y = gfx_ypos();
            gfx_draw_ellipse(xtocolumn,mouse_pos_y,50*size,50);
            size--;
        }
    }
    
}

void draw_grid(int row,int column,int margin_top){

    int column_offset = (WINDOW_WIDTH-5) / column;
	int row_offset = (WINDOW_HEIGHT-margin_top) / row;
    int x,y;
	gfx_color(0,200,100);
	for (x=column_offset; x< (WINDOW_WIDTH-5)/2; x = x +column_offset){
		gfx_line(x,margin_top,x, WINDOW_HEIGHT);
	}
    gfx_line_width(5);
	gfx_line(x,margin_top,x, WINDOW_HEIGHT);
    gfx_line_width(1);

    	for (x=column_offset+(WINDOW_WIDTH+5)/2; x< WINDOW_WIDTH; x = x +column_offset){
		gfx_line(x,margin_top,x, WINDOW_HEIGHT);
	}

	for (y=row_offset; y< WINDOW_HEIGHT; y = y +row_offset){
		gfx_line(0,y,WINDOW_WIDTH, y);
	}
}

int ytorow(int y){
    return y / (WINDOW_HEIGHT/10);
}

int xtocolumn(int x){
    return x / (WINDOW_WIDTH/20);
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
