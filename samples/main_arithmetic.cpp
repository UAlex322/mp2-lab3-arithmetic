// реализация пользовательского приложения
#include "../include/arithmetic.h"
#include <iostream>
using namespace std;

int main() {
	string expression;
	double value;

	cout << "CALCULATOR" << endl;
	cout << "Available operations: '+', '-', '*', '/'" << endl;
	cout << "Correct numbers: integers and real numbers with point - 3, 5, -1, 0.57, 3.1415926, -2.71828" << endl;
	cout << "You can enter 'exit' anytime you want to exit" << endl << endl;

	while (true) {
		cout << "Enter the expression and press Enter: ";
		getline(cin, expression);
		expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

		if (expression == "exit")
			break;

		bool exception_happened = true;
		cout.precision(16);
		while (exception_happened) {
			try {
				value = compute(expression);
				exception_happened = false;
			}
			catch (const runtime_error &err) {
				cout << err.what() << endl;

				cout << "Enter the expression and press Enter: ";
				getline(cin, expression);
				expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

				if (expression == "exit")
					break;
			}
		}
		if (expression == "exit") // exit if "exit" was entered in exception-loop
			break;

		cout << "Result: " << value << endl << endl;
	}

	cout << "Exiting..." << endl;
	return 0;
}
