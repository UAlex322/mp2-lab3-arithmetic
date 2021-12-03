// реализация пользовательского приложения
#include "../include/arithmetic.h"
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	string expr, no_whitespace_expr;
	double value;

	cout << "CALCULATOR" << endl;
	cout << "Available operations: '+', '-', '*', '/'" << endl;
	cout << "Correct numbers: integers and real numbers with point - 3, 5, -1, 0.57, 3.1415926, -2.71828" << endl;
	cout << "You can enter 'exit' anytime you want to exit" << endl << endl;

	while (true) {
		cout << "Enter the expr and press Enter: ";
		getline(cin, no_whitespace_expr);
		expr = no_whitespace_expr;
		no_whitespace_expr.erase(remove(no_whitespace_expr.begin(), no_whitespace_expr.end(), ' '), no_whitespace_expr.end());

		if (no_whitespace_expr == "exit")
			break;

		bool exception_happened = true;
		cout.precision(16);
		while (exception_happened) {
			try {
				value = compute(expr);
				exception_happened = false;
			}
			catch (const runtime_error &err) {
				cout << "Enter the expr and press Enter: ";
				getline(cin, expr);
				expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());

				if (expr == "exit")
					break;
			}
		}
		if (expr == "exit") // exit if "exit" was entered in exception-loop
			break;

		cout << "Result: " << value << endl << endl;
	}

	cout << "Exiting..." << endl;
	return 0;
}
