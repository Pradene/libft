#include "libft.h"

int ft_putsize_t(size_t n) {
    char buf[21];
    int i = 20;
    
    buf[i] = '\0';
    if (n == 0) {
        buf[--i] = '0';
    } else {
        while (n) {
            buf[--i] = (n % 10) + '0';
            n /= 10;
        }
    }
    
    return (ft_putstr(&buf[i]));
}