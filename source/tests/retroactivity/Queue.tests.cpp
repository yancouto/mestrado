#include "../../retroactivity/Queue.hpp"
#include <gtest/gtest.h>

#include <vector>

using namespace retroactivity::queue;

TEST(QSimple, Example) {
	Queue<int> q;
	q.Insert_Enqueue(10, 1);
	q.Insert_Enqueue(3, 2);
	EXPECT_EQ(q.Query_First(10), 2);
	EXPECT_EQ(q.Query_K_th(10, 2), 1);
	q.Insert_Dequeue(5);
	EXPECT_EQ(q.Query_First(10), 1);
	q.Delete_Dequeue(5);
	EXPECT_EQ(q.Query_First(10), 2);
	EXPECT_EQ(q.Query_K_th(10, 2), 1);
}

TEST(QSimple, VecIdentity) {
	for(int t = 0; t < 5; t++) {
		std::vector<int> v;
		Queue<int> p;
		for(int i = 0; i < 10000; i++) {
			v.push_back(rand() * RAND_MAX + rand());
			p.Insert_Enqueue(i, v[i]);
		}
		for(int i = 0; i < 10000; i++) {
			EXPECT_EQ(p.Query_First(10000 + i), v[i]);
			EXPECT_EQ(p.Query_K_th(10000 - 1, i + 1), v[i]);
			p.Insert_Dequeue(10000 + i);
		}
	}
}

TEST(QSimple, NonInt) {
	using std::string;
	Queue<string> q1 = Queue<string>();
	q1.Insert_Enqueue(100, "oioioi");
	EXPECT_EQ(q1.Query_First(1000), "oioioi");
	q1.Insert_Enqueue(200, "omar");
	EXPECT_EQ(q1.Query_First(300), "oioioi");
	q1.Insert_Dequeue(250);
	EXPECT_EQ(q1.Query_First(240), "oioioi");
	EXPECT_EQ(q1.Query_First(260), "omar");
	Queue<double> q2;
	q2.Insert_Enqueue(-666, 0.1234);
	EXPECT_LT(q2.Query_First(0), 0.15);
	Queue<bool> q3;
	for(int i = 0; i < 1000; i++)
		q3.Insert_Enqueue(i, (i % 15) == 7);
	for(int i = 100; i >= 0; i--)
		EXPECT_EQ(q3.Query_K_th(i, i + 1), ((i % 15) == 7));
}
