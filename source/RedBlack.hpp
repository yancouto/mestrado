#ifndef REDBLACK_HPP_
#define REDBLACK_HPP_

#include <vector>
#include <climits>
#include <iostream>
#include <unordered_set>
#include <cassert>

/* Implementação de uma árvore rubro-negra parcialmente persistente utilizando o método de Node
 * copying. A estrutura não usará ponteiros de pai persistents, então cada nó (que não é a raiz)
 * tem 1 ponteiro para ele, e assim é necessário apenas 1 campo extra.
 */

 template<class T> struct RedBlackTree;

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

	// Aponta para a próxima cópia (versão mais nova) deste nó, se ele não é atual.
	Node *nextVersion;

	Node(const T& val, int time) : timestamp(time), extraTimestamp(-1), red(true), value(val),
	  parent(nullptr), nextVersion(nullptr) { child[0] = child[1] = extra =  nullptr; }

	// Retorna o filho side do nó, considerando o filho adicional, se possível.
	Node* childAt(bool side, int time=INT_MAX);
	/* Muda o filho apropriado para v, possivelmente propagando essa mudança, e retorna o ponteiro
	 * para o nó que atualmente representa este nó. rb é a árvore à qual este nó pertence.
	 * É necessário informar side pois v pode ser nullptr ou ter valor igual. */
	Node* change(Node *v, bool side, RedBlackTree<T> &rb);

	// Returna a cópia desse nó, se ela foi criada nessa versão.
	inline Node* newestVersion() { return nextVersion == nullptr? this : nextVersion; }
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

	/* Insere o objeto val na ABB em sua versão mais atual.
	 * Tempo: O(lg(tamanho da ABB)) */
	void insert(const T& val);

	/* Remove o objeto val na ABB em sua versão mais atual, e retorna um ponteiro constante para
	 * o objeto, se ele tiver sido removido, e nullptr caso contrário.
	 * Tempo: O(lg(tamanho da ABB)) */
	const T* erase(const T& val);
};

// ||=============================================================================||
// ||=============================================================================||
// || Implementação. É necessário estar no mesmo arquivo por causa dos templates. ||
// ||=============================================================================||
// ||=============================================================================||

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

template<class T> Node<T>* Node<T>::change(Node<T> *v, bool side, RedBlackTree<T> &rb) {
	//std::cerr << this << "->" << side << " = " << v << "  |par " << parent <<  std::endl;
	const int time = rb.versionCount();
	if(extraTimestamp == time && side == extraSide) {
		if(extra != nullptr) extra->parent = nullptr;
		if(v != nullptr) v->parent = this;
		extra = v;
	} else if(timestamp == time) {
		if(child[side]) child[side]->parent = nullptr;
		if(v != nullptr) v->parent = this;
		child[side] = v;
	} else if(extraTimestamp == -1) {
		if(child[side]) child[side]->parent = nullptr;
		if(v != nullptr) v->parent = this;
		extra = v;
		extraTimestamp = time;
		extraSide = side;
	} else {
		// novo nó com valores atuais e extra vazio
		for(int i = 0; i < 2; i++)
			if(childAt(i) != nullptr)
				childAt(i)->parent = nullptr;
		Node<T> *u = new Node(*this);
		u->timestamp = time;
		u->extraTimestamp = -1;
		u->child[extraSide] = extra;
		u->child[side] = v;
		for(int i = 0; i < 2; i++)
			if(u->child[i] != nullptr)
				u->child[i]->parent = u;
		if(parent) u->parent = parent->change(u, parent->childAt(1) == this, rb);
		else if(this == rb.roots.back()) rb.roots.back() = u;
		nextVersion = u;
		return u;
	}
	return this;
}

template<class T> const T* RedBlackTree<T>::find(int time, const T& val) {
	Node<T> *u = roots[time];
	while(u != nullptr && (u->value < val || val < u->value))
		u = u->childAt(u->value < val, time);
	return u? &u->value : nullptr;
}

namespace helper { // funções auxiliares
/* Rotaciona em torno de x de forma que o filho side de x toma o lugar de x.
 * Assume que tal filho existe. Retorna o nó x, ou sua cópia, se foi necessário copiar. */
template<class T> Node<T>* rotate(Node<T> *x, bool side, RedBlackTree<T> &rb) {
	//std::cerr << "rotate " << x << "  " << side << std::endl;
	Node<T> *y = x->childAt(side);
	Node<T> *b = y->childAt(!side);
	if(b != nullptr) y = y->change(nullptr, !side, rb);
	x = x->newestVersion()->change(b, side, rb);
	if(x->parent != nullptr) x->parent->change(y, x->parent->childAt(1) == x, rb);
	else {
		y->parent = nullptr;
		assert(x == rb.roots.back());
		rb.roots.back() = y;
	}
	y = y->change(x, !side, rb);
	return x;
}

// Arruma violações rubronegras
template<class T> void fixUp(Node<T> *z, RedBlackTree<T> &rb) {
	while(z->parent != nullptr && z->parent->parent != nullptr && isRed(z->parent)) {
		Node<T> *y = z->parent->parent;
		if(isRed(y->childAt(0)) && isRed(y->childAt(1))) { // caso 1
			y->red = true;
			y->childAt(0)->red = false;
			y->childAt(1)->red = false;
			z = y;
		} else {
			bool side1 = (y->childAt(1) == z->parent);
			bool side2 = (z->parent->childAt(1) == z);
			if(side1 != side2) z = rotate(z->parent, side2, rb); // caso 2 -> caso 3
			z = rotate(z->parent->parent, side1, rb);
			z->red = true;
			z->parent->red = false;
		}
	}
}
}

template<class T> void RedBlackTree<T>::insert(const T& val) {
	//std::cerr << "======== INSERT " << val << std::endl;
	int time = versionCount() + 1;
	if(roots.back() == nullptr) {
		Node<T> *r = new Node<T>(val, time);
		r->red = false;
		//std::cerr << "add " << r << std::endl;
		roots.push_back(r);
		return;
	}
	roots.push_back(roots.back());
	Node<T> *u = roots.back(), *v = nullptr;
	while(u != nullptr) {
		v = u;
		u = u->childAt(u->value < val);
	}
	v = v->change(u = new Node<T>(val, time), v->value < val, *this);
	helper::fixUp(u, *this);
	//std::cerr << "roots.back() = " << u << " prev " << roots[roots.size() - 2] << std::endl;
	roots.back()->red = false; // Arrumando a regra 1
}

namespace helper {
/* Substitui u por v, assume que v não é descendente de u. v pode ser nulo.
 * Muda apenas os pais dos vxs, nem u nem v são invalidados pelo procedimento */
template<class T> void transplant(Node<T> *u, Node<T> *v, RedBlackTree<T> &rb) {
	if(v != nullptr && v->parent != nullptr) {
		v->parent->change(nullptr, v->parent->childAt(1) == v, rb);
		v->parent = nullptr;
	}
	assert(u->nextVersion == nullptr);
	if(u->parent != nullptr) u->parent->change(v, u == u->parent->childAt(1), rb);
	else rb.roots.back() = v;
	u->parent = nullptr;
}
template<class T> void deleteFixUp(Node<T> *z, bool side, RedBlackTree<T> &rb) {
}
}

template<class T> const T* RedBlackTree<T>::erase(const T& val) {
	Node<T> *u = roots.back();
	while(u != nullptr && (u->value < val || val < u->value))
		u = u->childAt(u->value < val);
	if(u == nullptr) return nullptr;
	const T* ret = &u->value;
	roots.push_back(roots.back());
	if(u->childAt(1) == nullptr) {
		Node<T> *l = u->childAt(0), *p = u->parent;
		if(l != nullptr) u = u->change(nullptr, 0, *this);
		helper::transplant(u, l, *this);
		if(!isRed(u) && p != nullptr)
			helper::deleteFixUp(p->newestVersion(), u == p->childAt(1, versionCount() - 1), *this);
	} else if(u->childAt(1)->childAt(0) == nullptr) {
		Node<T> *r = u->childAt(1);
		u = u->change(nullptr, 1, *this);
		helper::transplant(u, r, *this);
		r = r->change(u->childAt(0), 0, *this);
		if(!isRed(u) && r->parent != nullptr)
			helper::deleteFixUp(r->parent, r == r->parent->childAt(1), *this);
	} else {
		Node<T> *y = u->childAt(1);
		while(y->childAt(0) != nullptr)
			y = y->childAt(0);
		assert(y != u->childAt(1));
		Node<T> *yr = y->childAt(1), *yp = y->parent;
		if(yr != nullptr) y = y->change(nullptr, 1, *this);
		helper::transplant(y, yr, *this);
		assert(y->childAt(0) == nullptr && y->childAt(1) == nullptr);
		// nó y está isolado
		u = u->newestVersion();
		helper::transplant(u, y, *this);
		y = y->change(u->childAt(1), 1, *this);
		y = y->change(u->childAt(0), 0, *this);
		bool was_black = !isRed(y);
		y->red = u->red;
		yp = yp->newestVersion();
		if(was_black) helper::deleteFixUp(yp, yp->childAt(1) == yr, *this);
	}
	if(roots.back() != nullptr) roots.back()->red = false;
	return ret;
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
