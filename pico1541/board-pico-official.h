#ifndef _BOARD_PICO_OFFICIAL_H_
#define _BOARD_PICO_OFFICIAL_H_

// Initialize the board (timer, indicators, UART)
void board_init(void);
// Initialize the IO ports for IEC mode
void board_init_iec(void);

//We need map the GPIOs to a virtual 8 Bit Port register of some kind, 
//since the PI Pico does not have "ports" to speak of.
//it has 32 pins, however in order to provide flexibility, 
//the GPIOs shall be configurable:
#define GPIO_PIN_ATN          (2)
#define GPIO_PIN_CLK          (3)
#define GPIO_PIN_DATA         (4)
#define GPIO_PIN_RESET        (5)
#define GPIO_PIN_SRQ          (6)

#define IO_PIN_COUNT (5)

//while these pins must be unchanged,
//since they represent the bit position used through out the library!
#define VIO_PIN_ATN          (0)
#define VIO_PIN_CLK          (1)
#define VIO_PIN_DATA         (2)
#define VIO_PIN_RESET        (3)
#define VIO_PIN_SRQ          (4)

#define INLINE          static inline __attribute__((always_inline))

struct PICO_IO_MAPPING
{
    uint8_t gpio;
    uint8_t vio;
};

static struct PICO_IO_MAPPING global_pin_lookup_table[IO_PIN_COUNT] =   
{
    [VIO_PIN_ATN] = {
        .gpio = GPIO_PIN_ATN,
        .vio = VIO_PIN_ATN
    },
    [VIO_PIN_CLK]={
        .gpio = GPIO_PIN_CLK,
        .vio = VIO_PIN_CLK
    },
    [VIO_PIN_DATA]={
        .gpio = GPIO_PIN_DATA,
        .vio = VIO_PIN_DATA
    },
    [VIO_PIN_RESET]={
        .gpio = GPIO_PIN_RESET,
        .vio = VIO_PIN_RESET
    },
    [VIO_PIN_SRQ]={
        .gpio = GPIO_PIN_SRQ,
        .vio = VIO_PIN_SRQ
    }
};

#define IS_SIGNAL_SET(register8bit,io_signal) ((register8bit&io_signal)==io_signal)

/*
 * Mapping of iec lines to IO port signals.
 *
 * NOTE: the XAP1541 adapter Nate is using has separate I/O pins
 * for inputs and outputs, so we depend on the IN signal bits being
 * the OUT signal << 1.
 *
 * The below defined pins are the OUT signals only but we derive the
 * input pin numbers from them.
 */
#define IO_ATN          (1 << VIO_PIN_ATN)
#define IO_CLK          (1 << VIO_PIN_CLK)
#define IO_DATA         (1 << VIO_PIN_DATA)
#define IO_RESET        (1 << VIO_PIN_RESET)
#define IO_SRQ          (1 << VIO_PIN_SRQ)

#define IO_INDEFINED    (0xE0) //not supported lines!

#define IO_OUTPUT_MASK  (IO_ATN | IO_CLK | IO_DATA | IO_RESET | IO_SRQ)

// IEC and parallel port accessors
//#define CBM_PORT        PORTA
//#define CBM_DDR         DDRA
//#define CBM_PIN         PINA

//#define PAR_PORT_PORT   PORTC
//#define PAR_PORT_DDR    DDRC
//#define PAR_PORT_PIN    PINC

/*
 * Use always_inline to override gcc's -Os option. Since we measured each
 * inline function's disassembly and verified the size decrease, we are
 * certain when we specify inline that we really want it.
 */
//#define INLINE          static inline __attribute__((always_inline))

/*
 * Routines for getting/setting individual IEC lines and parallel port.
 *
 * We no longer add a short delay after changing line(s) state, even though
 * it takes about 0.5 us for the line to stabilize (measured with scope).
 * This is because we need to toggle SRQ quickly to send data to the 1571
 * and the delay was breaking our deadline.
 *
 * These are all inlines and this was incrementally measured that each
 * decreases the firmware size. Some (set/get) compile into a single
 * instruction (say, sbis). This works because the "line" argument is
 * almost always a constant.
 */

// INLINE void iec_set(uint8_t line)
// {
//     //uint32_t mask = 
//     //CBM_PORT |= line;
// }

// INLINE void iec_release(uint8_t line)
// {
//     //CBM_PORT &= ~line;
// }

// INLINE void iec_set_release(uint8_t s, uint8_t r)
// {
//     //CBM_PORT = (CBM_PORT & ~r) | s;
// }

// INLINE uint8_t iec_get(uint8_t line)
// {
//     //return ((CBM_PIN >> 1) & line) == 0 ? 1 : 0;
//     return 0;
// }

// /*INLINE uint8_t iec_pp_read(void)
// {
//     PAR_PORT_DDR = 0;
//     PAR_PORT_PORT = 0;
//     return PAR_PORT_PIN;
//     return 0;
// }

// INLINE void iec_pp_write(uint8_t val)
// {
//     PAR_PORT_DDR = 0xff;
//     PAR_PORT_PORT = val;
// }*/

// INLINE uint8_t iec_poll_pins(void)
// {
//     //return CBM_PIN >> 1;
//     return 0;
// }

void iec_set(uint8_t line);
void iec_release(uint8_t line);
void iec_set_release(uint8_t s, uint8_t r);
uint8_t iec_get(uint8_t line);
uint8_t iec_poll_pins(void);

// Status indicators (LEDs)
void board_init_leds(void);
uint8_t board_get_status(void);
void board_set_status(uint8_t status);
void board_update_display(void);
bool board_timer_fired(void);


#endif