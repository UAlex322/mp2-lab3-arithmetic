// реализация пользовательского приложения
#include "../include/arithmetic.h"	
#include <iostream>
#include <algorithm>
#include <sstream>

bool is_equal(const string &str, const string &compare) {
	stringstream ss;
	string word;
	size_t counter = 0;

	ss << str;
	while (ss >> word) ++counter;
	return (counter == 1 && word == compare);
}

int main() {
	string expr, no_whitespace_expr, choose;
	double value;

	cout << "CALCULATOR" << endl;
	cout << "Available operations: '+', '-', '*', '/', brackets - '(' and ')'" << endl;
	cout << "Correct numbers:\n1) Integers, real numbers with point: 3, 5, -1, 0.57, 3.1415926, -2.71828" << endl;
	cout << "2) Variables, only with letters: x, y, variable, number" << endl << endl;

	while (true) {
		cout << "Enter the expr and press Enter (enter 'exit' to exit): ";
		getline(cin, expr);

		if (is_equal(expr, "exit"))
			break;

		bool exception_happened = true;
		cout.precision(16);
		while (exception_happened) {
			try {
				Expression curr_expr(expr);
				while (true) {
					if (curr_expr.has_no_variables()) {
						cout << "Result: " << curr_expr.compute() << endl << endl;
						break;
					}
					else {
						double value = curr_expr.compute();
						cout << "Result: " << value << endl << endl;

						cout << "Do you want to continue with this expression? (yes/no/exit): ";
						getline(cin, choose);
						while (choose != "yes" && choose != "no" && choose != "exit") {
							cout << "Incorrect answer, try again" << endl;
							cout << "Do you want to continue with this expression? (yes/no/exit): ";
							getline(cin, choose);
						}

						if (is_equal(choose, "no") || is_equal(choose, "exit"))
							break;

					}
				}
				exception_happened = false;
			}
			catch (const runtime_error &err) {
				cout << "Enter the expr and press Enter (enter 'exit' to exit): ";
				getline(cin, expr);
				
				if (is_equal(expr, "exit"))
					break;
			}
		}

		if (is_equal(expr, "exit") || is_equal(choose, "exit")) // exit if "exit" was entered in exception-loop
			break;
		
	}

	cout << "Exiting..." << endl;
	return 0;
}
