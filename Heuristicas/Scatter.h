/*
 * Scatter.h
 *
 *  Created on: 12/04/2011
 *      Author: rsantana
 */

#ifndef SCATTER_H_
#define SCATTER_H_

#include "Routes.h"
#include "Input.h"
#include "Local_Search.h"
#include "Random.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ILS.h"
#include <algorithm>
#include <limits>

class Scatter {
private:

public:
	static double CalcRoutesDist(Routes* routes, Routes* x);
	static Routes** Diversification_Generation(int Psize,int b,Input* in, double y);
	static void Improvement_Method(int Psize, int max,Input* in, Routes** P);
	static bool Refset_Update(int Psize,int max, Routes** P, tuple** Pdistance, tuple** Pcosts, int b, Routes** Refset, tuple** RefsetVal, bool* new_sol);
	static void Cross_Routes(Input * in, Routes* rl ,Routes* x, Routes* routes);
	static void Cross_Routes_best(Input * in, Routes* rl ,Routes* x, Routes* routes);
	static void Combination_Method(Input * in, int Psize,int* max, Routes** P,int b, Routes** Refset, bool* new_sol);
	static void scatter(int Psize , int b, double y, Routes* Solution, Input* in);
};

#endif /* SCATTER_H_ */
