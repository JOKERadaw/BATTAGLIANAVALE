#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "bn_graphics.h"


int mouse_pos_x;
int mouse_pos_y;

/*void main(){
    int event = 0;
    int grid[ROW][COLUMN];
    memset(grid,0,ROW*COLUMN*sizeof(int));
    int boats[]={1,1,1,1,2,2,2,3,3,4};
    int size = 0;
    int valido = 0;
    int r,c;

    char message[1024] = "Ciao premi tasto destro per inserire una nave in verticale e sinistro in orizzontale";
    gfx_open(WINDOW_WIDTH,WINDOW_HEIGHT,"Battaglia Navale");
    gfx_color(255,255,255);
    draw_grid(ROW,COLUMN,50);
    gfx_draw_text(0,25,message,strlen(message));

    while (event != 'q'){
        event = gfx_wait();

        mouse_pos_x = gfx_xpos();
        mouse_pos_y = gfx_ypos();
        c=xtocolumn(mouse_pos_x);
        r=ytorow(mouse_pos_y)-1;
        //Posizionamento Navi
        if((int)event==1 && size < 10){
            valido = controllovalido(grid,boats[size],HORIZONTAL,c,r);
            if(valido != 0){
                
                gfx_draw_ellipse(snap_x(mouse_pos_x),snap_y(mouse_pos_y),OFFSET*boats[size],OFFSET);
                size++;
                for(int z=0;z<boats[size-1];z++)grid[r][c+z]=size;
                set_text("");
            }else{
                set_text("Posizione non valida");
            }

        }else if((int)event == 3 && size < 10){
            valido = controllovalido(grid,boats[size],VERTICAL,c,r);
            if(valido != 0){
                gfx_draw_ellipse(snap_x(mouse_pos_x),snap_y(mouse_pos_y),OFFSET,OFFSET*boats[size]);
                size++;
                for(int z=0;z<boats[size-1];z++)grid[r+z][c]=size;
                set_text("");

            }else{
                set_text("Posizione non valida");
            }
        }
        
        if(size >= 10){
            set_text("Se sei pronto premi il tasto p");
        }
        if(event == 'p' && size >= 10){
            break;
        }


        for(int y=0;y<10;y++){
            for(int x=0;x<10;x++){
                printf("%2d ",grid[y][x]);
            }
            printf("\n");
        }
        printf("\n");

    }
    
}
*/

int controllovalido(int grid[][COLUMN],int size,int v_or_h,int c, int r){
    int i = 0;
    int valido = 1;
    if(c >= (COLUMN/2)){
        valido = 0;
    }
    else if(c >= (COLUMN/2)-(size-1) && v_or_h == HORIZONTAL){
        valido = 0;
    }else if(r >= (ROW)-(size-1) && v_or_h == VERTICAL){
        valido = 0;
    }

    if(v_or_h == VERTICAL){
        while (i < size && valido != 0){
            if(grid[r+i][c] > 0){
                valido = 0;

            }

            i++;
        }
        
    }else{
        while (i < size && valido != 0){
            if(grid[r][c+i] > 0){
                valido = 0;
            }
            i++;
        }
    }
    return valido;
}

void draw_grid(int row,int column,int margin_top){

    int column_offset = (WINDOW_WIDTH-5) / column;
	int row_offset = (WINDOW_HEIGHT-margin_top) / row;
    int x,y;
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
    return y / ((WINDOW_HEIGHT-OFFSET)/ROW);
}

int xtocolumn(int x){
    return x / ((WINDOW_WIDTH-5)/COLUMN);
}

int snap_x(int x){
    return xtocolumn(x) * ((WINDOW_WIDTH-5)/COLUMN);
}

int snap_y(int y){
    return ytorow(y) * ((WINDOW_HEIGHT-OFFSET)/ROW);
}

void set_text(char str[]){
    gfx_color(0,0,0);
    gfx_line_width(OFFSET);
    gfx_line(0,25,WINDOW_WIDTH,25);
    gfx_color(255,255,255);
    gfx_draw_text(0,25,str,strlen(str));

}

int insert_ship(int grid[][COLUMN],int c, int r,int pos_x,int pos_y, int v_or_h,int boats[],int *size){
    int valido;
    if(v_or_h == HORIZONTAL){
        valido = controllovalido(grid,boats[*size],HORIZONTAL,c,r);
        if(valido != 0){
            gfx_draw_ellipse(snap_x(pos_x),snap_y(pos_y),OFFSET*boats[*size],OFFSET);
            (*size)++;
            for(int z=0;z<boats[*size-1];z++)grid[r][c+z]=*size;
            set_text("E' il tuo turno premi tasto destro per inserire una nave in verticale e sinistro in orizzontale");
        }else{
            set_text("Posizione non valida");
        }
    }else{
        valido = controllovalido(grid,boats[*size],VERTICAL,c,r);
        printf("%d,%d",pos_x,pos_y);
        if(valido != 0){
            gfx_draw_ellipse(snap_x(pos_x),snap_y(pos_y),OFFSET,OFFSET*boats[*size]);
            (*size)++;
            for(int z=0;z<boats[*size-1];z++)grid[r+z][c]=*size;
            set_text("");
        }else{
            set_text("Posizione non valida");
        }
    }
}