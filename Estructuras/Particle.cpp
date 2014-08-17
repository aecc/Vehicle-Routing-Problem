/*
 * Representacion de una particula utilizada en PSO
 *
 * Particle.cpp
 *
 *  Created on: 19/04/2011
 *      Author: aecc
 */

#include "Particle.h"
using namespace std;

int Particle::customers_dimensions_pointer;
int Particle::routes_dimensions_pointer;
Point * Particle::minimum_position;
Point * Particle::maximum_position;

Particle::Particle() {

	Particle::pos_customers_dimensions = new double[customers_dimensions_pointer];
	pos_routes_dimensions = new int[routes_dimensions_pointer];
	best_pos_customers_dimensions = new double[customers_dimensions_pointer];
	best_pos_routes_dimensions = new int[routes_dimensions_pointer];
	vel_customers_dimensions = new double[customers_dimensions_pointer];
	vel_routes_dimensions = new int[routes_dimensions_pointer];
	best_pos = numeric_limits<double>::max() -1.0;
	fitness = numeric_limits<double>::max() -1.0;

}
/*
 * Actualiza la mejor posicion encontrada con la posicion actual
 */
void Particle::updateBestPosition(double new_best_fitness) {

	for (int i=0; i<customers_dimensions_pointer; i++) {
		best_pos_customers_dimensions[i] = pos_customers_dimensions[i];
	}
	for (int i=0; i<routes_dimensions_pointer; i++) {
		best_pos_routes_dimensions[i] = pos_routes_dimensions[i];
	}
	best_pos = new_best_fitness;

}

/*
 * Retorna el fitness de la mejor posicion
 */
double Particle::getPersonalBestPosition() {
	return best_pos;
}

/*
 * Establece el fitness de la mejor posicion
 */
void Particle::setPersonalBestPosition(double new_best) {
	best_pos = new_best;
}

/*
 * Clona el contenido de una particula en esta
 */
void Particle::cloneDimensions(Particle * particle) {
	for (int i=0; i<Particle::customers_dimensions_pointer; i++) {
		pos_customers_dimensions[i] = particle->pos_customers_dimensions[i];
	}
	for (int i=0; i<Particle::routes_dimensions_pointer; i++) {
		pos_routes_dimensions[i] = particle->pos_routes_dimensions[i];
	}
	//setPersonalBestPosition(particle->best_pos);
}

/*
 * Establece el fitness de esa posicion
 */
void Particle::setFitness(double new_fitness) {
	fitness = new_fitness;
}

/*
 * Obtiene el fitness de la posicion
 */
double Particle::getFitness() {
	return fitness;
}

/*
 * Establece el Local Best copiando apuntadores
 */
void Particle::setLocalBest(Particle * new_local_best) {
	local_best = new_local_best;
}

/*
 * Establece el Near Neigbor Best copiando apuntadores
 */
void Particle::setNearNeighborBest(Particle * new_near_neighbor_best) {
	near_neighbor_best = new_near_neighbor_best;
}
/*
 * Obtiene el Local Best
 */
Particle * Particle::getLocalBest() {
	return local_best;
}

/*
 * Obtiene el Near Neigbor Best
 */
Particle * Particle::getNearNeighborBest() {
	return near_neighbor_best;
}

Particle::~Particle() {
	delete [] pos_customers_dimensions;
	delete [] pos_routes_dimensions;
	delete [] best_pos_customers_dimensions;
	delete [] best_pos_routes_dimensions;
	delete [] vel_customers_dimensions;
	delete [] vel_routes_dimensions;

}
