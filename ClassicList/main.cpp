#include <stdio.h>

#include "classic_list.h"
#include "../ListDump/list_dump.h"

int main()
{
    ClassicList list;

    ClassicListInit(&list, 10);

    ClassicListInsertAfter(&list, list.root, 20);

    ClassicListDtor(&list);


    return 0;
}
