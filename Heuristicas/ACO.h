/*
 * ACO.h
 *
 *  Created on: 08/02/2011
 *      Author: aecc
 */

#ifndef ACO_H_
#define ACO_H_
#include "Input.h"
#include "Local_Search.h"
#include "Routes.h"
#include <list>
#include <math.h>
#include "Quicksort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Random.h"

class ACO {
private:
	double alpha_parameter;
	double beta_parameter;
	double q_parameter;
	double ro_parameter;
	bool info;
public:
	double ** pheromones;
	double ** visibility;
	bool * visited;
	Input * in;
	Routes * runHeuristic(int);
	ACO(Input *,double=1.0,double=3.0,double=0.8,double=0.015);
	double calcInitialPheromone();
	double calcVisibility(int,int);
	double calcAttractiveness(int,int);
	bool isAllVisited();
	void resetVisited();
	void enableShowInfo();
	void disableShowInfo();
	virtual ~ACO();
	static const double epsilon_parameter = 0.001;
};

#endif /* ACO_H_ */
