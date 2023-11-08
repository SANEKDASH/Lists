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

    unsigned int status;
};

typedef enum
{
    kListClear        = 1 << 0,
    kHeadLessZero     = 1 << 1,
    kTailLessZero     = 1 << 2,
    kNullData         = 1 << 3,
    kCapacityLessZero = 1 << 4,
    kTailLessHead     = 1 << 5,

    kWrongUsingOfList = 1 << 6,
} ListErrs_t;

ListErrors_t ListVerify(List *list);

ListErrors_t ListConstructor(List *list);

ListErrors_t ListDestructor(List *list);

ListErrors_t ListAddAfter(List *list,
                          size_t pos,
                          ListElemType_t value);

ListErrors_t ListDelete(List *list,
                        size_t pos);

ListErrors_t TextDumpList(List *list);

void DumpListOpen();

void DumpListClose();

#endif
