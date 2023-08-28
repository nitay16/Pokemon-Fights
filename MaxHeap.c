/*
 * MaxHeap.c
 *
 *  Created on: Dec 12, 2019
 *      Author: ise
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "MaxHeap.h"
#include "Defs.h"


// A structure that represents a generic MaxHeap
struct t_MaxHeap {
	element* arr;
	int lastIndex;
	int MaxSize;
	char* HeapName;
	CopyFunctionElement copyElement;
	FreeFunctionElement freeElement;
	CompareFunctionElement compareElement;
	PrintFunctionElement printElement;

};

// Init the MaxHeap
MaxHeap CreateHeap(char* HeapName,int MaxSize,CopyFunctionElement copyfunc, FreeFunctionElement freefunc, CompareFunctionElement comparefunc, PrintFunctionElement printfunc){
	if(HeapName == NULL || MaxSize <= 0 || copyfunc == NULL || freefunc == NULL || comparefunc == NULL || printfunc == NULL)
		return NULL;

	MaxHeap Heap = (MaxHeap)malloc(sizeof(struct t_MaxHeap));
	if(Heap == NULL){
		printf("No memory available.\n");
		return NULL;
	}
	Heap->HeapName = (char*)malloc(sizeof(char)*(strlen(HeapName)+1));
	if (Heap->HeapName == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}

	Heap->arr = (element*)malloc(sizeof(element)*MaxSize);
	if (Heap->arr == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	for(int i = 0 ; i<MaxSize ; i++){
		Heap->arr[i] = NULL;
	}
	strcpy(Heap->HeapName, HeapName);
	Heap->MaxSize = MaxSize;
	Heap->lastIndex = 0;
	Heap->copyElement = copyfunc;
	Heap->freeElement = freefunc;
	Heap->compareElement = comparefunc;
	Heap->printElement = printfunc;
	return Heap;
}


static status Left(int Index){
	if(Index < 0)
		return NotValidInput;
	return 2*Index;
}

static status Right(int Index){
	if(Index < 0)
		return NotValidInput;
	return (2*Index)+1;
}

// Save the MaxHeap's attribute
static void MaxHeapify(MaxHeap Heap, int Index){
	if(Heap == NULL || Index<0)
		return;

	int l = Left(Index);
	int r = Right(Index);
	int largest;

	if(l<Heap->MaxSize && Heap->compareElement(Heap->arr[l],Heap->arr[Index])>0 ){
		largest = l;

	}
	else{
		largest = Index;
	}
	if(r<Heap->MaxSize && Heap->compareElement(Heap->arr[r],Heap->arr[largest])>0 ){
		largest = r;
	}
	if(largest != Index){
		element temp = Heap->arr[Index];
		Heap->arr[Index] = Heap->arr[largest];
		Heap->arr[largest] = temp;
		MaxHeapify(Heap, largest);
	}
}

// Destroy the Heap
status destroyHeap(MaxHeap Heap){
	if(Heap == NULL)
		return NotValidInput;
	if(Heap->lastIndex == 0){
		free(Heap->HeapName);
		free(Heap->arr);
		free(Heap);
		return success;
	}
	for(int i = 0 ; i < Heap->lastIndex ; i++){
		Heap->freeElement(Heap->arr[i]);
	}
	free(Heap->HeapName);
	free(Heap->arr);
	free(Heap);
	return success;
}

// Insert to Heap
status insertToHeap(MaxHeap Heap , element data){
	if(Heap == NULL || data == NULL)
		return NotValidInput;

	element toInsert = Heap->copyElement(data);
	Heap->freeElement(data);
	if(Heap->lastIndex < Heap->MaxSize){
		Heap->arr[Heap->lastIndex] = toInsert;
		if (Heap->arr[Heap->lastIndex] == NULL) // Checks if we Inserted the type to the designated index
		{
			return failure;
		}
		for(int i = Heap->lastIndex/2 ; i>=0 ; i--){
			MaxHeapify(Heap, i);
		}
	}
	Heap->lastIndex++;
	return success;

}

// Delete the Max element
element PopMaxHeap (MaxHeap Heap){
	if(Heap == NULL)
		return NULL;
	if(Heap->lastIndex == 0)
		return NULL;
	element max = Heap->copyElement(Heap->arr[0]);
	Heap->freeElement(Heap->arr[0]);
	Heap->arr[0] = Heap->arr[Heap->lastIndex - 1];
	Heap->arr[Heap->lastIndex - 1] = NULL;
	Heap->lastIndex--;
	MaxHeapify(Heap, 0);
	return max;

}

// Copy the Heap (deep copy)
MaxHeap copyHeap(MaxHeap Heap){
	if(Heap == NULL){
		return NULL;
	}
	MaxHeap CopyHeap = CreateHeap(Heap->HeapName, Heap->MaxSize ,Heap->copyElement, Heap->freeElement, Heap->compareElement , Heap->printElement);
	if(CopyHeap == NULL){
		return NULL;
	}
	for(int i=0 ; i<Heap->lastIndex ; i++){
		element copyElement = Heap->copyElement(Heap->arr[i]);
		insertToHeap(CopyHeap , copyElement);
	}
	return CopyHeap;
}

// Print the Heap elements
status printHeap(MaxHeap Heap){
	if(Heap == NULL){
		return NotValidInput;
	}
	if(Heap->lastIndex == 0){
		printf("%s:\nNo elements.\n\n", Heap->HeapName);
		return success;
	}
	MaxHeap CopyHeap = copyHeap(Heap);
	printf("%s:\n", CopyHeap->HeapName);
	for(int i=0 ; i<Heap->lastIndex ; i++){
		MaxHeap POP;
		printf("%d. ",i+1);
		POP = PopMaxHeap(CopyHeap);
		CopyHeap->printElement(POP);
		CopyHeap->freeElement(POP);
	}
	destroyHeap(CopyHeap);
	return success;

}

// return the Max element
element TopMaxHeap(MaxHeap Heap){
	if(Heap == NULL){
		return NULL;
	}
	if(Heap->lastIndex == 0){
		return NULL;
	}
	return Heap->arr[0];
}

char* getHeapId(MaxHeap Heap){
	return Heap->HeapName;
}

int getHeapCurrentSize(MaxHeap Heap){
	return Heap->lastIndex;
}

// Compare between Heaps
int compareHeap(MaxHeap Heap, MaxHeap HeapToCompare){
	if(Heap == NULL || HeapToCompare == NULL)
		return 1;
	if(strcmp(Heap->HeapName,HeapToCompare->HeapName) != 0 || Heap->MaxSize != HeapToCompare->MaxSize || Heap->lastIndex != HeapToCompare->lastIndex)
		return 1;
	for(int i=0 ; i<Heap->lastIndex ; i++){
		if(Heap->compareElement(Heap->arr[i],HeapToCompare->arr[i])==1)
			return 1;
	}
	return 0;
}

// Find if the name of the Heap is like the key
int CompareByName(MaxHeap Heap , char* key){
	if( Heap == NULL || key == NULL){
		return 1;
	}
	if(strcmp(getHeapId(Heap) , key) == 0 )
		return 0;
	return 1;

}

