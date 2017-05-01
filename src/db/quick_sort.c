#include "connection.h"

void swap(Element* a, Element* b){
    Element t = *a;
    *a = *b;
    *b = t;
}
 
int partition (ArrayList* list, int low, int high){
    Element* pivot = get(list,high);    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        if (get(list,j)->cost <= pivot->cost)
        {
            i++;    // increment index of smaller element
            swap(get(list,i), get(list,j));
        }
    }
    swap(get(list,i + 1), get(list,high));
    return (i + 1);
}
 
void quickSort(ArrayList* list, int low, int high){
    if (low < high){
        int pi = partition(list, low, high);
 
        quickSort(list, low, pi - 1);
        quickSort(list, pi + 1, high);
    }
}