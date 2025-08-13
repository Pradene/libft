#include "libft.h"

int	ft_puthex(unsigned long long n) {
	char	buf[17];
	int		i;

	buf[16] = '\0';
	i = 16;
	if (n == 0) {
		buf[--i] = '0';
	}
	while (n) {
		unsigned digit = n % 16;
		if (digit < 10) {
			buf[--i] = digit + '0';
		} else {
			buf[--i] = digit - 10 + 'A';
		}
		n /= 16;
	}
	return (ft_putstr(&buf[i]));
}

