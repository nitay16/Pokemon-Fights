#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"



typedef struct BiologicInfo
{
   float Height;
   float Weight;
   int Attack;
}BiologicInfo;

typedef struct PokemonType
{
   char *TypeName;
   int TypeAmount;
   struct PokemonType **effective_against_me;
   struct PokemonType **effective_against_others;
   int Length_against_me;
   int Length_against_others;
}PokemonType;


typedef struct Pokemon
{
   char *Name;
   char *Species;
   PokemonType *Type;
   BiologicInfo *Bio;
}Pokemon;


PokemonType* CreatePokemonType(char *);
BiologicInfo* InsertBiologicInfo(float , float , int );
Pokemon* CreatePokemon(char *,char *,float , float , int , PokemonType* );
status AddEffective_against_me(PokemonType* , PokemonType*);
status AddEffective_against_others(PokemonType* , PokemonType*);
status DeleteEffective_against_me(PokemonType* , char*);
status DeleteEffective_against_others(PokemonType* , char*);
status PrintPokemon(Pokemon*);
status PrintPokemonType(PokemonType*);
Pokemon* CopyPokemon(Pokemon *pokemon);
status DestroyPokemon(Pokemon*);
int EqualPokemon(Pokemon*, Pokemon*);
char* getType(Pokemon*);
int getAttack(Pokemon*,Pokemon* ,int* ,int*);


