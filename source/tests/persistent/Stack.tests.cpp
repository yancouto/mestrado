#include "../../persistent/Stack.hpp"
#include <gtest/gtest.h>

#include <vector>

using namespace stack;

TEST(STSimple, Example) {
	Stack<int> p0;
	Stack<int> p1 = p0.Push(5);
	Stack<int> p2 = p1.Push(7);
	Stack<int> p3 = p2.Push(6);
	Stack<int> tmp = p3;
	for(int x : {6, 7, 5}) {
		EXPECT_EQ(tmp.Top(), x);
		tmp = tmp.Pop();
	}
	Stack<int> p4 = p2.Pop();
	EXPECT_EQ(p3.Top(), 6);
	Stack<int> p5 = p4.Push(9);
	EXPECT_EQ(p4.Top(), 5);
	Stack<int> p6 = p0.Push(5);
	EXPECT_EQ(p5.Top(), 9);
	EXPECT_EQ(p6.Top(), 5);
}

TEST(STSimple, Reverse) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v;
		Stack<int> p;
		for(int i = 0; i < 100000; i++) {
			v.push_back(rand() * rand() + rand());
			p = p.Push(v[i]);
		}
		std::reverse(v.begin(), v.end());
		for(int i = 0; i < 100000; i++) {
			EXPECT_EQ(p.Top(), v[i]);
			p = p.Pop();
		}
	}
}

TEST(STSimple, NonInt) {
	using std::string;
	Stack<string> st1 = Stack<string>();
	st1 = st1.Push("oioioi");
	EXPECT_EQ(st1.Top(), "oioioi");
	st1 = st1.Push("omar");
	EXPECT_EQ(st1.Top(), "omar");
	EXPECT_EQ(st1.Pop().Top(), "oioioi");
	EXPECT_EQ(st1.K_th(1), "oioioi");
	Stack<double> st2 = Stack<double>();
	st2 = st2.Push(0.1234);
	EXPECT_LT(st2.Top(), 0.15);
	Stack<bool> st3 = Stack<bool>();
	for(int i = 0; i < 1000; i++)
		st3 = st3.Push((i % 15) == 7);
	for(int i = 999; i >= 0; i--) {
		EXPECT_EQ(st3.Top(), ((i % 15) == 7));
		st3 = st3.Pop();
	}
}

TEST(STPersistence, Vector) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v, perm;
		std::vector<Stack<int>> st;
		Stack<int> p = Stack<int>();
		for(int i = 0; i < 100000; i++) {
			v.push_back(rand() * rand() + rand());
			p = p.Push(v.back());
			st.push_back(p);
			perm.push_back(i);
		}
		std::random_shuffle(perm.begin(), perm.end());
		for(int i = 0; i < 100000; i++) {
			EXPECT_EQ(st[perm[i]].Top(), v[perm[i]]);
			EXPECT_EQ(st.back().K_th(v.size() - 1 - perm[i]), v[perm[i]]);
		}
	}
}

namespace {
std::vector<Stack<bool>> vbm;

const int D = 16;
}
void rec(int d, Stack<bool> s) {
	if(d == D)
		vbm.push_back(s);
	else {
		rec(d + 1, s.Push(false));
		rec(d + 1, s.Push(true));
	}
}

TEST(STPersistence, Bitmasks) {
	vbm.clear();
	rec(0, Stack<bool>());
	int p[D];
	for(int i = 0; i < D; i++) p[i] = i;
	for(int i = 0; i < (1 << D); i++) {
		Stack<bool> st = vbm[i];
		for(int d = 0; d < D; d++) {
			EXPECT_EQ((i >> d) & 1, st.Top());
			st = st.Pop();
		}
		std::random_shuffle(p, p + D);
		for(int d = 0; d < D; d++)
			EXPECT_EQ((i >> p[d]) & 1, vbm[i].K_th(p[d]));
	}
}
