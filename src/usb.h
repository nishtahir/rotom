#ifndef USB_H
#define USB_H

void usb_init();
void usb_poll(void);
void usb_write(uint8_t *bytes, uint32_t len);

#endif