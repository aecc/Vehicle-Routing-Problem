/*
 * Local_Search.h
 *
 *  Created on: 06/02/2011
 *      Author: rsantana
 */
#include "Routes.h"
#include "Input.h"
#include <stdlib.h>
#include <queue>
#include "Tabu_Queue.h"
#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

class Local_Search {
public:
	static int ls_shift_one(int nIter, Routes* solution, Input* in);
	static int ls_shift_two(int nIter, Routes* solution, Input* in);
	static int ls_swap_one(int nIter, Routes* solution, Input* in);
	static int ls_swap_two_one(int nIter, Routes* solution, Input* in);
	static int ls_swap_two_two(int nIter, Routes* solution, Input* in);
	static int ls_crossover(int nIter, Routes* solution, Input* in);
	static int ls_interchange(int nIter, Routes* solution, Input* in);
	static bool ls_or_opt_one(int, Routes*);
	static bool ls_or_opt_two(int, Routes*);
	static bool ls_or_opt_three(int, Routes*);
	static bool ls_two_opt(int, Routes*);
	static bool ls_reverse(int, Routes*);
	static bool swapIntraRoute(int, Routes*);
	static bool swapGTS(Routes*, Input *,bool**,Tabu_Queue*, double, double**);
	static bool relocateGTS(Routes*, Input * in, bool**, Tabu_Queue*, double, double**);
	static bool interChange1GTS(Routes*, Input *, bool**, Tabu_Queue*, double, double**);
	static bool interChange2GTS(Routes*, Input*, bool**, Tabu_Queue*, double, double**);
	static bool relocateIntraRoute(int, Routes*);
	static bool interChange1IntraRoute(int, Routes*);
	static int interChange1InterRoute(int, Routes*, Input*);
	static int interChange2InterRoute(int, Routes*, Input*);
	static const double epsilon_parameter = 0.001;
	static double diff_parameter; 		//Si empeora
	static double diff_neg_parameter;	//Si mejora
};

#endif /* LOCAL_SEARCH_H_ */
