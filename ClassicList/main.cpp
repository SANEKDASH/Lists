#include <stdio.h>

#include "classic_list.h"
#include "../ListDump/list_dump.h"

int main()
{
    ClassicListElem *root;

    root = ClassicListInit(10);

    ClassicListInsert(root, 15);
    //ClassicListInsert(root, 20);

    printf("%d", root->data);

    return 0;
}
