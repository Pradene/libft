#ifndef FT_SORT_H
#define FT_SORT_H

// Sorting functions
void ft_quicksort(void *base, size_t nb, size_t size, int (*comp)(const void *, const void *));
void ft_reverse(void *base, size_t count, size_t size);

#endif // FT_SORT_H