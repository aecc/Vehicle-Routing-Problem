/*
 * GTS.h
 *
 *  Created on: 02/03/2011
 *      Author: aecc
 */

#ifndef GTS_H_
#define GTS_H_
#include "Input.h"
#include "Routes.h"
#include <math.h>
#include "Random.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Local_Search.h"

class GTS {
private:
	double lambda1_parameter;
	double lambda2_parameter;
	int mni_parameter;
	int tabu_tenure_parameter;
	Input * in;
	Routes * routes;
	bool ** reduced_neighbourhood;
	int ** times_arc_penalized;
	double ** real_costs;
	double ** avg;
	bool info;
public:
	GTS(Input *, double, double, int);
	static void runConstructionHeuristic(Routes *, double);
	Routes * runHeuristic(int);
	static bool allCustomersAsigned(bool *,int);
	static void calcReductionScheme(Routes *, bool **);
	static void recalcReductionScheme(Routes *, bool **, int);
	static void calcAvg(Routes *, double **);
	void penalizeGLS(int&,int&);
	void unpenalizeGLS(int&,int&);
	void enableShowInfo();
	void disableShowInfo();
	virtual ~GTS();
};

#endif /* GTS_H_ */
