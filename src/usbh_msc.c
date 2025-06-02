#include "usbh_msc.h"
#include "diskio.h"

static void msc_user_callback(struct usbh_msc *msc, int event)
{
    // switch (event) {
    //     case USBH_MSC_EVENT_CONNECTED:
    //         printf("MSC device connected\n");
    //         break;
    //     case USBH_MSC_EVENT_DISCONNECTED:
    //         printf("MSC device disconnected\n");
    //         break;
    // }
}

void usb_msc_init(void)
{
    usbh_msc_register_callback(msc_user_callback);
}
