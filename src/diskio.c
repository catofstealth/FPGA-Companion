#include "diskio.h"
#include "usbh_msc.h"

extern struct usbh_msc *g_usbh_msc;  // Pointer to connected MSC device

DSTATUS disk_initialize(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;
    if (!g_usbh_msc) return STA_NOINIT;
    return 0;
}

DSTATUS disk_status(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;
    if (!g_usbh_msc) return STA_NOINIT;
    return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    if (!g_usbh_msc) return RES_NOTRDY;
    return usbh_msc_scsi_read10(g_usbh_msc, sector, buff, count) == 0 ? RES_OK : RES_ERROR;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    if (!g_usbh_msc) return RES_NOTRDY;
    return usbh_msc_scsi_write10(g_usbh_msc, sector, (uint8_t *)buff, count) == 0 ? RES_OK : RES_ERROR;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    if (!g_usbh_msc) return RES_NOTRDY;
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = g_usbh_msc->blocknum;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = g_usbh_msc->blocksize;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            return RES_OK;
        default:
            return RES_PARERR;
    }
}
