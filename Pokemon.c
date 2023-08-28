/*
 ============================================================================
 Name        : Pokemon.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Pokemon.h"
#include "Defs.h"


// Create the Pokemon type
PokemonType* CreatePokemonType(char *TypeName)
{
	if(TypeName == NULL) // Checks if the type name is valid
		return NULL;
	PokemonType *type = (PokemonType*)malloc(sizeof(PokemonType)); // Allocating memory for type
	if (type == NULL) // Check if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	type->TypeName = (char*)malloc(sizeof(char)*(strlen(TypeName)+1)); // Allocating memory for the name of the type
	if (type->TypeName == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	strcpy(type->TypeName, TypeName); // Copying the name we got, to the name of the type
	type->effective_against_me = NULL;
	type->effective_against_others = NULL;
    type->TypeAmount = 0;
    type->Length_against_me = 0;
    type->Length_against_others = 0;
	return type;
}

// Create Pokemon's biologic info
BiologicInfo* InsertBiologicInfo(float Height, float Weight, int Attack)
{
	BiologicInfo *info = (BiologicInfo*)malloc(sizeof(BiologicInfo)); // Allocating memory for biological info
	if (info == NULL) // Check if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	info->Height = Height;
	info->Weight = Weight;
	info->Attack = Attack;
	return info;
}

// Create the Pokemon
Pokemon* CreatePokemon(char *Name,char *Species,float Height, float Weight, int Attack, PokemonType* type)
{
	// Checks if the values are valid
	if(Name == NULL)
		return NULL;
	if(Species == NULL)
		return NULL;
	if(type == NULL)
		return NULL;
	Pokemon *pokemon = (Pokemon*)malloc(sizeof(Pokemon)); // Allocating memory for pokemon
	if (pokemon == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	pokemon->Name = (char*)malloc(sizeof(char)*(strlen(Name)+1)); // Allocating memory for pokemon's name
	if (pokemon->Name == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	strcpy(pokemon->Name, Name); // Copying the name we got, to the name of the pokemon
	pokemon->Species = (char*)malloc(sizeof(char)*(strlen(Species)+1)); // Allocating memory for pokemon's species
	if (pokemon->Species == NULL) // Checks if the allocation succeed
	{
		printf("No memory available.\n");
		return NULL;
	}
	strcpy(pokemon->Species, Species); // Copying the species we got, to the species of the pokemon
	pokemon->Bio=InsertBiologicInfo(Height, Weight, Attack); // Create pokemon's bio
	pokemon->Type = type;
	type->TypeAmount++; // Increasing counter for the amount of the type
	return pokemon;
}

// Adding pokemon type to the effective against me list of the source's type
status AddEffective_against_me(PokemonType *A, PokemonType *B)
{
	// Checking if the values are valid
	if(A == NULL || B == NULL)
	{
		return NotValidInput;
	}
	// Checking if the list is empty
	if(A->effective_against_me == NULL)
	{
		// In case list is empty, allocating new memory for the list
		A->effective_against_me = (PokemonType**)malloc(sizeof(PokemonType*));
		if (A->effective_against_me == NULL) // Checks if the allocation succeed
		{
			printf("No memory available.\n");
			return NoMemory;
		}
		A->effective_against_me[0] = B;
		A->Length_against_me++;
		return success;
	}
	// The list isn't empty
	else
	{
		// In case list isn't empty, reallocating memory for the list
		A->effective_against_me = (PokemonType**)realloc(A->effective_against_me,sizeof(A->effective_against_me+1)* sizeof(PokemonType*));
		if (A->effective_against_me == NULL) // Checks if the allocation succeed
		{
			printf("No memory available.\n");
			return NoMemory;
		}
		A->effective_against_me[A->Length_against_me] = B;
		if (A->effective_against_me[A->Length_against_me] == NULL) // Checks if we Insert the type to the designated index
		{
			return failure;
		}
		A->Length_against_me++;
		return success;
	}
}

// Adding pokemon type to the effective against others list of the source's type
status AddEffective_against_others(PokemonType *A, PokemonType *B)
{
	// Checking if the values are valid
	if(A == NULL || B == NULL)
	{
		return NotValidInput;
	}
	// Checking if the list is empty
	if(A->effective_against_others == NULL)
	{
		// In case list is empty, allocating new memory for the list
		A->effective_against_others = (PokemonType**)malloc(sizeof(PokemonType*));
		if (A->effective_against_others == NULL) // Checks if the allocation succeed
		{
			printf("No memory available.\n");
			return NoMemory;
		}
		A->effective_against_others[0] = B;
		A->Length_against_others++;
		return success;
	}
	// The list isn't empty
	else
	{
		// In case list isn't empty, reallocating memory for the list
		A->effective_against_others = (PokemonType**)realloc(A->effective_against_others,(sizeof(A->effective_against_others+1)*sizeof(PokemonType*)));
		if (A->effective_against_others == NULL) // Checks if the allocation succeed
		{
			printf("No memory available.\n");
			return NoMemory;
		}
		A->effective_against_others[A->Length_against_others] = B;
		if (A->effective_against_others[A->Length_against_others] == NULL) // Checks if we Insert the type to the designated index
		{
			return failure;
		}
		A->Length_against_others++;
		return success;
	}
}

// Deleting pokemon type to the effective against me list of the source's type
status DeleteEffective_against_me(PokemonType *A, char *B)
{
	// Checking if the values are valid
	if(A == NULL || B == NULL)
	{
		return NotValidInput;
	}
	// Checking if the list is empty
	if(A->effective_against_me == NULL)
		return failure;
	// The list isn't empty
	else
	{
		// Checks if type B is in A's list
		for (int Index = 0 ; Index < A->Length_against_me ; Index++)
		{
			if(strcmp(A->effective_against_me[Index]->TypeName,B) == 0)
			{
				// If this type is in the list, delete from the list
				PokemonType* temp;
				A->effective_against_me[Index] = NULL; // Put NULL instead of the type we want to delete
				// Once we arrived to the type we want to delete, we will re-order the array by moving the types to the left, for putting the NULL in the last index
				for (int i = Index  ; i < A->Length_against_me ; i++)
				{
					if(i==A->Length_against_me -1)
						break;
					temp = A->effective_against_me[i];
					A->effective_against_me[i] = A->effective_against_me[i+1];
					A->effective_against_me[i+1] = temp;
				}

				A->Length_against_me--;
				free(A->effective_against_me[A->Length_against_me]); // Freeing the last index in the array
				// In case we deleted the only type left on the list, we will free also the list itself
				if(A->Length_against_me == 0)
				{
					free(A->effective_against_me);
					A->effective_against_me = NULL;
					return success;
				}
				return success;
			}
		}
	return failure;
	}
}

// Deleting pokemon type to the effective against others list of the source's type
status DeleteEffective_against_others(PokemonType *A, char *B)
{
	// Checking if the values are valid
	if(A == NULL || B == NULL)
	{
		return NotValidInput;
	}
	// Checking if the list is empty
	if(A->effective_against_others == NULL)
		return failure;
	// The list isn't empty
	else
	{
		// Checks if type B is in A's list
		for (int Index = 0 ; Index < A->Length_against_others ; Index++)
		{

			if(strcmp(A->effective_against_others[Index]->TypeName,B) == 0)
			{
				// If this type is in the list, delete from the list
				PokemonType* temp;
				A->effective_against_others[Index] = NULL; // Put NULL instead of the type we want to delete
				// Once we arrived to the type we want to delete, we will re-order the array by moving the types to the left, for putting the NULL in the last index
				for (int i = Index  ; i < A->Length_against_others - 1; i++)
				{
					if(i==A->Length_against_others -1)
						break;
					temp = A->effective_against_others[i];
					A->effective_against_others[i] = A->effective_against_others[i+1];
					A->effective_against_others[i+1] = temp;
				}

				A->Length_against_others--;
				free(A->effective_against_others[A->Length_against_others]); // Freeing the last index in the array
				// In case we deleted the only type left on the list, we will free also the list itself
				if(A->Length_against_others == 0)
				{
					free(A->effective_against_others);
					A->effective_against_others = NULL;
					return success;
				}
				return success;
			}
		}
	return failure;
	}
}

// Print Pokemon's information
status PrintPokemon(Pokemon *pokemon)
{
	if(pokemon == NULL)
		return NotValidInput;
	else
	{
		printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n",pokemon->Name,pokemon->Species,pokemon->Type->TypeName,pokemon->Bio->Height,pokemon->Bio->Weight,pokemon->Bio->Attack);
		return success;
	}
}

// Print Type's information
status PrintPokemonType(PokemonType *type)
{
	if(type == NULL)
		return NotValidInput;
	else
		// There are no types in the lists
		if(type->Length_against_me == 0 && type->Length_against_others == 0)
		{
			printf("Type %s -- %d pokemons\n\n",type->TypeName,type->TypeAmount);
			return success;
		}
		// There are only types in the super effective against other list
		else if(type->Length_against_me == 0)
		{
			printf("Type %s -- %d pokemons\n\t%s moves are super-effective against:",type->TypeName,type->TypeAmount,type->TypeName);
			for(int Index = 0 ; Index<type->Length_against_others ; Index++)
			{
				if(Index == 0)
					printf("%s",type->effective_against_others[Index]->TypeName);
				else
					printf(" ,%s",type->effective_against_others[Index]->TypeName);
			}
			printf("\n\n");
			return success;
		}
		// There are only types in the super effective against me list
		else if(type->Length_against_others == 0 )
		{
			printf("Type %s -- %d pokemons\n\tThese types are super-effective against %s:",type->TypeName,type->TypeAmount,type->TypeName);
			for(int Index = 0 ; Index<type->Length_against_me ; Index++)
			{
				if(Index == 0)
					printf("%s",type->effective_against_me[Index]->TypeName);
				else
					printf(" ,%s",type->effective_against_me[Index]->TypeName);
			}
			printf("\n\n");
			return success;
		}
		// There types in all lists
		else
		{
			printf("Type %s -- %d pokemons\n\tThese types are super-effective against %s:",type->TypeName,type->TypeAmount,type->TypeName);
				for(int Index = 0 ; Index<type->Length_against_me ; Index++)
				{
					if(Index == 0)
						printf("%s",type->effective_against_me[Index]->TypeName);
					else
						printf(" ,%s",type->effective_against_me[Index]->TypeName);
				}
			printf("\n\t%s moves are super-effective against:",type->TypeName);
			for(int Index = 0 ; Index<type->Length_against_others ; Index++)
			{
				if(Index == 0)
					printf("%s",type->effective_against_others[Index]->TypeName);
				else
					printf(" ,%s",type->effective_against_others[Index]->TypeName);
			}
			printf("\n\n");
			return success;
		}
	return failure;
}

// Copy the Pokemon
Pokemon* CopyPokemon(Pokemon *pokemon){
	if(pokemon == NULL)
		return NULL;
	// Create new Pokemon
	Pokemon* copyPokemon = CreatePokemon(pokemon->Name,pokemon->Species,pokemon->Bio->Height,pokemon->Bio->Weight,pokemon->Bio->Attack,pokemon->Type);
	if(copyPokemon == NULL)
		return NULL;
	return copyPokemon;
}

// Destroy Pokemon
status DestroyPokemon(Pokemon *pokemon)
{
	free(pokemon->Name);
	free(pokemon->Species);
	free(pokemon->Bio);
	pokemon->Type->TypeAmount--;
	pokemon->Type = NULL;
	free(pokemon);
	return success;

}

// Check if the Pokemons are equal
int EqualPokemon(Pokemon *pokemon, Pokemon *toCompare){
	if(pokemon == NULL || toCompare == NULL){
		return -1;
	}
	if(pokemon->Bio->Attack > toCompare->Bio->Attack) // 1 - first bigger
		return 1;
	if(pokemon->Bio->Attack == toCompare->Bio->Attack)  // 0 - same
		return 0;
	return -1; // -1 - second bigger
}

char* getType(Pokemon *pokemon){
	if(pokemon == NULL)
		return NULL;
	return pokemon->Type->TypeName;
}

// return the first attack - second attack. also produce the attack of each element
int getAttack(Pokemon *pokemon,Pokemon *against ,int* pokemonAttack ,int* againstAttack){
	bool minus = false; // label - check if decreased
	*pokemonAttack = pokemon->Bio->Attack;
	*againstAttack = against->Bio->Attack;
	//Check if against is in the pokemon's effective against me list
	for(int i = 0 ; i<pokemon->Type->Length_against_me ; i++){
		if(strcmp(against->Type->TypeName,pokemon->Type->effective_against_me[i]->TypeName) == 0){
			*pokemonAttack = pokemon->Bio->Attack - 10;
			*againstAttack = against->Bio->Attack;
			minus = true;
			break;
		}
	}
	if(minus == false){
		//Check if pokemon is in the against's effective against other list
		for(int i = 0 ; i<against->Type->Length_against_others ; i++){
		if(strcmp(pokemon->Type->TypeName,against->Type->effective_against_others[i]->TypeName) == 0){
			*pokemonAttack = pokemon->Bio->Attack - 10;
			*againstAttack = against->Bio->Attack;
			minus = true;
			break;
		}
	}
	}
	minus = false;
	//Check if pokemon is in the against's effective against me list
	for(int i = 0 ; i<against->Type->Length_against_me ; i++){
		if(strcmp(pokemon->Type->TypeName,against->Type->effective_against_me[i]->TypeName) == 0){
			*pokemonAttack = pokemon->Bio->Attack ;
			*againstAttack = against->Bio->Attack - 10;
			minus = true;
			break;
		}
	}
	if(minus == false){
		//Check if against is in the pokemon's effective against other list
		for(int i = 0 ; i<pokemon->Type->Length_against_others ; i++){
		if(strcmp(against->Type->TypeName,pokemon->Type->effective_against_others[i]->TypeName) == 0){
			*pokemonAttack = pokemon->Bio->Attack ;
			*againstAttack = against->Bio->Attack - 10;
			minus = true;
			break;
		}
	}
	}
	return (*pokemonAttack - *againstAttack);
}













