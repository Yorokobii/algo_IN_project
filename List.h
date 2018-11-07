#ifndef LIST_H
#define LIST_H

/* List structure declaration*/

typedef struct Point{
    int x,y;
} Point;

typedef struct Node{

    Point point;
    struct Node* next;
    struct Node* prev;
} Node;

//new node
Node* NodeNew();

typedef struct List{
    Node* first;
    Node* current;
    int size;
} List;

//New
List* ListNew();

//Destructor
void ListDestroy(List*);

//next
void ListNext(List*);

//prev
void ListPrev(List*);

//get element at index
Point ListGetElementAt(List*, int);

int ListGetIndex(List*, Node*);

//insert to current and return the new current
Node* ListInsert(List*, Point);

//insert to index
void ListInsertAt(List*, Point, int);

//delete current
void ListDelete(List*);

//delete index
void ListDeleteAt(List*, int);

//display
void ListDisplay(List*);
void ListDisplayPoint(List*, int);

#endif //LIST_H