/*
 * Routes.cpp
 *
 *  Created on: 19/01/2011
 *      Author: aecc
 */

#include "Routes.h"
#include "Operators.h"
#include "Input.h"
#include "Local_Search.h"
#include "IO.h"
#include <iostream>
#include <stdio.h>
#include "Random.h"

using namespace std;

/* Constructor que necesita un apuntador a un arreglo de requerimientos al camion de cada cliente: Pickups - Deliveries
 * ncustom representa el numero de clientes
 * nrout representa el numero de vehiculos, si se deja en blanco se calculan usando calcNRoutes()
 */
Routes::Routes(Input * input, int nrout /* = 0*/) {
	costs = input->cost;
	nroutes = nrout;
	ncustomers = input->ncustomers;
	if (nrout==0)
		nroutes = calcNRoutes();
	customers = new int[ncustomers];
	customers_reverse = new int[ncustomers];
	capacity_pickup = new double[ncustomers];
	capacity_delivery = new double[ncustomers];
	vehicle_capacity = input->capacity;
	demands_pickup = input->pickups;
	demands_delivery = input->deliveries;
	capacity_at_depot_pickup = new double[nroutes];
	capacity_at_depot_delivery = new double[nroutes];
	routes = new int[nroutes];
	routes_costs = new double[nroutes];
	routes_reverse = new int[nroutes];
	last_route = 0;

	for (int i=0;i<nroutes;i++) {
		capacity_at_depot_pickup[i] = 0.0;
		routes[i] = -1;
		routes_reverse[i] = -1;
		routes_costs[i] = 0.0;
	}

	for (int i=0;i<ncustomers;i++) {
		customers[i] = -1;
		customers_reverse[i] = -1;
	}

}

/*
 * Clona la ruta de entrada en el objeto
 */
void Routes::clone(Routes * routes_old) {
	costs = routes_old->costs;
	nroutes = routes_old->nroutes;
	ncustomers = routes_old->ncustomers;
	nroutes = ncustomers;
	vehicle_capacity = routes_old->vehicle_capacity;
	demands_pickup = routes_old->demands_pickup;
	demands_delivery = routes_old->demands_delivery;
	last_route = routes_old->last_route;
	for (int i=0;i<nroutes;i++) {
		capacity_at_depot_pickup[i] = routes_old->capacity_at_depot_pickup[i];
		capacity_at_depot_delivery[i] = routes_old->capacity_at_depot_delivery[i];
		routes[i] = routes_old->routes[i];
		routes_reverse[i] = routes_old->routes_reverse[i];
		routes_costs[i] = routes_old->routes_costs[i];
	}

	for (int i=0;i<ncustomers;i++) {
		capacity_pickup[i] = routes_old->capacity_pickup[i];
		capacity_delivery[i] = routes_old->capacity_delivery[i];
		customers[i] = routes_old->customers[i];
		customers_reverse[i] = routes_old->customers_reverse[i];
	}

}

/*
 * Imprime los detalles de todas las rutas
 */
void Routes::printAllRoutes() {
	bool all_visited = true;
	bool visited[ncustomers+1];
	for (int i=0; i<ncustomers+1;i++) {
		visited[i] = false;
	}
	visited[0] = true;
	for (int i=0; i<last_route; i++) {
		int node = getArc(i,0);
		do {
			visited[node] = true;
			node = getArc(i,node);
		} while (node!=0);
		printRouteDetails(i);
	}
	for (int i=0; i<ncustomers+1;i++) {
		if (!visited[i]) {
			all_visited = false;
			break;
		}
	}
	printf("Todos visitados: %d\n",all_visited);
	printf("Costo total: %f\n\n",getTotalCost());
}

/*
 * Imprime los detalles de todas las rutas
 */
void Routes::printAllRoutesToFile(char * filename) {
	for (int i=0; i<last_route; i++)
		printRouteDetailsToFile(i,filename);
}

/*
 * Invierte el orden de todos los arcos entre nodos from y to.
 * Si se coloca from=0 y to=0 se invierte toda la ruta, igualmente si se dejan vacios
 * OJO: no altera los arcos seguidos y anterior a estos, hay que alterarlos manualmente
 * NO SE ACTUALIZAN CAPACIDADES
 */
void Routes::invertArcs(int route, int from/*=0*/, int to/*=0*/) {

	int i = from;
	do {
		int next = getArc(route,i);
		setReverseArc(route,i,next);
		i = next;
	} while (i!=to);
	i = from;
	do {
		int next = getReverseArc(route,i);
		setArc(route,next,i);
		i = next;
	} while (i!=to);
}

//Calcular el numero de rutas o vehiculos necesarios
int Routes::calcNRoutes() {
	return ncustomers;
}

/* Comprueba si las rutas ya fueron creadas en su totalidad */
bool Routes::isFull() {
	return nroutes==last_route+1;
}

/* Agrega un arco a la ruta del vehiculo SOLO PARA CONSTRUIR UNA RUTA NUEVA SEGURA */
void Routes::addFeasibleArc(int route, int ini, int end) {
	if (routes[route]==-1) {
		routes[route] = ini-1;
		routes_reverse[route] = end-1;
		customers[ini-1] = end-1;
		customers_reverse[end-1] = ini-1;
		capacity_pickup[ini-1] = demands_pickup[ini];
		capacity_delivery[ini-1] = demands_delivery[end];
		capacity_pickup[end-1] = capacity_pickup[ini-1] + demands_pickup[end];
		capacity_delivery[end-1] = 0;
		routes_costs[route] += costs[0][ini];
		capacity_at_depot_pickup[route] = capacity_pickup[end-1];
		capacity_at_depot_delivery[route] = capacity_delivery[ini-1];
	} else {
		routes_reverse[route] = end-1;
		customers[ini-1] = end-1;
		customers_reverse[end-1] = ini-1;
		capacity_pickup[end-1] = capacity_pickup[ini-1] + demands_pickup[end];
		capacity_at_depot_pickup[route] = capacity_pickup[end-1];

		//capacity_delivery[ini-1] = capacity_delivery[ini-1] + demands_delivery[end];
		capacity_delivery[end-1] = 0;
		if (demands_delivery[end]==0)
			return;

		int j = routes_reverse[route];
		j = customers_reverse[j];
		while (customers_reverse[j]!=-1) {
			capacity_delivery[j] += demands_delivery[end];
			j = customers_reverse[j];
		}
		capacity_delivery[j] += demands_delivery[end];
		capacity_at_depot_delivery[route] = capacity_delivery[j] + demands_delivery[j+1];
	}
	routes_costs[route] += costs[ini][end];
}

/*
 * Establece un nuevo arco totalmente incluyendo su reverso
 */
void Routes::setCompleteArc(int route, int from, int to) {

	setArc(route,from,to);
	setReverseArc(route,to,from);

}

/*
 * Checkea la factibilidad de una ruta desde el cliente 'from' al cliente 'to',
 * Si el cliente 'from' esta antes que 'to' en el orden de recorrido de la ruta sorted deberia ser True,
 * en caso contrartio False
 * OJO: Solo para cambios INTRA-RUTA
 */
bool Routes::checkFeasibility(int route, bool sorted /*= true*/, int from /*= 0*/, int to /*= 0*/) {

	if (from==0 || to==0) {
		from = getArc(route,0);
		to = getReverseArc(route,0);
		sorted = true;
		if((from==0)&&(to==0)){
			return true;
		}
	}


	int midd;
	int prev;

    if (sorted) {
        midd = to-1;
        prev = customers[midd];
    } else {
        int aux = from;
        midd = from-1;
        from = to;
        to = aux;
        prev = customers[midd];
    }

    //printf("prev:%d midd:%d\n",prev,midd);

	if (midd == from-1) return true; //TODO no estoy seguro

	if (getDeliveryCapacity(0,route) > vehicle_capacity)
		return false;
	if (getPickupCapacity(0,route) > vehicle_capacity)
		return false;

	double delivery_capacity = 0.0;
	double pickup_capacity = getPickupCapacity(prev+1,route);
	while (true) {

		delivery_capacity += demands_delivery[prev+1];
		pickup_capacity -= demands_pickup[prev+1];
		if (delivery_capacity + pickup_capacity > vehicle_capacity)
			return false;

		if (midd == from-1) break;
		prev = midd;
		midd = customers_reverse[midd];
		//printf("prev:%d midd:%d\n",prev,midd);

	}

	return true;

}


/*
 * Actualiza las capacidades de la ruta de los clientes desde from hasta to.
 * Se debe chequear factibilidad antes
 * Si el cliente 'from' esta antes que 'to' en el orden de recorrido de la ruta sorted deberia ser True,
 * en caso contrartio False
 * OJO: Si se realizo un cambio INTER-RUTA se debe hacer update de las capacidades iniciales y finales del vehiculo
 * antes de utilizar esta funcion
 */
bool Routes::updateRouteCapacity(int route, bool sorted /*= true*/, int from /*= 0*/, int to /*= 0*/) {

	if (from==0 || to==0) {
		from = getArc(route,0);
		to = getReverseArc(route,0);
	}

	int midd;
	int prev;

    if (sorted) {
        midd = to-1;
        prev = customers[midd];
    } else {
        int aux = from;
        midd = from-1;
        from = to;
        to = aux;
        prev = customers[midd];
    }

	//if (midd == from-1) return false; TODO no estoy seguro

	double delivery_capacity = 0.0;
	double pickup_capacity = getPickupCapacity(prev+1,route);
	while (true) {

		delivery_capacity += demands_delivery[prev+1];
		pickup_capacity -= demands_pickup[prev+1];
		setDeliveryCapacity(route,midd+1,delivery_capacity);
		setPickupCapacity(route,midd+1,pickup_capacity);

		if (midd == from-1) break;
		prev = midd;
		midd = customers_reverse[midd];

	}

    return true;

}

/*
 * Se debe chequear factibilidad primero
 * Actualiza las capacidades de la ruta de los clientes de una ruta.
 */
bool Routes::updateFullRouteCapacity(int route, double new_initial_capacity/*=-1.0*/, double new_final_capacity/*=-1.0*/) {

	if (new_initial_capacity==-1.0) {
		new_initial_capacity=0.0;
		new_final_capacity=0.0;
		int j = routes_reverse[route];
		if (j==-1) {
			setDeliveryCapacity(route,0,0.0);
			setPickupCapacity(route,0,0.0);
			return true;
		}
		while (customers_reverse[j]!=-1) {
			new_final_capacity += demands_pickup[j+1];
			new_initial_capacity += demands_delivery[j+1];
			j = customers_reverse[j];
		}
		new_final_capacity += demands_pickup[j+1];
		new_initial_capacity += demands_delivery[j+1];
	}
	setDeliveryCapacity(route,0,new_initial_capacity);
	setPickupCapacity(route,0,new_final_capacity);

	int from = getArc(route,0);
	int to = getReverseArc(route,0);
	int midd;
	int prev;

    midd = to-1;
    prev = customers[midd];

	if ((midd == -1)&& ((from-1)==-1)){
		setDeliveryCapacity(route,0,0.0);
		setPickupCapacity(route,0,0.0);
		return true;
	}

	double delivery_capacity = 0.0;
	double pickup_capacity = getPickupCapacity(prev+1,route);
	while (true) {

		delivery_capacity += demands_delivery[prev+1];
		pickup_capacity -= demands_pickup[prev+1];
		setDeliveryCapacity(route,midd+1,delivery_capacity);
		setPickupCapacity(route,midd+1,pickup_capacity);

		if (midd == from-1) break;
		prev = midd;
		midd = customers_reverse[midd];

	}

    return true;

}

void Routes::newRoute() {
	if (isFull())
		return;
	if (routes[0]==-1) {
		return;
	}
	last_route++;

}

int Routes::getFirstCustomer(int route) {
	return routes[route]+1;
}

/* Obtiene el siguiente cliente a visitar luego del ingresado como parametro */
int Routes::getArc(int route, int ini) {
	if(ini==0){
		return routes[route]+1;

	}else{
		return customers[ini-1]+1;
	}
}

int Routes::getReverseArc(int route, int ini) {
	if(ini==0){
		return routes_reverse[route]+1;
	}else{
		return customers_reverse[ini-1]+1;
	}
}
void Routes::setArc(int route,int customer, int value) {
	if (customer==0) {
		routes[route] = value-1;
	} else {
		customers[customer-1] = value-1;
	}
}

void Routes::setReverseArc(int route, int customer, int value) {
	if (customer==0) {
		routes_reverse[route] = value-1;
	} else {
		customers_reverse[customer-1] = value-1;
	}
}

/* Elimina todas las rutas para crea nuevas */
void Routes::reset(int nrout /*= 0*/) {
	if (nrout!=0) {
		delete [] routes;
		routes = new int[nrout];
		nroutes = nrout;
	}
	last_route = 0;
	for (int i=0;i<nroutes;i++) {
		this->capacity_at_depot_delivery[i]=0.0;
		capacity_at_depot_pickup[i] = 0.0;
		routes[i] = -1;
		routes_reverse[i] = -1;
		routes_costs[i] = 0.0;
	}

	for (int i=0;i<ncustomers;i++) {
		this->capacity_delivery[i]=0.0;
		this->capacity_pickup[i]=0.0;
		customers[i] = -1;
		customers_reverse[i] = -1;
	}
}

void Routes::setDeliveryCapacity(int route, int customer, double value) {
	if (customer==0){
		capacity_at_depot_delivery[route] = value;
	} else {
		capacity_delivery[customer-1] = value;
	}
}

void Routes::setPickupCapacity(int route, int customer, double value) {
	if (customer==0){
		capacity_at_depot_pickup[route] = value;
	} else {
		capacity_pickup[customer-1] = value;
	}
}

/*
 * customer 0: obtiene la capacidad de DELIVERY en este punto
 */
double Routes::getDeliveryCapacity(int customer, int route) {
	if (customer==0) {
		return capacity_at_depot_delivery[route];
	} else
		return capacity_delivery[customer-1];
}

/*
 * customer 0: obtiene la capacidad de Pickup en este punto
 */
double Routes::getPickupCapacity(int customer, int route) {
	if (customer==0) {
		return capacity_at_depot_pickup[route];
	} else
		return capacity_pickup[customer-1];
}

/*
 * Calcula el costo de una ruta desde el cliente start hasta el cliente end
 */
double Routes::calcTotalRouteCost(int route, int start /*= 0*/, int end /*= 0*/) {
	int k = start;
	int next = this ->getArc(route,k);
	double cost = 0.0;
	while(true){
		//cout << "next: " << next << "\n";
		cost += costs[k][next];
		if(next == end){
			break;
		}
		if(next == 0){
			//cout << "mal/n";
			return -1;
		}
		k = next;
		next = this->getArc(route,next);
	}
	return cost;
}

double Routes::calcTotalCost(){
	int i;
	double final_cost = 0.0;
	for(i = 0; i < this->last_route;i++){
		final_cost += this->calcTotalRouteCost(i);
	}
	return final_cost;
}

void Routes::calcAccCostFrom(double ** matrix_costs, double* acc_cost){
	int i, k, n;
	int last;
	n= this->last_route;
	acc_cost[0]=0.0;
	for(i=0;i<n;i++){
		last = 0;
		k= this->getFirstCustomer(i);
		while(true){
			if(k==0){
				break;
			}
			acc_cost[k]= acc_cost[last]+matrix_costs[last][k];
			last = k;
			k = this->getArc(i,k);
		}
	}

}

void Routes::calcAccCost(double* acc_cost){
	int i, k, n;
	int last;
	n= this->last_route;
	acc_cost[0]=0.0;
	for(i=0;i<n;i++){
		last = 0;
		k= this->getFirstCustomer(i);
		while(true){
			if(k==0){
				break;
			}
			acc_cost[k]= acc_cost[last]+this->costs[last][k];
			last = k;
			k = this->getArc(i,k);
		}
	}

}

/*
 * Imprime todos los detalles de la ruta
 */
void Routes::printRouteDetails(int route) {

	printf("Ruta %d: 0->",route);
	int next = routes[route];

	if (next==-1) {
		printf("0\n");
		return;
	}

	do {
		printf("%d->",next+1);
		next = customers[next];
		if(next == customers[next]){
			return;
		}
	} while (next!=-1);

	printf("0\n");
	printf("Ruta inversa %d: 0->",route);
	next = routes_reverse[route];

	do {
		printf("%d->",next+1);
		next = customers_reverse[next];
		if(next == customers_reverse[next]){
			return;
		}
	} while (next!=-1);

	printf("0\n");
	printf("Capacidad maxima del camion: %f\n",vehicle_capacity);
	printf("El camion comienza con: %f\n",capacity_at_depot_delivery[route]);
	printf("El camion termina con: %f\n",capacity_at_depot_pickup[route]);
	printf("El camion tiene en todo momento: ");
		next = routes[route];

		printf("%f->",getDeliveryCapacity(0,route));

		do {
			printf("%f->",getPickupCapacity(next+1,route)+getDeliveryCapacity(next+1,route));
			next = customers[next];

		} while (next!=-1);
		printf("X\n");

	printf("Capacidad de pickup: ");

	next = routes[route];

	printf("0.0->");

	do {
		printf("%f->",getPickupCapacity(next+1,route));
		next = customers[next];

	} while (next!=-1);
	printf("X\n");
	printf("Capacidad de delivery: ");

	next = routes[route];

	printf("%f->",getDeliveryCapacity(0,route));

	do {
		printf("%f->",getDeliveryCapacity(next+1,route));
		next = customers[next];

	} while (next!=-1);
	printf("X\n");

	double c1 = routes_costs[route];
	double c2 = calcTotalRouteCost(route);
	printf("Costo: %f\n",c1);
	printf("Costo Calculado: %f\n",c2);
	bool feasible = checkFeasibility(route);
	if (!feasible) {
		printf("RUTA NO FACTIBLE\n\n");
		exit(1);
	}
	printf("Factible antes: %d\n",feasible);
	updateFullRouteCapacity(route); //TODO
	feasible = checkFeasibility(route);
	if (!feasible) {
		printf("RUTA NO FACTIBLE\n\n");
		exit(1);
	}
	printf("Factible despues: %d\n\n",feasible);
	//if (c1!=c2) exit(1);

}

/*
 * Imprime todos los detalles de la ruta
 */
void Routes::printRouteDetailsToFile(int route, char * filename) {

	FILE * pFile;
	pFile = fopen (filename,"a");

	fprintf(pFile,"Ruta %d: 0->",route);
	int next = routes[route];

	if (next==-1) return;

	do {
		fprintf(pFile,"%d->",next+1);
		next = customers[next];
		if(next == customers[next]){
			return;
		}
	} while (next!=-1);

	fprintf(pFile,"0\n");
	fprintf(pFile,"Ruta inversa %d: 0->",route);
	next = routes_reverse[route];

	do {
		fprintf(pFile,"%d->",next+1);
		next = customers_reverse[next];
		if(next == customers_reverse[next]){
			return;
		}
	} while (next!=-1);

	fprintf(pFile,"0\n");
	fprintf(pFile,"Capacidad maxima del camion: %f\n",vehicle_capacity);
	fprintf(pFile,"El camion comienza con: %f\n",capacity_at_depot_delivery[route]);
	fprintf(pFile,"El camion termina con: %f\n",capacity_at_depot_pickup[route]);
	fprintf(pFile,"El camion tiene en todo momento: ");
		next = routes[route];

		fprintf(pFile,"%f->",getDeliveryCapacity(0,route));

		do {
			fprintf(pFile,"%f->",getPickupCapacity(next+1,route)+getDeliveryCapacity(next+1,route));
			next = customers[next];

		} while (next!=-1);
		fprintf(pFile,"X\n");

	fprintf(pFile,"Capacidad de pickup: ");

	next = routes[route];

	fprintf(pFile,"0.0->");

	do {
		fprintf(pFile,"%f->",getPickupCapacity(next+1,route));
		next = customers[next];

	} while (next!=-1);
	fprintf(pFile,"X\n");
	fprintf(pFile,"Capacidad de delivery: ");

	next = routes[route];

	fprintf(pFile,"%f->",getDeliveryCapacity(0,route));

	do {
		fprintf(pFile,"%f->",getDeliveryCapacity(next+1,route));
		next = customers[next];

	} while (next!=-1);
	fprintf(pFile,"X\n");

	double c1 = routes_costs[route];
	double c2 = calcTotalRouteCost(route);
	fprintf(pFile,"Costo: %f\n",c1);
	fprintf(pFile,"Costo Calculado: %f\n",c2);
	fprintf(pFile,"Factible: %d\n\n",checkFeasibility(route));
	//if (c1!=c2) exit(1);
	fclose (pFile);

}

/*
 * Obtiene el costo previamente calculado de la ruta
 */
double Routes::getRouteCost(int route) {
	return routes_costs[route];
}

/*
 * Calcula el costo total de la solucion
 */
double Routes::getTotalCost() {
	double total = 0.0;
	for (int i=0;i<last_route;i++){
		total += getRouteCost(i);
	}
	return total;
}

/*
 * Establece un nuevo costo constante para la ruta especificada
 */
void Routes::setRouteCost(int route, double new_cost) {
	routes_costs[route] = new_cost;
}

/*
 * Crea una solucion inicial mediante la heuristica del vecino mas cercano
 * TODO optimizar
 */
void Routes::createInitialRoutesNearestNeighbor(Routes * routes) {

	int i_best;
	double min_cost;
	int last_route = 0;
	int last_i;
	bool * visited = new bool[routes->ncustomers+1];
	for (int i=0; i<routes->ncustomers+1;i++)
		visited[i] = false;
	visited[0] = true;
	routes->newRoute();

	while (true) {
		last_i = 0;
		do {
			i_best = -1;
			min_cost = 1000000.0;

			for (int i=1; i<routes->ncustomers+1; i++) {
				if (i==last_i) continue;
				if (!visited[i]) {
					if (routes->costs[last_i][i] < min_cost) {
						routes->setArc(last_route,last_i,i);
						routes->setArc(last_route,i,0);
						routes->setReverseArc(last_route,i,last_i);
						routes->setReverseArc(last_route,0,i);
						routes->updateFullRouteCapacity(last_route);
						if (routes->checkFeasibility(last_route)) {
							min_cost = routes->costs[last_i][i];
							i_best = i;
						}
						routes->setArc(last_route,last_i,0);
						routes->setReverseArc(last_route,0,last_i);
					}
				}
			}
			if (i_best==-1)
				break;
			routes->setArc(last_route,last_i,i_best);
			routes->setReverseArc(last_route,i_best,last_i);
			routes->setArc(last_route,i_best,0);
			routes->setReverseArc(last_route,0,i_best);
			routes->updateFullRouteCapacity(last_route);
			visited[i_best] = true;
			last_i = i_best;

		} while (true);

		routes->newRoute();
		last_route++;
		bool all_serviced = true;
		for (int i=1; i<routes->ncustomers+1;i++) {
			if (!visited[i]) {
				all_serviced = false;
				break;
			}
		}
		if (!all_serviced)
			continue;

		break;
	}

	for (int i=0;i<routes->last_route;i++) {
	    	routes->updateFullRouteCapacity(i);
	    	double route_cost = 0.0;
	    	int j = routes->getArc(i,0);
	    	route_cost += routes->costs[0][j];
	    	int next = routes->getArc(i,j);
			while (next!=0) {
				route_cost += routes->costs[j][next];
				j = next;
				next = routes->getArc(i,j);
			}
			route_cost += routes->costs[j][0];
			routes->setRouteCost(i,route_cost);
	}



	delete [] visited;
}

/*
 * Crea una solucion inicial mediante la heuristica de insersión
 */
Routes* Routes::createInitialRoutesInsertion(Input *in, int v, double y) {
	Routes* routes = new Routes(in,in->ncustomers);
	int i_best, route_best;
	double min_cost, diff_cost;
	int last;
	bool * visited = new bool[routes->ncustomers+1];
	bool all_serviced = false;
	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);
	int rand,i,j,k;
	int limit=0;
	for (i=0; i<routes->ncustomers+1;i++)
		visited[i] = false;
	visited[0] = true;
	routes->last_route = 0;
	for(i=0;i<v;i++){
		routes->newRoute();
		while(true){
			rand = Random::genrand_int31();
			rand = (rand%routes->ncustomers)+1;
			if(!visited[rand]){
				routes->setArc(i,0,rand);
				routes->setReverseArc(i,rand,0);
				routes->setArc(i,rand,0);
				routes->setReverseArc(i,0,rand);
				visited[rand]=true;

				break;
			}
		}
	}

	routes->last_route = v;
	for(i=0;i<routes->last_route;i++){
//		cout << "randi: " << routes ->getRouteCost(i) << "\n";
		routes->setRouteCost(i,routes ->calcTotalRouteCost(i));
		routes->updateFullRouteCapacity(i);
//		cout << "randf: " << routes ->getRouteCost(i) << "\n";
	}

	while(true){

		for(i=1;i<routes->ncustomers+1;i++){
			if(visited[i]){
				continue;
			}
			i_best=-1;
			route_best=0;
			min_cost = numeric_limits<double>::max( );
			for(j=0;j<routes->last_route;j++){
				k=routes->getFirstCustomer(j);
				last=0;
				while(true){
					diff_cost = routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k] - (y*(routes->getRouteCost(j)+routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k]));
					if(diff_cost + 0.001 < min_cost){
						routes->setArc(j,last,i);
						routes->setReverseArc(j,i,last);
						routes->setArc(j,i,k);
						routes->setReverseArc(j,k,i);
						routes->updateFullRouteCapacity(j);
						if(routes->checkFeasibility(j)){
							min_cost=diff_cost;
							i_best=k;
							route_best=j;
						}
						routes->setArc(j,last,k);
						routes->setReverseArc(j,k,last);
						routes->updateFullRouteCapacity(j);
					}
					if(k==0){
						break;
					}
					last=k;
					k=routes->getArc(j,k);
				}
			}
			if(i_best!=-1){
				last = routes->getReverseArc(route_best,i_best);
				min_cost = routes->costs[last][i] + routes->costs[i][i_best] - routes->costs[last][i_best];
				routes->setArc(route_best,last,i);
				routes->setReverseArc(route_best,i,last);
				routes->setArc(route_best,i,i_best);
				routes->setReverseArc(route_best,i_best,i);
				routes->updateFullRouteCapacity(route_best);
				routes->setRouteCost(route_best,(routes->getRouteCost(route_best)+min_cost));
				visited[i]=true;
//				routes->printRouteDetails(route_best);
				continue;
			}

		}
		all_serviced=true;
		for(i=1;i<routes->ncustomers+1;i++){
			if(!visited[i]){
				all_serviced=false;
				break;
			}
		}

		if(all_serviced){

			break;
		}else{
			if(limit>3){
				all_serviced=false;
				//cout<<"la cago mal \n";
				limit=0;
				delete routes;
				Routes* routes = new Routes(in,in->ncustomers);
				for (i=0; i<routes->ncustomers+1;i++)
					visited[i] = false;
				visited[0] = true;
				routes->last_route = 0;
				for(i=0;i<v;i++){
					routes->newRoute();
					while(true){
						rand = Random::genrand_int31();
						rand = (rand%routes->ncustomers)+1;
						if(!visited[rand]){
							routes->setArc(i,0,rand);
							routes->setReverseArc(i,rand,0);
							routes->setArc(i,rand,0);
							routes->setReverseArc(i,0,rand);
							visited[rand]=true;

							break;
						}
					}
				}

				routes->last_route = v;
				for(i=0;i<routes->last_route;i++){
			//		cout << "randi: " << routes ->getRouteCost(i) << "\n";
					routes->setRouteCost(i,routes ->calcTotalRouteCost(i));
					routes->updateFullRouteCapacity(i);
			//		cout << "randf: " << routes ->getRouteCost(i) << "\n";
				}
			}
		}
		limit++;
	}

	delete [] visited;
	return routes;
}

/*
 * Crea una solucion inicial mediante la heuristica de insersión
 */
Routes* Routes::createInitialRoutesInsertionBest(Input *in, int v, double y) {
	Routes* routes = new Routes(in,in->ncustomers);
	int i_best, route_best;
	double min_cost, diff_cost;
	int last, i_min;
	bool * visited = new bool[routes->ncustomers+1];
	bool all_serviced = false;
	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);
	int rand,i,j,k;
	int t;
	int limit=0;
	for (i=0; i<routes->ncustomers+1;i++){
		visited[i] = false;
	}
	visited[0] = true;

	routes->last_route = 0;
	for(i=0;i<v;i++){
		routes->newRoute();
		while(true){
			rand = Random::genrand_int31();
			rand = (rand%routes->ncustomers)+1;
			if(!visited[rand]){
				routes->setArc(i,0,rand);
				routes->setReverseArc(i,rand,0);
				routes->setArc(i,rand,0);
				routes->setReverseArc(i,0,rand);
				visited[rand]=true;

				break;
			}
		}
	}

	routes->last_route = v;
	for(i=0;i<routes->last_route;i++){
//		cout << "randi: " << routes ->getRouteCost(i) << "\n";
		routes->setRouteCost(i,routes ->calcTotalRouteCost(i));
		routes->updateFullRouteCapacity(i);
//		cout << "randf: " << routes ->getRouteCost(i) << "\n";
	}

	while(true){
		for(t=1;t<routes->nroutes+1;t++){
			i_min = -1;
			i_best=-1;
			route_best=0;
			min_cost = numeric_limits<double>::max( );
			for(i=1;i<routes->ncustomers+1;i++){
				if(visited[i]){
					continue;
				}
				for(j=0;j<routes->last_route;j++){
					k=routes->getFirstCustomer(j);
					last=0;
					while(true){
					    //diff_cost = routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k] - (y*(routes->getRouteCost(j)+routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k]));
						diff_cost = routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k] - (y*(routes->costs[0][k] + routes->costs[k][0]));
						if(diff_cost + 0.001 < min_cost){
		//					routes->printRouteDetails(j);
							routes->setArc(j,last,i);
							routes->setReverseArc(j,i,last);
							routes->setArc(j,i,k);
							routes->setReverseArc(j,k,i);
							routes->updateFullRouteCapacity(j);
							if(routes->checkFeasibility(j)){
								min_cost=diff_cost;
								i_best=k;
								route_best=j;
								i_min=i;
							}
							routes->setArc(j,last,k);
							routes->setReverseArc(j,k,last);
							routes->updateFullRouteCapacity(j);
						}
						if(k==0){
							break;
						}
						last=k;
						k=routes->getArc(j,k);
					}
				}

			}

			if(i_best!=-1){
				i = i_min;
				last = routes->getReverseArc(route_best,i_best);
				min_cost = routes->costs[last][i] + routes->costs[i][i_best] - routes->costs[last][i_best];
				routes->setArc(route_best,last,i);
				routes->setReverseArc(route_best,i,last);
				routes->setArc(route_best,i,i_best);
				routes->setReverseArc(route_best,i_best,i);
				routes->updateFullRouteCapacity(route_best);
				routes->setRouteCost(route_best,(routes->getRouteCost(route_best)+min_cost));
				visited[i]=true;
				//				routes->printRouteDetails(route_best);
				continue;
			}
		}
		all_serviced=true;
		for(i=1;i<routes->ncustomers+1;i++){
			if(!visited[i]){
				all_serviced=false;
				break;
			}
		}

		if(all_serviced){

			break;
		}else{

			all_serviced=false;
			//cout<<"la cago mal \n";
			limit=0;
			delete routes;
			Routes* routes = new Routes(in,in->ncustomers);
			for (i=0; i<routes->ncustomers+1;i++){
				visited[i] = false;
			}
			visited[0] = true;
			routes->last_route = 0;
			for(i=0;i<v;i++){
				routes->newRoute();
				while(true){
					rand = Random::genrand_int31();
					rand = (rand%routes->ncustomers)+1;
					if(!visited[rand]){
						routes->setArc(i,0,rand);
						routes->setReverseArc(i,rand,0);
						routes->setArc(i,rand,0);
						routes->setReverseArc(i,0,rand);
						visited[rand]=true;

						break;
					}
				}

				routes->last_route = v;
				for(i=0;i<routes->last_route;i++){
					//		cout << "randi: " << routes ->getRouteCost(i) << "\n";
					routes->setRouteCost(i,routes ->calcTotalRouteCost(i));
					routes->updateFullRouteCapacity(i);
					//		cout << "randf: " << routes ->getRouteCost(i) << "\n";
				}
			}
		}
	}

	delete [] visited;
	return routes;
}


bool sort_tuples(tuple* i, tuple* j){
	if((i->cost) < (j->cost)){
		return true;
	}else if((i->cost) == (j->cost)){
		return (i->id > j->id);
	}
	return false;

};

bool sort_tuples2(tuple* i, tuple* j){
	if((i->cost) < (j->cost)){
		return true;
	}else if((i->cost) == (j->cost)){
		return (i->id < j->id);
	}
	return false;

};

Routes* Routes::createInitialRoutesGRASP(Input *in, double y, int ri){

	Routes* routes = new Routes(in,in->ncustomers);
	int i_best, last_route;
	double min_cost;
	int last;
	bool * visited = new bool[routes->ncustomers+1];
	bool all_serviced = false;
	unsigned long init[1]={time(NULL)+ri}, length=1;
	Random::init_by_array(init, length);
	int i,k, count;
	int j;
	int n = routes->ncustomers;

	tuple **client = new tuple*[routes->ncustomers+1];
	for(i=0;i<=n;i++){
		client[i] = new tuple();
	}
	for (i=1; i<routes->ncustomers+1;i++){
		visited[i] = false;
	}
	visited[0] = true;

	last_route = 1;
	k=0;
	routes->newRoute();
	routes->setCompleteArc(k,0,0);
	while(true){
		count=0;
		last = routes->getReverseArc(k,0);
		for(i=1;i<=n;i++){
			if(visited[i]){
				continue;
			}

			routes->setCompleteArc(k,last,i);
			routes->setCompleteArc(k,i,0);
			routes->updateFullRouteCapacity(k);
			if(routes->checkFeasibility(k)){
				client[count]->id= i;
				client[count]->cost = in->cost[last][i]+in->cost[i][0];
				count++;
			}
			routes->setCompleteArc(k,last,0);
			routes->updateFullRouteCapacity(k);

		}
		if(count==0){
			k++;
			last_route++;
			routes->newRoute();
			routes->setCompleteArc(k,0,0);
		}else{

			sort(client,client+count, sort_tuples);

/*
			for(i=0;i<count;i++){
				cout << "id: " << client[i]->id << " cost: " << client[i] ->cost << endl;
			}
*/

			j=1;
			min_cost = client[0]->cost + y*(client[count-1]->cost-client[0]->cost);
			while(true){
				if(j==count){
					break;
				}
				if(client[j-1]->cost - 0.001 > min_cost){
					break;
				}
				j++;
			}
			i_best = Random::genrand_int31() % j;
			//cout << "id: " << client[i_best]->id << " cost: " << client[i_best] ->cost << endl;
			visited[client[i_best]->id] = true;
			//routes->printRouteDetails(k);

			routes ->setCompleteArc(k,last,client[i_best]->id);
			routes ->setCompleteArc(k,client[i_best]->id,0);

			//routes->printRouteDetails(k);

			routes->updateFullRouteCapacity(k);
			routes->setRouteCost(k,routes->getRouteCost(k)+client[i_best]->cost - in ->cost[last][0]);

		}


		all_serviced=true;
		for(i=1;i<routes->ncustomers+1;i++){
			if(!visited[i]){
				all_serviced=false;
				break;
			}
		}
		if(all_serviced){

			break;
		}

	}
	for(i=0;i<=n;i++){
		delete client[i];
	}

	delete [] client;
	delete [] visited;
	routes->last_route = last_route;
	return routes;
};

/*
 * Crea las rutas inciales para GTS
 */
void Routes::createInitialRoutesGTS(Routes * routes, double lambda /*=0.05*/) {

	bool * assigned_customers = new bool[routes->ncustomers+1];
	for (int i=0; i<routes->ncustomers+1; i++)
		assigned_customers[i] = false;
	int * min_pos = new int[routes->ncustomers+1];
	int * min_route = new int[routes->ncustomers+1];
	double * min_cost = new double[routes->ncustomers+1];

	assigned_customers[0] = true;

	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);

	double g = Random::genrand_real3()*3.0;
	double f = Random::genrand_real1();

	routes->newRoute();

	while (!allCustomersAsigned(assigned_customers,routes->ncustomers)) {

		for (int i=0; i<routes->ncustomers+1; i++) {
			min_pos[i] = -1;
			min_route[i] = -1;
			min_cost[i] = numeric_limits<double>::max() -1.0;
		}

		double mcost = numeric_limits<double>::max() -1.0; 	// Costo minimo
		int mp = -1; 				// Posicion de insercion de costo minimo
		int mr = -1; 				// Ruta de insercion de costo minimo
		int mc = -1;				// Cliente con insercion de costo minimo

		for (int customer=1; customer<routes->ncustomers+1; customer++) {

			double customer_min_cost = numeric_limits<double>::max() -1.0;

			if (assigned_customers[customer]) continue;

			for (int route=0; route<routes->last_route+1; route++) {

				int prevNode = 0;
				int nextNode = routes->getArc(route,0);
				do {

					double 	cost = routes->costs[prevNode][customer] + routes->costs[customer][nextNode] -
							g*routes->costs[prevNode][nextNode] +
							f*fabs(routes->costs[prevNode][customer] - routes->costs[customer][nextNode]);

					if (cost<customer_min_cost) {
						routes->setArc(route,prevNode,customer);
						routes->setArc(route,customer,nextNode);
						routes->setReverseArc(route,customer,prevNode);
						routes->setReverseArc(route,nextNode,customer);
						routes->updateFullRouteCapacity(route);
						if (routes->checkFeasibility(route)) {
							customer_min_cost = cost;
							min_pos[customer] = prevNode;
							min_route[customer] = route;
							min_cost[customer] = cost;
							if (cost<mcost) {
								mcost = cost;
								mp = prevNode;
								mr = route;
								mc = customer;
							}
						}
						routes->setArc(route,prevNode,nextNode);
						routes->setReverseArc(route,nextNode,prevNode);
						routes->updateFullRouteCapacity(route);
					}

					prevNode = nextNode;
					nextNode = routes->getArc(route,nextNode);

				} while (prevNode!=0);


			}

		}

		if (mr==-1) {
			routes->newRoute();
			continue;
		}

		mcost = numeric_limits<double>::max() -1.0;
		for (int customer=1; customer<routes->ncustomers+1; customer++) {

			if (assigned_customers[customer]) continue;
			if (min_pos[customer]!=mp) continue;
			if (min_route[customer]!=mr) continue;

			double cost = min_cost[customer];
			cost = lambda*(routes->costs[customer][0] + routes->costs[0][customer]) - cost;

			if (cost<mcost) {
				mcost = cost;
				mc = customer;
			}

		}

		int prevNode = mp;
		int nextNode = routes->getArc(mr,mp);
		if (mr>routes->last_route+1) {
			routes->newRoute();
		}
		routes->setArc(mr,prevNode,mc);
		routes->setArc(mr,mc,nextNode);
		routes->setReverseArc(mr,mc,prevNode);
		routes->setReverseArc(mr,nextNode,mc);
		assigned_customers[mc] = true;
		//break;

	}

	for (int i=0;i<routes->last_route+1;i++) {
		routes->updateFullRouteCapacity(i);
		double cost = routes->calcTotalRouteCost(i);
		routes->setRouteCost(i,cost);
	}

	routes->last_route++;

	delete [] assigned_customers;
	delete [] min_pos;
	delete [] min_route;
	delete [] min_cost;
}


bool Routes::allCustomersAsigned(bool * assigned_customers, int size) {
	for (int i=1;i<size+1; i++)
		if (!assigned_customers[i])
			return false;
	return true;
}

Routes::~Routes() {
	delete [] customers;
	delete [] customers_reverse;
	delete [] capacity_pickup;
	delete [] capacity_delivery;
	delete [] capacity_at_depot_pickup;
	delete [] capacity_at_depot_delivery;
	delete [] routes;
	delete [] routes_costs;
	delete [] routes_reverse;
}


