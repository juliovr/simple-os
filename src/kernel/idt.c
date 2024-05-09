#include "types.h"

#define IDT_SIZE 256

// TODO: find a way to use the same variable in asm and in C,
// as the constant is in the a diferent compilation object (i.e. the bootsector).
#define CODE_SEG 0x8

#define USER_MODE 0x3
#define INTERRUPT_GATE_32_BIT 0xE

void print_char(char c);
void print_string(char *string);

struct GateDescriptor {
    u16 offset_0_15;
    u16 segment_selector;
    u8 reserved;
    u8 attributes; // P, DPL and Gate type
    u16 offset_16_31;
};

extern u8 interrupt_data[];
extern int interrupt_service_routine;
struct GateDescriptor idt[IDT_SIZE];

#define GATE_DESCRIPTOR(gate, offset, selector, p, dpl, type)                       \
{                                                                                   \
    (gate).offset_0_15 = ((offset) & 0xffff);                                       \
    (gate).segment_selector = (selector);                                           \
    (gate).reserved = 0;                                                            \
    (gate).attributes = ((p) << 7) | (((dpl) & 0x3) << 5) | 0 | ((type) & 0xf);     \
    (gate).offset_16_31 = ((offset >> 16) & 0xffff);                                \
}

void lidt(struct GateDescriptor *idt, int size)
{
    struct IDTR {
        u16 size;
        u16 offset1;
        u16 offset2;
    };
    // volatile u16 p[3];
    // p[0] = size - 1;
    // p[1] = (u16)idt;
    // p[2] = ((u16)idt) >> 16;
    // asm volatile("lidt (%0)" : : "r" (p));

    volatile struct IDTR idtr;
    idtr.size = size - 1;
    idtr.offset1 = (u16)idt;
    idtr.offset2 = ((u16)idt) >> 16;

    asm volatile("lidt (%0)" : : "r" (&idtr));
}

void init_idt()
{
    print_string("init_idt\n");
    
    // Print array data defined in asm.
    // for (int i = 0; i < 3; i++) {
    //     u8 data = interrupt_data[i];
    //     print_char('0' + data);
    // }

    for (int i = 0; i < IDT_SIZE; i++) {
        GATE_DESCRIPTOR(idt[i], interrupt_service_routine, CODE_SEG, 0, USER_MODE, INTERRUPT_GATE_32_BIT);
    }

    // Division by 0
    GATE_DESCRIPTOR(idt[0], interrupt_service_routine, CODE_SEG, 1, USER_MODE, INTERRUPT_GATE_32_BIT);


    lidt(idt, sizeof(idt));
}

void interrupt_handler()
{
    print_string("from interrupt_handler!\n");
}
