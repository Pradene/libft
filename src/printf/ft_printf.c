#include "libft.h"

// Output function implementations
static int output_to_stdout(void *output, const char *data, size_t len) {
    (void)output;
    return write(1, data, len);
}

static int output_to_fd(void *output, const char *data, size_t len) {
    int fd = *(int *)output;
    return write(fd, data, len);
}

static int output_to_file(void *output, const char *data, size_t len) {
    FILE *file = (FILE *)output;
    return (fwrite(data, 1, len, file));
}

// Generic output functions with consistent parameter order
int ft_printf_putchar(void *output, output_func output_func, char c) {
    return output_func(output, &c, 1);
}

PrintfFormat ft_printf_format_create() {
	PrintfFormat fmt;
    fmt.flags.minus = 0;
    fmt.flags.zero = 0;
    fmt.flags.hash = 0;
    fmt.flags.space = 0;
    fmt.flags.plus = 0;
    fmt.width = 0;
    fmt.precision = -1;
    fmt.modifier = MOD_NONE;
    fmt.specifier = SPEC_NONE;
	return (fmt);
}

long long apply_signed_modifier(va_list params, PrintfModifier mod) {
    switch (mod) {
        case MOD_HH: return ((signed char)va_arg(params, int));
        case MOD_H: return ((short)va_arg(params, int));
        case MOD_L: return (va_arg(params, long));
        case MOD_LL: return (va_arg(params, long long));
        case MOD_J: return (va_arg(params, long long));
        case MOD_Z: return (va_arg(params, size_t));
        case MOD_T: return (va_arg(params, ptrdiff_t));
        default: return (va_arg(params, int));
    }
}

unsigned long long apply_unsigned_modifier(va_list params, PrintfModifier mod) {
    switch (mod) {
        case MOD_HH: return ((unsigned char)va_arg(params, unsigned int));
        case MOD_H: return ((unsigned short)va_arg(params, unsigned int));
        case MOD_L: return (va_arg(params, unsigned long));
        case MOD_LL: return (va_arg(params, unsigned long long));
        case MOD_J: return (va_arg(params, unsigned long long));
        case MOD_Z: return (va_arg(params, size_t));
        case MOD_T: return (va_arg(params, ptrdiff_t));
        default: return (va_arg(params, unsigned int));
    }
}

int ft_printf_putstr(void *output, output_func output_func, const char *s) {
    if (!s) return 0;
    int len = ft_strlen(s);
    return output_func(output, s, len);
}

int ft_printf_padding(void *output, output_func output_func, int count, char c) {
    int printed = 0;
    while (count-- > 0) {
        int result = ft_printf_putchar(output, output_func, c);
        if (result < 0) return result;
        printed += result;
    }
    return printed;
}

static int put_hex_digits(void *output, output_func output_func, unsigned long long n, char c) {
    char digits[] = "0123456789abcdef";
    if (c == 'X') {
        digits[10] = 'A'; digits[11] = 'B'; digits[12] = 'C';
        digits[13] = 'D'; digits[14] = 'E'; digits[15] = 'F';
    }
    
    if (n >= 16) {
        int left = put_hex_digits(output, output_func, n / 16, c);
        int right = put_hex_digits(output, output_func, n % 16, c);
        if (left < 0 || right < 0) return -1;
        return left + right;
    } else {
        return ft_printf_putchar(output, output_func, digits[n]);
    }
}

static int ft_puthex(void *output, output_func output_func, unsigned long long n, char c) {
    if (n == 0) {
        return ft_printf_putchar(output, output_func, '0');
    } else {
        return put_hex_digits(output, output_func, n, c);
    }
}

static int put_unsigned_digits(void *output, output_func output_func, unsigned long long n) {
    int printed = 0;
    if (n >= 10) {
        int left = put_unsigned_digits(output, output_func, n / 10);
        if (left < 0) return left;
        printed += left;
    }
    int digit = ft_printf_putchar(output, output_func, '0' + n % 10);
    if (digit < 0) return digit;
    return printed + digit;
}

int ft_putchar_formatted(void *output, output_func output_func, PrintfFormat *fmt, char c) {
    int printed = 0;
    int padding = (fmt != NULL) ? (fmt->width - 1) : 0;
    
    if (padding > 0) {
        if (fmt->flags.minus) {
            int char_result = ft_printf_putchar(output, output_func, c);
            if (char_result < 0) return char_result;
            printed += char_result;
            
            int pad_result = ft_printf_padding(output, output_func, padding, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        } else {
            int pad_result = ft_printf_padding(output, output_func, padding, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
            
            int char_result = ft_printf_putchar(output, output_func, c);
            if (char_result < 0) return char_result;
            printed += char_result;
        }
    } else {
        int char_result = ft_printf_putchar(output, output_func, c);
        if (char_result < 0) return char_result;
        printed += char_result;
    }
    return printed;
}

int ft_putstr_formatted(void *output, output_func output_func, PrintfFormat *fmt, char *s) {
    int len, print_len;
    int printed = 0;
    
    if (!s) s = "(null)";
    
    len = ft_strlen(s);
    print_len = (fmt->precision >= 0 && fmt->precision < len) ? fmt->precision : len;
    
    int padding = fmt->width - print_len;
    
    if (!fmt->flags.minus && padding > 0) {
        int pad_result = ft_printf_padding(output, output_func, padding, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    for (int i = 0; i < print_len; i++) {
        int char_result = ft_printf_putchar(output, output_func, s[i]);
        if (char_result < 0) return char_result;
        printed += char_result;
    }
    
    if (fmt->flags.minus && padding > 0) {
        int pad_result = ft_printf_padding(output, output_func, padding, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    return printed;
}

int ft_putnbr_formatted(void *output, output_func output_func, PrintfFormat *fmt, long long n) {
    int printed = 0;
    int is_negative = (n < 0);
    unsigned long long num = (is_negative) ? -n : n;
    int len = 0;
    unsigned long long temp = num;
    int sign_len = (is_negative || fmt->flags.plus || fmt->flags.space) ? 1 : 0;
    
    // Calculate length
    if (num == 0) {
        len = 1;
    } else {
        while (temp) {
            len++;
            temp /= 10;
        }
    }
    
    if (fmt->precision == 0 && num == 0) {
        len = 0;
        int total_len = sign_len;
        int pad = fmt->width - total_len;
        
        if (!fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        if (is_negative) {
            int sign_result = ft_printf_putchar(output, output_func, '-');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        } else if (fmt->flags.plus) {
            int sign_result = ft_printf_putchar(output, output_func, '+');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        } else if (fmt->flags.space) {
            int sign_result = ft_printf_putchar(output, output_func, ' ');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        }
        
        if (fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        return printed;
    }
    
    int actual_len = (fmt->precision > len) ? fmt->precision : len;
    int total_len = actual_len + sign_len;
    int pad = fmt->width - total_len;
    
    if (!fmt->flags.minus) {
        if (fmt->flags.zero && fmt->precision < 0) {
            if (is_negative) {
                int sign_result = ft_printf_putchar(output, output_func, '-');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            } else if (fmt->flags.plus) {
                int sign_result = ft_printf_putchar(output, output_func, '+');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            } else if (fmt->flags.space) {
                int sign_result = ft_printf_putchar(output, output_func, ' ');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            }
            if (pad > 0) {
                int pad_result = ft_printf_padding(output, output_func, pad, '0');
                if (pad_result < 0) return pad_result;
                printed += pad_result;
            }
        } else {
            if (pad > 0) {
                int pad_result = ft_printf_padding(output, output_func, pad, ' ');
                if (pad_result < 0) return pad_result;
                printed += pad_result;
            }
            if (is_negative) {
                int sign_result = ft_printf_putchar(output, output_func, '-');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            } else if (fmt->flags.plus) {
                int sign_result = ft_printf_putchar(output, output_func, '+');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            } else if (fmt->flags.space) {
                int sign_result = ft_printf_putchar(output, output_func, ' ');
                if (sign_result < 0) return sign_result;
                printed += sign_result;
            }
        }
    } else {
        if (is_negative) {
            int sign_result = ft_printf_putchar(output, output_func, '-');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        } else if (fmt->flags.plus) {
            int sign_result = ft_printf_putchar(output, output_func, '+');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        } else if (fmt->flags.space) {
            int sign_result = ft_printf_putchar(output, output_func, ' ');
            if (sign_result < 0) return sign_result;
            printed += sign_result;
        }
    }
    
    if (fmt->precision > len) {
        int pad_result = ft_printf_padding(output, output_func, fmt->precision - len, '0');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (num == 0) {
        int zero_result = ft_printf_putchar(output, output_func, '0');
        if (zero_result < 0) return zero_result;
        printed += zero_result;
    } else {
        char digits[21];
        int i = 20;
        digits[i] = '\0';
        while (num) {
            digits[--i] = '0' + (num % 10);
            num /= 10;
        }
        int str_result = ft_printf_putstr(output, output_func, &digits[i]);
        if (str_result < 0) return str_result;
        printed += str_result;
    }
    
    if (fmt->flags.minus && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    return printed;
}

int ft_putunbr_formatted(void *output, output_func output_func, PrintfFormat *fmt, unsigned long long n) {
    int printed = 0;
    int len = 0;
    unsigned long long temp = n;
    
    if (n == 0) {
        len = 1;
    } else {
        while (temp) {
            len++;
            temp /= 10;
        }
    }
    
    if (fmt->precision == 0 && n == 0) {
        len = 0;
        int pad = fmt->width;
        
        if (!fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        if (fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        return printed;
    }
    
    int actual_len = (fmt->precision > len) ? fmt->precision : len;
    int pad = fmt->width - actual_len;
    
    if (!fmt->flags.minus && pad > 0) {
        char pad_char = (fmt->flags.zero && fmt->precision < 0) ? '0' : ' ';
        int pad_result = ft_printf_padding(output, output_func, pad, pad_char);
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (fmt->precision > len) {
        int pad_result = ft_printf_padding(output, output_func, fmt->precision - len, '0');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (n == 0) {
        int zero_result = ft_printf_putchar(output, output_func, '0');
        if (zero_result < 0) return zero_result;
        printed += zero_result;
    } else {
        int num_result = put_unsigned_digits(output, output_func, n);
        if (num_result < 0) return num_result;
        printed += num_result;
    }
    
    if (fmt->flags.minus && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    return printed;
}

int ft_puthex_formatted(void *output, output_func output_func, PrintfFormat *fmt, unsigned long long n, char c) {
    int printed = 0;
    int len = 0;
    unsigned long long temp = n;
    int prefix_len = (fmt->flags.hash && n != 0) ? 2 : 0;
    
    if (n == 0) {
        len = 1;
    } else {
        while (temp) {
            len++;
            temp /= 16;
        }
    }
    
    if (fmt->precision == 0 && n == 0) {
        len = 0;
    }
    
    int actual_len = (fmt->precision > len) ? fmt->precision : len;
    int total_len = actual_len + prefix_len;
    int pad = fmt->width - total_len;
    
    if (!fmt->flags.minus && pad > 0) {
        char pad_char = (fmt->flags.zero && fmt->precision < 0) ? '0' : ' ';
        if (pad_char == '0' && prefix_len > 0) {
            int zero_result = ft_printf_putchar(output, output_func, '0');
            if (zero_result < 0) return zero_result;
            printed += zero_result;
            
            int x_result = ft_printf_putchar(output, output_func, c == 'x' ? 'x' : 'X');
            if (x_result < 0) return x_result;
            printed += x_result;
            prefix_len = 0;
        }
        int pad_result = ft_printf_padding(output, output_func, pad, pad_char);
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (prefix_len > 0) {
        int zero_result = ft_printf_putchar(output, output_func, '0');
        if (zero_result < 0) return zero_result;
        printed += zero_result;
        
        int x_result = ft_printf_putchar(output, output_func, c == 'x' ? 'x' : 'X');
        if (x_result < 0) return x_result;
        printed += x_result;
    }
    
    if (fmt->precision > len) {
        int pad_result = ft_printf_padding(output, output_func, fmt->precision - len, '0');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (!(fmt->precision == 0 && n == 0)) {
        int hex_result = ft_puthex(output, output_func, n, c);
        if (hex_result < 0) return hex_result;
        printed += hex_result;
    }
    
    if (fmt->flags.minus && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    return printed;
}

int ft_putoct_formatted(void *output, output_func output_func, PrintfFormat *fmt, unsigned long long n) {
    int printed = 0;
    int len = 0;
    unsigned long long temp = n;
    
    // Calculate length
    len = (n == 0) ? 1 : 0;
    while (temp) {
        len++;
        temp /= 8;
    }
    
    int prefix_len = (fmt->flags.hash && n != 0) ? 1 : 0;
    int actual_len = (fmt->precision > len) ? fmt->precision : len;
    int total_len = actual_len + prefix_len;
    int pad = fmt->width - total_len;
    
    if (!fmt->flags.minus && !fmt->flags.zero && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (prefix_len) {
        int zero_result = ft_printf_putchar(output, output_func, '0');
        if (zero_result < 0) return zero_result;
        printed += zero_result;
    }
    
    if (!fmt->flags.minus && fmt->flags.zero && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, '0');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (fmt->precision > len) {
        int pad_result = ft_printf_padding(output, output_func, fmt->precision - len, '0');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    if (n == 0) {
        int zero_result = ft_printf_putchar(output, output_func, '0');
        if (zero_result < 0) return zero_result;
        printed += zero_result;
    } else {
        char digits[23];
        int i = 22;
        digits[i] = '\0';
        while (n) {
            digits[--i] = '0' + (n % 8);
            n /= 8;
        }
        int str_result = ft_printf_putstr(output, output_func, &digits[i]);
        if (str_result < 0) return str_result;
        printed += str_result;
    }
    
    if (fmt->flags.minus && pad > 0) {
        int pad_result = ft_printf_padding(output, output_func, pad, ' ');
        if (pad_result < 0) return pad_result;
        printed += pad_result;
    }
    
    return printed;
}

int ft_putptr_formatted(void *output, output_func output_func, PrintfFormat *fmt, unsigned long long n) {
    int printed = 0;
    int hex_len = 0;
    unsigned long long temp = n;
    
    if (n == 0) {
        char *nil_str = "(nil)";
        int len = 5;
        int pad = fmt->width - len;
        
        if (!fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        int str_result = ft_printf_putstr(output, output_func, nil_str);
        if (str_result < 0) return str_result;
        printed += str_result;
        
        if (fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
    } else {
        temp = n;
        hex_len = (n == 0) ? 1 : 0;
        while (temp) {
            hex_len++;
            temp /= 16;
        }
        
        int total_len = hex_len + 2;
        int pad = fmt->width - total_len;
        
        if (!fmt->flags.minus && pad > 0) {
            char pad_char = fmt->flags.zero ? '0' : ' ';
            int pad_result = ft_printf_padding(output, output_func, pad, pad_char);
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
        
        int prefix_result = ft_printf_putstr(output, output_func, "0x");
        if (prefix_result < 0) return prefix_result;
        printed += prefix_result;
        
        int hex_result = ft_puthex(output, output_func, n, 'x');
        if (hex_result < 0) return hex_result;
        printed += hex_result;
        
        if (fmt->flags.minus && pad > 0) {
            int pad_result = ft_printf_padding(output, output_func, pad, ' ');
            if (pad_result < 0) return pad_result;
            printed += pad_result;
        }
    }
    return printed;
}

int ft_printf_format(void *output, output_func output_func, va_list params, PrintfFormat *fmt) {
    switch (fmt->specifier) {
        case SPEC_C: return ft_putchar_formatted(output, output_func, fmt, va_arg(params, int));
        case SPEC_S: return ft_putstr_formatted(output, output_func, fmt, va_arg(params, char *));
        case SPEC_P: return ft_putptr_formatted(output, output_func, fmt, va_arg(params, unsigned long long));
        case SPEC_D:
        case SPEC_I: return ft_putnbr_formatted(output, output_func, fmt, apply_signed_modifier(params, fmt->modifier));
        case SPEC_U: return ft_putunbr_formatted(output, output_func, fmt, apply_unsigned_modifier(params, fmt->modifier));
        case SPEC_O: return ft_putoct_formatted(output, output_func, fmt, apply_unsigned_modifier(params, fmt->modifier));
        case SPEC_UPPER_X:
        case SPEC_X: return ft_puthex_formatted(output, output_func, fmt, apply_unsigned_modifier(params, fmt->modifier), (fmt->specifier == SPEC_X) ? 'x' : 'X');
        case SPEC_PERCENT: return ft_putchar_formatted(output, output_func, fmt, '%');
        default: return 0;
    }
}

int ft_printf_generic(void *output, output_func output_func, const char *format, va_list params) {
    int printed = 0;
    PrintfFormat fmt;
    
    int i = 0;
    while (format[i]) {
        if (format[i] != '%') {
            int result = ft_printf_putchar(output, output_func, format[i]);
            if (result < 0) return result;
            printed += result;
        } else {
            ++i;
            fmt = ft_printf_format_create();
            ft_printf_parse_format(format, &i, params, &fmt);
            int result = ft_printf_format(output, output_func, params, &fmt);
            if (result < 0) return result;
            printed += result;
        }
        ++i;
    }
    
    return printed;
}

int ft_printf(const char *format, ...) {
    va_list params;
    int result;
    
    va_start(params, format);
    result = ft_printf_generic(NULL, output_to_stdout, format, params);
    va_end(params);
    
    return result;
}

int ft_fprintf(FILE *stream, const char *format, ...) {
    va_list params;
    int result;
    
    if (!stream) return -1;
    
    va_start(params, format);
    result = ft_printf_generic(stream, output_to_file, format, params);
    va_end(params);
    
    return result;
}

int ft_dprintf(int fd, const char *format, ...) {
    va_list params;
    int result;
    
    if (fd < 0) return -1;
    
    va_start(params, format);
    result = ft_printf_generic(&fd, output_to_fd, format, params);
    va_end(params);
    
    return result;
}