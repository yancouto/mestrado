/** @file
 * Implementação de uma deque persistente de acordo com o capítulo 3.
 * Veja Deque para mais informação.
 */

#ifndef DEQUE1_HPP_
#define DEQUE1_HPP_

#include "Stack.hpp"

namespace deque1 {

/** Nó da deque.
 * Como a técnica é a mesma, precisamos do mesmo nó que na Stack.
 */
template<class T> using Node = stack::Node<T>;

/** Deque persistente utilizando LA e LCA.
 * Uma deque é uma lista que permite adições e remoções em suas duas pontas. Implementamos aqui
 * uma deque persistente que também permite acesso ao k-ésimo elemento. Esta utiliza a mesma
 * técnica que a [pilha](@ref stack::Stack) e a [fila](@ref queue::Queue), tanto que a
 * estrutura [Node](@ref stack::Node) é compartilhada entre a pilha e esta estrutura.
 *
 * A deque armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 3 da tese é que aqui as funções não recebem a
 * deque, mas são métodos desta.
 *
 * Um objeto Deque consiste de apenas dois ponteiros para nós das extremidades da deque. O objeto
 * é imutável, e como a implementação é funcional, podemos usá-la de forma persistente, como no
 * seguinte exemplo:
 *
 * @code
 * using namespace deque1;
 * Deque<int> d1; // ()
 * Deque<int> d2 = d1.PushFront(1); // (1)
 * Deque<int> d3 = d2.PushFront(2); // (2, 1)
 * Deque<int> d4 = d3.PushBack(0); // (2, 1, 0)
 * Deque<int> d5 = d2.PopBack(); // (2)
 * d4.Front(); // retorna 2
 * d4.Back(); // retorna 0
 * d4.K_th(1); // retorna 1
 * @endcode
 */
template<class T> class Deque {
public:
	/** Primeiro nó.
	 * Este é o nó que indica o primeiro elemento da deque. Os elementos da deque são os nós
	 * no caminho entre #first e #last.
	 */
	Node<T> *first;
	/** Ultimo nó.
	 * @see first para mais informação.
	 */
	Node<T> *last;

	/** Construtor vazio.
	 * Constrói uma deque sem nenhum elemento.
	 */
	Deque();

	/** @name Operações de Acesso
	 */
	///@{

	/** Acesso ao primeiro elemento.
	 * @returns O valor do primeiro elemento da deque.
	 * @pre A deque não deve estar vazia.
	 */
	const T& Front() const;

	/** Acesso ao último elemento.
	 * @returns O valor do último elemento da deque.
	 * @pre A deque não deve estar vazia.
	 */
	const T& Back() const;

	/** Acesso ao k-ésimo elemento.
	 * @param k O índice do elemento desejado. 0 é o primeiro elemento.
	 * @returns O valor do k-ésimo elemento da deque.
	 * @pre `0 &le; k < n`, onde \c n é o tamanho da deque.
	 */
	const T& K_th(int k) const;

	///@}

	/** @name Operações de Modificação
	 * Estas funções retornam uma **cópia** modificada da deque, e não alteram o objeto na qual
	 * são chamadas.
	 */
	///@{

	/** Inverte a deque.
	 * @returns Uma cópia da deque, mas com a ordem dos elementos invertida.
	 * @remark Apesar de ser uma função interessante por si só, esta função é principalmente
	 * usada para reduzir a quantidade de código para operações de Front ou Back, já que estas
	 * são simétricas.
	 */
	Deque<T> Swap() const;

	/** Inserção no início.
	 * @param x Valor a ser adicionado no início da deque.
	 * @returns Uma cópia da deque, mas com \p x como primeiro elemento.
	 */
	Deque<T> PushFront(const T& x) const;

	/** Inserção no final.
	 * @param x Valor a ser adicionado no final da deque.
	 * @returns Uma cópia da deque, mas com \p x como último elemento.
	 */
	Deque<T> PushBack(const T& x) const;

	/** Remoção do início.
	 * @returns Uma cópia da deque, mas sem seu primeiro elemento.
	 * @pre A deque não deve estar vazia.
	 */
	Deque<T> PopFront() const;

	/** Remoção do final.
	 * @returns Uma cópia da deque, mas sem seu último elemento.
	 * @pre A deque não deve estar vazia.
	 */
	Deque<T> PopBack() const;

	///@}

private:
	Deque(Node<T> *f, Node<T> *l);
};

} // namespace deque1

#include "Deque1.tpp"

#endif // DEQUE1_HPP_
