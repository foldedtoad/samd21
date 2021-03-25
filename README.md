# SSD1306 OLED display on SAMD21 + ZEPHYR

This project shows how the Zephy RTOS running on an Atmel SAMD21 SOC, can control a Solomon SSD1306 OLED display.

In this specific phyical configuration, the board was the **Adafruit_Feather_M0_Basic_Proto** board, combined with the 
**Adafruit_OLED_FeatherWing display+buttons**.

While the above configuration has been successfully run, other combinations of SAMD21 and SSD1306 display work as well or be easily modified to work.

A video showing this demo in action can be found [here](https://youtu.be/fuI0KuYwGZU "SAMD21 demo video")

This project doesn't provide a guide for connecting a JTAG/SWD emulator to the target SAMD21 board, but left as a QED.

**NOTE:** There is a bug in the Zephyr I2C driver for the SAMD21 (SAM0): a fix patch can be found in the *doc* directory, along with logic analyser traces showing both the original I2C transfer failure and (after patching) a trace showing a good transfer.  The *i2c_sam0.patch* file will modify the Zephyr file at *zephyr/drivers/i2c/i2c_sam0.c*.
