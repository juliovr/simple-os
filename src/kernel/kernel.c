#include "idt.c"
#include "pic.c"
#include "pit.c"

#define VIDEO_ADDRESS       0xb8000
#define MAX_ROWS            25
#define MAX_COLS            80
#define SCREEN_SIZE         (MAX_ROWS * MAX_COLS)
#define WHITE_ON_BLACK      0x0f
#define RED_ON_BLACK        0x04
#define CURSOR_CHAR         '_'

typedef struct {
    int x;
    int y;
} Cursor;

static Cursor cursor;
static u8 text_color;

void init_cursor()
{
    cursor.x = 0;
    cursor.y = 0;
}

void scroll_screen()
{
    // Copy memory to the previous line
    u16 *video_memory = (u16 *)VIDEO_ADDRESS;
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
    clear_char(cursor.y, cursor.x);
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

void backspace_cursor()
{
    clear_char(cursor.y, cursor.x);

    cursor.x--;
    if (cursor.x < 0) {
        cursor.x = 0;
        if (cursor.y > 0) {
            cursor.y--;

            // Find the last character on previous line
            u16 *video_memory = (u16 *)VIDEO_ADDRESS;
            int i;
            for (i = 0; i < MAX_COLS; i++) {
                int offset = ((cursor.y * MAX_COLS) + i);
                if (video_memory[offset] == 0) {
                    break;
                }
            }

            cursor.x = i;
        }
        
    }

    print_cursor(CURSOR_CHAR);
}


void print_char_at(char c, int row, int col)
{
    u16 *video_memory = (u16 *)VIDEO_ADDRESS;

    int offset = ((row * MAX_COLS) + col);
    video_memory[offset] = (text_color << 8 | c);
}

void print_cursor(char c)
{
    print_char_at(c, cursor.y, cursor.x);
}

void clear_char(int row, int col)
{
    u16 *video_memory = (u16 *)VIDEO_ADDRESS;
    int offset = ((cursor.y * MAX_COLS) + cursor.x);
    video_memory[offset] = 0;
}

void kprint_char(char c)
{
    switch (c)
    {
        case '\n': {
            set_cursor_next_line();
        } break;
        default: {
            print_char_at(c, cursor.y, cursor.x);
            advance_cursor();
        } break;
    }

    print_cursor(CURSOR_CHAR);
}

void print_string(char *string)
{
    while (*string) {
        kprint_char(*string++);
    }
}

void kprint(char *string)
{
    text_color = WHITE_ON_BLACK;
    print_string(string);
}

void kprint_error(char *string)
{
    text_color = RED_ON_BLACK;
    print_string(string);
}

void print_hex(int n)
{
    text_color = WHITE_ON_BLACK;
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
        kprint_char(c);
    }
    kprint_char('\n');
}

void kprintln(char *string)
{
    text_color = WHITE_ON_BLACK;
    kprint(string);
    kprint_char('\n');
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
    
    kprintln("Welcome to my simple kernel!");

    init_idt();

    sti();

    pic_initialize(0x20, 0x28);
    pic_mask(0);
    pic_unmask(1);

    init_pit(82); // 82 divides perfectly the PIT freq.

    kprintln("Test interrupt. Raise int 0:");
    INTERRUPT(0);

    kprintln("final!");
}
