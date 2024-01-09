#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

// screen info
char *fbp = 0;
int fbfd = 0;
long int screensize = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo orig_vinfo;

int open_framebuffer(const char* filename)
{
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) 
	{
      printf("Error: cannot open framebuffer device.\n");
      return(-1);
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
	    close(fbfd);
		return -1;
    }

	return 0;
}

void close_framebuffer()
{
    if (*fbp != 0) munmap(fbp, screensize);
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) 
	{
        printf("Error restoring FBIOPUT_VSCREENINFO.\n");
    }

    close(fbfd);
}