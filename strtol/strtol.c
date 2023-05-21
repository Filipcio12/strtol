#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int checkBase(const char** nPtr, char*** endPtr, int* base, char* c);
int checkBase0(const char** nPtr, char*** endPtr, int* base, char* c);
int checkBase16(const char** nPtr, char*** endPtr, int* base, char* c);
void addDigit(long* value, int d, int sign, int base);

long strtol(const char* nPtr, char** endPtr, int base)
{
	errno = 0;
	char c = *nPtr;
	long value = 0;
	int sign = 1;

	if ((base < 2 || base > 36) && base != 0) {
		errno = EINVAL;
		return 0;
	}

	while (isspace(c)) {
		c = *(++nPtr);
	}

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

	if (!isalnum(c)) {
		return 0;
	}

	int d = isdigit(c) ? (c - '0') : (isalpha(c) ? (toupper(c) - 'A' + 10) : 0);

	while (isalnum(c) && d < base) {
		addDigit(&value, d, sign, base);
		c = *(++nPtr);
		d = isdigit(c) ? (c - '0') : (isalpha(c) ? (toupper(c) - 'A' + 10) : 0);
	}

	if (endPtr != NULL) {
		*endPtr = (char*)nPtr;
	}

	return value;
}

void addDigit(long* value, int d, int sign, int base)
{
	if (sign == 1) {
			if ((*value) <= (LONG_MAX - d) / base) {
				(*value) = (*value) * base + d;
			}
			else {
				(*value) = LONG_MAX;
				errno = ERANGE;
			}
		}
		else if (sign == -1) {
			if ((*value) >= (LONG_MIN + d) / base) {
				(*value) = (*value) * base - d;
			}
			else {
				(*value) = LONG_MIN;
				errno = ERANGE;
			}
		}
}

int checkBase16(const char** nPtr, char*** endPtr, int* base, char* c)
{
	int d = *((*nPtr) + 2);

	if (isdigit(d)) {
		d = d - '0';
	}
	else if (isalpha(d)) {
		d = toupper(d) - 'A' + 10;
	}
	else {
		(*nPtr)++;
		*(*endPtr) = (char*)(*nPtr);
		return 0;
	}

	if (d >= 16) {
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
		int d = *((*nPtr) + 2);

		if (isdigit(d)) {
			d = d - '0';
		}
		else if (isalpha(d)) {
			d = toupper(d) - 'A' + 10;
		}
		else {
			(*nPtr)++;
			*(*endPtr) = (char*)(*nPtr);
			return 0;
		}

		if (d >= 16) {
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
