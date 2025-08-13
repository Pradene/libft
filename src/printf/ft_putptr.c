#include "libft.h"

int	ft_putptr(unsigned long long n) {
	int	size;

	size = 0;
	if (n == 0) {
		size += ft_putstr("(nil)");
	} else {
		size += write(1, "0x", 2);
		size += ft_puthex(n);
	}
	return (size);
}
