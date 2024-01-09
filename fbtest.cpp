#include <stdio.h>
#include <stdlib.h>

#include "framebuffer.h"
#include "fbFonts.h"
#include "fbGraphics.h"

// gcc fbtest.cpp -o fbtest -lstdc++

int main()
{
	int result = load_font("TB14.psf");
	if (result < 0) exit(1);

	if (open_framebuffer("/dev/fb0") == -1)
		exit(2);
	
	fbpointer = fbp;
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
	// text in rectangle
	font_place_text(510, 315, "Text in Box");
	//font_place_char(510, 315, 'A');
	
	sleep(5);
	
	close_framebuffer();
	close_font();
    return 0;
  
}
