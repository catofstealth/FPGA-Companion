#ifndef USBH_MSC_DRV_H
#define USBH_MSC_DRV_H

#include "config.h"
#include <ff.h>
#include <diskio.h>   // for DEV_SD

void OpenTestFile();
int MSCUSB_disk_initialize();
void ListDirectory(char* path);

#endif /* USBH_MSC_DRV_H */