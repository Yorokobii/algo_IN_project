#include "Polygon.h"
#include <stdlib.h>
#include <stdio.h>

void debug(char* str){
    printf("\n%s\n", str);
}

Node* NodeNew(){
    Node* node = malloc(sizeof(Node));

    node->point.x = 0;
    node->point.y = 0;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

//New
Polygon* PolyNew(){
    Polygon* ret = malloc(sizeof(Polygon));

    ret->first = NULL;
    ret->current = NULL;
    ret->size = 0;

    return ret;
}

void PolyDestroy(Polygon* _poly){
    Node* node = _poly->first;
    Node* next = NULL;
    while(node != NULL){
        next = node->next;
        free(node);
        node = next;
    }
    _poly->current = NULL;
    _poly->first = NULL;
    _poly->size = 0;
}

//next
void PolyNext(Polygon* _poly){
    //empty case
    if(_poly->first)
        if(_poly->current->next) 
            _poly->current = _poly->current->next;
}

//prev
void PolyPrev(Polygon* _poly){
    //empty case
    if(_poly->first)
        if(_poly->current->prev)
            _poly->current = _poly->current->prev;
}

//get element at index
Point PolyGetElementAt(Polygon* _poly, int index){
    if(index > 0 && index <= _poly->size){
        Node* n = _poly->first;
        int i=0;
        for(;i<index-1; ++i)
            n = n->next;
        return n->point;
    }
    else{
        Point defP;
        defP.x = 0;
        defP.y = 0;
        return defP;
    }
}

//insert to current and return the current new
Node* PolyInsert(Polygon* _poly, Point point){
    Node* node = NodeNew();
    node->point = point;
    //if non empty and insert place isn't first
    if(_poly->current && _poly->current != _poly->first){
        _poly->current->prev->next = node;
        node->prev = _poly->current->prev;
        _poly->current->prev = node;
        node->next = _poly->current;
        _poly->current = node;
    }
    else{//if poly empty or insert pos is first
        if(_poly->size > 0){
            node->next = _poly->current;
            _poly->current->prev = node;
        }
        _poly->current = node;
        _poly->first = node;
    }
    _poly->size++;

    return node;
}

//insert to index
void PolyInsertAt(Polygon* _poly, Point _point, int _index){
    if(_index > 0 && _index < _poly->size){ // not first nor last
        Node* node = _poly->first;
        for(int i=0; i<_index; ++i){
            node = node->next;
        }
        Node* add_node = NodeNew();
        add_node->point = _point;
        add_node->next = node;
        add_node->prev = node->prev;
        node->prev->next = add_node;
        node->prev = add_node;
        _poly->size++;
    }
    else if(_index == 0){ //insert first
        Node* add_node = NodeNew();
        add_node->point = _point;
        add_node->prev = NULL;
        if(_poly->size){//if empty
            add_node->next = NULL;
        }
        else{
            add_node->next = _poly->first;
        }
        _poly->first = add_node;
        _poly->size++;
    }
    else if(_index == _poly->size){ // insert last
        Node* node = _poly->first;
        for(int i=0; i<_index-1; ++i){
            node = node->next;
        }
        Node* add_node = NodeNew();
        add_node->point = _point;
        add_node->prev = node;
        add_node->next = NULL;
        node->next = add_node;
        _poly->size++;
    }
}

//delete current
void PolyDelete(Polygon* _poly){
    if(_poly->first){
        if(_poly->current->prev)
            _poly->current->prev->next = _poly->current->next;
        if(_poly->current->next)
            _poly->current->next->prev = _poly->current->prev;
        free(_poly->current);
        _poly->size--;
    }
}

//delete index
void PolyDeleteAt(Polygon* _poly, int _index){
    if(_poly->first && _index > 0 && _index <= _poly->size){
        Node* node = _poly->first;
        for(int i=0; i<_index; ++i){ // INDEX + 1 ??
            node = node->next;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
        _poly->size--;
    }
}

//////////// Display
void PolyDisplay(Polygon* _poly){
    if(_poly->first){
        printf("\n---------DISPLAY LIST----------\n");
        Node* node = _poly->first;
        for(int i=0; i<_poly->size; ++i){
            printf("%d : (%d,%d)\n", i, node->point.x, node->point.y);
            node = node->next;
        }
        printf("-------------------------------\n");
    }
    else{
        printf("YEET\n");
    }
}

void PolyDisplayPoint(Polygon* _poly, int _index){
    if(_poly->first){
       printf("\nPoint at %d : (%d,%d)\n", _index, PolyGetElementAt(_poly,_index).x, PolyGetElementAt(_poly,_index).y);
    }
}