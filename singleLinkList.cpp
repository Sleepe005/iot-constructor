#include <stdio.h>

template <typename T>
struct singleListItem {
    T data;
    singleListItem<T> *next;
};

template <typename T>
u_int16_t lengthList(singleListItem<T> *&head){
    if(head == NULL){return 0;}

    int len = 0;

    singleListItem<T> *bufIt = head;
    while (bufIt->next != NULL)
    {
        ++len;
        bufIt = bufIt->next;
    }
    
    return ++len;
}

template <typename T>
void printList(singleListItem<T> *&head){
    if(head == NULL){
        printf("List in NULL\n");
        return;
    }

    singleListItem<T> *bufIt = head;
    while(bufIt->next != NULL){
        printf(" %d ", bufIt->data);
        bufIt = bufIt->next;
    }
    printf(" %d\n", bufIt->data);
}

template <typename T>
void deleteList(singleListItem<T> *&head){
    if(head->next == NULL){
        head = NULL;
        delete head;
    }else{
        deleteList(head->next);
        deleteList(head);
    }
}

template <typename T>
void pushBack(singleListItem<T> *&head, T data){
    singleListItem<T> *newItem = new singleListItem<T>;
    newItem->next = NULL;
    newItem->data = data;

    if(head == NULL){
        head = newItem;
        return;
    }

    singleListItem<T> *bufIt = head;
    while(bufIt->next != NULL){
        bufIt = bufIt->next;
    }

    bufIt->next = newItem;
}

template <typename T>
void pushBackList(singleListItem<T> *&head, singleListItem<T> *&insertListItem){
    singleListItem<T> *bufIt = insertListItem;
    while (bufIt->next != NULL)
    {
        pushBack(head, bufIt->data);
        bufIt = bufIt->next;
    }
    pushBack(head, bufIt->data);
}

template <typename T>
void pushFront(singleListItem<T> *&head, T data){
    singleListItem<T> *bufHead = head;

    singleListItem<T> *newItem = new singleListItem<T>;
    newItem->data = data;
    newItem->next = bufHead;

    if(head == NULL){
        head = newItem;
        return;
    }

    head = newItem;
}

template <typename T>
void popBack(singleListItem<T> *&head){
    if(head == NULL){return;}

    if(head->next == NULL){
        delete head;
        head = NULL;
        return;
    }
    
    singleListItem<T> *bufIt = head;
    while(bufIt->next->next != NULL){
        bufIt = bufIt->next;
    }

    delete bufIt->next->next;
    bufIt->next = NULL;
}

template <typename T>
void popFront(singleListItem<T> *&head){
    if(head == NULL){return;}

    singleListItem<T> *bufHead = head;
    head = head->next;
    delete bufHead;
}

template <typename T>
void insertItem(singleListItem<T> *&head, int index, T data){
    if(index == 0 || head == NULL){pushFront(head, data);}
    else if(index == lengthList(head)){pushBack(head, data);}
    else{
        singleListItem<T> *newItem = new singleListItem<T>;
        newItem->data = data;

        int bufItIndex = 0;
        singleListItem<T> *bufIt = head;
        while (bufIt->next != NULL && bufItIndex != index-1)
        {
            bufIt = bufIt->next;
            ++bufItIndex;
        }

        newItem->next = bufIt->next;
        bufIt->next = newItem;
    }
}

template <typename T>
void deleteItem(singleListItem<T> *&head, int index){
    if(index == 0 || head == NULL){popFront(head);}
    else if(index == lengthList(head)-1 || lengthList(head) == 1){popBack(head);}
    else{
        int bufItIndex = 0;
        singleListItem<T> *bufIt = head;
        while (bufIt->next->next != NULL && bufItIndex + 1 != index)
        {
            bufIt = bufIt->next;
            ++bufItIndex;
        }

        singleListItem<T> *bufnext = bufIt->next;
        bufIt->next = bufIt->next->next;
        delete bufnext;
    }
}