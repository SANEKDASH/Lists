#include <stdlib.h>

#include "classic_list.h"

//================================================================================================

ClassicListElem *ClassicListInit(ClassicListValType_t value)
{
    ClassicListElem *root = (ClassicListElem *) calloc(1, sizeof(ClassicListElem));

    if (root == nullptr)
    {
        return nullptr;
    }

    root->data = value;

    root->next = root->prev = nullptr;

    return root;
}

//================================================================================================

ClassicListElem *ClassicListInsertAfter(ClassicListElem *elem,
                                        ClassicListValType_t value)
{
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

    return after;
}

//================================================================================================

ClassicListElem *ClassicListInsertBefore(ClassicListElem *elem,
                                         ClassicListValType_t value)
{
    ClassicListElem *before = (ClassicListElem *) calloc(1 ,sizeof(ClassicListElem));

    if (before == nullptr)
    {
        return nullptr;
    }

    before->data = value;
    before->next = elem;
    before->prev = nullptr;

    if (elem->prev != nullptr)
    {
        before->prev = elem->prev;

        elem->prev->next = before;
    }

    elem->prev = before;

    return before;
}

//================================================================================================

ClassicListElem *ClassicListDelete(ClassicListElem *elem)
{
    if (elem->prev == nullptr)
    {
        elem->next->prev = nullptr;
    }
    //hueta kakayato
    if (elem->next == nullptr)
    {
        elem->prev->next = nullptr;
    }

    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;

    elem->next = elem->prev = nullptr;

    free(elem);
    elem = nullptr;

    //what return?
    return nullptr;
}

//================================================================================================
