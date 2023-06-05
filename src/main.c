#include "stdio.h"
#include "stdlib.h"
#include "cutils/hashmap.h"
#include "cutils/hashfun.h"
#include "cutils/linkedlist.h"
#include "cutils/error.h"
#include "cutils/compare.h"

typedef struct house {
    int rooms;
    char *name;
} House;


bool compareHousesByRooms(const void* a) {
    if (a == NULL) return false;
    const House *aHouse = (House*)a;
    return CTLCompareString(aHouse->name, "House 2") == EQ;
}

const char *NIL = "NULL";

const char *houseToString(const void *house) {
    if (house == NULL) return NIL;
    const House *aHouse = (House*)house;
    return aHouse->name;
}

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
    const char *str4 = "Jenni";

    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListAdd(list, str1);
    CTLLinkedListAdd(list, str2);
    CTLLinkedListAdd(list, str3);

    printf("list contains %s : %s\n", str3,
        CTLLinkedListContains(list, str3, CTLCompareString) ? "true" : "false");
    printf("list contains %s : %s\n", str2, 
        CTLLinkedListContains(list, str2, CTLCompareString) ? "true" : "false");
    printf("list contains %s : %s\n", str4,
        CTLLinkedListContains(list, str4, CTLCompareString) ? "true" : "false");
    printf("list contains %s : %s\n", str1,
        CTLLinkedListContains(list, str1, CTLCompareString) ? "true" : "false");


    printf("List size %d \n", CTLLinkedListSize(list));
    const char *b = "Jeesus";
    CTLLinkedListAddFirst(list, b);

    printf("List size %d \n", CTLLinkedListSize(list));
    printf("List head: %s\n", (char *)CTLLinkedListGetFirst(list));

    CTLLinkedListDebugPrint(list);
    CTLLinkedListDebugPrint(list);
    CTLLinkedListAddFirst(list, NULL);
    CTLLinkedListAddFirst(list, NULL);
    CTLLinkedListDebugPrint(list);

    printf("\n\n");

    // Copy and filter
    CTLLinkedList copy = CTLLinkedListCopy(list);
    CTLLinkedListFilter(copy, CTLPredicateNotNull);
    CTLLinkedListDebugPrint(copy);

    // reverse
    CTLLinkedListDebugPrint(list);
    CTLLinkedListReverse(list);
    CTLLinkedListDebugPrint(list);

    const void *ptr = CTLLinkedListRemoveAt(list, 3);
    printf("removed %p\n", ptr);
    CTLLinkedListDebugPrint(list);

    int i = CTLLinkedListIndexOf(list, NULL, CTLCompareString);
    printf("index of (nil): %d\n", i);

    
    const char *name = "Teppo";
    printf("Teppo ptr:  %p\n", str3);
    i = CTLLinkedListIndexOf(list, name, CTLCompareString);
    printf("index of Teppo: %d\n", i);
    

    CTLLinkedListClear(list);
    CTLLinkedListFree(&list);
    CTLLinkedListFree(&copy);


    House *house5 = calloc(sizeof(House), 1);
    house5->name = "House 5";
    house5->rooms = 5;

    House *house2 = calloc(sizeof(House), 1);
    house2->name = "House 2";
    house2->rooms = 2;

    House *house3 = calloc(sizeof(House), 1);
    house3->name = "House 3";
    house3->rooms = 3;

    CTLLinkedList l = CTLLinkedListCreate();
    CTLLinkedListAdd(l, house2);
    CTLLinkedListAdd(l, house3);
    CTLLinkedListAdd(l, NULL);
    CTLLinkedListAdd(l, NULL);
    CTLLinkedListAdd(l, house5);


    CTLLinkedListDebugPrintf(l, houseToString);
    CTLLinkedListFilter(l, compareHousesByRooms);
    CTLLinkedListDebugPrintf(l, houseToString);

    return 0;
}
