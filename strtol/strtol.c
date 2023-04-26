#include <ctype.h>
#include <stdlib.h>

long strtol(const char* nPtr, char** endPtr, int base)
{
	int value = 0;
	char c = *nPtr;

	while (isalnum(c))
	{
		if (isdigit(c))
		{
			value = value * base + (c - '0');
		}
		else if (isalpha(c))
		{
			value = value * base + (toupper(c) - 'A');
		}
		
		c = *(++nPtr);
	}
	
	if (endPtr != NULL)
	{
		*endPtr = (char*) nPtr;
	}

	return value;
}
