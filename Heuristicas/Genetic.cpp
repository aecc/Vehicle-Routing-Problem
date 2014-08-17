/*
 * Genetic.cpp
 *
 *  Created on: 23/04/2011
 *      Author: rsantana
 */

#include "Genetic.h"

using namespace std;

void Genetic::solution_to_array(Routes* solution, int* client){
	int i,j,k;
	k=0;
	for(i=0;i<solution->last_route;i++){
		j=solution->getFirstCustomer(i);
		while(j!=0){
			client[k] = j;
			k++;
			j=solution->getArc(i,j);
		}
	}
}

void Genetic::create_children(int ncust, int* cust, Routes* child){
	int last_route = 1, i =0 , j;
	bool visited[ncust+1] , all_serviced = false;
    int min_route, last;
    double dif, min_cost;

	child->reset();
	for(i=0;i<=ncust;i++){
		visited[i]=false;
	}

	for(i=0;i<ncust;i++){
		if(visited[cust[i]]){
			continue;
		}

		j=last_route-1;
		last = child->getReverseArc(j,0);

		child->setCompleteArc(j,last,cust[i]);
		child->setCompleteArc(j,cust[i],0);
		child->updateFullRouteCapacity(j);

		if(child->checkFeasibility(j)){
			visited[cust[i]] = true;
			continue;
		}

		child->setCompleteArc(j,last,0);
		child->updateFullRouteCapacity(j);

		min_cost = numeric_limits<double>::max( );
		min_route = -1;


		for(j=0;j<last_route-1;j++){

			last = child->getReverseArc(j,0);
			dif = child->costs[last][cust[i]] + child->costs[cust[i]][0];

			if(dif+0.001 < min_cost ){

				child->setCompleteArc(j,last,cust[i]);
				child->setCompleteArc(j,cust[i],0);
				child->updateFullRouteCapacity(j);

				if(child->checkFeasibility(j)){
					min_cost = dif;
				}

				child->setCompleteArc(j,last,0);
				child->updateFullRouteCapacity(j);
			}
		}
		if(min_route != -1){

			last = child->getReverseArc(min_route ,0);
			child->setCompleteArc(min_route,last,cust[i]);
			child->setCompleteArc(min_route,cust[i],0);
			child->updateFullRouteCapacity(min_route);
			visited[cust[i]] = true;

		}else{

			last = 0;
			child->setCompleteArc(last_route,last,cust[i]);
			child->setCompleteArc(last_route,cust[i],0);
			child->updateFullRouteCapacity(last_route);
			visited[cust[i]] = true;
			last_route++;

		}
	}

	for(i=0;i<last_route;i++){
		child->setRouteCost(i,child->calcTotalRouteCost(i));
	}

	all_serviced = true;
	for(i=1;i<=ncust;i++){
		if(!visited[i]){
			all_serviced = false;
			break;
		}
	}
	if(all_serviced){

		child->last_route = last_route;

	}else{

		child->last_route = ncust;

	}
	//child->printAllRoutes();
}

void Genetic::crossover(int ncust, int* p1, int* p2 , Routes* c1, Routes* c2){

	  unsigned long init[1]={time(NULL)}, length=1;
	  Random::init_by_array(init, length);
	  int rand1 = Random::genrand_int31()%ncust;
      int rand2 = (Random::genrand_int31()%(ncust - rand1 - 1))+rand1+1;
      int cust1[ncust],cust2[ncust];
      int i , j, k;
      bool visited1[ncust+1], visited2[ncust+1];

      for(j=0;j<ncust+1;j++){
    	  visited1[j]=false;
    	  visited2[j]=false;
      }

      i=0;
      for(j=rand1;j<=rand2;j++){
    	  cust1[i]=p1[j];
    	  visited1[p1[j]] = true;
    	  cust2[i]=p2[j];
    	  visited2[p2[j]] = true;
    	  i++;
      }

      k=i;
      for(j=0;j<ncust;j++){
    	  if(!visited1[p2[j]]){
    		  cust1[i]=p2[j];
    		  visited1[p2[j]]=true;
    		  i++;
    	  }
    	  if(!visited2[p1[j]]){
    		  cust2[k]=p1[j];
    		  visited2[p1[j]]=true;
    		  k++;
    	  }
      }

      Genetic::create_children(ncust,cust1,c1);
      Genetic::create_children(ncust,cust2,c2);

}

void Genetic::invert(int* p , Routes* child){

	int ncust = child->ncustomers;
	int rand1 = Random::genrand_int31()%ncust;
    int rand2 = (Random::genrand_int31()%(ncust - rand1 - 1))+rand1+1;
    reverse(p+rand1, p+rand2);
    Genetic::create_children(ncust,p,child);

}

void Genetic::mutation(int* p1 , int* p2 , Routes* child1, Routes* child2 , Input* in){

		Genetic::invert(p1,child1);

		perturbation::swaps(in,child2);



}

void Genetic::genetic(int max_gen, int pop_size , Input* in , Routes* solution){

	int i ,j , max;
	Routes** pop = new Routes *[pop_size+2];
	tuple** pop_fit = new tuple *[pop_size+2];
	int** pop_array = new int *[pop_size+2];
	int r_max = 0;
	max = pop_size/2;
	unsigned long init[1]={time(NULL)}, length=1;
	Random::init_by_array(init, length);
	int rand1 ;
    int rand2 ;
    int rand3 ;

	for(i=0;i<pop_size;i++){

		pop[i] = Routes::createInitialRoutesGRASP(in,0.5,i);
		ILS::vnd(in,pop[i]);
		pop_fit[i] = new tuple();
		pop_fit[i] -> id = i;
		pop_fit[i] -> cost = 1/pop[i]->getTotalCost();
		pop_array[i] = new int[in->ncustomers];
		Genetic::solution_to_array(pop[i],pop_array[i]);
		if(r_max<pop[i]->last_route){
			r_max = pop[i]->last_route;
		}

	}

	for(i=pop_size;i<pop_size+2;i++){

		pop[i] = new Routes(in,in->ncustomers);
		pop_fit[i] = new tuple();
		pop_fit[i] -> id = i;
		pop_fit[i] -> cost = 0.0;
		pop_array[i] = new int[in->ncustomers];

	}


	for(i=0;i<max_gen;i++){
		for(j=0;j<max;j++){
			sort(pop_fit,pop_fit+pop_size+2,sort_tuples2);

			rand3 = (Random::genrand_int31()%101);

			if(rand3 < 95){
				rand1 = (Random::genrand_int31()%pop_size)+2;
				rand2 = (Random::genrand_int31()%pop_size)+2;

				Genetic::crossover(in->ncustomers , pop_array[pop_fit[rand1]->id] , pop_array[pop_fit[rand2]->id], pop[pop_fit[0]->id] , pop[pop_fit[1]->id]);

				Genetic::solution_to_array(pop[pop_fit[0]->id],pop_array[pop_fit[0]->id]);
				Genetic::solution_to_array(pop[pop_fit[1]->id],pop_array[pop_fit[1]->id]);


				rand3 = (Random::genrand_int31()%101);
				if(rand3 <=1){
					Genetic::mutation(pop_array[pop_fit[0]->id],pop_array[pop_fit[1]->id],pop[pop_fit[0]->id],pop[pop_fit[1]->id],in);

					Genetic::solution_to_array(pop[pop_fit[0]->id],pop_array[pop_fit[0]->id]);
					Genetic::solution_to_array(pop[pop_fit[1]->id],pop_array[pop_fit[1]->id]);
				}

				if(pop[pop_fit[0]->id]->last_route > r_max){
					pop_fit[0]->cost = 0;
				}else{
					pop_fit[0]->cost = 1/(pop[pop_fit[0]->id]->getTotalCost());
				}

				if(pop[pop_fit[1]->id]->last_route > r_max){
					pop_fit[1]->cost = 0;
				}else{
					pop_fit[1]->cost = 1/(pop[pop_fit[1]->id]->getTotalCost());
				}
			}
		}
	}

	sort(pop_fit,pop_fit+pop_size+2,sort_tuples2);

	int min = -1;
	double min_cost = numeric_limits<double>::max( );
	for(i=2;i<pop_size+2;i++){
		if(pop[i]->last_route>r_max){
			continue;
		}
		ILS::vnd(in,pop[i]);
		if(min_cost > pop[i]->getTotalCost()+0.001){
			min = i;
			min_cost = pop[i]->getTotalCost();
		}

	}

	solution->clone(pop[min]);

	for(i=0;i<pop_size+1;i++){
		delete pop[i];
		delete pop_fit[i];
		delete pop_array[i];
	}

	delete [] pop;
	delete [] pop_fit;
	delete [] pop_array;

}
