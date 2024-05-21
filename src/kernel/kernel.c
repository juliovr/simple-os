#include "idt.c"
#include "pic.c"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_SIZE (MAX_ROWS * MAX_COLS)
#define WHITE_ON_BLACK 0x0f

typedef struct {
    int x;
    int y;
} Cursor;

static Cursor cursor;

void init_cursor()
{
    cursor.x = 0;
    cursor.y = 0;
}

void scroll_screen()
{
    // Copy memory to the previous line
    short *video_memory = (short *)VIDEO_ADDRESS;
    int i;
    for (i = 0; i < (SCREEN_SIZE - MAX_COLS); i++) {
        video_memory[i] = video_memory[i + MAX_COLS];
    }

    // Clear any leftover char in the last line
    for (; i < SCREEN_SIZE; i++) {
        video_memory[i] = 0;
    }
}

void set_cursor_next_line()
{
    cursor.x = 0;
    cursor.y++;
    if (cursor.y >= MAX_ROWS) {
        cursor.y = MAX_ROWS - 1; // Set cursor at last line;
        scroll_screen();
    }
}

void advance_cursor()
{
    cursor.x++;
    if (cursor.x >= MAX_COLS) {
        set_cursor_next_line();
    }
}


void print_char_at(char c, int attribute, int row, int col)
{
    short *video_memory = (short *)VIDEO_ADDRESS;
    if (attribute == 0) {
        attribute = WHITE_ON_BLACK;
    }

    int offset = ((row * MAX_COLS) + col);
    video_memory[offset] = (attribute << 8 | c);
}

void print_char(char c)
{
    switch (c)
    {
        case '\n': {
            set_cursor_next_line();
        } break;
        default: {
            print_char_at(c, WHITE_ON_BLACK, cursor.y, cursor.x);
            advance_cursor();
        } break;
    }
}

void kprint(char *string)
{
    while (*string) {
        print_char(*string++);
    }
}

void print_hex(int n)
{
    kprint("0x");
    int size_bits = sizeof(n) * 8;
    int shift = size_bits - 4;
    char c;
    while (shift >= 0) {
        int value = (n >> shift) & 0xF;
        if (value >= 10) {
            c = value + ('A' - 10);
        } else {
            c = value + '0';
        }

        shift -= 4;
        print_char(c);
    }
    print_char('\n');
}

void kprintln(char *string)
{
    kprint(string);
    print_char('\n');
}

void clear_screen()
{
    short *video_memory = (short *)VIDEO_ADDRESS;
    for (int i = 0; i < SCREEN_SIZE; i++) {
        video_memory[i] = 0;
    }
}

void init_video()
{
    init_cursor();
    clear_screen();
}

void bootmain()
{
    init_video();
    
    kprint("Hello World!\n");

    init_idt();

    pic_initialize(0x20, 0x28);
    u16 irr = pic_read_irr();
    u16 isr = pic_read_isr();
    kprint("IRR = ");
    print_hex(irr);
    kprint("ISR = ");
    print_hex(isr);
}
