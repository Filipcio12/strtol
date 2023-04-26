#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

long strtol(const char* nPtr, char** endPtr, int base)
{
	char c = *nPtr;
	long value = 0;
	int sign = 1;
	
	// Incorrect base

	if ((base < 2 || base > 36) && base != 0)
	{
		return 0;
	}

	// Skipping white space

	while (isspace(c))
	{
		c = *(++nPtr);
	}

	// Checking sign

	if (c == '-')
	{
		sign = -1;
		c = *(++nPtr);
	}
	else if (c == '+')
	{
		c = *(++nPtr);
	}

	// Checking base

	if (base == 0)
	{
		if (c == '0' && (*(nPtr + 1) == 'x' || *(nPtr + 1) == 'X'))
		{
			base = 16;
			nPtr += 2;
			c = *nPtr;
		}
		else if (c == '0')
		{
			base = 8;
			c = *(++nPtr);
		}
		else
		{
			base = 10;
		}
	}
	else if (base == 16 && c == '0' && (*(nPtr + 1) == 'x' || *(nPtr + 1) == 'X'))
	{
		nPtr += 2;
		c = *nPtr;
	}
	
	
	
	while (isalnum(c))
	{
		if (isdigit(c) && value <= (LONG_MAX - (c - '0')) / base)
		{
			value = value * base + (c - '0');
		}
		else if (isalpha(c) && value <= (LONG_MAX - (toupper(c) - 'A' + 10)) / base)
		{
			value = value * base + (toupper(c) - 'A' + 10);
		}
		else
		{
			value = LONG_MAX;
		}
		
		
		c = *(++nPtr);
	}

	if (sign == 1 && value < 0)
	{
		value *= (-1);
	}
	
	
	if (endPtr != NULL)
	{
		*endPtr = (char*) nPtr;
	}

	return value * sign;
}
