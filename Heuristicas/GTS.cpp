/*
 * GTS.cpp
 *
 *  Created on: 02/03/2011
 *      Author: aecc
 *      TODO
 *      VND ls_crossover se queda pegado en mi maquina
 *      ls_interchange (interchange2) debe estar malo pues tarda demasiado en exceso y hace practicamente lo mismo que intechange1
 */

#include "GTS.h"
#include "ILS.h" //TODO
using namespace std;

GTS::GTS(Input * input, double lambda1, double lambda2, int tabu) {

	Routes * routs = new Routes(input);
	in = input;
	routes = routs;
	lambda1_parameter = lambda1;
	lambda2_parameter = lambda2;
	tabu_tenure_parameter = tabu;

	reduced_neighbourhood = new bool*[in->ncustomers+1];
	for (int i=0;i<in->ncustomers+1;i++)
		reduced_neighbourhood[i] = new bool[in->ncustomers+1];

	times_arc_penalized = new int*[in->ncustomers+1];
	for (int i=0;i<in->ncustomers+1;i++)
		times_arc_penalized[i] = new int[in->ncustomers+1];

	real_costs = new double*[in->ncustomers+1];
			for (int i=0;i<in->ncustomers+1;i++)
				real_costs[i] = new double[in->ncustomers+1];

	avg = new double*[in->ncustomers+1];
		for (int i=0;i<in->ncustomers+1;i++)
			avg[i] = new double[in->ncustomers+1];

	info = false;

}

/*
 * Calcula Avg, se trata de una matriz de costos promedios para utilizar en en el Guided Local Search como
 * parte de la funcion de penalizacion de arcos
 */
void GTS::calcAvg(Routes *routes, double ** avg) {

	for (int i=0; i<routes->ncustomers+1;i++) {
		for (int j=0; j<routes->ncustomers+1;j++) {
			if (i==j) {
				avg[i][j] = 0;
				continue;
			}
			double sum = 0.0;
			for (int k=0; k<routes->ncustomers+1;k++) {
				if (i!=k)
					sum += routes->costs[i][k];
				if (j!=k)
				sum += routes->costs[j][k];
			}
			double result = sum/(routes->ncustomers*2);
			avg[i][j] = result;
		}
	}

}

bool GTS::allCustomersAsigned(bool * assigned_customers, int size) {
	for (int i=1;i<size+1; i++)
		if (!assigned_customers[i])
			return false;
	return true;
}

/*
 * Corre la heuristica con el numero de iteraciones ingresadas
 */
Routes * GTS::runHeuristic(int n_iter) {

	calcReductionScheme(routes, reduced_neighbourhood);
	for (int i=0;i<in->ncustomers+1;i++)
		reduced_neighbourhood[i][i] = false;

	for (int i=0;i<in->ncustomers+1;i++)
		for (int j=0;j<in->ncustomers+1;j++)
			times_arc_penalized[i][j] = 0;

	for (int i=0;i<in->ncustomers+1;i++)
		for (int j=0;j<in->ncustomers+1;j++)
			real_costs[i][j] = routes->costs[i][j];

	calcAvg(routes,avg);

	mni_parameter = n_iter;
	int super_it = 0;
	int it = 0;
	int node_to_penalize, next_to_penalize;
	double min_cost = numeric_limits<double>::max() -1.0;
	Routes::createInitialRoutesGTS(routes,lambda1_parameter);
	Routes * best_routes = new Routes(in,in->ncustomers);
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);

	Tabu_Queue * queue = new Tabu_Queue(tabu_tenure_parameter);

	//routes->printAllRoutes();

	if (info) {
		printf("Costo inicial: %f\n\n",routes->getTotalCost());
	}

	while (it<mni_parameter) {

		penalizeGLS(node_to_penalize,next_to_penalize);

		bool better = false;
		//better = Local_Search::relocateGTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
		//better = Local_Search::swapGTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
		//better = Local_Search::interChange1GTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
		//better = Local_Search::interChange2GTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);

		double q_random = Random::genrand_real1();
		int case_it;
		if (q_random < 0.43) {
			case_it = 2;
		} else if (0.43<=q_random && q_random<0.86) {
			case_it = 3;
		} else if (0.86<=q_random && q_random<0.96) {
			case_it = 1;
		} else {
			case_it = 0;
		}

		while (!better) {
			switch (case_it % 5) {

			case 2:
				better = Local_Search::relocateGTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
				break;
			case 3:
				better = Local_Search::swapGTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
				break;
			case 1:
				better = Local_Search::interChange1GTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
				break;
			case 0:
				better = Local_Search::interChange2GTS(routes, in, reduced_neighbourhood,queue,min_cost,real_costs);
				break;
			case 4:

				double ** aux = in->cost;
				double ** aux2 = routes->costs;
				in->cost = real_costs;
				routes->costs = real_costs;
				ILS::vnd(in,routes);
				in->cost = aux;
				routes->costs = aux2;

				better = true;
			}
			case_it++;
		}


		if (it == n_iter -1) {
			double ** aux = in->cost;
			double ** aux2 = routes->costs;
			in->cost = real_costs;
			routes->costs = real_costs;
			ILS::vnd(in,routes);
			in->cost = aux;
			routes->costs = aux2;
		}

		//queue->print();
		if (routes->getTotalCost()  < min_cost) {
			min_cost = routes->getTotalCost();
			best_routes->clone(routes);
			/*
			char filename[50] = "rutas.txt";
			double ** aux = routes->costs;
			routes->costs = real_costs;
			routes->printAllRoutesToFile(filename);
			routes->costs = aux;
			*/

			it=0;
		} else {
			it++;
		}
		unpenalizeGLS(node_to_penalize,next_to_penalize);
		if (info) {
			printf("Costo: %f Iteracion: %d\n",routes->getTotalCost(),it);
		}
		//routes->printAllRoutes();
		//char name[256];
		//cin.getline (name,256);
		/*
		FILE * pFile;
		pFile = fopen ("plotSWAP2_cost.txt","a");
		fprintf(pFile,"%f\n",routes->getTotalCost());
		fclose (pFile);
		pFile = fopen ("plotSWAP2IT_cost.txt","a");
		fprintf(pFile,"%d\n",it);
		fclose (pFile);
		 */
		super_it++;


	}

	if (info) {
		best_routes->printAllRoutes();
	}

	//delete best_routes
	delete queue;

	return best_routes;
}

/*
 * Recalcula los nodos alcanzables desde el nodo seleccionado
 */
void GTS::recalcReductionScheme(Routes * routes, bool ** reduced_neighbourhood, int node) {
	double average = 0.0;
	for (int i=0;i<routes->ncustomers+1;i++) {
		average += routes->costs[node][i];
	}
	average = average/routes->ncustomers;

	for (int i=0;i<routes->ncustomers+1;i++) {
		if (routes->costs[node][i] < average) {
			reduced_neighbourhood[node][i] = true;
		} else {
			reduced_neighbourhood[node][i] = false;
		}
	}
}

/*
 * Calcula los nodos alcanzables desde todos los nodos utilizando el promedio de costo
 */
void GTS::calcReductionScheme(Routes * routes, bool ** reduced_neighbourhood) {
	double * average = new double[routes->ncustomers+1];
	for (int i=0;i<routes->ncustomers+1;i++) {
		double sum = 0.0;
		for (int j=0;j<routes->ncustomers+1;j++) {
			if (j==i) continue;
			sum += routes->costs[i][j];
		}
		average[i] = sum/routes->ncustomers;
	}

	for (int i=0;i<routes->ncustomers+1;i++) {
		for (int j=0;j<routes->ncustomers+1;j++) {
			if (j==i) continue;
			if (routes->costs[i][j] < average[i]) {
				reduced_neighbourhood[i][j] = true;
			} else {
				reduced_neighbourhood[i][j] = false;
			}
		}
	}

	delete [] average;
}

void GTS::unpenalizeGLS(int &node_best, int &next_best) {
	routes->costs[node_best][next_best] = real_costs[node_best][next_best];
	routes->costs[next_best][node_best] = real_costs[next_best][node_best];
	//recalcReductionScheme(routes,reduced_neighbourhood,node_best);
	//recalcReductionScheme(routes,reduced_neighbourhood,next_best);
}

/*
 * Guided Local Search encargado de penalizar el peor arco
 */
void GTS::penalizeGLS(int &node_best, int &next_best) {
	double max = 0.0;

	for(int route=0;route<routes->last_route;route++) {

		int node = 0;
		int next = routes->getArc(route,node);
		do {
			double result = (real_costs[node][next]/avg[node][next])/(1+times_arc_penalized[node][next]);
			if (result > max) {
				max = result;
				node_best = node;
				next_best = next;
			}
			node = next;
			next = routes->getArc(route,node);
		} while (node!=0);

	}

	//printf("Costo anterior arco %d->%d (%d): %f\n",node_best,next_best,times_arc_penalized[node_best][next_best],routes->costs[node_best][next_best]);
	//Penaliza el peor arco
	routes->costs[node_best][next_best] += times_arc_penalized[node_best][next_best]*lambda2_parameter*avg[node_best][next_best];
	routes->costs[next_best][node_best] += times_arc_penalized[next_best][node_best]*lambda2_parameter*avg[next_best][node_best];
	times_arc_penalized[node_best][next_best]++;
	times_arc_penalized[next_best][node_best]++;
	//printf("Costo despues arco %d->%d (%d): %f\n",node_best,next_best,times_arc_penalized[node_best][next_best],routes->costs[node_best][next_best]);

	//double ** aux = routes->costs;
	//routes->costs = real_costs;
	//recalcReductionScheme(routes,reduced_neighbourhood,node_best);
	//recalcReductionScheme(routes,reduced_neighbourhood,next_best);
	//routes->costs = aux;

}

/*
 * Muestra la informacion detallada en pantalla
 */
void GTS::enableShowInfo() {
	info = true;
}

/*
 * Deniega la informacion detallada en pantalla
 */
void GTS::disableShowInfo() {
	info = false;
}

GTS::~GTS() {

}


