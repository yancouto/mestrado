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

	// Retorna a versão atual da árvore.
	inline int current() { return roots.size() - 1; }

	/* Procura pelo objeto val na ABB criada com tempo time, e retorna um ponteiro constante para
	 * o objeto, se for encontrado, e nullptr caso contrário.
	 * Restrições: 0 <= time < version_count() */
	const T* Find(int time, const T& val);

	/* Insere o objeto val na ABB em sua versão mais atual.
	 * Tempo: O(lg(tamanho da ABB)) */
	void Insert(const T& val);

	/* Remove o objeto val na ABB em sua versão mais atual, e retorna um ponteiro constante para
	 * o objeto, se ele tiver sido removido, e nullptr caso contrário.
	 * Tempo: O(lg(tamanho da ABB)) */
	const T* erase(const T& val);

// private: TODO make it private in the future

	// Retorna a versão ativa do nó u, que é este ou sua cópia, se existir.
	Node<T>* Active(Node<T> *u);

	// Retorna o filho side de u, considerando o filho adicional, se possível.
	// Para ser usado em operações de acesso.
	Node<T>* Child(Node<T> *u, bool side, int version);

	// Retorna o filho side de u mais recente.
	// Para ser usado em operações de modificação.
	Node<T>* Child(Node<T> *u, bool side);

	/* Muda o filho side de u para v, possivelmente propagando essa mudança, e retorna o ponteiro
	 * para o nó que atualmente representa u. rb é a árvore à qual este nó pertence.
	 * É necessário informar side pois v pode ser nullptr ou ter valor igual à de outros nós. */
	Node<T>* Modify(Node<T> *u, bool side, Node<T> *v);

	/* Copia um nó e retorna sua cópia (assume que o nó é ativo no momento da chamada)
	 * Se necessário, propaga a cópia */
	Node<T>* Copy(Node<T> *u);

	/* Rotaciona em torno de u de forma que o filho side de u toma o lugar de u.
	 * Assume que tal filho existe. */
	void Rotate(Node<T> *u, bool side);
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

template<class T> Node<T>* RedBlackTree<T>::Active(Node<T> *u) {
	if(u == nullptr || u->copy == nullptr) return u;
	else return u->copy;
}

template<class T> Node<T>* RedBlackTree<T>::Child(Node<T> *u, bool side, int version) {
	if(u->extraTimestamp != -1 && u->extraSide == side && version >= u->extraTimestamp)
		return u->extra;
	return u->child[side];
}

template<class T> Node<T>* RedBlackTree<T>::Child(Node<T> *u, bool side) {
	return Child(Active(u), side, current());
}

template<class T> Node<T>* RedBlackTree<T>::Modify(Node<T> *u, bool side, Node<T> *v) {
	u = Active(u);
	if(u->timestamp < current()) {
		u->copy = Copy(u);
		u = u->copy;
	}
	if(u->child[side] != nullptr)
		u->child[side]->parent = nullptr;
	u->child[side] = Active(v);
	if(u->child[side] != nullptr)
		u->child[side]->parent = u;
	return u;
}

template<class T> Node<T>* RedBlackTree<T>::Copy(Node<T> *u) {
	Node<T> *u_ = new Node<T>(*u);
	//std::cerr << "copying  " << u << " to " << u_ << std::endl;
	if(u->extraTimestamp != -1) {
		u_->child[u->extraSide] = u->extra;
		u_->extraTimestamp = -1;
	}
	u_->timestamp = current();
	if(roots[current()] == u)
		roots[current()] = u_;
	for(int side = 0; side <= 1; side++)
		if(u_->child[side] != nullptr)
			u_->child[side]->parent = u_;
	u->parent = nullptr;
	if(u_->parent != nullptr) {
		Node<T> *v = u_->parent;
		assert(!v->copy);
		bool side = (Child(v, 1) == u);
		if(v->timestamp == current())
			v->child[side] = u_;
		else if(v->copy != nullptr) assert(false);
		else if(v->extraTimestamp == -1) {
			v->extraTimestamp = current();
			v->extra = u_;
			v->extraSide = side;
		} else {
			v->copy = Copy(v);
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
	if(u == nullptr) return nullptr;
	else return &u->value;
}

template<class T> void RedBlackTree<T>::Rotate(Node<T> *u, bool side) {
	Node<T> *v = Child(u, side);
	Node<T> *b = Child(v, !side);
	Modify(v, !side, nullptr);
	Modify(u, side, b);
	u = Active(u);
	if(u->parent != nullptr)
		Modify(u->parent, (Child(u->parent, 1) == u), v);
	else roots[current()] = Active(v);
	Modify(v, !side, u);
}

template<class T> void RedBlackTree<T>::Insert(const T& value) {
	roots.push_back(roots.back());
	Node<T> *x = new Node<T>(value, current());
	if(roots[current()] == nullptr)
		roots[current()] = x;
	else {
		Node<T> *u = roots[current()], *v;
		while(u != nullptr) {
			v = u;
			u = Child(u, (v->value < value));
		}
		Modify(v, (v->value < value), x);
	}
	while(x->red && x->parent != nullptr && x->parent->red) {
		Node<T> *y = x->parent;
		Node<T> *z = y->parent;
		bool sideX = (x == Child(y, 1));
		bool sideY = (y == Child(z, 1));
		Node<T> *w = Child(z, !sideY);
		if(w != nullptr && w->red) {
			z->red = true;
			y->red = false;
			w->red = false;
			x = z;
		} else {
			if(sideX != sideY) {
				Rotate(y, sideX);
				std::swap(x, y);
			}
			Rotate(z, sideY);
			Active(z)->red = true;
			Active(y)->red = false;
			break;
		}
	}
	roots[current()]->red = false;
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
			rb.Rotate(z, !side);
			z = rb.Active(z);
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
				rb.Rotate(w, side);
				w = rb.Active(w);
				std::swap(w->red, w->parent->red);
				w = w->parent;
			}
			rb.Rotate(z->newestVersion(), !side);
			z = rb.Active(z);
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
