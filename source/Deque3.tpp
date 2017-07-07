// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

#include <utility>
#include <cassert>

namespace deque3 {

template<class T> SubDeque<T>::SubDeque() : i(0), size_(0) {}

template<class T> int SubDeque<T>::size() const { return size_; }

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


template<class T> Deque<T>::Deque() : node(nullptr) {}

template<class T> Deque<T>::Deque(const Node *u) : node(u) {}

Node::Node(const Node *c) : child(c), next(nullptr), size(2 * (c? c->size : 0)) {}

/* Nesse namespace são implementadas versões de todos os métodos de Deque, mas estes recebem
 * Nodes e trabalham com ponteiros void.
 */
namespace {

// tipo da sub-deque
typedef std::pair<const void*, const void*> pair;

const void* _Front(const Node *a) {
	if(a->preffix.size() != 0)
		return a->preffix.front();
	else
		return a->suffix.front();
}

const void* _Back(const Node *a) {
	if(a->suffix.size() != 0)
		return a->suffix.back();
	else
		return a->preffix.back();
}

const Node* _PushFront(const Node *a, const void *x) {
	Node *a_ = Copy(a);
	a_->preffix.push_front(x);
	a_->size++;
	Check(a_);
	return a_;
}

const Node* _PushBack(const Node *a, const void *x) {
	Node *a_ = Copy(a);
	a_->suffix.push_back(x);
	a_->size++;
	Check(a_);
	return a_;
}

const Node* _PopFront(const Node *a) {
	Node *a_ = Copy(a);
	if(a_->preffix.size() != 0)
		a_->preffix.pop_front();
	else
		a_->suffix.pop_front();
	a_->size--;
	Check(a_);
	return a_;
}

const Node* _PopBack(const Node *a) {
	Node *a_ = Copy(a);
	if(a_->suffix.size() != 0)
		a_->suffix.pop_back();
	else
		a_->preffix.pop_back();
	a_->size--;
	Check(a_);
	return a_;
}

const void* _K_th(const Node *d, const Node *next, int k) {
	if(k < int(d->preffix.size())) return d->preffix[k];
	if((d->size - k - 1) < int(d->suffix.size()))
		return d->suffix[k - (d->size - int(d->suffix.size()))];
	k -= d->preffix.size();
	const pair *p;
	if(d->child != nullptr)
		p = static_cast<const pair*>(_K_th(d->child, next, k / 2));
	else
		p = static_cast<const pair*>(_K_th(next, next->next, k / 2));
	if((k % 2) == 0)
		return p->first;
	else
		return p->second;
}

} // namespace

template<class T> const T& Deque<T>::Front() const {
	return *static_cast<const T*>(_Front(node));
}

template<class T> const T& Deque<T>::Back() const {
	return *static_cast<const T*>(_Back(node));
}

template<class T> const T& Deque<T>::K_th(int k) const {
	return *static_cast<const T*>(_K_th(node, node->next, k));
}

template<class T> Deque<T> Deque<T>::PushFront(const T& x) const {
	return Deque<T>(_PushFront(node, new T(x)));
}

template<class T> Deque<T> Deque<T>::PushBack(const T& x) const {
	return Deque<T>(_PushBack(node, new T(x)));
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
		a->next = c;
		Fix(c);
	}
}

Node* Copy(const Node *a) {
	if(a == nullptr) return new Node(nullptr);
	return new Node(*a);
}

int Digit(const Node *a, bool last) {
	static const int digit[] = {2, 1, 0, 0, 1, 2};
	if(a->preffix.size() == 0 && last)
		return digit[a->suffix.size()];
	else if(a->suffix.size() == 0 && last)
		return digit[a->preffix.size()];
	else
		return std::max(digit[a->preffix.size()], digit[a->suffix.size()]);
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
		b = new Node(nullptr);
		last = true;
	}
	// Caso 1
	if(a->preffix.size() + a->suffix.size() + 2 * b->preffix.size() + 2 * b->suffix.size() <= 3) {
		if(b->preffix.size() != 0) {
			const pair* xy = static_cast<const pair*>(b->preffix.front()); b->preffix.pop_front();
			a->preffix.push_back(xy->first);
			a->preffix.push_back(xy->second);
		}
		if(b->suffix.size() != 0) {
			const pair* xy = static_cast<const pair*>(b->suffix.front()); b->suffix.pop_front();
			a->preffix.push_back(xy->first);
			a->preffix.push_back(xy->second);
		}
		if(a->suffix.size() != 0) {
			a->preffix.push_back(a->suffix.front()); a->suffix.pop_front();
		}
		delete b;
		b = nullptr;
	} else { // Caso 2
		b->size += (a->preffix.size() >= 4) + (a->suffix.size() >= 4);
		b->size -= (a->preffix.size() <= 1) + (a->suffix.size() <= 1);
		FixDeques(a->preffix, a->suffix, b->preffix, b->suffix);
		if(b->preffix.size() == 0 && b->suffix.size() == 0 && last) {
			delete b;
			b = nullptr;
		}
	}
	if(b != nullptr && Digit(b, last) == 1) {
		if(a->child == nullptr) {
			a->next = b->next;
			b->next = nullptr;
		}
		a->child = b;
	} else {
		if(b != nullptr && a->child != nullptr)
			b->next = a->next;
		a->next = b;
		a->child = nullptr;
	}
}

void FixDeques(SubDeque<const void*> &l, SubDeque<const void*> &r,
               SubDeque<const void*> &L, SubDeque<const void*> &R) {
	if(l.size() >= 4) {
		const void *b = l.back(); l.pop_back();
		const void *a = l.back(); l.pop_back();
		L.push_front(new pair(a, b));
	}
	if(r.size() >= 4) {
		const void *a = r.front(); r.pop_front();
		const void *b = r.front(); r.pop_front();
		R.push_back(new pair(a, b));
	}
	if(l.size() <= 1) {
		const pair *ab;
		if(L.size() != 0) {
			ab = static_cast<const pair*>(L.front()); L.pop_front();
		} else {
			ab = static_cast<const pair*>(R.front()); R.pop_front();
		}
		l.push_back(ab->first);
		l.push_back(ab->second);
	}
	if(r.size() <= 1) {
		const pair *ab;
		if(R.size() != 0) {
			ab = static_cast<const pair*>(R.back()); R.pop_back();
		} else {
			ab = static_cast<const pair*>(L.back()); L.pop_back();
		}
		r.push_front(ab->second);
		r.push_front(ab->first);
	}
}


} // namespace deque3
