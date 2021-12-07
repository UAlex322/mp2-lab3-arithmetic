// тесты для вычисления арифметических выражений

#include "arithmetic.h"
#include <cstdlib>
#include <ctime>
#include <gtest.h>
using namespace std;

TEST(arithmetic, positive_number_is_ok) {
	ASSERT_NO_THROW(Expression expr("3"); EXPECT_DOUBLE_EQ(expr.compute(), 3.0));
}

TEST(arithmetic, negative_number_is_ok) {
	ASSERT_NO_THROW(Expression expr("-3"); EXPECT_DOUBLE_EQ(expr.compute(), -3.0));
}

TEST(arithmetic, sum_of_two_positive_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("3+5"); EXPECT_DOUBLE_EQ(expr.compute(), 8.0));
}

TEST(arithmetic, difference_of_two_positive_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("3-5"); EXPECT_DOUBLE_EQ(expr.compute(), -2.0));
}

TEST(arithmetic, product_of_two_positive_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("3*5"); EXPECT_DOUBLE_EQ(expr.compute(), 15.0));
}

TEST(arithmetic, quotient_of_two_positive_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("3/5"); EXPECT_DOUBLE_EQ(expr.compute(), 0.6));
}

TEST(arithmetic, sum_of_negative_and_positive_number_is_ok) {
	ASSERT_NO_THROW(Expression expr1("-55+34"); EXPECT_DOUBLE_EQ(expr1.compute(), -21.0));
	ASSERT_NO_THROW(Expression expr2("34+(-55)"); EXPECT_DOUBLE_EQ(expr2.compute(), -21.0));
	
}

TEST(arithmetic, product_of_positive_and_negative_number_is_ok) {
	ASSERT_NO_THROW(Expression expr1("-13*8"); EXPECT_DOUBLE_EQ(expr1.compute(), -104.0));
	ASSERT_NO_THROW(Expression expr2("8*(-13)"); EXPECT_DOUBLE_EQ(expr2.compute(), -104.0));	
}

TEST(arithmetic, quotient_positive_and_negative_number_is_ok) {
	ASSERT_NO_THROW(Expression expr1("(-3)/5"); EXPECT_DOUBLE_EQ(expr1.compute(), -0.6));
	ASSERT_NO_THROW(Expression expr2("3/(-5)"); EXPECT_DOUBLE_EQ(expr2.compute(), -0.6));
}

TEST(arithmetic, sum_of_two_negative_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("(-35)+(-34)"); EXPECT_DOUBLE_EQ(expr.compute(), -69.0));
}

TEST(arithmetic, difference_of_two_negative_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("(-3)-(-5)"); EXPECT_DOUBLE_EQ(expr.compute(), 2.0));
}

TEST(arithmetic, product_of_two_negative_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("(-14)*(-30)"); EXPECT_DOUBLE_EQ(expr.compute(), 420.0));
}

TEST(arithmetic, quotient_of_two_negative_numbers_is_ok) {
	ASSERT_NO_THROW(Expression expr("(-3)/(-5)"); EXPECT_DOUBLE_EQ(expr.compute(), 0.6));
}

TEST(arithmetic, number_in_brackets_is_ok) {
	ASSERT_NO_THROW(Expression expr1("(((5)))"); EXPECT_DOUBLE_EQ(expr1.compute(), 5.0));
	ASSERT_NO_THROW(Expression expr2("(((-3)))"); EXPECT_DOUBLE_EQ(expr2.compute(), -3.0));
}

TEST(arithmetic, order_with_brackets_is_ok) {
	ASSERT_NO_THROW(Expression expr1("2*(2+3)"); EXPECT_EQ(expr1.compute(), 10.0));
	ASSERT_NO_THROW(Expression expr2("(2-3)/5"); EXPECT_EQ(expr2.compute(), -0.2));
}

TEST(arithmetic, multiple_bracket_expression_is_ok) {
	ASSERT_NO_THROW(Expression expr("(-3+(-5+(-7+(-9-(11)))))"); EXPECT_EQ(expr.compute(), -35.0));
}

TEST(arithmetic, random_sum_expression_is_ok) {
	srand(time(0));

	string expr;
	double ans = 0.0;

	for (size_t i = 0; i < 10; ++i) {
		int num = rand();
		expr += to_string(num) + "+";
		ans += num;
	}
	expr.erase(expr.end()-1);
	EXPECT_EQ(Expression(expr).compute(), ans);
}

TEST(arithmetic, floating_point_number_is_ok) {
	ASSERT_NO_THROW(Expression expr("27.18"); EXPECT_EQ(expr.compute(), 27.18));
}

TEST(arithmetic, throw_when_incorrect_dot_is_ok) {
	ASSERT_ANY_THROW(Expression expr("3.5.5"));
}

TEST(arithmetic, floating_point_operations_are_ok) {
	string expr[5]{"3.5+5.4", "4.5-3.19", "3.3*4.5", "7.2/2.5", "4.5+(7.2*5.0)"};
	double ans[5]{8.9, 1.31, 14.85, 2.88, 40.5};

	for (size_t i = 0; i < 5; ++i) {
		ASSERT_NO_THROW(Expression(expr[i]).compute());
		EXPECT_EQ(Expression(expr[i]).compute(), ans[i]);
	}
}

TEST(arithmetic, whitespaces_in_expression_are_ok) {
	ASSERT_NO_THROW(Expression expr("     3.5   +     5.8 "); EXPECT_EQ(expr.compute(), 9.3));
}

TEST(arithmetic, throw_when_incorrect_symbols_is_ok) {
	string exprs[10]{"%", "3 #$@ 5", "6.28, 3.14", "->3 11 !6", ".21", "3 > 5", "5[3]*41", "3+.5", "three pl%s ten", "a|b"};
	for (size_t i = 0; i < 10; ++i)
		ASSERT_ANY_THROW(Expression expr(exprs[i]); expr.compute());
}

TEST(arithmetic, throw_when_unpaired_brackets_is_ok) {
	string exprs[4]{")", "(", "(3+1))", "(3+5-(1+3)"};
	for (size_t i = 0; i < 4; ++i)
		ASSERT_ANY_THROW(Expression expr(exprs[i]); expr.compute());
}

TEST(arithmetic, throw_when_empty_expression_is_ok) {
	ASSERT_ANY_THROW(Expression expr("         "); expr.compute());
}

TEST(arithmetic, throw_when_missing_operations_is_ok) {
	ASSERT_ANY_THROW(Expression expr("3 5"));
	ASSERT_ANY_THROW(Expression expr("0.5772156 1.11211331"));
}

TEST(arithmetic, variables_are_ok) {
	string exprs[5]{"a", "VaR", "constant", "l", "xyz"};
	for (size_t i = 0; i < 5; ++i)
		ASSERT_NO_THROW(Expression expr(exprs[i]));
}

TEST(arithmetic, operations_with_variables_are_ok) {
	string exprs[5]{"a + b", "x * y / z", "(alpha + beta) * Gamma", "(pi*(r*r))", "(((pi)))*(pi/six)"};
	for (size_t i = 0; i < 5; ++i)
		ASSERT_NO_THROW(Expression expr(exprs[i]));
}

TEST(arithmetic, unary_minus_is_ok) {
	string exprs[5]{"4/-1", "4--1", "3/-(1+4)", "4/--(1)", "1---1"};
	double equal[5]{-4.0, 5.0, -0.6, 4.0, 0.0};
	for (size_t i = 0; i < 5; ++i)
		EXPECT_DOUBLE_EQ(Expression(exprs[i]).compute(), equal[i]);
}

TEST(arithmetic, throw_when_incorrect_unary_minus_is_ok) {
	string exprs[5]{"--", "1--", "3-+3", "-)", "-("};
	for (size_t i = 0; i < 5; ++i)
		ASSERT_ANY_THROW(Expression expr(exprs[i]));
}