#ifndef STRINGPRIVATE_H
# define STRINGPRIVATE_H

# include "String.h"

typedef struct {
    char    *data;
    size_t  length;
    size_t  capacity;
}   String_impl_;

# define MIN_CAPACITY 50

#endif
