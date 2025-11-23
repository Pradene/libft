#include "libft.h"

int ft_strrcmp(const char *a, const char *b) {
    unsigned char   a_char;
    unsigned char   b_char;
    size_t          a_len;
    size_t          b_len;

    a_len = ft_strlen(a);
    b_len = ft_strlen(b);
    while (a_len > 0 && b_len > 0) {
        a_char = a[--a_len];
        b_char = b[--b_len];
        if (a_char != b_char) {
            return (a_char - b_char);
        }
    }
    return (a_len - b_len);
}
