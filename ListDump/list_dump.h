#ifndef LIST_DUMP_HEADER
#define LIST_DUMP_HEADER

#include "../FastList/list.h"
#include "../ClassicList/classic_list.h"

#ifdef DEBUG

#define GRAPH_DUMP(list) GraphDumpList(list, __FILE__, __func__, __LINE__)

#else

#define GRAPH_DUMP(list) ;

#endif

void BeginListGraphDump();

void EndListGraphDump();

ListErrs_t GraphDumpList(List *list,
                         const char *file,
                         const char *func,
                         const int line);

void DumpClassicList(ClassicList *list);

//ListErrors_t TextDumpList(List *list);

void BeginListTextDump();

void EndListTextDump();

#endif
