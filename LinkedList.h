/*
 * LinkedList.h
 *
 *  Created on: Dec 11, 2019
 *      Author: ise
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include "Defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct t_LinkedList* LinkedList;
typedef void* element;
typedef element (*CopyFunction)(element);
typedef status (*FreeFunction)(element);
typedef int (*EqualFunction)(element,element);
typedef status (*PrintFunction)(element);
typedef int (*ContainFunction)(element,element);



//Create a new linked list
LinkedList createLinkedList(CopyFunction copyfunc, FreeFunction freefunc, EqualFunction equalfunc, PrintFunction printfunc, ContainFunction containfunc);

//Releases the memory allocated for the linked list
status destroyList(LinkedList list);

//Insert an element to the end of the linked list
status appendNode(LinkedList list, element data);

//Removes the element if found
status deleteNode(LinkedList list, element data);

//Prints all the elements in the linked list
status displayList(LinkedList list);

//Copy the list
LinkedList CopyList(LinkedList list);

//Checks if the element is found by key
element searchByKeyInList(LinkedList list,element key);



#endif /* LINKEDLIST_H_ */
