#include <iostream>
#include "equations.hpp"

int main() {
	using namespace std;
	
	const vector<double> NUMBERS = {69, 420};
	RandomEquationBuilder randomEquationBuilder(NUMBERS);
	const int GOAL_NUMBER_COUNT = 3;

	int TRY_COUNT = 25;

	for (int i=0; i<TRY_COUNT; ++i) {
		Equation equation = randomEquationBuilder.build(GOAL_NUMBER_COUNT);
		try {
			double result = equation.evaluate();
			cout << equation.toString() << "= " << result << '\n';
		} catch (std::runtime_error &rte) {
			cout << equation.toString() << "failed. " << rte.what() << '\n';
		}
	}
	return 0;
}