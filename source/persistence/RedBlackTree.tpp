// Esse arquivo é importado apenas por RedBlackTree.hpp e não deve ser importado manualmente.

namespace persistence {

namespace red_black_tree {

template<class T> Node<T>::Node(const T& val, int version) : child{nullptr, nullptr}, red(true),
                  value(val), timestamp(version), copy(nullptr), parent(nullptr), extra(nullptr),
                  extraTimestamp(-1) {}

template<class T> const T* RedBlackTree<T>::Find(const T& x, int version) {
	Node<T> *u = roots[version];
	while(u != nullptr && (x < u->value || u->value < x))
		u = Child(u, (u->value < x), version);
	if(u == nullptr)
		return nullptr;
	else
		return &u->value;
}

template<class T> void RedBlackTree<T>::Insert(const T& value) {
	roots.push_back(roots.back());
	Node<T> *x = new Node<T>(value, current());
	if(roots[current()] == nullptr)
		roots[current()] = x;
	else {
		Node<T> *u = roots[current()],
		        *v = nullptr;
		while(u != nullptr) {
			v = u;
			u = Child(u, (u->value < value));
		}
		Modify(v, (v->value < value), x);
	}
	// Arrumando possível violação causada pelo nó x.
	while(x->red && x->parent != nullptr && x->parent->red) {
		Node<T> *y = x->parent,
		        *z = y->parent;
		bool sideX = (Child(y, 1) == x);
		bool sideY = (Child(z, 1) == y);
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
	roots[current()]->red = false; // Caso x seja a raiz, e vermelho.
}

template<class T> const T* RedBlackTree<T>::Remove(const T& value) {
	roots.push_back(roots.back());
	Node<T> *u = roots[current()];
	while(u != nullptr && (value < u->value || u->value < value))
		u = Child(u, (u->value < value));
	if(u == nullptr)
		return nullptr;
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
		if(x == Child(u, 1))
			y = x;
		else
			y = x->parent;
		bool needFix = (!x->red && Child(x, 1) == nullptr);
		Transplant(x, Child(x, 1));
		Transplant(u, x);
		for(int side : {0, 1}) {
			Node<T> *child = Child(u, side);
			Modify(u, side, nullptr);
			Modify(x, side, child);
		}
		if(needFix)
			AddBlack(y, (Child(y, 1) == nullptr));
	}
	return ret;
}

template<class T> int RedBlackTree<T>::current() { return int(roots.size()) - 1; }

template<class T> RedBlackTree<T>::RedBlackTree() : roots(1, nullptr) {}

template<class T> RedBlackTree<T>::~RedBlackTree() {
	std::vector<Node<T>*> toDelete = roots;
	std::vector<Node<T>*> all;
	while(!toDelete.empty()) {
		Node<T> *u = toDelete.back(); toDelete.pop_back();
		if(u == nullptr || u->timestamp == -1) continue;
		u->timestamp = -1;
		if(u->child[0] != nullptr && u->child[0]->timestamp != -1) toDelete.push_back(u->child[0]);
		if(u->child[1] != nullptr && u->child[1]->timestamp != -1) toDelete.push_back(u->child[1]);
		if(u->extraTimestamp != -1 && u->extra->timestamp != -1) toDelete.push_back(u->extra);
		if(u->copy != nullptr && u->copy->timestamp != -1) toDelete.push_back(u->copy);
		all.push_back(u);
	}
	for(Node<T> *x : all) delete x;
}

template<class T> Node<T>* RedBlackTree<T>::Child(Node<T> *u, bool side, int version) const {
	if(u->extraTimestamp != -1 && u->extraSide == side && version >= u->extraTimestamp)
		return u->extra;
	return u->child[side];
}

template<class T> Node<T>* RedBlackTree<T>::Active(Node<T> *u) {
	if(u == nullptr || u->copy == nullptr)
		return u;
	return u->copy;
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
	u_->timestamp = current();
	if(u->extraTimestamp != -1) {
		u_->child[u->extraSide] = u->extra;
		u_->extraTimestamp = -1; // Limpando o campo extra.
	}
	if(roots[current()] == u)
		roots[current()] = u_;
	for(int side : {0, 1})
		if(u_->child[side] != nullptr)
			u_->child[side]->parent = u_;
	u->parent = nullptr;
	if(u_->parent != nullptr) {
		Node<T> *v = u_->parent;
		bool side = (Child(v, 1) == u);
		if(v->timestamp == current())
			v->child[side] = u_;
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

template<class T> void RedBlackTree<T>::Rotate(Node<T> *u, bool side) {
	Node<T> *v = Child(u, side);
	Node<T> *b = Child(v, !side);
	Modify(v, !side, nullptr);
	Modify(u, side, b);
	u = Active(u);
	if(u->parent != nullptr)
		Modify(u->parent, (Child(u->parent, 1) == u), v);
	else
		roots[current()] = Active(v);
	Modify(v, !side, u);
}

template<class T> Node<T>* RedBlackTree<T>::MinElement(Node<T> *u) {
	while(Child(u, 0) != nullptr)
		u = Child(u, 0);
	return u;
}

template<class T> void RedBlackTree<T>::Transplant(Node<T> *u, Node<T> *x) {
	x = Active(x);
	if(x != nullptr && x->parent != nullptr) // Removendo link para x, se existir.
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

template<class T> void RedBlackTree<T>::AddBlack(Node<T> *y, bool side) {
	y = Active(y);
	while(y != nullptr) {
		Node<T> *z = Child(y, !side);
		if(z->red) { // Caso 4 -> Caso 1, 2, ou 3.
			std::swap(y->red, z->red);
			Rotate(y, !side);
			y = Active(y);
			z = Child(y, !side);
		}
		Node<T> *z_x = Child(z, side),
		        *z_z = Child(z, !side);
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
			if(z_x != nullptr && z_x->red) { // Caso 3 -> Caso 2.
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

} // namespace red_black_tree

} // namespace persistence
