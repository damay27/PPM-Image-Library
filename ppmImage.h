//Daniel May - ppm Image Loader Class

#pragma once
#include <string>
#include <vector>
#include "badImageFile.h"
using namespace std;

class ppmImage
{
public:
	ppmImage();
	ppmImage(string fileName);
	ppmImage(int width, int height);


	~ppmImage();

	void getData(unsigned char** Pointer, int channel);

	vector<unsigned char> getPixel(int x, int y);
	unsigned char getPixelChannel(int x, int y, int channel);


	void setPixel(unsigned char red, unsigned char green, unsigned char blue, int x, int y);
	void setPixelChannel(unsigned char value, int x, int y, int channel);

	void createImage(int width, int height);
	void fillImage(unsigned char red, unsigned char green, unsigned char blue);

	void loadImage(string fileName);
	void saveImage(string fileName);

	int getWidth();
	int getHeight();

private:
	int width, height;
	vector<unsigned char> redVector, greenVector, blueVector;
};

