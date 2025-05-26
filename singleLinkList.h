template <typename T>
struct singleListItem {
    T data;
    singleListItem<T> *next;
};

template <typename T>
uint16_t lengthList(singleListItem<T>*&);

template <typename T>
void printList(singleListItem<T>*&);

template <typename T>
void deleteList(singleListItem<T>*&);

template <typename T>
void pushBack(singleListItem<T>*&, T);

template <typename T>
void pushBackList(singleListItem<T>*&, singleListItem<T>*&);

template <typename T>
void pushFront(singleListItem<T>*&, T);

template <typename T>
void popBack(singleListItem<T>*&);

template <typename T>
void popFront(singleListItem<T>*&);

template <typename T>
void insertItem(singleListItem<T>*&, int, T);

template <typename T>
void deleteItem(singleListItem<T>*&, int, T);