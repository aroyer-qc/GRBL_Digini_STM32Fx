/*
 * usb_msc_host.h
 *
 *  Created on: 21.03.2015
 *      Author: Florian
 */
//NOT Needed anymore

#ifndef LIB_USB_USB_MSC_HOST_H_
#define LIB_USB_USB_MSC_HOST_H_

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------*
 *  additional includes                                                *
 *---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*
 *  global definitions                                                 *
 *---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*
 *  type declarations                                                  *
 *---------------------------------------------------------------------*/

typedef enum
{
    USB_MSC_HOST_NO_INIT = 0,  // USB interface not initialized
    USB_MSC_DEV_DETACHED,      // no device connected
    USB_MSC_SPEED_ERROR,       // unsupported USB speed
    USB_MSC_DEV_NOT_SUPPORTED, // unsupported device
    USB_MSC_DEV_WRITE_PROTECT, // device is write protected
    USB_MSC_OVER_CURRENT,      // overcurrent detected
    USB_MSC_DEV_CONNECTED      // device connected and ready
} usb_msc_host_status_t;

/*---------------------------------------------------------------------*
 *  function prototypes                                                *
 *---------------------------------------------------------------------*/

void usb_msc_host_init (void);
usb_msc_host_status_t usb_msc_host_main (void);

/*---------------------------------------------------------------------*
 *  global data                                                        *
 *---------------------------------------------------------------------*/

extern usb_msc_host_status_t usb_msc_host_status;

/*---------------------------------------------------------------------*
 *  inline functions and function-like macros                          *
 *---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*
 *  eof                                                                *
 *---------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* LIB_USB_USB_MSC_HOST_H_ */
