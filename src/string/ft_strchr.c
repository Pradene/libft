#include "libft.h"

char	*ft_strchr(const char *str, int c) {
	int	i;
	int	size;

	if (!str) {
		return (0);
	}
	i = 0;
	size = (int)ft_strlen(str);
	while (i <= size) {
		if (str[i] == (char)c) {
			return ((char *)str + i);
		}
		i++;
	}
	return (0);
}
