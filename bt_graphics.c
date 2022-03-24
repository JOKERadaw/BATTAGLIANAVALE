#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
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