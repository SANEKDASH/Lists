#ifndef CLASSIC_LIST_HEADER
#define CLASSIC_LIST_HEADER

typedef int ClassicListValType_t;

struct ClassicListElem
{
    ClassicListValType_t data;
    ClassicListElem *next;
    ClassicListElem *prev;
};

struct ClassicList
{
    ClassicListElem *root;
    size_t elem_count;
};

typedef enum
{
    kClassicListSuccess,
} ClassicListErr_t;

ClassicListElem *ClassicListInit(ClassicList *list,
                                 ClassicListValType_t value);

ClassicListElem *ClassicListInsertAfter(ClassicList *list,
                                        ClassicListElem *elem,
                                        ClassicListValType_t value);

ClassicListElem *ClassicListInsertBefore(ClassicList *list,
                                         ClassicListElem *elem,
                                         ClassicListValType_t value);

ClassicListElem *ClassicListDelete(ClassicList *list,
                                   ClassicListElem *elem,
                                   ClassicListValType_t *ret_value);

ClassicListErr_t ClassicListDtor(ClassicList *list);

#endif
