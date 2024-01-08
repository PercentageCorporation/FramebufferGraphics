# Framebuffer Graphics
Graphics routines for Linux (Raspberry Pi) framebuffer screens.

The include file fbGraphics.h contains the following routines for generating simple shapes on a framebuffer mapped display.
This has been tested on a Raspberry Pi 3B+ running Bookworm Lite with an attached HDMI touchscreen:  
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
The line and circle drawing routines were implemented using Bresenham's line and circle algorithms. The fill routines were implemented by simply drawing lines between raster endpoints.
