#include "../../persistence/Queue.hpp"
#include <gtest/gtest.h>

#include <vector>

using namespace persistence::queue;

TEST(QSimple, Example) {
	Queue<int> p0;
	EXPECT_EQ(p0.Size(), 0);
	Queue<int> p1 = p0.Enqueue(5);
	Queue<int> p2 = p1.Enqueue(7);
	Queue<int> p3 = p2.Enqueue(6);
	EXPECT_EQ(p3.Size(), 3);
	Queue<int> tmp = p3;
	for(int x : {5, 7, 6}) {
		EXPECT_EQ(tmp.First(), x);
		tmp = tmp.Dequeue();
	}
	Queue<int> p4 = p2.Dequeue();
	EXPECT_EQ(p3.First(), 5);
	Queue<int> p5 = p4.Enqueue(9);
	EXPECT_EQ(p4.First(), 7);
	Queue<int> p6 = p0.Enqueue(5);
	EXPECT_EQ(p5.First(), 7);
	EXPECT_EQ(p6.First(), 5);
}

TEST(QSimple, VecIdentity) {
	for(int t = 0; t < 5; t++) {
		std::vector<int> v;
		Queue<int> p;
		for(int i = 0; i < 10000; i++) {
			v.push_back(rand() * RAND_MAX + rand());
			p = p.Enqueue(v[i]);
			EXPECT_EQ(p.Size(), v.size());
		}
		for(int i = 0; i < 10000; i++) {
			EXPECT_EQ(p.First(), v[i]);
			p = p.Dequeue();
			EXPECT_EQ(p.Size(), v.size() - i - 1);
		}
	}
}

TEST(QSimple, NonInt) {
	using std::string;
	Queue<string> st1 = Queue<string>();
	st1 = st1.Enqueue("oioioi");
	EXPECT_EQ(st1.First(), "oioioi");
	st1 = st1.Enqueue("omar");
	EXPECT_EQ(st1.First(), "oioioi");
	EXPECT_EQ(st1.Dequeue().First(), "omar");
	EXPECT_EQ(st1.K_th(1), "oioioi");
	Queue<double> st2 = Queue<double>();
	st2 = st2.Enqueue(0.1234);
	EXPECT_LT(st2.First(), 0.15);
	Queue<bool> st3 = Queue<bool>();
	for(int i = 0; i < 1000; i++)
		st3 = st3.Enqueue((i % 15) == 7);
	for(int i = 0; i < 100; i++) {
		EXPECT_EQ(st3.First(), ((i % 15) == 7));
		st3 = st3.Dequeue();
	}
}

TEST(QPersistence, Vector) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v, perm;
		std::vector<Queue<int>> st;
		Queue<int> p = Queue<int>();
		Queue<int> all = Queue<int>();
		for(int i = 0; i < 100000; i++) {
			v.push_back(rand() * RAND_MAX + rand());
			p = p.Enqueue(v.back());
			all = all.Enqueue(v.back());
			if(i >= 100) p = p.Dequeue();
			st.push_back(p);
			perm.push_back(i);
		}
		std::random_shuffle(perm.begin(), perm.end());
		for(int i = 0; i < 100000; i++) {
			EXPECT_EQ(st[perm[i]].First(), v[std::max(0, perm[i] - 99)]);
			EXPECT_EQ(all.K_th(perm[i] + 1), v[perm[i]]);
		}
	}
}

namespace {
std::vector<Queue<bool>> vbm;

const int D = 16;
}
void rec(int d, Queue<bool> s) {
	if(d == D)
		vbm.push_back(s);
	else {
		rec(d + 1, s.Enqueue(false));
		rec(d + 1, s.Enqueue(true));
	}
}

TEST(QPersistence, Bitmasks) {
	vbm.clear();
	rec(0, Queue<bool>());
	int p[D];
	for(int i = 0; i < D; i++) p[i] = i;
	for(int i = 0; i < (1 << D); i++) {
		Queue<bool> st = vbm[i];
		for(int d = D - 1; d >= 0; d--) {
			EXPECT_EQ((i >> d) & 1, st.First());
			st = st.Dequeue();
		}
		std::random_shuffle(p, p + D);
		for(int d = 0; d < D; d++)
			EXPECT_EQ((i >> (D - 1 - p[d])) & 1, vbm[i].K_th(p[d] + 1));
	}
}
