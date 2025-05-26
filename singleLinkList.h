#include <stdio.h>
#include <iostream>

template <typename T>
struct singleList {
    T data;
    singleList<T> *next;
};

template <typename T>
int lengthList(singleList<T> *&head){
    if(head == NULL){return 0;}

    int len = 0;

    singleList<T> *bufIt = head;
    while (bufIt->next != NULL)
    {
        ++len;
        bufIt = bufIt->next;
    }
    
    return ++len;
}

template <typename T>
void printList(singleList<T> *&head){
    if(head == NULL){
        printf("List in NULL\n");
        return;
    }

    singleList<T> *bufIt = head;
    while(bufIt->next != NULL){
        std::cout << " " << bufIt->data << " ";
        bufIt = bufIt->next;
    }
    std::cout << " " << bufIt->data << std::endl;
}

template <typename T>
void deleteList(singleList<T> *&head){
    if(head->next == NULL){
        head = NULL;
        delete head;
    }else{
        deleteList(head->next);
        deleteList(head);
    }
}

template <typename T>
void pushBack(singleList<T> *&head, T data){
    singleList<T> *newItem = new singleList<T>;
    newItem->next = NULL;
    newItem->data = data;

    if(head == NULL){
        head = newItem;
        return;
    }

    singleList<T> *bufIt = head;
    while(bufIt->next != NULL){
        bufIt = bufIt->next;
    }

    bufIt->next = newItem;
}

template <typename T>
void pushBackList(singleList<T> *&head, singleList<T> *&insertListItem){
    singleList<T> *bufIt = insertListItem;
    while (bufIt->next != NULL)
    {
        pushBack(head, bufIt->data);
        bufIt = bufIt->next;
    }
    pushBack(head, bufIt->data);
}

template <typename T>
void pushFront(singleList<T> *&head, T data){
    singleList<T> *bufHead = head;

    singleList<T> *newItem = new singleList<T>;
    newItem->data = data;
    newItem->next = bufHead;

    if(head == NULL){
        head = newItem;
        return;
    }

    head = newItem;
}

template <typename T>
void popBack(singleList<T> *&head){
    if(head == NULL){return;}

    if(head->next == NULL){
        delete head;
        head = NULL;
        return;
    }
    
    singleList<T> *bufIt = head;
    while(bufIt->next->next != NULL){
        bufIt = bufIt->next;
    }

    delete bufIt->next->next;
    bufIt->next = NULL;
}

template <typename T>
void popFront(singleList<T> *&head){
    if(head == NULL){return;}

    singleList<T> *bufHead = head;
    head = head->next;
    delete bufHead;
}

template <typename T>
void insertItem(singleList<T> *&head, int index, T data){
    if(index == 0 || head == NULL){pushFront(head, data);}
    else if(index == lengthList(head)){pushBack(head, data);}
    else{
        singleList<T> *newItem = new singleList<T>;
        newItem->data = data;

        int bufItIndex = 0;
        singleList<T> *bufIt = head;
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
void deleteItem(singleList<T> *&head, int index){
    if(index == 0 || head == NULL){popFront(head);}
    else if(index == lengthList(head)-1 || lengthList(head) == 1){popBack(head);}
    else{
        int bufItIndex = 0;
        singleList<T> *bufIt = head;
        while (bufIt->next->next != NULL && bufItIndex + 1 != index)
        {
            bufIt = bufIt->next;
            ++bufItIndex;
        }

        singleList<T> *bufnext = bufIt->next;
        bufIt->next = bufIt->next->next;
        delete bufnext;
    }
}