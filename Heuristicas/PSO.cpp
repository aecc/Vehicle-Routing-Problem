/*
 * Particle Swarm Optimization
 *
 * PSO.cpp
 *
 *  Created on: 19/04/2011
 *      Author: aecc
 * TODO PSO Solo para instancias que tengan coordenadas
 */

#include "PSO.h"
using namespace std;
#include "ILS.h" //TODO


PSO::PSO(Input * input, int nroutes, int L, int p_acc_const, int g_acc_const, int l_acc_const, int n_acc_const, double ini_inertia, double fin_inertia, int K) {

	in = input;
	info = false;
	dimensions_matrix = input->positions;
	personal_best_acceleration_constant_parameter = p_acc_const;
	global_best_acceleration_constant_parameter = g_acc_const;
	local_best_acceleration_constant_parameter = l_acc_const;
	near_best_acceleration_constant_parameter = n_acc_const;
	initial_inertia_parameter = ini_inertia;
	final_inertia_parameter = fin_inertia;
	K_parameter = K;

	int min_x = numeric_limits<int>::max() -1;
	int max_x = numeric_limits<int>::min() +1;
	int min_y = numeric_limits<int>::max() -1;
	int max_y = numeric_limits<int>::min() +1;
	for (int i=0;i<in->ncustomers+1;i++) {
		if (dimensions_matrix[i].x < min_x)
			min_x = dimensions_matrix[i].x;
		if (dimensions_matrix[i].x > max_x)
			max_x = dimensions_matrix[i].x;
		if (dimensions_matrix[i].y < min_y)
			min_y = dimensions_matrix[i].y;
		if (dimensions_matrix[i].y > max_y)
			max_y = dimensions_matrix[i].y;
	}
	minimum_position = new Point(min_x,min_y);
	maximum_position = new Point(max_x,max_y);
	Particle::minimum_position = minimum_position;
	Particle::maximum_position = maximum_position;
	Particle::customers_dimensions_pointer = in->ncustomers+1;
	Particle::routes_dimensions_pointer =  nroutes*2;

	L_parameter = L;
	swarm = new Particle[L_parameter];
	for (int i=0; i<L_parameter;i++) {
		swarm[i].near_neighbor_best = new Particle();
	}

}

/*
 * Corre la heuristica con el numero de iteraciones dada
 */
Routes * PSO::runHeuristic(int n_iter) {

	double inertia = initial_inertia_parameter;
	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);

	Particle * global_best = new Particle();
	global_best->setPersonalBestPosition(numeric_limits<double>::max()-1.0);

	//Inicializamos el vector velocidad en cero y posiciones aleatorias
	for (int i=0; i<L_parameter; i++) {
		for (int j=0; j<Particle::customers_dimensions_pointer; j++) {
			double rnd = Random::genrand_real1();
			swarm[i].pos_customers_dimensions[j] = rnd;
			swarm[i].best_pos_customers_dimensions[j] = rnd;
			swarm[i].vel_customers_dimensions[j] = 0.0;
		}
		for (int j=0; j<Particle::routes_dimensions_pointer; j++) {
			int rnd;
			if (i%2==0) {
				rnd = Random::genrand_int32() % (maximum_position->x - minimum_position->x);
				rnd += minimum_position->x;
			} else {
				rnd = Random::genrand_int32() % (maximum_position->y - minimum_position->y);
				rnd += minimum_position->y;
			}
			swarm[i].pos_routes_dimensions[j] = rnd;
			swarm[i].best_pos_routes_dimensions[j] = rnd;
			swarm[i].vel_routes_dimensions[j] = 0;
		}
	}

	Routes * routes = new Routes(in);

	//Actualiza numero minimo de vehiculos
	updateRequiredVehicles(routes);

	int it = 0;
	while (it<n_iter) {

		int num_feasible = 0;

		if (1-n_iter==0)
			inertia = 1.0;
		else
			inertia = final_inertia_parameter + ((it-n_iter)/(1-n_iter)*(initial_inertia_parameter-final_inertia_parameter));

		for (int particle=0; particle<L_parameter; particle++) {

			Particle * current_particle = &swarm[particle];
			double fitness;
			if (!decode(in, routes,dimensions_matrix,current_particle)) {
				fitness = routes->getTotalCost() + penalization;
			} else {
				fitness = routes->getTotalCost();
				num_feasible++;
			}
			current_particle->setFitness(fitness);

			//Actualiza el personal best
			if (fitness + epsilon_parameter < current_particle->getPersonalBestPosition()) {
				current_particle->updateBestPosition(fitness);
			}

			//Actualiza el global best
			if (fitness + epsilon_parameter < global_best->getPersonalBestPosition()) {
				global_best->cloneDimensions(current_particle);
				global_best->setPersonalBestPosition(fitness);
			}

			//Actualiza el local best
			//TODO investigar que son los "vecinos"
			double local_min_fitness = numeric_limits<double>::max()-1.0;
			int local_min = -1;
			int neighbor = particle;
			for (int i=0; i<(K_parameter/2);i++) {
				neighbor--;
			}
			if (neighbor>L_parameter-K_parameter-1) {
				neighbor = L_parameter-K_parameter-2;
			}
			if (neighbor<0) {
				neighbor = 0;
			}
			for (int i=0; i<K_parameter+1;i++) {
				if (neighbor>L_parameter) break;
				if (neighbor==particle) {
					neighbor++;
					continue;
				}
				double this_fitness = swarm[neighbor].getFitness();
				if (this_fitness + epsilon_parameter < local_min_fitness) {
					local_min_fitness = this_fitness;
					local_min = neighbor;
				}
				neighbor++;
			}
			//TODO probar copiando vectores en vez de apuntadores
			if (local_min!=-1) {
				current_particle->setLocalBest(&swarm[local_min]);
			} else {
				current_particle->setLocalBest(&swarm[particle]);
			}

			//Actualiza near neighbor best
			double max_fdr = numeric_limits<double>::min()+1.0;
			int ptr = Particle::customers_dimensions_pointer;
			for (int j=0;j<ptr;j++) {
				double best = -1.0;
				for (int i=0; i<L_parameter; i++) {
					double fdr = current_particle->getFitness() - swarm[i].getFitness();
					double fdr_den = fabs(current_particle->pos_customers_dimensions[j]-swarm[i].best_pos_customers_dimensions[j]);
					if (fdr_den == 0.0) {
						fdr = numeric_limits<double>::min()+1.0;
					} else {
						fdr /= fdr_den;
					}

					if (fdr + epsilon_parameter > max_fdr) {
						max_fdr = fdr;
						best = swarm[i].best_pos_customers_dimensions[j];
					}
				}
				if (best!=-1.0) {
					current_particle->near_neighbor_best->pos_customers_dimensions[j] = best;
				} else {
					current_particle->near_neighbor_best->pos_customers_dimensions[j] = current_particle->pos_customers_dimensions[j];
				}
			}

			ptr = Particle::routes_dimensions_pointer;
			for (int j=0;j<ptr;j++) {
				double best = -1.0;
				for (int i=0; i<L_parameter; i++) {
					if (particle==i) continue;
					double fdr = current_particle->getFitness() - swarm[i].getFitness();
					double fdr_den = fabs(current_particle->pos_routes_dimensions[j]-swarm[i].best_pos_routes_dimensions[j]);
					if (fdr_den == 0) {
						fdr = numeric_limits<double>::min()+1.0;
					} else {
						fdr /= fdr_den;
					}

					if (fdr + epsilon_parameter > max_fdr) {
						max_fdr = fdr;
						best = swarm[i].best_pos_routes_dimensions[j];
					}
				}
				if (best!=-1.0) {
					current_particle->near_neighbor_best->pos_routes_dimensions[j] = best;
				} else {
					current_particle->near_neighbor_best->pos_routes_dimensions[j] = current_particle->pos_routes_dimensions[j];
				}
			}

			//Actualiza la velocidad y la posicion de la particula
			//TODO Probar factor comun
			for (int i=0; i<Particle::customers_dimensions_pointer;i++) {
				double u = Random::genrand_real1();
				current_particle->vel_customers_dimensions[i] = inertia*(current_particle->vel_customers_dimensions[i]) +
																personal_best_acceleration_constant_parameter*u*(current_particle->best_pos_customers_dimensions[i]-current_particle->pos_customers_dimensions[i]) +
																global_best_acceleration_constant_parameter*u*(global_best->pos_customers_dimensions[i]-current_particle->pos_customers_dimensions[i]) +
																local_best_acceleration_constant_parameter*u*(current_particle->getLocalBest()->pos_customers_dimensions[i]-current_particle->pos_customers_dimensions[i]) +
																near_best_acceleration_constant_parameter*u*(current_particle->near_neighbor_best->pos_customers_dimensions[i]-current_particle->pos_customers_dimensions[i]);
			}
			for (int i=0; i<Particle::routes_dimensions_pointer;i++) {
				double u = Random::genrand_real1();
				current_particle->vel_routes_dimensions[i] = inertia*(current_particle->vel_routes_dimensions[i]) +
															personal_best_acceleration_constant_parameter*u*(current_particle->best_pos_routes_dimensions[i]-current_particle->pos_routes_dimensions[i]) +
															global_best_acceleration_constant_parameter*u*(global_best->pos_routes_dimensions[i]-current_particle->pos_routes_dimensions[i]) +
															local_best_acceleration_constant_parameter*u*(current_particle->getLocalBest()->pos_routes_dimensions[i]-current_particle->pos_routes_dimensions[i]) +
															near_best_acceleration_constant_parameter*u*(current_particle->near_neighbor_best->pos_routes_dimensions[i]-current_particle->pos_routes_dimensions[i]);
			}

			for (int i=0; i<Particle::customers_dimensions_pointer;i++) {
				current_particle->pos_customers_dimensions[i] += current_particle->vel_customers_dimensions[i];
			}
			for (int i=0; i<Particle::routes_dimensions_pointer;i++) {
				current_particle->pos_routes_dimensions[i] += current_particle->vel_routes_dimensions[i];
				if (i%2==0) {
					if (current_particle->pos_routes_dimensions[i]<current_particle->minimum_position->x) {
						current_particle->pos_routes_dimensions[i] = current_particle->minimum_position->x;
						current_particle->vel_routes_dimensions[i] = 0;
					}
					if (current_particle->pos_routes_dimensions[i]>current_particle->maximum_position->x) {
						current_particle->pos_routes_dimensions[i] = current_particle->maximum_position->x;
						current_particle->vel_routes_dimensions[i] = 0;
					}
				} else {
					if (current_particle->pos_routes_dimensions[i]<current_particle->minimum_position->y) {
						current_particle->pos_routes_dimensions[i] = current_particle->minimum_position->y;
						current_particle->vel_routes_dimensions[i] = 0;
					}
					if (current_particle->pos_routes_dimensions[i]>current_particle->maximum_position->y) {
						current_particle->pos_routes_dimensions[i] = current_particle->maximum_position->y;
						current_particle->vel_routes_dimensions[i] = 0;
					}
				}
			}
		}

		if (info) {
			printf("Iteracion: %d. Total de soluciones factibles de %d: %d\n",it,L_parameter,num_feasible);
		}
		it++;
	}

	decode(in,routes,dimensions_matrix,global_best);
	ILS::vnd(in, routes);

	if (info) {
		routes->printAllRoutes();
	}

	delete global_best;
	//delete routes;

	return routes;
}

/*
 * Se encarga de decodificar una particula en una ruta
 * Devuelve false si la ruta decodificada es infactible (le faltan clientes)
 * TODO optimizar evitando NEW en el heap
 */
bool PSO::decode(Input * in, Routes * routes, Point * dimensions_matrix, Particle * particle) {

	bool feasible = true;
	routes->reset();
	routes->newRoute();
	int * customer_priority_list = new int[Particle::customers_dimensions_pointer];
	for (int i=0;i<Particle::customers_dimensions_pointer;i++)
		customer_priority_list[i] = i;

	Point * routes_list = new Point[Particle::routes_dimensions_pointer/2];


	double * pos_cust_dim = new double[Particle::customers_dimensions_pointer];
	for (int i=0;i<Particle::customers_dimensions_pointer;i++)
		pos_cust_dim[i] = particle->pos_customers_dimensions[i];

	Quicksort::runQuicksortRank(pos_cust_dim,customer_priority_list,0,Particle::customers_dimensions_pointer-1);

	int j=0;
	for (int i=0;i<Particle::routes_dimensions_pointer/2;i++) {
		routes_list[i] = Point(particle->pos_routes_dimensions[j],particle->pos_routes_dimensions[j+1]);
		j+=2;
	}

	double * priority_matrix[Particle::customers_dimensions_pointer];
	for (int i=0;i<Particle::customers_dimensions_pointer;i++) {
			priority_matrix[i] = new double[Particle::routes_dimensions_pointer/2];
	}

	for (int j=0;j<Particle::routes_dimensions_pointer/2;j++) {
		routes->last_route++;
		priority_matrix[0][j] = numeric_limits<double>::max()-1.0;
	}

	for (int i=1;i<Particle::customers_dimensions_pointer;i++) {
		for (int j=0;j<Particle::routes_dimensions_pointer/2;j++) {
			priority_matrix[i][j] = Point::calcDistance(dimensions_matrix[i],routes_list[j]);
		}
	}

	//Realiza la insercion de el mejor cliente en la mejor posicion de la mejor ruta de routes
	for (int i=0;i<Particle::customers_dimensions_pointer;i++) {

		int customer = customer_priority_list[i];
		if (customer==0) continue;

		double required_cost = numeric_limits<double>::min()+1.0;
		bool completed = false;
		while (!completed) {
			double min_cost = numeric_limits<double>::max()-1.0;
			int best_route = -1;
			for (int route=0;route<Particle::routes_dimensions_pointer/2;route++) {
				if (priority_matrix[customer][route] < min_cost && priority_matrix[customer][route]>required_cost) {
					min_cost = priority_matrix[customer][route];
					best_route = route;
				}
			}
			if (best_route==-1) {
				feasible = false;
				break;
			}
			//routes->printAllRoutes();
			int node = 0;
			int next;
			double diff_cost_min = numeric_limits<double>::max()-1.0;
			int best_node = -1;
			int best_next = -1;
			do {
				next = routes->getArc(best_route,node);
				double diff_cost =  - routes->costs[node][next] +
						   	   	   	  routes->costs[node][customer] +
						   	   	   	  routes->costs[customer][next];

				if (diff_cost < diff_cost_min) {
					routes->setCompleteArc(best_route,node,customer);
					routes->setCompleteArc(best_route,customer,next);
					routes->updateFullRouteCapacity(best_route);
					if (routes->checkFeasibility(best_route)) {
						diff_cost_min = diff_cost;
						best_node = node;
						best_next = next;
					}
					routes->setCompleteArc(best_route,node,next);
				}
				node = next;

			} while (node!=0);

			if (best_node != -1) {
				completed = true;
				routes->setCompleteArc(best_route,best_node,customer);
				routes->setCompleteArc(best_route,customer,best_next);
				routes->setRouteCost(best_route,routes->getRouteCost(best_route)+diff_cost_min);
			} else {
				required_cost = min_cost;
			}
			routes->updateFullRouteCapacity(best_route);
		}

	}

	if (feasible) {
		ILS::vnd(in, routes);
	}

	for (int i=0;i<Particle::customers_dimensions_pointer;i++)
		delete [] priority_matrix[i];
	delete [] pos_cust_dim;
	delete [] customer_priority_list;
	delete [] routes_list;
	return feasible;
}

/*
 * 	Busca el numero apropiado de vehiculos
 */
void PSO::updateRequiredVehicles(Routes * routes) {
	double fit;
	for (int i=0; i<L_parameter;i++) {
		if (!decode(in,routes,dimensions_matrix,&swarm[i])) {
			continue;
		} else {
			fit = routes->getTotalCost();
			bool vehicles_setted = false;
			while (!vehicles_setted) {
				Particle::routes_dimensions_pointer-=2;
				if (!decode(in,routes,dimensions_matrix,&swarm[i])) {
					vehicles_setted = true;
				} else {
					double new_fit = routes->getTotalCost();
					if (new_fit + epsilon_parameter>fit) {
						break;
					}
				}
			}
			Particle::routes_dimensions_pointer+=2;
		}

	}
}

/*
 * Muestra la informacion detallada en pantalla
 */
void PSO::enableShowInfo() {
	info = true;
}

/*
 * Deniega la informacion detallada en pantalla
 */
void PSO::disableShowInfo() {
	info = false;
}

PSO::~PSO() {
}
