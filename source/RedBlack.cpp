#include "RedBlack.hpp"

// Links nulos são considerados pretos
template<class T> inline bool isRed(Node<T> *n) {
	if(n == nullptr) return false;
	else return n->red;
}

template<class T> Node<T>* Node<T>::childAt(bool side, int time) {
	if(extraTimestamp != -1 && time >= extraTimestamp && (value < extra->value) == side)
		return extra;
	return child[side];
}

template<class T> Node<T>* Node<T>::change(Node<T> *v, int time) {
	if(childAt(value < v->value)) childAt(value < v->value)->parent = nullptr;
	if(extraTimestamp == -1) {
		if(v != nullptr) v->parent = this;
		extra = v;
		extraTimestamp = time;
		return this;
	} else {
		// novo nó com valores atuais e extra vazio
		Node<T> *u = new Node(*this);
		u->extra = nullptr;
		u->extraTimestamp = -1;
		u->child[value < extra->value] = extra;
		u->child[value < v->value] = v;
		v->parent = u;
		if(u->parent) u->parent = u->parent->change(u, time);
		return u;
	}
}

template<class T> const T* RedBlackTree<T>::find(int time, const T& val) {
	Node<T> *u = roots[time];
	while(u != nullptr && (u->value < val || val < u->value))
		u = u->childAt(u->value < val, time);
	return u;
}

/* Rotaciona em torno de x de forma que o filho side de x toma o lugar de x.
  * Assume que tal filho existe. Retorna o nó x, ou sua cópia, se foi necessário copiar. */
template<class T> Node<T>* rotate(Node<T> *x, bool side, int time) {
	Node<T> *y = x->childAt(side);
	Node<T> *b = y->childAt(!side);
	if(x->parent != nullptr) x->parent->change(y, time);
	else y->parent = nullptr;
	y->change(x, time);
	return x->change(b, time);
}

// Arruma violações rubronegras, retorna a raiz atual
template<class T> Node<T>* fixUp(Node<T> *z, int time) {
	while(z->parent != nullptr && z->parent->parent != nullptr && isRed(z->parent)) {
		Node<T> *y = z->parent->parent;
		if(isRed(y->childAt(0)) && isRed(y->childAt(1))) { // caso 1
			y->red = true;
			y->childAt(0)->red = false;
			y->childAt(1)->red = false;
			z = y;
		} else {
			bool side1 = (y->value < z->value);
			bool side2 = (z->parent->value < z->value);
			if(side1 != side2) z = rotate(z->parent, side2, time); // caso 2 -> caso 3
			z = rotate(z->parent->parent, side1, time);
			z->red = true;
			z->parent->red = false;
		}
	}
	while(z->parent != nullptr) z = z->parent;
	return z;
}

template<class T> int RedBlackTree<T>::insert(const T& val) {
	if(roots.back() == nullptr) {
		Node<T> *r = new Node<T>(val);
		r->red = false;
		roots.push_back(r);
		return versionCount();
	}
	int time = versionCount() + 1;
	Node<T> *u = roots.back(), v = nullptr;
	while(u != nullptr) {
		v = u;
		u = u->childAt(u->value < val);
	}
	v = v->change(u = new Node<T>(val), time);
	Node<T> *r = fixUp(u, time);
	r->red = false; // Arrumando a regra 1
	roots.push_back(r);
	return versionCount();
}
