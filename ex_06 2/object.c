/**
 * @file object.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief object object implementation.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include <string.h>
#include "object.h"


static char const* get_name(void const*);


Object* construct_object(Object* self, char const* name)
{
    strcpy(self->name, name);
    self->get_name = get_name;

    return self;
}


static char const* get_name(void const* self)
{
    return ((Object const*)self)->name;
}
