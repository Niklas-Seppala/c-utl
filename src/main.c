#include "ctl/compare.h"
#include "ctl/error.h"
#include "ctl/hashfun.h"
#include "ctl/hashmap.h"
#include "ctl/linkedlist.h"
#include "ctl.h"
#include "ctl/io/logger.h"
#include "assert.h"

typedef struct house {
    int rooms;
    char *name;
} House;

bool houseByName(const void *a) {
    if (a == NULL) return false;
    const House *aHouse = (House *)a;
    return CTLCompareString(aHouse->name, "House 2") == EQ;
}

const char *NIL = "NULL";

const char *houseToString(const void *house) {
    if (house == NULL) return NIL;
    const House *aHouse = (House *)house;
    return aHouse->name;
}


int main() {
   
    // CTLLogSetup();
    // CTLLogPrintf(CTL_TRACE, "%s", "this is trace");
    // CTLLogPrintf(CTL_TEST, "%s", "this is test");
    // CTLLogPrintf(CTL_DEBUG, "%s", "this is debug");
    // CTLLogPrintf(CTL_INFO, "%s", "this is info");
    // CTLLogPrintf(CTL_WARN, "%s", "this is warning");
    // CTLLogPrintf(CTL_ERROR, "%s", "this is error");
    // CTLLogPrintf(CTL_FATAL, "%s", "this is fatal error");

    // CTLLogTeardown();

    // const char *key1 = "key";
    // const char *key2 = "key22";
    // const char *value1 = "value1";
    // const char *value2 = "value2";

    // CTLHashMap map = CTLHashMapCreate(1, 0.75f, CTLHashSDBM_STRING, CTLCompareStringKey);

    // uint64_t h1 = CTLHashSDBM_STRING(key1, 0);
    // uint64_t h2 = CTLHashSDBM_STRING(key2, 0);
    // printf("h1 %ld\n", h1 % 16);
    // printf("h1 %ld\n", h2 % 16);

    // CTLHashMapPut(map, key1, 0, value1);
    // CTLHashMapDebugPrint(map);
    // CTLHashMapPutIfAbsent(map, key2, 0, value2);
    // CTLHashMapDebugPrint(map);

    // printf("value1:  %s\n", (const char *)CTLHashMapGet(map, key1, 0));
    // printf("value2:  %s\n", (const char *)CTLHashMapGet(map, key2, 0));

    // const char *rp = "replacedValue";
    // bool wasReplaced = CTLHashMapReplace(map, key1, 0, rp);
    // printf("was replaced: %s\n", wasReplaced ? "true" : "false");
    // printf("value1:  %s\n", (const char *)CTLHashMapGet(map, key1, 0));

    // CTLLinkedList keys = CTLHashMapAllocateKeyList(map);
    // CTLIterator keyIterator = CTLLinkedListIterator(keys);
    // while (CTLIteratorHasNext(keyIterator)) {
    //     const char *key = CTLIteratorNext(keyIterator);
    //     printf("Key: %s\n", key);
    // }

    // printf("reset iter\n");
    // CTLLinkedListIteratorReset(keys);
    // while (CTLIteratorHasNext(keyIterator)) {
    //     const char *key = CTLIteratorNext(keyIterator);
    //     ssize_t index = CTLIteratorGetIndex(keyIterator);
    //     CTLLinkedListRemoveAt(keys, index);
    //     CTLIteratorRemovedCurrent(keyIterator);
    //     printf("Key: %s\n", key);
    // }

    // printf("remove iter values\n");
    // CTLLinkedListIteratorReset(keys);
    // CTLLinkedListDebugPrint(keys);

    // CTLLinkedListFree(&keys);

    // CTLHashMapClear(map);
    // CTLHashMapDebugPrint(map);

    // CTLHashMapFree(map);


    // const char *str1 = "Matti";
    // const char *str2 = "Pekka";
    // const char *str3 = "Teppo";

    // CTLLinkedList list = CTLLinkedListCreate();
    // CTLLinkedListAdd(list, str1);
    // CTLLinkedListAdd(list, str2);
    // CTLLinkedListAdd(list, str3);

    // CTLIterator iter = CTLLinkedAllocateListIterator(list);
    // while (CTLIteratorHasNext(iter)) {
    //     const char *str = CTLIteratorNext(iter);
    //     printf("%ld : %s\n", CTLIteratorGetIndex(iter), (char *)str);
    // }
    // CTLIteratorFreeNodeIterator(&iter);

    // printf("first: %s\n", (char *)CTLLinkedListGetFirst(list));
    // CTLLinkedListFree(&list);


    // printf("list contains %s : %s\n", str3,
    //     CTLLinkedListContains(list, str3, CTLCompareString) ? "true" : "false");
    // printf("list contains %s : %s\n", str2,
    //     CTLLinkedListContains(list, str2, CTLCompareString) ? "true" : "false");
    // printf("list contains %s : %s\n", str4,
    //     CTLLinkedListContains(list, str4, CTLCompareString) ? "true" : "false");
    // printf("list contains %s : %s\n", str1,
    //     CTLLinkedListContains(list, str1, CTLCompareString) ? "true" : "false");

    // printf("List size %d \n", CTLLinkedListSize(list));
    // const char *b = "Jeesus";
    // CTLLinkedListAddFirst(list, b);

    // printf("List size %d \n", CTLLinkedListSize(list));
    // printf("List head: %s\n", (char *)CTLLinkedListGetFirst(list));

    // CTLLinkedListDebugPrint(list);
    // CTLLinkedListDebugPrint(list);
    // CTLLinkedListAddFirst(list, NULL);
    // CTLLinkedListAddFirst(list, NULL);
    // CTLLinkedListDebugPrint(list);

    // printf("\n\n");

    // // Copy and filter
    // CTLLinkedList copy = CTLLinkedListCopy(list);
    // CTLLinkedListFilter(copy, CTLPredicateNotNull);
    // CTLLinkedListDebugPrint(copy);

    // // reverse
    // CTLLinkedListDebugPrint(list);
    // CTLLinkedListReverse(list);
    // CTLLinkedListDebugPrint(list);

    // const void *ptr = CTLLinkedListRemoveAt(list, 3);
    // printf("removed %p\n", ptr);
    // CTLLinkedListDebugPrint(list);

    // int i = CTLLinkedListIndexOf(list, NULL, CTLCompareString);
    // printf("index of (nil): %d\n", i);

    // const char *name = "Teppo";
    // printf("Teppo ptr:  %p\n", str3);
    // i = CTLLinkedListIndexOf(list, name, CTLCompareString);
    // printf("index of Teppo: %d\n", i);

    // CTLLinkedListClear(list);
    // CTLLinkedListFree(&list);
    // CTLLinkedListFree(&copy);

    // House *house5 = calloc(sizeof(House), 1);
    // house5->name = "House 5";
    // house5->rooms = 5;

    // House *house2 = calloc(sizeof(House), 1);
    // house2->name = "House 2";
    // house2->rooms = 2;

    // House *house3 = calloc(sizeof(House), 1);
    // house3->name = "House 3";
    // house3->rooms = 3;

    // CTLLinkedList l = CTLLinkedListCreate();
    // CTLLinkedListAdd(l, house2);
    // CTLLinkedListAdd(l, house3);
    // CTLLinkedListAdd(l, NULL);
    // CTLLinkedListAdd(l, NULL);
    // CTLLinkedListAdd(l, house5);

    // CTLLinkedListDebugPrintf(l, houseToString);
    // CTLLinkedListFilter(l, compareHousesByRooms);
    // CTLLinkedListDebugPrintf(l, houseToString);

    return 0;
}
