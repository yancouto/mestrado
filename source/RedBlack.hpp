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
	Node *copy;

	Node(const T& val, int time) : timestamp(time), extraTimestamp(-1), red(true), value(val),
	  parent(nullptr), copy(nullptr) { child[0] = child[1] = extra =  nullptr; }

	// Returna a cópia desse nó, se ela foi criada nessa versão.
	inline Node* newestVersion() { return copy == nullptr? this : copy; }
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
	const T* Find(int time, const T& val);

	/* Insere o objeto val na ABB em sua versão mais atual.
	 * Tempo: O(lg(tamanho da ABB)) */
	void insert(const T& val);

	/* Remove o objeto val na ABB em sua versão mais atual, e retorna um ponteiro constante para
	 * o objeto, se ele tiver sido removido, e nullptr caso contrário.
	 * Tempo: O(lg(tamanho da ABB)) */
	const T* erase(const T& val);

// private: TODO make it private in the future

	// Retorna o filho side de u, considerando o filho adicional, se possível.
	Node<T>* Child(Node<T> *u, bool side, int version=INT_MAX);

	/* Muda o filho side de u para v, possivelmente propagando essa mudança, e retorna o ponteiro
	 * para o nó que atualmente representa u. rb é a árvore à qual este nó pertence.
	 * É necessário informar side pois v pode ser nullptr ou ter valor igual à de outros nós. */
	Node<T>* Modify(Node<T> *u, bool side, Node<T> *v);

	/* Copia um nó e retorna sua cópia (assume que o nó é ativo no momento da chamada)
	 * Se necessário, propaga a cópia */
	Node<T>* Copy(Node<T> *u, int version);
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

template<class T> Node<T>* RedBlackTree<T>::Child(Node<T> *u, bool side, int version) {
	if(u->extraTimestamp != -1 && u->extraSide == side && version >= u->extraTimestamp)
		return u->extra;
	return u->child[side];
}

template<class T> Node<T>* RedBlackTree<T>::Modify(Node<T> *u, bool side, Node<T> *v) {
	int version = versionCount();
	assert(u->copy == nullptr);
	if(u->copy != nullptr)
		u = u->copy;
	if(u->timestamp < version) {
		u->copy = Copy(u, version);
		u = u->copy;
	}
	if(u->child[side] != nullptr)
		u->child[side]->parent = nullptr;
	u->child[side] = v;
	if(v != nullptr)
		v->parent = u;
	assert(!Child(u, 0) || !Child(u, 0)->copy);
	assert(!Child(u, 1) || !Child(u, 1)->copy);
	return u;
}

template<class T> Node<T>* RedBlackTree<T>::Copy(Node<T> *u, int version) {
	Node<T> *u_ = new Node<T>(*u);
	//std::cerr << "copying  " << u << " to " << u_ << std::endl;
	if(u->extraTimestamp != -1) {
		u_->child[u->extraSide] = u->extra;
		u_->extraTimestamp = -1;
	}
	u_->timestamp = version;
	if(roots[version] == u)
		roots[version] = u_;
	for(int side = 0; side <= 1; side++)
		if(u_->child[side] != nullptr)
			u_->child[side]->parent = u_;
	u->parent = nullptr;
	if(u_->parent != nullptr) {
		Node<T> *v = u_->parent;
		assert(!v->copy);
		bool side = (Child(v, 1) == u);
		if(v->timestamp == version)
			v->child[side] = u_;
		else if(v->copy != nullptr) assert(false);
		else if(v->extraTimestamp == -1) {
			v->extraTimestamp = version;
			v->extra = u_;
			v->extraSide = side;
		} else {
			v->copy = Copy(v, version);
			v->copy->child[side] = u_;
			u_->parent = v->copy;
		}
	}
	return u_;
}

template<class T> const T* RedBlackTree<T>::Find(int time, const T& val) {
	Node<T> *u = roots[time];
	while(u != nullptr && (u->value < val || val < u->value))
		u = Child(u, u->value < val, time);
	return u? &u->value : nullptr;
}

namespace helper { // funções auxiliares
/* Rotaciona em torno de x de forma que o filho side de x toma o lugar de x.
 * Assume que tal filho existe. Retorna o nó x, ou sua cópia, se foi necessário copiar. */
template<class T> Node<T>* rotate(Node<T> *x, bool side, RedBlackTree<T> &rb) {
	//std::cerr << "rotate " << x << "  " << side << std::endl;
	Node<T> *y = rb.Child(x, side);
	assert(y != nullptr);
	Node<T> *b = rb.Child(y, !side);
	if(b != nullptr) y = rb.Modify(y, !side, nullptr);
	x = rb.Modify(x->newestVersion(), side, b);
	if(x->parent != nullptr) rb.Modify(x->parent, rb.Child(x->parent, 1) == x, y);
	else {
		y->parent = nullptr;
		assert(x == rb.roots.back());
		rb.roots.back() = y;
	}
	y = rb.Modify(y, !side, x);
	return x;
}

// Arruma violações rubronegras
template<class T> void fixUp(Node<T> *z, RedBlackTree<T> &rb) {
	while(z->parent != nullptr && z->parent->parent != nullptr && isRed(z->parent)) {
		Node<T> *y = z->parent->parent;
		if(isRed(rb.Child(y, 0)) && isRed(rb.Child(y, 1))) { // caso 1
			y->red = true;
			rb.Child(y, 0)->red = false;
			rb.Child(y, 1)->red = false;
			z = y;
		} else {
			bool side1 = (rb.Child(y, 1) == z->parent);
			bool side2 = (rb.Child(z->parent, 1) == z);
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
		//std::cerr << u << " to " << Child(u, u->value < val) << std::endl;
		u = Child(u, u->value < val);
	}
	v = Modify(v, v->value < val, u = new Node<T>(val, time));
	helper::fixUp(u, *this);
	//std::cerr << "roots.back() = " << u << " prev " << roots[roots.size() - 2] << std::endl;
	roots.back()->red = false; // Arrumando a regra 1
}

namespace helper {
/* Substitui u por v, assume que v não é descendente de u. v pode ser nulo.
 * Muda apenas os pais dos vxs, nem u nem v são invalidados pelo procedimento */
template<class T> void transplant(Node<T> *u, Node<T> *v, RedBlackTree<T> &rb) {
	if(v != nullptr && v->parent != nullptr) {
		rb.Modify(v->parent, rb.Child(v->parent, 1) == v, nullptr);
		v->parent = nullptr;
	}
	assert(u->copy == nullptr);
	if(u->parent != nullptr) rb.Modify(u->parent, u == rb.Child(u->parent, 1), v);
	else rb.roots.back() = v;
	u->parent = nullptr;
}
template<class T> void deleteFixUp(Node<T> *z, bool side, RedBlackTree<T> &rb) {
	while(!isRed(rb.Child(z, side))) {
		if(isRed(rb.Child(z, !side))) { // caso 1
			z = rotate(z, !side, rb);
			std::swap(z->red, z->parent->red);
			assert(rb.Child(z, !side));
		}
		Node<T> *w = rb.Child(z, !side);
		assert(w);
		assert(!isRed(w));
		if(!isRed(rb.Child(w, 0)) && !isRed(rb.Child(w, 1))) { // caso 2
			w->red = true;
			if(z->parent == nullptr) return;
			else {
				side = (z == rb.Child(z->parent, 1));
				z = z->parent;
			}
		} else {
			if(!isRed(rb.Child(w, !side))) { // caso 3
				assert(isRed(rb.Child(w, side)));
				w = rotate(w, side, rb);
				std::swap(w->red, w->parent->red);
				w = w->parent;
			}
			z = rotate(z->newestVersion(), !side, rb);
			std::swap(z->red, z->parent->red);
			rb.Child(z->parent, !side)->red = false;
			return;
		}
	}
	rb.Child(z, side)->red = false;
}
}

template<class T> const T* RedBlackTree<T>::erase(const T& val) {
	Node<T> *u = roots.back();
	while(u != nullptr && (u->value < val || val < u->value))
		u = Child(u, u->value < val);
	if(u == nullptr) return nullptr;
	const T* ret = &u->value;
	roots.push_back(roots.back());
	if(Child(u, 1) == nullptr) {
		Node<T> *l = Child(u, 0);
		if(l != nullptr) u = Modify(u, 0, nullptr);
		Node<T> *p = u->parent;
		bool side = (p && Child(p, 1) == u);
		helper::transplant(u, l, *this);
		if(!isRed(u) && p != nullptr)
			helper::deleteFixUp(p->newestVersion(), side, *this);
	} else if(Child(Child(u, 1), 0) == nullptr) {
		Node<T> *r = Child(u, 1);
		u = Modify(u, 1, nullptr);
		helper::transplant(u, r, *this);
		r = Modify(r, 0, Child(u, 0));
		bool was_black = !isRed(r);
		r->red = u->red;
		if(was_black)
			helper::deleteFixUp(r, 1, *this);
	} else {
		Node<T> *y = Child(u, 1);
		while(Child(y, 0) != nullptr)
			y = Child(y, 0);
		Node<T> *yr = Child(y, 1), *yp = y->parent;
		if(yr != nullptr) y = Modify(y, 1, nullptr);
		helper::transplant(y, yr, *this); // nó y está isolado

		u = u->newestVersion();
		helper::transplant(u, y, *this);
		y = Modify(y, 1, Child(u, 1));
		y = Modify(y, 0, Child(u, 0));
		bool was_black = !isRed(y);
		y->red = u->red;
		yp = yp->newestVersion();
		if(was_black) helper::deleteFixUp(yp, Child(yp, 1) == yr, *this);
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
