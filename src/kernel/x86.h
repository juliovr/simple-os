#ifndef X86_H

void lidt(void *idtr)
{
    asm volatile("lidt (%0)" : : "r" (idtr));
}

#define X86_H
#endif