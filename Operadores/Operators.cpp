
#include "Routes.h"
//#include "IO.h"
#include "Input.h"
#include "Operators.h"
#include <iostream>

using namespace std;

double Operators::diff_parameter = 0.07;
double Operators::diff_neg_parameter = 0.0;

//check_shift(1,0) ILS
//Returns -1 if there were no modifications, else return 0
int Operators::check_shift_one(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in){
	int mod = -1;
	int valid;
	double capacity;
	//Iterators
	int i;
	int p;
	int k;
	//City before the "p" city
	int last = 0;
	//Added Cost with shift in the current position
	double dif = 0.0;

	for(i=0;i<solution->last_route;i++){
		if(i==route)
			continue;
		last = 0;
		p= solution->getFirstCustomer(i);

		while(true){

			dif = (in->cost[last][city] + in->cost[city][p] - in->cost[last][p]);
			dif = dif - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][solution->getArc(route,city)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city)]);

			if(dif + epsilon_parameter >= 0.0){
				if(p==0){
					break;
				}
				last = p;
				p = solution->getArc(i,p);
				continue;
			}

			if(dif + epsilon_parameter <*min_cost){

				valid = 1;
				capacity = solution->getPickupCapacity(0,i) + in->pickups[city];
				if(capacity> in->capacity)
					valid = 0;
				k = solution->getFirstCustomer(i);
				capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] ;
				if(capacity> in->capacity)
					valid = 0;
				while(k!=p && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				capacity = capacity - in->deliveries[city] + in->pickups[city];

				if(capacity> in->capacity)
					valid=0;

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				if(valid){
					mod = 0;
					*min = city;
					*min_cost = dif;
					*pos = p;
					*min_route = i;
				}
			}
			if(p==0)
				break;
			last = p;
			p = solution->getArc(i,p);
		}
	}
	return mod;
}

void Operators::shift_one(int city, int city_route, int min_pos, int min_route, Routes* solution, Input* in){
	int p = min_pos;
	int last = solution->getReverseArc(min_route,min_pos);
	int route = city_route;

	//Modify Costs
	solution -> routes_costs[city_route] = solution -> routes_costs[city_route] - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][solution->getArc(route,city)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city)]);
	if(solution -> routes_costs[city_route]<0.0){
		int i = 0;
		i++;
	}
	solution -> routes_costs[min_route] =  solution -> routes_costs[min_route] + (in->cost[last][city] + in->cost[city][p] - in->cost[last][p]);
	if(solution -> routes_costs[min_route]<0.0){
		int i = 0;
		i++;
	}
	// Change Arcs
	solution->setArc(city_route , solution->getReverseArc(city_route ,city) , solution->getArc(city_route ,city));
	solution->setReverseArc(city_route,solution->getArc(city_route, city), solution->getReverseArc(city_route ,city));

	solution->setArc(min_route , solution->getReverseArc(min_route ,min_pos) , city);
	solution->setReverseArc(min_route,city, solution->getReverseArc(min_route ,min_pos));
	solution->setArc(min_route, city , min_pos);
	solution->setReverseArc(min_route,min_pos, city);

	// Update Capacities;

	solution->updateFullRouteCapacity(city_route);
	solution->updateFullRouteCapacity(min_route);

}

int Operators::check_shift_two(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in){
	int mod = -1;
	int valid;
	double capacity;
	//Iterators
	int i;
	int p;
	int k;
	//City before the "p" city
	int last = 0;
	//Added Cost with shift in the current position
	double dif = 0.0;
	int city_next = solution->getArc(route,city);
	for(i=0;i<solution->last_route;i++){
		if(i==route)
			continue;
		last = 0;
		p= solution->getFirstCustomer(i);
		while(true){
			dif = (in->cost[last][city] + in->cost[city][city_next]+ in->cost[city_next][p] - in->cost[last][p]) - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][city_next] +in->cost[city_next][solution->getArc(route,city_next)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city_next)]);

			if(dif + epsilon_parameter>=0){
				if(p==0){
					break;
				}
				last = p;
				p = solution->getArc(i,p);
				break;
			}

			if(dif + epsilon_parameter <*min_cost){

				valid = 1;
				k = solution->getFirstCustomer(i);
				capacity = solution->getPickupCapacity(0,i) + in->pickups[city] + in->pickups[city_next];
				if(capacity> in->capacity)
					valid = 0;
				capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] + in->deliveries[city_next] ;
				if(capacity> in->capacity)
					valid = 0;
				while(k!=p && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + in->deliveries[city_next] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				capacity = capacity - in->deliveries[city] + in->pickups[city];

				if(capacity> in->capacity)
					valid=0;

				capacity = capacity - in->deliveries[city_next] + in->pickups[city_next];

				if(capacity> in->capacity)
					valid=0;

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + in->pickups[city_next] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				if(valid){
					mod = 0;
					*min = city;
					*min_cost = dif;
					*pos = p;
					*min_route = i;
				}
			}
			if(p==0)
				break;
			last = p;
			p = solution->getArc(i,p);
		}
	}
	return mod;
}


void Operators::shift_two(int city, int route, int min_pos, int min_route, Routes* solution, Input* in){
	int city_next = solution ->getArc(route, city);
	int city_route = route;
	//Modify Costs
	solution -> routes_costs[city_route] = solution -> routes_costs[city_route] - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][city_next] +in->cost[city_next][solution->getArc(route,city_next)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city_next)]);
	if(solution -> routes_costs[city_route]<0.0){
		int i = 0;
		i++;
	}
	solution -> routes_costs[min_route] =  solution -> routes_costs[min_route] + (in->cost[ solution->getReverseArc(min_route,min_pos)][city] + in->cost[city][city_next]+ in->cost[city_next][min_pos] - in->cost[solution->getReverseArc(min_route,min_pos)][min_pos]);
	if(solution -> routes_costs[min_route]<0.0){
		int i = 0;
		i++;
	}
	// Change Arcs
	solution->setArc(route , solution->getReverseArc(route ,city) , solution->getArc(route ,city_next));
	solution->setReverseArc(city_route,solution->getArc(city_route, city_next), solution->getReverseArc(city_route ,city));

	solution->setArc(min_route , solution->getReverseArc(min_route ,min_pos) , city);
	solution->setReverseArc(min_route,city, solution->getReverseArc(min_route ,min_pos));
	solution->setArc(min_route, city_next , min_pos);
	solution->setReverseArc(min_route,min_pos, city_next);

	// Update Capacities;

	solution->updateFullRouteCapacity(city_route);
	solution->updateFullRouteCapacity(min_route);

}


int Operators::check_swap_one(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in){

	int mod = -1;
		int valid;
		double capacity;
		//Iterators
		int i;
		int p;
		int k;
		//City before the "p" city
		int last = 0;
		//Added Cost with shift in the current position
		double dif = 0.0;

		for(i=route+1;i<solution->last_route;i++){
			if(i==route)
				continue;
			last = 0;
			p= solution->getFirstCustomer(i);
			if(p==0){
				continue;
			}
			while(true){

				if(p==0)
					break;
				//last a city, city a solution. [solution->getReverseArc(route, city)][p]+ in->cost[p][solution->getArc(route, city)]. ->getArc(i,p. Hago lo mismo de dif
				dif = (in->cost[last][city] + in->cost[city][solution ->getArc(i,p)] - (in->cost[last][p] + in ->cost[p][solution ->getArc(i,p)] )) - (in->cost[solution->getReverseArc(route, city)][city] + in->cost[city][solution->getArc(route, city)] - (in->cost[solution->getReverseArc(route, city)][p]+ in->cost[p][solution->getArc(route, city)]));

				if(dif + epsilon_parameter>=0){
					last = p;
					p = solution->getArc(i,p);
					continue;
				}
				if(dif + epsilon_parameter<*min_cost){

					// Check insertion at route i
					valid = 1;
					k = solution->getFirstCustomer(i);
					capacity = solution->getPickupCapacity(0,i) + in->pickups[city] - in->pickups[p];
					if(capacity> in->capacity)
						valid = 0;
					capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] - in->deliveries[p] ;
					if(capacity> in->capacity)
						valid = 0;
					while(k!=p && valid){
						capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i) - in->deliveries[p];
						if(capacity> in->capacity) {
							valid = 0;
							break;
						}
						k = solution -> getArc(i,k);
					}

					capacity = capacity - in->deliveries[city] + in->pickups[city];

					if(capacity> in->capacity)
						valid=0;

					k = solution -> getArc(i,k);

					while(k!=0 && valid){
						capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + solution ->getPickupCapacity(k,i) - in->pickups[p];
						if(capacity> in->capacity){
							valid = 0;
							break;
						}
						k = solution -> getArc(i,k);

					}

					//Check capacity of city route with city "p"

					k = solution->getFirstCustomer(route);
					capacity = solution->getPickupCapacity(0,route) + in->pickups[p] - in->pickups[city];
					if(capacity> in->capacity)
						valid = 0;

					capacity = solution->getDeliveryCapacity(0,route) + in->deliveries[p] - in->deliveries[city];

					if(capacity> in->capacity)
						valid = 0;

					while(k!=city && valid){
						capacity = solution->getDeliveryCapacity(k,route) + in->deliveries[p] + solution ->getPickupCapacity(k,route) - in->deliveries[city];
						if(capacity> in->capacity) {
							valid = 0;
							break;
						}
						k = solution -> getArc(route,k);
					}

					capacity = capacity - in->deliveries[p] + in->pickups[p];

					if(capacity> in->capacity)
						valid=0;

					k = solution -> getArc(route, k);

					while(k!=0 && valid){
						capacity = solution->getDeliveryCapacity(k,route) + in->pickups[p] + solution ->getPickupCapacity(k,route) - in->pickups[city];
						if(capacity> in->capacity){
							valid = 0;
							break;
						}
						k = solution -> getArc(route ,k);

					}


					if(valid){
						mod = 0;
						*min1 = city;
						*min_cost = dif;
						*min2 = p;
						*min_route1 = route;
						*min_route2 = i;
					}
				}
				last = p;
				p = solution->getArc(i,p);
			}
		}
		return mod;

}

void Operators::swap_one(int city, int city_route, int min, int min_route, Routes* solution, Input* in){
	int aux = solution->getArc(min_route, min);
	int aux_reverse= solution->getReverseArc(min_route,min);
	//Modify Costs
	solution -> routes_costs[city_route] = solution -> routes_costs[city_route] - (in->cost[solution->getReverseArc(city_route, city)][city] + in->cost[city][solution->getArc(city_route, city)] - (in->cost[solution->getReverseArc(city_route, city)][min]+ in->cost[min][solution->getArc(city_route, city)]));
	solution -> routes_costs[min_route] = solution -> routes_costs[min_route] + (in->cost[solution->getReverseArc(min_route, min)][city] + in->cost[city][solution ->getArc(min_route,min)] - (in->cost[solution->getReverseArc(min_route, min)][min] + in ->cost[min][solution ->getArc(min_route,min)] ));

	// Change Arcs
	solution->setArc(city_route , solution->getReverseArc(city_route ,city) , min);
	solution->setReverseArc(city_route,min,solution->getReverseArc(city_route ,city));
	solution->setArc(city_route , min, solution->getArc(city_route ,city));
	solution->setReverseArc(city_route,solution->getArc(city_route, city), min);

	solution->setArc(min_route , aux_reverse , city);
	solution->setReverseArc(min_route,city, aux_reverse);
	solution->setArc(min_route, city , aux);
	solution->setReverseArc(min_route,aux, city);

	// Update Capacities;

	solution->updateFullRouteCapacity(city_route);
	solution->updateFullRouteCapacity(min_route);

}

int Operators::check_swap_two_one(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in){

	int mod = -1;
	int valid;
	double capacity;
	int city_next = solution->getArc(route,city);
	//Iterators
	int i;
	int p;
	int k;
	//City before the "p" city
	int last = 0;
	//Added Cost with shift in the current position
	double dif = 0.0;

	for(i=0;i<solution->last_route;i++){
		if(i==route)
			continue;
		last = 0;
		p= solution->getFirstCustomer(i);
		while(true){

			if(p==0)
				break;

			dif = (in->cost[last][city] + in->cost[city][city_next] + in->cost[city_next][solution ->getArc(i,p)] - (in->cost[last][p] + in ->cost[p][solution ->getArc(i,p)] ));
			dif= dif - (in->cost[solution->getReverseArc(route, city)][city] + in->cost[city][city_next] + in->cost[city_next][solution->getArc(route, city_next)] - (in->cost[solution->getReverseArc(route, city)][p]+ in->cost[p][solution->getArc(route, city_next)]));

			if(dif + epsilon_parameter>=0){
				last = p;
				p = solution->getArc(i,p);
				continue;
			}
			if(dif + epsilon_parameter<*min_cost){

				// Check insertion at route i
				valid = 1;
				k = solution->getFirstCustomer(i);

				capacity = solution->getPickupCapacity(0,i) + in->pickups[city] + in->pickups[city_next] - in->pickups[p];
				if(capacity> in->capacity)
					valid = 0;
				capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] + in->deliveries[city_next]- in->deliveries[p];
				if(capacity> in->capacity)
					valid = 0;
				while(k!=p && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i) + in->deliveries[city_next] - in->deliveries[p];
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				capacity = capacity - in->deliveries[city]  + in->pickups[city];

				if(capacity> in->capacity)
					valid=0;

				capacity = capacity - in->deliveries[city_next] + in->pickups[city_next];

				if(capacity> in->capacity)
					valid=0;



				k = solution -> getArc(i,k);

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + in->pickups[city_next] + solution ->getPickupCapacity(k,i) - in->pickups[p];
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);

				}

				//Check capacity of city route with city "p"

				k = solution->getFirstCustomer(route);
				capacity = solution->getPickupCapacity(0,route) - in->pickups[city_next] - in->pickups[city] + in->pickups[p];
				if(capacity> in->capacity)
					valid = 0;
				capacity = solution->getDeliveryCapacity(0,route) + in->deliveries[p]  - in->deliveries[city]- in->deliveries[city_next];

				if(capacity> in->capacity)
					valid = 0;

				while(k!=city && valid){
					capacity = solution->getDeliveryCapacity(k,route) + in->deliveries[p]  + solution ->getPickupCapacity(k,route) - in->deliveries[city] - in->deliveries[city_next];
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(route,k);
				}

				capacity = capacity - in->deliveries[p] + in->pickups[p];

				if(capacity> in->capacity)
					valid=0;

				k = solution -> getArc(route, k);
				k = solution -> getArc(route, k);

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,route) + in->pickups[p] + solution ->getPickupCapacity(k,route) - in->pickups[city] - in->pickups[city_next];
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(route ,k);

				}


				if(valid){
					mod = 0;
					*min1 = city;
					*min_cost = dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}
			}
			last = p;
			p = solution->getArc(i,p);
		}
	}
	return mod;

}

void Operators::swap_two_one(int city, int city_route, int min, int min_route, Routes* solution, Input* in){
	int city_next = solution->getArc(city_route, city) ;
	int aux = solution->getArc(min_route, min);
	int aux_reverse= solution->getReverseArc(min_route,min);
	//Modify Costs

	solution -> routes_costs[city_route] = solution ->routes_costs[city_route] - (in->cost[solution->getReverseArc(city_route, city)][city] + in->cost[city][city_next] + in->cost[city_next][solution->getArc(city_route, city_next)] - (in->cost[solution->getReverseArc(city_route, city)][min]+ in->cost[min][solution->getArc(city_route, city_next)]));
	solution -> routes_costs[min_route] = solution ->routes_costs[min_route] + (in->cost[solution->getReverseArc(min_route, min)][city] + in->cost[city][city_next] + in->cost[city_next][solution ->getArc(min_route,min)] - (in->cost[solution->getReverseArc(min_route, min)][min] + in ->cost[min][solution ->getArc(min_route,min)] ));

	// Change Arcs
	solution->setArc(city_route , solution->getReverseArc(city_route ,city) , min);
	solution->setReverseArc(city_route,min,solution->getReverseArc(city_route ,city));
	solution->setArc(city_route , min, solution->getArc(city_route ,city_next));
	solution->setReverseArc(city_route,solution->getArc(city_route, city_next), min);

	solution->setArc(min_route , aux_reverse , city);
	solution->setReverseArc(min_route,city, aux_reverse);
	solution->setArc(min_route, city_next , aux);
	solution->setReverseArc(min_route,aux, city_next);

	// Update Capacities;

	solution->updateFullRouteCapacity(city_route);
	solution->updateFullRouteCapacity(min_route);


}


int Operators::check_swap_two_two(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in){

	int mod = -1;
	int valid;
	double capacity;
	int city_next = solution->getArc(route,city);
	//Iterators
	int i;
	int p;
	int k;
	//City before the "p" city
	int last = 0;
	int next;
	//Added Cost with shift in the current position
	double dif = 0.0;

	for(i=route+1;i<solution->last_route;i++){
		if(i==route)
			continue;
		last = 0;
		p= solution->getFirstCustomer(i);
		next = solution->getArc(i,p);
		if(p==0){
			continue;
		}
		while(true){

			if(next==0){
				break;
			}
			dif = (in->cost[last][city] + in->cost[city][city_next] + in->cost[city_next][solution ->getArc(i,next)] - (in->cost[last][p] + in->cost[p][next] + in ->cost[next][solution ->getArc(i,next)] ));
			dif= dif - (in->cost[solution->getReverseArc(route, city)][city] + in->cost[city][city_next] + in->cost[city_next][solution->getArc(route, city_next)] - (in->cost[solution->getReverseArc(route, city)][p]+ in->cost[p][next]+ in->cost[next][solution->getArc(route, city_next)]));

			if(dif + epsilon_parameter>=0){
				last = p;
				p = next;
				next = solution->getArc(i,next);
				continue;
			}
			if(dif + epsilon_parameter<*min_cost){

				// Check insertion at route i
				valid = 1;
				k = solution->getFirstCustomer(i);

				capacity = solution->getPickupCapacity(0,i) + in->pickups[city] + in->pickups[city_next] - in->pickups[p] - in->pickups[next] ;
				if(capacity> in->capacity)
					valid = 0;
				capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] + in->deliveries[city_next]- in->deliveries[p] - in->deliveries[next];
				if(capacity> in->capacity)
					valid = 0;
				while(k!=p && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i) + in->deliveries[city_next] - in->deliveries[p] - in->deliveries[next];
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				capacity = capacity - in->deliveries[city]  + in->pickups[city];

				if(capacity> in->capacity)
					valid=0;

				capacity = capacity - in->deliveries[city_next] + in->pickups[city_next];

				if(capacity> in->capacity)
					valid=0;

				k = solution -> getArc(i,k);
				k = solution -> getArc(i,k);

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + in->pickups[city_next] + solution ->getPickupCapacity(k,i) - in->pickups[p] - in->pickups[next];
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);

				}

				//Check capacity of city route with city "p"

				k = solution->getFirstCustomer(route);
				capacity = solution->getPickupCapacity(0,route) - in->pickups[city_next] - in->pickups[city] + in->pickups[p] + in->pickups[next];
				if(capacity> in->capacity)
					valid = 0;
				capacity = solution->getDeliveryCapacity(0,route) + in->deliveries[p] + in->deliveries[next] - in->deliveries[city]- in->deliveries[city_next];

				if(capacity> in->capacity)
					valid = 0;

				while(k!=city && valid){
					capacity = solution->getDeliveryCapacity(k,route) + in->deliveries[p] +  in->deliveries[next] + solution ->getPickupCapacity(k,route) - in->deliveries[city] - in->deliveries[city_next];
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(route,k);
				}

				capacity = capacity - in->deliveries[p] + in->pickups[p];

				if(capacity> in->capacity)
					valid=0;

				capacity = capacity - in->deliveries[next] + in->pickups[next];

				if(capacity> in->capacity)
					valid=0;

				k = solution -> getArc(route, k);
				k = solution -> getArc(route, k);

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,route) + in->pickups[p] + in->pickups[next] + solution ->getPickupCapacity(k,route) - in->pickups[city] - in->pickups[city_next];
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(route ,k);

				}


				if(valid){
					mod = 0;
					*min1 = city;
					*min_cost = dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}
			}
			last = p;
			p = next;
			next = solution->getArc(i,next);
		}
	}
	return mod;

}


void Operators::swap_two_two(int city, int city_route, int min, int min_route, Routes* solution, Input* in){
	int route = city_route;
	int next = solution->getArc(min_route,min);
	int city_next = solution->getArc(city_route, city) ;
	int aux = solution->getArc(min_route, next);
	int aux_reverse= solution->getReverseArc(min_route,min);
	//Modify Costs

	solution -> routes_costs[city_route] = solution ->routes_costs[city_route] - (in->cost[solution->getReverseArc(route, city)][city] + in->cost[city][city_next] + in->cost[city_next][solution->getArc(route, city_next)] - (in->cost[solution->getReverseArc(route, city)][min]+ in->cost[min][next]+ in->cost[next][solution->getArc(route, city_next)]));

	solution -> routes_costs[min_route] = solution ->routes_costs[min_route] + (in->cost[solution->getReverseArc(min_route,min)][city] + in->cost[city][city_next] + in->cost[city_next][solution ->getArc(min_route,next)] - (in->cost[solution->getReverseArc(min_route,min)][min] + in->cost[min][next] + in ->cost[next][solution ->getArc(min_route,next)] ));

	// Change Arcs
	solution->setArc(city_route , solution->getReverseArc(city_route ,city) , min);
	solution->setReverseArc(city_route,min,solution->getReverseArc(city_route ,city));
	solution->setArc(city_route , next, solution->getArc(city_route ,city_next));
	solution->setReverseArc(city_route,solution->getArc(city_route, city_next), next);

	solution->setArc(min_route , aux_reverse , city);
	solution->setReverseArc(min_route,city, aux_reverse);
	solution->setArc(min_route, city_next , aux);
	solution->setReverseArc(min_route,aux, city_next);

	// Update Capacities;

	solution->updateFullRouteCapacity(city_route);
	solution->updateFullRouteCapacity(min_route);


}

int Operators::check_crossover(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost){
	double dif;
	int p, next, last;
	int mod, city_next;
	int i;
	int aux,aux2;
	double cost_i, cost_route;
	mod = -1;
	city_next = solution->getArc(route,city);

	for(i = route+1;i<solution->last_route;i++){
		if(route==i){
			continue;
		}
		if(solution ->getFirstCustomer(i)==0){
			continue;
		}
		last = 0;
		p = 0;
		next = solution->getArc(i,p);
		while(true){
			if((city==0) && (p==0)){
				last = p;
				p = next;
				next =  solution->getArc(i,p);
				continue;
			}
			if((city_next==0)&&(next==0)){
				break;
			}
			//city a next, p a city_next. Hago lo mismo que diff
			if(city==0){
				cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];

				if(next==0){
					cost_route = 0.0;
				}else{
					cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				}
			}else if(p==0){

				if(city_next==0){
					cost_i = 0.0;
				}else{
					cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
				}

				cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];

			}else{

				if(city_next==0){
					cost_i = acc_cost[p] + in->cost[p][city_next];
				}else{
					cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
				}
				if(next==0){
					cost_route = acc_cost[city] + in->cost[city][next];
				}else{
					cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				}

			}


			dif = cost_i + cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);
			if(dif + epsilon_parameter>= 0.0){
				if(next==0){
					break;
				}
				last = p;
				p = next;
				next = solution->getArc(i,next);
				continue;
			}
			if(dif + epsilon_parameter< *min_cost){
				//solution->printRouteDetails(route);
				//solution->printRouteDetails(i);
				aux = solution->getReverseArc(i,0);
				aux2 = solution->getReverseArc(route,0);
				if(next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);

				}else if(city_next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(route, 0,aux);

				}else{

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}

				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);


				if(solution->checkFeasibility(i) && solution->checkFeasibility(route)){
					mod=0;
					*min1 = city;
					*min_cost = dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}
				if(city==0 && next == 0){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else if((p==0) && (city_next==0)){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else{
				}


				if(next==0){

					aux = solution->getReverseArc(i,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(route, 0,aux);

				}else if(city_next==0){

					aux = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux);

				}
				else{
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}
				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

			}
			if(next == 0){
				break;
			}

			last = p;
			p = next;
			next = solution->getArc(i,next);
		}
	}
	return mod;
}

void Operators::crossover(int city, int route, int min, int min_route, Routes* solution, Input* in, double* acc_cost){
	int aux;
	int p = min;
	int i = min_route;
	int next = solution->getArc(i,p);
	int city_next = solution->getArc(route,city);
	double cost_i;
	double cost_route;

	if(city==0){
		cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];

		if(next==0){
			cost_route = 0.0;
		}else{
			cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
		}
	}else if(p==0){

		if(city_next==0){
			cost_i = 0.0;
		}else{
			cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
		}

		cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];

	}else{

		if(city_next==0){
			cost_i = acc_cost[p] + in->cost[p][city_next];
		}else{
			cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
		}
		if(next==0){
			cost_route = acc_cost[city] + in->cost[city][next];
		}else{
			cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
		}

	}

	solution ->setRouteCost(i,cost_i);
	solution ->setRouteCost(route, cost_route);

	if(next==0){

		aux = solution->getReverseArc(route,0);
		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		solution ->setReverseArc(i,0,aux);

	}else if(city_next==0){

		aux = solution->getReverseArc(i,0);
		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		solution ->setReverseArc(route, 0,aux);

	}else{

		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		aux = solution->getReverseArc(i,0);
		solution ->setReverseArc(i,0,solution->getReverseArc(route,0));
		solution ->setReverseArc(route, 0,aux);

	}
	solution->updateFullRouteCapacity(i);
	solution->updateFullRouteCapacity(route);

}

int Operators::check_Interchange(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost){
	double dif;
	int p, next, last;
	int mod, city_next;
	int i;
	int aux,aux2;
	double cost_i, cost_route;
	mod = -1;
	city_next = solution->getArc(route,city);

	for(i = route+1;i<solution->last_route;i++){
		if(route==i){
			continue;
		}

		if(solution ->getFirstCustomer(i)==0){
			continue;
		}
		last = 0;
		p = 0;
		next = solution->getArc(i,p);
		while(true){
			if((city==0) && (p==0)){
				last = p;
				p = next;
				next =  solution->getArc(i,p);
				continue;
			}
			if((city_next==0)&&(next==0)){
				break;
			}
			//city a next, p a city_next. Hago lo mismo que diff
			if(city==0){
				cost_i = acc_cost[p]+acc_cost[city_next] + in->cost[p][city_next];

				if(next==0){
					cost_route = 0.0;
				}else{
					cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				}
			}else if(p==0){

				if(city_next==0){
					cost_i = 0.0;
				}else{
					cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
				}

				cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];

			}else{

				if(city_next==0){
					cost_i = acc_cost[p] + in->cost[p][city_next];
				}else{
					cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
				}
				if(next==0){
					cost_route = (solution->getRouteCost(route)-acc_cost[city]) + in->cost[city][next];
				}else{
					cost_route = (solution->getRouteCost(route)-acc_cost[city]) +(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				}

			}


			dif = cost_i + cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);
			if(dif + epsilon_parameter>= 0.0){
				if(next==0){
					break;
				}
				last = p;
				p = next;
				next = solution->getArc(i,next);
				continue;
			}
			if(dif + epsilon_parameter< *min_cost){
				//solution->printRouteDetails(route);
				//solution->printRouteDetails(i);
				aux = solution->getReverseArc(i,0);
				aux2 = solution->getReverseArc(route,0);
				if(next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);

				}else if(city_next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(route, 0,aux);

				}else{

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}

				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);


				if(solution->checkFeasibility(i) && solution->checkFeasibility(route)){
					mod=0;
					*min1 = city;
					*min_cost = dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}
				if(city==0 && next == 0){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else if((p==0) && (city_next==0)){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else{
				}


				if(next==0){

					aux = solution->getReverseArc(i,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(route, 0,aux);

				}else if(city_next==0){

					aux = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux);

				}
				else{
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}
				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

			}
			if(next == 0){
				break;
			}

			last = p;
			p = next;
			next = solution->getArc(i,next);
		}
	}
	return mod;
}

void Operators::Interchange(int city, int route, int min, int min_route, Routes* solution, Input* in, double* acc_cost){
	int i = min_route;
	solution->invertArcs(route);
	int aux;
	int p = min;
	int next = solution->getArc(i,p);
	int city_next = solution->getArc(route,city);
	double cost_i;
	double cost_route;

	if(city==0){
		cost_i = acc_cost[p]+acc_cost[city_next] + in->cost[p][city_next];

		if(next==0){
			cost_route = 0.0;
		}else{
			cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
		}
	}else if(p==0){

		if(city_next==0){
			cost_i = 0.0;
		}else{
			cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
		}

		cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];

	}else{

		if(city_next==0){
			cost_i = acc_cost[p] + in->cost[p][city_next];
		}else{
			cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
		}
		if(next==0){
			cost_route = (solution->getRouteCost(route)-acc_cost[city]) + in->cost[city][next];
		}else{
			cost_route = (solution->getRouteCost(route)-acc_cost[city]) +(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
		}

	}

	solution ->setRouteCost(i,cost_i);
	solution ->setRouteCost(route, cost_route);

	if(next==0){

		aux = solution->getReverseArc(route,0);
		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		solution ->setReverseArc(i,0,aux);

	}else if(city_next==0){

		aux = solution->getReverseArc(i,0);
		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		solution ->setReverseArc(route, 0,aux);

	}else{

		solution->setArc(route,city,next);
		solution->setReverseArc(route,next,city);
		solution->setArc(i,p,city_next);
		solution->setReverseArc(i,city_next,p);
		aux = solution->getReverseArc(i,0);
		solution ->setReverseArc(i,0,solution->getReverseArc(route,0));
		solution ->setReverseArc(route, 0,aux);

	}
	solution->updateFullRouteCapacity(i);
	solution->updateFullRouteCapacity(route);

}



/*
 * Mueve el cliente node1 y sus 2 siguientes de la ruta route a la posicion entre el cliente new_position1 y new_position2
 * Solo cuando ya se sabe que es factible lograrlo
 */
void Operators::relocateFeasibleCustomerIntraThree(int route, int node1, int new_position1, int new_position2, Routes * routes, bool sorted) {

	int node2 = routes->getArc(route, node1);
	int node3 = routes->getArc(route, node2);
	int next = routes->getArc(route, node3);
	int prev = routes->getReverseArc(route, node1);
	routes->setArc(route,prev,next);
	routes->setArc(route,new_position1,node1);
	routes->setArc(route,node3,new_position2);
	routes->setReverseArc(route,next,prev);
	routes->setReverseArc(route,node1,new_position1);
	routes->setReverseArc(route,new_position2,node3);
    routes->updateFullRouteCapacity(route);
	//routes->updateRouteCapacity(route,sorted,prev,node1);

}

/*
 * Mueve el cliente node1 y su siguiente de la ruta route a la posicion entre el cliente new_position1 y new_position2
 * Solo cuando ya se sabe que es factible lograrlo
 */
void Operators::relocateFeasibleCustomerIntraTwo(int route, int node1, int new_position1, int new_position2, Routes * routes, bool sorted) {

	int node2 = routes->getArc(route, node1);
	int next = routes->getArc(route, node2);
	int prev = routes->getReverseArc(route, node1);
	routes->setArc(route,prev,next);
	routes->setArc(route,new_position1,node1);
	routes->setArc(route,node2,new_position2);
	routes->setReverseArc(route,next,prev);
	routes->setReverseArc(route,node1,new_position1);
	routes->setReverseArc(route,new_position2,node2);
    routes->updateFullRouteCapacity(route);
	//routes->updateRouteCapacity(route,sorted,prev,node1);

}

/*
 * Mueve el cliente customer de la ruta route a la posicion entre el cliente customer1 y customer24
 * Solo cuando ya se sabe que es factible lograrlo
 */
void Operators::relocateFeasibleCustomerIntra(int route, int customer, int customer1, int customer2, Routes * routes, bool sorted) {

	int next = routes->getArc(route, customer);
	int prev = routes->getReverseArc(route, customer);
	routes->setArc(route,prev,next);
	routes->setArc(route,customer1,customer);
	routes->setArc(route,customer,customer2);
	routes->setReverseArc(route,next,prev);
	routes->setReverseArc(route,customer2,customer);
	routes->setReverseArc(route,customer,customer1);
    routes->updateFullRouteCapacity(route);
	//routes->updateRouteCapacity(route,sorted,prev,customer);

}


/*
 * Mueve el cliente customer de la ruta route a la posicion entre el cliente customer1 y customer24
 * Solo cuando ya se sabe que es factible lograrlo
 */
void Operators::exchangeFeasibleCustomersIntra(int route, int customer1, int customer2, Routes * routes, bool sorted) {

    int next1 = routes->getArc(route, customer1);
    int prev1 = routes->getReverseArc(route, customer1);
    int next2 = routes->getArc(route, customer2);
    int prev2 = routes->getReverseArc(route, customer2);

    //Caso en el que los clientes estan seguidos
    if (next1==customer2) {

    	routes->setArc(route,prev1,customer2);
    	routes->setArc(route,customer2,customer1);
    	routes->setArc(route,customer1,next2);
    	routes->setReverseArc(route,customer2,prev1);
    	routes->setReverseArc(route,customer1,customer2);
    	routes->setReverseArc(route,next2,customer1);

    } else {

		routes->setArc(route,prev1,customer2);
		routes->setArc(route,prev2,customer1);
		routes->setArc(route,customer1,next2);
		routes->setArc(route,customer2,next1);
		routes->setReverseArc(route,customer1,prev2);
		routes->setReverseArc(route,customer2,prev1);
		routes->setReverseArc(route,next2,customer1);
		routes->setReverseArc(route,next1,customer2);
    }
    //routes->updateRouteCapacity(route,!sorted,customer1,customer2);
    routes->updateFullRouteCapacity(route);
}

/*
 * Elimina los arcos entre customer1 y su siguiente, y customer2 y su siguiente. Crea dos arcos nuevos
 * entre ellos e invierta la direccion de la ruta necesaria para formar asi una ruta nueva
 */
void Operators::two_up(int route, int customer1, int customer2, Routes * routes) {

	int next1;
	int next2;
	next1 = routes->getArc(route,customer1);
	next2 = routes->getArc(route,customer2);
	routes->invertArcs(route,next2,customer1);
	routes->setCompleteArc(route,customer2,customer1);
	routes->setCompleteArc(route,next2,next1);
	routes->updateFullRouteCapacity(route);

}

/*
 * Igual a check_shift_one pero con los cambios necesarios para GTS
 */
int Operators::check_shift_oneGTS(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs){
	int mod = -1;
	int valid;
	double capacity;
	//Iterators
	int i;
	int p;
	int k;
	//City before the "p" city
	int last = 0;
	//Added Cost with shift in the current position
	double dif = 0.0;

	for(i=0;i<solution->last_route;i++){
		if(i==route)
			continue;
		last = 0;
		p= solution->getFirstCustomer(i);

		while(true){

			if (reduced_neighbourhood[last][city] == false ||
				reduced_neighbourhood[city][p] == false) {
					if(p==0){
						break;
					}
					last = p;
					p = solution->getArc(i,p);
					continue;
			}

			dif = (in->cost[last][city] + in->cost[city][p] - in->cost[last][p]);
			dif = dif - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][solution->getArc(route,city)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city)]);

			if(dif + epsilon_parameter<*min_cost  && (dif > diff_parameter || dif < -1*diff_neg_parameter)){

				valid = 1;
				capacity = solution->getPickupCapacity(0,i) + in->pickups[city];
				if(capacity> in->capacity)
					valid = 0;
				k = solution->getFirstCustomer(i);
				capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] ;
				if(capacity> in->capacity)
					valid = 0;
				while(k!=p && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity) {
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				capacity = capacity - in->deliveries[city] + in->pickups[city];

				if(capacity> in->capacity)
					valid=0;

				while(k!=0 && valid){
					capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + solution ->getPickupCapacity(k,i);
					if(capacity> in->capacity){
						valid = 0;
						break;
					}
					k = solution -> getArc(i,k);
				}

				if(valid){

					double real_dif = (real_costs[last][city] + real_costs[city][p] - real_costs[last][p]);
					real_dif = real_dif - (real_costs[solution->getReverseArc(route,city)][city] + real_costs[city][solution->getArc(route,city)] - real_costs[solution->getReverseArc(route,city)][solution->getArc(route,city)]);

					if (solution->getTotalCost()+real_dif+epsilon_parameter> best_solution) {
						Tabu_Row row(city,-1,-1,i,Tabu_Queue::relocate);
						if (queue->find(row)) {
							if(p==0){
								break;
							}
							last = p;
							p = solution->getArc(i,p);
							continue;
						}
					}

					mod = 0;
					*min = city;
					*min_cost = real_dif;
					*pos = p;
					*min_route = i;
				}

			}
			if(p==0)
				break;
			last = p;
			p = solution->getArc(i,p);
		}
	}
	return mod;
}

/*
 * Igual a check_swap_one pero con los cambios necesarios para GTS
 */
int Operators::check_swap_oneGTS(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs){

	int mod = -1;
		int valid;
		double capacity;
		//Iterators
		int i;
		int p;
		int k;
		//City before the "p" city
		int last = 0;
		//Added Cost with shift in the current position
		double dif = 0.0;

		for(i=route+1;i<solution->last_route;i++){
			if(i==route)
				continue;
			last = 0;
			p= solution->getFirstCustomer(i);
			if(p==0){
				continue;
			}
			while(true){

				if(p==0)
					break;

				if (reduced_neighbourhood[last][city] == false ||
					reduced_neighbourhood[city][solution ->getArc(i,p)] == false ||
					reduced_neighbourhood[solution->getReverseArc(route, city)][p] == false ||
					reduced_neighbourhood[p][solution->getArc(route, city)] == false) {
						last = p;
						p = solution->getArc(i,p);
						continue;
				}
				dif = (in->cost[last][city] + in->cost[city][solution ->getArc(i,p)] - (in->cost[last][p] + in ->cost[p][solution ->getArc(i,p)] )) - (in->cost[solution->getReverseArc(route, city)][city] + in->cost[city][solution->getArc(route, city)] - (in->cost[solution->getReverseArc(route, city)][p]+ in->cost[p][solution->getArc(route, city)]));

				if(dif + epsilon_parameter<*min_cost  && (dif > diff_parameter || dif < -1*diff_neg_parameter)){

					// Check insertion at route i
					valid = 1;
					k = solution->getFirstCustomer(i);
					capacity = solution->getPickupCapacity(0,i) + in->pickups[city] - in->pickups[p];
					if(capacity> in->capacity)
						valid = 0;
					capacity = solution->getDeliveryCapacity(0,i) + in->deliveries[city] - in->deliveries[p] ;
					if(capacity> in->capacity)
						valid = 0;
					while(k!=p && valid){
						capacity = solution->getDeliveryCapacity(k,i) + in->deliveries[city] + solution ->getPickupCapacity(k,i) - in->deliveries[p];
						if(capacity> in->capacity) {
							valid = 0;
							break;
						}
						k = solution -> getArc(i,k);
					}

					capacity = capacity - in->deliveries[city] + in->pickups[city];

					if(capacity> in->capacity)
						valid=0;

					k = solution -> getArc(i,k);

					while(k!=0 && valid){
						capacity = solution->getDeliveryCapacity(k,i) + in->pickups[city] + solution ->getPickupCapacity(k,i) - in->pickups[p];
						if(capacity> in->capacity){
							valid = 0;
							break;
						}
						k = solution -> getArc(i,k);

					}

					//Check capacity of city route with city "p"

					k = solution->getFirstCustomer(route);
					capacity = solution->getPickupCapacity(0,route) + in->pickups[p] - in->pickups[city];
					if(capacity> in->capacity)
						valid = 0;

					capacity = solution->getDeliveryCapacity(0,route) + in->deliveries[p] - in->deliveries[city];

					if(capacity> in->capacity)
						valid = 0;

					while(k!=city && valid){
						capacity = solution->getDeliveryCapacity(k,route) + in->deliveries[p] + solution ->getPickupCapacity(k,route) - in->deliveries[city];
						if(capacity> in->capacity) {
							valid = 0;
							break;
						}
						k = solution -> getArc(route,k);
					}

					capacity = capacity - in->deliveries[p] + in->pickups[p];

					if(capacity> in->capacity)
						valid=0;

					k = solution -> getArc(route, k);

					while(k!=0 && valid){
						capacity = solution->getDeliveryCapacity(k,route) + in->pickups[p] + solution ->getPickupCapacity(k,route) - in->pickups[city];
						if(capacity> in->capacity){
							valid = 0;
							break;
						}
						k = solution -> getArc(route ,k);

					}


					if(valid){

						double real_dif = (real_costs[last][city] + real_costs[city][solution ->getArc(i,p)] - (real_costs[last][p] + real_costs[p][solution ->getArc(i,p)] )) - (real_costs[solution->getReverseArc(route, city)][city] + real_costs[city][solution->getArc(route, city)] - (real_costs[solution->getReverseArc(route, city)][p]+ real_costs[p][solution->getArc(route, city)]));

						if (solution->getTotalCost()+real_dif+ epsilon_parameter> best_solution) {
							Tabu_Row row(city,-1,-1,i,Tabu_Queue::swap);
							if (queue->find(row)) {
								last = p;
								p = solution->getArc(i,p);
								continue;
							}
						}
						mod = 0;
						*min1 = city;
						*min_cost = real_dif;
						*min2 = p;
						*min_route1 = route;
						*min_route2 = i;

					}

				}
				last = p;
				p = solution->getArc(i,p);
			}
		}
		return mod;

}


/*
 * Igual a check_crossover pero con los cambios necesarios para GTS
 */
int Operators::check_crossoverGTS(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs, double * real_acc_cost){
	double dif;
	int p, next, last;
	int mod, city_next;
	int i;
	int aux,aux2;
	double cost_i, cost_route;
	mod = -1;
	city_next = solution->getArc(route,city);

	for(i = route+1;i<solution->last_route;i++){
		if(route==i){
			continue;
		}
		if(solution ->getFirstCustomer(i)==0){
			continue;
		}
		last = 0;
		p = 0;
		next = solution->getArc(i,p);
		while(true){
			bool better = false;
			double real_cost_route;
			double real_cost_i;
			if((city==0) && (p==0)){
				last = p;
				p = next;
				next =  solution->getArc(i,p);
				continue;
			}
			if((city_next==0)&&(next==0)){
				break;
			}

			if (reduced_neighbourhood[city][next] == false ||
				reduced_neighbourhood[p][city_next] == false) {
					if(next==0){
						break;
					}
					last = p;
					p = next;
					next = solution->getArc(i,next);
					continue;
			}
			if(city==0){
				cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
				real_cost_i = real_acc_cost[p]+(solution->getRouteCost(route)-real_acc_cost[city_next]) + real_costs[p][city_next];

				if(next==0){
					cost_route = 0.0;
					real_cost_route = 0.0;
				}else{
					cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
					real_cost_route = real_acc_cost[city]+(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];
				}
			}else if(p==0){

				if(city_next==0){
					cost_i = 0.0;
					real_cost_i = 0.0;
				}else{
					cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p]+(solution->getRouteCost(route)-real_acc_cost[city_next]) + real_costs[p][city_next];
				}

				cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				real_cost_route = real_acc_cost[city]+(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];

			}else{

				if(city_next==0){
					cost_i = acc_cost[p] + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p] + real_costs[p][city_next];
				}else{
					cost_i = acc_cost[p]+(solution->getRouteCost(route)-acc_cost[city_next]) + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p]+(solution->getRouteCost(route)-real_acc_cost[city_next]) + real_costs[p][city_next];
				}
				if(next==0){
					cost_route = acc_cost[city] + in->cost[city][next];
					real_cost_route = real_acc_cost[city] + real_costs[city][next];
				}else{
					cost_route = acc_cost[city]+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
					real_cost_route = real_acc_cost[city]+(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];
				}

			}


			dif = cost_i + cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);

			if(dif + epsilon_parameter < *min_cost  && (dif > diff_parameter || dif < -1*diff_neg_parameter)){
				//solution->printRouteDetails(route);
				//solution->printRouteDetails(i);
				aux = solution->getReverseArc(i,0);
				aux2 = solution->getReverseArc(route,0);
				if(next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);

				}else if(city_next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(route, 0,aux);

				}else{

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}

				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

				if(solution->checkFeasibility(i) && solution->checkFeasibility(route)){
					better = true;
				}
				if(city==0 && next == 0){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else if((p==0) && (city_next==0)){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else{
				}

				if(next==0){

					aux = solution->getReverseArc(i,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(route, 0,aux);

				}else if(city_next==0){

					aux = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux);

				}
				else{
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}
				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

				if (better) {

					double real_dif = real_cost_i + real_cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);

					if (solution->getTotalCost()+real_dif+epsilon_parameter> best_solution) {
						Tabu_Row row(city,-1,-1,i,Tabu_Queue::interchange1,p,-1);
						if (queue->find(row)) {
							if(next==0){
								break;
							}
							last = p;
							p = next;
							next = solution->getArc(i,next);
							continue;
						}
					}
					mod=0;
					*min1 = city;
					*min_cost = real_dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}

			}
			if(next == 0){
				break;
			}

			last = p;
			p = next;
			next = solution->getArc(i,next);
		}
	}
	return mod;
}

/*
 * Igual a check_Interchange pero con los cambios necesarios para GTS
 */
int Operators::check_InterchangeGTS(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs, double * real_acc_cost){
	double dif;
	int p, next, last;
	int mod, city_next;
	int i;
	int aux,aux2;
	double cost_i, cost_route;
	mod = -1;
	city_next = solution->getArc(route,city);

	for(i = route+1;i<solution->last_route;i++){
		if(route==i){
			continue;
		}

		if(solution ->getFirstCustomer(i)==0){
			continue;
		}
		last = 0;
		p = 0;
		next = solution->getArc(i,p);
		while(true){

			bool better = false;
			double real_cost_route;
			double real_cost_i;

			if((city==0) && (p==0)){
				last = p;
				p = next;
				next =  solution->getArc(i,p);
				continue;
			}
			if((city_next==0)&&(next==0)){
				break;
			}

			if (	reduced_neighbourhood[city][next] == false ||
					reduced_neighbourhood[p][city_next] == false) {
				if(next==0){
					break;
				}
				last = p;
				p = next;
				next = solution->getArc(i,next);
				continue;
			}

			if(city==0){
				cost_i = acc_cost[p]+acc_cost[city_next] + in->cost[p][city_next];
				real_cost_i = real_acc_cost[p]+real_acc_cost[city_next]+real_costs[p][city_next];

				if(next==0){
					cost_route = 0.0;
					real_cost_route = 0.0;
				}else{
					cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
					real_cost_route = (solution->getRouteCost(route)-real_acc_cost[city])+(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];
				}
			}else if(p==0){

				if(city_next==0){
					cost_i = 0.0;
					real_cost_i = 0.0;
				}else{
					cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p]+(real_acc_cost[city_next]) + real_costs[p][city_next];
				}

				cost_route = (solution->getRouteCost(route)-acc_cost[city])+(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
				real_cost_route = (solution->getRouteCost(route)-real_acc_cost[city])+(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];

			}else{

				if(city_next==0){
					cost_i = acc_cost[p] + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p] + real_costs[p][city_next];
				}else{
					cost_i = acc_cost[p]+(acc_cost[city_next]) + in->cost[p][city_next];
					real_cost_i = real_acc_cost[p]+(real_acc_cost[city_next]) + real_costs[p][city_next];
				}
				if(next==0){
					cost_route = (solution->getRouteCost(route)-acc_cost[city]) + in->cost[city][next];
					real_cost_route = (solution->getRouteCost(route)-real_acc_cost[city]) + real_costs[city][next];
				}else{
					cost_route = (solution->getRouteCost(route)-acc_cost[city]) +(solution->getRouteCost(i)-acc_cost[next]) + in->cost[city][next];
					real_cost_route = (solution->getRouteCost(route)-real_acc_cost[city]) +(solution->getRouteCost(i)-real_acc_cost[next]) + real_costs[city][next];
				}

			}

			dif = cost_i + cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);

			if(dif + epsilon_parameter < *min_cost  && (dif > diff_parameter || dif < -1*diff_neg_parameter)){
				//solution->printRouteDetails(route);
				//solution->printRouteDetails(i);
				aux = solution->getReverseArc(i,0);
				aux2 = solution->getReverseArc(route,0);
				if(next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);

				}else if(city_next==0){

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(route, 0,aux);

				}else{

					solution->setArc(route,city,next);
					solution->setReverseArc(route,next,city);
					solution->setArc(i,p,city_next);
					solution->setReverseArc(i,city_next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}

				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

				if(solution->checkFeasibility(i) && solution->checkFeasibility(route)){
					better = true;
				}
				if(city==0 && next == 0){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else if((p==0) && (city_next==0)){
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
				}else{
				}

				if(next==0){

					aux = solution->getReverseArc(i,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(route, 0,aux);

				}else if(city_next==0){

					aux = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux);

				}
				else{
					aux = solution->getReverseArc(i,0);
					aux2 = solution->getReverseArc(route,0);
					solution->setArc(route,city,city_next);
					solution->setReverseArc(route,city_next,city);
					solution->setArc(i,p,next);
					solution->setReverseArc(i,next,p);
					solution ->setReverseArc(i,0,aux2);
					solution ->setReverseArc(route, 0,aux);

				}
				solution->updateFullRouteCapacity(i);
				solution->updateFullRouteCapacity(route);

				if (better) {

					double real_dif = real_cost_i + real_cost_route - solution->getRouteCost(i) - solution->getRouteCost(route);

					if (solution->getTotalCost()+real_dif+epsilon_parameter> best_solution) {
						Tabu_Row row(city,-1,-1,i,Tabu_Queue::interchange2,p,-1);
						if (queue->find(row)) {
							if(next==0){
								break;
							}
							last = p;
							p = next;
							next = solution->getArc(i,next);
							continue;
						}
					}
					mod=0;
					*min1 = city;
					*min_cost = real_dif;
					*min2 = p;
					*min_route1 = route;
					*min_route2 = i;
				}

			}
			if(next == 0){
				break;
			}

			last = p;
			p = next;
			next = solution->getArc(i,next);
		}
	}
	return mod;
}
