// реализация функций и классов для вычисления арифметических выражений

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "../include/arithmetic.h"
using namespace std;

#define NUM 1
#define OP 2
#define OPEN_BR 3
#define CLOSE_BR 4
// Типы токенов: 1 - число, 2 - операция, 3 - открывающая скобка, 4 - закрывающая скобка

Expression::Expression(const string &expr) {
	_tokens = parse(expr);
	vector<string> errors = check();

	if (_tokens.empty() || !errors.empty()) {
		cout << "Expression is incorrect!" << endl;
		if (_tokens.empty())
			cout << "Empty expression" << endl;
		for (string &error : errors) {
			cout << error << endl;
		}
		cout << endl;
		throw runtime_error("Expr is incorrect");
	}

	make_postfix_notation();
}

vector<Expression::Token> Expression::parse(const string &expr) {
	vector<Token> tokens;
	size_t first_digit_pos = -1;	// позиция первой цифры секущего считываемого числа; -1, если в данный момент не считывается число
	size_t first_letter_pos = -1;	// позиция первой буквы текущей считываемой переменной; -1, если в данный момент не считывается переменная
	bool was_dot = false;			// была ли считана точка действительного числа

	for (size_t i = 0; i < expr.size(); ++i) {
		// если не считывается число, то пробел несущественен
		if (expr[i] == ' ' && first_digit_pos == -1 && first_letter_pos == -1) continue;

		else if ('a' <= expr[i] && expr[i] <= 'z' || 'A' <= expr[i] && expr[i] <= 'Z') {
			// запоминаем позицию, если встретили первую цифру очередной переменной или функции
			if (first_letter_pos == -1)
				first_letter_pos = i;
		}

		else if ('0' <= expr[i] && expr[i] <= '9') {
			// запоминаем позицию, если встретили первую цифру очередную числа
			if (first_digit_pos == -1)
				first_digit_pos = i;
		}

		// встретили точку действительного числа
		else if (first_digit_pos != -1 && expr[i] == '.' && !was_dot)
			was_dot = true;

		// встретилось что-то, отличное от цифры или точки, если для последнего числа она уже была считана
		else {
			was_dot = false;

			// число считано
			if (first_digit_pos != -1) {
				if (!tokens.empty() && tokens.back().name == "-" && (tokens.size() == 1 || tokens[tokens.size()-2].type == OP || tokens[tokens.size()-2] == Token(OPEN_BR, -1))) {
					tokens.pop_back();
					tokens.push_back(Token(NUM, -stod(expr.substr(first_digit_pos, i-first_digit_pos)), "", first_digit_pos));
				}
				else {
					tokens.push_back(Token(NUM, stod(expr.substr(first_digit_pos, i-first_digit_pos)), "", first_digit_pos));
				}

				first_digit_pos = -1;
			}

			// переменная считана
			if (first_letter_pos != -1) {
				if (!tokens.empty() && tokens.back().name == "-" && (tokens.size() == 1 || tokens[tokens.size()-2].type == OP || tokens[tokens.size()-2] == Token(OPEN_BR, -1))) {
					tokens.pop_back();
					tokens.push_back(Token(NUM, -1.0, expr.substr(first_letter_pos, i-first_letter_pos), first_letter_pos));
				}
				else {
					tokens.push_back(Token(NUM, 1.0, expr.substr(first_letter_pos, i-first_letter_pos), first_letter_pos));
				}

				first_letter_pos = -1;
			}

			// считана открывающая скобка
			if (expr[i] == '(')
				tokens.push_back(Token(OPEN_BR, 0.0, "", i));

			// считана закрывающая скобка
			else if (expr[i] == ')')
				tokens.push_back(Token(CLOSE_BR, 0.0, "", i));

			// считано что-то другое - операция или недопустимый символ
			else if (expr[i] != ' ')
				tokens.push_back(Token(OP, 0.0, string(1, expr[i]), i));
		}
	}

	// проверка, что могло остаться несчитанное число
	if (first_digit_pos != -1) {
		// проверка, что перед этим был унарный минус - либо до этого вообще не было символов
		if (!tokens.empty() && tokens.back().name == "-" && (tokens.size() == 1 || tokens[tokens.size()-2].type == OP || tokens[tokens.size()-2] == Token(OPEN_BR, 0))) {
			tokens.pop_back();
			tokens.push_back(Token(NUM, -stod(expr.substr(first_digit_pos, expr.size()-first_digit_pos)), "", first_digit_pos));
		}
		else
			tokens.push_back(Token(NUM, stod(expr.substr(first_digit_pos, expr.size()-first_digit_pos)), "", first_digit_pos));
	}

	// проверка, что могло остаться несчитанное имя переменной
	if (first_letter_pos != -1) {
		if (!tokens.empty() && tokens.back().name == "-" && (tokens.size() == 1 || tokens[tokens.size()-2].type == OP || tokens[tokens.size()-2] == Token(OPEN_BR, -1))) {
			tokens.pop_back();
			tokens.push_back(Token(NUM, -1.0, expr.substr(first_letter_pos, expr.size()-first_letter_pos), first_letter_pos));
		}
		else
			tokens.push_back(Token(NUM, 1.0, expr.substr(first_letter_pos, expr.size()-first_letter_pos), first_letter_pos));
	}

	return tokens;
}

vector<string> Expression::check() {
	char last_state = OPEN_BR;		// тип последнего считанного токена
	MyStack<Token> correct_brackets; // стек проверки правильной скобочной последовательности
	vector<string> errors;

	for (const Token &token : _tokens) {
		switch (token.type) {
			case NUM:
				if (last_state == NUM)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect number after number");
				else if (last_state == CLOSE_BR)
					errors.push_back("Error at position " + to_string(token.pos) + ": didn't expect number after closing bracket");

				break;

			case OP:
				// если символа нет в таблице допустимых операций, выражение некорректно
				if (priority.find(token.name) == priority.end())
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

void Expression::make_postfix_notation() {
	vector<Token> postfix;
	MyStack<Token> ops;

	for (const Token &token: _tokens) {
		switch (token.type) {
			case NUM:
				if (token.name != "")
					variable_positions[token.name].push_back(postfix.size());
				postfix.push_back(token);

				break;

			case OP:
				if (!ops.empty() && priority[ops.top().name] >= priority[token.name])
					while (!ops.empty() && ops.top().type != OPEN_BR && priority[ops.top().name] >= priority[token.name]) {
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

	_tokens = postfix;
}

double Expression::compute() {
	vector<Token> copy_tokens = _tokens; // копия набора лексем для вычисления, чтобы не портить исходные данные
	MyStack<double> operands;
	double result, op1, op2;
	
	if (!variable_positions.empty()) {
		cout << "Enter values of variables:" << endl;
		for (auto &var : variable_positions) {
			string name = var.first, str_value;
			double value;
			bool exception_happened = true;

			cout << name << " = ";
			getline(cin, str_value);

			// обработка неправильного ввода
			while (exception_happened) {
				try {
					value = stod(str_value);
					exception_happened = false;
				}
				catch (...) {
					cout << "Incorrect input, try again" << endl;
					cout << name << " = ";
					getline(cin, str_value);
				}
			}

			for (auto &pos : variable_positions[name])
				copy_tokens[pos].num = (copy_tokens[pos].num == -1.0) ? -value : value;
		}
	}

	for (Token &token: copy_tokens) {
		switch (token.type) {
			case NUM:
				operands.push(token.num);
				break;

			case OP:
				op1 = operands.top();
				operands.pop();
				op2 = operands.top();
				operands.pop();

				string name = token.name;
				if (name == "+")
					result = op2 + op1;
				else if (name == "-")
					result = op2 - op1;
				else if (name == "*")
					result = op1 * op2;
				else if (name == "/")
					result = op2 / op1;

				operands.push(result);
				break;
		}
	}

	return operands.top();
}

bool Expression::has_no_variables() {
	return variable_positions.empty();
}