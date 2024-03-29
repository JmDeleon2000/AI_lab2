#include "BMP.h"
#include <fstream>
#include <iostream>




// implementation for the BMP_image interface
// default contructor
BMP_image::BMP_image() {}

BMP_image::BMP_image(const char* filename)
{
	using namespace std;

	int headerSize;
	ifstream text;
	text.open(filename, ios::binary);

	text.seekg(10);
	text.read((char*)&headerSize, 4);
	text.seekg(18);
	text.read((char*)&width, 4);
	text.read((char*)&height, 4);
	text.seekg(headerSize);
	image = new BGR_pixel * [height];

	int i, j = 0;
	while (j < height)
	{
		i = 0;
		image[j] = new BGR_pixel[width];
		while (i < width)
		{
			text.read((char*)&image[j][i], 3);
			i++;
		}
		j++;
	}
	text.close();
}

BMP_image::BMP_image(int width, int height)
{
	int i, j = 0;
	this->width = width;
	this->height = height;
	image = new BGR_pixel * [height];
	while (j < height)
	{
		i = 0;
		image[j] = new BGR_pixel[width];
		while (i < width)
		{
			image[j][i].b = 255;
			image[j][i].g = 255;
			image[j][i].r = 255;
			i++;
		}
		j++;
	}
}

// outputs data from a BMP_image object to a .bmp file
void BMP_image::output_BMP(BMP_image* img, const char* filename)
{
	unsigned char header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char headerinfo[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	const int filesize = img->width * img->height * pixel_size;


	*(int*)(header + 2) = filesize;

	*(int*)(headerinfo + 4) = img->width;

	*(int*)(headerinfo + 8) = img->height;

	FILE* dump;
	if (fopen_s(&dump, filename, "wb"))
	{
		std::cerr << "error outputing bmp" << std::endl;
		return;
	}

	fwrite(header, 1, 14, dump);
	fwrite(headerinfo, 1, 40, dump);

	int i, j = 0;
	while (j < img->height)
	{
		i = 0;
		while (i < img->width)
		{
			fwrite(&img->image[j][i], 1, pixel_size, dump);
			i++;
		}
		j++;
	}

	fclose(dump);
	//for (int i = 0; i < img->width; i++)
	//	delete[] img->image[i];
	//delete[] img->image;
}

