#pragma once
#include <exception>
using namespace std;

class badImageFile : public exception
{
public:
	badImageFile(const char input[]);
	~badImageFile();

	virtual const char* what();


private:
	char message[1000];
};

