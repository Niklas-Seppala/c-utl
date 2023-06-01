#include "stdio.h"
#include "stdlib.h"
#include "cutils/hashmap.h"
#include "cutils/hashfun.h"
#include "cutils/linkedlist.h"

int main()
{
    // const char *key = "jo";
    // printf("0x%lx\n", CTLHashDJB2_STRING(key, 0));
    // printf("0x%lx\n", CTLHashDJB2_STRING(key, 0));

    // CTLHashMap map = CTLHashMapCreate(1, 0.75f, CTLHashSDBM_STRING);
    // CTLHashMapDebugPrint(map);

    // const char *value = "matti";

    // CTLHashMapPut(map, key, 0, value);
    // CTLHashMapDebugPrint(map);

    // const void * data = CTLHashMapGet(map, key, 0);
    // printf("%s\n", (char*)data);

    // data = CTLHashMapRemove(map, key, 0);
    // CTLHashMapDebugPrint(map);

    // data = CTLHashMapGet(map, key, 0);
    // printf("%p\n", data);

    // CTLHashMapFree(map);

    const char *str1 = "Matti";
    const char *str2 = "Pekka";
    const char *str3 = "Teppo";

    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListAdd(list, str1);
    CTLLinkedListAdd(list, str2);
    CTLLinkedListAdd(list, str3);

    CTLLinkedListFree(&list);
    return 0;
}
