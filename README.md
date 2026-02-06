# LGL Studio VFD display driver for RPI Pico C/C++ SDK

WIP

Made for VFD displays from LGL Studio

Currently only 6ch variant is tested, change DIM setup in vfd.c to support 8ch and 16ch variants:
    display SKU dependent: 6-digit: 0x05 8-digit: 0x07 16-digit: 0x0f

https://www.aliexpress.com/item/1005002932421503.html?spm=a2g0o.order_list.order_list_main.151.10de1802iXYhR6

![6ch VFD Display from aliexpress](https://ae01.alicdn.com/kf/S76123abd1cd7427aadb467ca46f095e8D.jpg)

## Features

- [x] Init display
- [x] Write char
- [x] Write str
- [x] Fullbright test
- [x] Brightness adjustment
- [ ] Custom font support | VFD_WriteUserFont
- [ ] Symbol support for 12ch+ variants | VFD_Write_ADRAM

## Running

1. Clone pico-sdk from rpi
1. Clone submodules in pico-sdk folder for USB support `git submodule update init`
1. Set PICO_SDK_PATH to cloned pico-sdk folder
1. `mkdir build && cd build`
1. `cmake .. && make -j4`
1. Flash the UF2
