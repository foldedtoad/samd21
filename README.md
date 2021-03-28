# SSD1306 OLED display on SAMD21 + ZEPHYR

This project shows how the Zephy RTOS running on an Atmel SAMD21 SOC, can control a Solomon SSD1306 OLED display.

In this specific phyical configuration, the board was the **Adafruit_Feather_M0_Basic_Proto** board, combined with the 
**Adafruit_OLED_FeatherWing display+buttons**.

This demo project use the *cfb_xxx"* (Monochrome Character Framebuffer (see *./zephyr/include/display/cfb.h*)) API provided in Zephyr.  The demo is purposefully kept simple, e.g. no elaborate UI, such as the *lvgl* library, is used for example.

In addition, the Feather Display board has 3 user switches.  These have been configured such that when pressed a message will be displayed on the RTT console.

While the above configuration has been successfully run, other combinations of SAMD21 and SSD1306 display work as well or be easily modified to work.

A video showing this demo in action can be found [here](https://youtu.be/fuI0KuYwGZU "SAMD21 demo video")

This project doesn't provide a guide for connecting a JTAG/SWD emulator to the target SAMD21 board, but left as a QED.  
There is a [photo](https://github.com/foldedtoad/samd21/blob/master/docs/Adafruit_Feather_SWD_wiring.jpg "SWD wiring") which shows how to wire the Adafruit Feather board for SWD.  
This project was developed using the Segger JLINK JTAG/SWD adapter with the Segger [Ozone](https://github.com/foldedtoad/samd21/blob/master/docs/samd21_ozone_debug.png "Ozone debugger") debugger. The GDB debugger should be equally useable too.

## Building the Project
This project has been tested with the current (as of March 27, 2021) Zephyr V2.5.  
It has been tested on both Linux (Ubuntu 18.04) and Mac OSX Big Sur.  
This project should build on Windows-based systems, but has not been developed or test on that platform.

### Clearing the Bootloader Protection Bit
You may need to clear the bootloader protection bit in the NVM.  
Here is [link](https://roamingthings.de/posts/use-j-link-to-change-the-boot-loader-protection-of-a-sam-d21 ) explaining how to do remove the bootloader protection bit which will allow you to load this project's binary onto the SoC.

### Deleting the Bootloader Sections from Memory
The *adafruit_feather_m0_basic_proto.overlay* file has been modified to remove the bootloader code section.  This frees some extra code-space memory for application use: about 4K. (The 4K is the segment size previously allocated for the bootloader code.)

## SAMD21 I2C Bug and a Patch
There is a bug in the Zephyr I2C driver for the SAMD21 (SAM0): a fix patch can be found in the *doc* directory, along with logic analyser traces showing both the original I2C transfer failure and (after patching) a trace showing a good transfer.  
This [screenshot](https://github.com/foldedtoad/samd21/blob/master/docs/i2c_sam0_analysis_compare.png) gives a brief analysis of the issue.

The *i2c_sam0.patch* file will modify the Zephyr file at *zephyr/drivers/i2c/i2c_sam0.c*.  
Assume the following file/directory layout (adjust to your specific layout).  
  * Path to Zephyr root:  ~/zephyr/zephyrproject/zephyr
  * Path to samd21 project root:  ~/zephyr/samd21

To apply the patch, do the following.
```
  cd ~/zephyr/zephyrproject/zephyr
  patch -p1 < ~/zephyr/samd21/docs/i2c_sam0.patch
```
