#include "ff.h"  // Include FatFS headers from SDK not src
//#include <stdio.h>
//#include <stdlib.h>
#include <diskio.h>
//#include <string.h>

//#include "sysctrl.h"


#include <usbh_core.h>
#include <usbh_msc.h>



#include "usbh_msc_drv.h"  // Include the USB MSC headers

#include "debug.h"

// Define a global variable for FatFS
static FATFS usbfs;  // FatFS file system object
static FIL fil[MAX_DRIVES];
static DWORD *lktbl[MAX_DRIVES];

struct usbh_msc *active_msc_class;

static int fs_init() 
{
  FRESULT res_msc;

  for(int i=0;i<MAX_DRIVES;i++)
    lktbl[i] = NULL;

//do this on init instead of here? or do this for every connected device?
// #ifdef DEV_USB
//   FATFS_DiskioDriverTypeDef MSC_DiskioDriver = { NULL };
//   MSC_DiskioDriver.disk_status = msc_status;
//   MSC_DiskioDriver.disk_initialize = msc_initialize;
//   MSC_DiskioDriver.disk_write = msc_write;
//   MSC_DiskioDriver.disk_read = msc_read;
//   MSC_DiskioDriver.disk_ioctl = msc_ioctl;
//   MSC_DiskioDriver.error_code_parsing = Translate_Result_Code;

//   disk_driver_callback_init(DEV_SD, &MSC_DiskioDriver);
// #endif

// #ifdef ESP_PLATFORM
//   const ff_diskio_impl_t sdc_impl = {
//     .init = sdc_disk_initialize,
//     .status = sdc_disk_status,
//     .read = sdc_disk_read,
//     .write = sdc_disk_write,
//     .ioctl = sdc_disk_ioctl,
//   };

//   ff_diskio_register(0, &sdc_impl);
// #endif

  MSCUSB_disk_initialize(); //????

  res_msc = f_mount(&usbfs, "/dev/sda", 1);
  if (res_msc != FR_OK) {
    msc_debugf("mount fail,res:%d", res_msc);
    return -1;
  }


  return 0;
}

//override the __WEAK run method called in usbh_msc
//use this to get the msc_class object we need to interface with our device
void usbh_msc_run(struct usbh_msc *msc_class)
{
    active_msc_class = msc_class;

    //do this on insert so we can check we are initialised?
    usbh_msc_scsi_init(active_msc_class);

    //create a FatFS object and register it here...

    if(MSCUSB_disk_initialize() != RES_OK)
    {
        msc_debugf("USB storage failed to initialise successfully\n");
        return;
    }

    msc_debugf("MSC Device configured and ready\r\n");



    //do this part in our main logic elsewhere ...

    OpenTestFile();

}

// -------------------- fatfs read/write interface to USB MSC Connected to USB hub -------------------

// static DRESULT msc_read(BYTE *buff, LBA_t sector, UINT count) {
//   msc_debugf("msc_read(%p,%lu,%u)", buff, sector, count);
//   msc_read_sector(sector, buff);
//   return 0;
// }

// static DRESULT msc_write(const BYTE *buff, LBA_t sector, UINT count) {
//   msc_debugf("msc_write(%p,%lu,%u)", buff, sector, count);
//   msc_write_sector(sector, buff);
//   return 0;
// }

// static DRESULT msc_ioctl(BYTE cmd, void *buff) {
//   msc_debugf("msc_ioctl(%d,%p)", cmd, buff);

//   switch(cmd) {
//   case GET_SECTOR_SIZE:
//     *((WORD*) buff) = 512;
//     return RES_OK;
//     break;
//   }

//   return RES_ERROR;
// }

// //we want to handle everything ourselves?
// DRESULT disk_ioctl(__attribute__((unused)) BYTE pdrv, BYTE cmd, void *buff) { return sdc_ioctl(cmd, buff); }
// DRESULT disk_read(__attribute__((unused)) BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) { return sdc_read(buff, sector, count); }
// DRESULT disk_write(__attribute__((unused)) BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) { return sdc_write(buff, sector, count); }
// DSTATUS disk_status(__attribute__((unused)) BYTE pdrv) { return 0; }

// //init is done via call back when USB device is loaded
// //we want to load the file system there?


// static int fs_init() {
//   FRESULT res_msc;

//   for(int i=0;i<MAX_DRIVES;i++)
//     lktbl[i] = NULL;

// //#ifdef DEV_SD
//   FATFS_DiskioDriverTypeDef MSC_DiskioDriver = { NULL };
//   MSC_DiskioDriver.disk_status = msc_status;
//   MSC_DiskioDriver.disk_initialize = msc_initialize;
//   MSC_DiskioDriver.disk_write = msc_write;
//   MSC_DiskioDriver.disk_read = msc_read;
//   MSC_DiskioDriver.disk_ioctl = msc_ioctl;
//   MSC_DiskioDriver.error_code_parsing = Translate_Result_Code;

//   //DEV_SD is the physical drive number, we only have the path but we can get this via lookup

//   disk_driver_callback_init(DEV_USB, &MSC_DiskioDriver);
// //#endif

// //only doing BL616 for now..
// // #ifdef ESP_PLATFORM
// //   const ff_diskio_impl_t sdc_impl = {
// //     .init = sdc_disk_initialize,
// //     .status = sdc_disk_status,
// //     .read = sdc_disk_read,
// //     .write = sdc_disk_write,
// //     .ioctl = sdc_disk_ioctl,
// //   };

// //   ff_diskio_register(0, &sdc_impl);
// // #endif

//   // getting here with a timeout either means that there
//   // is no matching core on the FPGA or that there is no
//   // SD card inserted

//   //TODO use location of usb storage rather than the first?
//   res_msc = f_mount(&usbfs, "/de/sda", 1);
//   if (res_msc != FR_OK) {
//     sdc_debugf("mount fail,res:%d", res_msc);
//     sys_set_rgb(0x400000);  // red, failed
//     return -1;
//   }

//   return 0;
// }


//implement FatFS using the local source code copy instead of the one provide in bouffalo SDK.
//we need to link this to the MSC stuff



struct usbh_msc *active_msc_class;

int MSCUSB_disk_status(void)
{
    return 0;
}

int MSCUSB_disk_initialize(void)
{
    char *path = "/dev/sda";
    //not sure if we need the below...
    active_msc_class = (struct usbh_msc *)usbh_find_class_instance(path);
    if (active_msc_class == NULL) {
        printf("do not find %s\r\n", path);
        return RES_NOTRDY;
    }

    char *mntpath = "/usb";
    FRESULT res = f_mount(&usbfs, mntpath, 1);  // Mount the root directory
    if (res == FR_OK)
    {
        msc_debugf("USB filesystem failed to mount successfully %d\n", res);
        return RES_ERROR;
    }

    msc_debugf("USB storage mounted successfully!\n");
    return RES_OK;
}

int MSCUSB_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    msc_debugf("MSC_READ_SECTOR is being called instead of SDK version");
    return usbh_msc_scsi_read10(active_msc_class, sector, buff, count);
}

int MSCUSB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    return usbh_msc_scsi_write10(active_msc_class, sector, buff, count);
}

int MSCUSB_disk_ioctl(BYTE cmd, void *buff)
{
    int result = 0;

    switch (cmd) {
        case CTRL_SYNC:
            result = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(WORD *)buff = active_msc_class->blocksize;
            result = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            result = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD *)buff = active_msc_class->blocknum;
            result = RES_OK;
            break;

        default:
            result = RES_PARERR;
            break;
    }

    return result;
}

DSTATUS MSCUSB_Translate_Result_Code(int result)
{
    return result;
}

void fatfs_mscusbh_driver_register(void)
{
    FATFS_DiskioDriverTypeDef USBH_DiskioDriver = { NULL };

    USBH_DiskioDriver.disk_status = MSCUSB_disk_status;
    USBH_DiskioDriver.disk_initialize = MSCUSB_disk_initialize;
    USBH_DiskioDriver.disk_write = MSCUSB_disk_write;
    USBH_DiskioDriver.disk_read = MSCUSB_disk_read;
    USBH_DiskioDriver.disk_ioctl = MSCUSB_disk_ioctl;
    USBH_DiskioDriver.error_code_parsing = MSCUSB_Translate_Result_Code;

    msc_debugf("FatFS Driver Registered\n");

    disk_driver_callback_init(DEV_USB, &USBH_DiskioDriver);
}




void OpenTestFile()
{
    FIL file;

    FRESULT res = f_open(&file, "test.txt", FA_READ);
    if (res == FR_OK) {
        msc_debugf("Opened file successfully.\n");

        // Example: Read from the file
        char buffer[256];
        UINT bytesRead;
        res = f_read(&file, buffer, sizeof(buffer), &bytesRead);
        if (res == FR_OK) {
            msc_debugf("Read %u bytes: %s\n", bytesRead, buffer);
        } else {
            msc_debugf("Error reading file test.txt.\n");
        }

        f_close(&file);
    } else {
        msc_debugf("Failed to open file test.txt. %d\n", res);
    }
}


void ListDirectory(char* path)
{
    msc_debugf("Listing directory %s", path);

    DIR dir;
    UINT BytesWritten;
    char string[128];

    //we want to be able to check initiailisationa nd mount status and so this once ratehr than doing ti eahc time
    if(MSCUSB_disk_initialize() != RES_OK)
    {
        msc_debugf("USB storage failed to initialise successfully\n");
        return;
    }

    FRESULT res = f_opendir(&dir, path);

    if (res != FR_OK)
    {
        msc_debugf("res = %d f_opendir\n", res);
        return;
    }

    FILINFO fno;

    res = f_readdir(&dir, &fno);

    if (res != FR_OK)
        msc_debugf("res = %d f_readdir\n", res);

    sprintf(string, "%c%c%c%c %10d %s/%s",
        ((fno.fattrib & AM_DIR) ? 'D' : '-'),
        ((fno.fattrib & AM_RDO) ? 'R' : '-'),
        ((fno.fattrib & AM_SYS) ? 'S' : '-'),
        ((fno.fattrib & AM_HID) ? 'H' : '-'),
        (int)fno.fsize, path, fno.fname);

    msc_debugf("%s", string);



}
