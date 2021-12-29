/**
 * @file object.h
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief object object header.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef OBJECT_H
#define OBJECT_H



enum {
    MAX_STR_LEN = 256,
};


typedef char const* (*FuncGetName)(void const*);


struct object {
    char name[MAX_STR_LEN];
    FuncGetName get_name;
};
typedef struct object Object;


extern Object* construct_object(Object*, char const*);



#endif
