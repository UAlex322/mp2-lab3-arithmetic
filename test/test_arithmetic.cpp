// тесты для вычисления арифметических выражений

#include "arithmetic.h"
#include <cstdlib>
#include <ctime>
#include <gtest.h>
using namespace std;

TEST(arithmetic, positive_number_is_ok) {
	string expr = "3";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 3.0);
}

TEST(arithmetic, negative_number_is_ok) {
	string expr = "-3";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), -3.0);
}

TEST(arithmetic, sum_of_two_positive_numbers_is_ok) {
	string expr = "3+5";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 8.0);
}

TEST(arithmetic, difference_of_two_positive_numbers_is_ok) {
	string expr = "3-5";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), -2.0);
}

TEST(arithmetic, product_of_two_positive_numbers_is_ok) {
	string expr = "3*5";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 15.0);
}

TEST(arithmetic, quotient_of_two_positive_numbers_is_ok) {
	string expr = "3/5";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 0.6);
}

TEST(arithmetic, sum_of_negative_and_positive_number_is_ok) {
	string expr1 = "-55+34";
	string expr2 = "34+(-55)";

	ASSERT_NO_THROW(compute(expr1); compute(expr2));
	EXPECT_DOUBLE_EQ(compute(expr1), -21.0);
	EXPECT_DOUBLE_EQ(compute(expr2), -21.0);
}

TEST(arithmetic, product_of_positive_and_negative_number_is_ok) {
	string expr1 = "-13*8";
	string expr2 = "8*(-13)";

	ASSERT_NO_THROW(compute(expr1); compute(expr2));
	EXPECT_DOUBLE_EQ(compute(expr1), -104.0);
	EXPECT_DOUBLE_EQ(compute(expr2), -104.0);
}

TEST(arithmetic, quotient_positive_and_negative_number_is_ok) {
	string expr1 = "(-3)/5";
	string expr2 = "3/(-5)";

	ASSERT_NO_THROW(compute(expr1); compute(expr2));
	EXPECT_DOUBLE_EQ(compute(expr1), -0.6);
	EXPECT_DOUBLE_EQ(compute(expr2), -0.6);
}

TEST(arithmetic, sum_of_two_negative_numbers_is_ok) {
	string expr = "(-35)+(-34)";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), -69.0);
}

TEST(arithmetic, difference_of_two_negative_numbers_is_ok) {
	string expr = "(-3)-(-5)";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 2.0);
}

TEST(arithmetic, product_of_two_negative_numbers_is_ok) {
	string expr = "(-14)*(-30)";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 420.0);
}

TEST(arithmetic, quotient_of_two_negative_numbers_is_ok) {
	string expr = "(-3)/(-5)";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_DOUBLE_EQ(compute(expr), 0.6);
}

TEST(arithmetic, number_in_brackets_is_ok) {
	string expr1 = "(((5)))";
	string expr2 = "(((-3)))";

	ASSERT_NO_THROW(compute(expr1); compute(expr2));
	EXPECT_DOUBLE_EQ(compute(expr1), 5.0);
	EXPECT_DOUBLE_EQ(compute(expr2), -3.0);
}

TEST(arithmetic, order_with_brackets_is_ok) {
	string expr1 = "2*(2+3)";
	string expr2 = "(2-3)/5";

	ASSERT_NO_THROW(compute(expr1); compute(expr2));
	EXPECT_EQ(compute(expr1), 10.0);
	EXPECT_EQ(compute(expr2), -0.2);
}

TEST(arithmetic, multiple_bracket_expression_is_ok) {
	string expr = "(-3+(-5+(-7+(-9-(11)))))";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_EQ(compute(expr), -35.0);
}

TEST(arithmetic, random_sum_expression_is_ok) {
	srand(time(0));

	string expr = "";
	double ans = 0.0;

	for (size_t i = 0; i < 10; ++i) {
		int num = rand();
		expr += to_string(num) + "+";
		ans += num;
	}
	expr.erase(expr.end()-1);
	EXPECT_EQ(compute(expr), ans);
}

TEST(arithmetic, floating_point_number_is_ok) {
	string expr = "27.18";

	ASSERT_NO_THROW(compute(expr));
	EXPECT_EQ(compute(expr), 27.18);
}

TEST(arithmetic, throw_when_incorrect_dot_is_ok) {
	string expr = "3.5.5";

	ASSERT_ANY_THROW(compute(expr));
}

TEST(arithmetic, floating_point_operations_are_ok) {
	string expr[5]{"3.5+5.4", "4.5-3.19", "3.3*4.5", "7.2/2.5", "4.5+(7.2*5.0)"};
	double ans[5]{8.9, 1.31, 14.85, 2.88, 40.5};

	for (size_t i = 0; i < 5; ++i) {
		ASSERT_NO_THROW(compute(expr[i]));
		EXPECT_EQ(compute(expr[i]), ans[i]);
	}
}

TEST(arithmetic, whitespaces_in_expression_are_ok) {
	string expr = "     3.5   +     5.8 ";
	
	ASSERT_NO_THROW(compute(expr));
	EXPECT_EQ(compute(expr), 9.3);
}

TEST(arithmetic, throw_when_incorrect_symbols_is_ok) {
	string expr[10]{"%", "3 #$@ 5", "6.28 mod 3.14", "->3 11 !6", ".21", "3,5", "5[3]*41", "3+.5", "three plus ten", "a/b"};
	for (size_t i = 0; i < 10; ++i)
		ASSERT_ANY_THROW(compute(expr[i]));
}

TEST(arithmetic, throw_when_unpaired_brackets_is_ok) {
	string expr[4]{")", "(", "(3+1))", "(3+5-(1+3)"};
	for (size_t i = 0; i < 4; ++i)
		ASSERT_ANY_THROW(compute(expr[i]));
}