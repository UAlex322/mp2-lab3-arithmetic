// ���������� ������� � ������� ��� ���������� �������������� ���������

#include <map>
#include <vector>
#include <stdexcept>
#include "../include/arithmetic.h"
using namespace std;

std::map<char, unsigned char> priority{{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};
// Priority of operations

#define NUM 1
#define OP 2
#define OPEN_BR 3
#define CLOSE_BR 4
// ���� �������: 1 - �����, 2 - ��������, 3 - ����������� ������, 4 - ����������� ������

struct Token {
	char type;		// ����� ���� �� ����������� ���� ��������
	double num;		// �������� �����, ���� ����� ������ �����
	char op;		// ������ ��������, ���� ����� ������ ��������

	explicit Token(char type = 0, double num = 0): type(type), num(num), op(0) {}
	explicit Token(char type, char op): type(type), op(op) {}

	bool operator== (const Token &token) {
		return (type == token.type && op == token.op);
	}
};

std::vector<Token> parse(std::string &expr);							// ��������� -> ������
bool check(std::string &expr);											// �������� ��������� �� ������������
std::vector<Token> make_postfix_notation(std::vector<Token> tokens);	// ��������� ������� -> ����������� ������
double compute_postfix(std::vector<Token> postfix);						// ���������� ��������� � ����������� ������


vector<Token> parse(string &expr) {
	vector<Token> tokens;
	int first_digit_pos = -1;		// ������� ������ ����� ���������� ���������� �����
	bool was_dot = false;			// ���� �� ������� ����� ��������������� �����

	for (size_t i = 0; i < expr.size(); ++i) {
		if (expr[i] == ' ') continue;

		if ('0' <= expr[i] && expr[i] <= '9') {
			// ���������� �������, ���� ��������� ������ ����� ��������� �����
			if (first_digit_pos == -1)
				first_digit_pos = i; 
		}

		// ��������� ����� ��������������� �����
		else if (first_digit_pos != -1 && expr[i] == '.' && !was_dot)
			was_dot = true;

		// ����������� ���-��, �������� �� ����� ��� �����, ���� ��� ���������� ����� ��� ��� ���� �������
		else {
			was_dot = false;

			// ����� �������
			if (first_digit_pos != -1) {
				tokens.push_back(Token(NUM, stod(expr.substr(first_digit_pos, i-first_digit_pos))));
				first_digit_pos = -1;
			}

			// ������� ����������� ������
			if (expr[i] == '(')
				tokens.push_back(Token(OPEN_BR));

			// ������� ����������� ������
			else if (expr[i] == ')')
				tokens.push_back(Token(CLOSE_BR));
			// ������ �����, ������� ����� ���� �������
			else if (expr[i] == '-' && (tokens.empty() || tokens.back().type == OPEN_BR)) {
				tokens.push_back(Token(NUM, 0.0));
				tokens.push_back(Token(OP, '-'));
			}
			// ������� ���-�� ������ - �������� ��� ������������ ������
			else
				tokens.push_back(Token(OP, expr[i]));
		}
	}

	// ��������, ��� ����� �������� ����������� �����
	if (first_digit_pos != -1) {
		tokens.push_back(Token(NUM, stod(expr.substr(first_digit_pos, expr.size()-first_digit_pos))));
	}

	return tokens;
}

bool check(vector<Token> &tokens) {
	char last_state = OPEN_BR;		// ��� ���������� ���������� ������
	MyStack<char> correct_brackets; // ���� �������� ���������� ��������� ������������������

	for (Token &token : tokens) {
		switch (token.type) {
			case NUM:
				if (last_state == NUM || last_state == CLOSE_BR)
					return false;
				break;

			case OP:
				// ���� ������� ��� � ������� ���������� ��������, ��������� �����������
				if (last_state == OP || last_state == OPEN_BR || priority.find(token.op) == priority.end())
					return false;
				break;

			case OPEN_BR:
				if (last_state == NUM || last_state == CLOSE_BR)
					return false;
				correct_brackets.push('(');
				break;

			case CLOSE_BR:
				if (last_state == OP || last_state == OPEN_BR || correct_brackets.empty())
					return false;
				correct_brackets.pop();
				break;
		}

		last_state = token.type;
	}

	return correct_brackets.empty();
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

				switch (token.op) {
					case '+':
						result = op2 + op1;
						break;
					case '-':
						result = op2 - op1;
						break;
					case '*':
						result = op2 * op1;
						break;
					case '/':
						result = op2 / op1;
						break;
				}

				operands.push(result);
				break;
		}
	}

	return operands.top();
}

double compute(std::string &expr) { // performs all the stages
	vector<Token> tokens = parse(expr);
	if (check(tokens)) {
		tokens = make_postfix_notation(tokens);
		return compute_postfix(tokens);
	}
	else
		throw runtime_error("Expression is incorrect, try again\n");
}