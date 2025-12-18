#include "libft/libft.h"

static size_t count_words(const char *s, char c) {
  size_t i;
  size_t count;

  i = 1;
  count = 0;
  while (s[i - 1]) {
    if ((s[i] == c || s[i] == 0) && s[i - 1] != c) {
      count++;
    }
    i++;
  }
  return (count);
}

char **ft_split(const char *s, char c) {
  char **strings;
  size_t len;
  size_t i;
  size_t j;

  strings = malloc((count_words(s, c) + 1) * sizeof(char *));
  len = 0;
  j = 0;
  i = 0;
  while (j < ft_strlen(s) + 1) {
    if (s[j] != c && s[j] != 0) {
      len++;
    }
    if (len && (s[j] == c || s[j] == 0)) {
      strings[i] = ft_strndup(&s[j] - len, len);
      len = 0;
      i++;
    }
    j++;
  }
  strings[i] = 0;
  return (strings);
}

char **ft_split_whitespace(const char *s) { return (ft_split(s, ' ')); }
