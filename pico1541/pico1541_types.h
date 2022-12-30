#ifndef _PICO1541_TYPES_H_INCLUDED_
#define _PICO1541_TYPES_H_INCLUDED_

// Vendor and product ID. These are owned by Nate Lawson, do not reuse.
//TODO: add a different VID/PID, later...
#define PICO1541_VID                 0x16d0
#define PICO1541_PID                 0x0504

// PICO1541_INIT reports this versions
#define PICO1541_VERSION             8
#define PICO1541_MINIMUM_COMPATIBLE_VERSION 7

// USB parameters for descriptor configuration
#define PICO_BULK_IN_ENDPOINT        3
#define PICO_BULK_OUT_ENDPOINT       4
#define PICO_ENDPOINT_0_SIZE         8

// control transactions
#define PICO1541_ECHO                0
#define PICO1541_INIT                (PICO1541_ECHO + 1)
#define PICO1541_RESET               (PICO1541_ECHO + 2)
#define PICO1541_SHUTDOWN            (PICO1541_ECHO + 3)
#define PICO1541_ENTER_BOOTLOADER    (PICO1541_ECHO + 4)
#define PICO1541_TAP_BREAK           (PICO1541_ECHO + 5)
#define PICO1541_GITREV              (PICO1541_ECHO + 6)
#define PICO1541_GCCVER              (PICO1541_ECHO + 7)
#define PICO1541_LIBCVER             (PICO1541_ECHO + 8)

// Adapter capabilities, but device may not support them
#define PICO1541_CAP_CBM             0x01 // supports CBM commands
/*
#define PICO1541_CAP_NIB             0x02 // parallel nibbler
#define PICO1541_CAP_NIB_SRQ         0x04 // 1571 serial nibbler
#ifdef IEEE_SUPPORT
#define PICO1541_CAP_IEEE488         0x08 // GPIB (PET) parallel bus
#else
#define PICO1541_CAP_IEEE488         0
#endif
#ifdef TAPE_SUPPORT
#define PICO1541_CAP_TAP             0x10 // 153x tape support
#else
#define PICO1541_CAP_TAP             0
#endif
*/

//For now only CBM capabilities.
#define PICO1541_CAPABILITIES        (PICO1541_CAP_CBM)

/*#define PICO1541_CAPABILITIES        (PICO1541_CAP_CBM |      \
                                     PICO1541_CAP_NIB |      \
                                     PICO1541_CAP_TAP |      \
                                     PICO1541_CAP_IEEE488)*/

// Actual auto-detected status
#define PICO1541_DOING_RESET         0x01 // no clean shutdown, will reset now
#define PICO1541_NO_DEVICE           0x02 // no IEC device present yet
#define PICO1541_IEEE488_PRESENT     0x10 // IEEE-488 device connected
#define PICO1541_TAPE_PRESENT        0x20 // 153x tape device connected

// Sizes for commands and responses in bytes
#define PICO_CMDBUF_SIZE             4 // Command block (out)
#define PICO_STATUSBUF_SIZE          3 // Waiting status value (in)
#define PICO_DEVINFO_SIZE            8 // Response to PICO1541_INIT msg (in)

/*
 * Control msg command timeout. Since the longest command we run in this
 * mode is PICO1541_RESET (or INIT if it has to run RESET), we chose a
 * time greater than the maximum a device has to respond after ATN
 * goes active. The IEC spec lists Tat as 1 ms assuming the device is
 * alive, so we're being very generous.
 */

#define PICO1541_TIMEOUT             1.5

// Read/write commands, protocol type defined below
#define PICO1541_READ                8
#define PICO1541_WRITE               (PICO1541_READ + 1)

/*
 * Maximum size for USB transfers (read/write commands, all protocols).
 * This should be ok for the raw USB protocol. I haven't tested this much
 * but at least 8192 works (e.g. for nib protocol reads. For longer
 * transfers, the usermode code should break it up into chunks this size.
 */
#define PICO_MAX_XFER_SIZE           32768

/*
 * Individual control commands. Those that can take a while and thus
 * report async status are marked with "async".
 */
#define PICO1541_IOCTL               16
#define PICO1541_GET_EOI             (PICO1541_IOCTL + 7)
#define PICO1541_CLEAR_EOI           (PICO1541_IOCTL + 8)
//#define PICO1541_PP_READ             (PICO1541_IOCTL + 9)
//#define PICO1541_PP_WRITE            (PICO1541_IOCTL + 10)
#define PICO1541_IEC_POLL            (PICO1541_IOCTL + 11)
#define PICO1541_IEC_WAIT            (PICO1541_IOCTL + 12)
#define PICO1541_IEC_SETRELEASE      (PICO1541_IOCTL + 13)
/*#define PICO1541_PARBURST_READ       (PICO1541_IOCTL + 14)
#define PICO1541_PARBURST_WRITE      (PICO1541_IOCTL + 15)
#define PICO1541_SRQBURST_READ       (PICO1541_IOCTL + 16)
#define PICO1541_SRQBURST_WRITE      (PICO1541_IOCTL + 17)
#define PICO1541_TAP_MOTOR_ON            (PICO1541_IOCTL + 50)
#define PICO1541_TAP_GET_VER             (PICO1541_IOCTL + 51)
#define PICO1541_TAP_PREPARE_CAPTURE     (PICO1541_IOCTL + 52)
#define PICO1541_TAP_PREPARE_WRITE       (PICO1541_IOCTL + 53)
#define PICO1541_TAP_GET_SENSE           (PICO1541_IOCTL + 54)
#define PICO1541_TAP_WAIT_FOR_STOP_SENSE (PICO1541_IOCTL + 55)
#define PICO1541_TAP_WAIT_FOR_PLAY_SENSE (PICO1541_IOCTL + 56)
#define PICO1541_TAP_MOTOR_OFF           (PICO1541_IOCTL + 57)
*/

#define IS_CMD_ASYNC(x)             ((x) == PICO1541_IEC_WAIT)
/*

/*
 * Status return values for async commands.
 * Status response also contains a second data word, 16-bits little-endian.
 * This value, accessed via PICO_GET_STATUS_VAL(), usually gives a length
 * but is command-specific.
 */

#define PICO1541_IO_BUSY             1
#define PICO1541_IO_READY            2
#define PICO1541_IO_ERROR            3

// Tape/disk mode error return values for PICO1541_ioctl, PICO1541_read, PICO1541_write.

/*#define PICO1541_Error_NoTapeSupport      -100
#define PICO1541_Error_NoDiskTapeMode     -101
#define PICO1541_Error_TapeCmdInDiskMode  -102
#define PICO1541_Error_DiskCmdInTapeMode  -103*/

// Macros to retrive the status and extended value (usually a length).
#define PICO_GET_STATUS(buf)         (buf[0])
#define PICO_GET_STATUS_VAL(buf)     (((buf[2]) << 8) | (buf[1]))

/*
 * Basic CBM and special protocols for use with PICO1541_READ/WRITE
 * We use the upper nibble for the protocol type so we can use the
 * lower nibble for additional protocol-specific flags.
 */

#define PICO_RW_PROTO(x)             ((x) & 0xf0)
#define PICO_RW_FLAGS(x)             ((x) & 0x0f)
#define PICO1541_CBM                 (1 << 4) // Standard CBM protocol
#define PICO1541_S1                  (2 << 4) // serial1
/*#define PICO1541_S2                  (3 << 4) // serial2
#define PICO1541_PP                  (4 << 4) // Parallel
#define PICO1541_P2                  (5 << 4) // 2-byte parallel
#define PICO1541_NIB                 (6 << 4) // burst nibbler parallel
#define PICO1541_NIB_COMMAND         (7 << 4) // BN parallel commands
#define PICO1541_NIB_SRQ             (8 << 4) // 1571 serial nibbler
#define PICO1541_NIB_SRQ_COMMAND     (9 << 4) // Serial commands
#define PICO1541_TAP                (10 << 4) // tape read/write
#define PICO1541_TAP_CONFIG         (11 << 4) // tape send/receive configuration*/

// Flags for use with write and PICO1541_CBM protocol
#define PICO_WRITE_TALK              (1 << 0)
#define PICO_WRITE_ATN               (1 << 1)

// Request an early exit from nib read via burst_read_track_var()
//#define PICO1541_NIB_READ_VAR        0x8000*/


#endif