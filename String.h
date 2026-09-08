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

int     string_construct(String *this, const char *str);
void    string_destruct(String *this);
int     string_copy_construct(String *this, const String *other);
String  *string_assign(String *lhs, const String *rhs);

char    *string_begin(String *this);
char    *string_end(String *this);
// char    *string_rbegin(const String *this);
// char    *string_rend(const String *this);

// size_t  string_size(const String *this);
size_t  string_length(const String *this);
// size_t  string_max_size(const String *this);
// int     string_resize(String *this, size_t n, char pad);
size_t  string_capacity(const String *this);
int     string_reserve(String *this, size_t res_arg);
void    string_clear(String *this);
bool    string_empty(const String *this);

char    string_at(const String *this, size_t pos);

String  *string_append(String *this, const char *str);
// int     string_push_back(String *this);
String  *string_insert(String *this, size_t pos, const char *str);
String  *string_erase(String *this, size_t pos, size_t n);
String  *string_replace(String *this, size_t pos, size_t n, const char *str);

const char    *string_c_str(const String *this);
// char    *string_data(const String *this);
/* dst に最大 n 文字コピーする。終端 NUL は付けない (std::string::copy と同じ)。
   戻り値は実際にコピーした文字数、エラー時は STRING_NPOS */
size_t  string_copy(const String *this, char *dst, size_t n, size_t pos);
size_t  string_find(const String *this, const char *str);
// size_t  string_rfind(const String *this, char *str);
// size_t  string_find_first_of(const String *this, char c);
// size_t  string_find_last_of(const String *this, char c);
// size_t  string_find_first_not_of(const String *this, char c);
// size_t  string_find_last_not_of(const String *this, char c);
/* dst は未構築の String を渡すこと (構築済みだと元の data がリークする)。
   成功したら呼び出し側が string_destruct で解放する */
int     string_substr(String *dst, const String *this, size_t pos, size_t n);
int     string_compare(const String *this, const String *rhs);

int     string_join(String *dst, const String *lhs, const String *rhs);
bool    string_equal(const String *lhs, const String *rhs);
bool    string_not_equal(const String *lhs, const String *rhs);
bool    string_less_than(const String *lhs, const String *rhs);
bool    string_less_equal(const String *lhs, const String *rhs);
bool    string_greater_than(const String *lhs, const String *rhs);
bool    string_greater_equal(const String *lhs, const String *rhs);

int     string_print(int fd, String *this);
// int     string_read(int fd, String *this);
// int     string_getline(String *dst, int fd);

int     string_swap(String *lhs, String *rhs);

# endif
