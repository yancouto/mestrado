#include "../RedBlack.hpp"
#include "gtest/gtest.h"

#include <vector>
#include <algorithm>

inline int rnd(int l, int r) {
	double p = rand() / (double(RAND_MAX) + 1);
	return l + (r - l + 1) * p;
}

template<class T> int rec(Node<T> *u, T *min, T *max) {
	if(u == nullptr) return 0;
	if(min) EXPECT_FALSE(u->value < *min) << "Não é uma ABB";
	if(max) EXPECT_FALSE(*max < u->value) << "Não é uma ABB";
	int bh1 = rec(u->childAt(0), min, &u->value);
	int bh2 = rec(u->childAt(1), &u->value, max);
	EXPECT_EQ(bh1, bh2) << "Alturas pretas devem ser iguais";
	if(u->red) {
		EXPECT_FALSE(isRed(u->childAt(0))) << "Não devem existir nós vermelhos consecutivos";
		EXPECT_FALSE(isRed(u->childAt(1))) << "Não devem existir nós vermelhos consecutivos";
	}
	return bh1 + (!u->red);
}

// Checa se as propriedades rubronegras são seguidas nos nós ativos
template<class T> void check(RedBlackTree<T> &rb) {
	Node<T> *u = rb.roots.back();
	if(u == nullptr) return;
	EXPECT_FALSE(u->red) << "Raiz deve ser preta";
	rec(u, (int*) nullptr, (int*) nullptr);
}

TEST(Simple, Case3) {
	RedBlackTree<int> rb;
	rb.insert(1); check(rb);
	rb.insert(3); check(rb);
	rb.insert(2); check(rb);
}

TEST(Simple, Insert) {
	RedBlackTree<int> rb1;
	for(int i = 0; i < 10; i++)
		rb1.insert(i);
	check(rb1);
	for(int i = 9; i >= 0; i--)
		EXPECT_NE(rb1.find(rb1.versionCount(), i), nullptr);
}

TEST(Simple, KeepsRedBlack) {
	RedBlackTree<int> rb;
	for(int i = 0; i < 1000; i++) {
		rb.insert(rand() % 800);
		check(rb);
	}
}

// Coloca os valores em um vetor ordenado
void traverse(Node<int> *u, std::vector<int> &v) {
	if(u == nullptr) return;
	traverse(u->childAt(0), v);
	v.push_back(u->value);
	traverse(u->childAt(1), v);
}

// Testa se a árvore rubronegra ordena um vetor específico
void testSort(std::vector<int> &v) {
	RedBlackTree<int> rb;
	for(int x : v)
		rb.insert(x);
	check(rb);
	std::sort(v.begin(), v.end()); // STL sort
	std::vector<int> ans;
	traverse(rb.roots.back(), ans);
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

TEST(Sorting, Repeated) {
	std::vector<int> v (10000, 10000);
	testSort(v);
}

TEST(Sorting, SortedReversed) {
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

TEST(Sorting, Small) { randomVecs(1000, 6, 15); }
TEST(Sorting, Medium) { randomVecs(1000, 100, 1000); }
TEST(Sorting, Large) { randomVecs(10, 10000, 100000); }

void testPersistence(const std::vector<int> &v) {
	int n = v.size();
	RedBlackTree<int> rb;
	for(int i = 0; i < n; i++)
		rb.insert(v[i]);
	for(int q = 0; q < 2 * n; q++) {
		int x = rnd(0, n - 1);
		int y = rnd(0, n - 1);
		const int *z = rb.find(x + 1, v[y]);
		if(z != nullptr) EXPECT_EQ(*z, v[y]);
		EXPECT_EQ(z != nullptr, y <= x);
	}
	for(int i = 0; i < n; i++)
		for(int q = -2; q <= 2; q++) {
			if(q + i < 0 || q + i >= n) continue;
			int x = i, y = q + i;
			const int *z = rb.find(x + 1, v[y]);
			if(z != nullptr) EXPECT_EQ(*z, v[y]);
			EXPECT_EQ(z != nullptr, y <= x);
		}

}

TEST(Persistence, SortedAndReversed) {
	for(int n : {10, 100, 1000, 100000}) {
		std::vector<int> v;
		for(int i = 0; i < n; i++)
			v.push_back(i);
		testPersistence(v);
		std::reverse(v.begin(), v.end());
		testPersistence(v);
	}
}


TEST(Persistence, Shuffled) {
	for(int n : {10, 100, 1000, 100000}) {
		std::vector<int> v;
		for(int i = 0; i < n; i++)
			v.push_back(i);
		random_shuffle(v.begin(), v.end());
		testPersistence(v);
	}
}
