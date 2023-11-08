#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define DUMP_NAME "list.dmp.html"

#define EXPAND 'e'
#define PRESS  'p'

static FILE *dmp_file = nullptr;

static const size_t kStartListSize = 8;

static const int kPoisonVal = 0xBADBABA;

static const int kMultiplier = 2;

static ListErrors_t ResizeList(List *list, const char mode);

static void MemSetList(List *list, ListElemType_t value);

//================================================================================================

ListErrors_t ListVerify(List *list)
{
    if (list->head < 0)
    {
        list->status |= kHeadLessZero;
    }

    if (list->tail < 0)
    {
        list->status |= kTailLessZero;
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

    return list->status;
}

//================================================================================================

ListErrors_t ListConstructor(List *list)
{
    list->capacity = kStartListSize;

    list->data = (ListElemType_t *) calloc(list->capacity + 1, sizeof(ListElemType_t));
    list->prev = (int *) calloc(list->capacity + 1, sizeof(int));
    list->next = (int *) calloc(list->capacity + 1, sizeof(int));

    list->status = kListClear;

    list->tail = list->head = 0;
    list->free = 1;

    list->data[0] = kPoisonVal;
    list->prev[0] = list->tail;
    list->next[0] = list->head;

    MemSetList(list, kPoisonVal);
    TextDumpList(list);

    return kListClear;
}

//================================================================================================

ListErrors_t ListDestructor(List *list)
{
    list->data = (ListElemType_t *) ((char *)list->data - sizeof(ListElemType_t));
    list->prev = (int *) ((char *)list->prev - sizeof(int));
    list->next = (int *) ((char *)list->next - sizeof(int));

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
    ListVerify(list);

    if (list->prev[pos] = -1)
    {
        return kWrongUsingOfList;
    }

    if (list->status != kListClear)
    {
        return list->status;
    }

    if (list->tail > list->capacity)
    {
        ResizeList(list, EXPAND);
    }

    size_t data_pos = list->free;
    list->free = list->next[data_pos];

    list->data[data_pos] = value;
    list->prev[data_pos] = pos;
    list->next[data_pos] = list->next[pos];

    list->prev[list->next[pos]] = data_pos;
    list->next[pos] = data_pos;

    TextDumpList(list);

    return kListClear;
}

//================================================================================================

static ListErrors_t ResizeList(List *list, const char mode)
{
    if (mode == PRESS)
    {
        list->capacity /= kMultiplier;
    }
    else if (mode == EXPAND)
    {
        list->capacity *= kMultiplier;
    }

    list->data = (ListElemType_t *) realloc(list->data, list->capacity * sizeof(ListElemType_t));
    list->prev = (int *) realloc(list->prev, list->capacity * sizeof(int));
    list->next = (int *) realloc(list->next, list->capacity * sizeof(int));

    return kListClear;
}

//================================================================================================

ListErrors_t ListDelete(List *list,
                        size_t pos)
{
    ListVerify(list);

    if (list->prev[pos] = -1)
    {
        return kWrongUsingOfList;
    }

    if (list->status != kListClear)
    {
        return list->status;
    }

    list->prev[list->next[pos]] = list->prev[pos];
    list->next[list->prev[pos]] = list->next[pos];

    list->data[pos] = kPoisonVal;
    list->next[pos] = list->free;
    list->prev[pos] = -1;

    list->free = pos;

    TextDumpList(list);

    return kListClear;
}

//================================================================================================

ListErrors_t TextDumpList(List *list)
{
    fprintf(dmp_file, "head = %d\ntail = %d\nfree = %d\n",
                      list->head,
                      list->tail,
                      list->free);
    for (size_t i = 0; i < list->capacity; i ++)
    {
        fprintf(dmp_file, "\tdata[%d] = %d; prev[%d] = %d; next[%d] = %d\n\n",
                          i,
                          list->data[i],
                          i,
                          list->prev[i],
                          i,
                          list->next[i]);
    }
    fprintf(dmp_file, "---------------------------------------\n");

    return kListClear;
}

//================================================================================================

void DumpListOpen()
{
    dmp_file = fopen(DUMP_NAME, "w");

    if (dmp_file == nullptr)
    {
        perror("DumpListOpen() failed to open dump file");
    }
    fprintf(dmp_file, "<pre>\n");
}

//================================================================================================

void DumpListClose()
{
    if (dmp_file != nullptr)
    {
        fclose(dmp_file);
        dmp_file = nullptr;

    }
}

//================================================================================================

static void MemSetList(List *list, ListElemType_t value)
{
    for (size_t i = list->tail + 1; i < list->capacity; ++i)
    {
        list->data[i] = value;

        list->next[i] = i + 1;

        list->prev[i] = -1;
    }
}



