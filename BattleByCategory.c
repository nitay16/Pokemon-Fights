/*
 * BattleByCategory.c
 *
 *  Created on: Dec 17, 2019
 *      Author: ise
 */
#include "BattleByCategory.h"
#include "MaxHeap.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct battle_s{
	int Capacity;
	int NumberOfCategories;
	char** CategoriesNames;
	LinkedList CategoryList;
	equalFunction equalElement;
	copyFunction copyElement;
	freeFunction freeElement;
	getCategoryFunction getCategory;
	getAttackFunction getAttack;
	printFunction printElement;
};

// Casting to Heap
static element CopyToHeap(element E){
	MaxHeap NewHeap = (MaxHeap)E;
	return copyHeap(NewHeap);
}

// Casting to Heap
static status FreeToHeap(element E){
	MaxHeap Heap = (MaxHeap)E;
	return destroyHeap(Heap);
}

// Casting to Heap
static int CompareToHeap(element E1 , element E2){
	MaxHeap NewHeap1 = (MaxHeap)E1;
	MaxHeap NewHeap2 = (MaxHeap)E2;
	return compareHeap(NewHeap1,NewHeap2);
}

// Casting to Heap
static status PrintToHeap(element E){
	MaxHeap NewHeap = (MaxHeap)E;
	return printHeap(NewHeap);
}

// Casting to Heap
static int ContainToHeap(element E , element key){
	MaxHeap NewHeap = (MaxHeap)E;
	char* NewCategory = (char*)key;
	return CompareByName(NewHeap,NewCategory);


}

//Init the Battle
Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement){
	if(capacity <= 0 || numberOfCategories <= 0 || categories == NULL || equalElement == NULL || copyElement == NULL || freeElement == NULL || getCategory == NULL || getAttack == NULL || printElement == NULL)
		return NULL;
	Battle battle = (Battle)malloc(sizeof(struct battle_s));
	if(battle == NULL){
		printf("No memory available.\n");
		return NULL;
	}

	battle->Capacity = capacity;
	battle->NumberOfCategories = numberOfCategories;

	//Creating array of pointers to categories
	battle->CategoriesNames = (char**)malloc(sizeof(char*)*(numberOfCategories));
	if (battle->CategoriesNames == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}

	// Seprates the types by ","
	char *token;
	token = strtok(categories,",");
	int i = 0;
	while(token!=NULL){
		battle->CategoriesNames[i] = token;
		i++;
		token = strtok(NULL,",");
	}
	battle->copyElement = copyElement;
	battle->equalElement = equalElement;
	battle->freeElement = freeElement;
	battle->getAttack = getAttack;
	battle->getCategory = getCategory;
	battle->printElement = printElement;

	// Create linked list of Heaps
	battle->CategoryList = createLinkedList(CopyToHeap,FreeToHeap,CompareToHeap,PrintToHeap,ContainToHeap);
	if(battle->CategoryList == NULL){
		return NULL;
	}
	//Creating Heaps for each node
	for(int i =0 ; i < numberOfCategories ; i++){
		MaxHeap HeapToInsert = CreateHeap(battle->CategoriesNames[i],capacity,copyElement,freeElement,equalElement,printElement);
		if(HeapToInsert == NULL){
			return NULL;
		}
		appendNode(battle->CategoryList, HeapToInsert);
	}
	return battle;
}

// Destroy the Battle
status destroyBattleByCategory(Battle b){
	if(b == NULL){
		return NotValidInput;
	}
	destroyList(b->CategoryList);
	for(int i =0 ; i < b->NumberOfCategories ; i++){
		b->CategoriesNames[i] = NULL;
	}
	free(b->CategoriesNames);
	free(b);
	return success;
}

// Insert object to the battle
status insertObject(Battle b, element elem){
	if(b == NULL || elem == NULL)
		return NotValidInput;
	MaxHeap toInsert = searchByKeyInList(b->CategoryList,b->getCategory(elem));
	if( toInsert != NULL){
		if(getHeapCurrentSize(toInsert) < b->Capacity){
			insertToHeap(toInsert, elem);
			return success;
		}
	}
	return failure;
}

// Print the battle
void displayObjectsByCategories(Battle b){
	if(b == NULL){
		return;
	}
	displayList(b->CategoryList);
}

// Remove the Max element by category
element removeMaxByCategory(Battle b,char* category){
	if(b == NULL || category == NULL)
		return NULL;
	MaxHeap toRemove = searchByKeyInList(b->CategoryList,category);
	return PopMaxHeap(toRemove);
}


int getNumberOfObjectsInCategory(Battle b,char* category){
	if(b == NULL || category == NULL)
		return 0;
	MaxHeap toCheck = searchByKeyInList(b->CategoryList,category);
	return getHeapCurrentSize(toCheck);
}

// Fight
element fight(Battle b,element elem){
	int AttackCheck = 0;
	int AttackMax = 0;
	int Attackelem = 0;
	element TheMax;
	element Check;
	int fight1;
	int fight2;
	// Finding the first category that not empty
	for ( int i = 0 ; i < b->NumberOfCategories ; i++){
		TheMax = TopMaxHeap(searchByKeyInList(b->CategoryList,b->CategoriesNames[i]));
		if( TheMax == NULL){
			continue;
		}
		else{
			// Finding the second category that not empty
			for( int j = 1 ; j < b->NumberOfCategories ; j++){
				Check = TopMaxHeap(searchByKeyInList(b->CategoryList,b->CategoriesNames[j]));
				if( Check == NULL){
					continue;
				}
				else
					// Comparing attacks between the  two category's max with the elem
					fight1=b->getAttack(Check,elem,&AttackCheck,&Attackelem);
					fight2=b->getAttack(TheMax,elem,&AttackMax,&Attackelem);
					// If the Check attack is bigger than the TheMax, change between them
					if(fight1 > fight2){
						TheMax = Check;
						AttackMax = AttackCheck;
						AttackCheck = 0;
					}
			}
			// If there are no second category to compare
			if( Check == NULL){
				// Getting TheMax attack against elem
				fight2=b->getAttack(TheMax,elem,&AttackMax,&Attackelem);
				break;
			}
			break;
		}
	}
	// If there are no categories to compare
	if(TheMax == NULL){
		printf("There are no Pokemons to fight against you.\n");
		return elem;
	}

	printf("The final battle between:\n");
	b->printElement(elem);
	printf("In this battle his attack is :%d\n\nagainst ",Attackelem);
	b->printElement(TheMax);
	printf("In this battle his attack is :%d\n\n",AttackMax);
	if(b->getAttack(TheMax,elem,&AttackMax,&Attackelem) > 0){
		printf("THE WINNER IS:\n");
		b->printElement(TheMax);
		return TheMax;
	}
	else if(b->getAttack(TheMax,elem,&AttackMax,&Attackelem) == 0){
		printf("IT IS A DRAW.\n");
		return TheMax;
	}
	else
		printf("THE WINNER IS:\n");
		b->printElement(elem);
		return elem;
}








































