#define WINDOW_WIDTH 1005
#define WINDOW_HEIGHT 550
#define ROW 10
#define COLUMN 20
#define OFFSET 500 / ROW
#define VERTICAL 3
#define HORIZONTAL 1
void draw_grid(int row,int column,int margin_top);
int ytorow(int y);
int xtocolumn(int x);
int snap_x(int x);
int snap_y(int y);
int controllovalido(int grid[][COLUMN],int size,int v_or_h,int c, int r);
void set_text(char str[]);
int insert_ship(int grid[][COLUMN],int c, int r,int pos_x,int pos_y, int v_or_h,int boats[],int *size);
