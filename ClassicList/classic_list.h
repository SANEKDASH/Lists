typedef int ClassicListValType_t;

struct ClassicListElem
{
    ClassicListValType_t data;
    ClassicListElem *next;
    ClassicListElem *prev;
};

struct ClassicList
{
    ClassicListElem head;
    ClassicListElem tale;
};

typedef enum
{
    kClassicListSuccess,
} ClassicListErr_t;

ClassicListElem *ClassicListInit(ClassicListValType_t value);

ClassicListElem *ClassicListInsert(ClassicListElem *elem,
                                   ClassicListValType_t value);

ClassicListElem *ClassicListInsertBefore(ClassicListElem *elem,
                                         ClassicListValType_t value);

ClassicListElem *ClassicListDelete(ClassicListElem *elem);

