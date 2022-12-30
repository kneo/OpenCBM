#ifndef _CPU_PICO_OFFICIAL_H_
#define _CPU_PICO_OFFICIAL_H_

// Initialize the CPU (clock rate, UART)
static inline void cpu_init(void)
{
    //TODO:
    //-initialize CPU core 2,
    //-initialize timer on core 2,
    //-

    // Disable clock division. This takes us from 1 MHz -> 8 MHz.
    //clock_prescale_set(clock_div_1);

    // Enable watchdog timer and set for 1 second.
    //wdt_enable(WDTO_1S);
}

// Timer and delay functions
#define DELAY_MS(x) sleep_ms(x)
#define DELAY_US(x) sleep_us(x)


#endif