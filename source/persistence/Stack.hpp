/** @file
 * Implementação de uma pilha persistente.
 * Veja Stack para mais informação.
 */

#ifndef PERSISTENCE_STACK_HPP_
#define PERSISTENCE_STACK_HPP_

namespace persistence {

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
	 * Este campo armazena um ponteiro para o próximo nó da lista ligada, que no caso é o nó
	 * do elemento anterior da pilha.
	 */
	Node *next;

	/** Profundidade do nó.
	 * Se seguirmos o link #next por #depth vezes chegamos no nó \c null.
	 */
	const int depth;

	/** Nó de pulo.
	 * Aponta para o nó \c u com `u.#depth = J(#depth)`, onde \c J é a operação de diminuir em 1 o
	 * digito menos significativo não-nulo na base skew-binary canônica.
	 * @see \cite Myers83 para mais informação sobre skew-binary.
	 */
	Node *jmp;

	/** k-ésimo ancestral.
	 * @param k Indice do ancestral.
	 * @returns O k-ésimo ancestral do nó. Se `k = 0`, devolve o próprio nó.
	 */
	Node* K_Ancestor(int k);

	/** Número de pointeiros para o nó.
	 * Este campo, não persistente, armazena, a todo momento, quantos ponteiros existem para esse
	 * nó. Ele conta os ponteiros #nxt de outros nós e ponteiros armazenados em
	 * #stack::Stack::node.
	 *
	 * Isto é feito pois C++ não tem
	 * [garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)),
	 * logo devemos manualmente deletar os nós que não tem mais como ser atingidos. Este campo
	 * é usado para remover o nó quando não existem mais ponteiros para ele. Já que a estrutura
	 * é acíclica, isso acontece quando o vértice não é mais atingível.
	 *
	 * Como esta é uma classe auxiliar, toda a remoção acontece na classe stack::Stack.
	 */
	int ptr_ct;

	Node(const T& x, Node *nx);
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;
};

/** Pilha persistente.
 * Uma pilha implementada usando lista ligada, é automaticamente persistente pois este método
 * nunca modifica nenhum nó, e portanto esta é uma implementação funcional.
 * Para podermos ter acesso ao k-ésimo elemento, utilizamos uma técnica desenvolvida por
 * Myers \cite Myers83 , que se aproveita da representação em skew-binary de um número para
 * subir k links na lista ligada em tempo logaritmico, apenas armazenando um ponteiro e
 * um inteiro em cada nó.
 *
 * A pilha armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 2 da tese é que aqui as funções não recebem a
 * pilha, mas são métodos desta.
 *
 * Um objeto Stack é apenas um ponteiro para um nó (Node) que é o último elemento da pilha. O
 * objeto é imutável, e continua válido mesmo após chamadas de #Push e #Pop, já que estas
 * devolvem uma **cópia** modificada da estrutura. Dessa forma, é possível acessar (e modificar)
 * versões antigas da estrutura, como no exemplo abaixo:
 *
 * @code
 * using namespace persistence::stack;
 * Stack<int> p1; // ()
 * Stack<int> p2 = p1.Push(1); // (1)
 * Stack<int> p3 = p2.Push(2); // (1, 2)
 * Stack<int> p4 = p3.Push(0); // (1, 2, 0)
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
	 * O nó #node é o último elemento da pilha.
	 * @see Node
	 */
	Node<T> *node;

	/** Construtor de pilha vazia.
	 * Contrói uma pilha sem nenhum elemento.
	 */
	Stack();

	/** @name Operações de Acesso
	 */
	///@{

	/** Topo da pilha.
	 * @returns O valor do último elemento da pilha.
	 * @pre A pilha não deve estar vazia.
	 * @see K_th
	 *
	 * @ex
	 * @code
	 * stack::Stack<int> st;
	 * st = st.Push(2);
	 * st = st.Push(3);
	 * st.Top(); // devolve 3
	 * st.Pop().Top(); // devolve 2
	 * @endcode
	 */
	const T& Top() const;

	/** k-ésimo elemento.
	 * @param k O elemento desejado. 1 é o primeiro elemento da pilha.
	 * @returns O valor do k-ésimo elemento da pilha.
	 * @pre `1 &le; k &le; n`, onde \c n é o tamanho da pilha.
	 * @remark `Top()` é equivalente a `K_th(n)`
	 *
	 * @ex
	 * @code
	 * stack::Stack<int> st;
	 * st = st.Push(2);
	 * st = st.Push(3);
	 * st.K_th(0); // devolve 2
	 * st.K_th(1); // devolve 3
	 * @endcode
	 */
	const T& K_th(int k) const;

	/** Tamanho da pilha.
	 * @returns O tamanho da pilha
	 */
	int Size() const;


	///@}

	/** @name Operações de Modificação
	 * Estas funções devolvem uma **cópia** modificada da pilha, e não alteram o objeto na qual
	 * são chamadas.
	 */
	///@{

	/** Inserção da valor.
	 * @param x Valor a ser adicionado.
	 * @returns Uma cópia desta pilha com o valor \p x em seu fim.
	 */
	Stack<T> Push(const T& x) const;

	/** Remoção do topo.
	 * @returns Uma cópia desta pilha sem seu último elemento.
	 * @pre A pilha não deve estar vazia.
	 */
	Stack<T> Pop() const;

	///@}

	~Stack();
	
	Stack& operator=(const Stack &o);
	Stack(const Stack &o);
private:
	Stack(Node<T> *u);
};

} // namespace stack

} // namespace persistence

#include "Stack.tpp"

#endif // PERSISTENCE_STACK_HPP_
