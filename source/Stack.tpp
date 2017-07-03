// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

namespace stack {

template<class T> Node<T>::Node(const T& x, const Node<T> *nx) : val(x), next(nx) {
	if(nx == nullptr) {
		len = 1;
		jmp = nullptr;
	} else {
		len = nx->len + 1;
		const Node<T> *j = nx->jmp;
		int jlen = j? j->len : 0;
		int j2len = j && j->jmp? j->jmp->len : 0;
		if(nx->len - jlen == jlen - j2len)
			jmp = j->jmp;
		else
			jmp = nx;
	}
}

template<class T> Stack<T>::Stack() : node(nullptr) {}
template<class T> Stack<T>::Stack(const Node<T> *u) : node(u) {}

template<class T> const T& Stack<T>::Top() const { return node->val; }

template<class T> const T& Stack<T>::K_th(int k) const {
	int nlen = node->len - k;
	const Node<T> *u = node;
	while(u->len != nlen) {
		if(u->jmp && u->jmp->len >= nlen)
			u = u->jmp;
		else
			u = u->next;
	}
	return u->val;
}

template<class T> Stack<T> Stack<T>::Push(const T& x) const {
	return Stack<T>(new Node<T>(x, node));
}

template<class T> Stack<T> Stack<T>::Pop() const { return Stack<T>(node->next); }

} // namespace stack
