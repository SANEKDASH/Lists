#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#include "list.h"
#include "../ListDump/list_dump.h"
#include "../debug/debug.h"
#include "../debug/color_print.h"


#ifdef DEBUG

#define TEXT_DUMP_LIST(list) TextDumpList(list)
#define DEBUG_ON(...) __VA_ARGS__

#else

#define TEXT_DUMP_LIST(list)
#define DEBUG_ON(...)

#endif

static const size_t kStartListSize = 8;

static const int kPoisonVal = 0xBADBABA;

static const int kFreePoison = -1;

static const int kMultiplier = 2;



static ListErrors_t ResizeList(List *list, size_t new_size);

static void MemSetList(List *list, ListElemType_t value);

static ListErrors_t ListLinearization(List *list);

//================================================================================================

ListErrors_t ListVerify(List *list)
{
    CHECK(list);

    if (list->head < 0)
    {
        list->status |= kHeadLessZero;
    }

    if (list->data == nullptr)
    {
        list->status |= kNullData;
    }

    if (list->capacity < 0)
    {
        list->status |= kCapacityLessZero;
    }

    if (list->tail < list->head)
    {
        list->status |= kTailLessHead;
    }

    if (list->tail < 0)
    {
        list->status |= kTailLessZero;
    }

    if (list->elem_count < 0)
    {
        list->status |= kElemCountLessZero;
    }

    if (list->prev[0] != list->tail)
    {
        list->status |= kWrongPrev;
    }

    if (list->next[list->tail] != 0)
    {
        list->status |= kWrongTailNext;
    }

    return list->status;
}

//================================================================================================

ListErrors_t ListConstructor(List *list)
{
    CHECK(list);

    list->capacity = kStartListSize;

    list->data = (ListElemType_t *) calloc(list->capacity, sizeof(ListElemType_t));
    list->prev = (int *)            calloc(list->capacity, sizeof(int));
    list->next = (int *)            calloc(list->capacity, sizeof(int));

    if (list->data == nullptr || list->prev == nullptr || list->next == nullptr)
    {
        list->status = kFailedAllocation;

        // free???? (nullptr)?
        return list->status;
    }

    list->status = kListClear;

    list->tail       = 0;
    list->head       = 0;
    list->free       = 1;
    list->elem_count = 0;

    MemSetList(list, kPoisonVal);

    list->data[0] = kPoisonVal;
    list->next[0] = list->head;
    list->prev[0] = list->head;

    return kListClear;
}

//================================================================================================

ListErrors_t ListDestructor(List *list)
{
    CHECK(list);

    free(list->data);
    free(list->prev);
    free(list->next);

    list->data = list->prev = list->next = nullptr;

    return kListClear;
}

//================================================================================================

ListErrors_t ListAddAfter(List *list,
                          size_t pos,
                          ListElemType_t value)
{
    CHECK(list);

    GRAPH_DUMP(list);

    if (ListVerify(list) != kListClear)
    {
        return list->status;
    }

    if (list->prev[pos] == kFreePoison || pos < 0)
    {
        return kWrongUsingOfList;
    }

    if (list->next[list->free] == 0)
    {
        ResizeList(list, list->capacity * kMultiplier);
    }

    ++list->elem_count;

    size_t data_pos = list->free;
    list->free = list->next[data_pos];

    if (list->next[pos] == 0)
    {
        list->tail = data_pos;
    }

    list->data[data_pos] = value;
    list->prev[data_pos] = pos;
    list->next[data_pos] = list->next[pos];

    list->prev[list->next[pos]] = data_pos;
    list->next[pos]             = data_pos;

    GRAPH_DUMP(list);

    return kListClear;
}

//================================================================================================

ListErrors_t ListAddBefore(List *list,
                           size_t pos,
                           ListElemType_t value)
{
    CHECK(list);

    return ListAddAfter(list, pos -1, value);
}

//================================================================================================

static ListErrors_t ResizeList(List *list, size_t new_size)
{
    CHECK(list);

    ListLinearization(list);

    list->capacity = new_size;

    list->data = (ListElemType_t *) realloc(list->data, list->capacity * sizeof(ListElemType_t));
    list->prev = (int *)            realloc(list->prev, list->capacity * sizeof(int));
    list->next = (int *)            realloc(list->next, list->capacity * sizeof(int));

/*
    Cppreference : https://en.cppreference.com/w/c/memory/free

    The function accepts (and does nothing with) the null pointer
    to reduce the amount of special-casing. Whether allocation succeeds or not,
    the pointer returned by an allocation function can be passed to free().
*/

    if (list->data == nullptr || list->prev == nullptr || list->next == nullptr)
    {
        list->status |= kFailedReallocation;

        free(list->data);
        free(list->prev);
        free(list->next);

        return kFailedReallocation;
    }

    MemSetList(list, kPoisonVal);

    return kListClear;
}

//================================================================================================

ListErrors_t ListDelete(List *list,
                        size_t pos,
                        ListElemType_t *ret_val)
{
    CHECK(list);
    CHECK(ret_val);

    if (ListVerify(list) != kListClear)
    {
        return list->status;
    }

    if (list->prev[pos] == kFreePoison || pos <= 0)
    {
        return kWrongUsingOfList;
    }

    if ((list->elem_count < list->capacity / (kMultiplier * kMultiplier)) &&
        (list->capacity > kStartListSize))
    {
        ResizeList(list, list->capacity / kMultiplier);
    }

    *ret_val = list->data[pos];

    if (pos == list->tail)
    {
        list->tail = list->prev[pos];
    }

    list->prev[list->next[pos]] = list->prev[pos];
    list->next[list->prev[pos]] = list->next[pos];

    list->data[pos] = kPoisonVal;
    list->next[pos] = list->free;
    list->prev[pos] = -1;

    list->free = pos;

    --list->elem_count;

    GRAPH_DUMP(list);

    return kListClear;
}

//================================================================================================

static void MemSetList(List *list, ListElemType_t value)
{
    CHECK(list);

    size_t i = list->tail + 1;

    for ( ; i < list->capacity - 1 ; ++i)
    {
            list->data[i] = value;

            list->next[i] = i + 1;

            list->prev[i] = -1;
    }

    list->data[i] = value;
    list->next[i] = 0;
    list->prev[i] = -1;

    GRAPH_DUMP(list);
}

//================================================================================================

static ListErrors_t ListLinearization(List *list)
{

    ListElemType_t *line_data = (ListElemType_t *) calloc(list->capacity, sizeof(ListElemType_t));

    if (line_data == nullptr)
    {
        return kFailedAllocation;
    }

    size_t pos = list->next[0];
    size_t i = 1;

    while (i < list->elem_count)
    {
        line_data[i] = list->data[pos];

        pos = list->next[pos];

        list->next[i - 1] = i;

        list->prev[i] = i - 1;

        ++i;
    }

    line_data[i] = list->data[pos];

    free(list->data);

    list->next[i - 1] = i;

    list->prev[i] = i - 1;
    list->next[i] = 0;

    list->prev[0] = i;

    list->data = line_data;
    list->tail = i;
    list->free = i + 1;

    return kListClear;
}

//================================================================================================
