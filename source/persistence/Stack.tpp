// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

namespace persistence {

namespace stack {

template<class T> Node<T>::Node(const T& x, Node<T> *nx) : val(x), next(nx), depth(nx? nx->depth + 1 : 1), ptr_ct(0) {
	if(nx == nullptr) {
		jmp = nullptr;
	} else {
		nx->ptr_ct++;
		const Node<T> *j = nx->jmp;
		int jdepth = j? j->depth : 0;
		int j2depth = j && j->jmp? j->jmp->depth : 0;
		if(nx->depth - jdepth == jdepth - j2depth)
			jmp = j->jmp;
		else
			jmp = nx;
	}
}

template<class T> Node<T>* Node<T>::K_Ancestor(int k) {
	int ndepth = depth - k;
	Node<T> *u = this;
	while(u->depth != ndepth) {
		if(u->jmp && u->jmp->depth >= ndepth)
			u = u->jmp;
		else
			u = u->next;
	}
	return u;
}

template<class T> Stack<T>::Stack() : node(nullptr) {}
template<class T> Stack<T>::Stack(Node<T> *u) : node(u) { if(node != nullptr) node->ptr_ct++; }
template<class T> Stack<T>::Stack(const Stack<T> &o) : node(nullptr) { *this = o; }

namespace {

template<class T> void ptr_clean(Node<T> *u) {
	while(u != nullptr && --u->ptr_ct == 0) {
		Node<T> *nx = u->next;
		delete u;
		u = nx;
	}
}

}

template<class T> Stack<T>& Stack<T>::operator=(const Stack<T> &o) {
	ptr_clean(node);
	node = o.node;
	if(node) node->ptr_ct++;
	return *this;
}

template<class T> Stack<T>::~Stack() {
	ptr_clean(node);
}

template<class T> const T& Stack<T>::Top() const { return node->val; }

template<class T> const T& Stack<T>::K_th(int k) const {
	return node->K_Ancestor(node->depth - k)->val;
}

template<class T> int Stack<T>::Size() const {
	return node? node->depth : 0;
}

template<class T> Stack<T> Stack<T>::Push(const T& x) const {
	return Stack<T>(new Node<T>(x, node));
}

template<class T> Stack<T> Stack<T>::Pop() const { return Stack<T>(node->next); }

} // namespace stack

} // namespace persistence
