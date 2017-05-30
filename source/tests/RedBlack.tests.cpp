#include "../RedBlack.hpp"
#include "gtest/gtest.h"

#include <vector>
#include <algorithm>
#include <set>

inline int rnd(int l, int r) {
	double p = rand() / (double(RAND_MAX) + 1);
	return l + (r - l + 1) * p;
}

template<class T> void deb(Node<T> *x, RedBlackTree<T> &rb) {
	if(x == nullptr) return;
	std::cerr << '(';
	deb(rb.Child(x, 0), rb);
	std::cerr << ' ' << x->value << (x->red? " " : "_");
	deb(rb.Child(x, 1), rb);
	std::cerr << ')';
}

template<class T> void debug(Node<T> *root, RedBlackTree<T> &rb) {
	deb(root, rb);
	std::cerr << std::endl;
}

template<class T> int rec(Node<T> *u, T *min, T *max, RedBlackTree<T> &rb) {
	if(u == nullptr) return 0;
	EXPECT_TRUE(u->copy == nullptr) << "Nó ativo não deve ter cópia";
	if(min) EXPECT_FALSE(u->value < *min) << "Não é uma ABB";
	if(max) EXPECT_FALSE(*max < u->value) << "Não é uma ABB";
	//if(rb.Child(u, 0)) EXPECT_EQ(u, rb.Child(u, 0)->parent) << "Links de pai errados";
	//if(rb.Child(u, 1)) EXPECT_EQ(u, rb.Child(u, 1)->parent) << "Links de pai errados";
	int bh1 = rec(rb.Child(u, 0), min, &u->value, rb);
	int bh2 = rec(rb.Child(u, 1), &u->value, max, rb);
	EXPECT_EQ(bh1, bh2) << "Alturas pretas devem ser iguais";
	if(u->red) {
		EXPECT_FALSE(isRed(rb.Child(u, 0))) << "Não devem existir nós vermelhos consecutivos";
		EXPECT_FALSE(isRed(rb.Child(u, 1))) << "Não devem existir nós vermelhos consecutivos";
	}
	return bh1 + (!u->red);
}

// Checa se as propriedades rubronegras são seguidas nos nós ativos
template<class T> void check(RedBlackTree<T> &rb) {
	Node<T> *u = rb.roots.back();
	//debug(u, rb);
	if(u == nullptr) return;
	EXPECT_FALSE(u->red) << "Raiz deve ser preta";
	rec(u, (T*) nullptr, (T*) nullptr, rb);
}

TEST(RBSimple, SimpleDuplicate) {
	RedBlackTree<int> rb;
	rb.insert(1); check(rb);
	rb.insert(1); check(rb);
	Node<int> *u = rb.roots.back();
	EXPECT_TRUE((rb.Child(u, 0) != nullptr) ^ (rb.Child(u, 1) != nullptr)) << "Exatamente um"
	             << " filho";
}

TEST(RBSimple, Case3) {
	RedBlackTree<int> rb;
	rb.insert(1); check(rb);
	rb.insert(3); check(rb);
	rb.insert(2); check(rb);
}

TEST(RBSimple, Insert) {
	RedBlackTree<int> rb1;
	for(int i = 0; i < 10; i++)
		rb1.insert(i);
	check(rb1);
	for(int i = 9; i >= 0; i--)
		EXPECT_FALSE(rb1.Find(rb1.versionCount(), i) == nullptr);
}

TEST(RBSimple, KeepsRedBlack) {
	RedBlackTree<int> rb;
	for(int i = 0; i < 1000; i++) {
		rb.insert(rnd(1, 800));
		check(rb);
	}
}

TEST(RBSimple, Strings) {
	using std::string;
	RedBlackTree<string> rb;
	for(string s : {"oi", "tchau", "teste", "aaa", "aa"}) {
		rb.insert(s);
		check(rb);
	}
	for(string s : {"tchau", "aaa", "teste", "aa", "oi"})
		EXPECT_TRUE(rb.Find(rb.versionCount(), s) != nullptr);
	for(string s : {"a", "aaaa", "oi!", "lalala", "tcha", "chau"})
		EXPECT_FALSE(rb.Find(rb.versionCount(), s) != nullptr);
}

// Coloca os valores em um vetor ordenado
void traverse(Node<int> *u, std::vector<int> &v, RedBlackTree<int> &rb) {
	if(u == nullptr) return;
	traverse(rb.Child(u, 0), v, rb);
	v.push_back(u->value);
	traverse(rb.Child(u, 1), v, rb);
}

// Testa se a árvore rubronegra ordena um vetor específico
void testSort(std::vector<int> &v) {
	RedBlackTree<int> rb;
	for(int x : v)
		rb.insert(x);
	check(rb);
	std::sort(v.begin(), v.end()); // STL sort
	std::vector<int> ans;
	traverse(rb.roots.back(), ans, rb);
	ASSERT_EQ(v, ans) << "Ordenacao incorreta";
}

// Testa T vetores aleatórios com tamanhos de l a r
void randomVecs(int T, int l, int r) {
	for(int t = 0; t < T; t++) {
		int n = rnd(l, r);
		std::vector<int> v;
		for(int i = 0; i < n; i++)
			v.push_back(rand());
		testSort(v);
	}
}

TEST(RBSorting, Repeated) {
	std::vector<int> v (10000, 10000);
	testSort(v);
}

TEST(RBSorting, SortedReversed) {
	for(int t = 0; t < 10; t++) {
		std::vector<int> v;
		for(int i = 0; i < 30000; i++)
			v.push_back(rand() % 30000);
		std::sort(v.begin(), v.end());
		if(t % 2)
			std::reverse(v.begin(), v.end());

		testSort(v);
	}
}

TEST(RBSorting, Small) { randomVecs(1000, 6, 15); }
TEST(RBSorting, Medium) { randomVecs(1000, 100, 1000); }
TEST(RBSorting, Large) { randomVecs(10, 10000, 100000); }

void testPersistence(const std::vector<int> &v) {
	int n = v.size();
	RedBlackTree<int> rb;
	for(int i = 0; i < n; i++)
		rb.insert(v[i]);
	for(int q = 0; q < 2 * n; q++) {
		int x = rnd(0, n - 1);
		int y = rnd(0, n - 1);
		const int *z = rb.Find(x + 1, v[y]);
		if(z != nullptr) EXPECT_EQ(*z, v[y]);
		EXPECT_EQ(z != nullptr, y <= x);
	}
	for(int i = 0; i < n; i++)
		for(int q = -2; q <= 2; q++) {
			if(q + i < 0 || q + i >= n) continue;
			int x = i, y = q + i;
			const int *z = rb.Find(x + 1, v[y]);
			if(z != nullptr) EXPECT_EQ(*z, v[y]);
			EXPECT_EQ(z != nullptr, y <= x);
		}
}

TEST(RBPersistence, SortedAndReversed) {
	for(int n : {10, 100, 1000, 100000}) {
		std::vector<int> v;
		for(int i = 0; i < n; i++)
			v.push_back(i);
		testPersistence(v);
		std::reverse(v.begin(), v.end());
		testPersistence(v);
	}
}


TEST(RBPersistence, Shuffled) {
	for(int n : {10, 100, 1000, 100000}) {
		std::vector<int> v;
		int p = 0;
		for(int i = 0; i < n; i++) {
			p = p * 123123123 + 84737; // não vai ciclar pois não é par
			v.push_back(p);
		}
		random_shuffle(v.begin(), v.end());
		testPersistence(v);
	}
}

void testPersRem(const std::vector<int> &v, int k) {
	int n = v.size();
	RedBlackTree<int> rb;
	for(int i = 0; i < n; i++) {
		rb.insert(v[i]);
		if(i >= k) rb.erase(v[i - k]);
	}
	for(int q = 0; q < 2 * n; q++) {
		int x = rnd(0, n - 1 + std::max(n - k, 0));
		int y = rnd(0, n - 1);
		const int *z = rb.Find(x + 1, v[y]);
		if(z != nullptr) EXPECT_EQ(*z, v[y]);
		int mn = (std::max(x - k, 0) + 1) / 2;
		int mx = x - mn;
		EXPECT_EQ(z != nullptr, mn <= y && y <= mx);
	}
}

TEST(RBPersistence, WithRemove) {
	for(int n : {10, 100, 1000, 50000}) {
		std::vector<int> v;
		for(int i = 0; i < n; i++)
			v.push_back(i);
		testPersRem(v, n / 3);
		std::reverse(v.begin(), v.end());
		testPersRem(v, n / 3);
		std::random_shuffle(v.begin(), v.end());
		testPersRem(v, n / 3);
	}
}

TEST(RBRemove, Simpler) {
	RedBlackTree<int> rb;
	EXPECT_FALSE(rb.erase(10) != nullptr); check(rb);
	rb.insert(12); check(rb);
	EXPECT_TRUE(rb.erase(12) != nullptr); check(rb);
	EXPECT_FALSE(rb.erase(12) != nullptr); check(rb);
	rb.insert(1); check(rb);
	rb.insert(2); check(rb);
	rb.insert(3); check(rb);
	rb.insert(4); check(rb);
	EXPECT_TRUE(rb.erase(2) != nullptr); check(rb);
	EXPECT_TRUE(rb.erase(1) != nullptr); check(rb);
}

TEST(RBRemove, Simple) {
	RedBlackTree<int> rb;
	for(int x : {1, 5, 7, 10})
		rb.insert(x);
	for(int x : {5, 10}) {
		EXPECT_TRUE(rb.erase(x) != nullptr) << "should contain";
		check(rb);
	}
	for(int x : {1, 7, 1, 1})
		EXPECT_TRUE(rb.Find(rb.versionCount(), x) != nullptr) << "should find";
	for(int x : {10, -1, 5, 12})
		EXPECT_FALSE(rb.Find(rb.versionCount(), x) != nullptr) << "should not find";
}

TEST(RBRemove, Odds) {
	RedBlackTree<long long> rb;
	for(int i = 0; i <= 100; i++)
		rb.insert(i);
	check(rb);
	for(int i = 1; i <= 100; i += 2)
		EXPECT_TRUE(rb.erase(i) != nullptr) << "should remove";
	check(rb);
	for(int i = 100; i >= 0; i--)
		EXPECT_EQ(rb.Find(rb.versionCount(), i) != nullptr, (i % 2) == 0);
}

TEST(RBRemove, Invalid) {
	RedBlackTree<char> rb;
	for(int c = 'a'; c <= 'z'; c++)
		rb.insert(c);
	for(int c : {'a', 'z', 'c', 'a', 'a', '?'})
		rb.erase(c);
	EXPECT_TRUE(rb.erase('b') != nullptr);
	EXPECT_FALSE(rb.erase('=') != nullptr);
	check(rb);
	for(int c = 'a' - 12; c <= 'z' + 7; c++)
		EXPECT_EQ(rb.Find(rb.versionCount(), c) != nullptr, c > 'c' && c < 'z');
}

TEST(RBRemove, Repeated) {
	RedBlackTree<int> rb;
	int ct[3] = {0, 0, 0};
	for(int i = 0; i < 50000; i++) {
		int x = rnd(0, 2);
		ct[x]++;
		rb.insert(x);
	}
	int p[3] = {0, 1, 2};
	while(ct[0] + ct[1] + ct[2] > 0) {
		std::random_shuffle(p, p + 3);
		int i = 0;
		while(ct[p[i]] == 0) i++;
		int x = p[i];
		ct[x]--;
		ASSERT_TRUE(rb.erase(x) != nullptr);
		if(((ct[0] + ct[1] + ct[2]) % 2500) == 0) check(rb);
	}
}

TEST(RBRemove, Breaks) {
	RedBlackTree<int> rb;
	for(int x : {3, 5, 1, 4, 2, 6}) {
		rb.insert(x);
		check(rb);
	}
	for(int x : {4, 5, 6}) {
		EXPECT_TRUE(rb.erase(x) != nullptr);
		check(rb);
	}
}

void doLarge(bool isOrdered) {
	std::vector<int> v;
	std::multiset<int> s;
	RedBlackTree<int> rb;
	for(int i = 0; i <= 1000000; i++) {
		int p = rnd(1, 100);
		if(p <= 30 && v.size() > 1) {
			int j = rnd(0, v.size() - 1);
			if(isOrdered) j = v.size() - 1;
			std::swap(v[j], v.back());
			EXPECT_TRUE(rb.erase(v.back()) != nullptr);
			s.erase(s.find(v.back()));
			v.pop_back();
		} else if(p >= 85 && v.size() > 0) {
			int j = rnd(0, v.size() - 1);
			EXPECT_TRUE(rb.Find(rb.versionCount(), v[j]) != nullptr);
			int x = rand();
			EXPECT_EQ(rb.Find(rb.versionCount(), x) != nullptr, s.find(x) != s.end());
		} else {
			if(isOrdered) v.push_back(i);
			else v.push_back(rand() * rand() + rand());
			rb.insert(v.back());
			s.insert(v.back());
		}
		if((i % 100000) == 0) check(rb);
	}
}

TEST(RBRemove, Large) { doLarge(false); }
TEST(RBRemove, LargeOrdered) { doLarge(true); }
