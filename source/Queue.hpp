/** @file
 * Implementação de uma fila persistente.
 * Veja Queue para mais informação.
 */

#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include "Stack.hpp"
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
 * Um object Queue consiste de uma pilha e um inteiro indicando o tamanho da fila. O objeto é
 * imutável, e como a implementação é funcional, podemos usá-la de forma persistente, como no
 * seguinte exemplo:
 *
 * @code
 * Queue<int> q1; // ()
 * Queue<int> q2 = q1.Enqueue(1); // (1)
 * Queue<int> q3 = q2.Enqueue(2); // (2, 1)
 * Queue<int> q4 = q3.Enqueue(0); // (0, 2, 1)
 * Queue<int> q5 = q2.Dequeue(); // (2)
 * @endcode
 */
template<class T> class Queue {
public:
	/** Pilha associada.
	 * A fila consiste dos #size primeiros elementos de #stack, os restantes são ignorados.
	 */
	Stack<T> stack;
	/** Tamanho da fila.
	 * Como #stack pode possuir mais de #size elementos, esta variável serve para indiciar a
	 * posição do último elemento da fila.
	 */
	int size;

	/** Construtor de fila vazia.
	 */
	Queue();

	/** @name Operações de acesso
	 */
	///@{

	/** Último elemento da fila.
	 * @returns O valor do último elemento da fila.
	 * @pre A fila não deve estar vazia.
	 * @see K_th
	 *
	 * @ex
	 * @code
	 * queue::Queue<int> q;
	 * q = q.Enqueue(2);
	 * q = q.Enqueue(3);
	 * q.Last(); // retorna 2
	 * q.Dequeue().Last() // retorna 3
	 * @endcode
	 */
	const T& Last() const;

	/** k-ésimo elemento.
	 * @param k O elemento desejado. 0 é o primeiro elemento da fila.
	 * @returns O valor do k-ésimo elemento da fila.
	 * @pre `0 &le; k < #size`
	 * @remark `Last()` é equivalente a `K_th(size - 1)`
	 *
	 * @ex
	 * @code
	 * queue::Queue<int> q;
	 * q = q.Enqueue(2);
	 * q = q.Enqueue(3);
	 * q.K_th(0); // retorna 3
	 * q.K_th(1) // retorna 2
	 * @endcode
	 */
	const T& K_th(int k) const;

	///@}

	/** @name Operações de Modificação
	 * Estas funções retornam uma **cópia** modificada da fila, e não alteam o objeto na qual
	 * são chamadas.
	 */
	///@{

	/** Inserção de valor.
	 * @param x Valor a ser adicionado.
	 * @returns Uma nova fila como esta mas com o valor \p x em seu início.
	 */
	Queue<T> Enqueue(const T& x) const;

	/** Remoção do final.
	 * @returns Uma fila como esta mas sem seu último elemento.
	 * @pre A fila não deve estar vazia.
	 */
	Queue<T> Dequeue() const;

	///@}

private:
	Queue<T>(Stack<T>, int);
};

} // namespace queue

#include "Queue.tpp"

#endif // QUEUE_HPP_
