#include "List.h"
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
List* ListNew(){
    List* ret = malloc(sizeof(List));

    ret->first = NULL;
    ret->current = NULL;
    ret->size = 0;

    return ret;
}

void ListDestroy(List* _list){
    Node* node = _list->first;
    Node* next = NULL;
    while(node != NULL){
        next = node->next;
        free(node);
        node = next;
    }
    _list->current = NULL;
    _list->first = NULL;
    _list->size = 0;
}

//next
void ListNext(List* _list){
    //empty case
    if(_list->first)
        if(_list->current->next) 
            _list->current = _list->current->next;
}

//prev
void ListPrev(List* _list){
    //empty case
    if(_list->first)
        if(_list->current->prev)
            _list->current = _list->current->prev;
}

//get element at index
Point ListGetElementAt(List* _list, int index){
    if(index > 0 && index <= _list->size){
        Node* n = _list->first;
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

int ListGetIndex(List* l, Node* n){
    int i = 0;
    while((void*)l != (void*)n){
        if(!n->next)
            return -1;
        i++;
        n = n->next;
    }
    return i;
}

//insert to current and return the current new
Node* ListInsert(List* _list, Point point){
    Node* node = NodeNew();
    node->point = point;
    //if non empty and insert place isn't first
    if(_list->current && _list->current != _list->first){
        _list->current->prev->next = node;
        node->prev = _list->current->prev;
        _list->current->prev = node;
        node->next = _list->current;
        _list->current = node;
    }
    else{//if list empty or insert pos is first
        if(_list->size > 0){
            node->next = _list->current;
            _list->current->prev = node;
        }
        _list->current = node;
        _list->first = node;
    }
    _list->size++;

    return node;
}

//insert to index
void ListInsertAt(List* _list, Point _point, int _index){
    Node* add_node;
    if(_index > 0 && _index < _list->size){ // not first nor last
        Node* node = _list->first;
        for(int i=0; i<_index; ++i){
            node = node->next;
        }
        add_node = NodeNew();
        add_node->point = _point;
        add_node->next = node;
        add_node->prev = node->prev;
        node->prev->next = add_node;
        node->prev = add_node;
        _list->size++;
    }
    else if(_index == 0){ //insert first
        add_node = NodeNew();
        add_node->point = _point;
        add_node->prev = NULL;
        if(!_list->size){//if empty
            add_node->next = NULL;
        }
        else{
            add_node->next = _list->first;
            _list->first->prev = add_node;
        }
        _list->first = add_node;
        _list->size++;
    }
    else if(_index == _list->size){ // insert last
        Node* node = _list->first;
        for(int i=0; i<_index-1; ++i){
            node = node->next;
        }
        add_node = NodeNew();
        add_node->point = _point;
        add_node->prev = node;
        add_node->next = NULL;
        node->next = add_node;
        _list->size++;
    }
    //update current
    if(add_node)
        if(ListGetIndex(_list, add_node) < ListGetIndex(_list, _list->current))
            _list->current = _list->current->prev;
}

//delete current
void ListDelete(List* _list){
    if(_list->first){
        if(_list->current->prev)
            _list->current->prev->next = _list->current->next;
        if(_list->current->next)
            _list->current->next->prev = _list->current->prev;
        Node* tmp;
        if(_list->current->next)
            tmp = _list->current->next;
        else
            tmp = _list->current->prev;
        free(_list->current);
        _list->current = tmp;
        _list->size--;
    }
}

//delete index
void ListDeleteAt(List* _list, int _index){
    if(_list->first && _index > 0 && _index <= _list->size){
        Node* node = _list->first;
        for(int i=0; i<_index-1; ++i){ // INDEX + 1 ??
            node = node->next;
        }
        if(node->prev)
            node->prev->next = node->next;
        if(node->next)
            node->next->prev = node->prev;
        if(node == _list->first)
            _list->first = _list->first->next;
        if(node == _list->current){
            if(_list->current->next)
                _list->current = _list->current->next;
            else
                _list->current = _list->current->prev;
        }
        free(node);
        _list->size--;
    }
}

//////////// Display
void ListDisplay(List* _list){
    if(_list->first){
        printf("\n---------DISPLAY LIST----------\n");
        Node* node = _list->first;
        for(int i=0; i<_list->size; ++i){
            printf("%d : (%d,%d)", i, node->point.x, node->point.y);
            if(node == _list->current) printf(" -- Current");
            printf("\n");
            node = node->next;
        }
        printf("-------------------------------\n");
    }
    else{
        printf("YEET\n");
    }
}

void ListDisplayPoint(List* _list, int _index){
    if(_list->first){
       printf("\nPoint at %d : (%d,%d)\n", _index, ListGetElementAt(_list,_index).x, ListGetElementAt(_list,_index).y);
    }
}