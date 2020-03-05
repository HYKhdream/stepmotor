#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define is_digit(c)   ((c) >= '0' && (c) <= '9')

int x_atoi(char *str)
{
	long val;
	char c;
	int radix;
	int s = 0;

	while ((c = *str) == ' ') str++;
	if (c == '-') {
		s = 1;
		c = *(++str);
	}
	else if (c == '+') {
		s = 0;
		c = *(++str);
	}
	if (c == '0') {
		c = *(++str);
		if (c <= ' ') {
			return 0;
		}
		else if (c == 'x') {
			radix = 16;
			c = *(++str);
		}
		else if (c == 'b') {
			radix = 2;
			c = *(++str);
		}
		else if (is_digit(c))
			radix = 8;
		else
			return 0;
	}
	else {
		if (!is_digit(c))
			return 0;
		radix = 10;
	}
	val = 0;
	while (c > ' ') {
		if (is_digit(c)) {
			c -= '0';
		}
		else {
			if (c >= 'a') c -= 0x20;
			if (c < 'A' || c > 'F')
			       	break;
			c -= 'A';
		       	c += 10;
		}
		if (c >= radix)
		       	break;
		val = val * radix + c;
		c = *(++str);
	}
	if (s)
		val = -val;
	return val;
} 

#if 0
//#define x_atoi	atoi
int main()
{
	printf("%d\n", x_atoi("  ABCD"));
	printf("%d\n", x_atoi("  -abCD"));
	printf("%d\n", x_atoi("  55SS"));
	printf("%d\n", x_atoi("  +55FF"));
	printf("%d\n", x_atoi("  -55BB"));
	printf("%d\n", x_atoi("  0xaWW"));
	printf("%d\n", x_atoi("  -055TT"));
	printf("%d\n", x_atoi("  0b1001G"));

	return 0;
}
#endif

