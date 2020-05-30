#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

void kernel_main() {
    isr_install();
    irq_install();

    asm("int $2");
    asm("int $3");

    kprint("Type something, it will go through the kernel\n"
        "Type END to halt the CPU or PAGE to request a kmalloc()\n> ");

    char kern_start_str[16] = "";
    char kern_end_str[16] = "";
    hex_to_ascii(kern_start, kern_start_str);
    hex_to_ascii(kern_end, kern_end_str);
    kprint("kernel in memory start: ");      kprint(kern_start_str);      kprint("\n");
    kprint("kernel in memory end  : ");      kprint(kern_end_str);      kprint("\n");

}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if (strcmp(input, "PAGE") == 0) {
        /* Lesson 22: Code to test kmalloc, the rest is unchanged */
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");

    }
    kprint("You said: ");
    kprintColor("You said: ",RED_ON_WHITE);

    kprint(input);
    kprint("\n> ");
}
