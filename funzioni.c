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

