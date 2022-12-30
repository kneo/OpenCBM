
#include "pico1541.h"

/*
#define LED_UPPER_RED   LEDS_LED3
#define LED_UPPER_GREEN LEDS_LED4
#define LED_LOWER_RED   LEDS_LED1
#define LED_LOWER_GREEN LEDS_LED2
*/

static uint8_t statusValue;
static uint8_t statusMask;
static bool has_board_timer_fired;

#ifdef DEBUG
// Send a byte to the UART for debugging printf()
static int uart_putchar(char c, FILE *stream)
{
    /*if (c == '\n')
        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR1A, UDRE1);
    UDR1 = c;*/
    return -1;
}
//static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
#endif // DEBUG

static struct repeating_timer board_timer;

bool board_timer_callback(struct repeating_timer *t) {
    has_board_timer_fired = true;
    return true;
}

void board_init_leds(void)
{
    //TODO: add some status LEDs. 
}

// Initialize the board (timer, indicator LEDs, UART)
void board_init(void)
{
    has_board_timer_fired=false;
    
    void* user_data = NULL;

    add_repeating_timer_ms(100, board_timer_callback, user_data, &board_timer);

    board_init_leds();

}

// Initialize the board IO ports for IEC mode
// This function has to work even if the ports were left in an indeterminate
// state by a prior initialization (e.g., auto-probe for IEEE devices).
void board_init_iec(void)
{

    for(uint8_t pin = 0; pin<IO_PIN_COUNT; pin++)
    {
        gpio_init(global_pin_lookup_table[pin].gpio);
        gpio_set_dir(global_pin_lookup_table[pin].gpio, GPIO_IN);
    }

    /*CBM_DDR = IO_OUTPUT_MASK;
    CBM_PORT = (uint8_t)~IO_OUTPUT_MASK;
    PAR_PORT_PORT = 0;
    PAR_PORT_DDR = 0;*/
}

INLINE void SetSignalOutHigh(uint8_t vio_pin)
{
        gpio_set_dir(global_pin_lookup_table[vio_pin].gpio, GPIO_OUT);
        gpio_put (global_pin_lookup_table[vio_pin].gpio, true);
}

void iec_set(uint8_t line)
{
    if(IS_SIGNAL_SET(line,IO_ATN))
    {
        SetSignalOutHigh(VIO_PIN_ATN);
    }

    if(IS_SIGNAL_SET(line,IO_CLK))
    {
        SetSignalOutHigh(VIO_PIN_CLK);    
    }

    if(IS_SIGNAL_SET(line,IO_DATA))
    {
        SetSignalOutHigh(VIO_PIN_DATA);    
    }

    if(IS_SIGNAL_SET(line,IO_RESET))
    {
        SetSignalOutHigh(VIO_PIN_RESET);    
    }

    if(IS_SIGNAL_SET(line,IO_SRQ))
    {
        SetSignalOutHigh(VIO_PIN_SRQ);   
    }
}

INLINE void SetSignalInLow(uint8_t vio_pin)
{
        gpio_put (global_pin_lookup_table[vio_pin].gpio, false);    
        gpio_set_dir(global_pin_lookup_table[vio_pin].gpio, GPIO_IN);
}

void iec_release(uint8_t line)
{
    if(IS_SIGNAL_SET(line,IO_ATN))
    {
        SetSignalInLow(VIO_PIN_ATN);
    }

    if(IS_SIGNAL_SET(line,IO_CLK))
    {
        SetSignalInLow(VIO_PIN_CLK);
    }

    if(IS_SIGNAL_SET(line,IO_DATA))
    {
        SetSignalInLow(VIO_PIN_DATA);
    }

    if(IS_SIGNAL_SET(line,IO_RESET))
    {
        SetSignalInLow(VIO_PIN_RESET);
    }

    if(IS_SIGNAL_SET(line,IO_SRQ))
    {
        SetSignalInLow(VIO_PIN_SRQ);
    }
}

INLINE int8_t ResolveGPIO(uint8_t line)
{
    if(IS_SIGNAL_SET(line,IO_ATN))
    {
        return global_pin_lookup_table[VIO_PIN_ATN].gpio;
    }

    if(IS_SIGNAL_SET(line,IO_CLK))
    {
        return global_pin_lookup_table[VIO_PIN_CLK].gpio;
    }

    if(IS_SIGNAL_SET(line,IO_DATA))
    {
        return global_pin_lookup_table[VIO_PIN_DATA].gpio;
    }

    if(IS_SIGNAL_SET(line,IO_RESET))
    {
        return global_pin_lookup_table[VIO_PIN_RESET].gpio;
    }

    if(IS_SIGNAL_SET(line,IO_SRQ))
    {
        return global_pin_lookup_table[VIO_PIN_SRQ].gpio;
    }

    return -1;
}

void iec_set_release(uint8_t s, uint8_t r)
{
    iec_set(s);
    iec_release(r);
}

uint8_t iec_get(uint8_t line)
{
    bool gpio_value = gpio_get(ResolveGPIO(line));
    return (gpio_value==true)? 1 : 0;
}

/*INLINE uint8_t iec_pp_read(void)
{
    PAR_PORT_DDR = 0;
    PAR_PORT_PORT = 0;
    return PAR_PORT_PIN;
    return 0;
}

INLINE void iec_pp_write(uint8_t val)
{
    PAR_PORT_DDR = 0xff;
    PAR_PORT_PORT = val;
}*/

uint8_t iec_poll_pins(void)
{
    uint8_t result = 0;

    result |= gpio_get(ResolveGPIO(VIO_PIN_ATN))    == 1 ? VIO_PIN_ATN : 0;
    result |= gpio_get(ResolveGPIO(VIO_PIN_CLK))    == 1 ? VIO_PIN_CLK : 0;
    result |= gpio_get(ResolveGPIO(VIO_PIN_DATA))   == 1 ? VIO_PIN_DATA : 0;
    result |= gpio_get(ResolveGPIO(VIO_PIN_RESET))  == 1 ? VIO_PIN_RESET : 0;
    result |= gpio_get(ResolveGPIO(VIO_PIN_SRQ))    == 1 ? VIO_PIN_SRQ : 0;

    return result;
}

uint8_t board_get_status()
{
    return statusValue;
}

// Status indicators (LEDs for this board)
void board_set_status(uint8_t status)
{
    statusValue = status;

    /*switch (status) {
    case STATUS_INIT:
        LEDs_SetAllLEDs(LED_UPPER_RED);
        break;
    case STATUS_CONNECTING:
        LEDs_SetAllLEDs(LED_UPPER_GREEN);
        break;
    case STATUS_READY:
        LEDs_SetAllLEDs(LED_LOWER_GREEN);
        break;
    case STATUS_ACTIVE:
        // Toggle both green LEDs while busy
        statusMask = LED_UPPER_GREEN | LED_LOWER_GREEN;
        LEDs_SetAllLEDs(statusMask);
        break;
    case STATUS_ERROR:
        // Set both red on error
        statusMask = LED_UPPER_RED | LED_LOWER_RED;
        LEDs_SetAllLEDs(statusMask);
        break;
    default:
        DEBUGF(DBG_ERROR, "badstsval %d\n", status);
    }*/
}

/*
 * Callback for when the timer fires.
 * Update LEDs or do other tasks that should be done about every
 */
void board_update_display()
{
    /*if (statusValue == STATUS_ACTIVE || statusValue == STATUS_ERROR)
        LEDs_SetAllLEDs(LEDs_GetLEDs() ^ statusMask);*/
}

/*
 * Signal that the board_update_display() should be called if the timer
 * has fired (every ~100 ms).
 */
bool board_timer_fired()
{
    bool result = has_board_timer_fired;

    if(has_board_timer_fired)
    {
        has_board_timer_fired = false;
    }

    return result;
}
