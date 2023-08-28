/*
 * Defs.h
 *
 *  Created on: Nov 27, 2019
 *      Author: ise
 */

#ifndef DEFS_H_
#define DEFS_H_

typedef enum e_bool{false,true}bool;
typedef enum e_ststus{success,failure,NoMemory,NotValidInput}status;

typedef void* element;
typedef element (*copyFunction)(element);
typedef status (*freeFunction)(element);
typedef int (*equalFunction)(element,element);//1 - first bigger, 0 - same, -1 - second bigger
typedef status (*printFunction)(element);
typedef char* (*getCategoryFunction)(element);
typedef int (*getAttackFunction)(element,element,int*,int*);//return the first attack - second attack. also produce the attack of each element


#endif /* DEFS_H_ */
