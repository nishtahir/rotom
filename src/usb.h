#ifndef USB_H
#define USB_H

#define INCLUDE_DFU_INTERFACE

#include <stdlib.h>
#include <string.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#ifdef INCLUDE_DFU_INTERFACE
#include <libopencm3/cm3/scb.h>
#include <libopencm3/usb/dfu.h>
#endif
#include "descriptors.h"

// Reference to active USB device
static usbd_device *usbd_dev;

static enum usbd_request_return_codes hid_control_request(
    usbd_device *dev,
    struct usb_setup_data *req,
    uint8_t **buf,
    uint16_t *len,
    void (**complete)(usbd_device *dev, struct usb_setup_data *req))
{
    (void)complete;
    (void)dev;

    if ((req->bmRequestType != 0x81) ||
        (req->bRequest != USB_REQ_GET_DESCRIPTOR) ||
        (req->wValue != 0x2200))
        return USBD_REQ_NOTSUPP;

    /* Handle the HID report descriptor. */
    *buf = (uint8_t *)hid_report_descriptor;
    *len = sizeof(hid_report_descriptor);

    return USBD_REQ_HANDLED;
}

#ifdef INCLUDE_DFU_INTERFACE
static void dfu_detach_complete(usbd_device *dev, struct usb_setup_data *req)
{
    (void)req;
    (void)dev;

    char *const marker = (char *)0x20004800;
    const char key[] = "remain-in-loader";
    memcpy(marker, key, sizeof(key));
    scb_reset_system();
}

static enum usbd_request_return_codes dfu_control_request(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
                                                          void (**complete)(usbd_device *dev, struct usb_setup_data *req))
{
    (void)buf;
    (void)len;
    (void)dev;

    if ((req->bmRequestType != 0x21) || (req->bRequest != DFU_DETACH))
        return USBD_REQ_NOTSUPP; /* Only accept class request. */

    *complete = dfu_detach_complete;

    return USBD_REQ_HANDLED;
}
#endif

static void hid_set_config(usbd_device *dev, uint16_t wValue)
{
    (void)wValue;
    // http://libopencm3.org/docs/latest/stm32f1/html/group__usb__driver__defines.html#gaadc222d45c63e01811952f8478ac7943
    // This USB stack only supports a maximum of 8 endpoints
    usbd_ep_setup(dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 8, NULL);

    usbd_register_control_callback(
        dev,
        USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
        USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
        hid_control_request);
#ifdef INCLUDE_DFU_INTERFACE
    usbd_register_control_callback(
        dev,
        USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
        USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
        dfu_control_request);
#endif
}

void usb_init();
void usb_poll(void);
void usb_write(uint8_t *bytes, uint32_t len);

#endif