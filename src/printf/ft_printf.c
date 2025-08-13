#include "libft.h"

int	ft_putformated(va_list params, const char *s, int *i) {
	int		size = 0;
	char	c = s[*i];

	if (c == 'c') {
		size += ft_putchar(va_arg(params, int));
	} else if (c == 's') {
		size += ft_putstr(va_arg(params, char *));
	} else if (c == 'p') {
		size += ft_putptr(va_arg(params, unsigned long long));
	} else if (c == 'd' || c == 'i') {
		size += ft_putnbr(va_arg(params, int));
	} else if (c == 'u') {
		size += ft_putunbr(va_arg(params, unsigned int));
	} else if (c == 'x' || c == 'X') {
		size += ft_puthex(va_arg(params, unsigned long long));
	} else if (ft_strncmp(&s[*i], "zu", 2) == 0) {
		size += ft_putsize_t(va_arg(params, size_t));
		*i += 1;
	} else if (c == '%') {
		size += ft_putchar('%');
	}
	return (size);
}

int	ft_printf(const char *s, ...) {
	int		i;
	int		size;
	va_list	params;

	i = 0;
	size = 0;
	va_start(params, s);
	while (s[i]) {
		if (s[i] != '%') {
			size += ft_putchar(s[i]);
		} else {
			++i;
			size += ft_putformated(params, s, &i);
		}
		i++;
	}
	va_end(params);
	return (size);
}
