#define _POSIX_C_SOURCE 200809L
#include "StringPrivate.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

_Static_assert(sizeof(String) == sizeof(String_impl_),
               "String buffer is not equal to String_impl_");

static size_t   grow_capacity_(size_t length)
{
    size_t  want = length * 2;
    return (MIN_CAPACITY >= want ? MIN_CAPACITY : want);
}

int     string_construct(String *s, const char *str)
{
    if (!s) return (-1);
    if (!str) str = "";
    String_impl_    *self = (String_impl_*)s;
    self->length = strlen(str);
    self->capacity = grow_capacity_(self->length);
    self->data = calloc(self->capacity, sizeof(char));
    if (!self->data) return (-1);
    strcpy(self->data, str);
    return (0);
}

void    string_destruct(String *s)
{
    if (!s) return ;
    String_impl_    *self = (String_impl_*)s;
    free(self->data);
    self->data = NULL;
    self->length = 0;
    self->capacity = 0;
}

int string_copy_construct(String *s, const String *oth)
{
    if (!s || !oth) return (-1);
    String_impl_    *self = (String_impl_*)s;
    const String_impl_    *other = (const String_impl_*)oth;
    self->length = other->length;
    self->capacity = other->capacity;
    self->data = calloc(self->capacity, sizeof(char));
    if (!self->data) return (-1);
    strcpy(self->data, other->data);
    return (0);
}

String  *string_assign(String *l, const String *r)
{
    if (!l || !r) return (NULL);
    if (l != r) {
        String_impl_        *self  = (String_impl_ *)l;
        const String_impl_  *other = (const String_impl_ *)r;
        if (self->capacity < other->length + 1)
        {
            size_t  new_cap = grow_capacity_(other->length);
            char    *tmp = calloc(new_cap, sizeof(char));
            if (!tmp) return (NULL);
            free(self->data);
            self->data = tmp;
            self->capacity = new_cap;
        }
        memcpy(self->data, other->data, other->length + 1);
        self->length = other->length;
    }
    return (l);
}

char    *string_begin(String *s)
{
    if (!s) return (NULL);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->data);
}

char    *string_end(String *s)
{
    if (!s) return (NULL);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->data + self->length);
}


size_t  string_length(const String *s)
{
    if (!s) return (-1);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->length);
}

size_t  string_capacity(const String *s)
{
    if (!s) return (-1);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->capacity);
}

int     string_reserve(String *s, size_t res_arg)
{
    if (!s) return (-1);
    String_impl_    *self = (String_impl_*)s;
    if (self->capacity < res_arg) {
        char    *tmp = realloc(self->data, res_arg);
        if (!tmp) return (-1);
        self->data = tmp;
        self->capacity = res_arg;
    }
    return (0);
}

void    string_clear(String *s)
{
    if (!s) return ;
    String_impl_    *self = (String_impl_*)s;
    memset(self->data, 0, self->capacity);
    self->length = 0;
    return ;
}

bool    string_empty(const String *s)
{
    if (!s) return (-1);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->length == 0);
}


char    string_at(const String *s, size_t pos)
{
    if (!s) return (0);
    const String_impl_    *self = (const String_impl_*)s;
    if (self->length <= pos)
        return (0);
    return (self->data[pos]);
}


String  *string_append(String *s, const char *str)
{
    if (!s || !str) return (NULL);
    String_impl_    *self = (String_impl_*)s;
    size_t  expected_cap = self->length + strlen(str) + 1;
    if (self->capacity < expected_cap)
    {
        size_t  new_cap = grow_capacity_(expected_cap);
        char    *tmp = realloc(self->data, new_cap);
        if (!tmp) return (NULL);
        self->data = tmp;
        self->capacity = new_cap;
    }
    strcpy(self->data + self->length, str);
    self->length += strlen(str);
    return (s);
}

String  *string_insert(String *s, size_t pos, const char *str)
{
    if (!s || !str) return (NULL);
    String_impl_    *self = (String_impl_*)s;
    if (pos > self->length) return (NULL);

    size_t  add = strlen(str);
    size_t  need = self->length + add + 1;
    if (self->capacity < need)
    {
        size_t  new_cap = grow_capacity_(self->length + add);
        char    *tmp = realloc(self->data, new_cap);
        if (!tmp) return (NULL);
        self->data = tmp;
        self->capacity = new_cap;
    }
    memmove(self->data + pos + add, self->data + pos, self->length - pos + 1);
    memcpy(self->data + pos, str, add);
    self->length += add;
    return (s);
}

String  *string_erase(String *s, size_t pos, size_t n)
{
    if (!s) return (NULL);
    String_impl_    *self = (String_impl_*)s;
    if (pos > self->length) return (NULL);

    size_t  rest = self->length - pos;
    if (n > rest) n = rest;
    memmove(self->data + pos, self->data + pos + n, rest - n + 1);
    self->length -= n;
    return (s);
}

String  *string_replace(String *s, size_t pos, size_t n, const char *str)
{
    if (!s || !str) return (NULL);
    String_impl_    *self = (String_impl_*)s;
    if (pos > self->length) return (NULL);

    size_t  rest = self->length - pos;
    if (n > rest) n = rest;

    size_t  add = strlen(str);
    size_t  new_len = self->length - n + add;
    if (self->capacity < new_len + 1)
    {
        size_t  new_cap = grow_capacity_(new_len);
        char    *tmp = realloc(self->data, new_cap);
        if (!tmp) return (NULL);
        self->data = tmp;
        self->capacity = new_cap;
    }
    memmove(self->data + pos + add, self->data + pos + n, rest - n + 1);
    memcpy(self->data + pos, str, add);
    self->length = new_len;
    return (s);
}

const char    *string_c_str(const String *s)
{
    if (!s) return (NULL);
    const String_impl_    *self = (const String_impl_*)s;
    return (self->data);
}

size_t  string_copy(const String *s, char *dst, size_t n, size_t pos)
{
    if (!s || !dst) return (STRING_NPOS);
    const String_impl_    *self = (const String_impl_*)s;
    if (pos > self->length) return (STRING_NPOS);
    size_t  rest = self->length - pos;
    if (n > rest) n = rest;
    memcpy(dst, self->data + pos, n);
    return (n);
}

size_t  string_find(const String *s, const char *str)
{
    if (!s || !str) return (STRING_NPOS);
    const String_impl_    *self = (const String_impl_*)s;
    const char  *hit = strstr(self->data, str);
    if (!hit) return (STRING_NPOS);
    return ((size_t)(hit - self->data));
}

int     string_substr(String *d, const String *s, size_t pos, size_t n)
{
    if (!d || !s) return (-1);
    const String_impl_    *self = (const String_impl_*)s;
    if (pos > self->length) return (-1);

    size_t  rest = self->length - pos;
    if (n > rest) n = rest;

    String_impl_    *dst = (String_impl_*)d;
    dst->length = n;
    dst->capacity = grow_capacity_(n);
    dst->data = calloc(dst->capacity, sizeof(char));
    if (!dst->data) return (-1);
    memcpy(dst->data, self->data + pos, n);
    return (0);
}

int     string_compare(const String *s, const String *r)
{
    if (!s || !r) return (-1);
    const String_impl_    *self = (const String_impl_*)s;
    const String_impl_    *rhs = (const String_impl_*)r;
    return (strcmp(self->data, rhs->data));
}


int     string_join(String *d, const String *l, const String *r)
{
    if (!d || !l || !r) return (-1);
    const String_impl_    *lhs = (const String_impl_*)l;
    const String_impl_    *rhs = (const String_impl_*)r;
    String_impl_          *dst = (String_impl_*)d;

    dst->length = lhs->length + rhs->length;
    dst->capacity = grow_capacity_(dst->length);
    dst->data = calloc(dst->capacity, sizeof(char));
    if (!dst->data) return (-1);
    memcpy(dst->data, lhs->data, lhs->length);
    memcpy(dst->data + lhs->length, rhs->data, rhs->length + 1);
    return (0);
}

bool    string_equal(const String *l, const String *r)
{
    if (l == r) return (true);
    if (!l || !r) return (false);
    const String_impl_    *lhs = (const String_impl_*)l;
    const String_impl_    *rhs = (const String_impl_*)r;
    return (strcmp(lhs->data, rhs->data) == 0);
}

bool    string_not_equal(const String *l, const String *r)
{
    return (!string_equal(l, r));
}

bool    string_less_than(const String *l, const String *r)
{
    if (!l || !r) return (false);
    const String_impl_    *lhs = (const String_impl_*)l;
    const String_impl_    *rhs = (const String_impl_*)r;
    return (strcmp(lhs->data, rhs->data) < 0);
}

bool    string_less_equal(const String *l, const String *r)
{
    return (string_less_than(l, r) || string_equal(l, r));
}

bool    string_greater_than(const String *l, const String *r)
{
    return (!string_less_equal(l, r));
}

bool    string_greater_equal(const String *l, const String *r)
{
    return (!string_less_than(l, r));
}

int     string_print(int fd, String *s)
{
    if (!s) return (-1);
    String_impl_    *self = (String_impl_*)s;
    return (dprintf(fd, "%s", self->data));
}

int     string_swap(String *l, String *r)
{
    if (!l || !r) return (-1);
    String_impl_    *lhs = (String_impl_*)l;
    String_impl_    *rhs = (String_impl_*)r;
    String_impl_    tmp = *lhs;

    *lhs = *rhs;
    *rhs = tmp;
    return (0);
}

