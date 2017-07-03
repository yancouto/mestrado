// Esse arquivo é importado apenas por Stack.hpp e não deve ser importado manualmente.

namespace stack {

template<class T> Node<T>::Node(const T& x, const Node<T> *nx) : val(x), next(nx) {}

template<class T> Stack<T>::Stack() : node(nullptr) {}
template<class T> Stack<T>::Stack(const Node<T> *u) : node(u) {}

template<class T> const T& Stack<T>::Top() { return node->val; }

template<class T> Stack<T> Stack<T>::Push(const T& x) {
	return Stack<T>(new Node<T>(x, node));
}

template<class T> Stack<T> Stack<T>::Pop() { return Stack<T>(node->next); }

} // namespace stack
