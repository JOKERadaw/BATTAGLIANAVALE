#include<stdio.h>
#include<string.h>	
#include<stdlib.h>	
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>			//write
void  play(int socket) ;
int connessione_server(int porta);
#include <stdlib.h>
#include "gfx.h"
#define WINDOW_WIDTH 1005
#define WINDOW_HEIGHT 550
#include<stdio.h>
int mouse_pos_x;
int mouse_pos_y;
void draw_grid(int row,int column,int margin_top);
int ytorow(int y);
int xtocolumn(int x);
int snap_x(int x);
int snap_y(int y);
int controllavalido(int size,int mat[][11],int x,int y,int orientazione);
void posboat(int mat[][11],boats[]){
    //int boats[]={1,1,1,1,2,2,2,3,3,4},mat[11][11]={0};
    int c = 0;
    int size = 0;
    int z,x,y;
    for(x=0;x<10;x++){mat[10][x]=1;mat[x][10]=1;}

    gfx_open(WINDOW_WIDTH,WINDOW_HEIGHT,"Battaglia Navale");
    draw_grid(10,20,50);
    gfx_draw_text(0,25,"Ciao",4);

    while (c != 'q'&&size<10){
        c = gfx_wait();
        mouse_pos_x = gfx_xpos();
        mouse_pos_y = gfx_ypos();
        x=xtocolumn(mouse_pos_x);
        y=ytorow(mouse_pos_y)-1;
        if((int)c==1){
            if(mouse_pos_x < WINDOW_WIDTH/2-5&&controllavalido(boats[size],mat,x,y,1)){
                gfx_draw_ellipse(snap_x(mouse_pos_x),snap_y(mouse_pos_y),50,50*boats[size]);
                size++;
                for(z=0;z<boats[size-1];z++)mat[y+z][x]=size;
            }
        }else if((int)c==3){
            if(mouse_pos_x < WINDOW_WIDTH/2-5&&controllavalido(boats[size],mat,x,y,0)){
                gfx_draw_ellipse(snap_x(mouse_pos_x),snap_y(mouse_pos_y),50*boats[size],50);
                size++;
                for(z=0;z<boats[size-1];z++)mat[y][x+z]=size;
            }
        }
        for(y=0;y<10;y++){
            for(x=0;x<10;x++){
                printf("%d ",mat[y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
int controllavalido(int size,int mat[][11],int x,int y,int orientazione){
    int valido=1,z;
    if(orientazione){
        for(z=0;z<size;z++){
            if(mat[y+z][x]>0){
                gfx_line_width(22);
                gfx_color(0,0,0);
                gfx_line(0,25,WINDOW_WIDTH,25 );
                gfx_color(255,255,255);
                gfx_draw_text(0,25,"POSIZIONE NON VALIDA",20);
                gfx_line_width(1);
                valido=0;
                break;
            }
        }
    }else{
        for(z=0;z<size;z++){
            if(mat[y][x+z]>0){
                gfx_line_width(22);
                gfx_color(0,0,0);
                gfx_line(0,25,WINDOW_WIDTH,25 );
                gfx_color(255,255,255);
                gfx_draw_text(0,25,"POSIZIONE NON VALIDA",20);
                gfx_line_width(1);
                valido=0;
                gfx_color(255,255,255);
                break;
            }
        }
    }
    if (valido){

           gfx_line_width(22);
           gfx_color(0,0,0);
           gfx_line(0,25,WINDOW_WIDTH,25 );
           gfx_color(255,255,255);
           gfx_line_width(1);
    }
    return valido;
}

void draw_grid(int row,int column,int margin_top){

    int column_offset = (WINDOW_WIDTH-5) / column;
    int row_offset = (WINDOW_HEIGHT-margin_top) / row;
    int x,y;
    gfx_color(255,255,255);
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
    return y / ((WINDOW_HEIGHT-50)/10);
}

int xtocolumn(int x){
    return x / ((WINDOW_WIDTH-5)/20);
}

int snap_x(int x){
    return xtocolumn(x) * ((WINDOW_WIDTH-5)/20);
}

int snap_y(int y){
    return ytorow(y) * ((WINDOW_HEIGHT-50)/10);
}
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
	int boats[]={1,1,1,1,2,2,2,3,3,4},mat[11][11]={0};
	posboat(mat,boats);
	int buffer[2];
	int message;
	int tocca_a_me=1;
	for (int i =0; i<10; i++) {
		if (tocca_a_me) {
			sleep(1);
			mouse_pos_x = gfx_xpos();
        		mouse_pos_y = gfx_ypos();
        		x=xtocolumn(mouse_pos_x)-10;
        		y=ytorow(mouse_pos_y)-1;//-1 è il 50 di padding che abbiamo in cima 
			buffer[0]=x;buffer[1]=y;
			write(socket , buffer , 2);
			tocca_a_me = 0;
		}else {
			message=read(socket,message,1); //Bloccante, attende di ricevere i dati
			if(!message)drawcross(x+10,y,0);
			else drawcross(x+10,y,1);
			printf("Letti %d caratteri ---> ricevuto %s ",lenb, buffer);
			tocca_a_me = 1;
		}
	}
}
