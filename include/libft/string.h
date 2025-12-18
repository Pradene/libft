#ifndef FT_STRING_H
#define FT_STRING_H

#include <stddef.h>
#include <stdlib.h>

// String functions
int ft_atoi(const char *str);
char *ft_itoa(int n);
char **ft_split(char const *s, char c);
char *ft_strcat(char *dest, const char *src);
char *ft_strcpy(char *dest, const char *src);
char *ft_strdup(const char *s);
size_t ft_strlen(const char *str);
size_t ft_strlcpy(char *dest, const char *src, size_t n);
size_t ft_strlcat(char *dest, const char *src, size_t n);
char *ft_strchr(const char *str, int c);
char *ft_strrchr(const char *str, int c);
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_strnstr(const char *s1, const char *s2, size_t n);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(char const *s1, char const *set);
void ft_strtolower(char *s);
char *ft_strndup(const char *str, int n);
char *ft_substr(const char *s, unsigned int start, size_t n);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
void ft_striteri(char *s, void (*f)(unsigned int, char *));
int ft_strrcmp(const char *a, const char *b);

#endif // FT_STRING_H
