#ifndef _PICO1541_H_INCLUDED_
#define _PICO1541_H_INCLUDED_

#include <stdio.h>
#include <pico/stdlib.h>
#include <string.h>
#include <hardware/uart.h>

// All supported models. Add new ones below.
#define PICO_OFFICIAL (0) //The official PI PICO Board from the Raspberry Foundation.

//TODO: Set model for debugging here, use CMAKE later.
#define MODEL PICO_OFFICIAL

#if MODEL == PICO_OFFICIAL
#include "board-pico-official.h"
#include "cpu-pico-official.h"
#endif

#include "pico1541_types.h"  

#ifdef DEBUG
#include <stdio.h>
#define DBG_ERROR   0
#define DBG_INFO    1
#define DBG_ALL     DBG_ERROR
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DBG_ALL
#endif
#define DEBUGF(level, format, args...)      \
    do {                                    \
        if (DEBUG_LEVEL >= level)           \
            printf_P(PSTR(format), ##args); \
    } while (0)
#else
#define DEBUGF(level, format, args...)
#endif

// Status levels to notify the user (e.g. LEDS)
#define STATUS_INIT             0
#define STATUS_CONNECTING       1
#define STATUS_READY            2
#define STATUS_ACTIVE           3
#define STATUS_ERROR            4


/* specifiers for the IEC lines (must match values from opencbm.h) */
#define IEC_DATA    0x01
#define IEC_CLOCK   0x02
#define IEC_ATN     0x04
#define IEC_RESET   0x08
#define IEC_SRQ     0x10

/* specifiers for the IEEE-488 lines (must match values from opencbm.h) */
#define IEE_NDAC    0x01 // Not data accepted
#define IEE_NRFD    0x02 // Not ready for data
#define IEE_ATN     0x04 // Attention, similar to IEC ATN
#define IEE_IFC     0x08 // Interface clear, similar to IEC RESET
#define IEE_DAV     0x10 // Data valid
#define IEE_EOI     0x20 // End or identify
#define IEE_REN     0x40 // Remote enable (not really used)
#define IEE_SRQ     0x80 // Device service request

// Definition of EEPROM cells used for certain configuration settings
// Please always add new settings at the end of this declaration
enum {
    EEPROM_SerialNumber = 0,
};

extern volatile uint8_t eoi;
extern volatile bool doDeviceReset;
extern volatile bool device_running;

//TODO: USB IO functions and command handlers
bool TimerWorker(void);
void SetAbortState(void);
/*
int8_t usbHandleControl(uint8_t cmd, uint8_t *replyBuf);
int8_t usbHandleBulk(uint8_t *request, uint8_t *status);
void USB_ResetConfig(void);
bool USB_ReadBlock(uint8_t *buf, uint8_t len);
bool USB_WriteBlock(uint8_t *buf, uint8_t len);
uint8_t AbortOnReset(void);
void usbInitIo(uint16_t len, uint8_t dir);
void usbIoDone(void);
int8_t usbSendByte(uint8_t data);
int8_t usbRecvByte(uint8_t *data);
void Set_usbDataLen(uint16_t Len);
*/

// IEC functions
#define PICO_WRITE_TALK          (1 << 0)
#define PICO_WRITE_ATN           (1 << 1)
#define IEC_DELAY()             DELAY_US(2) // Time for IEC lines to change

// IEC or IEEE handlers for protocols
struct ProtocolFunctions {
    void (*cbm_reset)(bool forever);
    uint16_t (*cbm_raw_write)(uint16_t len, uint8_t flags);
    uint16_t (*cbm_raw_read)(uint16_t len);
    bool (*cbm_wait)(uint8_t line, uint8_t state);
    uint8_t (*cbm_poll)(void);
    void (*cbm_setrelease)(uint8_t set, uint8_t release);
};

// Global pointer to protocol, set by cbm_init()
extern struct ProtocolFunctions *cmds;

// Initializers for each protocol
struct ProtocolFunctions *cbm_init(void);
struct ProtocolFunctions *iec_init(void);
#ifdef IEEE_SUPPORT
//struct ProtocolFunctions *ieee_init(void);
#endif


/*
 * Special protocol handlers:
 * cbm - default CBM serial or IEEE-488
 * s1 - serial
 * s2 - serial
 * p2 - parallel
 * pp - parallel
 * nib - nibbler parallel
 * Tape - 153x tape
 */
uint8_t s1_read_byte(void);
void s1_write_byte(uint8_t c);

/*
uint8_t s2_read_byte(void);
void s2_write_byte(uint8_t c);
uint8_t p2_read_byte(void);
void p2_write_byte(uint8_t c);
void pp_read_2_bytes(uint8_t *c);
void pp_write_2_bytes(uint8_t *c);
uint8_t nib_parburst_read(void);
int8_t nib_read_handshaked(uint8_t *c, uint8_t toggle);
void nib_parburst_write(uint8_t data);
int8_t nib_write_handshaked(uint8_t data, uint8_t toggle);
*/





#endif