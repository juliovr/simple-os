#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_SIZE (MAX_ROWS * MAX_COLS)
#define WHITE_ON_BLACK 0x0f

typedef struct {
    int x;
    int y;
} Cursor;

Cursor cursor;

void print_char_at(char c, int row, int col)
{
    short *video_memory = (short *)VIDEO_ADDRESS;
    int offset = ((row * MAX_COLS) + col);
    video_memory[offset] = (WHITE_ON_BLACK << 8 | c);
}

void clear_screen()
{
    short *video_memory = (short *)VIDEO_ADDRESS;
    for (int i = 0; i < SCREEN_SIZE; i++) {
        video_memory[i] = 0;
    }
}

void bootmain()
{
    clear_screen();
    
    cursor.x = 0;
    cursor.y = 10;
    print_char_at('X', 0, 10);

    // print_char_at('A', cursor.y, cursor.x);
}
