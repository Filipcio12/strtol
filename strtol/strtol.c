#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int checkBase(const char** nPtr, char*** endPtr, int* base, char* c);
int checkBase0(const char** nPtr, char*** endPtr, int* base, char* c);
int checkBase16(const char** nPtr, char*** endPtr, int* base, char* c);

long strtol(const char* nPtr, char** endPtr, int base)
{
	errno = 0;
	char c = *nPtr;
	long value = 0;
	int sign = 1;

	// Incorrect base

	if ((base < 2 || base > 36) && base != 0) {
		errno = EINVAL;
		return 0;
	}

	// Skipping white space

	while (isspace(c)) {
		c = *(++nPtr);
	}

	// Checking sign

	if (c == '-') {
		sign = -1;
		c = *(++nPtr);
	}
	else if (c == '+') {
		c = *(++nPtr);
	}

	if (!checkBase(&nPtr, &endPtr, &base, &c)) {
		return 0;
	}

	int digit;

	if (isdigit(c)) {
		digit = c - '0';
	}
	else if (isalpha(c)) {
		digit = toupper(c) - 'A' + 10;
	}
	else {
		return 0;
	}

	if (digit >= base) {
		*endPtr = (char*)nPtr;
		return 0;
	}

	while (digit < base) {
		if (sign == 1) {
			if (value <= (LONG_MAX - digit) / base) {
				value = value * base + digit;
			}
			else {
				value = LONG_MAX;
				errno = ERANGE;
			}
		}
		else if (sign == -1) {
			if (value >= (LONG_MIN + digit) / base) {
				value = value * base - digit;
			}
			else {
				value = LONG_MIN;
				errno = ERANGE;
			}
		}

		c = *(++nPtr);

		if (isdigit(c)) {
			digit = c - '0';
		}
		else if (isalpha(c)) {
			digit = toupper(c) - 'A' + 10;
		}
		else {
			break;
		}
	}

	if (endPtr != NULL) {
		*endPtr = (char*)nPtr;
	}

	return value;
}

int checkBase16(const char** nPtr, char*** endPtr, int* base, char* c)
{
	int digit = *((*nPtr) + 2);

	if (isdigit(digit)) {
		digit = digit - '0';
	}
	else if (isalpha(digit)) {
		digit = toupper(digit) - 'A' + 10;
	}
	else {
		(*nPtr)++;
		*(*endPtr) = (char*)(*nPtr);
		return 0;
	}

	if (digit >= 16) {
		(*nPtr)++;
		*(*endPtr) = (char*)(*nPtr);
		return 0;
	}

	(*nPtr) += 2;
	(*c) = *(*nPtr);
	
	return 1;
}

int checkBase0(const char** nPtr, char*** endPtr, int* base, char* c)
{
	if ((*c) == '0' && (*((*nPtr) + 1) == 'x' || *((*nPtr) + 1) == 'X')) {
		int digit = *((*nPtr) + 2);

		if (isdigit(digit)) {
			digit = digit - '0';
		}
		else if (isalpha(digit)) {
			digit = toupper(digit) - 'A' + 10;
		}
		else {
			(*nPtr)++;
			*(*endPtr) = (char*)(*nPtr);
			return 0;
		}

		if (digit >= 16) {
			(*nPtr)++;
			*(*endPtr) = (char*)(*nPtr);
			return 0;
		}

		(*base) = 16;
		(*nPtr) += 2;
		(*c) = *(*nPtr);
	}
	else if ((*c) == '0') {
		(*base) = 8;
		(*c) = *(++(*nPtr));
	}
	else {
		(*base) = 10;
	}

	return 1;
}

int checkBase(const char** nPtr, char*** endPtr, int* base, char* c)
{
	if ((*base) == 0 && checkBase0(nPtr, endPtr, base, c) == 0) {
		return 0;
	}
	else if (
			(*base) == 16 && (*c) == '0' &&
			(*((*nPtr) + 1) == 'x' || *((*nPtr) + 1) == 'X') &&
			checkBase16(nPtr, endPtr, base, c) == 0
		) {
		return 0;
	}

	return 1;
}
