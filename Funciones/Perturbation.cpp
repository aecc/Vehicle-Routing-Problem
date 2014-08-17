/*
 * Perturbation.cpp
 *
 *  Created on: 27/02/2011
 *      Author: rsantana
 */

#include "Routes.h"
#include "Input.h"
#include "Operators.h"
#include "Perturbation.h"
#include "Random.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Local_Search.h"
#include <vector>
#include <algorithm>
using namespace std;


int perturbation::ejection_chain(Input *in, Routes *solution){

	if(solution->last_route>12){
		return 0;
	}
	int aux;
	int min;
	int pos;
	int min_route;
	double min_cost;
	int mod = -1;
	int valid;
	double capacity;
	int city;
	int route;
	//Iterators
	int i;
	int p;
	int k;
	int j;
	int size;
	//City before the "p" city
	int last = 0;
	//Added Cost with shift in the current position
	double dif = 0.0;
	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);
	for(j=0;j<solution->last_route;j++){
		i=(j+1)%solution->last_route;
		while(perturbation::route_size(in,solution,i)==1){
			i = (i+1)%solution->last_route;
		}
		route = j;
		min=0;
		min_cost = 90000000000.0;
		pos = 0;
		min_route = 0;
		size = perturbation::route_size(in,solution,j);
		if(size==1){
			continue;
		}
		aux = (Random::genrand_int31()%(size-1))+1;
		city=solution->getFirstCustomer(j);
		for(int z = 1;z< aux;z++){
			city = solution->getArc(j,city);
		}
		last = 0;
		p= solution->getFirstCustomer(i);
		while(true){
			dif = (in->cost[last][city] + in->cost[city][p] - in->cost[last][p]);
			dif = dif - (in->cost[solution->getReverseArc(route,city)][city] + in->cost[city][solution->getArc(route,city)] - in->cost[solution->getReverseArc(route,city)][solution->getArc(route,city)]);

			if(dif<min_cost){

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
					min = city;
					min_cost = dif;
					pos = p;
					min_route = i;
				}
			}
			if(p==0)
				break;
			last = p;
			p = solution->getArc(i,p);
		}
		if(min!=0){
			Operators::shift_one(min,route,pos,min_route,solution,in);

		}
	}
	return 0;
}

int perturbation::route_size(Input *in, Routes *solution, int route){
	int city;
	int res = 0;
	city = solution->getFirstCustomer(route);
	while(city!=0){
		res++;
		city = solution->getArc(route,city);
	}
	if(res == 0){
		return 1;
	}
	return res+1;
}

void perturbation::Double_Bridge_Exec(Input *in, Routes *solution, int route){
	int size = perturbation::route_size(in, solution, route);
	int arcs[4];
	int prev[4];
	int pos[4];
	int last;
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);
    int i,j;

    if(size<8){
    	return;
    }


	int rand;
    while(true){
    	rand = ((Random::genrand_int31()%(size))+1);
		arcs[0]=rand;
    	rand = ((Random::genrand_int31()%(size))+1);
		arcs[1]=rand;
    	rand = ((Random::genrand_int31()%(size))+1);
		arcs[2]=rand;
    	rand = ((Random::genrand_int31()%(size))+1);
		arcs[3]=rand;

		sort(arcs,arcs+4);

		if((arcs[0]==arcs[1]) || ((arcs[0]+1)==arcs[1]) ){
			continue;
		}
		if((arcs[1]==arcs[2]) || ((arcs[1]+1)==arcs[2]) ){
			continue;
		}
		if((arcs[2]==arcs[3]) || ((arcs[2]+1)==arcs[3]) ){
			continue;		}
		if(((arcs[1]+1)%size)==arcs[0]){
			continue;		}
		if(arcs[3]==0){
			continue;		}
		break;
    }
    int prueba[4];
    prueba[0]=arcs[0];
    prueba[1]=arcs[1];
    prueba[2]=arcs[2];
    prueba[3]=arcs[3];

    i= solution->getFirstCustomer(route);
    j=1;
    last=0;


    while(true){
    	if(arcs[0]==j){
    		prev[0]=last;
    		pos[0]=i;
    	}else if(arcs[1]==j){
    		prev[1]=last;
    		pos[1]=i;
    	}
    	if(arcs[2]==j){
    		prev[2]=last;
    		pos[2]=i;
    	}else if(arcs[3]==j){
    		prev[3]=last;
    		pos[3]=i;
    	}
    	if(i==0){
    		break;
    	}
    	last = i;
    	i=solution->getArc(route,i);
    	j++;
    }

    solution->setArc(route,prev[0],pos[2]);
    solution->setReverseArc(route,pos[2],prev[0]);
    solution->setArc(route,prev[2],pos[0]);
    solution->setReverseArc(route,pos[0],prev[2]);
    solution->setArc(route,prev[1],pos[3]);
    solution->setReverseArc(route,pos[3],prev[1]);
    solution->setArc(route,prev[3],pos[1]);
    solution->setReverseArc(route,pos[1],prev[3]);
    solution ->updateFullRouteCapacity(route);
    if(solution->checkFeasibility(route)){
    	double new_cost = solution->calcTotalRouteCost(route);
    	solution->setRouteCost(route,new_cost);
    	return;
    }
    solution->setArc(route,prev[1],pos[1]);
    solution->setReverseArc(route,pos[1],prev[1]);
    solution->setArc(route,prev[0],pos[0]);
    solution->setReverseArc(route,pos[0],prev[0]);
    solution->setArc(route,prev[2],pos[2]);
    solution->setReverseArc(route,pos[2],prev[2]);
    solution->setArc(route,prev[3],pos[3]);
    solution->setReverseArc(route,pos[3],prev[3]);
    solution ->updateFullRouteCapacity(route);

}
int perturbation::Double_Bridge(Input *in, Routes *solution){
	int i;
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);
    int rand;

    if(solution->last_route < 5){
    	for(i=0;i<solution->last_route;i++){
    		perturbation::Double_Bridge_Exec(in,solution, i);
    	}
    }else{
    	i=0;
    	for(i=0;i<5;i++){
    		rand = Random::genrand_int31();
    		perturbation::Double_Bridge_Exec(in,solution, (rand)%(solution->last_route));
    	}
    }
    return 0;
}

void perturbation::swaps(Input* in, Routes *solution){
	int cust_routes[solution->ncustomers+1];
	int i,j,p,t;
	int k,city;
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);
    int rand;
    int cust[2];
    int next1;
    int prev1;
    int next2;
    int prev2;
    int route;
    double diff_cost;
    int valid;
    double capacity;

	for(i=0;i< solution->last_route;i++){
		j = solution->getFirstCustomer(i);
		while(j!=0){
			cust_routes[j]=i;
			j=solution->getArc(i,j);
		}
	}
	t=0;
	while(t!=2){
		rand = Random::genrand_int31();
		cust[0] = ((rand) % in->ncustomers)+1;
		rand = Random::genrand_int31();
		cust[1] = ((rand) % in->ncustomers)+1;
		if(cust[0]==cust[1]){
			continue;
		}
		if(cust_routes[cust[0]]==cust_routes[cust[1]]){
			route = cust_routes[cust[1]];
			p = solution->getFirstCustomer(route);
			while(p!=0){
				if(p==cust[0]){
					i=cust[0];
					j=cust[1];
					break;
				}
				if(p==cust[1]){
					i=cust[1];
					j=cust[0];
					break;
				}
				p=solution->getArc(route,p);
			}

			next1 = solution->getArc(route, i);
			prev1 = solution->getReverseArc(route, i);
			next2 = solution->getArc(route, j);
			prev2 = solution->getReverseArc(route, j);

			//solution->printRouteDetails(route);
			if (next1==j) {
				diff_cost =  - solution->costs[prev1][i] -
							   solution->costs[i][next1] - solution->costs[j][next2]
							+ solution->costs[prev1][j] + solution->costs[prev2][i]
							+ solution->costs[i][next2] + solution->costs[j][next1]
							+ solution->costs[j][i];

				solution->setArc(route,prev1,j);
				solution->setArc(route,j,i);
				solution->setArc(route,i,next2);
				solution->setReverseArc(route,j,prev1);
				solution->setReverseArc(route,i,j);
				solution->setReverseArc(route,next2,i);

			} else {
				diff_cost =  - solution->costs[prev1][i] -
									solution->costs[i][next1] - solution->costs[prev2][j] -
									solution->costs[j][next2] + solution->costs[prev1][j] +
									solution->costs[prev2][i] + solution->costs[i][next2] +
									solution->costs[j][next1];

				solution->setArc(route,prev1,j);
				solution->setArc(route,prev2,i);
				solution->setArc(route,i,next2);
				solution->setArc(route,j,next1);
				solution->setReverseArc(route,i,prev2);
				solution->setReverseArc(route,j,prev1);
				solution->setReverseArc(route,next2,i);
				solution->setReverseArc(route,next1,j);
			}
			//solution->printRouteDetails(route);
			//routes->printRouteDetails(route);
			//printf("Entre. i:%d j:%d\n",i,j);
			solution->updateFullRouteCapacity(route);
			if (solution->checkFeasibility(route)) {
				solution->setRouteCost(route,solution->getRouteCost(route)+diff_cost);
				t++;
				int z = cust_routes[j];
				cust_routes[j] = cust_routes[i];
				cust_routes[i] = z;
				continue;
			}

			if (next1==j) {

				solution->setArc(route,prev1,i);
				solution->setArc(route,i,j);
				solution->setArc(route,j,next2);
				solution->setReverseArc(route,i,prev1);
				solution->setReverseArc(route,j,i);
				solution->setReverseArc(route,next2,j);

			} else {

				solution->setArc(route,prev1,i);
				solution->setArc(route,prev2,j);
				solution->setArc(route,j,next2);
				solution->setArc(route,i,next1);
				solution->setReverseArc(route,j,prev2);
				solution->setReverseArc(route,i,prev1);
				solution->setReverseArc(route,next2,j);
				solution->setReverseArc(route,next1,i);
			}
			//solution->printRouteDetails(route);
			solution->updateFullRouteCapacity(route);
		}else{

			// Check insertion at route i
			city = cust[0];
			p = cust[1];
			route = cust_routes[cust[0]];
			i = cust_routes[cust[1]];
			//solution->printRouteDetails(i);
			//solution->printRouteDetails(route);
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
				Operators::swap_one(city,route,p,i,solution,in);
				t++;
				int z = cust_routes[p];
				cust_routes[p] = cust_routes[city];
				cust_routes[city] = z;
			}
		}
	}
}

void perturbation::perturbate(Input* in, Routes* solution){
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);
    init[0]= time(NULL) + Random::genrand_int31();
    Random::init_by_array(init, length);
    int rand = Random::genrand_int31();

    int i = (rand % 3);

    if(i==0){
    	perturbation::Double_Bridge(in, solution);
    }else if(i==1){
    	//cout << "ejec chain\n";
    	perturbation::ejection_chain(in, solution);
    }else if(i==2){
    	//cout << "swaps\n";
    	perturbation::swaps(in,solution);
    }

}
