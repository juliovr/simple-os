#include "types.h"
#include "x86.h"

#define IDT_SIZE 256

#define KERNEL_MODE 0x0
#define USER_MODE 0x3
#define GATE_TYPE_TASK 0x5
#define GATE_TYPE_INTERRUPT_32_BIT 0xE
#define GATE_TYPE_TRAP_32_BIT 0xF

// TODO: find a way to use the same variable in asm and in C,
// as the constant is in the a diferent compilation object (i.e. the bootsector).
#define CODE_SEG 0x8
#define DATA_SEG 0x10

void print_char(char c);
void kprint(char *string);
void kprintln(char *string);
void print_hex(int n);

struct GateDescriptor {
    u16 offset_0_15;
    u16 segment_selector;
    u8 reserved;
    u8 attributes; // P, DPL and Gate type
    u16 offset_16_31;
};

struct GateDescriptor idt[IDT_SIZE];

#define GATE_DESCRIPTOR(gate, offset, segment, p, dpl, type)                       \
{                                                                                  \
    (gate).offset_0_15 = (((u32)(offset)) & 0xffff);                               \
    (gate).segment_selector = (segment);                                           \
    (gate).reserved = 0;                                                           \
    (gate).attributes = ((p) << 7) | (((dpl) & 0x3) << 5) | 0 | ((type) & 0xf);    \
    (gate).offset_16_31 = (((u32)(offset) >> 16) & 0xffff);                        \
}

#pragma pack(push,1)
struct IDTR {
    u16 size;
    u16 lo_offset;
    u16 hi_offset;
};
#pragma pack(pop)

struct IDTR idtr;

struct Registers {
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by "pusha"
    u32 int_no, errcode;                         // pushed by specific isr
    u32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

void print_registers(struct Registers r)
{
    kprint("edi = "); print_hex(r.edi);
    kprint("esi = "); print_hex(r.esi);
    kprint("ebp = "); print_hex(r.ebp);
    kprint("esp = "); print_hex(r.esp);
    kprint("ebx = "); print_hex(r.ebx);
    kprint("edx = "); print_hex(r.edx);
    kprint("ecx = "); print_hex(r.ecx);
    kprint("eax = "); print_hex(r.eax);
    kprint("int_no = "); print_hex(r.int_no);
    kprint("errcode = "); print_hex(r.errcode);
    kprint("eip = "); print_hex(r.eip);
    kprint("cs = "); print_hex(r.cs);
    kprint("eflags = "); print_hex(r.eflags);
    kprint("useresp = "); print_hex(r.useresp);
    kprint("ss = "); print_hex(r.ss);
}

void isr_handler(struct Registers r)
{
    print_registers(r);
    kprint("from interrupt_handler!\n");
    for (;;) ;
}

extern void isr_generic_handler();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void init_idt()
{
    kprint("init_idt\n");

    for (int i = 0; i < IDT_SIZE; i++) {
        GATE_DESCRIPTOR(idt[i], isr_generic_handler, CODE_SEG, 0, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    }

    GATE_DESCRIPTOR(idt[0],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[1],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[2],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[3],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[4],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[5],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[6],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[7],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[8],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[9],  isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[10], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[11], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[12], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[13], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[14], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[15], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[16], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[17], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[18], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[19], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[20], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[21], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[22], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[23], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[24], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[25], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[26], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[27], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[28], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[29], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[30], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[31], isr0, CODE_SEG, 1, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);


    // asm instruction throws an error when using 32-bit register, so the offset is
    // splitted into 2 u16 values.
    idtr.size = sizeof(idt) - 1;
    idtr.lo_offset = (u16)idt;
    idtr.hi_offset = ((u16)idt) >> 16;

    lidt(&idtr);
}

