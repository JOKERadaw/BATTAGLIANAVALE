	
	char buffer[2000],cordinate_colpite[4],risposta[1024];
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
            if(id == turno){
                set_text("Premi la casella da colpire del nemico");
                event = gfx_wait();
                if((int) event == 1){
                    mouse_pos_x = gfx_xpos();
                    mouse_pos_y = gfx_ypos();
                    c=xtocolumn(mouse_pos_x);
                    r=ytorow(mouse_pos_y);
                    if(c >= 10 && r > 0){
                        sprintf(cordinate_colpite,"%d %d",c,r);
                        printf("%d,%d\n",c,r);
                        gfx_line_width(1);
                        write(socket,cordinate_colpite,strlen(cordinate_colpite));
                        turno = !turno;
                        lenb = read(socket,risposta,1024);
                        risposta[lenb] = '\0';
						if(risposta[0]=='M')drawcross(c,r,0);
						else drawcross(c,r,1);
						set_text(risposta);
                        gfx_flush();
                    }
                }
            }else{
                lenb = read(socket,cordinate_colpite,1024);
                cordinate_colpite[lenb] = '\0';
                turno = !turno;
                sscanf(cordinate_colpite,"%d %d",&cord[0],&cord[1]);
                if(grid[cord[1] - 1][cord[0]-10] > 0){
                    strcpy(risposta,"Colpita");
                }else{
                    strcpy(risposta,"Mancata");
                }
				for(int x=0;x<20;x++){
				
					for(int y=0;y<10;y++){
						printf("%d ",grid[x][y]);
					}
					printf("\n");
				}
                write(socket,risposta,strlen(risposta));

                set_text("Premi la casella da colpire del nemico");
            }
        }

        
    }
}
