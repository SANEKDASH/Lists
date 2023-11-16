#include <stdio.h>
#include <stdlib.h>

#include "../FastList/list.h"
#include "../ClassicList/classic_list.h"
#include "list_dump.h"

static size_t call_count = 0;

static FILE *log_file = nullptr;

static const char * const log_file_name = "list.dmp.html";

//================================================================================================

/*ListErrors_t TextDumpList(List *list)
{
    CHECK(list);

    if (dmp_file == nullptr)
    {
        return kListClear;
    }

    fprintf(dmp_file, "head = %d\ntail = %d\nfree = %d\n",
                      list->head,
                      list->size,
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
}*/

//================================================================================================

void BeginListGraphDump()
{
    system("rm -f *.png");

    log_file = fopen(log_file_name, "w");

    if (log_file == nullptr)
    {
        perror("BeginListGraphDump() failed to open file");

        return;
    }

    fprintf(log_file, "<pre>\n");
}

//================================================================================================

void EndListGraphDump()
{
    if (log_file == nullptr)
    {
        perror("EndLostGrapDump() failed to write an info");

        return;
    }

    fclose(log_file);
}

//================================================================================================

ListErrs_t GraphDumpList(List *list,
                         const char *file,
                         const char *func,
                         const int line)
{
    FILE *dot_file = fopen("list.dmp.dot", "w");

    if (dot_file == nullptr)
    {
        perror("GraphDumpList() failed to open dump file");

        return kFailedToOpenFile;
    }

    static char cmd_command[200] = {0};

    #define LOG_PRINT(...) fprintf(dot_file, __VA_ARGS__)

    LOG_PRINT("digraph List\n{\n"
              "\trankdir = TB;\n"
              "\tgraph [bgcolor = \"black\", splines = \"ortho\"]\n"
              "\tnode[color =\"black\", fontsize=14];\n"
              "\tedge[color = \"red\", fontcolor = \"blue\",fontsize = 12];\n\n\n");

    LOG_PRINT("{rank = max;\n"
              "TAIL [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"TAIL : %d\" ]\n"
              "ELEM_COUNT [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"ELEM COUNT : %d\" ]\n"
              "FREE [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"FREE : %d\" ]\n"
              "HEAD [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"HEAD : %d\" ]}\n"
              "ELEM_COUNT->TAIL->FREE->HEAD [weight = 100, color = \"invis\"]",
              list->tail,
              list->elem_count,
              list->free,
              list->head);

    LOG_PRINT("{rank = max; node0 [style = filled, fillcolor = \"lightblue\", shape=Mrecord, label = "
              "\"<name> NODE_0 | {data : %d | <next> next : %d | <prev> prev : %d}\"]}",
              list->data[0],
              list->next[0],
              list->prev[0]);

    LOG_PRINT("{rank = same;\n");

    for (size_t i = 0; i < list->capacity; i++)
    {
        LOG_PRINT("node%d [style = filled, fillcolor = \"%s\", shape=Mrecord, label = "
                  "\"<name> NODE_%d | {data : %d | <next> next : %d | <prev> prev : %d}\"]\n",
                  i,
                  (list->prev[i] == -1) ? "pink" : "lightblue",
                  i,
                  list->data[i],
                  list->next[i],
                  list->prev[i]);
    }

    LOG_PRINT( "}\n");

    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        LOG_PRINT("node%d->node%d [weight = 100, color = \"invis\"]\n",
                  i,
                  i + 1);
    }

    for (size_t i = 0; i < list->capacity; i++)
    {
        LOG_PRINT("node%d->node%d [weight = 0, color = \"%s\"]\n",
                  i,
                  list->next[i],
                  (list->prev[i] == -1) ? "orange" : "red");
    }

    #ifdef PRINT_PREV

    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->prev[i] != -1)
        {
            LOG_PRINT("node%d->node%d [weight = 0, color = \"yellow\"]",
                      i,
                      list->prev[i]);
        }
    }

    #endif

    LOG_PRINT("TAIL->node%d [weight = 0, color = \"green\"]\n"
              "FREE->node%d [weight = 0, color = \"green\"]\n"
              "HEAD->node%d [weight = 0, color = \"green\"]\n",
              list->tail,
              list->free,
              list->head);

    LOG_PRINT("\n\n}");

    fclose(dot_file);

    sprintf(cmd_command, "dot -Tpng list.dmp.dot -o graphdump%d.png", call_count);

    system(cmd_command);

    fprintf(log_file, "Called from file: %s\n"
                      "Called from function: %s\n"
                      "Line: %d\n"
                      "<img height=\"150px\" src=\"graphdump%d.png\">\n"
                      "-----------------------------------------------------------------\n",
                      file,
                      func,
                      line,
                      call_count);


    ++call_count;
}

//================================================================================================

/*void DumpClassicList(ClassicList *list)
{
    LOG_PRINT("digraph List\n{\n"
              "\trankdir=LR;\n"
              "\tgraph [bgcolor=\"white\"]\n"
              "\tnode[color=\"black\",fontsize=14];\n"
              "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n\n\n");

    ClassicListElem *curr_node = list->root;

    for (size_t i = 0; i < list->elem_count; i++)
    {
        LOG_PRINT("node%d [style=filled,fillcolor=\"lightblue\"shape=Mrecord,label="
                  "\"<name> NODE-%d | {data : %d |",
                  i,
                  i,
                  curr_node->data);
        "<prev> prev : %d}\"]\n"

        if (curr_node->next == nullptr)
        {
            LOG_PRINT("<next> next : null |");
        }
        else
        {
            Log_PRINT("<next> next : %d |", i + 1);
        }

        if (curr_node->prev != nullptr)
        {
            LOG_PRINT("<prev> prev : %d}\"]\n", i - 1);
        }
        else
        {
            LOG_PRINT("<prev> prev : null}\"]\n");
        }
    }

    for (size_t i = 0; i < list->capacity; i++)
    {
        LOG_PRINT( "node%d->node%d [weight = 100, color = \"invis\"]\n", i, i + 1);
    }
    for (size_t i = 0; i < list->capacity; i++)
    {
        LOG_PRINT( "node%d->node%d \n", i, list->next[i]);
    }

    LOG_PRINT( "\n\n}");
}
*/
//================================================================================================

#undef LOG_PRINT
