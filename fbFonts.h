#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdint>

unsigned short V1 = 0x0436;
unsigned long V2 = 0x864ab572;

#define PSF1_FONT_MAGIC 0x0436
#define PSF_FONT_MAGIC 0x864ab572
 
typedef struct {
    uint16_t magic; // Magic bytes for identification.
    uint8_t fontMode; // PSF font mode.
    uint8_t characterSize; // PSF character size.
} PSF1_Header;
 
typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
} PSF_font;

PSF_font*	PSFfont;
FILE* fp;
char* fontbuf;

int load_font(const char* filename)
{
struct stat st;
int size;
	
	int result = stat(filename, &st);
	//printf("Result: %d\r\n", result);
	if (result != 0)
	{
		printf("File not found!\r\n");
		return -1;
	}
	
	size = st.st_size;
	printf("Size of %s: %d\r\n", filename, size);
	
	fp = fopen(filename,"rb"); /*open file*/

    if (fp == NULL)
	{
        printf("Error: There was an Error reading the file %s \n", filename);           
        exit(1);
    }
	
	fontbuf = (char*)malloc(size);
    int bytes_read = fread(fontbuf, sizeof(unsigned char), size, fp);
	if (bytes_read != size)
	{ /* if count of read bytes != calculated size of .bin file -> ERROR*/
        printf("Error: There was an Error reading the file\r\n");
        exit(1);
    }
	
	if (*(unsigned long *)fontbuf == V2)
		printf("Version 2\r\n");
	else if (*(unsigned short *)fontbuf == V1)
		printf("Version 2\r\n");
	else
	{
		printf("Unknown version\r\n");
		exit(3);
	}
	
	PSFfont = (PSF_font *)fontbuf;
	
	printf("%d, %d, %d, %d, %d\r\n", PSFfont->headersize, PSFfont->numglyph, PSFfont->bytesperglyph, PSFfont->height, PSFfont->width);

	return 0;
}

void close_font()
{
	free(fontbuf);
}

char* get_font_ptr(int charId)
{
	int offset = PSFfont->headersize + (PSFfont->bytesperglyph * charId) + 12;
	//printf("Offset: %d : %X\r\n", charId, offset);
	return (char*)&fontbuf[offset];
}
