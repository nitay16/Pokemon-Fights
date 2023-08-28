/*
 * MaxHeap.h
 *
 *  Created on: Dec 12, 2019
 *      Author: ise
 */

#ifndef MAXHEAP_H_
#define MAXHEAP_H_
#include "Defs.h"

typedef struct t_MaxHeap* MaxHeap;
typedef void* element;
typedef element (*CopyFunctionElement)(element);
typedef status (*FreeFunctionElement)(element);
typedef int (*CompareFunctionElement)(element,element);
typedef status (*PrintFunctionElement)(element);


//Create a new max heap
MaxHeap CreateHeap(char* HeapName,int MaxSize,CopyFunctionElement copyfunc, FreeFunctionElement freefunc, CompareFunctionElement comparefunc, PrintFunctionElement printfunc);

//Releases the memory allocated for the max heap
status destroyHeap(MaxHeap Heap);

//Insert an element to the max heap
status insertToHeap(MaxHeap Heap, element data);

//Prints all the elements in the max heap
status printHeap(MaxHeap Heap);

//Delete and return the max element
element PopMaxHeap(MaxHeap Heap);

MaxHeap copyHeap(MaxHeap Heap);

//Return max element
element TopMaxHeap(MaxHeap Heap);

//Return Heap name
char* getHeapId(MaxHeap Heap);

//Return heap size
int getHeapCurrentSize(MaxHeap Heap);

// Compare two Heaps
int compareHeap(MaxHeap Heap, MaxHeap HeapToCompare);

// Compare Heap name to key
int CompareByName(MaxHeap Heap , char* key);


#endif /* MAXHEAP_H_ */
