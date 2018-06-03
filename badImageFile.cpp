#include "badImageFile.h"
#include <string>
using namespace std;



badImageFile::badImageFile(const char input[])
{
	strcpy_s(message, input);
}


badImageFile::~badImageFile()
{
}

const char* badImageFile::what()
{
	return message;
}
