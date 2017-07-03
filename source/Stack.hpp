/** @file
 * Implementação de uma pilha persistente.
 * Veja Stack para mais informação.
 */

#ifndef STACK_HPP_
#define STACK_HPP_

namespace stack {

/** Nó da pilha persistente.
 * @see Stack para mais informação.
 */
template<class T> class Node {
public:
	/** Valor do nó.
	 */
	T val;
	/** Próximo nó.
	 * Este campo armazena um ponteiro para o próximo nó da lista ligada.
	 */
	const Node *next;

	Node(const T& x, const Node *nx);
};

/** Pilha persistente.
 * Uma pilha implementada usando lista ligada, é automaticamente persistente pois este método
 * nunca modifica nenhum nó, e portanto esta é uma implementação funcional.
 *
 * A pilha armazena elementos do tipo `T`, e não faz nenhuma suposição sobre esse tipo.
 * Uma diferença do código discutido no Capítulo 2 da tese é que aqui as funções não recebem a
 * pilha, mas são métodos desta.
 *
 * Um objeto Stack é apenas um ponteiro para um nó (Node) que é o topo da pilha. O objeto é
 * imutável, e continua válido mesmo após chamadas de #Push e #Pop, já que estas retornam uma
 * **cópia** modificada da estrutura. Dessa forma, é possível acessar (e modificar) versões
 * antigas da estrutura, como no código abaixo:
 *
 * \code
 * using namespace stack;
 * Stack<int> p1; // ()
 * Stack<int> p2 = p1.push(1); // (1)
 * Stack<int> p3 = p2.push(2); // (2, 1)
 * Stack<int> p4 = p3.push(0); // (0, 2, 1)
 * Stack<int> p5 = p2.pop(); // (1)
 * \endcode
 *
 * Note que, quando usados em um contexto persistente, os nós da pilha não formam mais uma lista
 * ligada, mas sim uma árvore, já que vários nós podem apontar para o mesmo nó. Adicionar um nó
 * no começo da lista é na verdade adicionar uma folha nesta árvore.
 */
template<class T> class Stack {
public:
	/** Topo da pilha.
	 * O nó #node é o primeiro elemento da pilha. O ponteiro é constante pois não podemos
	 * modificar este nó.
	 * @see Node
	 */
	const Node<T> *node;

	/** Construtor de pilha vazia.
	 * Contrói uma pilha sem nenhum elemento.
	 */
	Stack();

	/** @name Operações de acesso
	 */
	///@{

	/** Topo da pilha.
	 * @returns O valor no topo da pilha.
	 * @pre A pilha não deve estar vazia.
	 */
	const T& Top();

	///@}

	/** @name Operações de modificação
	 * Estas funções retornam uma **cópia** modificada da pilha, e não alteram o objeto na qual
	 * são chamadas
	 */
	///@{

	/** Inserção da valor.
	 * @param x Valor a ser adicionado.
	 * @returns Uma nova pilha como esta mas com o valor \p x em seu topo.
	 */
	Stack<T> Push(const T& x);

	/** Remoção do topo.
	 * @returns Uma nova pilha como esta mas sem seu topo.
	 * @pre A pilha não deve estar vazia.
	 */
	Stack<T> Pop();

	///@}
	
	Stack(const Node<T> *u);
};

} // namespace stack

#include "Stack.tpp"

#endif
