// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

namespace stack {

template<class T> Node<T>::Node(const T& x, const Node<T> *nx) : val(x), next(nx) {
	if(nx == nullptr) {
		depth = 1;
		jmp = nullptr;
	} else {
		depth = nx->depth + 1;
		const Node<T> *j = nx->jmp;
		int jdepth = j? j->depth : 0;
		int j2depth = j && j->jmp? j->jmp->depth : 0;
		if(nx->depth - jdepth == jdepth - j2depth)
			jmp = j->jmp;
		else
			jmp = nx;
	}
}

template<class T> const Node<T>* Node<T>::K_Ancestor(int k) const {
	int ndepth = depth - k;
	const Node<T> *u = this;
	while(u->depth != ndepth) {
		if(u->jmp && u->jmp->depth >= ndepth)
			u = u->jmp;
		else
			u = u->next;
	}
	return u;
}

template<class T> Stack<T>::Stack() : node(nullptr) {}
template<class T> Stack<T>::Stack(const Node<T> *u) : node(u) {}

template<class T> const T& Stack<T>::Top() const { return node->val; }

template<class T> const T& Stack<T>::K_th(int k) const {
	return node->K_Ancestor(k)->val;
}

template<class T> Stack<T> Stack<T>::Push(const T& x) const {
	return Stack<T>(new Node<T>(x, node));
}

template<class T> Stack<T> Stack<T>::Pop() const { return Stack<T>(node->next); }

} // namespace stack
