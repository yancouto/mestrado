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

	/** Profundidade do nó.
	 * Se seguirmos o link #next por #depth vezes chegamos no nó \c null.
	 */
	int depth;

	/** Nó de pulo.
	 * Aponta para o nó \c u com `u.#depth = J(#depth)`, onde \c J é a operação de diminuir em 1 o
	 * digito menos significativo não-nulo na base skew-binary canônica.
	 * @see \cite Myers83 para mais informação sobre skew-binary.
	 */
	const Node *jmp;

	/** k-ésimo ancestral.
	 * @param k Indice do ancestral.
	 * @returns O k-ésimo ancestral do nó. Se `k = 0`, retorna o próprio nó.
	 */
	const Node* K_Ancestor(int k) const;

	Node(const T& x, const Node *nx);
};

/** Pilha persistente.
 * Uma pilha implementada usando lista ligada, é automaticamente persistente pois este método
 * nunca modifica nenhum nó, e portanto esta é uma implementação funcional.
 * Para podermos ter acesso ao k-ésimo elemento, utilizamos uma técnica desenvolvida por
 * Myers \cite Myers83 , que se aproveita da representação em skew-binary de um número para
 * subir k links na lista ligada em tempo logaritmico, apenas armazenando um ponteiro e
 * um inteiro em cada nó.
 *
 * A deque armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 2 da tese é que aqui as funções não recebem a
 * pilha, mas são métodos desta.
 *
 * Um objeto Stack é apenas um ponteiro para um nó (Node) que é o topo da pilha. O objeto é
 * imutável, e continua válido mesmo após chamadas de #Push e #Pop, já que estas retornam uma
 * **cópia** modificada da estrutura. Dessa forma, é possível acessar (e modificar) versões
 * antigas da estrutura, como no exemplo abaixo:
 *
 * @code
 * using namespace stack;
 * Stack<int> p1; // ()
 * Stack<int> p2 = p1.Push(1); // (1)
 * Stack<int> p3 = p2.Push(2); // (2, 1)
 * Stack<int> p4 = p3.Push(0); // (0, 2, 1)
 * Stack<int> p5 = p2.Pop(); // (1)
 * @endcode
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

	/** @name Operações de Acesso
	 */
	///@{

	/** Topo da pilha.
	 * @returns O valor no topo da pilha.
	 * @pre A pilha não deve estar vazia.
	 * @see K_th
	 *
	 * @ex
	 * @code
	 * stack::Stack<int> st;
	 * st = st.Push(2);
	 * st = st.Push(3);
	 * st.Top(); // retorna 3
	 * st.Pop().Top(); // retorna 2
	 * @endcode
	 */
	const T& Top() const;

	/** k-ésimo elemento.
	 * @param k O elemento desejado. 0 é o topo da pilha.
	 * @returns O valor do k-ésimo elemento da pilha.
	 * @pre `0 &le; k < n`, onde \c n é o tamanho da pilha.
	 * @remark `Top()` é equivalente a `K_th(0)`
	 *
	 * @ex
	 * @code
	 * stack::Stack<int> st;
	 * st = st.Push(2);
	 * st = st.Push(3);
	 * st.K_th(0); // retorna 3
	 * st.K_th(1); // retorna 2
	 * @endcode
	 */
	const T& K_th(int k) const;

	///@}

	/** @name Operações de Modificação
	 * Estas funções retornam uma **cópia** modificada da pilha, e não alteram o objeto na qual
	 * são chamadas.
	 */
	///@{

	/** Inserção da valor.
	 * @param x Valor a ser adicionado.
	 * @returns Uma nova pilha como esta mas com o valor \p x em seu topo.
	 */
	Stack<T> Push(const T& x) const;

	/** Remoção do topo.
	 * @returns Uma nova pilha como esta mas sem seu topo.
	 * @pre A pilha não deve estar vazia.
	 */
	Stack<T> Pop() const;

	///@}
	
private:
	Stack(const Node<T> *u);
};

} // namespace stack

#include "Stack.tpp"

#endif
