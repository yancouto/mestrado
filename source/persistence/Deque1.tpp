// Esse arquivo é importado apenas por Deque1.hpp e não deve ser importado manualmente.

#include <utility>

namespace persistence {

namespace deque1 {

template<class T> Deque<T>::Deque() : first(nullptr), last(nullptr) {}

template<class T> Deque<T>::Deque(Node<T> *f, Node<T> *l) : first(f), last(l) {
	if(first != nullptr) first->ptr_ct++;
	if(last != nullptr) last->ptr_ct++;
}
template<class T> Deque<T>::Deque(const Deque<T> &o) : first(nullptr), last(nullptr) { *this = o; }

namespace {

template<class T> void ptr_clean(Node<T> *u) {
	while(u != nullptr && --u->ptr_ct == 0) {
		Node<T> *nx = u->next;
		delete u;
		u = nx;
	}
}

} // namespace

template <class T> Deque<T>& Deque<T>::operator=(const Deque<T> &o) {
	if(&o == this) return *this;
	ptr_clean(first);
	ptr_clean(last);
	first = o.first;
	last = o.last;
	if(first != nullptr) first->ptr_ct++;
	if(last != nullptr) last->ptr_ct++;
	return *this;
}

template <class T> Deque<T>::~Deque() {
	ptr_clean(first);
	ptr_clean(last);
}



template<class T> const T& Deque<T>::Front() const { return first->val; }

template<class T> const T& Deque<T>::Back() const { return last->val; }

namespace {

template<class T> Node<T>* LCA(Node<T> *a, Node<T> *b) {
	if(a->depth > b->depth) std::swap(a, b);
	b = b->K_Ancestor(b->depth - a->depth);
	if(a == b) return a;
	while(a->next != b->next) {
		if(a->jmp != nullptr && b->jmp != nullptr && a->jmp != b->jmp) {
			a = a->jmp;
			b = b->jmp;
		} else {
			a = a->next;
			b = b->next;
		}
	}
	return a->next;
}

}


template<class T> const T& Deque<T>::K_th(int k) const {
	const Node<T> *mid = LCA(first, last);
	int l1 = first->depth - mid->depth;
	int l2 = last->depth - mid->depth;
	if(k <= l1)
		return first->K_Ancestor(k)->val;
	else
		return last->K_Ancestor(l1 + l2 - k)->val;
}

template<class T> Deque<T> Deque<T>::Swap() const { return Deque<T>(last, first); }

template<class T> Deque<T> Deque<T>::PushFront(const T& x) const {
	if(first == nullptr) {
		Node<T> *u = new Node<T>(x, nullptr);
		return Deque<T>(u, u);
	} else
		return Deque<T>(new Node<T>(x, first), last);
}

template<class T> Deque<T> Deque<T>::PushBack(const T& x) const {
	return Swap().PushFront(x).Swap();
}

template<class T> Deque<T> Deque<T>::PopFront() const {
	if(first == last)
		return Deque<T>();
	else if(LCA(first, last) == first)
		return Deque<T>(last->K_Ancestor(last->depth - first->depth - 1), last);
	else
		return Deque<T>(first->next, last);
}

template<class T> Deque<T> Deque<T>::PopBack() const { return Swap().PopFront().Swap(); }

} // namespace deque1

} // namespace persistence
