/*
 * Main.cpp
 *
 *  Created on: 03/03/2011
 *      Author: aecc
 */

#include "Routes.h"
#include "Operators.h"
#include "Input.h"
#include "Local_Search.h"
#include "IO.h"
#include <iostream>
#include <stdio.h>
#include "ACO.h"
#include "GTS.h"
#include "PSO.h"
#include "ILS.h"
#include "Scatter.h"
#include "Genetic.h"
#include <string.h>
using namespace std;

void showUse(char* program);

int main(int argc, char *argv[]) {

	char heuristic[100];
	char file_name[100];
	char file_type[100];
	double alpha, beta, ro, q, lambda1, lambda2, w1, wt , y;
	int n_iter, mni, tabu, L, nroutes, cp, cg, cl, cn, K, nLS, b ,p;
	bool h, n, f, al, be, q0, ro0, ft, l1, l2, m, t, L0, nroutes0, cp0, cg0, cl0, cn0;
	bool w10, wt0, K0, info, LS , yf, b1, p1;
	h = n = f = al = be = q0 = ro0 = ft = l1 = l2 = m = t = L0 = nroutes0 = false;
	cp0 = cg0 = cl0 = cn0 = w10 = wt0 = K0 = info = LS = yf = b1 = p1= false;

	for (int i = 1; i < argc; i += 2) {

		if (argv[i] == string("--info")) {
			info = true;
		} else if (argv[i] == string("-h")) {
			strcpy(heuristic,argv[i+1]);
			h = true;
		} else if (argv[i] == string("-LS")) {
			nLS =  atoi(argv[i+1]);
			LS = true;
		}  else if (argv[i] == string("-p")) {
			p =  atoi(argv[i+1]);
			p1 = true;
		} else if (argv[i] == string("-b")) {
			b =  atoi(argv[i+1]);
			b1 = true;

		} else if(argv[i] == string("-y")) {
			y = strtod(argv[i+1],NULL);
			yf = true;
		} else if(argv[i] == string("-n")) {
			n_iter = atoi(argv[i+1]);
			n = true;
		} else if(argv[i] == string("-cp")) {
			cp = atoi(argv[i+1]);
			cp0 = true;
		} else if(argv[i] == string("-cg")) {
			cg = atoi(argv[i+1]);
			cg0 = true;
		} else if(argv[i] == string("-cl")) {
			cl = atoi(argv[i+1]);
			cl0 = true;
		} else if(argv[i] == string("-cn")) {
			cn = atoi(argv[i+1]);
			cn0 = true;
		} else if(argv[i] == string("-L")) {
			L = atoi(argv[i+1]);
			L0 = true;
		} else if(argv[i] == string("-K")) {
			K = atoi(argv[i+1]);
			K0 = true;
		} else if(argv[i] == string("-nroutes")) {
			nroutes = atoi(argv[i+1]);
			nroutes0 = true;
		} else if(argv[i] == string("-mni")) {
			mni = atoi(argv[i+1]);
			m = true;
		} else if(argv[i] == string("-w1")) {
			w1 = strtod(argv[i+1],NULL);
			w10 = true;
		} else if(argv[i] == string("-wt")) {
			wt = strtod(argv[i+1],NULL);
			wt0 = true;
		} else if(argv[i] == string("-lambda1")) {
			lambda1 = strtod(argv[i+1],NULL);
			l1 = true;
		} else if(argv[i] == string("-lambda2")) {
			lambda2 = strtod(argv[i+1],NULL);
			l2 = true;
		} else if(argv[i] == string("-tabu")) {
			tabu = atoi(argv[i+1]);
			t = true;
		} else if(argv[i] == string("-f")) {
			strcpy(file_name,argv[i+1]);
			f = true;
		} else if(argv[i] == string("-ft")) {
			strcpy(file_type,argv[i+1]);
			ft = true;
		} else if(argv[i] == string("-alpha")) {
			alpha = strtod(argv[i+1],NULL);
			al = true;
		} else if(argv[i] == string("-beta")) {
			beta = strtod(argv[i+1],NULL);
			be = true;
		} else if(argv[i] == string("-q")) {
			q = strtod(argv[i+1],NULL);
			q0 = true;
		} else if(argv[i] == string("-ro")) {
			ro = strtod(argv[i+1],NULL);
			ro0 = true;
		} else {
			fprintf(stderr, "Error: Argumento inválido: %s %s\n", argv[i], argv[i+1]);
			fprintf(stderr, "       No existe opción %s\n", argv[i]);
			showUse(argv[0]);
			return 1;
		}
	}

	if (!f || !ft) {
		fprintf(stderr, "Error: Faltan argumentos por definir\n");
		showUse(argv[0]);
		return 1;
	}

	Input * in;

	if (file_type == string("Dethloff")) {
		in = IO::readFileDethloff(file_name);
	} else if (file_type == string("SalhiNagy")) {
		in = IO::readFileSalhiNagy(file_name);
	} else if (file_type == string("Min")) {
		//TODO
		return 0;
	} else {
		fprintf(stderr, "Error: Tipo de problema desconocido\n");
		showUse(argv[0]);
		return 1;
	}

	if (heuristic == string("ACO")) {
		//-f vrpnc11y.txt -ft SalhiNagy -h ACO -n 20 -alpha 1.0 -beta 3.0 -q 0.8 -ro 0.015
		if (!al || !be || !ro0 || !q0 || !n) {
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}
		double average = 0.0;
		double best_all = numeric_limits<double>::max() -1.0;
		int it = 1;
		Routes * best_routes = new Routes(in,in->ncustomers);
		for (int i=0; i<it; i++) {
			ACO * aco = new ACO(in,alpha,beta,q,ro);
			if (info) {
				aco->enableShowInfo();
			}
			Routes * routes = aco->runHeuristic(n_iter);
			double best_result = routes->getTotalCost();
			if (best_result < best_all) {
				best_all = best_result;
				best_routes->clone(routes);
			}
			printf("La mejor solucion obtenida mediante ACO fue de: %f\n",best_result);
			average += best_result;
		}
		average /=it;
		printf("\n");
		if (info) {
			best_routes->printAllRoutes();
		}
		//printf("\nEl costo promedio en %d iteraciones de ACO fue de: %f\n",it,average);
		//printf("El menor costo obtenido en %d iteraciones de ACO fue de: %f\n",it,best_all);


	} else if (heuristic == string("GTS")) {
		//-f vrpnc11y.txt -ft SalhiNagy -h GTS -mni 6000 -lambda1 0.05 -lambda2 0.05 -tabu 30
		if (!t || !l1 || !l2 || !m) {
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}
		Routes * best_routes = new Routes(in,in->ncustomers);
		double average = 0.0;
		double best_all = numeric_limits<double>::max() -1.0;
		int it = 1;
		for (int i=0; i<it; i++) {
			GTS * gts = new GTS(in,lambda1,lambda2,tabu);
			if (info) {
				gts->enableShowInfo();
			}
			Routes * routes = gts->runHeuristic(mni);
			double best_result = routes->getTotalCost();
			if (best_result < best_all) {
				best_all = best_result;
				best_routes->clone(routes);
			}
			printf("La mejor solucion obtenida mediante GTS fue de: %f\n",best_result);
			average += best_result;
		}
		average /=it;
		printf("\n");
		if (info) {
			best_routes->printAllRoutes();
		}
		//printf("\nEl costo promedio en %d iteraciones de GTS fue de: %f\n",it,average);
		//printf("El menor costo obtenido en %d iteraciones de GTS fue de: %f\n",it,best_all);

	} else if (heuristic == string("PSO")) {
		//-f vrpnc11y.txt -ft SalhiNagy -h PSO -n 30 -L 50 -nroutes 4 -cp 1 -cg 0 -cl 1 -cn 2 -w1 0.9 -wt 0.4 -K 5
		if (!n || !L0 || !nroutes0 || !cp0 || !cg0 || !cl0 || !cn0 || !w10 || !wt0 || !K0) {
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}

		Routes * best_routes = new Routes(in,in->ncustomers);
		double average = 0.0;
		double best_all = numeric_limits<double>::max() -1.0;
		int it = 10;
		for (int i=0; i<it; i++) {
			PSO * pso = new PSO(in,nroutes,L,cp,cg,cl,cn,w1,wt,K);
			if (info) {
				pso->enableShowInfo();
			}
			Routes * routes = pso->runHeuristic(n_iter);
			double best_result = routes->getTotalCost();
			if (best_result < best_all) {
				best_all = best_result;
				best_routes->clone(routes);
			}
			printf("La mejor solucion obtenida mediante PSO fue de: %f\n",best_result);
			average += best_result;
		}
		average /=it;
		printf("\n");
		if (info) {
			best_routes->printAllRoutes();
		}
		//printf("\nEl costo promedio en %d iteraciones de PSO fue de: %f\n",it,average);
		//printf("El menor costo obtenido en %d iteraciones de PSO fue de: %f\n",it,best_all);

	} else if (heuristic == string("ILS")) {
		//-f vrpnc11y.txt -ft SalhiNagy -h ILS -n 15 -LS 80 -y 0.30


		if(!n || !yf || !LS){
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}

		Routes* solution = new Routes(in, in->ncustomers);
		Routes* aux = new Routes(in, in->ncustomers);
		Routes::createInitialRoutesNearestNeighbor(aux);
		//Routes* aux = Routes::createInitialRoutesGRASP(in,0.75,0);

		ILS::ils_best(n_iter,nLS,in,solution,aux->last_route,y);

		if (info) {
			solution->printAllRoutes();
		}

		double best_result = solution->calcTotalCost();

		printf("\nLa mejor solucion obtenida mediante ILS fue de: %f\n",best_result);
	} else if (heuristic == string("IGA")) {
		//-f vrpnc11y.txt -ft SalhiNagy -h IGA -n 200 -p 40 --info

		if(!n||!p1){
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}

		Routes* solution = new Routes(in, in->ncustomers);

		Genetic::genetic(n_iter,p,in,solution);

		if (info) {
			solution->printAllRoutes();
		}


		double best_result = solution->calcTotalCost();

		printf("\nLa mejor solucion obtenida mediante IGA fue de: %f\n",best_result);

	} else if (heuristic == string("SCA")) {
		//TODO
		//-f vrpnc11y.txt -ft SalhiNagy -h SCA -n 120 -b 10 -y 0.5 --info

		if(!n || !yf || !b1){
			fprintf(stderr, "Error: Faltan parametros\n");
			showUse(argv[0]);
			return 1;
		}

		Routes* solution = new Routes(in, in->ncustomers);

		Scatter::scatter(n_iter,b,y,solution,in);

		if (info) {
			solution->printAllRoutes();
		}

		double best_result = solution->calcTotalCost();

		printf("\nLa mejor solucion obtenida mediante SCA fue de: %f\n",best_result);

	} else {
		fprintf(stderr, "Error: Tipo de heuristica desconocida\n");
		showUse(argv[0]);
		return 1;
	}

/*
    int ncustomers = 8;
    double * demands_pickup = new double[ncustomers+1];
    double * demands_delivery = new double[ncustomers+1];
    double ** cost = new double*[ncustomers+1];
                for (int i=0;i<ncustomers+1;i++)
                        cost[i] = new double[ncustomers+1];

    for (int i=0; i<ncustomers+1; i++)
        for (int j=0; j<ncustomers+1; j++)
            cost[i][j] = 1.0;
    cost[1][0] = 0.0;
    cost[1][8] = 0.0;
    cost[8][1] = 0.0;
    cost[2][7] = 0.0;
    cost[7][2] = 0.0;

    demands_pickup[1] = 5.0;
    demands_pickup[2] = 5.0;
    demands_pickup[3] = 6.0;
    demands_pickup[4] = 1.0;
    demands_pickup[5] = 1.0;
    demands_pickup[6] = 2.0;
    demands_pickup[7] = 2.0;
    demands_pickup[8] = 0.0;
    demands_delivery[1] = 2.0;
    demands_delivery[2] = 1.0;
    demands_delivery[3] = 3.0;
    demands_delivery[4] = 1.0;
    demands_delivery[5] = 2.0;
    demands_delivery[6] = 1.0;
    demands_delivery[7] = 3.0;
    demands_delivery[8] = 5.0;

    Input * in = new Input(ncustomers,20);

    in->cost = cost;
    in->deliveries = demands_delivery;
    in->pickups = demands_pickup;
*/
	/*
	Input* in = IO::readFileSalhiNagy("vrpnc11y.txt");
	GTS * gts = new GTS(in);
	for (int i=0;i<gts->routes->last_route;i++)
		gts->routes->printRouteDetails(i);

	cout << "Total: " << gts->routes->getTotalCost() << endl;

	Ant * aco = new Ant(in);
	//aco->runHeuristic(100);
	 * */


	/*
	Routes::createInitialRoutesNearestNeighborRandom(routes);
	for (int i=0;i<routes->last_route;i++)
			routes->printRouteDetails(i);
	return 0;

	Ant * aco = new Ant(in);
	int n_times = 1;
	double average = 0.0;

	cout << "[";
	for (int i=0; i<n_times; i++) {
		double best_result = aco->runHeuristic(100);
		if (i==n_times-1) {
			cout << best_result;
		} else {
			cout << best_result << ",";
		}
		average += best_result;
	}
	cout << "]\n";

	cout <<  "\nLa solucion promedio fue de: " << average/n_times << endl;

    //Routes * routes = new Routes(in,in->ncustomers);
*/

    /*
    routes->newRoute();
    routes->addFeasibleArc(0,1,2);
    routes->addFeasibleArc(0,2,3);
    routes->addFeasibleArc(0,3,4);
    routes->addFeasibleArc(0,4,5);
    routes->addFeasibleArc(0,5,6);
    routes->newRoute();
    routes->addFeasibleArc(1,7,8);
    //routes->addRoute();

    routes->printRouteDetails(0);
    if (Local_Search::swapIntraRoute(0,routes)) {
    	cout << "Operador relocate mejoro la solucion\n" << endl;
    } else {
    	cout << "Operador relocate no pudo mejorar la solucion\n" << endl;
    }
    */
	/*
    Routes::createInitialRoutesNearestNeighbor(routes);
    for (int i=0;i<routes->last_route;i++)
    	routes->printRouteDetails(i);

    cout << "\nCosto total del tour: " << routes->getTotalCost() << endl;

    cout << "\nCambio:" << endl;

	int res = Local_Search::ls_shift_one(500,routes,in);
    for (int i=0;i<routes->last_route;i++)
    	routes->printRouteDetails(i);

    cout << "\nCosto total del tour: " << routes->getTotalCost() << endl;
    */

    /*
    int ants[8] = {1,2,3,4,5,6,7,8};
    double costos[8] = {0.3,0.4,0.1,0.6,0.7,0.8,0.9,1.0};
    Quicksort::runQuicksortCosts(ants, costos, 1, 7);
    for (int i=0; i<8; i++)
    	cout << " " << ants[i];
    	*/

    //Operators::exchangeFeasibleCustomersIntra(2,0,4,routes,true);
    /*
    if (Operators::relocateCustomerIntra(1,0,3,4,routes,true)) {
        cout << "Es factible realizar ese relocate" << endl;
    } else {
        cout << "No es factible realizar ese relocate" << endl;
    }*/
    //routes->printRouteDetails(0);
/*
    int city = 1;
    int route = 0;
    int min = 0;
    double min_cost = 500.0;
    int min_route = 0;
    int pos = 0;
    int mod;
    mod = Operators::check_shift( 2 , route, &min, &min_cost, &min_route, &pos, routes , in);
    mod = Operators::check_shift( city, route, &min, &min_cost, &min_route, &pos, routes , in);

    routes->printRouteDetails(1);
    cout << "mod: "<<mod<<" pos: " << pos << " min: "<<min<<" min_cost "<<min_cost << " min_route: " << min_route;
*/

    return 0;
}

void showUse(char* program) {
	printf("USO DEL PROGRAMA\n");
	printf("    %s -f filename -ft filetype -h heuristic [OPCIONES HEURISTICA]\n", program);

	printf("\nOPCIONES\n");
	printf("    -f string       Nombre del archivo con el problema\n");
	printf("    -ft string      Tipo del problema (Dethloff,SalhiNagy,Min)\n");
	printf("    -h string       Heuristica a utilizar (ACO,GTS,PSO,ILS,IGA,SCA)\n");
	printf("OPCIONES ACO:\n");
	printf("    -n int          Numero de iteraciones\n");
	printf("    -alpha double   Valor parametro alpha\n");
	printf("    -beta double    Valor parametro beta\n");
	printf("    -q double       Valor parametro q\n");
	printf("    -ro double      Valor parametro ro\n");
	printf("OPCIONES GTS:\n");
	printf("OPCIONES PSO:\n");
	printf("OPCIONES ILS:\n");
	printf("OPCIONES IGA:\n");
	printf("OPCIONES SCA:\n");
}
