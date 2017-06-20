// Esse arquivo é importado apenas por RedBlackTree.hpp e não deve ser importado manualmente.

#include <climits>
#include <iostream>
#include <unordered_set>
#include <cassert>

template<class T>
Node<T>::Node(const T& val, int time) : timestamp(time), extraTimestamp(-1), red(true), value(val),
  parent(nullptr), copy(nullptr) { child[0] = child[1] = extra =  nullptr; }

/* Arvore rubro negra que armazena objetos do tipo T.
 * T deve ser comparável (possuir operator <) */
template<class T>
RedBlackTree<T>::RedBlackTree() { roots.push_back(nullptr); }

// Retorna a versão atual da árvore.
template<class T>
inline int RedBlackTree<T>::current() { return roots.size() - 1; }

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

template<class T> void RedBlackTree<T>::Modify(Node<T> *u, bool side, Node<T> *v) {
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

template<class T> void RedBlackTree<T>::Transplant(Node<T> *u, Node<T> *x) {
	x = Active(x);
	if(x != nullptr && x->parent != nullptr)
		Modify(x->parent, (Child(x->parent, 1) == x), nullptr);
	u = Active(u);
	Node<T> *v = u->parent;
	if(v != nullptr)
		Modify(v, (Child(v, 1) == u), x);
	else
		roots[current()] = x;
	if(x != nullptr)
		x->red = u->red;
}

template<class T> Node<T>* RedBlackTree<T>::MinElement(Node<T> *u) {
	while(Child(u, 0) != nullptr)
		u = Child(u, 0);
	return u;
}

template<class T> const T* RedBlackTree<T>::Remove(const T& val) {
	roots.push_back(roots.back());
	Node<T> *u = roots.back();
	while(u != nullptr && (u->value < val || val < u->value))
		u = Child(u, u->value < val);
	if(u == nullptr) return nullptr;
	const T* ret = &u->value;
	Node<T> *v = u->parent;
	if(Child(u, 1) == nullptr) {
		bool needFix = (v != nullptr && !u->red && Child(u, 0) == nullptr);
		Transplant(u, Child(u, 0));
		if(needFix)
			AddBlack(v, (Child(v, 1) == nullptr));
	} else {
		Node<T> *x = MinElement(Child(u, 1));
		Node<T> *y;
		if(x == Child(u, 1)) y = x;
		else y = x->parent;
		bool needFix = (!x->red && Child(x, 1) == nullptr);
		Transplant(x, Child(x, 1));
		Transplant(u, x);
		for(int side = 0; side <= 1; side++) {
			Node<T> *child = Child(u, side);
			Modify(u, side, nullptr);
			Modify(x, side, child);
		}
		if(needFix)
			AddBlack(y, (Child(y, 1) == nullptr));
	}
	if(roots[current()] != nullptr)
		roots[current()]->red = false;
	return ret;
}

template<class T> void RedBlackTree<T>::AddBlack(Node<T> *y, bool side) {
	y = Active(y); // Versão mais atual de y
	while(y != nullptr) {
		Node<T> *z = Child(y, !side);
		if(z->red) { // Arrumando caso 4 para caso 1, 2 ou 3.
			std::swap(y->red, z->red);
			Rotate(y, !side);
			y = Active(y);
			z = Child(y, !side);
		}
		Node<T> *z_x = Child(z, side);
		Node<T> *z_z = Child(z, !side);
		if((z_x == nullptr || !z_x->red) && (z_z == nullptr || !z_z->red)) { // Caso 1.
			z->red = true;
			if(y == roots[current()] || y->red) {
				y->red = false;
				break;
			} else {
				side = (Child(y->parent, 1) == y);
				y = y->parent;
			}
		} else {
			if(z_x != nullptr && z_x->red) { // Arrumando caso 3 para caso 2.
				std::swap(z->red, z_x->red);
				Rotate(z, side);
				y = Active(y);
				z = Child(y, !side);
				z_z = Child(z, !side);
			}
			std::swap(y->red, z->red); // Caso 2.
			z_z->red = false;
			Rotate(y, !side);
			break;
		}
	}
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
