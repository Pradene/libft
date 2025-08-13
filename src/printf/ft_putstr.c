#include "libft.h"

int	ft_putstr(char *s) {
	if (!s) {
		return (ft_putstr("(null)"));
	}
	write(1, s, ft_strlen(s));
	return (ft_strlen(s));
}
