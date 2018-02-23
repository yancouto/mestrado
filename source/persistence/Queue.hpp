/** @file
 * Implementação de uma fila persistente.
 * Veja Queue para mais informação.
 */

#ifndef PERSISTENCE_QUEUE_HPP_
#define PERSISTENCE_QUEUE_HPP_

#include "Stack.hpp"

namespace persistence {
using stack::Stack;

namespace queue {

/** Fila persistente.
 * A fila é implementada usando a [pilha persistente](@ref stack::Stack), já que esta permite
 * acesso ao k-ésimo elemento, e isto é o bastante para usá-la como uma fila.
 *
 * A deque armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 2 da tese é que aqui as funções não recebem a
 * fila, mas são métodos desta.
 *
 * Um object Queue consiste de uma pilha e um inteiro indicando o número de elementos já removidos
 * desta. O objeto é imutável, e como a implementação é funcional, podemos usá-la de forma
 * persistente, como no seguinte exemplo:
 *
 * @code
 * using namespace persistence::queue;
 * Queue<int> q1; // ()
 * Queue<int> q2 = q1.Enqueue(1); // (1)
 * Queue<int> q3 = q2.Enqueue(2); // (1, 2)
 * Queue<int> q4 = q3.Enqueue(0); // (1, 2, 0)
 * Queue<int> q5 = q2.Dequeue(); // (2)
 * @endcode
 */
template<class T> class Queue {
public:
	/** Pilha associada.
	 * A fila consiste dos elementos de #stack, ignorando os #rem primeiros.
	 */
	Stack<T> stack;
	/** Contagem de elementos removidos.
	 * Esta variável conta quantas vezes #Dequeue já foi chamada.
	 */
	int rem;

	/** Construtor de fila vazia.
	 */
	Queue();

	/** @name Operações de acesso
	 */
	///@{

	/** Primeiro elemento da fila.
	 * @returns O valor do primeiro elemento da fila.
	 * @pre A fila não deve estar vazia.
	 * @see K_th
	 *
	 * @ex
	 * @code
	 * queue::Queue<int> q;
	 * q = q.Enqueue(2);
	 * q = q.Enqueue(3);
	 * q.First(); // retorna 2
	 * q.Dequeue().First() // retorna 3
	 * @endcode
	 */
	const T& First() const;

	/** k-ésimo elemento.
	 * @param k O elemento desejado. 1 é o primeiro elemento da fila.
	 * @returns O valor do k-ésimo elemento da fila.
	 * @pre `1 &le; k &le; #size`
	 * @remark `First()` é equivalente a `K_th(1)`
	 *
	 * @ex
	 * @code
	 * queue::Queue<int> q;
	 * q = q.Enqueue(2);
	 * q = q.Enqueue(3);
	 * q.K_th(1); // retorna 2
	 * q.K_th(2) // retorna 3
	 * @endcode
	 */
	const T& K_th(int k) const;

	/** Tamanho da fila.
	 * @returns O tamanho da fila
	 */
	int Size() const;

	///@}

	/** @name Operações de Modificação
	 * Estas funções retornam uma **cópia** modificada da fila, e não alteram o objeto na qual
	 * são chamadas.
	 */
	///@{

	/** Inserção de valor.
	 * @param x Valor a ser adicionado.
	 * @returns Uma cópia desa fila com o valor \p x em seu final.
	 */
	Queue<T> Enqueue(const T& x) const;

	/** Remoção do início.
	 * @returns Uma cópia desta fila sem seu primeiro elemento.
	 * @pre A fila não deve estar vazia.
	 */
	Queue<T> Dequeue() const;

	///@}

private:
	Queue<T>(Stack<T>, int);
};

} // namespace queue

} // namespace persistence

#include "Queue.tpp"

#endif // PERSISTENCE_QUEUE_HPP_
