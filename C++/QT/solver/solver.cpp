// g++ solver.cpp -I /opt/gurobi501/linux32/include/  -L /opt/gurobi501/linux32/lib/ -lgurobi_c++ -L /opt/gurobi501/linux32/lib -lgurobi50

#include "gurobi_c++.h"
using namespace std;
vector <double> solve(vector <vector <double> > M) {
vector <double> solution;
  try {	
    GRBEnv env = GRBEnv();

	int numPoints = M.size();
	int numCircles = M.back().size();
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



for (int i=0;i<numPoints;i++){
	solution.push_back((double)p[i].get(GRB_DoubleAttr_X));
//	cout << p[i].get(GRB_DoubleAttr_X) << endl;
}
/*
    cout << x.get(GRB_StringAttr_VarName) << " "
         << x.get(GRB_DoubleAttr_X) << endl;
    cout << y.get(GRB_StringAttr_VarName) << " "
         << y.get(GRB_DoubleAttr_X) << endl;
    cout << z.get(GRB_StringAttr_VarName) << " "
         << z.get(GRB_DoubleAttr_X) << endl;
*/
//    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return solution;
}
