#ifndef CONNECTION_H
#define CONNECTION_H

#include "vertexid.h"
#include <stdlib.h>

typedef struct {
	double cost;
	vertexid_t vertex1;
	vertexid_t vertex2;
} Element;

typedef struct {
    int current;
    int size;
    int increment_rate;
    Element *elements;
} ArrayList;

typedef enum {
    RIFHT, LEFT
} Shift;

// public functions:
void        init           (ArrayList*const);
void        initWithSize   (ArrayList*const, int);
void        initWithSizeAndIncRate    (ArrayList*const, int, int);
void        clean          (ArrayList*);
int         add            (ArrayList*const, Element);
int         arrayInsert         (ArrayList*const, Element, int);
Element*    removeAt       (ArrayList*const, int);
void        clear          (ArrayList*const);
int         set            (ArrayList*const, Element, int);
Element*    get            (ArrayList*const, int);
void        print          (const ArrayList*const);
int         lastIndexOf    (const ArrayList*const, Element);
int         indexOf        (const ArrayList*const, Element);
int         isEmpty        (const ArrayList*const);

// static (private) utility functions:

/* Abstracting the print method of the element by delegating it to the element itself (OOP-like feature) */
void    printElement(const Element*const);
void    shift(ArrayList *const list, int index, int rooms, Shift dir);
void    wide(ArrayList* const);
void    arraryCopy(void *dest, int dIndex, const void* src, int sIndex, int len, int destLen, size_t size);
#endif