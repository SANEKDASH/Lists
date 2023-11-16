#ifndef LIST_HEADER
#define LIST_HEADER

typedef int ListElemType_t;

typedef unsigned int ListErrors_t;

struct List
{
    ListElemType_t *data;
    int *prev;
    int *next;

    int head;
    int tail;
    int free;

    size_t capacity;
    size_t elem_count;

    unsigned int status;
};

typedef enum
{
    kListClear          = 1 << 0,
    kHeadLessZero       = 1 << 1,
    kTailLessZero       = 1 << 2,
    kNullData           = 1 << 3,
    kCapacityLessZero   = 1 << 4,
    kTailLessHead       = 1 << 5,
    kSizeLessZero       = 1 << 6,
    kElemCountLessZero  = 1 << 7,
    kWrongPrev          = 1 << 8,
    kWrongTailNext      = 1 << 9,

    kWrongUsingOfList   = 1 << 10,
    kFailedAllocation   = 1 << 11,
    kFailedReallocation = 1 << 12,
    kFailedToOpenFile   = 1 << 13,
} ListErrs_t;

ListErrors_t ListVerify(List *list);

ListErrors_t ListConstructor(List *list);

ListErrors_t ListDestructor(List *list);

ListErrors_t ListAddAfter(List *list,
                          size_t pos,
                          ListElemType_t value);

ListErrors_t ListAddBefore(List *list,
                           size_t pos,
                           ListElemType_t value);

ListErrors_t ListDelete(List *list,
                        size_t pos,
                        ListElemType_t *ret_val);

ListErrors_t TextDumpList(List *list);

#endif
