#include "libft.h"

int	ft_putchar(char c) {
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *s) {
	if (!s) {
		return (ft_putstr("(null)"));
	}
	write(1, s, ft_strlen(s));
	return (ft_strlen(s));
}
