/*
 * LinkedList.c
 *
 *  Created on: Dec 11, 2019
 *      Author: ise
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkedList.h"
#include "Defs.h"

// A structure that represents linked-list's node
typedef struct t_Node {
	struct t_Node* next;
	element data;
} Node;


// A structure that represents a generic linked list
struct t_LinkedList {
	Node* Head;
	Node* Tail;
	int length;
	CopyFunction copyelement;
	FreeFunction freeelement;
	EqualFunction equalelement;
	PrintFunction printelement;
	ContainFunction containelement;
};
 // Init the Node
static Node* createNode(LinkedList list, element data) {
	if (list == NULL || data == NULL)
		return NULL;
	Node* node = (Node*) malloc(sizeof(Node));
	if (node == NULL){
		printf("No memory available.\n");
		return NULL;
	}
	node->data = list->copyelement(data);
	node->next = NULL;
	return node;
}

// Init the linked list
LinkedList createLinkedList(CopyFunction copyfunc, FreeFunction freefunc,EqualFunction equalfunc, PrintFunction printfunc,ContainFunction containfunc) {
	if (copyfunc == NULL || freefunc == NULL || equalfunc == NULL || printfunc == NULL || containfunc == NULL)
		return NULL;
	LinkedList list = (LinkedList) malloc(sizeof(struct t_LinkedList));
	if (list == NULL){
		printf("No memory available.\n");
		return NULL;
	}
	list->Head = NULL;
	list->Tail = NULL;
	list->length = 0;
	list->copyelement = copyfunc;
	list->freeelement = freefunc;
	list->equalelement = equalfunc;
	list->printelement = printfunc;
	list->containelement = containfunc;
	return list;
}

// Destroy the list
status destroyList(LinkedList list) {
	if (list == NULL)
		return NotValidInput;

	if (list->Head == NULL)
		return failure;

	Node* node = list->Head;
	while (node != NULL) {
		Node* temp = node->next;
		// Delete the node
		deleteNode(list, node->data);
		node = temp;
	}
	free(list);
	return success;

}

// Appened Node
status appendNode(LinkedList list, element data) {
	if (list == NULL || data == NULL)
		return NotValidInput;

	Node* toAppend = createNode(list, data);
	if (toAppend == NULL){
		printf("No memory available.\n");
		return NoMemory;
	}
	list->freeelement(data);

	if (list->Head == NULL) {
		list->Head = toAppend;
		list->Tail = toAppend;
		list->length++;
		return success;
	}

	else {
		list->Tail->next = toAppend;
		list->Tail = toAppend;
		list->length++;
		return success;
	}
}

// Delete the Node
status deleteNode(LinkedList list, element data) {
	if (list == NULL || data == NULL)
		return NotValidInput;

	if (list->Head == NULL)
		return failure;
	element toCheck = list->copyelement(data);
	Node* node = list->Head;

	if (list->equalelement(node->data, toCheck) == 0) {
		Node* temp = node->next;
		list->Head = temp;
		list->freeelement(node->data);
		list->length--;
		free(node);
		list->freeelement(toCheck);
		return success;
	}

	while (node != NULL) {
		if (list->equalelement(node->next->data, toCheck) == 0) {
			Node* temp = node->next;
			if (node->next == list->Tail) {
				list->Tail = node;
				node->next = NULL;
				list->freeelement(temp->data);
				list->length--;
				free(temp);
				list->freeelement(toCheck);
				return success;
			} else {
				node->next = node->next->next;
				list->freeelement(temp->data);
				list->length--;
				free(temp);
				list->freeelement(toCheck);
				return success;
			}
		} else {
			node = node->next;
		}
	}
	return failure;
}

// Print the list
status displayList(LinkedList list) {
	if (list == NULL) {
		return NotValidInput;
	}
	Node* node = list->Head;
	while (node != NULL) {
		list->printelement(node->data);
		node = node->next;
	}
	return success;
}

// Search if key exist in the list
element searchByKeyInList(LinkedList list, element key) {
	if (list == NULL || key == NULL || list->Head == NULL) {
		return NULL;
	}
	Node* node = list->Head;
	while (node != NULL) {
		if (list->containelement(node->data, key) == 0) {
			return node->data;
		} else {
			node = node->next;
		}
	}
	return NULL;
}

