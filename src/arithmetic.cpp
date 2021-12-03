// реализация функций и классов для вычисления арифметических выражений

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "../include/arithmetic.h"
using namespace std;

std::map<string, unsigned char> priority{{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
// Priority of operations

#define NUM 1
#define OP 2
#define OPEN_BR 3
#define CLOSE_BR 4
// Типы токенов: 1 - число, 2 - операция, 3 - открывающая скобка, 4 - закрывающая скобка

struct Token {
	char type;		// имеет одно из определённых выше значений
	size_t pos;		// позиция начала токена в строке с выражением
	double num;		// значение числа, если токен хранит число
	string op;		// символ операции, если токен хранит операцию

	explicit Token(char type = 0, size_t pos = -1, double num = 0.0): type(type), pos(pos), num(num), op("") {}
	explicit Token(char type, size_t pos, string op): type(type), pos(pos), op(op) {}

	bool operator==(const Token &token) {
		return (type == token.type && op == token.op);
	}
};

std::vector<Token> parse(const string &expr);							// выражение -> токены
bool check(const string &expr);											// проверка выражения на корректность
std::vector<Token> make_postfix_notation(const vector<Token> tokens);	// инфиксная нотация -> постфиксная запись
double compute_postfix(const vector<Token> postfix);					// вычисление выражения в постфиксной записи


vector<Token> parse(string &expr) {
	vector<Token> tokens;
	size_t first_digit_pos = -1;	// позиция первой цифры секущего считываемого числа; -1, если в данный момент не считывается число
	bool was_dot = false;			// была ли считана точка действительного числа

	for (size_t i = 0; i < expr.size(); ++i) {
		if (expr[i] == ' ') continue;

		if ('0' <= expr[i] && expr[i] <= '9') {
			// запоминаем позицию, если встретили первую цифру очередную числа
			if (first_digit_pos == -1)
				first_digit_pos = i;
		}

		// встретили точку действительного числа
		else if (first_digit_pos != -1 && expr[i] == '.' && !was_dot)
			was_dot = true;

		// встретилось что-то, отличное от цифры или точки, есле для последнего числа она уже была считана
		else {
			was_dot = false;

			// число считано
			if (first_digit_pos != -1) {
				tokens.push_back(Token(NUM, first_digit_pos, stod(expr.substr(first_digit_pos, i-first_digit_pos))));
				first_digit_pos = -1;
			}

			// считана открывающая скобка
			if (expr[i] == '(')
				tokens.push_back(Token(OPEN_BR, i));

			// считана закрывающая скобка
			else if (expr[i] == ')')
				tokens.push_back(Token(CLOSE_BR, i));

			// считан минус, который может быть унарным
			else if (expr[i] == '-' && (tokens.empty() || tokens.back().type == OPEN_BR)) {
				tokens.push_back(Token(NUM, -1, 0.0));
				tokens.push_back(Token(OP, i, "-"));
			}

			// считано что-то другое - операция или недопустимый символ
			else
				tokens.push_back(Token(OP, i, string(1,expr[i])));
		}
	}

	// проверка, что могло остаться несчитанное число
	if (first_digit_pos != -1) {
		tokens.push_back(Token(NUM, first_digit_pos, stod(expr.substr(first_digit_pos, expr.size()-first_digit_pos))));
	}

	return tokens;
}

vector<string> check(vector<Token> &tokens) {
	char last_state = OPEN_BR;		// тип последнего считанного токена
	MyStack<Token> correct_brackets; // стек проверки правильной скобочной последовательности
	vector<string> errors;

	for (Token &token : tokens) {
		switch (token.type) {
			case NUM:
				if (last_state == NUM)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect number after number");
				else if (last_state == CLOSE_BR)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect number after closing bracket");

				break;

			case OP:
				// если символа нет в таблице допустимых операций, выражение некорректно
				if (priority.find(token.op) == priority.end())
					errors.push_back("Error at position " + to_string(token.pos) + ": incorrect symbol");
				else if (last_state == OP)
						errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect operation after operation");
				else if (last_state == OPEN_BR)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect operation after opening bracket");

				break;

			case OPEN_BR:
				if (last_state == NUM)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect opening bracket after number");
				else if (last_state == CLOSE_BR)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect opening bracket after closing bracket");

				correct_brackets.push(token);
				break;

			case CLOSE_BR:
				if (last_state == OP)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect closing bracket after operation");
				else if (last_state == OPEN_BR)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect closing bracket after opening bracket");
				else if (correct_brackets.empty()) {
					errors.push_back("Error at position " + to_string(token.pos) + ": no matching opening bracket");
					correct_brackets.push(token);
				}
				else
					correct_brackets.pop();
				break;
		}

		last_state = token.type;
	}

	while (!correct_brackets.empty()) {
		errors.push_back("Error at position " + to_string(correct_brackets.top().pos) + ": unpaired bracket");
		correct_brackets.pop();
	}

	return errors;
}

std::vector<Token> make_postfix_notation(std::vector<Token> tokens) {
	vector<Token> postfix;
	MyStack<Token> ops;

	for (Token &token: tokens) {
		switch (token.type) {
			case NUM:
				postfix.push_back(token);
				break;

			case OP:
				if (!ops.empty() && priority[ops.top().op] >= priority[token.op])
					while (!ops.empty() && ops.top().type != OPEN_BR && priority[ops.top().op] >= priority[token.op]) {
						postfix.push_back(ops.top());
						ops.pop();
					}
				ops.push(token);
				break;

			case OPEN_BR:
				ops.push(token);
				break;

			case CLOSE_BR:
				while (!ops.empty() && ops.top().type != OPEN_BR) {
					postfix.push_back(ops.top());
					ops.pop();
				}
				ops.pop();
				break;
		}
	}

	while (!ops.empty()) {
		postfix.push_back(ops.top());
		ops.pop();
	}

	return postfix;
}

double compute_postfix(vector<Token> postfix) {
	MyStack<double> operands;
	double result, op1, op2;

	for (Token &token: postfix) {
		switch (token.type) {
			case NUM:
				operands.push(token.num);
				break;

			case OP:
				op1 = operands.top();
				operands.pop();
				op2 = operands.top();
				operands.pop();

				string op = token.op;
				if (op == "+")
					result = op2 + op1;
				else if (op == "-")
					result = op2 - op1;
				else if (op == "*")
					result = op1 * op2;
				else if (op == "/")
					result = op2 / op1;

				operands.push(result);
				break;
		}
	}

	return operands.top();
}

double compute(std::string &expr) { // performs all the stages
	vector<Token> tokens = parse(expr);
	vector<string> errors = check(tokens);
	if (errors.empty()) {
		tokens = make_postfix_notation(tokens);
		return compute_postfix(tokens);
	}
	else {
		std::cout << "Expression is incorrect!\n";
		for (string &error : errors)
			cout << error << endl;
		cout << endl;
		throw runtime_error("Expr is incorrect");
	}
}