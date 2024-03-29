#pragma once
#include <vector>

//defines the interface for managing BMP images (task 1.1)

// defines the pixel size as 3 bytes per pixel
#define pixel_size  3 

// represents a pixel as stored by the .bmp format
struct BGR_pixel
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};



//class that opens and holds bmp image information
class BMP_image 
{
public:
	int width = 0;
	int height = 0;
	BGR_pixel** image = nullptr;

public:
	BMP_image();
	BMP_image(const char* filename);
	BMP_image(int, int);
	static void output_BMP(BMP_image* img, const char* filename);
};


