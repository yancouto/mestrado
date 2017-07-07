/** @file
 * Implementação de uma deque persistente de acordo com o capítulo 4.
 * Veja Deque para mais informação.
 */

#ifndef DEQUE2_HPP_
#define DEQUE2_HPP_

namespace deque2 {

/** Nó da deque recursiva.
 * Um nó representa uma deque persistente e tem três campos opcionais: #preffix, #center e #suffix.
 * A deque resultante é a deque que tem como primeiro elemento #preffix (se for não-nulo),
 * último elemento #suffix (se for não-nulo), e demais elementos são armazenados em pares
 * na sub-deque #center .
 *
 * Usamos ponteiros para void ao invés de um tipo \c T de template pois é necessário que o campo
 * #center armazene uma deque de pares de T, e se isso fosse feito com templates, geraria um
 * loop infinito em tempo de compilação, já que C++ tentaria instanciar todos os tipos possíveis
 * de Deque, que seriam infinitos.
 */
class Node {
public:
	/** Prefixo da deque.
	 * Se for não-nulo, #preffix é um ponteiro para o valor do primeiro elemento da deque.
	 */
	const void* preffix;
	/** Sub-deque central.
	 * Uma deque que armazena pares de elementos de \c T , e tem os elementos "do meio" da deque.
	 */
	const Node *center;
	/** Sufixo da deque.
	 * Se for não-nulo, #suffix um ponteiro para o valor do último elemento da deque.
	 */
	const void* suffix;

	/** Tamanho da deque.
	 * Número de elementos total da deque. Esse número vai ser duas vezes o número de elementos
	 * em #center mais um para cada ponteiro não-nulo em #preffix e #suffix .
	 */
	int size;
};

/** Deque persistente de estrutura recursiva.
 * Uma deque é uma lista que permite adições e remoções em suas duas pontas. Implementamos aqui
 * uma deque persistente que também permite acesso ao k-ésimo elemento. Esta utiliza uma tecnica
 * descrita por Kaplan e Tarjan @cite KaplanT1999 .
 *
 * A deque armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 4 da tese é que aqui as funções não recebem a
 * deque, mas são métodos desta. Além disso, as funções Pop não retornam o objeto removido, apenas
 * a nova deque.
 *
 * O objeto é imutável, e como a implementação é funcional, podemos usá-la de forma persistente,
 * como no seguinte exemplo:
 *
 * @code
 * using namespace deque2;
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
	/** Nó principal.
	 * Este é o nó que representa a deque inteira.
	 * @see Node para saber a razão desta classe não ter o tipo \c T .
	 */
	const Node *node;

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
	Deque(const Node *u);
};

} // namespace deque2

#include "Deque2.tpp"

#endif // DEQUE2_HPP_