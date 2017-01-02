#ifndef EQUATIONS_HPP
#define EQUATIONS_HPP

#include <random>
#include <stack>
#include <vector>

class EquationPiece {
private:
public:
	enum class Type { Number, Operation };
	Type type;
	char operation{'\0'};
	double number{0};
	EquationPiece(char op);
	EquationPiece(double num);
};

class Equation {
private:
	std::vector<EquationPiece> equationPieces;
	std::string doubleToString(double num) const;
public:
	static const std::vector<char> OPERATIONS;
	void addNumber(double num);
	void addOperation(char op);
	std::string toString() const;
	double evaluate();
};

class RandomEquationBuilder {
private:
	const std::vector<double> NUMBERS;
	std::mt19937 eng;
	void createRandomEngine();
	double randomNumber();
	char randomOperation();
public:
	RandomEquationBuilder(const std::vector<double> NUMS);
	Equation build(const int DESIRED_NUMBER_COUNT_IN_EQUATION);
};

#endif //EQUATIONS_HPP