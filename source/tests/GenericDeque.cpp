#include <vector>
#include <deque>
#include <string>

double rnd() { return rand() / double(RAND_MAX); }

void eq_vec(Deque<int> d, std::vector<int> v) {
	Deque<int> tmp = d, tmp2 = d;
	for(int i = 0; i < int(v.size()); i++) {
		EXPECT_EQ(d.K_th(i), v[i]);
		EXPECT_EQ(tmp.Front(), v[i]);
		EXPECT_EQ(tmp2.Back(), v[v.size() - 1 - i]);
		tmp = tmp.PopFront();
		tmp2 = tmp2.PopBack();
	}
}

TEST(DSimple, Simple) {
	Deque<int> d0;
	d0 = d0.PushBack(2);
	d0 = d0.PushFront(3);
	EXPECT_EQ(d0.Back(), 2);
	EXPECT_EQ(d0.Front(), 3);
	d0 = d0.PopBack();
	EXPECT_EQ(d0.Front(), 3);
	EXPECT_EQ(d0.Back(), 3);

}

TEST(DSimple, Example) {
	Deque<int> d0 = Deque<int>();
	Deque<int> d1 = d0.PushBack(3);
	eq_vec(d1, {3});
	Deque<int> d2 = d1.PushBack(4);
	eq_vec(d2, {3, 4});
	Deque<int> d3 = d2.PushFront(2);
	eq_vec(d3, {2, 3, 4});
	Deque<int> d4 = d3.PushFront(1);
	eq_vec(d4, {1, 2, 3, 4});
	Deque<int> d5 = d3.PopBack();
	eq_vec(d5, {2, 3});
	Deque<int> d6 = d5.PopBack();
	eq_vec(d6, {2});
	Deque<int> d7 = d6.PushFront(9);
	eq_vec(d7, {9, 2});
	Deque<int> d8 = d6.PopFront();
	eq_vec(d8, {});
	Deque<int> d9 = d8.PushBack(6);
	eq_vec(d9, {6});
}

TEST(DSimple, VectorAndKth) {
	for(int t = 0; t < 5; t++) {
		std::vector<int> v, p;
		Deque<int> d;
		for(int i = 0; i < 10000; i++) {
			v.push_back(rand() * rand() + rand());
			d = d.PushBack(v.back());
			p.push_back(i);
		}
		std::cout << "Done build" << std::endl;
		eq_vec(d, v);
		std::random_shuffle(p.begin(), p.end());
		for(int i = 0; i < int(v.size()); i++)
			EXPECT_EQ(d.K_th(i), v[i]);
		int l = 0, r = v.size() - 1;
		while(l <= r) {
			if(rnd() <= .5) {
				EXPECT_EQ(d.Front(), v[l++]);
				d = d.PopFront();
			} else {
				EXPECT_EQ(d.Back(), v[r--]);
				d = d.PopBack();
			}
		}
	}
}

using std::string;
string randomStr() {
	string x;
	for(int i = 0; i < 10; i++)
		x += char(rand());
	return x;
}

TEST(DSimple, StdDeque) {
	for(double p : {.5, .5, .1, .9, .001, .999}) {
		Deque<string> d; std::deque<string> sd;
		for(int i = 0; i < 50000; i++) {
			if(rnd() <= p && !sd.empty()) {
				if(rnd() <= .5) {
					EXPECT_EQ(d.Back(), sd.back());
					d = d.PopBack();
					sd.pop_back();
				} else {
					EXPECT_EQ(d.Front(), sd.front());
					d = d.PopFront();
					sd.pop_front();
				}
			} else {
				string x = randomStr();
				if(rnd() <= .5) {
					sd.push_back(x);
					d = d.PushBack(x);
				} else {
					sd.push_front(x);
					d = d.PushFront(x);
				}
			}
		}
	}
}

TEST(DPersist, Simple) {
	std::vector<Deque<int>> dq;
	Deque<int> d;
	for(int i = 0; i < 100; i++) {
		d = d.PushBack(i);
		dq.push_back(d);
	}
	for(int i = 0; i < 100; i++) {
		EXPECT_EQ(d.K_th(i), i);
		EXPECT_EQ(dq[i].Back(), i);
	}
}

TEST(DPersist, StoreRand) {
	std::vector<int> rnd, p;
	std::vector<Deque<int>> dq;
	Deque<int> d;
	for(int i = 0; i < 100000; i++) {
		rnd.push_back(rand());
		if(rnd.back() & 1) d = d.PushBack(i);
		else d = d.PushFront(i);
		dq.push_back(d);
		p.push_back(i);
	}
	std::random_shuffle(p.begin(), p.end());
	for(int i = 0; i < 100000; i++) {
		if(rnd[p[i]] & 1) EXPECT_EQ(p[i], dq[p[i]].Back());
		else EXPECT_EQ(p[i], dq[p[i]].Front());
	}
}

TEST(DSimple, StdDequeStored) {
	using pii = std::pair<int, int>;
	struct Query {
		int i;
		int front;
		pii val;
	};
	for(double p : {.5, .5, .1, .9, .001, .999}) {
		Deque<pii> d; std::deque<pii> sd;
		std::vector<Deque<pii>> dq;
		std::vector<Query> qu;
		for(int i = 0; i < 100000; i++) {
			if(rnd() <= p && !sd.empty()) {
				if(rnd() <= .5) {
					qu.push_back({i - 1, 0, sd.back()});
					EXPECT_EQ(d.Back(), sd.back());
					d = d.PopBack();
					sd.pop_back();
				} else {
					qu.push_back({i - 1, 1, sd.front()});
					EXPECT_EQ(d.Front(), sd.front());
					d = d.PopFront();
					sd.pop_front();
				}
			} else {
				pii x(rand(), rand());
				if(rnd() <= .5) {
					qu.push_back({i, 0, x});
					sd.push_back(x);
					d = d.PushBack(x);
				} else {
					qu.push_back({i, 1, x});
					sd.push_front(x);
					d = d.PushFront(x);
				}
			}
			if(sd.size() > 3) {
				int x = (random() % (int(sd.size()) - 2)) + 2;
				qu.push_back({i, x, sd[x]});
			}
			dq.push_back(d);
		}
		random_shuffle(qu.begin(), qu.end());
		for(Query q : qu) {
			if(q.front == 0)
				EXPECT_EQ(dq[q.i].Back(), q.val);
			else if(q.front == 1)
				EXPECT_EQ(dq[q.i].Front(), q.val);
			else
				EXPECT_EQ(dq[q.i].K_th(q.front), q.val);
		}
	}
}
