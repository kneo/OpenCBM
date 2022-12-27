#ifndef _CPU_PICO_OFFICIAL_H_
#define _CPU_PICO_OFFICIAL_H_

// Initialize the CPU (clock rate, UART)
static inline void cpu_init(void)
{
    // Disable clock division. This takes us from 1 MHz -> 8 MHz.
    //clock_prescale_set(clock_div_1);

    // Enable watchdog timer and set for 1 second.
    //wdt_enable(WDTO_1S);
}

/*static inline void
cpu_bootloader_start(void)
{
    // XXX This does not work yet, use the hardware buttons
    clock_prescale_set(clock_div_8);
    TCCR1B = 0;
    OCR1A = 0;
    __asm__ __volatile__ ("jmp 0xf000" "\n\t");
}*/

// Timer and delay functions
#define DELAY_MS(x) sleep_ms(x)
#define DELAY_US(x) sleep_us(x)


#endif