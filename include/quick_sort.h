#ifndef __QUICK_SORT_H
#define __QUICK_SORT_H

#include "connection.h"

void swap(Element* a, Element* b);
 
int partition (ArrayList* list, int low, int high);
 
void quickSort(ArrayList* list, int low, int high);

#endif
