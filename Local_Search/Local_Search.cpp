/*
 * Local_Search.cpp
 *
 *  Created on: 06/02/2011
 *      Author: rsantana
 */

#include "Local_Search.h"
#include "Routes.h"
#include "Input.h"
#include "Operators.h"
#include <iostream>
#include <stdio.h>

double Local_Search::diff_parameter = 0.05;
double Local_Search::diff_neg_parameter = 0.0;

int Local_Search::ls_shift_one(int nIter, Routes* solution, Input* in){
	int i;
	int k;
	int mod = -1;
	int res = -1;
	int city;
	int route;
	int min = 0;
	double min_cost = 1.0;
	int min_route = 0;
	int pos = 0;

	for(k=0;k<nIter;k++){
		min =0;
		min_cost = 1.0;
		min_route = 0;
		pos = 0;
		mod = -1;
		for(i=0;i<solution->last_route;i++){
			city = solution->getArc(i,0);
			while(true){
				if(city==0){
					break;
				}
				if((Operators::check_shift_one(city,i,&min,&min_cost,&min_route,&pos,solution,in))==0){
					route = i;
					mod = 0;
				};
				city = solution->getArc(i,city);
			}
		}
		if(mod == 0){
			Operators::shift_one(min,route,pos,min_route,solution,in);
			res = 0;
		}else{
			return res;
		}
	}
	res = 1;
	return res;
}

int Local_Search::ls_shift_two(int nIter, Routes* solution, Input* in){
	int i;
	int k;
	int mod = -1;
	int res = -1;
	int city;
	int city_next;
	int route;
	int min = 0;
	double min_cost = 1.0;
	int min_route = 0;
	int pos = 0;

	for(k=0;k<nIter;k++){
		min =0;
		min_cost = 1.0;
		min_route = 0;
		pos = 0;
		mod = -1;
		for(i=0;i<solution->last_route;i++){
			city = solution->getArc(i,0);
			city_next = solution->getArc(i,city);
			while(true){
				if(city_next==0){
					break;
				}
				if((Operators::check_shift_two(city,i,&min,&min_cost,&min_route,&pos,solution,in))==0){
					route = i;
					res = 0;
					mod = 0;
				};
				city = city_next;
				city_next = solution->getArc(i,city_next);
			}
		}
		if(mod == 0){
			Operators::shift_two(min,route,pos,min_route,solution,in);
		}else{
			return res;
		}
	}
	return 1;
}


int Local_Search::ls_swap_one(int nIter, Routes* solution, Input* in){

	int i;
	int k;
	int res = -1;
	int mod =-1;
	int city;
	int min1 , min2 = 0;
	double min_cost = 1.0;
	int min_route1, min_route2 = 0;

	for(k=0;k<nIter;k++){
		min1 = 0;
		min2 = 0;
		mod = -1;
		min_cost = 1.0;
		min_route1 = 0;
		min_route2 = 0;
		for(i=0;i<solution->last_route;i++){
			city = solution->getArc(i,0);
			while(true){
				if(city==0){
					break;
				}
				if( (Operators::check_swap_one(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,solution,in))==0){
					res = 0;
					mod = 0;
				};
				city = solution->getArc(i,city);
			}
		}
		if(mod==0){
			Operators::swap_one(min1,min_route1,min2,min_route2,solution,in);
		}else{
			return res;
		}
	}
	return 1;
}

int Local_Search::ls_swap_two_one(int nIter, Routes* solution, Input* in){
	int res = -1;
	int i;
	int k;
	int mod = -1;
	int city;
	int min1 , min2 = 0;
	double min_cost = 1;
	int min_route1, min_route2 = 0;
	int city_next;
	for(k=0;k<nIter;k++){
		mod = -1;
		min1 = 0;
		min2 = 0;
		min_cost = 1.0;
		min_route1 = 0;
		min_route2 = 0;
		for(i=0;i<solution->last_route;i++){
			city = solution->getArc(i,0);
			if(city==0){
				continue;
			}
			city_next = solution->getArc(i,city);
			while(true){
				if(city_next==0){
					break;
				}
				if((Operators::check_swap_two_one(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,solution,in))==0){
					mod = 0;
					res = 0;
				};
				city = city_next;
				city_next = solution->getArc(i,city_next);
			}
		}
		if(mod==0){
			Operators::swap_two_one(min1,min_route1,min2,min_route2,solution,in);
		}else{
			return res;
		}
	}
	return 1;

}

int Local_Search::ls_swap_two_two(int nIter, Routes* solution, Input* in){
	int res = -1;
	int i;
	int k;
	int mod = -1;
	int city;
	int min1 , min2 = 0;
	double min_cost = 1;
	int min_route1, min_route2 = 0;
	int city_next;
	for(k=0;k<nIter;k++){
		mod = -1;
		min1 = 0;
		min2 = 0;
		min_cost = 1.0;
		min_route1 = 0;
		min_route2 = 0;
		for(i=0;i<solution->last_route;i++){
			city = solution->getArc(i,0);
			if(city==0){
				continue;
			}
			city_next = solution->getArc(i,city);
			while(true){
				if(city_next==0){
					break;
				}
				if((Operators::check_swap_two_two(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,solution,in))==0){
					mod = 0;
					res = 0;
				};
				city = city_next;
				city_next = solution->getArc(i,city_next);
			}
		}
		if(mod==0){
			Operators::swap_two_two(min1,min_route1,min2,min_route2,solution,in);
		}else{
			return res;
		}
	}
	return 1;

}

int Local_Search::ls_crossover(int nIter, Routes* solution, Input* in){
	int res = -1, mod = -1;
	int i, k;
	int city;
	int min1 , min2;
	double min_cost = 1;
	int min_route1, min_route2;
	double acc_cost[solution->ncustomers+1];
	for(k=0;k<nIter;k++){
			mod = -1;
			min1 = 0;
			min2 = 0;
			min_cost = 500.0;
			min_route1 = 0;
			min_route2 = 0;
			solution->calcAccCost(acc_cost);
			for(i=0;i<solution->last_route;i++){
				if(solution->getArc(i,0)==0){
					continue;
				};
				city = 0;
				while(true){
					if((Operators::check_crossover(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,solution,in,acc_cost))==0){
						mod = 0;
						res = 0;
					};
					city = solution->getArc(i,city);
					if(city==0){
						break;
					}
				}
			}
			if(mod==0){
				Operators::crossover(min1,min_route1,min2,min_route2,solution,in,acc_cost);
			}else{
				//cout<<"test = "<<test<<"\n";
				return res;
			}
		}
	return 1;
}

int Local_Search::ls_interchange(int nIter, Routes* solution, Input* in){
	int res = -1, mod = -1;
	int i, k;
	int city;
	int min1 , min2;
	double min_cost = 1;
	int min_route1, min_route2;
	double acc_cost[solution->ncustomers+1];
	for(k=0;k<nIter;k++){
			mod = -1;
			min1 = 0;
			min2 = 0;
			min_cost = 500.0;
			min_route1 = 0;
			min_route2 = 0;
			solution->calcAccCost(acc_cost);
			for(i=0;i<solution->last_route;i++){
				if(solution->getArc(i,0)==0){
					continue;
				};
				solution ->invertArcs(i);
				city = 0;
				while(true){
					if((Operators::check_Interchange(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,solution,in,acc_cost))==0){
						mod = 0;
						res = 0;
					};
					city = solution->getArc(i,city);
					if(city==0){
						break;
					}
				}
				solution ->invertArcs(i);
			}
			if(mod==0){
				Operators::Interchange(min1,min_route1,min2,min_route2,solution,in,acc_cost);
			}else{
				//cout<<"test = "<<test<<"\n";
				return res;
			}
		}
	return 1;
}

/*
 * Intercambio de rutas, se dividen 2 rutas en principio y final, se conecta el principio de una
 * con el final de la otra, y el principio de la segunda con el final de la primera.
 * Todas las divisiones y todos los pares de ruta son considerados
 * Alias del ls_crossover
 */
int Local_Search::interChange1InterRoute(int n_iter, Routes* routes, Input * in) {
	return Local_Search::ls_crossover(n_iter, routes, in);
}

/*
 * Intercambio de rutas, se dividen 2 rutas en principio y final, se conecta el principio de una
 * con el principio de la otra, y el final de la segunda con el final de la primera.
 * Todas las divisiones y todos los pares de ruta son considerados
 */
int Local_Search::interChange2InterRoute(int nIter, Routes* solution, Input* in) {
	return Local_Search::ls_interchange(nIter, solution, in);
}



/*
 * Alias para ls_two_opt
 */
bool Local_Search::interChange1IntraRoute(int route, Routes * routes) {
	return ls_two_opt(route, routes);
}

/*
 * Local Search utilizando operador 2-opt INTRA RUTA
 */
bool Local_Search::ls_two_opt(int route, Routes * routes) {

	int node1;
	int next1;
	int node2;
	int next2;
	int node1_best = -1;
	int node2_best = -1;
	double diff_cost_min = 0.0;

	node1 = 0;
	next1 = routes->getArc(route,node1);

	do {
		node2 = routes->getArc(route,next1);
		if (node2==0) break;
		next2 = routes->getArc(route,node2);
		do {
			if (node1==next2) break;
			double diff_cost = 	- routes->costs[node1][next1] -
								routes->costs[node2][next2] +
								routes->costs[node1][node2] +
								routes->costs[next1][next2];

			if (diff_cost + epsilon_parameter < 0.0) {
				routes->invertArcs(route,next2,node1);
				routes->setArc(route,node2,node1);
				routes->setArc(route,next2,next1);
				routes->setReverseArc(route,node1,node2);
				routes->setReverseArc(route,next1,next2);
				routes->updateFullRouteCapacity(route);
				if (routes->checkFeasibility(route)) {
					node1_best = node1;
					node2_best = node2;
					diff_cost_min = diff_cost;
				}
				routes->invertArcs(route,node1,next2);
				routes->setArc(route,node1,next1);
				routes->setArc(route,node2,next2);
				routes->setReverseArc(route,next1,node1);
				routes->setReverseArc(route,next2,node2);
			}
			if (next2==0) break;
			node2 = next2;
			next2 = routes->getArc(route,node2);
		} while (true);
		if (next1==0) break;
		node1 = next1;
		next1 = routes->getArc(route,node1);
	} while (true);

	if (node1_best==-1) {
		routes->updateFullRouteCapacity(route);
		return false;
	}

	Operators::two_up(route,node1_best,node2_best,routes);
	routes->setRouteCost(route,routes->getRouteCost(route)+diff_cost_min);
	return true;
}


/*
 * Local Search utilizando el operador de relocate INTRA RUTA
 * Modifica la ruta obteniendo el mejor cambio posible (chequeando factibilidad y costo)
 * realizando un relocate entre tres clientes contiguos y sus posibles nuevas posiciones.
 * Devuelve True en caso en que se pudo mejorar la solucion y se hizo
 * Devuelve False en caso de que no hubo mejoras en la solucion
 * MEJOR - MEJOR
 */
bool Local_Search::ls_or_opt_three(int route, Routes * routes) {

	double diff_cost_min = 0.0;
	int node1_best = -1;
	int new_position1_best = -1;
	int new_position2_best = -1;
	bool sorted_best = false;

	int node1=routes->getArc(route,0);
	int node2=routes->getArc(route,node1);
	int node3=routes->getArc(route,node2);

	if (node1==0 || node2==0 || node3==0) return false;


	while (node3!=0) {

		int new_position1 = 0;
		bool sorted = false;
		int next_node = routes->getArc(route, node3);
		int prev_node = routes->getReverseArc(route, node1);
		do {

			int new_position2 = routes->getArc(route, new_position1);
			if (new_position2 == node1 || new_position2 == node2 || new_position2 == node3 || new_position2 == next_node) {
				sorted = true;
				new_position1 = routes->getArc(route,new_position1);
				continue;
			}

			double diff_cost = 	- routes->costs[prev_node][node1] -
								routes->costs[node3][next_node] -
								routes->costs[new_position1][new_position2] +
								routes->costs[prev_node][next_node] +
								routes->costs[new_position1][node1] +
								routes->costs[node3][new_position2];

			if (diff_cost + epsilon_parameter < 0.0) {
				routes->setArc(route,prev_node,next_node);
				routes->setArc(route,new_position1,node1);
				routes->setArc(route,node3,new_position2);
				routes->setReverseArc(route,next_node,prev_node);
				routes->setReverseArc(route,node1,new_position1);
				routes->setReverseArc(route,new_position2,node3);
				routes->updateFullRouteCapacity(route);
				if (routes->checkFeasibility(route)) {
					diff_cost_min = diff_cost;
					node1_best = node1;
					new_position1_best = new_position1;
					new_position2_best = new_position2;
					sorted_best = sorted;
				}
				routes->setArc(route,new_position1,new_position2);
				routes->setArc(route,prev_node,node1);
				routes->setArc(route,node3,next_node);
				routes->setReverseArc(route,new_position2,new_position1);
				routes->setReverseArc(route,node1,prev_node);
				routes->setReverseArc(route,next_node,node3);
			}
			new_position1 = routes->getArc(route,new_position1);

		} while (new_position1!=0);

		node1=routes->getArc(route,node1);
		node2=routes->getArc(route,node1);
		node3=routes->getArc(route,node2);

	}

	if (node1_best==-1) {
		routes->updateFullRouteCapacity(route);
		return false;
	}

	Operators::relocateFeasibleCustomerIntraThree(route,node1_best,new_position1_best,new_position2_best,routes,sorted_best);
	routes->setRouteCost(route,routes->getRouteCost(route)+diff_cost_min);
	return true;


}

/*
 * Local Search utilizando el operador de relocate INTRA RUTA
 * Modifica la ruta obteniendo el mejor cambio posible (chequeando factibilidad y costo)
 * realizando un relocate entre dos clientes contiguos y sus posibles nuevas posiciones.
 * Devuelve True en caso en que se pudo mejorar la solucion y se hizo
 * Devuelve False en caso de que no hubo mejoras en la solucion
 * MEJOR - MEJOR
 */
bool Local_Search::ls_or_opt_two(int route, Routes * routes) {

	double diff_cost_min = 0.0;
	int node1_best = -1;
	int new_position1_best = -1;
	int new_position2_best = -1;
	bool sorted_best = false;

	int node1=routes->getArc(route,0);
	int node2=routes->getArc(route,node1);

	if (node1==0 || node2==0) return false;

	while (node2!=0) {

		int new_position1 = 0;
		bool sorted = false;
		int next_node = routes->getArc(route, node2);
		int prev_node = routes->getReverseArc(route, node1);
		do {

			int new_position2 = routes->getArc(route, new_position1);
			if (new_position2 == node1 || node2 == new_position2 || new_position2 == next_node) {
				sorted = true;
				new_position1 = routes->getArc(route,new_position1);
				continue;
			}

			double diff_cost = 	- routes->costs[prev_node][node1] -
								routes->costs[node2][next_node] -
								routes->costs[new_position1][new_position2] +
								routes->costs[prev_node][next_node] +
								routes->costs[new_position1][node1] +
								routes->costs[node2][new_position2];

			if (diff_cost + epsilon_parameter < 0.0) {
				routes->setArc(route,prev_node,next_node);
				routes->setArc(route,new_position1,node1);
				routes->setArc(route,node2,new_position2);
				routes->setReverseArc(route,next_node,prev_node);
				routes->setReverseArc(route,node1,new_position1);
				routes->setReverseArc(route,new_position2,node2);
				routes->updateFullRouteCapacity(route);
				if (routes->checkFeasibility(route)) {
					diff_cost_min = diff_cost;
					node1_best = node1;
					new_position1_best = new_position1;
					new_position2_best = new_position2;
					sorted_best = sorted;
				}
				routes->setArc(route,new_position1,new_position2);
				routes->setArc(route,prev_node,node1);
				routes->setArc(route,node2,next_node);
				routes->setReverseArc(route,new_position2,new_position1);
				routes->setReverseArc(route,node1,prev_node);
				routes->setReverseArc(route,next_node,node2);
			}
			new_position1 = routes->getArc(route,new_position1);

		} while (new_position1!=0);

		node1=routes->getArc(route,node1);
		node2=routes->getArc(route,node1);

	}

	if (node1_best==-1) {
		routes->updateFullRouteCapacity(route);
		return false;
	}

	Operators::relocateFeasibleCustomerIntraTwo(route,node1_best,new_position1_best,new_position2_best,routes,sorted_best);
	routes->setRouteCost(route,routes->getRouteCost(route)+diff_cost_min);
	return true;

}


/*
 * Alias a relocateIntraRoute.
 * Local Search utilizando el operador de relocate INTRA RUTA
 * Modifica la ruta obteniendo el mejor cambio posible (chequeando factibilidad y costo)
 * realizando un relocate entre un cliente y sus posibles nuevas posiciones.
 * Devuelve True en caso en que se pudo mejorar la solucion y se hizo
 * Devuelve False en caso de que no hubo mejoras en la solucion
 * MEJOR - MEJOR
 */
bool Local_Search::ls_or_opt_one(int route, Routes * routes) {

	return Local_Search::relocateIntraRoute(route,routes);

}

/*
 * Realiza la inversion de sentido completa de una ruta si en la ruta invertida la capacidad maxima del camion
 * disminuye
 */
bool Local_Search::ls_reverse(int route, Routes * routes) {

	double initial_max_capacity = routes->getDeliveryCapacity(0,route);
	for (int customer = routes->getArc(route,0); customer!=0 ; customer = routes->getArc(route,customer)) {
		if (routes->getDeliveryCapacity(customer,route) + routes->getPickupCapacity(customer,route) > initial_max_capacity) {
			initial_max_capacity = routes->getDeliveryCapacity(customer,route) + routes->getPickupCapacity(customer,route);
		}
	}
	if (routes->getPickupCapacity(0,route) > initial_max_capacity) {
		initial_max_capacity = routes->getPickupCapacity(0,route);
	}
	routes->invertArcs(route);
	routes->updateFullRouteCapacity(route);
	double final_max_capacity = routes->getDeliveryCapacity(0,route);
	for (int customer = routes->getArc(route,0); customer!=0 ; customer = routes->getArc(route,customer)) {
			if (routes->getDeliveryCapacity(customer,route) + routes->getPickupCapacity(customer,route) > final_max_capacity) {
				final_max_capacity = routes->getDeliveryCapacity(customer,route) + routes->getPickupCapacity(customer,route);
			}
	}
	if (routes->getPickupCapacity(0,route) > final_max_capacity) {
		final_max_capacity = routes->getPickupCapacity(0,route);
	}

	if (final_max_capacity < initial_max_capacity) {
		return true;
	}

	routes->invertArcs(route);
	routes->updateFullRouteCapacity(route);

	return false;
}

/*
 * Local Search utilizando el Operador swap intra-ruta.
 * Modifica la ruta obteniendo el mejor cambio posible (chequeando factibilidad y costo)
 * realizando un swap entre los clientes.
 * Devuelve True en caso en que se pudo mejorar la solucion y se hizo
 * Devuelve False en caso de que no hubo mejoras en la solucion
 * MEJOR - MEJOR
 */
bool Local_Search::swapIntraRoute(int route, Routes * routes) {

	double diff_cost_min = 0.0;
	int i_best = -1;
	int j_best = -1;

	for (int i=routes->getArc(route,0); i!=0;i=routes->getArc(route,i)) {
		for (int j=routes->getArc(route,i); j!=0; j=routes->getArc(route,j)) {

		    int next1 = routes->getArc(route, i);
		    int prev1 = routes->getReverseArc(route, i);
		    int next2 = routes->getArc(route, j);
		    int prev2 = routes->getReverseArc(route, j);

		    if (next1==j) {

				double diff_cost =  - routes->costs[prev1][i] -
							   routes->costs[i][next1] - routes->costs[j][next2]
							+ routes->costs[prev1][j] + routes->costs[prev2][i]
							+ routes->costs[i][next2] + routes->costs[j][next1]
							+ routes->costs[j][i];

				if (diff_cost + epsilon_parameter < 0.0) {

					routes->setArc(route,prev1,j);
					routes->setArc(route,j,i);
					routes->setArc(route,i,next2);
					routes->setReverseArc(route,j,prev1);
					routes->setReverseArc(route,i,j);
					routes->setReverseArc(route,next2,i);
					routes->updateFullRouteCapacity(route);
					if (routes->checkFeasibility(route)) {
						diff_cost_min = diff_cost;
						i_best = i;
						j_best = j;
					}

					routes->setArc(route,prev1,i);
					routes->setArc(route,i,j);
					routes->setArc(route,j,next2);
					routes->setReverseArc(route,i,prev1);
					routes->setReverseArc(route,j,i);
					routes->setReverseArc(route,next2,j);
				}

			} else {

				double diff_cost =  - routes->costs[prev1][i] -
									routes->costs[i][next1] - routes->costs[prev2][j] -
									routes->costs[j][next2] + routes->costs[prev1][j] +
									routes->costs[prev2][i] + routes->costs[i][next2] +
									routes->costs[j][next1];

				if (diff_cost + epsilon_parameter < 0.0) {

					routes->setArc(route,prev1,j);
					routes->setArc(route,prev2,i);
					routes->setArc(route,i,next2);
					routes->setArc(route,j,next1);
					routes->setReverseArc(route,i,prev2);
					routes->setReverseArc(route,j,prev1);
					routes->setReverseArc(route,next2,i);
					routes->setReverseArc(route,next1,j);
					routes->updateFullRouteCapacity(route);

					if (routes->checkFeasibility(route)) {
						diff_cost_min = diff_cost;
						i_best = i;
						j_best = j;
					}

					routes->setArc(route,prev1,i);
					routes->setArc(route,prev2,j);
					routes->setArc(route,j,next2);
					routes->setArc(route,i,next1);
					routes->setReverseArc(route,j,prev2);
					routes->setReverseArc(route,i,prev1);
					routes->setReverseArc(route,next2,j);
					routes->setReverseArc(route,next1,i);


				}

			}

		}
	}

	if (i_best==-1) {
		routes->updateFullRouteCapacity(route);
		return false;
	}

	Operators::exchangeFeasibleCustomersIntra(route,i_best,j_best,routes,true);
	routes->setRouteCost(route,routes->getRouteCost(route)+diff_cost_min);
	return true;

}

/*
 * Local Search utilizando el operador de relocate INTRA RUTA
 * Modifica la ruta obteniendo el mejor cambio posible (chequeando factibilidad y costo)
 * realizando un relocate entre un cliente y sus posibles nuevas posiciones.
 * Devuelve True en caso en que se pudo mejorar la solucion y se hizo
 * Devuelve False en caso de que no hubo mejoras en la solucion
 * MEJOR - MEJOR
 */
bool Local_Search::relocateIntraRoute(int route, Routes * routes) {

	double diff_cost_min = 0.0;
	int i_best = -1;
	int j_best = -1;
	int j_next_best = -1;
	bool sorted_best = false;

	for (int i=routes->getArc(route,0); i!=0;i=routes->getArc(route,i)) {
		int j=0;
		bool sorted = false;
		do {

			int next1 = routes->getArc(route, i);
			int prev1 = routes->getReverseArc(route, i);
			int next2 = routes->getArc(route, j);
			if (i==next2 || i==j) {
				sorted = true;
				j=routes->getArc(route,j);
				continue;
			}

			double diff_cost = 	- routes->costs[prev1][i] -
								routes->costs[i][next1] + routes->costs[prev1][next1] -
								routes->costs[j][next2] + routes->costs[j][i] +
								routes->costs[i][next2];

			if (diff_cost + epsilon_parameter < 0.0) {
				routes->setArc(route,prev1,next1);
				routes->setArc(route,i,next2);
				routes->setArc(route,j,i);
				routes->setReverseArc(route,next1,prev1);
				routes->setReverseArc(route,next2,i);
				routes->setReverseArc(route,i,j);
				routes->updateFullRouteCapacity(route);
				if (routes->checkFeasibility(route)) {
					diff_cost_min = diff_cost;
					i_best = i;
					j_best = j;
					j_next_best = next2;
					sorted_best = sorted;
				}
				routes->setArc(route,prev1,i);
				routes->setArc(route,i,next1);
				routes->setArc(route,j,next2);
				routes->setReverseArc(route,i,prev1);
				routes->setReverseArc(route,next1,i);
				routes->setReverseArc(route,next2,j);
			}
			j=routes->getArc(route,j);

		} while (j!=0);
	}

	if (i_best==-1) {
		routes->updateFullRouteCapacity(route);
		return false;
	}

	Operators::relocateFeasibleCustomerIntra(route,i_best,j_best,j_next_best,routes,sorted_best);
	routes->setRouteCost(route,routes->getRouteCost(route)+diff_cost_min);
	return true;

}

/*
 * Local Search encargado del movimiento RELOCATE para la metaheuristica GTS
 */
bool Local_Search::relocateGTS(Routes * routes, Input * in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs) {

	//INTRARUTA
	double diff_cost_min = numeric_limits<double>::max() -1.0;
	int i_best = -1;
	int j_best = -1;
	int j_next_best = -1;
	int i_next_best = -1;
	bool sorted_best = false;
	int route_best = -1;

	for (int route = 0; route < routes->last_route; route++) {

		for (int i=routes->getArc(route,0); i!=0;i=routes->getArc(route,i)) {
			int j=0;
			bool sorted = false;
			do {

				bool better = false;
				int next1 = routes->getArc(route, i);
				int prev1 = routes->getReverseArc(route, i);
				int next2 = routes->getArc(route, j);

				if (i==next2 || i==j) {
					sorted = true;
					j=routes->getArc(route,j);
					continue;
				}

				if (prev1==next2 || next1==j) { //Se podria cambiar agregando las dos posibilidades de reversa al row en el tabu, y verificando las dos en el find
					j=routes->getArc(route,j);
					continue;
				}

				if (reduced_neighbourhood[prev1][next1] == false ||
					reduced_neighbourhood[i][next2] == false ||
					reduced_neighbourhood[j][i] == false) {
					if (j==0) break;
					j=routes->getArc(route,j);
					continue;
				}

				double diff_cost = 	- routes->costs[prev1][i] -
									routes->costs[i][next1] + routes->costs[prev1][next1] -
									routes->costs[j][next2] + routes->costs[j][i] +
									routes->costs[i][next2];

				if(diff_cost + epsilon_parameter < diff_cost_min && (diff_cost > diff_parameter || diff_cost < -1*diff_neg_parameter)) {
					routes->setArc(route,prev1,next1);
					routes->setArc(route,i,next2);
					routes->setArc(route,j,i);
					routes->setReverseArc(route,next1,prev1);
					routes->setReverseArc(route,next2,i);
					routes->setReverseArc(route,i,j);
					routes->updateFullRouteCapacity(route);
					if (routes->checkFeasibility(route)) {
						better = true;
					}
					routes->setArc(route,prev1,i);
					routes->setArc(route,i,next1);
					routes->setArc(route,j,next2);
					routes->setReverseArc(route,i,prev1);
					routes->setReverseArc(route,next1,i);
					routes->setReverseArc(route,next2,j);
				}

				if (better) {

					double real_diff_cost = - real_costs[prev1][i] -
											real_costs[i][next1] + real_costs[prev1][next1] -
											real_costs[j][next2] + real_costs[j][i] +
											real_costs[i][next2];

					if (routes->getTotalCost()+real_diff_cost + epsilon_parameter> best_solution) {
						Tabu_Row row(i,-1,-1,route,Tabu_Queue::relocate);
						if (queue->find(row)) {

							if (j==0) break;
							j=routes->getArc(route,j);
							continue;
						}
					}

					diff_cost_min = real_diff_cost;
					i_best = i;
					j_best = j;
					j_next_best = next2;
					i_next_best = next1;
					sorted_best = sorted;
					route_best = route;
				}

				j=routes->getArc(route,j);

			} while (j!=0);
		}
		routes->updateFullRouteCapacity(route);
	}

	//INTER RUTA
	int mod = -1;
	int city;
	int route;
	int min = 0;
	double min_cost = numeric_limits<double>::max() -1.0;
	int min_route = 0;
	int pos = 0;

	for(int i=0;i<routes->last_route;i++){
		city = routes->getArc(i,0);
		while(true){
			if(city==0){
				break;
			}
			if((Operators::check_shift_oneGTS(city,i,&min,&min_cost,&min_route,&pos,routes,in,reduced_neighbourhood,queue,best_solution,real_costs))==0){
				route = i;
				mod = 0;
			}
			city = routes->getArc(i,city);
		}
	}

	// min ciudad que voy a mover
	// i, donde esta la ciudad originalmente donde esta min
	// pos posicion donde la voy a insertar (antes de la ciudad)
	// min_route donde la voy a insertar, donde esta pos
	// min_cost el mejor diferencial

//FIN INTER RUTA

	if (route_best == -1 && mod == -1) {
		return false;
	}

	if (route_best == -1) {
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::shift_one(min,route,pos,min_route,routes,in);
		in->cost = aux;
		Tabu_Row row(min,-1,-1,min_route,Tabu_Queue::relocate);
		queue->push(row);
		//row.print();
		return true;
	}

	if (mod == -1) {
		Operators::relocateFeasibleCustomerIntra(route_best,i_best,j_best,j_next_best,routes,sorted_best);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		Tabu_Row row(i_best,-1,-1,route_best,Tabu_Queue::relocate);
		queue->push(row);
		//row.print();
		return true;
	}

	//Se realiza el operador shift inter ruta si el diferencial del costo es menor al intra ruta
	if (min_cost < diff_cost_min) {
		if (min_cost>10.0) {
			return false;
		}
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::shift_one(min,route,pos,min_route,routes,in);
		in->cost = aux;
		Tabu_Row row(min,-1,-1,min_route,Tabu_Queue::relocate);
		queue->push(row);
		//row.print();
	} else {
		if (diff_cost_min>10.0) {
			return false;
		}
		Operators::relocateFeasibleCustomerIntra(route_best,i_best,j_best,j_next_best,routes,sorted_best);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		Tabu_Row row(i_best,-1,-1,route_best,Tabu_Queue::relocate);
		queue->push(row);
		//row.print();
	}
	return true;

}


/*
 * Local Search encargado del movimiento SWAP para la metaheuristica GTS
 */
bool Local_Search::swapGTS(Routes * routes, Input * in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs) {

	//INTRARUTA
	double diff_cost_min = numeric_limits<double>::max() -1.0;
	int i_best = -1;
	int j_best = -1;
	int route_best = -1;

	for (int route = 0; route < routes->last_route; route++) {

		for (int i=routes->getArc(route,0); i!=0;i=routes->getArc(route,i)) {
			for (int j=routes->getArc(route,i); j!=0; j=routes->getArc(route,j)) {

				bool better = false;

				int next1 = routes->getArc(route, i);
				int prev1 = routes->getReverseArc(route, i);
				int next2 = routes->getArc(route, j);
				int prev2 = routes->getReverseArc(route, j);
				double diff_cost;
				double real_diff_cost;

				if (next1==j) {

					if (reduced_neighbourhood[prev1][j] == false ||
						reduced_neighbourhood[i][next2] == false ||
						reduced_neighbourhood[j][i] == false) {
						if (j==0) break;
						continue;
					}
					diff_cost =  - routes->costs[prev1][i] -
								   routes->costs[i][next1] - routes->costs[j][next2]
								+ routes->costs[prev1][j] + routes->costs[prev2][i]
								+ routes->costs[i][next2] + routes->costs[j][next1]
								+ routes->costs[j][i];

					if(diff_cost + epsilon_parameter < diff_cost_min && (diff_cost > diff_parameter || diff_cost < -1*diff_neg_parameter)) {

						routes->setArc(route,prev1,j);
						routes->setArc(route,j,i);
						routes->setArc(route,i,next2);
						routes->setReverseArc(route,j,prev1);
						routes->setReverseArc(route,i,j);
						routes->setReverseArc(route,next2,i);
						routes->updateFullRouteCapacity(route);
						if (routes->checkFeasibility(route)) {
							real_diff_cost = - real_costs[prev1][i] -
									   real_costs[i][next1] - real_costs[j][next2]
									+ real_costs[prev1][j] + real_costs[prev2][i]
									+ real_costs[i][next2] + real_costs[j][next1]
									+ real_costs[j][i];
							better = true;
						}

						routes->setArc(route,prev1,i);
						routes->setArc(route,i,j);
						routes->setArc(route,j,next2);
						routes->setReverseArc(route,i,prev1);
						routes->setReverseArc(route,j,i);
						routes->setReverseArc(route,next2,j);
					}

				} else {

					if (reduced_neighbourhood[prev1][j] == false ||
						reduced_neighbourhood[prev2][i] == false ||
						reduced_neighbourhood[i][next2] == false ||
						reduced_neighbourhood[j][next1] == false) {
						continue;
					}

					diff_cost =  - routes->costs[prev1][i] -
										routes->costs[i][next1] - routes->costs[prev2][j] -
										routes->costs[j][next2] + routes->costs[prev1][j] +
										routes->costs[prev2][i] + routes->costs[i][next2] +
										routes->costs[j][next1];

					if(diff_cost + epsilon_parameter < diff_cost_min && (diff_cost > diff_parameter || diff_cost < -1*diff_neg_parameter)) {

						routes->setArc(route,prev1,j);
						routes->setArc(route,prev2,i);
						routes->setArc(route,i,next2);
						routes->setArc(route,j,next1);
						routes->setReverseArc(route,i,prev2);
						routes->setReverseArc(route,j,prev1);
						routes->setReverseArc(route,next2,i);
						routes->setReverseArc(route,next1,j);
						routes->updateFullRouteCapacity(route);

						if (routes->checkFeasibility(route)) {
							better = true;
							real_diff_cost =  - real_costs[prev1][i] -
												real_costs[i][next1] - real_costs[prev2][j] -
												real_costs[j][next2] + real_costs[prev1][j] +
												real_costs[prev2][i] + real_costs[i][next2] +
												real_costs[j][next1];
						}

						routes->setArc(route,prev1,i);
						routes->setArc(route,prev2,j);
						routes->setArc(route,j,next2);
						routes->setArc(route,i,next1);
						routes->setReverseArc(route,j,prev2);
						routes->setReverseArc(route,i,prev1);
						routes->setReverseArc(route,next2,j);
						routes->setReverseArc(route,next1,i);


					}

				}

				if (better) {
					if (routes->getTotalCost()+real_diff_cost+epsilon_parameter > best_solution) {
						Tabu_Row row(i,-1,-1,route,Tabu_Queue::swap);
						if (queue->find(row)) {
							continue;
						}
					}
					diff_cost_min = real_diff_cost;
					i_best = i;
					j_best = j;
					route_best = route;
				}

			}
		}
		routes->updateFullRouteCapacity(route);
	}

	// INTERRUTA
	int i;
	int res = -1;
	int mod =-1;
	int city;
	int min1 , min2 = 0;
	double min_cost = numeric_limits<double>::max() -1.0;
	int min_route1, min_route2 = 0;

	min1 = 0;
	min2 = 0;
	mod = -1;
	min_cost = numeric_limits<double>::max();
	min_route1 = 0;
	min_route2 = 0;
	for(i=0;i<routes->last_route;i++){
		city = routes->getArc(i,0);
		while(true){
			if(city==0){
				break;
			}
			if( (Operators::check_swap_oneGTS(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,routes,in,reduced_neighbourhood,queue,best_solution, real_costs))==0){
				res = 0;
				mod = 0;
			}
			city = routes->getArc(i,city);
		}
	}

	//Fin INTER RUTA

	if (route_best == -1 && mod == -1) {
		return false;
	}

	if (route_best == -1) {
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::swap_one(min1,min_route1,min2,min_route2,routes,in);
		in->cost = aux;
		Tabu_Row row(min1,-1,-1,min_route1,Tabu_Queue::swap);
		queue->push(row);
		Tabu_Row row2(min2,-1,-1,min_route2,Tabu_Queue::swap);
		queue->push(row2);
		return true;
	}

	if (mod == -1) {
		Operators::exchangeFeasibleCustomersIntra(route_best,i_best,j_best,routes,true);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		Tabu_Row row(i_best,-1,-1,route_best,Tabu_Queue::swap);
		queue->push(row);
		Tabu_Row row2(j_best,-1,-1,route_best,Tabu_Queue::swap);
		queue->push(row2);
		return true;
	}

	//Se realiza el operador shift inter ruta si el diferencial del costo es menor al intra ruta
	if (min_cost < diff_cost_min) {
		if (min_cost>10.0) {
			return false;
		}
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::swap_one(min1,min_route1,min2,min_route2,routes,in);
		in->cost = aux;
		Tabu_Row row(min1,-1,-1,min_route1,Tabu_Queue::swap);
		queue->push(row);
		Tabu_Row row2(min2,-1,-1,min_route2,Tabu_Queue::swap);
		queue->push(row2);
	} else {
		if (diff_cost_min>10.0) {
			return false;
		}
		Operators::exchangeFeasibleCustomersIntra(route_best,i_best,j_best,routes,true);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		Tabu_Row row(i_best,-1,-1,route_best,Tabu_Queue::swap);
		queue->push(row);
		Tabu_Row row2(j_best,-1,-1,route_best,Tabu_Queue::swap);
		queue->push(row2);
	}

	return true;

}

/*
 * Local Search encargado del movimiento INTERCHANGE1 para la metaheuristica GTS
 */
bool Local_Search::interChange1GTS(Routes * routes, Input * in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs) {

	int node1_best = -1;
	int node2_best = -1;
	int node1_next_best = -1;
	int node2_next_best = -1;
	double diff_cost_min = numeric_limits<double>::max() -1.0;
	int route_best = -1;

	for (int route = 0; route < routes->last_route; route++) {

		int node1;
		int next1;
		int node2;
		int next2;
		node1 = 0;
		next1 = routes->getArc(route,node1);

		do {
			node2 = routes->getArc(route,next1);
			if (node2==0) break;
			next2 = routes->getArc(route,node2);
			do {
				bool better = false;

				if (node1==next2) break;

				if (reduced_neighbourhood[node2][node1] == false ||
					reduced_neighbourhood[next2][next1] == false) {
						if (next2==0) break;
						node2 = next2;
						next2 = routes->getArc(route,node2);
						continue;
				}

				double diff_cost = 	- routes->costs[node1][next1] -
									routes->costs[node2][next2] +
									routes->costs[node1][node2] +
									routes->costs[next1][next2];

				if(diff_cost + epsilon_parameter < diff_cost_min && (diff_cost > diff_parameter || diff_cost < -1*diff_neg_parameter)) {

					routes->invertArcs(route,next2,node1);
					routes->setArc(route,node2,node1);
					routes->setArc(route,next2,next1);
					routes->setReverseArc(route,node1,node2);
					routes->setReverseArc(route,next1,next2);
					routes->updateFullRouteCapacity(route);
					if (routes->checkFeasibility(route)) {
						better = true;
					}
					routes->invertArcs(route,node1,next2);
					routes->setArc(route,node1,next1);
					routes->setArc(route,node2,next2);
					routes->setReverseArc(route,next1,node1);
					routes->setReverseArc(route,next2,node2);

				}

				if (better) {

					double real_diff_cost = - real_costs[node1][next1] -
										real_costs[node2][next2] +
										real_costs[node1][node2] +
										real_costs[next1][next2];

					if (routes->getTotalCost()+real_diff_cost+epsilon_parameter> best_solution) {
						Tabu_Row row(node1,-1,-1,route,Tabu_Queue::interchange1,node2,-1);
						if (queue->find(row)) {
							if (next2==0) break;
							node2 = next2;
							next2 = routes->getArc(route,node2);
							continue;
						}
					}
					node1_next_best = next1;
					node2_next_best = next2;
					node1_best = node1;
					node2_best = node2;
					diff_cost_min = real_diff_cost;
					route_best = route;

				}

				if (next2==0) break;
				node2 = next2;
				next2 = routes->getArc(route,node2);
			} while (true);
			if (next1==0) break;
			node1 = next1;
			next1 = routes->getArc(route,node1);
		} while (true);
		routes->updateFullRouteCapacity(route);
	}

	//INTERRUTA
	int res, mod = -1;
	int city;
	int min1 = 0, min2 = 0;
	double min_cost = numeric_limits<double>::max() -1.0;
	int min_route1 = 0, min_route2 = 0;
	double acc_cost[routes->ncustomers+1];
	double real_acc_cost[routes->ncustomers+1];
	routes->calcAccCost(acc_cost);
	routes->calcAccCostFrom(real_costs,real_acc_cost);
	for(int i=0;i<routes->last_route;i++){
		if(routes->getArc(i,0)==0){
			continue;
		}
		city = 0;
		while(true){
			if((Operators::check_crossoverGTS(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,routes,in,acc_cost,reduced_neighbourhood,queue,best_solution, real_costs,real_acc_cost))==0){
				mod = 0;
				res = 0;
			}
			city = routes->getArc(i,city);
			if(city==0){
				break;
			}
		}
	}

	if (route_best == -1 && mod == -1) {
		return false;
	}

	if (route_best == -1) {
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::crossover(min1,min_route1,min2,min_route2,routes,in,real_acc_cost);
		in->cost = aux;
		Tabu_Row row(min1,-1,-1,min_route2,Tabu_Queue::interchange1,min2,-1);
		queue->push(row);
		//row.print();
		return true;
	}

	if (mod == -1) {
		//routes->printRouteDetails(route_best);
		Operators::two_up(route_best,node1_best,node2_best,routes);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		//cout << "Cambio: " << node1_best << " " << node1_next_best << " " << node2_best << " " << node2_next_best << endl;
		//routes->printRouteDetails(route_best);
		Tabu_Row row(node1_best,-1,-1,route_best,Tabu_Queue::interchange1,node2_best,-1);
		queue->push(row);
		//row.print();
		return true;
	}

	//Se realiza el operador shift inter ruta si el diferencial del costo es menor al intra ruta
	if (min_cost < diff_cost_min) {
		if (min_cost>10.0) {
			return false;
		}
		double ** aux = in->cost;
		in->cost = real_costs;
		Operators::crossover(min1,min_route1,min2,min_route2,routes,in,real_acc_cost);
		in->cost = aux;
		Tabu_Row row(min1,-1,-1,min_route2,Tabu_Queue::interchange1,min2,-1);
		queue->push(row);
		//row.print();
	} else {
		if (diff_cost_min>10.0) {
			return false;
		}
		//routes->printRouteDetails(route_best);
		Operators::two_up(route_best,node1_best,node2_best,routes);
		routes->setRouteCost(route_best,routes->getRouteCost(route_best)+diff_cost_min);
		//cout << "Cambio: " << node1_best << " " << node1_next_best << " " << node2_best << " " << node2_next_best << endl;
		//routes->printRouteDetails(route_best);
		Tabu_Row row(node1_best,-1,-1,route_best,Tabu_Queue::interchange1,node2_best,-1);
		queue->push(row);
		//row.print();
		//Tabu_Row row2(node1_best,-1,-1,route_best,Tabu_Queue::interchange1,node2_best,-1);
		//queue->push(row2);
	}

	return true;

}

/*
 * Local Search encargado del movimiento INTERCHANGE2 para la metaheuristica GTS
 */
bool Local_Search::interChange2GTS(Routes * routes, Input * in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs) {
	int res, mod = -1;
	int i;
	int city;
	int min1 , min2;
	double min_cost = 1;
	int min_route1, min_route2;
	double acc_cost[routes->ncustomers+1];
	double real_acc_cost[routes->ncustomers+1];
	mod = -1;
	min1 = 0;
	min2 = 0;
	min_cost = numeric_limits<double>::max() -1.0;
	min_route1 = 0;
	min_route2 = 0;
	routes->calcAccCost(acc_cost);
	routes->calcAccCostFrom(real_costs,real_acc_cost);
	for(i=0;i<routes->last_route;i++){
		if(routes->getArc(i,0)==0){
			continue;
		}
		routes ->invertArcs(i);
		city = 0;
		while(true){
			if((Operators::check_InterchangeGTS(city,i,&min1,&min2,&min_cost,&min_route1,&min_route2,routes,in,acc_cost,reduced_neighbourhood,queue,best_solution, real_costs,real_acc_cost))==0){
				mod = 0;
				res = 0;
			}
			city = routes->getArc(i,city);
			if(city==0){
				break;
			}
		}
		routes ->invertArcs(i);
	}

	if (mod == -1) {
		return false;
	}

	double ** aux = in->cost;
	in->cost = real_costs;
	Operators::Interchange(min1,min_route1,min2,min_route2,routes,in,real_acc_cost);
	in->cost = aux;
	Tabu_Row row(min1,-1,-1,min_route2,Tabu_Queue::interchange2,min2,-1);
	queue->push(row);
	//row.print();
	return true;

}
