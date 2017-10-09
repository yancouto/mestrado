// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

#include <utility>
#include <cassert>
#include <algorithm>
#include <iostream>

namespace deque3 {

template<class T> SubDeque<T>::SubDeque() : i(0), size_(0) {}

template<class T> int SubDeque<T>::size() const { return size_; }

template<class T> bool SubDeque<T>::empty() const { return size_ == 0; }

template<class T> void SubDeque<T>::clear() { size_ = 0; }

template<class T> void SubDeque<T>::push_back(const T& x) {
	assert(size_ < 5);
	v[(i + size_++) % 5] = x;
}

template<class T> void SubDeque<T>::push_front(const T& x) {
	assert(size_ < 5);
	if(--i == -1) i = 4;
	v[i] = x;
	size_++;
}

template<class T> void SubDeque<T>::pop_back() {
	assert(size_ > 0);
	size_--;
}

template<class T> void SubDeque<T>::pop_front() {
	assert(size_ > 0);
	if(++i == 5) i = 0;
	size_--;
}

template<class T> const T& SubDeque<T>::back() const {
	assert(size_ > 0);
	return v[(i + size_ - 1) % 5];
}

template<class T> const T& SubDeque<T>::front() const {
	assert(size_ > 0);
	return v[i];
}

template<class T> const T& SubDeque<T>::operator[](int i) const {
	assert(i >= 0 && i < size_);
	return v[(this->i + i) % 5];
}

/* Nesse namespace são implementadas versões de todos os métodos de Deque, mas estes recebem
 * Nodes e trabalham com ponteiros void.
 */
namespace {

// tipo da sub-deque
typedef std::pair<any*, any*> pair;

any* _Front(Node *a) {
	if(a->prefix.size() != 0)
		return a->prefix.front();
	else
		return a->suffix.front();
}

any* _Back(const Node *a) {
	if(a->suffix.size() != 0)
		return a->suffix.back();
	else
		return a->prefix.back();
}

Node* _PushFront(Node *a, any *x) {
	Node *a_ = Copy(a);
	a_->prefix.push_front(x);
	x->add_ref();
	a_->size++;
	Check(a_);
	return a_;
}

Node* _PushBack(Node *a, any *x) {
	Node *a_ = Copy(a);
	a_->suffix.push_back(x);
	x->add_ref();
	a_->size++;
	Check(a_);
	return a_;
}

Node* _PopFront(Node *a) {
	Node *a_ = Copy(a);
	_Front(a_)->rem_ref();
	if(a_->prefix.size() != 0)
		a_->prefix.pop_front();
	else
		a_->suffix.pop_front();
	a_->size--;
	Check(a_);
	return a_;
}

Node* _PopBack(Node *a) {
	Node *a_ = Copy(a);
	_Back(a_)->rem_ref();
	if(a_->suffix.size() != 0)
		a_->suffix.pop_back();
	else
		a_->prefix.pop_back();
	a_->size--;
	Check(a_);
	return a_;
}

any* _K_th(Node *d, Node *next, int k) {
	if(k < int(d->prefix.size())) return d->prefix[k];
	if((d->size - k - 1) < int(d->suffix.size()))
		return d->suffix[k - (d->size - int(d->suffix.size()))];
	k -= d->prefix.size();
	const pair *p;
	if(d->child != nullptr)
		p = static_cast<const pair*>(_K_th(d->child, next, k / 2)->ptr);
	else
		p = static_cast<const pair*>(_K_th(next, next->next, k / 2)->ptr);
	if((k % 2) == 0)
		return p->first;
	else
		return p->second;
}

} // namespace

template<class T> const T& Deque<T>::Front() const {
	return *static_cast<const T*>(_Front(node)->ptr);
}

template<class T> const T& Deque<T>::Back() const {
	return *static_cast<const T*>(_Back(node)->ptr);
}

template<class T> const T& Deque<T>::K_th(int k) const {
	return *static_cast<const T*>(_K_th(node, node->next, k)->ptr);
}

template<class T> Deque<T> Deque<T>::PushFront(const T& x) const {
	return Deque<T>(_PushFront(node, new any(new T(x))));
}

template<class T> Deque<T> Deque<T>::PushBack(const T& x) const {
	return Deque<T>(_PushBack(node, new any(new T(x))));
}

template<class T> Deque<T> Deque<T>::PopFront() const {
	return Deque<T>(_PopFront(node));
}

template<class T> Deque<T> Deque<T>::PopBack() const {
	return Deque<T>(_PopBack(node));
}

void Check(Node *a) {
	if(Digit(a, (a->child == nullptr && a->next == nullptr)) == 2)
		Fix(a);
	else if(a->next != nullptr &&
	        Digit(a->next, (a->next->child == nullptr && a->next->next == nullptr)) == 2) {
		Node *c = Copy(a->next);
		c->add_ref();
		if(a->next != nullptr) a->next->safe_rem_ref();
		a->next = c;
		Fix(c);
	}
}

Node* Copy(const Node *a) {
	if(a == nullptr) return new Node();
	return new Node(*a);
}

int Digit(const Node *a, bool last) {
	static const int digit[] = {2, 1, 0, 0, 1, 2};
	if(a->prefix.size() == 0 && last)
		return digit[a->suffix.size()];
	else if(a->suffix.size() == 0 && last)
		return digit[a->prefix.size()];
	else
		return std::max(digit[a->prefix.size()], digit[a->suffix.size()]);
}

void Fix(Node *a) {
	Node *b; bool last;
	if(a->child != nullptr) {
		b = Copy(a->child);
		last = (a->next == nullptr && b->child == nullptr);
	} else if(a->next != nullptr) {
		b = Copy(a->next);
		last = (b->next == nullptr && b->child == nullptr);
	} else {
		b = new Node();
		last = true;
	}
	b->level = a->level + 1;
	// Caso 1
	if(a->prefix.size() + a->suffix.size() + 2 * b->prefix.size() + 2 * b->suffix.size() <= 3) {
		if(b->prefix.size() != 0) {
			any* xy_ = b->prefix.front(); b->prefix.pop_front();
			const pair* xy = static_cast<const pair*>(xy_->ptr);
			a->prefix.push_back(xy->first); xy->first->add_ref();
			a->prefix.push_back(xy->second); xy->second->add_ref();
			xy_->rem_ref();
		}
		if(b->suffix.size() != 0) {
			any* xy_ = b->suffix.back(); b->suffix.pop_back();
			const pair* xy = static_cast<const pair*>(xy_->ptr);
			a->prefix.push_back(xy->first); xy->first->add_ref();
			a->prefix.push_back(xy->second); xy->second->add_ref();
			xy_->rem_ref();
		}
		if(a->suffix.size() != 0) {
			a->prefix.push_back(a->suffix.front()); a->suffix.pop_front();
		}
		delete b;
		b = nullptr;
	} else { // Caso 2
		b->size += (a->prefix.size() >= 4) + (a->suffix.size() >= 4);
		b->size -= (a->prefix.size() <= 1) + (a->suffix.size() <= 1);
		FixDeques(a->prefix, a->suffix, b->prefix, b->suffix);
		if(b->prefix.size() == 0 && b->suffix.size() == 0 && last) {
			delete b;
			b = nullptr;
		}
	}
	if(b != nullptr && Digit(b, last) == 1) {
		if(a->child == nullptr) {
			if(a->next != nullptr) a->next->safe_rem_ref();
			a->next = b->next;
			b->next = nullptr;
		}
		b->add_ref();
		if(a->child != nullptr) a->child->safe_rem_ref();
		a->child = b;
	} else {
		if(b != nullptr && a->child != nullptr) {
			if(a->next != nullptr) a->next->add_ref();
			if(b->next != nullptr) b->next->safe_rem_ref();
			b->next = a->next;
		}
		if(b != nullptr) b->add_ref();
		if(a->next != nullptr) a->next->safe_rem_ref();
		a->next = b;
		if(a->child != nullptr) a->child->safe_rem_ref();
		a->child = nullptr;
	}
}

// ADD PARAMETER TO THIS AND OTHER FUNCS
void FixDeques(SubDeque<any*> &l, SubDeque<any*> &r,
               SubDeque<any*> &L, SubDeque<any*> &R) {
	if(l.size() >= 4) {
		any *b = l.back(); l.pop_back();
		any *a = l.back(); l.pop_back();
		L.push_front(new any(new pair(a, b)));
		L.front()->add_ref();
	}
	if(r.size() >= 4) {
		any *a = r.front(); r.pop_front();
		any *b = r.front(); r.pop_front();
		R.push_back(new any(new pair(a, b)));
		R.back()->add_ref();
	}
	if(l.size() <= 1) {
		any *ab_;
		if(L.size() != 0) {
			ab_ = L.front(); L.pop_front();
		} else {
			ab_ = R.front(); R.pop_front();
		}
		const pair *ab = static_cast<const pair*>(ab_->ptr);
		l.push_back(ab->first); ab->first->add_ref();
		l.push_back(ab->second); ab->second->add_ref();
		ab_->rem_ref();
	}
	if(r.size() <= 1) {
		any *ab_;
		if(R.size() != 0) {
			ab_ = R.back(); R.pop_back();
		} else {
			ab_ = L.back(); L.pop_back();
		}
		const pair *ab = static_cast<const pair*>(ab_->ptr);
		r.push_front(ab->second); ab->second->add_ref();
		r.push_front(ab->first); ab->first->add_ref();
		ab_->rem_ref();
	}
}

template<class T> Deque<T>::Deque() : node(nullptr) {}

template<class T> Deque<T>::Deque(Node *u) : node(u) {
	if(node != nullptr)
		node->add_ref();
}

Node::Node() : child(nullptr), next(nullptr), size(0), level(0), ref_ct(0) {
}

Node::Node(const Node &o) {
	*this = o;
	for(int i = 0; i < prefix.size(); i++)
		prefix[i]->add_ref();
	for(int i = 0; i < suffix.size(); i++)
		suffix[i]->add_ref();
	ref_ct = 0;
	if(child != nullptr) child->add_ref();
	if(next != nullptr) next->add_ref();
}

void Node::add_ref() { ref_ct++; }

template<class T> void Node::rem_ref() {
	if(--ref_ct == 0) {
		for(int i = 0; i < prefix.size(); i++)
			prefix[i]->rem_ref<T>(level);
		for(int i = 0; i < suffix.size(); i++)
			suffix[i]->rem_ref<T>(level);
		if(child != nullptr)
			child->rem_ref<T>();
		if(next != nullptr)
			next->rem_ref<T>();
		child = next = nullptr;
		prefix.clear();
		suffix.clear();
		delete this;
	}
}

Node::~Node() {
	assert(prefix.empty());
	assert(suffix.empty());
	if(child != nullptr)
		child->safe_rem_ref();
	if(next != nullptr)
		next->safe_rem_ref();
}

void Node::safe_rem_ref() { assert(--ref_ct != 0); }

template<class T> Deque<T>::~Deque() {
	if(node != nullptr)
		node->rem_ref<T>();
}

template<class T> Deque<T>::Deque(const Deque &o) : Deque(o.node) {}

template<class T> Deque<T>& Deque<T>::operator=(const Deque &o) {
	if(o.node != nullptr) o.node->add_ref();
	if(node != nullptr) node->rem_ref<T>();
	node = o.node;
	return *this;
}

} // namespace deque3
