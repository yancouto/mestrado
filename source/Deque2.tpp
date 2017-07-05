// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

#include <utility>

namespace deque2 {


template<class T> Deque<T>::Deque() : node(nullptr) {}

template<class T> Deque<T>::Deque(const Node *u) : node(u) {}


/* Nesse namespace são implementadas versões de todos os métodos de Deque, mas estes recebem
 * Nodes e trabalham com ponteiros void.
 */
namespace {

// tipo da sub-deque
typedef std::pair<const void*, const void*> pair;

// retorno das operações Pop, um par com o elemento removido e a nova Deque
typedef std::pair<const void*, const Node*> ValDeque;

const void* _Front(const Node *d) {
	if(d->preffix != nullptr)
		return d->preffix;
	else if(d->center == nullptr)
		return d->suffix;
	else
		return static_cast<const pair*>(_Front(d->center))->first;
}

const void* _Back(const Node *d) {
	if(d->suffix != nullptr)
		return d->suffix;
	else if(d->center == nullptr)
		return d->preffix;
	else
		return static_cast<const pair*>(_Back(d->center))->second;
}

const Node* _PushFront(const Node *d, const void *x) {
	if(d == nullptr)
		return new Node{x, nullptr, nullptr, 1};
	else if(d->preffix == nullptr)
		return new Node{x, d->center, d->suffix, d->size + 1};
	else
		return new Node{nullptr, _PushFront(d->center, new pair(x, d->preffix)), d->suffix,
		                d->size + 1};
}

const Node* _PushBack(const Node *d, const void *x) {
	if(d == nullptr)
		return new Node{nullptr, nullptr, x, 1};
	else if(d->suffix == nullptr)
		return new Node{d->preffix, d->center, x, d->size + 1};
	else
		return new Node{d->preffix, _PushBack(d->center, new pair(d->suffix, x)), nullptr,
		                d->size + 1};
}

ValDeque _PopFront(const Node *d) {
	if(d->preffix != nullptr && d->size == 1)
		return ValDeque(d->preffix, nullptr);
	else if(d->preffix != nullptr)
		return ValDeque(d->preffix, new Node{nullptr, d->center, d->suffix, d->size - 1});
	else if(d->center == nullptr)
		return ValDeque(d->suffix, nullptr);
	else {
		ValDeque vd = _PopFront(d->center);
		const pair *x = static_cast<const pair*>(vd.first);
		return ValDeque(x->first, new Node{x->second, vd.second, d->suffix, d->size - 1});
	}
}

ValDeque _PopBack(const Node *d) {
	if(d->suffix != nullptr && d->size == 1)
		return ValDeque(d->suffix, nullptr);
	else if(d->suffix != nullptr)
		return ValDeque(d->suffix, new Node{d->preffix, d->center, nullptr, d->size - 1});
	else if(d->center == nullptr)
		return ValDeque(d->preffix, nullptr);
	else {
		ValDeque vd = _PopBack(d->center);
		const pair *x = static_cast<const pair*>(vd.first);
		return ValDeque(x->second, new Node{d->preffix, vd.second, x->first, d->size - 1});
	}
}

const void* _K_th(const Node *d, int k) {
	if(k == 0 && d->preffix != nullptr)
		return d->preffix;
	if(k == d->size - 1 && d->suffix != nullptr)
		return d->suffix;
	if(d->preffix != nullptr)
		k--;
	if((k % 2) == 0)
		return static_cast<const pair*>(_K_th(d->center, k / 2))->first;
	else
		return static_cast<const pair*>(_K_th(d->center, k / 2))->second;
}

} // namespace

template<class T> const T& Deque<T>::Front() const {
	return *static_cast<const T*>(_Front(node));
}

template<class T> const T& Deque<T>::Back() const {
	return *static_cast<const T*>(_Back(node));
}

template<class T> const T& Deque<T>::K_th(int k) const {
	return *static_cast<const T*>(_K_th(node, k));
}

template<class T> Deque<T> Deque<T>::PushFront(const T& x) const {
	return Deque<T>(_PushFront(node, new T(x)));
}

template<class T> Deque<T> Deque<T>::PushBack(const T& x) const {
	return Deque<T>(_PushBack(node, new T(x)));
}

template<class T> Deque<T> Deque<T>::PopFront() const {
	return Deque<T>(_PopFront(node).second);
}

template<class T> Deque<T> Deque<T>::PopBack() const {
	return Deque<T>(_PopBack(node).second);
}


} // namespace deque2
