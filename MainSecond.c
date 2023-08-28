/*
 * MainSecond.c
 *
 *  Created on: Dec 21, 2019
 *      Author: ise
 */

#include <stdio.h>
#include "Defs.h"
#include "BattleByCategory.h"
#include "Pokemon.h"

// Remove the Type and the Type's information
static void RemoveType(PokemonType *type)
{
    free(type->effective_against_me);
	free(type->effective_against_others);
	free(type->TypeName);
	free(type);
}


// Casting to Pokemon*
static element CopyToPokemon(element E){
	Pokemon* NewPokemon = (Pokemon*)E;
	return CopyPokemon(NewPokemon);
}

// Casting to Pokemon*
static status FreeToPokemon(element E){
	Pokemon* NewPokemon = (Pokemon*)E;
	return DestroyPokemon(NewPokemon);
}

// Casting to Pokemon*. 1 - first bigger, 0 - same, -1 - second bigger
static int EqualToPokemon(element E1, element E2){
	Pokemon* NewPokemon = (Pokemon*)E1;
	Pokemon* PokemonCompare = (Pokemon*)E2;
	return EqualPokemon(NewPokemon, PokemonCompare);
}

// Casting to Pokemon*
static status PrintToPokemon(element E){
	Pokemon* NewPokemon = (Pokemon*)E;
	return PrintPokemon(NewPokemon);
}

// Casting to Pokemon*
static char* GetPokemonType(element E){
	Pokemon* NewPokemon = (Pokemon*)E;
	return getType(NewPokemon);
}

// Casting to Pokemon*. return the first attack - second attack. also produce the attack of each element
static int GetPokemonAttack(element E1, element E2 ,int* attack1 ,int* attack2){
	Pokemon* NewPokemon = (Pokemon*)E1;
	Pokemon* PokemonCompare = (Pokemon*)E2;
	return getAttack(NewPokemon,PokemonCompare ,attack1 ,attack2);
}




int main(int argc,char* argv[])
{

	  //Open the file for reading
	  char *line_buf = NULL;
	  size_t line_buf_size = 0;
	  ssize_t line_size;
	  FILE *fp = fopen(argv[3], "r");

	  if (!fp)
	  {
		  printf("Unable to open file");
		  		return 1;
	  }

	  //Get the first line of the file
	  line_size = getline(&line_buf, &line_buf_size, fp);


	  char *token;
	  char* types;
	  int sizeTypes = atoi(argv[1]); // The number of the types in the file
	  int Capacity = atoi(argv[2]); // The capacity in each type


	  PokemonType* pokemontype;
	  PokemonType** typesList = (PokemonType**)malloc(sizeof(PokemonType*)*sizeTypes); // Allocating new memory for the type's list
	  Pokemon* pokemon;
	  Battle battle;

	  int PokemonLineNum = 0; // Check if we are pass the "Pokemons" line

	  //Loop through until we are done with the file
	  for(int line_count = 1; line_count>=0 && line_size != -1 ; line_count++)
	  {
		  // break the loop if the size of types in the system is 0
		  if(sizeTypes == 0){
			 battle = NULL;
			 break;
		  }


		  // When we arrive to "Pokemons" line, we save a variable so we will know we passed through this line
		  if(strcmp(line_buf,"Pokemons\r\n") == 0)
		  {
			  PokemonLineNum = line_count;
			  // Get the next line
			  line_size = getline(&line_buf, &line_buf_size, fp);
			  if(Capacity == 0) // break the loop if the size of pokemons in the system is 0
				  break;
		  }

		  //Create all the Pokemons Type and save it in the system
		  if(line_count == 2)
		  {
			token = strtok(line_buf,"\r\n"); // The name of the Pokemons Types
			types = (char*)malloc(sizeof(char)*(strlen(token)+1)); // Allocating memory for the names of the types
			types = strcpy(types,token);
			// Creating the Battle
			battle = createBattleByCategory(Capacity,sizeTypes,types,EqualToPokemon,CopyToPokemon,FreeToPokemon,GetPokemonType,GetPokemonAttack,PrintToPokemon);
			//free(types);

			// Create types list
			int i = 0;
			token = strtok(line_buf,",\r\n");
			while(token!=NULL){
				PokemonType* pokemontype = CreatePokemonType(token);
				// Check if the allocation succeed, if not, free all the allocation done till here
				if(pokemontype == NULL){
					for(int j = 0 ; j <= i ; j++)
					{
						RemoveType(typesList[i]);

					}
					free(typesList);
			 		free(line_buf);
			 		line_buf = NULL;
			 		//Close the file now that we are done with it
			 		fclose(fp);
					return 1;
				}
				typesList[i] = pokemontype;
				i++;
				token = strtok(NULL,",");
		  }
		 }

		  PokemonType* A;
		  PokemonType* B;
		  status Status;

		  // Adding B to A's effective against me list / effective against others list
		  if(PokemonLineNum == 0 && line_count>2 )
		  {

			 token = strtok(line_buf,"\t"); // Delete the /t
			 token = strtok(token," "); // the A Pokemon Type
			 for (int i=0 ; i<sizeTypes ; i++)
			 {
				 if(strcmp(typesList[i]->TypeName,token) == 0)
			 	 {
					 A = typesList[i]; // A pointer to the type in the system
			 		 break;
			 	 }
			 }
			 token = strtok(NULL,":"); // Check if we need to add to other or me
			 if(strcmp(token,"effective-against-me") == 0)
			 {
				 token = strtok(NULL,",\r\n"); // the B Pokemon Type
				 while(token!=NULL)
				 {
					 for (int i=0 ; i<sizeTypes ; i++)
					 {
						 if(strcmp(typesList[i]->TypeName,token) == 0)
						 {
							 B = typesList[i]; // A pointer to the type in the system
				 			 break;
				 		 }
					 }
				 	Status = AddEffective_against_me(A,B);
				 	// Check if the allocation succeed, if not, free all the allocation done till here
				 	if(Status == NoMemory)
				 	{
				 		for(int i = 0 ; i < sizeTypes ; i++)
				 		{
				 			RemoveType(typesList[i]);
				 		}
				 		free(typesList);
				 		//Free the allocated line buffer
				 		free(line_buf);
				 		line_buf = NULL;
				 		//Close the file now that we are done with it
				 		fclose(fp);
				 		return 1;
				 	}
				 	token = strtok(NULL,",\r\n");
			     }

		      }
			 else if(strcmp(token,"effective-against-other") == 0)
			 {
				 token = strtok(NULL,",\r\n"); // the B Pokemon Type
				 while(token!=NULL)
				 {
					 for (int i=0 ; i<sizeTypes ; i++)
					 {
						 if(strcmp(typesList[i]->TypeName,token) == 0)
						 {
							 B = typesList[i]; // A pointer to the type in the system
				 			 break;
				 		 }
					 }
					Status = AddEffective_against_others(A,B);
				 	// Check if the allocation succeed, if not, free all the allocation done till here
				 	if(Status == NoMemory)
				 	{
				 		for(int i = 0 ; i < sizeTypes; i++)
				 		{
				 			RemoveType(typesList[i]);
				 		}
				 		free(typesList);
				 		//Free the allocated line buffer
				 		free(line_buf);
				 		line_buf = NULL;
				 		//Close the file now that we are done with it
				 		fclose(fp);
				 		return 1;
				 	}
				 	token = strtok(NULL,",\r\n");
			     }
			 }
		  }

		     // Create the Pokemons
			 char *PokemonName;
			 char *species;
			 float Height;
			 float Weight;
			 int Attack;
			 int bio = 1;

			 if(PokemonLineNum != 0 && line_count > 2)
			 {

				 if(line_buf  == NULL) // break the loop when we arrive to the end of file
					 break;

				 if(strcmp(line_buf,"Pokemons\r\n") == 0){ // break the loop if there are no Pokemons in the file
					 break;
				 }

				 token = strtok(line_buf,",\r\n"); // The Pokemon Name to create
				 PokemonName = token; // A pointer to the Pokemon name
				 bio++;
				 while(token!=NULL)
				 {
					 if(bio == 2)
					 {
						token = strtok(NULL,",\r\n"); // The Pokemon species
						species = token;
						bio++;
					 }
					 if(bio==3)
					 {
						 token = strtok(NULL,",\r\n"); // The Pokemon Height
						 Height = atof(token);
						 bio++;
					 }
					 if (bio==4)
					 {
						 token = strtok(NULL,",\r\n"); // The Pokemon Weight
						 Weight = atof(token);
						 bio++;
					 }
					 if (bio==5)
					 {
						 token = strtok(NULL,",\r\n"); // The Pokemon Attack
						 Attack = atoi(token);
						 bio++;
					 }
					 if(bio==6)
					 {
						 token = strtok(NULL,",\r\n"); // The Pokemon Type
						 for (int i=0 ; i<sizeTypes ; i++)
						 {
							if(strcmp(typesList[i]->TypeName,token) == 0)
							{
								pokemontype = typesList[i]; // A pointer to the type in the system
								break;
							}
						 }
						 bio++;
					 }
					 if(bio==7)
					 {
						 token = strtok(NULL,",\r\n");
						 pokemon = CreatePokemon(PokemonName,species,(float)Height,(float)Weight,Attack,pokemontype);
						 Status = insertObject(battle, pokemon);
						 // Check if the allocation succeed, if not, free all the allocation done till here
						 if(pokemon == NULL || Status == NotValidInput)
						 {
							for(int i = 0 ; i < sizeTypes ; i++)
							{
								RemoveType(typesList[i]);
							}
							free(typesList);
							destroyBattleByCategory(battle);

							//Free the allocated line buffer
							free(line_buf);
							line_buf = NULL;
							//Close the file now that we are done with it
							fclose(fp);
							return 1;
						 }
					 }
				 }
			 }
	      //Get the next line
	      line_size = getline(&line_buf, &line_buf_size, fp);
	  }
	  //Free the allocated line buffer
	  free(line_buf);
	  line_buf = NULL;



	  //Close the file now that we are done with it
	  fclose(fp);

	  char TypeName[300];
	  char PokemonName[300];
	  char Species[300];
	  float height;
	  float weight;
	  int attack;
	  bool Exist = false;
	  bool NoPokemonTypes = false;
	  bool FullCapacity = false;
	  Pokemon* PokemonToInsert;
	  Pokemon* PokemonToPrint;
	  Pokemon* ToRemove;
	  Pokemon* ToFight;
	  PokemonType* TypeToInsert;
	  status Status;
	  char ch = 49;
	  while(ch != 54 )
	  {
		  printf("Please choose one of the following numbers:\n");
		  printf("1 : Print all Pokemons by types\n");
		  printf("2 : Print all Pokemons types\n");
		  printf("3 : Insert Pokemon to battles training camp\n");
		  printf("4 : Remove stronger Pokemon by type\n");
		  printf("5 : Fight\n");
		  printf("6 : Exit\n");
		  scanf("%s",&ch);

		  switch(ch)
		  {
		  case(49): // Print all Pokemons by type
				  displayObjectsByCategories(battle);
		  	  	  break;
		  case(50):// Print all Pokemon's types
				  for(int i = 0 ; i<sizeTypes ; i++){
					  PrintPokemonType(typesList[i]);
				  }
		  	  	  break;
		  case(51):// Insert Pokemon to battles training camp
				  Exist = false; // label - The type doesn't exist
		  	  	  FullCapacity = false; // label - The type didn't arrive to full capacity
				  printf("Please enter Pokemon type name:\n");
		  	  	  scanf("%s",TypeName);
		  	  	  // Find the Pokemon's type in the system
		  	  	  for(int i = 0 ; i<sizeTypes ; i++){
		  	  		  if(strcmp(typesList[i]->TypeName,TypeName) == 0){
		  	  			  Exist = true; // label - The type exist
		  	  			  TypeToInsert = typesList[i];
		  	  			  if(getNumberOfObjectsInCategory(battle ,TypeToInsert->TypeName)>= Capacity){
		  	  				FullCapacity =true; // label - The type arrive to full capacity
		  	  				break;
		  	  			  }
		  	  		  }
		  	  	  }
		  	  	  if(Exist == false){
		  	  		  printf("Type name doesn't exist.\n");
		  	  		  break;
		  	  	  }

		  	  	  if(FullCapacity == true){
		  	  		  printf("Type at full capacity.\n");
		  	  		  break;
		  	  	  }


		  	  	printf("Please enter Pokemon name:\n");
		  	  	scanf("%s",PokemonName);
		  	  	printf("Please enter Pokemon species:\n");
		  	  	scanf("%s",Species);
		  	  	printf("Please enter Pokemon height:\n");
		  	  	scanf("%f",&height);
		  	  	printf("Please enter Pokemon weight:\n");
		  	  	scanf("%f",&weight);
		  	  	printf("Please enter Pokemon attack:\n");
		  	  	scanf("%d",&attack);
		  	  	// Create the Pokemon to insert
		  	  	PokemonToInsert = CreatePokemon(PokemonName,Species,height,weight,attack,TypeToInsert);
		  	  	// Create the Pokemon to print
		  	  	PokemonToPrint = CreatePokemon(PokemonName,Species,height,weight,attack,TypeToInsert);
		  	  	// insert the Pokemon to the system
		  	  	Status = insertObject(battle, PokemonToInsert);
		  	    // close the menu and remove all the data in the system
		  	  	if(PokemonToInsert == NULL || PokemonToPrint == NULL|| Status == NotValidInput){
					if(sizeTypes == 0){
						free(typesList);
						return 1;
					}
					Status = destroyBattleByCategory(battle);
			  	  	for(int i = 0 ; i < sizeTypes ; i++)
			  		{
			  			RemoveType(typesList[i]);

			  		}
			  		free(typesList);
			  		free(types);
					return 1;
		  	  	}

		  	  	// The insertion succeed
		  	  	if(Status == success){
		  	  		printf("The Pokemon was successfully added.\n");
		  	  		PrintPokemon(PokemonToPrint);
		  	  		DestroyPokemon(PokemonToPrint); // Remove the Pokemon to print
		  	  		break;
		  	  	}

		  	  	break;
		  case (52):// Remove stronger Pokemon by type
				  Exist = false; // label - The type doesn't exist
			      NoPokemonTypes = false; // label - There are Pokemon in the type
				  printf("Please enter type name:\n");
  	  	  	  	  scanf("%s",TypeName);
  	  	  	  	  // Find the Pokemon's type in the system
  	  	  	  	  for(int i = 0 ; i<sizeTypes ; i++){
  	  	  	  		  if(strcmp(typesList[i]->TypeName,TypeName) == 0){
  	  	  	  			  Exist = true;  // label - The type exist
  	  	  	  			  if(getNumberOfObjectsInCategory(battle,TypeName) == 0){
  	  	  	  				  NoPokemonTypes = true;// label - There are Pokemon in the type
  	  	  	  				  printf("There is no Pokemon to remove.\n");
  	  	  	  				  break;
  	  	  	  			  }
  	  	  	  			  if(NoPokemonTypes == false){
  	  	  	  				  // The Pokemon to remove
  	  	  	  				  ToRemove = removeMaxByCategory(battle,TypeName);
  	  	  	  				  if(ToRemove != NULL){
  	  	  	  					  printf("the strongest Pokemon was removed:\n");
  	  	  	  					  PrintPokemon(ToRemove);
  	  	  	  					  DestroyPokemon(ToRemove); // Remove the Pokemon
  	  	  	  					  break;
  	  	  	  				  }
  	  	  	  				  // if NULL close the menu and remove all the data in the system
  	  	  	  				  else{
  	  	  	  					  if(sizeTypes == 0){
  	  	  	  						  free(typesList);
  	  	  	  						  return 1;
  	  	  	  					  }
  	  	  	  					  Status = destroyBattleByCategory(battle);
  	  	  	  					  for(int i = 0 ; i < sizeTypes ; i++)
  	  	  	  					  {
  	  	  	  						  RemoveType(typesList[i]);
  	  	  	  					  }
  	  	  	  					  free(typesList);
  	  	  	  					  free(types);
  	  	  	  					  return 1;
  	  	  	  				 }
  	  	  	  			  }
  	  	  	  		  }
  	  	  	  	  }
  	  	  	  	  if(Exist == false){
  	  	  	  		  printf("Type name doesn't exist.\n");
  	  	  	  		  break;
  	  	  	  	  }
  	  	  	  	  break;
		  case(53):// Fight
				  Exist = false; // label - The type doesn't exist
				  printf("Please enter Pokemon type name:\n");
  	  	  	  	  scanf("%s",TypeName);
  	  	  	  	  // Find the Pokemon's type in the system
  	  	  	  	  for(int i = 0 ; i<sizeTypes ; i++){
  	  	  	  		  if(strcmp(typesList[i]->TypeName,TypeName) == 0){
  	  	  	  			  Exist = true; // label - The type exist
  	  	  	  			  TypeToInsert = typesList[i];

  	  	  	  		  }
  	  	  	  	  }
  	  	  	  	  if(Exist == false){
  	  	  	  		  printf("Type name doesn't exist.\n");
  	  	  	  		  break;
  	  	  	  	  }
  		  	  	printf("Please enter Pokemon name:\n");
  		  	  	scanf("%s",PokemonName);
  		  	  	printf("Please enter Pokemon species:\n");
  		  	  	scanf("%s",Species);
  		  	  	printf("Please enter Pokemon height:\n");
  		  	  	scanf("%f",&height);
  		  	  	printf("Please enter Pokemon weight:\n");
  		  	  	scanf("%f",&weight);
  		  	  	printf("Please enter Pokemon attack:\n");
  		  	  	scanf("%d",&attack);
  		  	  	// The Pokemon to Fight
  		  	  	ToFight = CreatePokemon(PokemonName,Species,height,weight,attack,TypeToInsert);
		  	  	if(ToFight == NULL){
					if(sizeTypes == 0){
						free(typesList);
						return 1;
					}
					Status = destroyBattleByCategory(battle);
			  	  	for(int i = 0 ; i < sizeTypes ; i++)
			  		{
			  			RemoveType(typesList[i]);

			  		}
			  		free(typesList);
			  		free(types);
					return 1;
		  	  	}
		  	  	printf("you choose to fight with:\n");
		  	  	PrintPokemon(ToFight);
		  	  	PokemonToInsert = fight(battle,ToFight);
		  	  	// if NULL close the menu and remove all the data in the system
		  	  	if(PokemonToInsert == NULL){
					if(sizeTypes == 0){
						free(typesList);
						return 1;
					}
					Status = destroyBattleByCategory(battle);
			  	  	for(int i = 0 ; i < sizeTypes ; i++)
			  		{
			  			RemoveType(typesList[i]);

			  		}
			  		free(typesList);
			  		free(types);
					return 1;
		  	  	}
		  	  	DestroyPokemon(ToFight);
		  	  	break;
		  case(54)://Exit
				if(sizeTypes == 0){
					free(typesList);
					printf("All the memory cleaned and the program is safely closed.\n");
					break;
				}
				Status = destroyBattleByCategory(battle);
		  	  	for(int i = 0 ; i < sizeTypes ; i++)
		  		{
		  			RemoveType(typesList[i]);

		  		}
		  		free(typesList);
		  		free(types);
				if(Status == success){
					printf("All the memory cleaned and the program is safely closed.\n");
				}
				break;
		  default:// not valid input
			  printf("Please choose a valid number.\n");

		  }

		}
	  return 0;
}
