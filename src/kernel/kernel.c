
void bootmain()
{
    // char *video_memory = (char *)0xb8000;

    // *video_memory = 'X';
    short *video_memory = (short *)0xb8000;
    for (int i = 0; i < 20; i++) {
        *(video_memory + i) = (0x0f << 8 | 'X');
    }
}
