#include <stdio.h>
#include <stdlib.h>

#include "fbGraphics.h"

int main()
{
    int fbfd = 0;
    struct fb_var_screeninfo orig_vinfo;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) 
	{
      printf("Error: cannot open framebuffer device.\n");
      return(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) 
	{
      printf("Error reading FBIOGET_VSCREENINFO.\n");
    }
    printf("Screen dimensions: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) 
	{
      printf("Error reading FBIOGET_FSCREENINFO.\n");
    }
	
	printf("Line length: %d\r\n", finfo.line_length);

    // map fb to user mem 
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char*)mmap(0, 
              screensize, 
              PROT_READ | PROT_WRITE, 
              MAP_SHARED, 
              fbfd, 
              0);

    if ((long)fbp == -1) 
	{
        printf("Failed to mmap.\n");
    }
    else 
	{
		clear_screen();

		line(200, 75, 900, 75, GREEN);
		// unfilled rectangle
		rectangle(800, 200, 100, 300, BLUE, false); 
		// filled rectangle
		rounded_rectangle( 500, 300, 200 , 50, 10, RED, true); 
		// unfilled circle
		circle(150, 300, 100, BLUE, false);
		// filled circle
		circle(375, 300, 100, GREEN, true);
		
        sleep(5);
    }

    // return screen to original condition
    munmap(fbp, screensize);
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) 
	{
        printf("Error restoring FBIOPUT_VSCREENINFO.\n");
    }

    close(fbfd);

    return 0;
  
}
