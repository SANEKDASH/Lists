#include <stdio.h>

#include "list.h"
#include "../ListDump/list_dump.h"
#include "../debug/debug.h"

#define DEBUG

int main()
{
    BeginListGraphDump();

    List list = {};

    ListConstructor(&list);

    for (size_t i = 0; i < 5; i++)
    {
        ListAddAfter(&list, i, i);
    }

    int a = 0;
    ListDelete(&list, 3, &a);
    ListAddAfter(&list, 5, 228);
    ListAddAfter(&list, 5, 1337);
    ListAddAfter(&list, 5, 1488);
    printf("AAA\n");

    ListDelete(&list, 3, &a);
    ListDelete(&list, 4, &a);
    ListDelete(&list, 5, &a);
    ListDelete(&list, 1, &a);
    ListDelete(&list, 2, &a);

    ListDestructor(&list);

    EndListGraphDump();

    return 0;
}
