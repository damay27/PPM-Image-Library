#include "ppmImage.h"
#include <fstream>.
using namespace std;



ppmImage::ppmImage()
{

}

ppmImage::ppmImage(string fileName)
{
	loadImage(fileName);
}

ppmImage::ppmImage(int width, int height)
{
	createImage(width, height);
}


ppmImage::~ppmImage()
{

}


//sets the input pointer to point to the memory used by the color channel vectors
void ppmImage::getData(unsigned char** Pointer, int channel)
{
	switch (channel)
	{
	case 0:
		*Pointer = redVector.data();
		break;
	case 1:
		*Pointer = greenVector.data();
		break;
	case 2:
		*Pointer = blueVector.data();
		break;
	default:
		*Pointer = NULL;
	}

}


int ppmImage::getWidth()
{
	return width;
}

int ppmImage::getHeight()
{
	return height;
}


//read the RGB values of a pixel with the results returned as vectors
vector<unsigned char> ppmImage::getPixel(int x, int y)
{
	unsigned char red = redVector.at(y*width + x);
	unsigned char green = greenVector.at(y*width + x);
	unsigned char blue = blueVector.at(y*width + x);

	return vector<unsigned char>({ red, green, blue });

}

//read only one channel of a pixel
unsigned char ppmImage::getPixelChannel(int x, int y, int channel)
{
	switch (channel)
	{
	case 0:
		return redVector.at(y*width + x);
		break;
	case 1:
		return greenVector.at(y*width + x);
		break;
	case 2:
		return blueVector.at(y*width + x);
		break;
	default:
		return 0;
	}
}


//set all channels of a pixel at once
void ppmImage::setPixel(unsigned char red, unsigned char green, unsigned char blue, int x, int y)
{
	redVector.at(y*width + x) = red;
	greenVector.at(y*width + x) = green;
	blueVector.at(y*width + x) = blue;
}


//this function lets you set the value of each color channel of a pixel indevidually
void ppmImage::setPixelChannel(unsigned char value, int x, int y, int channel)
{
	switch (channel)
	{
	case 0:
		redVector.at(y*width + x) = value;
		break;
	case 1:
		greenVector.at(y*width + x) = value;
		break;
	case 2:
		blueVector.at(y*width + x) = value;
		break;
	}
}



void ppmImage::createImage(int width, int height)
{
	//set the member variables
	this->width = width;
	this->height = height;

	//allocate and fill the vectors
	redVector.resize(width*height);
	greenVector.resize(width*height);
	blueVector.resize(width*height);
}


void ppmImage::fillImage(unsigned char red, unsigned char green, unsigned char blue)
{
	//fill each channel with the input value
	fill(redVector.begin(), redVector.end(), red);
	fill(greenVector.begin(), greenVector.end(), green);
	fill(blueVector.begin(), blueVector.end(), blue);
}


//This function takes the path to an image file as input
//and loads the data from that file into the object
void ppmImage::loadImage(string fileName)
{
	//open the file and make sure it opened correctly
	ifstream file;
	file.open(fileName);


	//make sure the file is not empty
	file.seekg(0, ios::end);
	int end = file.tellg();
	if (end <= 0)
	{
		throw badImageFile("FILE EMPTY");
	}

	file.seekg(0, ios::beg);

	string line;

	//skip all comment lines
	getline(file, line);
	while (line.at(0) == '#') {
		getline(file, line);
	}

	//check that the header is correct
	if (line != "P3")
	{
		throw badImageFile("BAD HEARDER");
	}



	//skip all comment lines
	getline(file, line);
	while (line.at(0) == '#') {
		getline(file, line);
	}

	int lineLength = line.size();


	//separate the numbers at the whitespace and convert them to int
	for (int i = 0; i < lineLength; i++)
	{
		if (line[i] == ' ')
		{
			string num1String = line.substr(0, i);
			string num2String = line.substr(i + 1, lineLength);

			width = stoi(num1String);
			height = stoi(num2String);
			break;
		}
	}

	//skip all comment lines
	getline(file, line);
	while (line.at(0) == '#') {
		getline(file, line);
	}

	//check and make sure the color depth is no more than 8 bit
	int colorDepth = stoi(line);
	if (colorDepth > 255)
	{
		throw badImageFile("COLOR DEPTH > 8bit");
	}


	redVector.resize(width*height);
	greenVector.resize(width*height);
	blueVector.resize(width*height);

	//start reading pixel data
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{

			char c = 0;
			string number;
			//RGB holds the value of which channel the data belongs to
			char RGB = 0;

			//read the file one character at a time
			while (RGB<3)
			{
				c = (char)file.get();

				//when you reach a newline or space
				if (c == ' ' || c == '\n' || c == '\t') {


					//convert the number to an int and place it in the correct vector for its color
					unsigned char value = (unsigned char)stoi(number);
					number = "";

					switch (RGB)
					{
					case 0:
						redVector.at(row * width + col) = value;
						break;
					case 1:
						greenVector.at(row * width + col) = value;
						break;
					case 2:
						blueVector.at(row * width + col) = value;
						break;
					}

					//increment RGB and reset if needed
					RGB++;

				}
				//if the number has not been fully read yet place the character in the number string
				else
				{
					number.push_back(c);
				}

			}

		}
	}

	//close the file
	file.close();
}


void ppmImage::saveImage(string fileName)
{
	ofstream file;
	file.open(fileName);


	//write the header, image size, and color depth to the file
	file << "P3\n";
	file << "#Saved with ppmImage library\n";
	file << to_string(width) << " " << to_string(height) << "\n";
	file << "255\n";


	//loop through the image data and write each pixel separated by tabs
	//rows are separated by newline characters
	for (int row = 0;row < height;row++)
	{
		for (int col = 0;col < width;col++)
		{
			unsigned char red = redVector.at(row*width + col);
			unsigned char green = greenVector.at(row*width + col);
			unsigned char blue = blueVector.at(row*width + col);

			file << to_string(red) << ' ' << to_string(green) << ' ' << to_string(blue);


			//end each pixel with a tab unless at the end of a row, then use a newline
			if (col < width - 1)
			{
				file << '\t';
			}
			else
			{
				file << '\n';
			}
		}

	}

	//close the file
	file.close();

}