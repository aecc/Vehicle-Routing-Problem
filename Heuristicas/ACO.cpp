/*
 * Ant.cpp
 *
 *  Created on: 08/02/2011
 *      Author: aecc
 *
 */

#include "ILS.h" //TODO
#include "ACO.h"
#include <cmath>
using namespace std;

ACO::ACO(Input * input, double alpha, double beta, double q, double ro) {

	alpha_parameter = alpha;
	beta_parameter = beta;
	q_parameter = q;
	ro_parameter = ro;
	info = false;

	in = input;
    pheromones = new double*[in->ncustomers+1];
                for (int i=0;i<in->ncustomers+1;i++)
                        pheromones[i] = new double[in->ncustomers+1];

	visibility = new double*[in->ncustomers+1];
				for (int i=0;i<in->ncustomers+1;i++)
					visibility[i] = new double[in->ncustomers+1];

    visited = new bool[in->ncustomers+1];

}

double ACO::calcVisibility(int i, int j) {
	if (in->cost[i][j] == 0.0) return 0.0;
	double vis = (in->cost[i][0] + in->cost[0][j] - in->cost[i][j]) / in->cost[i][j];
	if (isnan(vis)) return 0.0;
	return vis;
}

void ACO::resetVisited() {
	for (int i=0; i<in->ncustomers+1; i++)
		visited[i] = false;
}

bool ACO::isAllVisited() {
	for (int i=0; i<in->ncustomers+1; i++)
		if (!visited[i]) return false;
	return true;
}

double ACO::calcAttractiveness(int i, int j) {
	double att = (pow(pheromones[i][j],alpha_parameter)*pow(visibility[i][j],beta_parameter));
	return att;
}

Routes * ACO::runHeuristic(int niter) {

	for (int i=0; i<in->ncustomers+1; i++)
		for (int j=0; j<in->ncustomers+1; j++)
			visibility[i][j] = calcVisibility(i,j);

    double initial_pheromone = calcInitialPheromone();

    for (int i=0; i<in->ncustomers+1; i++)
        for (int j=0; j<in->ncustomers+1; j++)
        	pheromones[i][j] = initial_pheromone;

	Routes * routes = new Routes(in,in->ncustomers);
	Routes * best_routes = new Routes(in,in->ncustomers);
	int * feasible_list = new int[in->ncustomers+1];
	list<int> ** arcs_in_ants = new list<int>*[in->ncustomers+1];
    for (int i=0;i<in->ncustomers+1;i++)
    	arcs_in_ants[i] = new list<int>[in->ncustomers+1];
	double * ants_costs = new double[in->ncustomers+1];
	double * ants_costs_aux = new double[in->ncustomers+1];
	int * ants = new int[in->ncustomers+1];
	int * ants2 = new int[in->ncustomers+1];
	ants_costs[0] = 1000000.0;
	bool last_iter = false;
	double best_solution = 10000000.0;
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);

	for (int iter=0;iter<niter;iter++) {

		if (iter+1>=niter) {
			last_iter = true;
		}
		//Vacia las listas de la matriz de arcos usados
		for (int k=0;k<in->ncustomers+1;k++)
			for (int l=0;l<in->ncustomers+1;l++)
				arcs_in_ants[k][l].clear();

		for (int i=1;i<in->ncustomers+1;i++)
			ants[i] = i;

		for (int ant=1; ant<in->ncustomers+1;ant++) {

			int i_best;
			double min_cost;
			int last_route = 0;
			int last_i;
			bool first = true;

			resetVisited();
			routes->reset();
			visited[0] = true;
			routes->newRoute();

			last_i = ant;
			routes->setArc(last_route,0,ant);
			routes->setArc(last_route,ant,0);
			routes->setReverseArc(last_route,0,ant);
			routes->setReverseArc(last_route,ant,0);
			routes->updateFullRouteCapacity(last_route);
			visited[ant] = true;

			//Se comienza a construir todas las rutas desde el principio
			while (true) {

				if (first) {
					last_i = ant;
					first = false;
				} else {
					last_i = 0;
				}

				//Se construye una nueva ruta
				do {

					int n_feasibles = 0;
					double max_attractiveness = -1.0;
					double attractiveness_sum = 0.0;
					bool choice_most_favorable = false;
					i_best = -1;
					min_cost = 1000000.0;

					double q_random;
					q_random = Random::genrand_res53();

					if (q_random<q_parameter) {
						choice_most_favorable = true;
					}

					for (int i=1; i<routes->ncustomers+1; i++) {
						if (i==last_i) continue;
						if (!visited[i]) {

							routes->setArc(last_route,last_i,i);
							routes->setArc(last_route,i,0);
							routes->setReverseArc(last_route,i,last_i);
							routes->setReverseArc(last_route,0,i);
							routes->updateFullRouteCapacity(last_route);
							if (routes->checkFeasibility(last_route)) {
								feasible_list[n_feasibles++] = i;
								double attractiveness = calcAttractiveness(last_i,i);
								attractiveness_sum += attractiveness;
								if (attractiveness>max_attractiveness) {
									max_attractiveness = attractiveness;
									i_best = i;
								}
							}
							routes->setArc(last_route,last_i,0);
							routes->setReverseArc(last_route,0,last_i);
						}
					}

					//routes->printRouteDetails(last_route);
					//Se escoge el cliente con mayor atractividad
					if (choice_most_favorable) {

						if (i_best == -1) break;

					} else {

						if (i_best == -1) break;

						q_random = Random::genrand_res53();

						double rank = 0.0;
						for (int k=0; k<n_feasibles ; k++) {
							if (attractiveness_sum==0.0) break;
							double prob = calcAttractiveness(last_i,feasible_list[k])/attractiveness_sum;
							if (rank<q_random && q_random<rank+prob) {
								i_best = feasible_list[k];
								break;
							}
							rank += prob;
						}

						if (i_best == -1) break;

					}

					routes->setArc(last_route,last_i,i_best);
					routes->setReverseArc(last_route,i_best,last_i);
					routes->setArc(last_route,i_best,0);
					routes->setReverseArc(last_route,0,i_best);
					//routes->printRouteDetails(last_route);
					visited[i_best] = true;
					last_i = i_best;


				} while (true);

				routes->updateFullRouteCapacity(last_route);

				routes->newRoute();
				last_route++;

				if (!isAllVisited())
					continue;

				break;

			}

			for (int i=0;i<routes->last_route;i++) {
					routes->updateFullRouteCapacity(i);
					double route_cost = 0.0;
					int j = routes->getArc(i,0);
					arcs_in_ants[0][j].push_back(ant);
					route_cost += routes->costs[0][j];
					int next = routes->getArc(i,j);
					while (next!=0) {
						arcs_in_ants[j][next].push_back(ant);
						route_cost += routes->costs[j][next];
						j = next;
						next = routes->getArc(i,j);
					}
					arcs_in_ants[j][0].push_back(ant);
					route_cost += routes->costs[j][0];
					routes->setRouteCost(i,route_cost);
			}

			/*
			 * Realiza los Local Search
			 */

			ILS::vnd(in, routes);
/*
			for (int k=0; k<routes->last_route; k++)
				while (Local_Search::relocateIntraRoute(k,routes));

			for (int k=0; k<routes->last_route; k++)
				while (Local_Search::swapIntraRoute(k,routes));

			int res = 1;
			while (res==1) {
				//cout << "Costo: " << routes->getTotalCost() << endl;
				res = Local_Search::ls_shift_one(1,routes,in);
			}

			res = 1;
			while (res==1) {
				//cout << "Costo: " << routes->getTotalCost() << endl;
				res = Local_Search::ls_swap_one(1,routes,in);
			}
			*/

			ants_costs[ant] = routes->getTotalCost();
			if (ants_costs[ant]<best_solution) {
				best_solution = ants_costs[ant];
				best_routes->clone(routes);
			}

			if (info) {
				cout << "Iteracion: " << iter << " Ant: " << ant << " Costo: " << routes->getTotalCost() << endl;
			}

		}

		//Guarda la mejor solucion
		for (int k=1;k<in->ncustomers+1;k++) {
			ants_costs_aux[k] = ants_costs[k];
		}

		Quicksort::runQuicksortCosts(ants, ants_costs_aux, 1, in->ncustomers);
		for (int k=1;k<in->ncustomers+1;k++)
			ants2[k] = k;
		Quicksort::runQuicksortRank(ants, ants2, 1, in->ncustomers);

		/*
		 * Actualiza feromonas
		 */
		for (int k=0;k<in->ncustomers+1;k++){
			for (int l=0;l<in->ncustomers+1;l++) {
				double increment = 0.0;
				list<int>::iterator it;
				for ( it=arcs_in_ants[k][l].begin() ; it != arcs_in_ants[k][l].end(); it++ ) {
					int ant_in_arc = *it;
					increment += (in->ncustomers - ants2[ant_in_arc] + 1)/ants_costs[ant_in_arc];
				}
				double pheromone = pheromones[k][l];
				pheromone = (1-ro_parameter)*pheromone + increment;
				pheromones[k][l] = pheromone;

			}
		}

	}

	if (info) {
		best_routes->printAllRoutes();
	}

	//delete routes;
	delete [] feasible_list;
	delete [] arcs_in_ants;
	delete [] ants_costs;
	delete [] ants_costs_aux;
	delete [] ants;
	delete [] ants2;

	return best_routes;

}

/*
 * Calcula la cantidad inicial de feromonas
 */
double ACO::calcInitialPheromone() {
	Routes * routes = new Routes(in,in->ncustomers);
	Routes::createInitialRoutesNearestNeighbor(routes);
	if (info) {
		printf("Costo inicial: %f\n\n",routes->getTotalCost());
	}
	double cost = routes->getTotalCost();
	delete routes;
	double p0 = 1/(in->ncustomers*cost);
	return p0;
}

/*
 * Muestra la informacion detallada en pantalla
 */
void ACO::enableShowInfo() {
	info = true;
}

/*
 * Deniega la informacion detallada en pantalla
 */
void ACO::disableShowInfo() {
	info = false;
}

ACO::~ACO() {
}
