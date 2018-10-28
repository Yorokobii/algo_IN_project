#ifndef POLYGON_H
#define POLYGON_H

/* Polygone structure declaration*/

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

typedef struct Polygon{
    Node* first;
    Node* current;
    int size;
} Polygon;

//New
Polygon* PolyNew();

//Destructor
void PolyDestroy(Polygon*);

//next
void PolyNext(Polygon*);

//prev
void PolyPrev(Polygon*);

//get element at index
Point PolyGetElementAt(Polygon*, int);

//insert to current and return the new current
Node* PolyInsert(Polygon*, Point);

//insert to index
void PolyInsertAt(Polygon*, Point, int);

//delete current
void PolyDelete(Polygon*);

//delete index
void PolyDeleteAt(Polygon*, int);

//display
void PolyDisplay(Polygon*);
void PolyDisplayPoint(Polygon*, int);

#endif //POLYGON_H