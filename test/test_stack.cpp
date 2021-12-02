// тесты для стека

#include <gtest.h>
#include <cstdlib>
#include "stack.h"

TEST(MyStack, can_create_default_stack) {
	ASSERT_NO_THROW(MyStack<int> a);
}

TEST(MyStack, default_stack_is_empty) {
	MyStack<int> a;
	EXPECT_EQ(a.size(), 0);
}

TEST(MyStack, can_create_stack_with_definite_capacity) {
	ASSERT_NO_THROW(MyStack<int> a(10));
}

TEST(MyStack, can_create_stack_by_copy_constructor) {
	MyStack<int> a(10);
	ASSERT_NO_THROW(MyStack<int> b = a);
}

TEST(MyStack, copy_constructor_is_correct) {
	srand(time(0));

	MyStack<int> a(10);
	for (size_t i = 0; i < 10; ++i)
		a.push(rand());
	MyStack<int> b = a;

	EXPECT_EQ(a.size(), b.size());
	for (size_t i = 0; i < 10; ++i) {
		EXPECT_EQ(a.top(), b.top());
		a.pop(), b.pop();
	}
}

TEST(MyStack, push_is_correct_for_empty_stack) {
	MyStack<int> a;

	ASSERT_NO_THROW(a.push(3));
	EXPECT_EQ(a.size(), 1);
	EXPECT_EQ(a.top(), 3);
}

TEST(MyStack, push_is_correct_for_medium_stack) {
	MyStack<int> a(10);

	ASSERT_NO_THROW(a.push(2); a.push(3); a.push(4); a.push(5));
	EXPECT_EQ(a.size(), 4);
	EXPECT_EQ(a.top(), 5);
}

TEST(MyStack, push_is_correct_for_full_stack) {
	MyStack<int> a(10);

	for (size_t i = 0; i < 10; ++i)
		ASSERT_NO_THROW(a.push(i));

	ASSERT_NO_THROW(a.push(10));
	EXPECT_EQ(a.size(), 11);
	EXPECT_EQ(a.top(), 10);
}

TEST(MyStack, pop_is_correct_for_empty_stack) {
	MyStack<int> a;

	ASSERT_ANY_THROW(a.pop());
}

TEST(MyStack, pop_is_correct_for_non_empty_stack) {
	MyStack<int> a;

	a.push(1), a.push(2);
	ASSERT_NO_THROW(a.pop());
	EXPECT_EQ(a.top(), 1);
}

TEST(MyStack, top_is_correct_for_empty_stack) {
	MyStack<int> a;

	ASSERT_ANY_THROW(a.top());
}

TEST(MyStack, top_is_correct_for_non_empty_stack) {
	srand(time(0));

	MyStack<int> a;
	int arr[10];
	for (int i = 9; i >= 0; --i) {
		arr[i] = rand();
		a.push(arr[i]);
	}

	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(a.top());
		EXPECT_EQ(a.top(), arr[i]);
		a.pop();
	}
}

TEST(MyStack, size_is_correct) {
	srand(time(0));

	MyStack<int> a(0);
	EXPECT_EQ(a.size(), 0);

	for (size_t i = 0; i < 10; ++i) {
		MyStack<int> b;
		size_t stack_size = rand() % 100;
		for (size_t i = 0; i < stack_size; ++i)
			b.push(42);
		EXPECT_EQ(b.size(), stack_size);
	}
}

TEST(MyStack, clear_is_correct) {
	MyStack<int> a;
	for (size_t i = 0; i < 42; ++i)
		a.push(42);

	ASSERT_NO_THROW(a.clear());
	EXPECT_EQ(a.size(), 0);
}

TEST(MyStack, empty_is_correct) {
	MyStack<int> a;

	EXPECT_EQ(a.empty(), true);

	a.push(42);
	EXPECT_EQ(a.empty(), false);

	a.pop();
	EXPECT_EQ(a.empty(), true);
}

