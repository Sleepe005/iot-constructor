#include <stdio.h>
#include "singleLinkList.h"

int main(){
    singleList<int> *list = nullptr;
    pushBack(list, 1);

    singleList<int> *addlist = nullptr;
    pushBack(addlist, 2);
    pushBack(addlist, 3);

    pushBackList(list, addlist);
    printList(list);
}