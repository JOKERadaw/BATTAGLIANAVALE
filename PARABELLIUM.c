	
#include <stdio.h>
#include <string.h>	
#include <stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>	
#include <time.h>
#include "multiplayer.h"
#include "bn_graphics.h"
#include "gfx.h"
#include "funzioni.h"

#define CLIENT 1
#define SERVER 0
void  play(int socket,int turno);

int id;

int main(int argc , char *argv[])
{
	int socket_desc;
	int porta;
	int turno;
    char t[2];
	srand(time(NULL));
    if(argc ==2){
	    porta = atoi(argv[1]);
    	socket_desc=connessione_server(porta);
		turno = rand()%2;
        id = SERVER;
        sprintf(t,"%d",turno);
        write(socket_desc,t,1);
    }else if(argc == 3){
	    porta = atoi(argv[2]);
	    socket_desc=connessione_client(argv[1],porta);
        id = CLIENT;
    }else{
        perror("specificare la porta di esecuzione 1025-32000 ...Esempio  $./server 8888(Server)\n");
        perror("specificare indirizzo e porta del server ...Esempio  $./server 127.0.0.1 8888(Client)\n");
		return -1;
    }
    if(id == CLIENT){
        int len =read(socket_desc,t,1);
        t[len] = '\0';
        turno = atoi(t);
    }
    play(socket_desc,turno);
	close(socket_desc);
	return 0;
}


void  play(int socket,int turno) {
	char buffer[2000],cordinate_colpite[4];
    int cord[2];
	int lenb;
	int mouse_pos_x,mouse_pos_y;
	int event = 0;
    int grid[ROW][COLUMN];
    memset(grid,0,ROW*COLUMN*sizeof(int));

    int boats[]={1,1,1,1,2,2,2,3,3,4};
    int size = 0;
    int valido = 0;
    int posiziona =1;
    int r,c;
    int risposta;
    gfx_open(WINDOW_WIDTH,WINDOW_HEIGHT,"Battaglia Navale");
    gfx_color(255,255,255);
    draw_grid(ROW,COLUMN,50);
    set_text("E' il tuo turno premi tasto destro per inserire una nave in verticale e sinistro in orizzontale");
        
   while (event != 'q'){
        if(posiziona == 1){
            event = gfx_wait();
            mouse_pos_x = gfx_xpos();
            mouse_pos_y = gfx_ypos();
            c=xtocolumn(mouse_pos_x);
            r=ytorow(mouse_pos_y)-1;
            if((int)event==1 && size < 10 && posiziona == 1){
                insert_ship(grid,c,r,mouse_pos_x,mouse_pos_y,HORIZONTAL,boats,&size);

            }else if((int)event == 3 && size < 10 && posiziona == 1){
                insert_ship(grid,c,r,mouse_pos_x,mouse_pos_y,VERTICAL,boats,&size);
            }
            
            if(size >= 10 && posiziona == 1){
                set_text("Se sei pronto premi il tasto p");
            }
            if(event == 'p' && size >= 10 && posiziona ==1){
                write(socket,"pronto",6);
                set_text("Aspetta che il tuo avversario finisca di posizionare le navi");
                gfx_flush();
                lenb = read(socket,buffer,1024);
                posiziona = 0;

            }
        }else{
            if(id == turno%2&&risposta!=314159){
                
                set_text("Premi la casella da colpire del nemico");
                event = gfx_wait();
                if((int) event == 1){
		    if(risposta=314159){set_text("hai perso L");break;}
                    while(mouse_pos_x<500){
                    mouse_pos_x = gfx_xpos();
                    mouse_pos_y = gfx_ypos();
                    c=xtocolumn(mouse_pos_x)-10;
                    r=ytorow(mouse_pos_y)-1;
                    sprintf(cordinate_colpite,"%d %d",c,r);
                    printf("%d,%d\n",c,r);
                    }//Verifico se ho cliccato una cella giusta quindi che non sono andato sul mio campo se è successo non mando nulla ma riascolto un nuovo evento
                    
                    write(socket,cordinate_colpite,strlen(cordinate_colpite));
                    turno ++;
                    set_text("Attendi il tuo turno");
                    
                    read(socket,risposta,sizeof(risposta));
                    
                    if(!risposta){drawcross(c+10,y+1,0);set_text("mancato");};
                    else if(risposta){drawcross(c+10,y+1,1);set_text("colpito!");}
                    else{ drawcross(c+10,y+1,1);set_text("affondato!");}
                    if(risposta==314159){set_text("hai vinto GG");break;}
                    gfx_flush();
                }
            }else{
                set_text("Attendi il tuo turno");
                gfx_flush();
                lenb = read(socket,cordinate_colpite,1024);
                cordinate_colpite[lenb] = '\0';
                turno ++;
                sscanf(cordinate_colpite,"%d %d",&cord[0],&cord[1]);
                printf("%d,%d\n",cord[0],cord[1]);
                printf("Ci sono");
                risposta=mattack(grid,boats,cord);
                if(turno>18)
                    if(checkwin(boats))
                        risposta=314159;
                write(socket,risposta,sizeof(risposta));
                set_text("Premi la casella da colpire del nemico");
            }
        }
    }
}
