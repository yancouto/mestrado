/** @file
 * Implementação de uma deque persistente de acordo com o capítulo 5.
 * Veja Deque para mais informação.
 */

#ifndef DEQUE3_HPP_
#define DEQUE3_HPP_

#include <deque>

namespace deque3 {

/** Deque não persistente de até 5 elementos.
 * Usada como auxiliar por deque3::Deque. Funciona como std::deque.
 * Implementada com um vetor cíclico. Classe auxiliar, não deve ser usada.
 */
template<class T> class SubDeque {
public:
	inline int size() const;

	void push_back(const T &x);
	void push_front(const T &x);

	void pop_back();
	void pop_front();

	const T& front() const;
	const T& back() const;


	const T& operator [](int i) const;

	SubDeque();

private:
	/** Vetor que armazena os elementos.
	 * armazena até 5 elementos.
	 */
	T v[5];

	/** Posição de início.
	 * Nesta posição se iniciam os elementos da deque.
	 */
	int i;

	/** Tamanho da deque.
	 * Variável interna.
	 */
	int size_;
};

/** Nó da deque de Kaplan e Tarjan.
 * Um nó representa uma deque persistente e tem três campos: #preffix, #center e #suffix.
 * #preffix e #suffix são deques não persistentes de tamanho até 5.
 * A deque resultante é a deque que tem como primeiros elementos os elementos de #preffix,
 * últimos elementos os de #suffix, e demais elementos são armazenados em pares na sub-deque
 * #center .
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
	SubDeque<const void*> preffix;
	/** Sub-deque central.
	 * Uma deque que armazena pares de elementos de \c T , e tem os elementos "do meio" da deque.
	 */
	const Node *child;
	/** Sufixo da deque.
	 * Se for não-nulo, #suffix um ponteiro para o valor do último elemento da deque.
	 */
	SubDeque<const void*> suffix;

	/** Próximo nó na pilha de nós.
	 * É o próximo nó que tem digito diferente de 1.
	 */
	const Node *next;

	/** Tamanho da deque.
	 * Número de elementos total da deque. Esse número vai ser duas vezes o número de elementos
	 * em #center mais o tamanho de #preffix e #suffix.
	 */
	int size;

	/** Construtor.
	 * Sub-deques são inicializadas vazias. #next é inicializado com \c null.
	 * @param c Nó #center
	 */
	Node(const Node *c);
};

/** Deque persistente de Kaplan e Tarjan.
 * Uma deque é uma lista que permite adições e remoções em suas duas pontas. Implementamos aqui
 * uma deque persistente que também permite acesso ao k-ésimo elemento. Esta utiliza uma tecnica
 * descrita por Kaplan e Tarjan @cite KaplanT1999 .
 *
 * A deque armazena elementos do tipo `T`, e só assume que esse tipo tem um construtor de cópia.
 * Uma diferença do código discutido no Capítulo 5 da tese é que aqui as funções não recebem a
 * deque, mas são métodos desta. Além disso, as funções Pop não retornam o objeto removido, apenas
 * a nova deque.
 *
 * O objeto é imutável, e como a implementação é funcional, podemos usá-la de forma persistente,
 * como no seguinte exemplo:
 *
 * @code
 * using namespace deque3;
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

/** @name Funções auxiliares
 * Estas funções, detalhadas no Capítulo 5, são utilizadas pelas funções internas da deque.
 */
///@{

/** Checa e restaura a regularidade.
 * Checa se a estrutura é regular, se não for, chama Fix para a tornar regular novamente.
 * @param a Nó raiz da deque.
 * @relates deque3::Deque
 * @warning Essa função deve ser chamada com uma cópia do nó, já que ela modifica \p a.
 */
void Check(Node *a);

/** Cópia de nó.
 * Retorna uma cópia do nó, ou um novo nó vazio se \p a é nulo.
 * @param a Nó a ser copiado
 * @returns Cópia de \p a.
 * @relates deque3::Deque
 */
Node* Copy(const Node *a);

/** Dígito do nível \p a.
 * @param a Nível cujo dígito é retornado.
 * @param last Booleano indicando se este é o último nível.
 * @returns Dígitio do nó \p a.
 * @relates deque3::Deque
 */
int Digit(const Node *a, bool last);

/** Transforma uma estrutura semi-regular em regular.
 * @param a Primeiro nó com dígito 2.
 * @relates deque3::Deque
 * @warning Essa função deve ser chamada com uma cópia do nó, já que ela modifica \p a.
 */
void Fix(Node *a);

/** Balanceia as sub-deques \p l e \p r.
 * Dado que \p l e \p são sub-deques do primeiro nível com dígito 2 e a estrutura é semi-regular,
 * esse procedimento transforma o dígito do nível em 0, aumentando o do próximo em nó máximo 1,
 * ou esvaziando este nível.
 * @param l, r Sub-deques de um nível \c i.
 * @param L, R Sub-deques do nível \c i+1.
 * @pre Devem existir mais de 3 elementos de nível \c i entre todas as sub-deques, ou seja,
 * `l.size() + r.size() + 2*L.size() + 2*R.size() > 3`.
 * @relates deque3::Deque
 */
void FixDeques(SubDeque<const void*> &l, SubDeque<const void*> &r,
               SubDeque<const void*> &L, SubDeque<const void*> &R);

///@}

} // namespace deque3

#include "Deque3.tpp"

#endif // DEQUE3_HPP_
