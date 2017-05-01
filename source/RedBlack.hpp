#ifndef REDBLACK_HPP_
#define REDBLACK_HPP_

#include <vector>
#include <climits>
#include <iostream>
#include <unordered_set>

/* Implementação de uma árvore rubro-negra parcialmente persistente utilizando o método de Node
 * copying. A estrutura não usará ponteiros de pai, então cada nó (que não é a raiz) tem 1 ponteiro
 * para ele, e assim é necessário apenas 1 campo extra.
 */

/* Estrutura que representa um nó da ABB que armazena objetos do tipo T. */
template<class T> struct Node {
	// Tempo em que o nó foi criado
	int timestamp;
	// Filho esquerdo (0) e direito (1)
	Node *child[2];
	/* Filho adicional, seu tempo de criação e lado. Os outros valores são ignorados se
	 * extraTimestamp for -1 */
	Node *extra; int extraTimestamp; bool extraSide;
	// Cor do nó. Este campo não é persistente (só é válido para nós atuais)
	bool red;
	// Valor armazenado no nó
	T value;
	// Pai do nó (só é válido para nós atuais)
	Node *parent;

	Node(const T& val) : extraTimestamp(-1), red(true), value(val),
	  parent(nullptr) { child[0] = child[1] = extra =  nullptr; }

	// Retorna o filho side do nó, considerando o filho adicional, se possível.
	Node* childAt(bool side, int time=INT_MAX);
	/* Muda o filho apropriado para v, possivelmente propagando essa mudança, e retorna o ponteiro
	 * para o nó que atualmente representa este nó. time é o tempo atual.
	 * É necessário informar side pois v pode ser nullptr ou ter valor igual. */
	Node* change(Node *v, int time, bool side);
};

/* Arvore rubro negra que armazena objetos do tipo T.
 * T deve ser comparável (possuir operator <) */
template<class T> struct RedBlackTree {
	// Versão 0 é a árvore vazia
	std::vector<Node<T>*> roots;

	RedBlackTree() { roots.push_back(nullptr); }

	// Destrutor, libera toda a memória alocada.
	~RedBlackTree();

	// Ignora a versão 0
	inline int versionCount() { return roots.size() - 1; }

	/* Procura pelo objeto val na ABB criada com tempo time, e retorna um ponteiro constante para
	 * o objeto, se for encontrado, e nullptr caso contrário.
	 * Restrições: 0 <= time < version_count() */
	const T* find(int time, const T& val);

	/* Insere o objeto val na ABB em sua versão mais atual, e retorna o tempo da nova versão.
	 * Tempo: O(lg(tamanho da ABB)) */
	int insert(const T& val);

	/* Remove o objeto val na ABB em sua versão mais atual, e retorna o tempo da nova versão.
	 * Tempo: O(lg(tamanho da ABB)) */
	int erase(const T& val);
};

// Implementação. É necessário estar no mesmo arquivo por causa dos templates.

// Links nulos são considerados pretos
template<class T> inline bool isRed(Node<T> *n) {
	if(n == nullptr) return false;
	else return n->red;
}

template<class T> Node<T>* Node<T>::childAt(bool side, int time) {
	if(extraTimestamp != -1 && time >= extraTimestamp && extraSide == side)
		return extra;
	return child[side];
}

template<class T> Node<T>* Node<T>::change(Node<T> *v, int time, bool side) {
	//std::cerr << "change " << value << "." << side << " (" << this << ") to " << (v? v->value : -1) << " (" << v << ")" << std::endl;
	//std::cerr << "parent = " << parent << std::endl;
	if(extraTimestamp == time && side == extraSide) {
		if(v != nullptr) v->parent = this;
		extra = v;
	} else if(timestamp == time) {
		if(v != nullptr) v->parent = this;
		child[side] = v;
	} else if(extraTimestamp == -1) {
		//std::cerr << v << "->parent = " << this << std::endl;
		if(v != nullptr) v->parent = this;
		extra = v;
		extraTimestamp = time;
		extraSide = side;
		//std::cerr << "\n\n\n" << std::endl;
	} else {
		// novo nó com valores atuais e extra vazio
		Node<T> *u = new Node(*this);
		//std::cerr << "Copy Time! (" << u << ")" << std::endl;
		u->extraTimestamp = -1;
		u->child[extraSide] = extra;
		u->child[side] = v;
		for(int i = 0; i < 2; i++)
			if(u->child[i] != nullptr)
				u->child[i]->parent = u;
		if(parent) u->parent = parent->change(u, time, parent->childAt(1) == this);
		//std::cerr << "\n\n\n" << std::endl;
		return u;
	}
	return this;
}

template<class T> const T* RedBlackTree<T>::find(int time, const T& val) {
	time = versionCount();
	Node<T> *u = roots[time];
	while(u != nullptr && (u->value < val || val < u->value))
		u = u->childAt(u->value < val, time);
	return u? &u->value : nullptr;
}

namespace helper {
/* Rotaciona em torno de x de forma que o filho side de x toma o lugar de x.
 * Assume que tal filho existe. Retorna o nó x, ou sua cópia, se foi necessário copiar. */
template<class T> Node<T>* rotate(Node<T> *x, bool side, int time) {
	Node<T> *y = x->childAt(side);
	Node<T> *b = y->childAt(!side);
	if(x->parent != nullptr) x->parent->change(y, time, x->parent->childAt(1) == x);
	else y->parent = nullptr;
	y = y->change(x, time, !side);
	return x->change(b, time, side);
}

// Arruma violações rubronegras, retorna a raiz atual
template<class T> Node<T>* fixUp(Node<T> *z, int time) {
	while(z->parent != nullptr && z->parent->parent != nullptr && isRed(z->parent)) {
		Node<T> *y = z->parent->parent;
		if(isRed(y->childAt(0)) && isRed(y->childAt(1))) { // caso 1
			//std::cerr << "Case 1" << std::endl;
			y->red = true;
			y->childAt(0)->red = false;
			y->childAt(1)->red = false;
			z = y;
		} else {
			bool side1 = (y->childAt(1) == z->parent);
			bool side2 = (z->parent->childAt(1) == z);
			//std::cerr << "Case 2/3 (" << side1 << ", " << side2 << ")" << std::endl;
			if(side1 != side2) z = rotate(z->parent, side2, time); // caso 2 -> caso 3
			z = rotate(z->parent->parent, side1, time);
			z->red = true;
			z->parent->red = false;
		}
	}
	while(z->parent != nullptr) z = z->parent;
	return z;
}
}

template<class T> int RedBlackTree<T>::insert(const T& val) {
	//std::cerr << "Inserting " << val << std::endl;
	if(roots.back() == nullptr) {
		Node<T> *r = new Node<T>(val);
		r->red = false;
		roots.push_back(r);
		return versionCount();
	}
	int time = versionCount() + 1;
	Node<T> *u = roots.back(), *v = nullptr;
	while(u != nullptr) {
		v = u;
		u = u->childAt(u->value < val);
	}
	v = v->change(u = new Node<T>(val), time, v->value < val);
	Node<T> *r = helper::fixUp(u, time);
	r->red = false; // Arrumando a regra 1
	roots.push_back(r);
	return versionCount();
}

template<class T> RedBlackTree<T>::~RedBlackTree() {
	std::unordered_set<Node<T>*> nodes; // evita liberar o mesmo nó duas vezes
	std::vector<Node<T>*> &toDelete = roots;
	while(!toDelete.empty()) {
		Node<T> *u = toDelete.back();
		toDelete.pop_back();
		if(u == nullptr || !nodes.insert(u).second) continue;
		if(u->child[0]) toDelete.push_back(u->child[0]);
		if(u->child[1]) toDelete.push_back(u->child[1]);
		if(u->extra) toDelete.push_back(u->extra);
		delete u;
	}
}

#endif
