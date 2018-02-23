// Esse arquivo é importado apenas por Queue.hpp e não deve ser importado manualmente.

namespace persistence {

namespace queue {

template<class T> Queue<T>::Queue() : rem(0) {}
template<class T> Queue<T>::Queue(Stack<T> st, int rm) : stack(st), rem(rm) {}

template<class T> const T& Queue<T>::First() const { return K_th(1); }

template<class T> const T& Queue<T>::K_th(int k) const { return stack.K_th(k + rem); }

template<class T> int Queue<T>::Size() const { return stack.Size() - rem; }

template<class T> Queue<T> Queue<T>::Enqueue(const T& x) const {
	return Queue<T>(stack.Push(x), rem);
}

template<class T> Queue<T> Queue<T>::Dequeue() const { return Queue<T>(stack, rem + 1); }

} // namespace queue

} // namespace persistence
