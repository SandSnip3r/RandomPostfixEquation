#include "equations.hpp"
#include <algorithm>
#include <stdexcept>

EquationPiece::EquationPiece(char op) : type(Type::Operation), operation(op) {}

EquationPiece::EquationPiece(double num) : type(Type::Number), number(num) {}

std::string Equation::doubleToString(double num) const {
	std::string doubleString = std::to_string(num);
	doubleString.erase(doubleString.find_last_not_of('0') + 1, std::string::npos);
	if (!doubleString.empty() && doubleString.back() == '.') {
		doubleString.pop_back();
	}
	return doubleString;
}

void Equation::addNumber(double num) {
	equationPieces.emplace_back(num);
}

void Equation::addOperation(char op) {
	if (find(OPERATIONS.begin(), OPERATIONS.end(), op) == OPERATIONS.end()) {
		throw std::runtime_error(std::string("Invalid operation ")+op+" added to equation.");
	}
	equationPieces.emplace_back(op);
}

std::string Equation::toString() const {
	std::string equationString;
	for (auto piece : equationPieces) {
		if (piece.type == EquationPiece::Type::Number) {
			//Convert number to string and add it to the resulting string
			equationString += doubleToString(piece.number);
		} else {
			//Add operation to resulting string
			equationString += piece.operation;
		}
		equationString += ' ';
	}
	return equationString;
}

double Equation::evaluate() {
	std::stack<double> evaluationStack;
	for (auto piece : equationPieces) {
		if (piece.type == EquationPiece::Type::Number) {
			//Next thing in the equation is a number, add it to the stack
			evaluationStack.emplace(piece.number);
		} else {
			//Next thing in the equation is a operation,
			//	pop the previous 2 numbers
			if (evaluationStack.size() < 2) {
				throw std::runtime_error("Trying to evaluate equation. Failed because too few numbers exist.");
			}
			double num1 = evaluationStack.top();
			evaluationStack.pop();
			double num2 = evaluationStack.top();
			evaluationStack.pop();

			//	apply the operation
			double result;
			switch (piece.operation) {
			case '+':
				result = num2+num1;
				break;
			case '-':
				result = num2-num1;
				break;
			case '*':
				result = num2*num1;
				break;
			case '/':
				if (num1 == 0) {
					throw std::runtime_error("Trying to divide by 0.");
				}
				result = num2/num1;
				break;
			default:
				//Cant get here because addOperation disallows invalid operations
				break;
			}

			//	push the result onto the stack
			evaluationStack.push(result);
		}
	}
	if (evaluationStack.size() != 1) {
		throw std::runtime_error("Trying to evaluate equation. Failed because too few operations exist.");
	}
	return evaluationStack.top();
}

const std::vector<char> Equation::OPERATIONS = {'+','-','*','/'};

void RandomEquationBuilder::createRandomEngine() {
	std::random_device rd;
	std::vector<unsigned int> seeds;
	seeds.reserve(std::mt19937::state_size);
	for (size_t i=0; i<std::mt19937::state_size; ++i) {
		seeds.emplace_back(rd());
	}
	std::seed_seq s(seeds.begin(), seeds.end());
	eng = std::mt19937(s);
}

double RandomEquationBuilder::randomNumber() {
	std::uniform_int_distribution<int> dist(0,NUMBERS.size()-1);
	return NUMBERS[dist(eng)];
}

char RandomEquationBuilder::randomOperation() {
	std::uniform_int_distribution<int> dist(0,Equation::OPERATIONS.size()-1);
	return Equation::OPERATIONS[dist(eng)];
}

RandomEquationBuilder::RandomEquationBuilder(const std::vector<double> NUMS) : NUMBERS(NUMS) {
	createRandomEngine();
}

Equation RandomEquationBuilder::build(const int DESIRED_NUMBER_COUNT_IN_EQUATION) {
	Equation equation;
	int numberCount=0;
	int balance=0;
	std::bernoulli_distribution opOrNumDist(0.5);
	while (numberCount < DESIRED_NUMBER_COUNT_IN_EQUATION) {
		if (balance >= 2) {
			//50/50 chance to add number or operation
			if (opOrNumDist(eng)) {
				//Add operation
				equation.addOperation(randomOperation());
				--balance;
			} else {
				//Add number
				equation.addNumber(randomNumber());
				++numberCount;
				++balance;
			}
		} else {
			//Too few numbers in the equation, must add number
			equation.addNumber(randomNumber());
			++numberCount;
			++balance;
		}
	}
	while (balance > 1) {
		//Not enough operations in the equation
		equation.addOperation(randomOperation());
		--balance;
	}
	return equation;
}