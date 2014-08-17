/*
 * ILS.h
 *
 *  Created on: 13/03/2011
 *      Author: rsantana
 */
#include "Routes.h"
#include "Input.h"
#include "Perturbation.h"
#include "Local_Search.h"
#include "Random.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef ILS_H_
#define ILS_H_

class ILS {

public:
	static void vnd_intra(Routes* solution);
	static int vnd(Input* in, Routes* solution);
	static int vnd_fast(Input* in, Routes* solution);
	static void ils(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double y);
	static void ils_GRASP(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double y);
	static void ils_best(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double y);
};

#endif /* ILS_H_ */
