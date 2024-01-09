#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

// gcc fbtest8.c -o fbtest8 -lstdc++ -lm

unsigned int RED = 0b1111100000000000;
unsigned int GREEN = 0b0000011111100000;
unsigned int BLUE = 0b0000000000011111;
unsigned int WHITE = 0b1111111111111111;

char* fbpointer;

void clear_screen()
{
	if (fbpointer == 0 || screensize == 0) return;
	memset(fbpointer, 0, screensize);
}

void put_pixel(int x, int y, unsigned int c)
{
	//printf("Pix %d %d %d\r\n", x, y, pix_offset);
    // calculate the pixel's byte offset inside the buffer
    unsigned int pix_offset = (x * 2) + (y * finfo.line_length);
    *((unsigned short*)(fbpointer + pix_offset)) = c;
}

// Bresenham's algorithm 
void line(int x0, int y0, int x1, int y1, unsigned short color )
{
   int dx, dy, sx, sy, x, y;
 
	dx = abs(x1-x0);
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	
	dy = -abs(y1-y0);
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;

	int e = dx + dy;
	 
	while(1)
	{
		put_pixel(x0, y0, color);
		if((x0 == x1) && (y0 == y1)) break;
		int e2 = e * 2;
		
		if (e2 >= dy)
		{
			if (x0 == x1) break;
			e = e + dy;
			x0 = x0 + sx;
		}
		
		if (e2 < dx)
		{
			if (y0 == y1) break;
			e = e + dx;
			y0 = y0 + sy;
		}
	}	
}

// draw circle in four quadrants
void circle_points(int xc, int yc, int x, int y, unsigned short color) 
{ 
	// top left
    put_pixel(xc-x, yc-y, color); 
    put_pixel(xc-y, yc-x, color); 

	// top right
    put_pixel(xc+x, yc-y, color); 
    put_pixel(xc+y, yc-x, color); 

	// bottom left
    put_pixel(xc-x, yc+y, color); 
	put_pixel(xc-y, yc+x, color); 

	// bottom right
    put_pixel(xc+x, yc+y, color); 
    put_pixel(xc+y, yc+x, color); 
} 

// fill circle drawing lines between left and right points in each of four quadrants
void circle_filled(int xc, int yc, int x, int y, unsigned short color) 
{ 
	int x2 = x + x;
	line(xc-x, yc+y, xc+x, yc+y, color);
	line(xc-x, yc-y, xc+x, yc-y, color);
	line(xc-y, yc+x, xc+y, yc+x, color);
	line(xc-y, yc-x, xc+y, yc-x, color);
} 
  
  
void rounded_corners_points(int xc, int yc, int x, int y, int width, int height, int r, unsigned short color, bool filled)
{
	int xtla = xc - x + r;
	int ytla = yc - y + r;
	int xtlb = xc - y + r;
	int ytlb = yc - x + r;

	int xtra = xc + x + width - r;
	int ytra = yc - y + r;
	int xtrb = xc + y + width - r;
	int ytrb = yc - x + r;

	int xbla = xc - x + r;
	int ybla = yc + y - r + height;
	int xblb = xc - y + r;
	int yblb = yc + x - r + height;

	int xbra = xc + x - r + width;
	int ybra = yc + y - r + height;
	int xbrb = xc + y - r + width;
	int ybrb = yc + x - r + height;

	if (filled)
	{
		line(xtla, ytla, xtra, ytra, color);
		line(xtlb, ytlb, xtrb, ytrb, color);
		line(xbla, ybla, xbra, ybra, color);
		line(xblb, yblb, xbrb, ybrb, color);
	}
	else
	{
    put_pixel(xtla, ytla, color); 
    put_pixel(xtlb, ytlb, color); 
	
    put_pixel(xtra, ytra, color); 
    put_pixel(xtrb, ytrb, color); 
	
    put_pixel(xbla, ybla, color); 
    put_pixel(xblb, yblb, color); 
	
    put_pixel(xbra, ybra, color); 
    put_pixel(xbrb, ybrb, color); 
	}
}

void corners(int xc, int yc, int width, int height, int r, unsigned int color, bool filled) 
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    rounded_corners_points(xc, yc, x, y, width, height, r, color, filled);
    while (y >= x) 
    { 
        x++; 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
		
		rounded_corners_points(xc, yc, x, y, width, height, r, color, filled);
    } 
} 

void rectangle(int x, int y, int width, int height, unsigned short color, bool filled )
{
	if (filled)
	{
		for (int ix = 0; ix < height; ++ix)
		{
			line(x, y+ix, x+width, y+ix, color);
		}
	}
	else
	{
		line(x, y, x+width, y, color);	// top
		line(x, y, x, y+height, color);	// left side
		line(x+width, y, x+width, y+height, color);	// right side
		line(x, y+height, x+width, y+height, color);	// bottom
	}
}

void rounded_rectangle(int x, int y, int width, int height, int r, unsigned short color, bool filled )
{
	if (!filled)
	{
	line(x+r, y, x+width-r, y, color);	// top
	line(x, y+r, x, y+height-r, color);	// left side
	line(x+width, y+r, x+width, y+height-r, color);	// right side
	line(x+r, y+height, x+width-r, y+height, color);	// bottom
	}
	else
	{
		for( int iy = y + r; iy < y + height - r; ++iy)
		{
			line(x, iy, x+width, iy, color);	// top
		}
	}
	corners(x, y, width, height, r, color, filled);
}

// Bresenham's algorithm 
void circle(int xc, int yc, int r, unsigned int color, bool filled) 
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    filled ? circle_filled(xc, yc, x, y, color) : circle_points(xc, yc, x, y, color); 
    while (y >= x) 
    { 
        x++; 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
		
		filled ? circle_filled(xc, yc, x, y, color) : circle_points(xc, yc, x, y, color); 
		//usleep(100000);
    } 
} 

void font_place_char(int x, int y, int font_width, int font_height, char ch)
{
	unsigned char bits;
	int bytes_per_row = font_width / 8;
	int yy = y;

	char* fontptr = get_font_ptr(ch);
	for( int i = 0; i < font_height; ++i)
	{	
		int xx1 = x;
		for(int b = 0; b < bytes_per_row; ++b)
		{
			bits = *(unsigned char*)fontptr++;
			//printf("%02X ", bits);
			for(int j = 0; j < 8; ++j)
			{
				if ((bits & 0x80) != 0)
				{
					put_pixel(xx1, yy, WHITE);
				}
				bits = bits << 1;
				++xx1;
			}
		}
		//printf("\r\n");
		++yy;
	}
}

void font_place_text(int x, int y, int font_width, int font_height, const char* text)
{
	while(char ch = *text++)
	{
		font_place_char(x, y, font_width, font_height, ch);
		x += font_width;
	}
}	
  

