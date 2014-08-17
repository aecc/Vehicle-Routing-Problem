/*
 * Genetic.h
 *
 *  Created on: 23/04/2011
 *      Author: rsantana
 */

#include "Routes.h"
#include "Input.h"
#include "Perturbation.h"
#include "Local_Search.h"
#include "ILS.h"
#include "Random.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>


#ifndef GENETIC_H_
#define GENETIC_H_


class Genetic {
public:
	static void solution_to_array(Routes* solution, int* client);
	static void create_children(int ncust, int* cust, Routes* child);
	static void crossover(int ncust, int* p1, int* p2 , Routes* c1, Routes* c2);
	static void invert(int* p , Routes* child);
	static void mutation(int* p1 , int* p2 , Routes* child1, Routes* child2 , Input* in);
	static void genetic(int max_gen, int pop_size, Input* in, Routes* solution);
};



#endif /* GENETIC_H_ */
