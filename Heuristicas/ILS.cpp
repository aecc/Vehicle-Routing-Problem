/*
 * ILS.cpp
 *
 *  Created on: 13/03/2011
 *      Author: rsantana
 */

#include "ILS.h"

using namespace std;
void ILS::vnd_intra(Routes* solution){
	int i,k;

	int rand;
    unsigned long init[1]={time(NULL)}, length=1;
    Random::init_by_array(init, length);
    bool cont =  true;
	for(i=0;i<= solution->last_route;i++){
		k=0;
		while(true){
			if(k==6){
				break;
			}
			rand = Random::genrand_int31()%3;
			if(rand == 0){
				cont = (Local_Search::ls_or_opt_one(i,solution));
			}
			if(rand==1){
				cont = (Local_Search::ls_or_opt_two(i,solution));

			}
			if(rand==2){
				cont = (Local_Search::ls_or_opt_three(i,solution));
			}
			if(cont){
				k=1;
			}else{
				cont = true;
				k++;
			}

		}
		while(Local_Search::ls_two_opt(i,solution));
		while(Local_Search::swapIntraRoute(i,solution));
		while(Local_Search::ls_reverse(i,solution));
	}

}

int ILS::vnd(Input* in, Routes* solution){

	int niter = 1;
	while(true){
		if(Local_Search::ls_shift_one(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}

		if(Local_Search::ls_crossover(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}


		if(Local_Search::ls_swap_one(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}

		if(Local_Search::ls_shift_two(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}

		if(Local_Search::ls_swap_two_one(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}

		if(Local_Search::ls_swap_two_two(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}
/*
		if(Local_Search::ls_interchange(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}
*/

		break;
	}
	return 1;
}

int ILS::vnd_fast(Input* in, Routes* solution){

	int niter = 1;

		while(Local_Search::ls_shift_one(niter,solution,in)==1);
		ILS::vnd_intra(solution);

		while(Local_Search::ls_crossover(niter,solution,in)==1);
		ILS::vnd_intra(solution);

		while(Local_Search::ls_swap_one(niter,solution,in)==1);
		ILS::vnd_intra(solution);

		while(Local_Search::ls_shift_two(niter,solution,in)==1);
		ILS::vnd_intra(solution);

		while(Local_Search::ls_swap_two_one(niter,solution,in)==1);
		ILS::vnd_intra(solution);

		while(Local_Search::ls_swap_two_two(niter,solution,in)==1);
		ILS::vnd_intra(solution);
/*
		if(Local_Search::ls_interchange(niter,solution,in)==1){
			ILS::vnd_intra(solution);
			continue;
		}
*/


	return 1;
}


void ILS::ils(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double  y){
	double solution_cost;
	double final_cost;
	double best_cost;
	Routes* best = new Routes(in,in->ncustomers);
	solution_cost = numeric_limits<double>::max( );
	final_cost = solution_cost;
	best_cost = solution_cost;
	int k=0;
	int i=0;

	for(;;){
		Routes* solution = Routes::createInitialRoutesInsertion(in,v,y);
		ILS::vnd_intra(solution);
		solution_cost=solution->getTotalCost();
		best->clone(solution);
		best_cost = solution_cost;
		i=0;
		while(i<nLS){
			ILS::vnd(in,solution);
			solution_cost = solution->getTotalCost();
			if(solution_cost + Local_Search::epsilon_parameter <best_cost){
				best->clone(solution);
				best_cost = solution->getTotalCost();
				i=0;
			}
			solution->clone(best);
			perturbation::perturbate(in,solution);
			i++;
		}


		if(best_cost + Local_Search::epsilon_parameter <final_cost){
			final_solution->clone(best);
			final_cost = best->getTotalCost();
		}
		k++;
		if(k==maxIter){
			break;
		}
		delete solution;
	}
}


void ILS::ils_GRASP(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double  y){
	double solution_cost;
	double final_cost;
	double best_cost;
	Routes* best = new Routes(in,in->ncustomers);
	solution_cost = numeric_limits<double>::max( );
	final_cost = solution_cost;
	best_cost = solution_cost;
	int k = 0, i = 0;

	for(;;){
		Routes* solution = Routes::createInitialRoutesGRASP(in,y,k);
		ILS::vnd_intra(solution);
		solution_cost=solution->getTotalCost();
		best->clone(solution);
		best_cost = solution_cost;
		i=0;
		while(i<nLS){
			ILS::vnd(in,solution);
			solution_cost = solution->getTotalCost();
			if(solution_cost + Local_Search::epsilon_parameter <best_cost){
				best->clone(solution);
				best_cost = solution_cost;
				i=0;
			}
			solution->clone(best);
			perturbation::perturbate(in,solution);
			i++;
		}

		delete solution;
		if(best_cost + Local_Search::epsilon_parameter <final_cost){
			final_solution->clone(best);
			final_cost = best_cost;
			k=0;
		}
		k++;
		if(k==maxIter){
			break;
		}
	}
}

void ILS::ils_best(int maxIter, int nLS, Input* in, Routes* final_solution, int v, double  y){
	double solution_cost;
	double final_cost;
	double best_cost;
	Routes* best = new Routes(in,in->ncustomers);
	solution_cost = numeric_limits<double>::max( );
	final_cost = solution_cost;
	best_cost = solution_cost;

	//delete solution;

	int k=0;
	int i=0;

	for(;;){
		Routes* solution = Routes::createInitialRoutesInsertionBest(in,v,y);
		ILS::vnd_intra(solution);
		solution_cost=solution->getTotalCost();
		best->clone(solution);
		best_cost = solution_cost;
		i=0;
		while(i<nLS){
			ILS::vnd(in,solution);



			solution_cost = solution->getTotalCost();
			if(solution_cost + Local_Search::epsilon_parameter <best_cost){
				best->clone(solution);
				best_cost = solution->getTotalCost();
				i=0;
			}
			/*
			else{

				if(i==(nLS-1)){
					if(Local_Search::ls_interchange(1,best,in)==1){
						ILS::vnd_intra(best);
						cout << "\t interchange!!!!! \n";
						i=0;

					}
					if(i==0){
						best_cost = best->getTotalCost();
					}
				}

			}
*/
			solution->clone(best);
			perturbation::perturbate(in,solution);
			i++;
		}


		if(best_cost + Local_Search::epsilon_parameter <final_cost){
			final_solution->clone(best);
			final_cost = best->getTotalCost();
		}
		k++;
		if(k==maxIter){
			break;
		}
		delete solution;
	}

	delete best;

}

