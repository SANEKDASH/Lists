#include <stdlib.h>
#include <stdio.h>

#include "../debug/debug.h"
#include "../debug/color_print.h"
#include "classic_list.h"

static ClassicListValType_t kPoisonVal = 0xBADBABA;

//================================================================================================

ClassicListElem *ClassicListInit(ClassicList *list,
                                 ClassicListValType_t value)
{
    CHECK(list);

    ClassicListElem *root = (ClassicListElem *) calloc(1, sizeof(ClassicListElem));

    if (root == nullptr)
    {
        return nullptr;
    }

    list->elem_count = 1;
    root->data = value;

    list->root = root;

    root->next = root->prev = nullptr;

    return root;
}

//================================================================================================

ClassicListElem *ClassicListInsertAfter(ClassicList *list,
                                        ClassicListElem *elem,
                                        ClassicListValType_t value)
{
    CHECK(elem);

    ClassicListElem *after = (ClassicListElem *) calloc(1, sizeof(ClassicListElem));

    if (after == nullptr)
    {
        return nullptr;
    }

    after->data = value;
    after->next = nullptr;
    after->prev = elem;


    if (elem->next != nullptr)
    {
        after->next = elem->next;

        elem->next->prev = after;
    }

    elem->next = after;

    ++list->elem_count;

    return after;
}

//================================================================================================

ClassicListElem *ClassicListInsertBefore(ClassicList *list,
                                         ClassicListElem *elem,
                                         ClassicListValType_t value)
{
    CHECK(elem);
    CHECK(list);

    return ClassicListInsertAfter(list, elem->prev, value);
}

//================================================================================================

ClassicListElem *ClassicListDelete(ClassicList *list,
                                   ClassicListElem *elem,
                                   ClassicListValType_t *ret_value)
{
    CHECK(elem);

    if (elem->prev == nullptr)
    {
        elem->next->prev = nullptr;
    }

    if (elem->next == nullptr)
    {
        elem->prev->next = nullptr;
    }

    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;

    elem->next = elem->prev = nullptr;

    free(elem);
    elem = nullptr;

    --list->elem_count;

    //what return?
    return nullptr;
}

//================================================================================================

ClassicListErr_t ClassicListDtor(ClassicList *list)
{
    ClassicListElem *curr_node = list->root;

    for (size_t i = 1; i < list->elem_count; ++i)
    {
        curr_node = curr_node->next;

        curr_node->prev->data = kPoisonVal;

        curr_node->prev->next = nullptr;

        free(curr_node->prev);

        curr_node->prev = nullptr;
    }

    return kClassicListSuccess;
}

//================================================================================================
