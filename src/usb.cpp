#include "usb.h"

// Control buffer to hold data received during
// control requests with DATA stage
uint8_t usbd_control_buffer[128];

void usb_poll(void)
{
    usbd_poll(usbd_dev);
}

void usb_write(uint8_t *bytes, uint32_t len)
{
    usbd_ep_write_packet(usbd_dev, 0x81, bytes, len);
}

void usb_init()
{
    // USB is on GPIO bank A
    // PA11 (USB-), PA12 (USB+)
    rcc_periph_clock_enable(RCC_GPIOA);

    /*
       * This is a somewhat common cheap hack to trigger device re-enumeration
       * on startup.  Assuming a fixed external pullup on D+, (For USB-FS)
       * setting the pin to output, and driving it explicitly low effectively
       * "removes" the pullup.  The subsequent USB init will "take over" the
       * pin, and it will appear as a proper pullup to the host.
       * The magic delay is somewhat arbitrary, no guarantees on USBIF
       * compliance here, but "it works" in most places.
       */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    gpio_clear(GPIOA, GPIO12);
    for (unsigned i = 0; i < 800000; i++)
    {
        __asm__("nop");
    }

    // Initialize USB device
    usbd_dev = usbd_init(
        &st_usbfs_v1_usb_driver,
        &dev_descr,
        &config,
        usb_strings,
        3,
        usbd_control_buffer,
        sizeof(usbd_control_buffer));

    usbd_register_set_config_callback(usbd_dev, hid_set_config);
}
