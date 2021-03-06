// g++ solver.cpp -I /opt/gurobi501/linux32/include/  -L /opt/gurobi501/linux32/lib/ -lgurobi_c++ -L /opt/gurobi501/linux32/lib -lgurobi50
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c++.h"
using namespace std;
vector <double> solve(int verbose, vector <vector <double> > M) {
	vector <double> solution;
	try {	
		GRBEnv env = GRBEnv();

		int numPoints = M.size();
		int numCircles = M.back().size();
		GRBVar p[numPoints];
		double coeff[numPoints];
		GRBModel model = GRBModel(env);
		GRBLinExpr expr;

		if(!verbose)
			model.getEnv().set(GRB_IntParam_OutputFlag, 0);


		for (int i=0;i<numPoints;i++){
			p[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
			coeff[i] = 1.0;
		}

		expr.addTerms(coeff, p,numPoints);

		model.update();

		model.setObjective(expr, GRB_MINIMIZE);

		for(int i=0;i<numCircles;i++){
			GRBLinExpr cexpr;
			double ccoeff[numPoints];
			for(int j=0;j<numPoints;j++){
				ccoeff[j] = M[j].back();
				M[j].pop_back();
			}
			cexpr.addTerms(ccoeff,p,numPoints);
			model.addConstr(cexpr, GRB_GREATER_EQUAL,1.0);
		}
    
		// Optimize model
		model.optimize();



		for (int i=0;i<numPoints;i++){
			solution.push_back((double)p[i].get(GRB_DoubleAttr_X));
		}

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return solution;
}

vector <double> solve2(vector <vector <double> > M) {
vector <double> solution;
  try {	
    GRBEnv env = GRBEnv();
	vector <vector <double> > N = M;
	int numPoints = M.size();
	int numCircles = M.back().size();
	//cout<<"solving for "<<numCircles<<" circles and "<<numPoints<<" points"<<endl;
	GRBVar p[numPoints];
	double coeff[numPoints];
        GRBModel model = GRBModel(env);
	GRBLinExpr expr;

   // Create variables
	for (int i=0;i<numPoints;i++){
		p[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
		coeff[i] = 1.0;
	}

	expr.addTerms(coeff, p,numPoints);

    // Integrate new variables

    model.update();

    // Set objective: maximize x + y + 2 z

    model.setObjective(expr, GRB_MINIMIZE);

	for(int i=0;i<numCircles;i++){
		GRBLinExpr cexpr;
		double ccoeff[numPoints];
		for(int j=0;j<numPoints;j++){
			ccoeff[j] = M[j].back();
			M[j].pop_back();
		}
		cexpr.addTerms(ccoeff,p,numPoints);
	   model.addConstr(cexpr, GRB_GREATER_EQUAL,1.0);
	}
    
    // Optimize model

    model.optimize();

	int c=0;
	for (int i=0;i<numPoints;i++){
		solution.push_back((double)p[i].get(GRB_DoubleAttr_X));
		c += solution.back();
	}
	model.addConstr(expr, GRB_EQUAL, c);

	int temp;
	int temp2;
	for (int i=0;i<numPoints;i++){
		temp=0;
		while(N.back().size()){
			temp = temp + (N.back()).back();
			N.back().pop_back();
		}			
		coeff[i] = temp;
		N.pop_back();
	}

	expr.addTerms(coeff, p,numPoints);
	if (rand()%2)
	   model.setObjective(expr, GRB_MINIMIZE);
	else
	   model.setObjective(expr, GRB_MAXIMIZE);
	model.optimize();
	solution.clear();
	c=0;
	for (int i=0;i<numPoints;i++){
		solution.push_back((double)p[i].get(GRB_DoubleAttr_X));
		c += solution.back();
	}


  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
//    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return solution;
}
