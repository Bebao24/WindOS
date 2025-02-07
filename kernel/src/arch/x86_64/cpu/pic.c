#include "pic.h"
#include <system.h>

#define PIC1_COMMAND_PORT           0x20
#define PIC1_DATA_PORT              0x21
#define PIC2_COMMAND_PORT           0xA0
#define PIC2_DATA_PORT              0xA1

typedef enum
{
	PIC_ICW1_ICW4           = 0x01,
	PIC_ICW1_SINGLE         = 0x02,
	PIC_ICW1_INTERVAL4      = 0x04,
	PIC_ICW1_LEVEL          = 0x08,
	PIC_ICW1_INITIALIZE     = 0x10
} PIC_ICW1;

typedef enum {
    PIC_ICW4_8086           = 0x1,
    PIC_ICW4_AUTO_EOI       = 0x2,
    PIC_ICW4_BUFFER_MASTER  = 0x4,
    PIC_ICW4_BUFFER_SLAVE   = 0x0,
    PIC_ICW4_BUFFERRED      = 0x8,
    PIC_ICW4_SFNM           = 0x10
} PIC_ICW4;

void PIC_Remap(uint8_t offsetPIC1, uint8_t offsetPIC2)
{
	// Initialize word control 1
	x64_outb(PIC1_COMMAND_PORT, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4);
	x64_iowait();
	x64_outb(PIC2_COMMAND_PORT, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4);
	x64_iowait();

	// Initialize word control 2
	x64_outb(PIC1_DATA_PORT, offsetPIC1);
	x64_iowait();
	x64_outb(PIC2_DATA_PORT, offsetPIC2);
	x64_iowait();

	// Initialize word control 3
	x64_outb(PIC1_DATA_PORT, 0x04);
	x64_iowait();
	x64_outb(PIC2_DATA_PORT, 0x02);
	x64_iowait();

	// Initialize word control 4
	x64_outb(PIC1_DATA_PORT, PIC_ICW4_8086);
	x64_iowait();
	x64_outb(PIC2_DATA_PORT, PIC_ICW4_8086);
	x64_iowait();

	// Clear data registers
	x64_outb(PIC1_DATA_PORT, 0);
	x64_iowait();
	x64_outb(PIC2_DATA_PORT, 0);
	x64_iowait();
}
