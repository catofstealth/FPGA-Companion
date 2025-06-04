/*
  main.c - MiSTeryNano FPGA Companion Pi Pico variant

*/

#include "../mcu_hw.h"

#include "../config.h"
#include "../sysctrl.h"
#include "../sdc.h"
#include "../osd.h"
#include "../menu.h"
#include "../core.h"
#include "../inifile.h"
#include "../debug.h"
#include "../xml.h"
#include "../at_wifi.h"

#include "../usbh_msc_drv.h"

/*-----------------------------------------------------------*/
/*---            main FPGA communication task            ----*/
/*-----------------------------------------------------------*/

TaskHandle_t com_task_handle = NULL;
TaskHandle_t testReadUsbMscDevice_handle = NULL;


static void com_task(__attribute__((unused)) void *p ) {
  debugf("Starting main communication task");

  // startup FPGA, this will also put the core into reset
  if(sys_wait4fpga()) {
    // FPGA is ready and can be talked to

    // initialitze SD card
    sdc_init();

    // try to load a config .xml from sd card. If the core has identified itself,
    // then e.g. atarist.xml will be read. otherwise config.xml
    FIL fil;
    if(f_open(&fil, sys_get_config_name(), FA_OPEN_EXISTING | FA_READ) == FR_OK) {
      config_init();

      UINT br; char c;
      debugf("Loading XML config from file");

      // read byte by byte. Slow but that doesn't hurt ...
      FRESULT r = f_read(&fil, &c, 1, &br);
      while(r == FR_OK && br)
      {
	      xml_parse(c);
	      r = f_read(&fil, &c, 1, &br);
      }
      f_close(&fil);

      config_dump();

    } else {
      // no XML on SD card, try to load from core itself
      char *cfg_str = sys_get_config();
      if(cfg_str) {
        debugf("Loading XML config from core");
        config_init();
        char *c = cfg_str;
        while(*c) xml_parse(*c++);
        config_dump();
      } else
      {
        debugf("No valid config found, neither on sd card nor in core");
      }

    }

    // process any pending interrupt. Filter out irq 1 which is the
    // FPGA cold boot event which we ignore since we just booted outselves
    sys_handle_interrupts(sys_irq_ctrl(0xff), true);

    // by default, DB9 interrupts are disabled. Reading
    // the DB9 state enables them. This is what hid_handle_event
    // does.
    hid_handle_event();

    if(!cfg) {
      // finally release FPGA from reset
      sys_set_val('R', 0);
    }

    // initialize on-screen-display and menu system
    osd_init();
    menu_init();

    // open disk images, either defaults set in sdc_init or
    // user configure ones from the ini file
    sdc_mount_defaults();

    // finally prepare for wifi communication
    at_wifi_init();

    debugf("Entering main loop");

    for(;;) {
      mcu_hw_irq_ack();  // (re-)enable interrupt
      ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
      sys_handle_interrupts(sys_irq_ctrl(0xff), false);
    }
  }

  /* This will only be reached if the FPGA is not ready */
  /* So loop foreever while e.g. USB is still being handled */
  /* e.g. for debugging */
  for(;;) vTaskDelay(pdMS_TO_TICKS(250));
}

static void TestReadUsbMscDevice(__attribute__((unused)) void *p )
{
  for(;;){

    vTaskDelay(pdMS_TO_TICKS(8000));

    printf("Attempting to fetch test data \r\n");

    OpenTestFile(); //try to run our USB driver to get the test file

    ListDirectory("/");
    // ListDirectory("/dev/sda");
    // ListDirectory("/dev/sda/");

    vTaskDelay(pdMS_TO_TICKS(1000));
  } 
}

#ifdef ESP_PLATFORM
void app_main( void )
#else
int main( void )
#endif
{
  mcu_hw_init(); //sets up the USB Host stuff, MSC registered at /dev/sda
//add this to HW init? Or call after SD Card Init?
  extern void fatfs_mscusbh_driver_register(void);
  fatfs_mscusbh_driver_register();

  // run FPGA com thread
  xTaskCreate( com_task, "FPGA Com", 4096, NULL, CONFIG_MAX_PRIORITY-1, &com_task_handle );

  //load our test for reading USB MSC
  //printf("Main loop entering test function :\r\n");
  xTaskCreate( TestReadUsbMscDevice, "Test Read Usb Msc Device", 4096, NULL, CONFIG_MAX_PRIORITY-1, &testReadUsbMscDevice_handle );

  mcu_hw_main_loop();

#ifndef ESP_PLATFORM
  return 0;
#endif
}
/*-----------------------------------------------------------*/

