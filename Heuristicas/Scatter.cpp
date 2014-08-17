/*
 * Scatter.cpp
 *
 *  Created on: 12/04/2011
 *      Author: rsantana
 */

#include "Scatter.h"
using namespace std;
Routes** Scatter::Diversification_Generation(int Psize,int b,Input* in, double y){
	int i;
	Routes** P = new Routes *[Psize+b];
	for(i=0;i<Psize;i++){
		P[i] = Routes::createInitialRoutesGRASP(in,y,i);
		//cout << "i: " << i << " cost " << P[i]->getTotalCost();
	}
	for(i = Psize ;i<Psize+b;i++){
		P[i]= new Routes(in,in->ncustomers);
	}
	return P;
}

void Scatter::Improvement_Method(int Psize,int max, Input* in, Routes** P){
	int i;
	int nIter = 1;
	for(i=0;i<max;i++){
		while(true){
			if(Local_Search::ls_shift_one(nIter,P[i],in)==1)
				continue;
			if(Local_Search::ls_swap_one(nIter,P[i],in)==1)
				continue;
			if(Local_Search::ls_crossover(nIter,P[i],in)==1)
				continue;
			for(int j = 0; j<P[i]->last_route;j++){
				while(Local_Search::ls_or_opt_one(j,P[i]));
				while(Local_Search::swapIntraRoute(j,P[i]));
				while(Local_Search::ls_two_opt(j,P[i]));
			}
			break;
		}
	}

}

double Scatter::CalcRoutesDist(Routes* routes , Routes* x){
	int i,j,k;
	double acc;
	int r;
	if(routes->last_route>= x->last_route){
		r = routes->last_route;
	}else{
		r = x->last_route;
	}
	double ClientCount[r][r];
	bool row_taken[r];
	bool column_taken[r];
	double comp[routes->ncustomers+1];
	for(i=0;i<r;i++){
		row_taken[i]=false;
		column_taken[i]=false;
		for(j=0;j<r;j++){
			for(k=1;k<=routes->ncustomers;k++){
				comp[k] = 0.0;
			}
			if((routes->last_route-1<i)||(x->last_route-1<j)){
				ClientCount[i][j]=0.0;
				continue;
			}
			k=routes->getFirstCustomer(i);
			while(k!=0){
				comp[k]+=1.0;
				k = routes->getArc(i,k);
			}
			k = x->getFirstCustomer(j);
			while(k!=0){
				comp[k]+=1.0;
				k = x->getArc(j,k);
			}
			acc = 0.0;
			for(k=1;k<=routes->ncustomers;k++){
				if(comp[k]==2){
					acc+=1.0;
				}
				comp[k]=0;
			}
			ClientCount[i][j] = acc;

		}
	}

/*
	for(i=0;i<r;i++){
		cout<<endl;
		for(j=0;j<r;j++){
			cout<<" "<<ClientCount[i][j]<<" ";
		}
	}
*/


	double max;
	int max_i,max_j;
	acc = 0.0;
	while(true){
		max=-1.0;
		for(i=0;i<r;i++){
			if( (!(row_taken[i])) || (!(column_taken[i])) ){
				break;
			}
		}
		if(i==r){
			break;
		}
		for(i=0;i<r;i++){
			if(row_taken[i]){
				continue;
			}
			for(j=0;j<r;j++){
				if(column_taken[j]){
					continue;
				}
				if(ClientCount[i][j]>max){
					max = ClientCount[i][j];
					max_i = i;
					max_j = j;
				}
			}
		}
		acc += max;
		row_taken[max_i]=true;
		column_taken[max_j]=true;
	}



	return (((double)routes->ncustomers) - acc);
}

bool Scatter::Refset_Update(int Psize,int max, Routes** P, tuple** Pdistance, tuple** Pcosts, int b, Routes** Refset, tuple** RefsetVal, bool* new_sol){
	int i,j,b2,b1;
	double acc;
	bool upd = false;
	b2=b/2;
	b1 = b - b2;


	for(i=0;i<max;i++){
		Pcosts[i] -> id = i ;
		Pcosts[i] -> cost = P[i]->getTotalCost();
		Pdistance[i] -> id = i;
		Pdistance[i] -> cost = 0.0;
	}
	for(i=max;i<Psize;i++){
		Pcosts[i] -> id = i ;
		Pcosts[i] -> cost = numeric_limits<double>::max();
		Pdistance[i] -> id = i;
		Pdistance[i] -> cost = 0.0;
	}


	j=0;
	for(i=Psize;i<Psize+b1;i++){
		new_sol[j]=false;
		Pcosts[i] -> id = i ;
		Pcosts[i] -> cost = Refset[RefsetVal[j]->id]->getTotalCost();
		if(Refset[RefsetVal[j]->id]->last_route==0){
			Pcosts[i] -> cost = numeric_limits<double>::max();
		}
		P[i] ->clone(Refset[RefsetVal[j]->id]);
		j++;
	}
	j=b1;
	for(i=Psize+b1;i<Psize+b;i++){
		new_sol[j]=false;
		Pdistance[i] -> id = i ;
		Pdistance[i] -> cost = RefsetVal[j]->cost;
		if(Refset[RefsetVal[j]->id]->last_route == 0){
			Pdistance[i] -> cost = 0.0;
		}
		P[i] ->clone(Refset[RefsetVal[j]->id]);
		j++;
	}

	sort(Pcosts,Pcosts+Psize+b1, sort_tuples);
	/*

	for(i=0;i<Psize;i++){
		cout << "Pcost-id: " << Pcosts[i]->id << " Pcost: " << Pcosts[i] ->cost << endl;
	}
	for(i=0;i<b1;i++){
		cout << "refval - id: " << RefsetVal[i]->id << " refcost: " << RefsetVal[i] ->cost << endl;
	}

*/
	for(i=0;i<b1;i++){
		Refset[RefsetVal[i]->id]->clone(P[Pcosts[i]->id]);
		RefsetVal[i]->cost = Pcosts[i]->cost;
		if(Pcosts[i]->id<Psize){
			P[Pcosts[i]->id]->last_route = 0;
			upd = true;
			new_sol[RefsetVal[i]->id]=true;
		}
	}
	sort(RefsetVal,RefsetVal+b1, sort_tuples);
	/*


	for(i=0;i<b1;i++){
		cout << "refval - id: " << RefsetVal[i]->id << " refcost: " << RefsetVal[i] ->cost << endl;
	}
 */
	for(i=0;i<max;i++){
		if(P[i]->last_route==0){
			Pdistance[i]->cost = 0.0;
			continue;
		}
		acc=0.0;
		for(j=0;j<b;j++){
			if(Refset[j]->last_route==0){
				continue;
			}
			acc += Scatter::CalcRoutesDist(P[i],Refset[j]);
		}
		Pdistance[i]->cost = acc;
	}
	sort(Pdistance,Pdistance+Psize+b, sort_tuples2);
	/*
	for(i=0;i<Psize+b;i++){
		cout << "Pdistance: " << Pdistance[i]->id << " Pdiscost: " << Pdistance[i] ->cost << endl;
	}
	for(i=b1;i<b;i++){
			cout << "Refval id: " << RefsetVal[i]->id << " refvalcost: " << RefsetVal[i] ->cost << endl;
		}

	 * */
	j = Psize+b-1;
	for(i=(b1);i<b;i++){

		Refset[RefsetVal[i]->id]->clone(P[Pdistance[j]->id]);
		RefsetVal[i]->cost = Pdistance[j]->cost;
		if(Pdistance[j]->id<Psize){
			P[Pdistance[j]->id]->last_route = 0;
			upd = true;
			new_sol[RefsetVal[i]->id]=true;
		}
		j--;
	}
	sort(RefsetVal+b1,RefsetVal+b, sort_tuples);
	/*
	for(i=b1;i<b;i++){
		cout << "Refval id: " << RefsetVal[i]->id << " refvalcost: " << RefsetVal[i] ->cost << endl;
	}

	*/

	return upd;
}

void Scatter::Cross_Routes(Input * in, Routes* rl ,Routes* x,Routes* routes){
	int i_best, route_best;
	double min_cost, diff_cost;
	int last,last_route;
	bool * visited = new bool[routes->ncustomers+1];
	bool all_serviced = false;
	int i,j,k;
	int r;

	for (i=0; i<routes->ncustomers+1;i++)
		visited[i] = false;
	visited[0] = true;



	if(rl->last_route>= x->last_route){
		r = rl->last_route;
	}else{
		r = x->last_route;
	}
	last_route=0;
	int comp[rl->ncustomers+1];
	for(i=0;i<r;i++){
		for(k=1;k<=rl->ncustomers;k++){
			comp[k] = 0;
		}
		if((rl->last_route-1<i)||(x->last_route-1<i)){
			continue;
		}
		k=x->getFirstCustomer(i);
		while(k!=0){
			comp[k]+=1;
			k = x->getArc(i,k);
		}
		last=0;
		k = rl->getFirstCustomer(i);
		while(k!=0){
			if(comp[k]){
				routes->setCompleteArc(last_route,last,k);
				last = k;
				visited[k]=true;
			}
			k = rl->getArc(i,k);
		}
		if(last!=0){
			routes->setCompleteArc(last_route,last,0);
			last_route++;
		}


	}
	routes->last_route = last_route;

	//routes->printAllRoutes();
	//Insertion
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
					diff_cost = routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k];
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
			routes->last_route++;
			routes->setCompleteArc(routes->last_route,0,0);
		}


	}

	delete [] visited;
}


void Scatter::Cross_Routes_best(Input * in, Routes* rl ,Routes* x,Routes* routes){
	int i_best, route_best;
	double min_cost, diff_cost;
	int last,last_route;
	bool * visited = new bool[routes->ncustomers+1];
	bool all_serviced = false;
	int i,j,k;
	int r;

	for (i=0; i<routes->ncustomers+1;i++)
		visited[i] = false;
	visited[0] = true;



	if(rl->last_route>= x->last_route){
		r = rl->last_route;
	}else{
		r = x->last_route;
	}
	last_route=0;
	int comp[rl->ncustomers+1];
	for(i=0;i<r;i++){
		for(k=1;k<=rl->ncustomers;k++){
			comp[k] = 0;
		}
		if((rl->last_route-1<i)||(x->last_route-1<i)){
			continue;
		}
		k=x->getFirstCustomer(i);
		while(k!=0){
			comp[k]+=1;
			k = x->getArc(i,k);
		}
		last=0;
		k = rl->getFirstCustomer(i);
		while(k!=0){
			if(comp[k]){
				routes->setCompleteArc(last_route,last,k);
				last = k;
				visited[k]=true;
			}
			k = rl->getArc(i,k);
		}
		if(last!=0){
			routes->setCompleteArc(last_route,last,0);
			last_route++;
		}


	}
	routes->last_route = last_route;

	//routes->printAllRoutes();
	//Insertion
	for(i=0;i<routes->last_route;i++){
		//		cout << "randi: " << routes ->getRouteCost(i) << "\n";
		routes->setRouteCost(i,routes ->calcTotalRouteCost(i));
		routes->updateFullRouteCapacity(i);
		//		cout << "randf: " << routes ->getRouteCost(i) << "\n";
	}

	while(true){

		for(int t=1;t<routes->nroutes+1;t++){
			int i_min = -1;
			int i_best=-1;
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
						diff_cost = routes->costs[last][i] + routes->costs[i][k] -routes->costs[last][k];
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
			routes->last_route++;
			routes->setCompleteArc(routes->last_route,0,0);
		}


	}

	delete [] visited;
}


void Scatter::Combination_Method(Input * in, int Psize,int* max, Routes** P,int b, Routes** Refset, bool* new_sol){

	int i,j;
	*max = 0;
	Routes* aux = new Routes(in,in->ncustomers);
	for(i=0;i<b;i++){
		for(j=i+1;j<b;j++){
			if(new_sol[i]||new_sol[j]){
				Scatter::Cross_Routes_best(in,Refset[i],Refset[j],aux);
				P[(*max)]->clone(aux);
				aux->reset();
				(*max)++;
			}
		}
	}
	delete aux;
}



void Scatter::scatter(int Psize , int b, double y, Routes* Solution, Input* in){
	Routes** P;
	Routes** Refset = new Routes* [b];
	tuple **Pdistance = new tuple *[Psize+b];
	tuple **Pcosts = new tuple *[Psize+b];
	tuple **RefsetVal =  new tuple *[b];
	bool NewSolutions = true;
	bool* new_sol = new bool[b];
	int i, max;

	if(in->ncustomers > 200){
		Psize = (b*(b-1))/2;
	}
	if(Psize < (b*(b-1))/2){
		Psize = (b*(b-1))/2;
	}

	max = Psize;
	for(i=0;i<(b-(b/2));i++){
		RefsetVal[i] = new tuple();
		RefsetVal[i] -> id = i;
		RefsetVal[i] ->cost = numeric_limits<double>::max();
		Refset[i] = new Routes(in,in->ncustomers);
		new_sol[i] = false;
	}
	for(i=(b-(b/2));i<b;i++){
		RefsetVal[i] = new tuple();
		RefsetVal[i] -> id = i;
		RefsetVal[i] ->cost = 0.0;
		Refset[i] = new Routes(in,in->ncustomers);
		new_sol[i] = false;
	}
	for(i=0;i<Psize+b;i++){
		Pdistance[i] = new tuple();
		Pcosts[i] = new tuple();
	}

	P = Scatter::Diversification_Generation(Psize,b,in,y);
	Scatter::Improvement_Method(Psize,max ,in,P);
	NewSolutions = Scatter::Refset_Update(Psize,max,P,Pdistance,Pcosts,b,Refset,RefsetVal,new_sol);

	while(NewSolutions){

		NewSolutions = false;
		Scatter::Combination_Method(in,Psize,&max,P,b,Refset,new_sol);
		Scatter::Improvement_Method(Psize,max ,in,P);
		NewSolutions = Scatter::Refset_Update(Psize,max,P,Pdistance,Pcosts,b,Refset,RefsetVal,new_sol);

	}

	int min = 0;
	ILS::vnd(in,Refset[0]);
	double min_cost = Refset[0]->getTotalCost();
	for(i=1;i<b;i++){
		ILS::vnd(in,Refset[i]);
		if(min_cost>Refset[i]->getTotalCost()+0.001){
			min = i;
			min_cost = Refset[i]->getTotalCost();
		}
	}

	Solution->clone(Refset[min]);


	for(i=0;i<b;i++){
		delete Refset[i];
		delete RefsetVal[i];
	}

	for(i=0;i<Psize+b;i++){
		delete P[i];
		delete Pcosts[i];
		delete Pdistance[i];
	}

	delete [] Refset;
	delete [] RefsetVal;
	delete [] new_sol;
	delete [] P;
	delete [] Pcosts;
	delete [] Pdistance;

}
