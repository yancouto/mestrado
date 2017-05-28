#include "../Stack.hpp"
#include "gtest/gtest.h"

#include <vector>

TEST(STSimple, Example) {
	Stack<int> *p0 = new Stack<int>();
	Stack<int> *p1 = p0->push(5);
	Stack<int> *p2 = p1->push(7);
	Stack<int> *p3 = p2->push(6);
	Stack<int> *tmp = p3;
	for(int x : {6, 7, 5}) {
		EXPECT_EQ(tmp->top(), x);
		tmp = tmp->pop();
	}
	Stack<int> *p4 = p2->pop();
	EXPECT_EQ(p3->top(), 6);
	Stack<int> *p5 = p4->push(9);
	EXPECT_EQ(p4->top(), 5);
	Stack<int> *p6 = p0->push(5);
	EXPECT_EQ(p5->top(), 9);
	EXPECT_EQ(p6->top(), 5);
}

TEST(STSimple, Reverse) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v;
		Stack<int> *p = new Stack<int>();
		for(int i = 0; i < 100000; i++) {
			v.push_back(rand() * rand() + rand());
			p = p->push(v[i]);
		}
		std::reverse(v.begin(), v.end());
		for(int i = 0; i < 100000; i++) {
			EXPECT_EQ(p->top(), v[i]);
			p = p->pop();
		}
	}
}

TEST(STSimple, NonInt) {
	using std::string;
	Stack<string> *st1 = new Stack<string>();
	st1 = st1->push("oioioi");
	EXPECT_EQ(st1->top(), "oioioi");
	st1 = st1->push("omar");
	EXPECT_EQ(st1->top(), "omar");
	EXPECT_EQ(st1->pop()->top(), "oioioi");
	Stack<double> *st2 = new Stack<double>();
	st2->push(0.1234);
	EXPECT_LT(st2->top(), 0.15);
	Stack<bool> *st3 = new Stack<bool>();
	for(int i = 0; i < 1000; i++)
		st3 = st3->push((i % 15) == 7);
	for(int i = 999; i >= 0; i--) {
		EXPECT_EQ(st3->top(), ((i % 15) == 7));
		st3 = st3->pop();
	}
}

TEST(STPersistence, Vector) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v, perm;
		std::vector<Stack<int>*> st;
		Stack<int> *p = new Stack<int>();
		for(int i = 0; i < 100000; i++) {
			v.push_back(rand() * rand() + rand());
			p = p->push(v.back());
			st.push_back(p);
			perm.push_back(i);
		}
		std::random_shuffle(perm.begin(), perm.end());
		for(int i = 0; i < 100000; i++)
			EXPECT_EQ(st[perm[i]]->top(), v[perm[i]]);
	}
}
