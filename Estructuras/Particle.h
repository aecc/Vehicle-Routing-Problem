/*
 * Particle.h
 *
 *  Created on: 19/04/2011
 *      Author: aecc
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_
#include <math.h>
#include <limits>
#include "Point.h"

class Particle {
private:
	double best_pos;
	double fitness;
	Particle * local_best;
public:
	static int routes_dimensions_pointer;
	static int customers_dimensions_pointer;
	static Point * maximum_position;
	static Point * minimum_position;
	Particle * near_neighbor_best;
	int ncustomers;
	int nroutes;
	double * pos_customers_dimensions;
	int * pos_routes_dimensions;
	double * best_pos_customers_dimensions;
	int * best_pos_routes_dimensions;
	double * vel_customers_dimensions;
	int * vel_routes_dimensions;
	void setLocalBest(Particle*);
	void setNearNeighborBest(Particle*);
	Particle * getLocalBest();
	Particle * getNearNeighborBest();
	void updateBestPosition(double);
	double getPersonalBestPosition();
	void setPersonalBestPosition(double);
	void cloneDimensions(Particle*);
	double getFitness();
	void setFitness(double);
	Particle();
	virtual ~Particle();
};

#endif /* PARTICLE_H_ */
