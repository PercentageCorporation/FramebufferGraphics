# Framebuffer Graphics
Graphics routines for Linux (Raspberry Pi) framebuffer screens.

The include file fbGraphics.h contains the following routines for generating simple shapes on a framebuffer mapped display.  
This has been tested on a Raspberry Pi 3B+ with an attached HDMI touchscreen running Bookworm Lite:  
~~~
2023-12-11-raspios-bookworm-arm64-lite.img
~~~

- line
- circle (filled or unfilled)
- rectangle (filled or unfilled)
- rounded_rectangle (filled or unfilled)

A test routine is included that can be compiled an run using the following commands:
```
gcc fbtest.cpp -o fbtest -lstdc++
./fbtest
```
