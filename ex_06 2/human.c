/**
 * @file human.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief human object implementation.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include "human.h"


static char const* get_name(void const*);
static Gender get_gender(void const*);
static Job get_job(void const*);


Human* construct_human(Human* self, char const* name, Gender g, Job j)
{
    construct_object(&self->super, name);
    self->gender     = g;
    self->job        = j;
    self->get_name   = get_name;
    self->get_gender = get_gender;
    self->get_job    = get_job;

    return self;
}


static char const* get_name(void const* self)
{
    return ((Human const*)self)->super.get_name(self);
}


static Gender get_gender(void const* self)
{
    return ((Human const*)self)->gender;
}


static Job get_job(void const* self)
{
    return ((Human const*)self)->job;
}
