// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

#include <utility>

namespace deque2 {

namespace {

// tipo da sub-deque
typedef std::pair<any*, any*> pair;

// retorno das operações Pop, um par com o elemento removido e a nova Deque
typedef std::pair<any*, Node*> ValDeque;

} // namespace

any::any(const void *p) : ptr(p), ref_ct(0) {}

void any::add_ref() { ref_ct++; }

template<class T> void any::rem_ref(int level) {
	if(--ref_ct == 0) {
		if(level == 0) delete static_cast<const T*>(ptr);
		else {
			const pair *p = static_cast<const pair*>(ptr);
			p->first->rem_ref<T>(level - 1);
			p->second->rem_ref<T>(level - 1);
			delete p;
		}
		delete this;
	}
}


/* Nesse namespace são implementadas versões de todos os métodos de Deque, mas estes recebem
 * Nodes e trabalham com ponteiros void.
 */
namespace {

any* _Front(const Node *d) {
	if(d->preffix != nullptr)
		return d->preffix;
	else if(d->center == nullptr)
		return d->suffix;
	else
		return static_cast<const pair*>(_Front(d->center)->ptr)->first;
}

any* _Back(const Node *d) {
	if(d->suffix != nullptr)
		return d->suffix;
	else if(d->center == nullptr)
		return d->preffix;
	else
		return static_cast<const pair*>(_Back(d->center)->ptr)->second;
}

Node* _PushFront(const Node *d, any *x) {
	if(d == nullptr)
		return new Node(x, nullptr, nullptr, 1);
	else if(d->preffix == nullptr)
		return new Node(x, d->center, d->suffix, d->size + 1);
	else {
		any *a = new any(new pair(x, d->preffix));
		x->add_ref();
		d->preffix->add_ref();
		return new Node(nullptr, _PushFront(d->center, a), d->suffix, d->size + 1);
	}
}

Node* _PushBack(const Node *d, any *x) {
	if(d == nullptr)
		return new Node(nullptr, nullptr, x, 1);
	else if(d->suffix == nullptr)
		return new Node(d->preffix, d->center, x, d->size + 1);
	else {
		any *a = new any(new pair(d->suffix, x));
		d->suffix->add_ref();
		x->add_ref();
		return new Node(d->preffix, _PushBack(d->center, a), nullptr, d->size + 1);
	}
}

ValDeque _PopFront(const Node *d) {
	if(d->preffix != nullptr && d->size == 1)
		return ValDeque(d->preffix, nullptr);
	else if(d->preffix != nullptr)
		return ValDeque(d->preffix, new Node(nullptr, d->center, d->suffix, d->size - 1));
	else if(d->center == nullptr)
		return ValDeque(d->suffix, nullptr);
	else {
		ValDeque vd = _PopFront(d->center);
		const pair *x = static_cast<const pair*>(vd.first->ptr);
		return ValDeque(x->first, new Node(x->second, vd.second, d->suffix, d->size - 1));
	}
}

ValDeque _PopBack(const Node *d) {
	if(d->suffix != nullptr && d->size == 1)
		return ValDeque(d->suffix, nullptr);
	else if(d->suffix != nullptr)
		return ValDeque(d->suffix, new Node(d->preffix, d->center, nullptr, d->size - 1));
	else if(d->center == nullptr)
		return ValDeque(d->preffix, nullptr);
	else {
		ValDeque vd = _PopBack(d->center);
		const pair *x = static_cast<const pair*>(vd.first->ptr);
		return ValDeque(x->second, new Node(d->preffix, vd.second, x->first, d->size - 1));
	}
}

any* _K_th(const Node *d, int k) {
	if(k == 0 && d->preffix != nullptr)
		return d->preffix;
	if(k == d->size - 1 && d->suffix != nullptr)
		return d->suffix;
	if(d->preffix != nullptr)
		k--;
	if((k % 2) == 0)
		return static_cast<const pair*>(_K_th(d->center, k / 2)->ptr)->first;
	else
		return static_cast<const pair*>(_K_th(d->center, k / 2)->ptr)->second;
}

} // namespace

Node::Node(any *p, Node *c, any *s, int sz) : preffix(p), center(c), suffix(s), size(sz), ref_ct(0) {
	if(preffix != nullptr) preffix->add_ref();
	if(center != nullptr) center->add_ref();
	if(suffix != nullptr) suffix->add_ref();
}

void Node::add_ref() { ref_ct++; }

template<class T> void Node::rem_ref(int level) {
	if(--ref_ct == 0) {
		if(preffix != nullptr)
			preffix->rem_ref<T>(level);
		if(center != nullptr)
			center->rem_ref<T>(level + 1);
		if(suffix != nullptr)
			suffix->rem_ref<T>(level);
		delete this;
	}
}

template<class T> Deque<T>::Deque() : node(nullptr) {}

template<class T> Deque<T>::Deque(Node *u) : node(u) {
	if(node != nullptr)
		node->add_ref();
}

template<class T> Deque<T>::Deque(const Deque &o) : Deque(o.node) {}

template<class T> Deque<T>& Deque<T>::operator=(const Deque<T> &o) {
	if(o.node != nullptr) o.node->add_ref();
	if(node != nullptr) node->rem_ref<T>();
	node = o.node;
	return *this;
}

template<class T> Deque<T>::~Deque() {
	if(node != nullptr) node->rem_ref<T>();
}

template<class T> const T& Deque<T>::Front() const {
	return *static_cast<const T*>(_Front(node)->ptr);
}

template<class T> const T& Deque<T>::Back() const {
	return *static_cast<const T*>(_Back(node)->ptr);
}

template<class T> const T& Deque<T>::K_th(int k) const {
	return *static_cast<const T*>(_K_th(node, k)->ptr);
}

template<class T> Deque<T> Deque<T>::PushFront(const T& x) const {
	return Deque<T>(_PushFront(node, new any(new T(x))));
}

template<class T> Deque<T> Deque<T>::PushBack(const T& x) const {
	return Deque<T>(_PushBack(node, new any(new T(x))));
}

template<class T> Deque<T> Deque<T>::PopFront() const {
	return Deque<T>(_PopFront(node).second);
}

template<class T> Deque<T> Deque<T>::PopBack() const {
	return Deque<T>(_PopBack(node).second);
}


} // namespace deque2
