/*
 * Particle Swarm Optimization
 *
 * PSO.h
 *
 *  Created on: 19/04/2011
 *      Author: aecc
 */

#ifndef PSO_H_
#define PSO_H_
#include "Input.h"
#include "Routes.h"
#include "Particle.h"
#include "Quicksort.h"
#include "Random.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include "Local_Search.h"

class PSO {
private:
	Input * in;
	int L_parameter;
	static const double penalization = 10000.0;
	int personal_best_acceleration_constant_parameter;
	int global_best_acceleration_constant_parameter;
	int local_best_acceleration_constant_parameter;
	int near_best_acceleration_constant_parameter;
	double initial_inertia_parameter;
	double final_inertia_parameter;
	int K_parameter;
	bool info;
public:
	Point * dimensions_matrix;
	Point * maximum_position;
	Point * minimum_position;
	Particle * swarm;
	Routes * runHeuristic(int);
	PSO(Input*, int, int, int, int, int, int, double, double, int);
	void updateRequiredVehicles(Routes*);
	static bool decode(Input*, Routes*,Point*,Particle*);
	void enableShowInfo();
	void disableShowInfo();
	virtual ~PSO();
	static const double epsilon_parameter = 0.001;

};

#endif /* PSO_H_ */
