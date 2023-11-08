#include <stdio.h>

#include "list.h"
#include "../ListDump/list_dump.h"

int main()
{
    BeginListDump();

    List list = {};

    ListConstructor(&list);

    ListAddAfter(&list, 0, 10);
    ListAddAfter(&list, 1, 11);
    ListDelete(&list, 1);
    ListAddAfter(&list, 0, 10);


    //ListAddAfter(&list, 1, 15);
    //ListDelete(&list, 1);

    DumpList(&list);

    ListDestructor(&list);

    EndListDump();

    return 0;
}
