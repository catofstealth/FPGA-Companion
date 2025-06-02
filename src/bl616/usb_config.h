/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef CHERRYUSB_CONFIG_H
#define CHERRYUSB_CONFIG_H

/* ================ USB common Configuration ================ */

#define CONFIG_USB_PRINTF(...) printf(__VA_ARGS__)

#ifndef CONFIG_USB_DBG_LEVEL
#define CONFIG_USB_DBG_LEVEL USB_DBG_INFO
#endif

/* Enable print with color */
#define CONFIG_USB_PRINTF_COLOR_ENABLE

/* data align size when use dma or use dcache */
#ifndef CONFIG_USB_ALIGN_SIZE
#define CONFIG_USB_ALIGN_SIZE 4
#endif

//#define CONFIG_USB_DCACHE_ENABLE

/* attribute data into no cache ram */
#define USB_NOCACHE_RAM_SECTION __attribute__((section(".noncacheable")))

/* ================= USB Device Stack Configuration ================ */

/* Ep0 in and out transfer buffer */
#ifndef CONFIG_USBDEV_REQUEST_BUFFER_LEN
#define CONFIG_USBDEV_REQUEST_BUFFER_LEN 512
#endif

/* Setup packet log for debug */
// #define CONFIG_USBDEV_SETUP_LOG_PRINT

/* Send ep0 in data from user buffer instead of copying into ep0 reqdata
 * Please note that user buffer must be aligned with CONFIG_USB_ALIGN_SIZE
*/
// #define CONFIG_USBDEV_EP0_INDATA_NO_COPY

/* Check if the input descriptor is correct */
// #define CONFIG_USBDEV_DESC_CHECK

/* Enable test mode */
// #define CONFIG_USBDEV_TEST_MODE

/* enable advance desc register api */
// CONFIG_USBDEV_ADVANCE_DESC

/* move ep0 setup handler from isr to thread */
// #define CONFIG_USBDEV_EP0_THREAD

#ifndef CONFIG_USBDEV_EP0_PRIO
#define CONFIG_USBDEV_EP0_PRIO 4
#endif

#ifndef CONFIG_USBDEV_EP0_STACKSIZE
#define CONFIG_USBDEV_EP0_STACKSIZE 2048
#endif

#ifndef CONFIG_USBDEV_MSC_MAX_LUN
#define CONFIG_USBDEV_MSC_MAX_LUN 1
#endif

#ifndef CONFIG_USBDEV_MSC_MAX_BUFSIZE
#define CONFIG_USBDEV_MSC_MAX_BUFSIZE 512
#endif

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_VERSION_STRING
#define CONFIG_USBDEV_MSC_VERSION_STRING "0.01"
#endif

/* move msc read & write from isr to while(1), you should call usbd_msc_polling in while(1) */
// #define CONFIG_USBDEV_MSC_POLLING

/* move msc read & write from isr to thread */
// #define CONFIG_USBDEV_MSC_THREAD

#ifndef CONFIG_USBDEV_MSC_PRIO
#define CONFIG_USBDEV_MSC_PRIO 4
#endif

#ifndef CONFIG_USBDEV_MSC_STACKSIZE
#define CONFIG_USBDEV_MSC_STACKSIZE 2048
#endif

#ifndef CONFIG_USBDEV_AUDIO_VERSION
#define CONFIG_USBDEV_AUDIO_VERSION 0x0100
#endif

#ifndef CONFIG_USBDEV_AUDIO_MAX_CHANNEL
#define CONFIG_USBDEV_AUDIO_MAX_CHANNEL 8
#endif

/* This parameter affects usb performance, and depends on (TCP_WND)tcp eceive windows size,
 * you can change to 2K ~ 16K and must be larger than TCP RX windows size in order to avoid being overflow.
 */
#ifndef CONFIG_USBHOST_RNDIS_ETH_MAX_RX_SIZE
#define CONFIG_USBHOST_RNDIS_ETH_MAX_RX_SIZE (2048)
#endif

/* Because lwip do not support multi pbuf at a time, so increasing this variable has no performance improvement */
#ifndef CONFIG_USBHOST_RNDIS_ETH_MAX_TX_SIZE
#define CONFIG_USBHOST_RNDIS_ETH_MAX_TX_SIZE (2048)
#endif

/* This parameter affects usb performance, and depends on (TCP_WND)tcp eceive windows size,
 * you can change to 2K ~ 16K and must be larger than TCP RX windows size in order to avoid being overflow.
 */
#ifndef CONFIG_USBHOST_CDC_NCM_ETH_MAX_RX_SIZE
#define CONFIG_USBHOST_CDC_NCM_ETH_MAX_RX_SIZE (2048)
#endif
/* Because lwip do not support multi pbuf at a time, so increasing this variable has no performance improvement */
#ifndef CONFIG_USBHOST_CDC_NCM_ETH_MAX_TX_SIZE
#define CONFIG_USBHOST_CDC_NCM_ETH_MAX_TX_SIZE (2048)
#endif

/* This parameter affects usb performance, and depends on (TCP_WND)tcp eceive windows size,
 * you can change to 2K ~ 16K and must be larger than TCP RX windows size in order to avoid being overflow.
 */
#ifndef CONFIG_USBHOST_ASIX_ETH_MAX_RX_SIZE
#define CONFIG_USBHOST_ASIX_ETH_MAX_RX_SIZE (2048)
#endif
/* Because lwip do not support multi pbuf at a time, so increasing this variable has no performance improvement */
#ifndef CONFIG_USBHOST_ASIX_ETH_MAX_TX_SIZE
#define CONFIG_USBHOST_ASIX_ETH_MAX_TX_SIZE (2048)
#endif

/* This parameter affects usb performance, and depends on (TCP_WND)tcp eceive windows size,
 * you can change to 2K ~ 16K and must be larger than TCP RX windows size in order to avoid being overflow.
 */
#ifndef CONFIG_USBHOST_RTL8152_ETH_MAX_RX_SIZE
#define CONFIG_USBHOST_RTL8152_ETH_MAX_RX_SIZE (2048)
#endif
/* Because lwip do not support multi pbuf at a time, so increasing this variable has no performance improvement */
#ifndef CONFIG_USBHOST_RTL8152_ETH_MAX_TX_SIZE
#define CONFIG_USBHOST_RTL8152_ETH_MAX_TX_SIZE (2048)
#endif

/* ================ USB HOST Stack Configuration ================== */

#define CONFIG_USBHOST_MAX_RHPORTS          1
#define CONFIG_USBHOST_MAX_EXTHUBS          2
#define CONFIG_USBHOST_MAX_EHPORTS          4
#define CONFIG_USBHOST_MAX_INTERFACES       4
#define CONFIG_USBHOST_MAX_INTF_ALTSETTINGS 2
#define CONFIG_USBHOST_MAX_ENDPOINTS        4
#define CONFIG_USBHOST_MAX_CDC_ACM_CLASS    0
#define CONFIG_USBHOST_MAX_HID_CLASS        6
#define CONFIG_USBHOST_MAX_MSC_CLASS        2
#define CONFIG_USBHOST_MAX_XBOX_CLASS       2
#define CONFIG_USBHOST_MAX_AUDIO_CLASS      0
#define CONFIG_USBHOST_MAX_VIDEO_CLASS      0
//#define CONFIG_CHERRYUSB_HOST_RTL8152       1
//#define CONFIG_CHERRYUSB_HOST_ASIX          1

#define CONFIG_USBHOST_DEV_NAMELEN 16

#ifndef CONFIG_USBHOST_PSC_PRIO
#define CONFIG_USBHOST_PSC_PRIO 0
#endif

#ifndef CONFIG_USBHOST_PSC_STACKSIZE
#define CONFIG_USBHOST_PSC_STACKSIZE 2048
#endif

//#define CONFIG_USBHOST_GET_STRING_DESC

// #define CONFIG_USBHOST_MSOS_ENABLE
#ifndef CONFIG_USBHOST_MSOS_VENDOR_CODE
#define CONFIG_USBHOST_MSOS_VENDOR_CODE 0x00
#endif

/* Ep0 max transfer buffer */
#ifndef CONFIG_USBHOST_REQUEST_BUFFER_LEN
#define CONFIG_USBHOST_REQUEST_BUFFER_LEN 2048
#endif

#ifndef CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT
#define CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT 500
#endif

#ifndef CONFIG_USBHOST_MSC_TIMEOUT
#define CONFIG_USBHOST_MSC_TIMEOUT 5000
#endif

/* ================ USB Device Port Configuration ================*/

#ifndef CONFIG_USBDEV_MAX_BUS
#define CONFIG_USBDEV_MAX_BUS 1 // for now, bus num must be 1 except hpm ip
#endif

#ifndef CONFIG_USBDEV_EP_NUM
#define CONFIG_USBDEV_EP_NUM 8
#endif

/* When your chip hardware supports high-speed and wants to initialize it in high-speed mode, the relevant IP will configure the internal or external high-speed PHY according to CONFIG_USB_HS. */
// #define CONFIG_USB_HS

/* ---------------- FSDEV Configuration ---------------- */
//#define CONFIG_USBDEV_FSDEV_PMA_ACCESS 2 // maybe 1 or 2, many chips may have a difference

/* ================ USB Host Port Configuration ==================*/
#ifndef CONFIG_USBHOST_MAX_BUS
#define CONFIG_USBHOST_MAX_BUS 1
#endif

#ifndef CONFIG_USBHOST_PIPE_NUM
#define CONFIG_USBHOST_PIPE_NUM 10
#endif

/* ---------------- EHCI Configuration ---------------- */

#define CONFIG_USB_EHCI_HCCR_BASE       (0x20072000)
#define CONFIG_USB_EHCI_HCOR_BASE       (0x20072000 + 0x10)
// #define CONFIG_USB_EHCI_INFO_ENABLE
#define CONFIG_USB_EHCI_HCCR_OFFSET     (0x0)
#define CONFIG_USB_EHCI_FRAME_LIST_SIZE 1024
// #define CONFIG_USB_EHCI_QH_NUM          CONFIG_USBHOST_PIPE_NUM
// #define CONFIG_USB_EHCI_QTD_NUM         3
// #define CONFIG_USB_EHCI_ITD_NUM         5
#define CONFIG_USB_EHCI_HCOR_RESERVED_DISABLE
// #define CONFIG_USB_EHCI_CONFIGFLAG
// #define CONFIG_USB_EHCI_ISO
// #define CONFIG_USB_EHCI_WITH_OHCI
// #define CONFIG_USB_EHCI_PORT_POWER
// #define CONFIG_USB_PINGPONG_ENABLE
// #define CONFIG_USB_TRIPLE_ENABLE

/* ---------------- OHCI Configuration ---------------- */
#define CONFIG_USB_OHCI_HCOR_OFFSET (0x0)

/* ---------------- XHCI Configuration ---------------- */
#define CONFIG_USB_XHCI_HCCR_OFFSET (0x0)

#define CONFIG_USB_HS
#define ATTR_FAST_RAM_SECTION __attribute__((section(".fast")))
/* ================ USB Dcache Configuration ==================*/

#ifdef CONFIG_USB_DCACHE_ENABLE
/* style 1*/
// void usb_dcache_clean(uintptr_t addr, uint32_t size);
// void usb_dcache_invalidate(uintptr_t addr, uint32_t size);
// void usb_dcache_flush(uintptr_t addr, uint32_t size);

/* style 2*/
// #define usb_dcache_clean(addr, size)
// #define usb_dcache_invalidate(addr, size)
// #define usb_dcache_flush(addr, size)
#endif

#ifndef usb_phyaddr2ramaddr
#define usb_phyaddr2ramaddr(addr) (addr)
#endif

#ifndef usb_ramaddr2phyaddr
#define usb_ramaddr2phyaddr(addr) (addr)
#endif

#endif
