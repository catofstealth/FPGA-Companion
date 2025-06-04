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


//override the __WEAK run method called in usbh_msc
//use this to get the msc_class object we need to interface with our device
void usbh_msc_run(struct usbh_msc *msc_class)
{
    //do this on insert so we can check we are initialised?
    usbh_msc_scsi_init(msc_class);

    //create a FatFS object and register it here...

    // if(MSCUSB_disk_initialize() != RES_OK)
    // {
    //     msc_debugf("USB storage failed to initialise successfully\n");
    //     return;
    // }

    // msc_debugf("MSC Device configured and ready\r\n");



    //do this part in our main logic elsewhere ...

    //OpenTestFile();

}

//implement FatFS using the local source code copy instead of the one provide in bouffalo SDK.
//we need to link this to the MSC stuff

struct usbh_msc *active_msc_class;

int MSCUSB_disk_status(void)
{
    return 0;
}

int MSCUSB_fs_init()
{
    char *mntpath = "/usb";
    FRESULT res = f_mount(&usbfs, mntpath, 1);  // Mount the root directory
    if (res == FR_OK)
    {
        msc_debugf("USB filesystem failed to mount successfully %d\n", res);
        return RES_ERROR;
    }

    msc_debugf("USB storage mounted successfully!\n");
}

int MSCUSB_fs_close()
{
    char *mntpath = "/usb";
    FRESULT res = f_mount(NULL, mntpath, 1);  // Mount the root directory
    if (res == FR_OK)
    {
        msc_debugf("USB filesystem failed to unmount successfully %d\n", res);
        return RES_ERROR;
    }

    msc_debugf("USB storage unmounted successfully!\n");
}

int MSCUSB_disk_initialize(void)
{
    // initialisation is taken care for us, we just need to mount the file system
    // now the interrupt has been loaded. We probably want to move this into a thread
    // to prevent watchdog reset timerz

    // char *path = "/dev/sda";
    // //not sure if we need the below...
    // active_msc_class = (struct usbh_msc *)usbh_find_class_instance(path);
    // if (active_msc_class == NULL) {
    //     printf("do not find %s\r\n", path);
    //     return RES_NOTRDY;
    // }
    
    //fatfs uses /usb mount point but msc driver uses /dev/sdX!
    //MSCUSB_fs_init(); //try calling this from main next time to mount/unmount?

    //msc_debugf("USB storage mounted successfully!\n");
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

    msc_debugf("Opening file test.txt.\n");

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
