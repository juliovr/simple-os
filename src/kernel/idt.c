#include "types.h"
#include "x86.h"

#define IDT_SIZE                    256

#define KERNEL_MODE                 0x0
#define USER_MODE                   0x3
#define GATE_TYPE_TASK              0x5
#define GATE_TYPE_INTERRUPT_32_BIT  0xE
#define GATE_TYPE_TRAP_32_BIT       0xF

#define FLAG_PRESENT                1
#define FLAG_NOT_PRESENT            0

// TODO: find a way to use the same variable in asm and in C,
// as the constant is in the a diferent compilation object (i.e. the bootsector).
#define CODE_SEG 0x8
#define DATA_SEG 0x10

/* ISRs number for IRQs */
#define IRQ_0   32
#define IRQ_1   33
#define IRQ_2   34
#define IRQ_3   35
#define IRQ_4   36
#define IRQ_5   37
#define IRQ_6   38
#define IRQ_7   39
#define IRQ_8   40
#define IRQ_9   41
#define IRQ_10  42
#define IRQ_11  43
#define IRQ_12  44
#define IRQ_13  45
#define IRQ_14  46
#define IRQ_15  47

void print_char(char c);
void kprint(char *string);
void kprintln(char *string);
void print_hex(int n);
void kprint_error(char *string);

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
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;     // pushed by "pusha"
    u32 int_no, errcode;                            // pushed by specific isr
    u32 eip, cs, eflags, useresp, ss;               // pushed by the processor automatically
};


typedef void (*irq_handler_t)(struct Registers *r);
irq_handler_t irq_handlers[IDT_SIZE];

void add_irq_handler(int int_no, irq_handler_t irq_handler)
{
    if (int_no >= 0 && int_no < IDT_SIZE) {
        irq_handlers[int_no] = irq_handler;
    }
}


static char *exception_names[] = {
    "Divide by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void print_registers(struct Registers *r)
{
    kprint("edi = "); print_hex(r->edi);
    kprint("esi = "); print_hex(r->esi);
    kprint("ebp = "); print_hex(r->ebp);
    kprint("esp = "); print_hex(r->esp);
    kprint("ebx = "); print_hex(r->ebx);
    kprint("edx = "); print_hex(r->edx);
    kprint("ecx = "); print_hex(r->ecx);
    kprint("eax = "); print_hex(r->eax);
    kprint("int_no = "); print_hex(r->int_no);
    kprint("errcode = "); print_hex(r->errcode);
    kprint("eip = "); print_hex(r->eip);
    kprint("cs = "); print_hex(r->cs);
    kprint("eflags = "); print_hex(r->eflags);
    kprint("useresp = "); print_hex(r->useresp);
    kprint("ss = "); print_hex(r->ss);
}


void isr_handler(struct Registers *r)
{
    if (r->int_no < IRQ_0) {
        // kprint_error("Error: ");
        // kprint_error(exception_names[r->int_no]);
        // kprint_error("\n");

        irq_handler_t handler = irq_handlers[r->int_no];
        if (handler != 0) {
            handler(r);
        }
    } else {
        kprint("Unhandled exception: ");
        print_hex(r->int_no);
        print_registers(r);
    }
}

void irq_handler(struct Registers *r)
{
    pic_send_eoi(r->int_no - IRQ_0); // The first IRQ has interrupt number 32.

    irq_handler_t handler = irq_handlers[r->int_no];
    if (handler != 0) {
        handler(r);
    }
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

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void handler_int_0(struct Registers *r)
{
    kprint_error("Error: ");
    kprint_error(exception_names[r->int_no]);
    kprint_error("\n");
}

void init_idt()
{
    for (int i = 0; i < IDT_SIZE; i++) {
        GATE_DESCRIPTOR(idt[i], isr_generic_handler, CODE_SEG, FLAG_NOT_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    }

    // ISRs
    GATE_DESCRIPTOR(idt[0],  isr0, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[1],  isr1, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[2],  isr2, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[3],  isr3, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[4],  isr4, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[5],  isr5, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[6],  isr6, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[7],  isr7, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[8],  isr8, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[9],  isr9, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[10], isr10, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[11], isr11, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[12], isr12, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[13], isr13, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[14], isr14, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[15], isr15, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[16], isr16, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[17], isr17, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[18], isr18, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[19], isr19, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[20], isr20, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[21], isr21, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[22], isr22, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[23], isr23, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[24], isr24, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[25], isr25, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[26], isr26, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[27], isr27, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[28], isr28, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[29], isr29, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[30], isr30, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[31], isr31, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);

    // IRQs
    GATE_DESCRIPTOR(idt[32],  irq0, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[33],  irq1, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[34],  irq2, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[35],  irq3, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[36],  irq4, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[37],  irq5, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[38],  irq6, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[39],  irq7, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[40],  irq8, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[41],  irq9, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[42], irq10, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[43], irq11, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[44], irq12, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[45], irq13, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[46], irq14, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);
    GATE_DESCRIPTOR(idt[47], irq15, CODE_SEG, FLAG_PRESENT, KERNEL_MODE, GATE_TYPE_INTERRUPT_32_BIT);

    add_irq_handler(0, handler_int_0);

    // asm instruction throws an error when using 32-bit register, so the offset is
    // splitted into 2 u16 values.
    idtr.size = sizeof(idt) - 1;
    idtr.lo_offset = (u16)idt;
    idtr.hi_offset = ((u16)idt) >> 16;

    lidt(&idtr);
}
