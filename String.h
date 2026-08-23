#ifndef STRING_H
# define STRING_H

# include <stddef.h>
# include <stdbool.h>

typedef union String
{
    unsigned char   _[24];
    void            *_align_p;
    size_t          _align_s;
}   String;

# define STRING_NPOS ((size_t)-1)

int     string_construct(String *self, const char *str);
void    string_destruct(String *self);
int     string_copy_construct(String *self, const String *other);
String  *string_assign(String *lhs, const String *rhs);

char    *string_begin(String *self);
char    *string_end(String *self);
// char    *string_rbegin(const String *self);
// char    *string_rend(const String *self);

// size_t  string_size(const String *self);
size_t  string_length(const String *self);
// size_t  string_max_size(const String *self);
// int     string_resize(String *self, size_t n, char pad);
size_t  string_capacity(const String *self);
int     string_reserve(String *self, size_t res_arg);
void    string_clear(String *self);
bool    string_empty(const String *self);

char    string_at(const String *self, size_t pos);

String  *string_append(String *self, const char *str);
// int     string_push_back(String *self);
String  *string_insert(String *self, size_t pos, const char *str);
String  *string_erase(String *self, size_t pos, size_t n);
String  *string_replace(String *self, size_t pos, size_t n, const char *str);

const char    *string_c_str(const String *self);
// char    *string_data(const String *self);
/* dst に最大 n 文字コピーする。終端 NUL は付けない (std::string::copy と同じ)。
   戻り値は実際にコピーした文字数、エラー時は STRING_NPOS */
size_t  string_copy(const String *self, char *dst, size_t n, size_t pos);
size_t  string_find(const String *self, const char *str);
// size_t  string_rfind(const String *self, char *str);
// size_t  string_find_first_of(const String *self, char c);
// size_t  string_find_last_of(const String *self, char c);
// size_t  string_find_first_not_of(const String *self, char c);
// size_t  string_find_last_not_of(const String *self, char c);
/* dst は未構築の String を渡すこと (構築済みだと元の data がリークする)。
   成功したら呼び出し側が string_destruct で解放する */
int     string_substr(String *dst, const String *self, size_t pos, size_t n);
int     string_compare(const String *self, const String *rhs);

int     string_join(String *dst, const String *lhs, const String *rhs);
bool    string_equal(const String *lhs, const String *rhs);
bool    string_not_equal(const String *lhs, const String *rhs);
bool    string_less_than(const String *lhs, const String *rhs);
bool    string_less_equal(const String *lhs, const String *rhs);
bool    string_greater_than(const String *lhs, const String *rhs);
bool    string_greater_equal(const String *lhs, const String *rhs);

int     string_print(int fd, String *self);
// int     string_read(int fd, String *self);
// int     string_getline(String *dst, int fd);

int     string_swap(String *lhs, String *rhs);

# endif
