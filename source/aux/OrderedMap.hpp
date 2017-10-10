/** @file
 * Implementação de um mapa ordenado, a estrutura auxiliar descrita na Seção 8.2
 * Veja OrderedMap para mais informação.
 */

#ifndef AUX_ORDEREDMAP_HPP_
#define AUX_ORDEREDMAP_HPP_

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace aux {

/** Mapa ordenado.
 * Um OrderedMap é uma estrutura que armazena pares chave-valor, ordenados pela chave.
 * Cada chave está associada a no máximo um valor, mas o mesmo valor pode estar associado a muitas
 * chaves.
 * É possível adicionar e remover pares, e além disso acessar valores usando a ordenação de suas
 * chaves.
 *
 * Esta estrutura foi descrita na Seção 8.2.
 */
template<typename K, typename V> class OrderedMap {
	__gnu_pbds::tree<K, V, std::less<K>,
		__gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> set;
	public:
	/// @name Funções de modificação
	/// @{
	/** Inserção de par chave-valor.
	 * Insere a chave \p k associada ao valor \p v.
	 * @param k Chave a ser adicionada.
	 * @param v Valor associado à chave.
	 * @note Se já existe uma chave \p k na estrutura, ela é sobrescrita.
	 */
	inline void Insert(const K &k, V v);

	/** Remoção de chave.
	 * Remove a chave \p k, se existir, e seu valor associado.
	 * @param k Chave a ser removida.
	 */
	inline void Remove(const K &k);
	/// @}
	
	/// @name Funções de acesso
	/// @{
	/** Acesso ao k-ésimo elemento.
	 * @param k Indíce da chave (1-indexed).
	 * @returns Valor associado à \p k -ésima menor chaven no mapa.
	 * @pre \p k deve estar entre 1 e o número de pares no mapa.
	 */
	inline V& K_th(int k);

	/** Contagem de chaves menores ou iguais à \p k.
	 * @param k Chave a ser comparada.
	 * @returns A quantidade de chaves menores ou iguais a \p k.
	 */
	inline int Count(const K &k);
	/// @}
};

} // namespace aux

#include "OrderedMap.tpp"

#endif // AUX_ORDEREDMAP_HPP_
