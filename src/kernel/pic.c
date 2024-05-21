/*
 * Code for Programmable Interrupt Controller (PIC) to handle hardware interrupts.
 * The chip is compose of 2 PIC:
 *     - Master PIC (PIC1)
 *     - Slave PIC  (PIC2)
 *
 * Command word of 8259 is divided into two parts :
 *     - Initialization Command Words (ICW), 4 in total.
 *     - Operating Command Words (OCW), 3 in total.
 *
 */

#define PIC1            0x20        /* IO base address for master PIC */
#define PIC2            0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND    (PIC1)      /* Command port for master PIC */
#define PIC1_DATA       (PIC1 + 1)  /* Data port for master PIC */
#define PIC2_COMMAND    (PIC2)      /* Command port for slave PIC */
#define PIC2_DATA       (PIC2 + 1)  /* Data port for slave PIC */

#define PIC_EOI         0x20        /* End-Of-Interrupt command code */

#define ICW4_8086       0x01

#define OCW3_READ_IRR   0x0A
#define OCW3_READ_ISR   0x0B

/*
 * Send End-Of-Interrupt.
 * This is issued to the PIC chips at the end of an IRQ-based interrupt routine.
 */
void pic_send_eoi(u8 irq)
{
    // If the IRQ comes from the slave PIC, it is neccesary to issue the command to both PIC chips.
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

/*
 * In protected mode, the IRQs 0 to 7 conflict with the CPU exception which are reserved by Intel up until 0x1F.
 * So, a remapping is needed to adjust the beginning offset.
 * There are offset arguments for master and slave PICs separately.
 *
 * It consists of 4 ICW:
 *     ICW1: Issue the initialization command to both PICs (code 0x11).
 *     ICW2: Set the vector offset.
 *     ICW3: How it is wired to master/slave.
 *     ICW4: Additional information about the environment.
*/
void pic_initialize(int offset_master, int offset_slave)
{
    // Save masks
    u8 mask_master = inb(PIC1_DATA);
    u8 mask_slave = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);       // ICW1 on master
    io_wait();
    outb(PIC2_COMMAND, 0x11);       // ICW1 on slave
    io_wait();
    outb(PIC1_DATA, offset_master); // ICW2 on master
    io_wait();
    outb(PIC2_DATA, offset_slave);  // ICW2 on slave
    io_wait();
    outb(PIC1_DATA, 4);             // ICW3: tell master PIC that there is a slave PIC at IRQ2 (0000 0100).
    io_wait();
    outb(PIC2_DATA, 2);             // ICW3: tell slave PIC its cascade identity (0000 0010).
    io_wait();

    outb(PIC1_DATA, ICW4_8086);     // ICW4: have the PICs use 8086 mode instead of 8080.
    io_wait();
    outb(PIC2_DATA, ICW4_8086);     // ICW4: have the PICs use 8086 mode instead of 8080.
    io_wait();

    // Restore masks
    outb(PIC1_DATA, mask_master);
    outb(PIC2_DATA, mask_slave);
}


/*
 * Helper function. 
 */
u16 __pic_read_register(int ocw3)
{
    outb(PIC1_COMMAND, 0x0A);
    outb(PIC2_COMMAND, 0x0A);
    return ((inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND));
}
/*
 * Read the Interrupt Request Register (IRR).
 * As IRR is 8 bit, the high 8 bits are from slave PIC and low 8 bits are from master PIC.
 */
u16 pic_read_irr()
{
    return __pic_read_register(OCW3_READ_IRR);
}

/*
 * Read the In-Service Register (ISR).
 * As ISR is 8 bit, the high 8 bits are from slave PIC and low 8 bits are from master PIC.
 */
u16 pic_read_isr()
{
    return __pic_read_register(OCW3_READ_ISR);
}
