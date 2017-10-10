// Esse arquivo é importado apenas por OrderedSet.hpp e não deve ser importado manualmente.

namespace aux {

template<typename K, typename V> void OrderedMap<K, V>::Insert(const K &k, V v) {
	set[k] = v;
}

template<typename K, typename V> void OrderedMap<K, V>::Remove(const K &k) {
	set.erase(k);
}

template<typename K, typename V> V& OrderedMap<K, V>::K_th(int k) {
	return set.find_by_order(k - 1)->second;
}

template<typename K, typename V> int OrderedMap<K, V>::Count(const K &k) {
	return set.order_of_key(k) + (set.find(k) != set.end());
}

} // namespace aux
