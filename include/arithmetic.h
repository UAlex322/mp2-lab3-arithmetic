// объ¤вление функций и классов дл¤ вычислени¤ арифметических выражений
#pragma once
#include <string>
#include <vector>
#include <map>
#include "stack.h"
using namespace std;

class Expression {
	std::map<string, char> prior{{"+", -2}, {"-", -2}, {"*", -1}, {"/", -1}, {"-u", 1}};
	// Приоритет операций (левоассоциативные имеют отрицательное значение, правоассоциативные - положительное)

	struct Token {
		char type;		// имеет одно из определённых выше значений
		size_t pos;		// позиция начала токена в строке с выражением
		double num;		// значение числа, если токен хранит число
		string name;	// символ операции, если хранится операция, или название переменной, если хранится число

		explicit Token(char type = 0, double num = 0.0, const string& name = "", size_t pos = -1): type(type), pos(pos), num(num), name(name) {}

		bool operator==(const Token &token) {
			return (type == token.type && name == token.name);
		}
		bool operator!=(const Token &token) {
			return !(*this == token);
		}
	};

	vector<Token> parse(const string &expr);	// выражение -> токены
	vector<string> check();						// проверка выражения на корректность
	void make_postfix_notation();				// инфиксная нотация -> постфиксная запись

	// DATA
	vector<Token> _tokens;							// набор лексем в постфиксной записи
	map<string, vector<int>> variable_positions;	// пары из названий переменных и индексов лексем в постфиксной записи, в которых они находятся

public:

	Expression(const string &expr);
	double compute();							// вычисление выражения в постфиксной записи
	bool has_no_variables();					// проверка на наличие переменных в выражении
};