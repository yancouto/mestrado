// Esse arquivo é importado apenas por OrderedSet.hpp e não deve ser importado manualmente.

namespace retroactivity {

namespace queue {

template<typename T> void Queue<T>::Insert_Enqueue(int t, const T &x) {
	enq.Insert(t, x);
}

template<typename T> void Queue<T>::Insert_Dequeue(int t) {
	deq.Insert(t, true);
}

template<typename T> void Queue<T>::Delete_Enqueue(int t) {
	enq.Remove(t);
}

template<typename T> void Queue<T>::Delete_Dequeue(int t) {
	deq.Remove(t);
}

template<typename T> T& Queue<T>::Query_K_th(int t, int k) {
	int d_t = deq.Count(t);
	return enq.K_th(d_t + k);
}

template<typename T> T& Queue<T>::Query_First(int t) {
	return Query_K_th(t, 1);
}

} // namespace queue

} // namespace retroactivity
