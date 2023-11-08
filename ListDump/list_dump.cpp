#include <stdio.h>

#include "../FastList/list.h"
#include "../ClassicList/classic_list.h"
#include "list_dump.h"

static FILE *log_file = nullptr;

//================================================================================================

void BeginListDump()
{
    log_file = fopen("list.dmp.dot", "w");
}

//================================================================================================

void EndListDump()
{
    fclose(log_file);
}

//================================================================================================

void DumpList(List *list)
{
    fprintf(log_file, "digraph List\n{\n"
                      "\trankdir=LR;\n"
                      "\tgraph [bgcolor=\"darkgrey\", splines=\"ortho\"]\n"
                      "\tnode[color=\"black\",fontsize=14];\n"
                      "\tedge[color=\"red\",fontcolor=\"blue\",fontsize=12];\n\n\n");


    for (size_t i = 0; i < list->capacity; i++)
    {
        printf("capacity - %d", list->capacity);
        fprintf(log_file, "node%d [style=filled,fillcolor=\"lightblue\"shape=Mrecord,label=\"<name> NODE_%d | {data : %d | <next> next : %d | <prev> prev : %d}\"]\n",
                          i,
                          i,
                          list->data[i],
                          list->next[i],
                          list->prev[i]);
    }

    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        fprintf(log_file, "node%d->node%d [weight = 100, color = \"invis\"]\n", i, i + 1);
    }
    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        fprintf(log_file, "node%d->node%d \n", i, list->next[i]);
    }
    for (size_t i = 0 ; i < list->capacity -1; i++)
    {
        if (list->prev[i] != -1)
        {
            fprintf(log_file, "node%d->node%d [color = \"yellow\"]\n", i, list->prev[i]);
        }
    }

    fprintf(log_file, "\n\n}");
}

//================================================================================================

/*void DumpClassicList(ClassicListElem *root)
{
       fprintf(log_file, "digraph List\n{\n"
                          "\trankdir=LR;\n"
                          "\tgraph [bgcolor=\"white\"]\n"
                          "\tnode[color=\"black\",fontsize=14];\n"
                          "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n\n\n");

    for (size_t i = 0; root->next != nullptr; i++)
    {
        fprintf(log_file, "node%d [style=filled,fillcolor=\"lightblue\"shape=Mrecord,label=\"<name> NODE-%d | {data : %d | <next> next : %d | <prev> prev : %d}\"]\n",
                          i,
                          i,
                          ,
                          list->next[i],
                          list->prev[i]);
    }

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(log_file, "node%d->node%d [weight = 100, color = \"invis\"]\n", i, i + 1);
    }
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(log_file, "node%d->node%d \n", i, list->next[i]);
    }

    fprintf(log_file, "\n\n}");
}*/

//================================================================================================
