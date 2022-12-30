#include "pico1541.h"

#define LED_PIN (25)

// Flag indicating we should abort any in-progress data transfers
volatile bool doDeviceReset;

// Flag for whether we are in EOI state
volatile uint8_t eoi;

// Are we in an active state? If so, run the command loop.
volatile bool device_running;

// Is the USB bus connected? If so, wait to enter active state.
static volatile bool usb_connected;

//static bool USB_BulkWorker(void);

int main()
{
    stdio_init_all();

    cpu_init();

    //TODO: map USB libraries, 
    //USB_Init();

    /*while (!usb_connected)
    {
        //wdt_reset();
    }*/
    
    // Indicate device not ready
    board_init();
    board_set_status(STATUS_INIT);

    struct ProtocolFunctions* test = cbm_init();

    uint8_t counter = 0;

    while(true)
    {
        /*while(device_running)
        {
            //TODO: usb loop worker

            //

            if (!TimerWorker())
            {
                doDeviceReset = false;
            }
        }*/

        //test->


        //printf("Hello World!\n");
        counter++;
        sleep_ms(500);
    }
}


void EVENT_USB_Device_Connect(void)
{
/*    DEBUGF(DBG_ALL, "usbcon\n");
    board_set_status(STATUS_CONNECTING);
    doDeviceReset = false;
    usb_connected = true;*/
}

void EVENT_USB_Device_Disconnect(void)
{
    /*
    DEBUGF(DBG_ALL, "usbdiscon\n");

    // Halt the main() command loop and indicate we are not configured
    usb_connected = false;
    device_running = false;
    board_set_status(STATUS_INIT);*/
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
    /*DEBUGF(DBG_ALL, "usbconfchg\n");

    // Clear out any old configuration before allocating
    USB_ResetConfig();*/

    /*
     * Setup and enable the two bulk endpoints. This must be done in
     * increasing order of endpoints (3, 4) to avoid fragmentation of
     * the USB RAM.
     */
    /*Endpoint_ConfigureEndpoint(XUM_BULK_IN_ENDPOINT, EP_TYPE_BULK,
        ENDPOINT_DIR_IN, XUM_ENDPOINT_BULK_SIZE, ENDPOINT_BANK_DOUBLE);
    Endpoint_ConfigureEndpoint(XUM_BULK_OUT_ENDPOINT, EP_TYPE_BULK,
        ENDPOINT_DIR_OUT, XUM_ENDPOINT_BULK_SIZE, ENDPOINT_BANK_DOUBLE);

    // Indicate USB connected and ready to start event loop in main()
    board_set_status(STATUS_READY);
    device_running = true;*/
}

void EVENT_USB_Device_UnhandledControlRequest(void)
{
    /*uint8_t replyBuf[XUM_DEVINFO_SIZE];
    int8_t len;*/

    /*
     * Ignore non-class requests. We also only handle commands
     * that don't transfer any data or just transfer it into the host.
     */
    /*if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) !=
        REQTYPE_CLASS) {
        DEBUGF(DBG_ERROR, "bad ctrl req %x\n",
            USB_ControlRequest.bmRequestType);
        return;
    }

    // Process the command and get any returned data
    memset(replyBuf, 0, sizeof(replyBuf));
    len = usbHandleControl(USB_ControlRequest.bRequest, replyBuf);
    if (len == -1) {
        DEBUGF(DBG_ERROR, "ctrl req err\n");
        board_set_status(STATUS_ERROR);
        return;
    }

    // Control request was handled so ack it to allow another
    Endpoint_ClearSETUP();

    // Send data back to host and finalize the status phase
    if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) ==
        REQDIR_DEVICETOHOST) {
        Endpoint_Write_Control_Stream_LE(replyBuf, len);
        Endpoint_ClearOUT();
    } else {
        while (!Endpoint_IsINReady())
            ;
        Endpoint_ClearIN();
    }*/
}

static bool USB_BulkWorker()
{
    /*uint8_t cmdBuf[XUM_CMDBUF_SIZE], statusBuf[XUM_STATUSBUF_SIZE];
    int8_t status;*/

    /*
     * If we are not connected to the host or a command has not yet
     * been sent, no more processing is required.
     */
    /*if (USB_DeviceState != DEVICE_STATE_Configured)
        return false;
    Endpoint_SelectEndpoint(XUM_BULK_OUT_ENDPOINT);
    if (!Endpoint_IsReadWriteAllowed())
        return false;

#ifdef DEBUG
    // Dump the status of both endpoints before getting the command
    Endpoint_SelectEndpoint(XUM_BULK_IN_ENDPOINT);
    DEBUGF(DBG_INFO, "bsti %x %x %x %x %x %x %x %x\n",
        Endpoint_GetCurrentEndpoint(),
        Endpoint_BytesInEndpoint(), Endpoint_IsEnabled(),
        Endpoint_IsReadWriteAllowed(), Endpoint_IsConfigured(),
        Endpoint_IsINReady(), Endpoint_IsOUTReceived(), Endpoint_IsStalled());
    Endpoint_SelectEndpoint(XUM_BULK_OUT_ENDPOINT);
    DEBUGF(DBG_INFO, "bsto %x %x %x %x %x %x %x %x\n",
        Endpoint_GetCurrentEndpoint(),
        Endpoint_BytesInEndpoint(), Endpoint_IsEnabled(),
        Endpoint_IsReadWriteAllowed(), Endpoint_IsConfigured(),
        Endpoint_IsINReady(), Endpoint_IsOUTReceived(), Endpoint_IsStalled());
#endif

    // Read in the command from the host now that one is ready.
    if (!USB_ReadBlock(cmdBuf, sizeof(cmdBuf))) {
        board_set_status(STATUS_ERROR);
        return false;
    }

    // Allow commands to leave the extended status untouched
    memset(statusBuf, 0, sizeof(statusBuf));*/

    /*
     * Decode and process the command.
     * usbHandleBulk() stores its extended result in the output buffer,
     * up to XUM_STATUSBUF_SIZE.
     *
     * Return values:
     *   >0: completed ok, send the return value and extended status
     *    0: completed ok, don't send any status
     *   -1: error, no status
     */
    /*status = usbHandleBulk(cmdBuf, statusBuf);
    if (status > 0) {
        statusBuf[0] = status;
        USB_WriteBlock(statusBuf, sizeof(statusBuf));
    } else if (status < 0) {
        DEBUGF(DBG_ERROR, "usbblk err\n");
        board_set_status(STATUS_ERROR);
        Endpoint_StallTransaction();
        return false;
    }

    return true;*/

    return false;
}

/*
 * Stall all endpoints and set a flag indicating any current transfers
 * should be aborted. IO loops will see this flag in TimerWorker().
 */
void SetAbortState()
{
    /*uint8_t origEndpoint = Endpoint_GetCurrentEndpoint();

    doDeviceReset = true;
    Endpoint_SelectEndpoint(XUM_BULK_OUT_ENDPOINT);
    Endpoint_StallTransaction();
    Endpoint_SelectEndpoint(XUM_BULK_IN_ENDPOINT);
    Endpoint_StallTransaction();

    Endpoint_SelectEndpoint(origEndpoint);*/
}

/*
 * Periodic maintenance task. This code can be called at any point, but
 * at least needs to be called enough to reset the watchdog.
 *
 * If the board's timer has triggered, we also update the board's display
 * or any other functions it does when the timer expires.
 */
bool TimerWorker()
{
    /*wdt_reset();*/

    // Inform the caller to quit the current transfer if we're resetting.
    if (doDeviceReset)
        return false;

    // If the timer has fired, update the board display
    if (board_timer_fired())
        board_update_display();
    return true;
    //return false;
}

/*
 * The Linux and OSX call the configuration changed entry each time
 * a transaction is started (e.g., multiple runs of cbmctrl status).
 * We need to reset the endpoints before reconfiguring them, otherwise
 * we get a hang the second time through.
 *
 * We keep the original endpoint selected after returning.
 */
void USB_ResetConfig()
{
    /*static uint8_t endpoints[] = {
        XUM_BULK_IN_ENDPOINT, XUM_BULK_OUT_ENDPOINT, 0,
    };
    uint8_t lastEndpoint, *endp;

    lastEndpoint = Endpoint_GetCurrentEndpoint();

    for (endp = endpoints; *endp != 0; endp++) {
        Endpoint_SelectEndpoint(*endp);
        Endpoint_ResetFIFO(*endp);
        Endpoint_ResetDataToggle();
        if (Endpoint_IsStalled())
            Endpoint_ClearStall();
    }

    Endpoint_SelectEndpoint(lastEndpoint);*/
}

/*
 * Read a block from the host's OUT endpoint, handling aborts.
 */
bool USB_ReadBlock(uint8_t *buf, uint8_t len)
{
    // Get the requested data from the host
    /*Endpoint_SelectEndpoint(XUM_BULK_OUT_ENDPOINT);
    Endpoint_Read_Stream_LE(buf, len, AbortOnReset);

    // Check if the current command is being aborted by the host
    if (doDeviceReset)
        return false;

    Endpoint_ClearOUT();
    return true;*/
    return false;
}

/*
 * Send a block to the host's IN endpoint, handling aborts.
 */
bool USB_WriteBlock(uint8_t *buf, uint8_t len)
{
    // Get the requested data from the host
    /*Endpoint_SelectEndpoint(XUM_BULK_IN_ENDPOINT);
    Endpoint_Write_Stream_LE(buf, len, AbortOnReset);

    // Check if the current command is being aborted by the host
    if (doDeviceReset)
        return false;

    Endpoint_ClearIN();
    return true;*/

    return false;
}

/*
 * Callback for the Endpoint_Read/Write_Stream functions. We abort the
 * current stream transfer if the user sent a reset message to the
 * control endpoint.
 */
uint8_t AbortOnReset()
{
    //return doDeviceReset ? STREAMCALLBACK_Abort : STREAMCALLBACK_Continue;
    return 0;
}
