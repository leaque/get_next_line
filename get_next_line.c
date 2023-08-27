#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE  2147483646


#endif


//strjoin
//strdup
//strlcat
//son okuma \0 mu kontrol edecek fonksiyon (strlen evolved)
//stringchr
//ggwp

int ft_strlen(char const *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

size_t ft_strlcpy(char *dst, const char *src, size_t size)
{
    size_t i;

    i = 0;
    while (i < size - 1 && src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }

    if (size > 0)
        dst[i] = '\0';
    
    while (src[i] != '\0')
        i++;
    
    return (i);
}


char *ft_strchr(const char *s, int c)
{
    if (!s)
        return (0);
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }

    if (c == '\0')
        return (char *)s;

    return (0);
}

char *ft_strjoin(char const *s1, char const *s2)
{
    int i;
    int j;
    i = 0;
    j = 0;
    int len_s1 = 0;
    int len_s2 = 0;
    if (s1)
        len_s1 = ft_strlen(s1);
    if (s2) 
        len_s2 = ft_strlen(s2);
    int total_len = len_s1 + len_s2;

    char *res = malloc(total_len + 1);
    if (!res) 
        return NULL;

    if (!s1 && !s2) 
    {
        free(res);
        return NULL;
    }

    if (s1)
    {
        while (i < len_s1)
        {
            res[i] = s1[i];
            i++;
        }
    }
    if (s2)
    {
        while (j < len_s2)
        {
            res[i + j] = s2[j];
            j++;
        }
    }
    res[i + j] = '\0';

    return res;
}

char *ft_strdup(const char *s)
{
    char *res;
    size_t s_len;

    if (!s)
        return (0);
    s_len = ft_strlen(s);
    res = malloc((s_len + 1) * sizeof(char));
    if (!res)
    {
        errno = ENOMEM;
        return (0);
    }
    ft_strlcpy(res, s, s_len + 1);
    return (res);
}

void *ft_memmove(void *dest, const void *src, size_t n)
{
    unsigned char *d;
    const unsigned char *s;

    d = dest;
    s = src;

    if (d == s || n == 0)
        return dest;
    
    if (d > s && d < s + n)
    {
        d = d + n;
        s = s + n;
        while (n)
        {
            d--;
            s--;
            *d = *s;
            n--;
        }
    }
    else
    {
        while (n)
        {
            *d = *s;
            d++;
            s++;
            n--;
        }
    }

    return (dest);
}

int ft_next(char *string)
{
    int i;

    i = 0;
    while (string[i] && (string[i] != '\n' || string[i] != '\0'))
        i++;
    return i;
}

char *ft_line(char *string)
{
    int i;

    i = 0;
    while (string[i] != '\n')
        i++;
    i = i + 1;
    ft_memmove(string, &string[i], ft_strlen(string) - i + 1);
    string[ft_strlen(string)] = '\0';
    return (string);
}

char *get_next_line(int fd)
{
    int lines_read;
    static char *big_stash;
    char *line;
    char *temp_stash;
    char *small_stash;
    int len;

    if (fd < 0 || BUFFER_SIZE < 1 || BUFFER_SIZE > 2147483647)
        return NULL;
    small_stash = malloc(sizeof(char) * (BUFFER_SIZE + 1));
    while ((lines_read = read(fd, small_stash, BUFFER_SIZE)) > 0)
    {
        line = NULL;
        small_stash[lines_read] = '\0';
        temp_stash = ft_strjoin(big_stash, small_stash);

        if (big_stash)
            free(big_stash);
        big_stash = temp_stash;
        //so far no seg faults
        if (ft_strchr(big_stash, 10) != NULL)
        {
            len = ft_next(big_stash);
            line = malloc(len + 1);
            ft_strlcpy(line, big_stash, len + 1);
            big_stash = ft_line(big_stash);
            free(small_stash);
            small_stash = NULL;
            return (line);
        }
    }
    if (lines_read == 0 && big_stash && *big_stash)
    {
        line = ft_strdup(big_stash);
        free(big_stash);
        big_stash = NULL;
        free(small_stash);
        small_stash = NULL;

        return (line);
    }
    if (lines_read == -1)
    {
            free(small_stash);
            small_stash = NULL;
            free(big_stash);
            big_stash = NULL;
            return NULL;
    }
    free(small_stash);
            small_stash = NULL;
            free(big_stash);
            big_stash = NULL;
            return NULL;
}

int main()
{
    int fd;
    char *line;
    int ret;

    fd = open("sample.txt", O_RDONLY);
    if (fd == -1)
        return 1;


    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return 0;
}