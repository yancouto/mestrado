/** @file
 * Implementação de uma fila retroativa, como apresentada no Capítulo 2.
 * Veja Queue para mais informação.
 */

#ifndef RETROACTIVITY_QUEUE_HPP_
#define RETROACTIVITY_QUEUE_HPP_

#include "../aux/OrderedMap.hpp"

namespace retroactivity {

namespace queue {

/** Fila retroativa.
 * Uma fila retroativa implementada como no Capítulo 8. Ela usa uma ED auxiliar
 * (aux::OrderedMap).
 *
 * A fila armazena elementos do tipo \c T, e só assume que esse tipo tem um construtor
 * de cópia.
 */
template<typename T> class Queue {
	public:

	/** Lista de Enqueues, ordenados por tempo.
	 * O mapa tem como chaves inteiros (tempo), e como valor os valores adicionados
	 * pelos Enqueues.
	 */
	aux::OrderedMap<int, T> enq;
	/** Lista de Dequeues, ordenados por tempo.
	 * O mapa tem como chaves inteiros (tempo), e como valor um bool, já que não
	 * é necessário armazenar nenhum valor.
	 */
	aux::OrderedMap<int, bool> deq;

	/// @name Inserção de operações
	/// @{
	/** Inserção de operação Enqueue.
	 * Insere a operação `Enqueue(x)` no tempo \p t. A operação `Enqueue(x)` adiciona
	 * o elemento \c x ao final da fila.
	 * @param t Tempo no qual a operação será inserida.
	 * @param x Elemento adicionado por Enqueue.
	 * @pre Não deve existir operação no tempo \p t.
	 */
	void Insert_Enqueue(int t, const T &x);

	/** Inserção de operação Dequeue.
	 * Insere a operação `Dequeue()` no tempo \p t. A operação `Dequeue()` remove o
	 * primeiro elemento da fila.
	 * @param t Tempo no qual a operação será inserida
	 * @pre Não deve existir operação no tempo \p t.
	 */
	void Insert_Dequeue(int t);
	/// @}

	/// @name Remoção de operações
	/// @{
	/** Remove operação Enqueue no tempo \p t.
	 * @param t Tempo no qual a operação deve ser removida.
	 * @pre Deve existir uma operação de Enqueue no tempo \p t.
	 */
	void Delete_Enqueue(int t);

	/** Remove operação Dequeue no tempo \p t.
	 * @param t Tempo no qual a operação deve ser removida.
	 * @pre Deve existir uma operação de Dequeue no tempo \p t.
	 */
	void Delete_Dequeue(int t);
	/// @}

	/// @name Operações de acesso
	/// @{
	/** Devolve o \p k -ésimo elemento no tempo \p t.
	 * @param t Tempo no qual a busca deve ser feita.
	 * @param k Índice do elemento a ser devolvido (1-indexed).
	 * @returns O \p k -ésimo elemento na fila no tempo \p t.
	 * @pre \p k deve estar entre 1 e o número de elementos na fila no tempo \p t.
	 */
	T& Query_K_th(int t, int k);

	/** Devolve o primeiro elemento no tempo \p t.
	 * @param t Tempo no qual a busca deve ser feita.
	 * @returns O primeiro elemento da fila no tempo \p t.
	 * @pre A fila não deve estar vazia no tempo \p t.
	 */
	T& Query_First(int t);
	/// @}
};

} // namespace queue

} // namespace retroactive

#include "Queue.tpp"

#endif // RETROACTIVITY_QUEUE_HPP_
