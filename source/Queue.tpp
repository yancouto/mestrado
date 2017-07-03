// Esse arquivo é importado apenas por Queue.hpp e não deve ser importado manualmente.

namespace queue {

template<class T> Queue<T>::Queue() : size(0) {}
template<class T> Queue<T>::Queue(Stack<T> st, int sz) : stack(st), size(sz) {}

template<class T> const T& Queue<T>::Last() const { return K_th(size - 1); }

template<class T> const T& Queue<T>::K_th(int k) const { return stack.K_th(k); }

template<class T> Queue<T> Queue<T>::Enqueue(const T& x) const {
	return Queue<T>(stack.Push(x), size + 1);
}

template<class T> Queue<T> Queue<T>::Dequeue() const { return Queue<T>(stack, size - 1); }


} // namespace queue
