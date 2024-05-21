#ifndef X86_H

static inline 
void lidt(void *idtr)
{
    asm volatile("lidt (%0)" : : "r" (idtr));
}

static inline 
void outb(u16 port, u8 data)
{
    asm volatile("out %0, %1" : : "a" (data), "Nd" (port));
}

static inline 
void outw(u16 port, u16 data)
{
    asm volatile("out %0, %1" : : "a" (data), "Nd" (port));
}

static inline
u8 inb(u16 port)
{
    u8 data;
    asm volatile("in %1, %0": "=a" (data) : "Nd" (port));

    return data;
}

static inline
u16 inw(u16 port)
{
    u16 data;
    asm volatile("in %1, %0": "=a" (data) : "Nd" (port));

    return data;
}

/*
 * Wait a very small amount of time (1 to 4 microseconds, generally).
 * Useful for implementing a small delay for PIC remapping on old hardware or generally as a simple but imprecise wait. 
 * You can do an IO operation on any unused port: the Linux kernel by default uses port 0x80, 
 *which is often used during POST to log information on the motherboard's hex display but almost always unused after boot. 
 */
static inline
void io_wait()
{
    outb(0x80, 0);
}

#define X86_H
#endif