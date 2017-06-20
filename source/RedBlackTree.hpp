/** @file
 * Implementação de uma árvore rubro-negra (ARN) parcialmente persistente.
 * Veja RedBlackTree para mais informação.
 */

#ifndef REDBLACKTREE_HPP_
#define REDBLACKTREE_HPP_

#include <vector>

/** Nó da ARN parcialmente persistente.
 * @see RedBlackTree Para mais informação sobre a ARN.
 */
template<class T> class Node {
	public:

	/** @name Campos originais
	 * Campos existentes em um nó de uma ARN não-persistente.
	 */
	///@{

	/** Nós filhos do nó.
	 * 0 é o nó esquerdo e 1 o direito.
	 */
	Node *child[2];

	/** Cor do nó (vermelho ou preto)
	 * Este campo só armazena a cor de nós ativos, ou seja, nós com `#copy = nullptr`.
	 * Caso o nó não seja ativo, o valor é inválido e deve ser ignorado.
	 */
	bool red;

	/** Valor armazenado pelo nó
	 * Como este é um nó de uma árvore de busca binária, vale que os valores dos nós em sua
	 * subárvore esquerda (descendentes de `#child[0]`) tem valor menor ou igual à `#value`,
	 * e valores de nós em sua subárvore direita tem valor maior ou igual à `#value`.
	 */
	const T value;
	///@}

	/** @name Campos persistentes
	 * Campos usados para manter o nó persistente.
	 */
	///@{

	/** Tempo de criação do nó.
	 * Índice da operação de modificação na qual o nó foi criado.
	 */
	int timestamp;

	/** Cópia do nó (se não é ativo).
	 * Operações de modificação podem causar a cópia de nós. Nesse caso, o nó que foi criado
	 * pela cópia deste é armazenado neste campo.
	 *
	 * Se `#copy = null`, o nó é ativo e é válido para as versões a partir da versão #timestamp
	 * até a mais atual. Caso contrário, o nó é valido para as versões a partir de #timestamp
	 * até `copy.timestamp - 1`.
	 */
	Node *copy;

	/** Nó pai do nó.
	 * Este campo só armazena os pais de nós ativos, ou seja, nós com `#copy = nullptr`.
	 * Caso o nó não seja ativo, **nullptr** é armazenado.
	 *
	 * Mais especificamente, o campo armazena qual nó ativo, se algum, aponta para este nó.
	 */
	Node *parent;

	/** @name Ponteiro extra
	 * Campos associados ao ponteiro extra deste nó. O nó tem um ponteiro extra se
	 * `#extraTimestamp != -1`. Nesse caso, esse ponteiro extra é #extra, foi criado na versão
	 * #extraTimestamp, e tem lado #extraSide.
	 */
	///@{

	/** Ponteiro extra.
	 * Se #extraTimestamp = -1, esse campo é inválido e deve ser ignorado. Caso contrário,
	 * representa o ponteiro extra do nó. Seu valor pode ser nulo. O campo #extraSide indica
	 * se este é um ponteiro esquerdo ou direito.
	 */
	Node *extra;

	/** Lado do ponteiro extra.
	 * Se #extraTimestamp = -1, esse campo é inválido e deve ser ignorado. Caso contrário,
	 * representa o lado do ponteiro extra.
	 * @see #extra
	 * @remark É necessário armazenar este valor pois o ponteiro extra pode ser nulo, ou
	 * podem existir valores repetidos na árvore, e nesses casos não é possível inferir o
	 * lado do ponteiro apenas comparando seu valor com #value.
	 */
	bool extraSide;

	/** Tempo de criação do ponteiro extra.
	 * O campo armazena a versão em que o ponteiro extra foi adicionado ao nó, ou `-1` se este
	 * nó não tem ponteiro extra.
	 */
	int extraTimestamp;

	///@}

	/** Construtor padrão.
	 * @param val Valor para o nó.
	 * @param version Versão de criação.
	 */
	Node(const T& val, int version);
};

/** Árvore rubro-negra (ARN) parcialmente persistente.
 * Feita utilizando o método de %Node copying, por Driscoll et. al. \cite DriscollSST1989 ,
 * para tornar a estrutura persistente e baseado na implementação (não persistente)
 * de Cormen et. al. \cite CormenRedBlack .
 *
 * O tipo \c T deve ser comparável usando o operador \p <.
 *
 * Ao criar a árvore, esta tem versão 0 e está vazia. Cada operação de modificação (#Insert,
 * #Remove) modifica a versão mais nova da árvore e cria uma nova versão. As operações de
 * acesso (#Find) podem ser realizadas qualquer versão já criada da ARN.
 */
template<class T> class RedBlackTree {
	public:

	/** @name Biblioteca pública
	 * Estas funções podem ser usadas mesmo sem saber a estrutura por trás, como em um
	 * [set](http://www.cplusplus.com/reference/set/set/), que é implementado usando uma ARN
	 * mas cujas funções não dependem disso e podem ser implementadas com outras estruturas.
	 *
	 * As funções servem como uma ED que armazena um conjunto de elementos, parcialmente
	 * persistente, com inserção e remoção em tempo logaritmico.
	 */
	///@{
	/** Busca por um valor em dada versão da ARN.
	 * @param val Valor a ser buscado.
	 * @param version Versão da árvore na qual o valor deve ser buscado.
	 * @returns Um ponteiro para o valor de **algum** nó com valor \p val na árvore, ou nulo
	 * se tal nó não existir.
	 */
	const T* Find(const T& x, int version);

	/** Inserção de valor na ARN.
	 * Cria um novo nó com valor \p val e adiciona este nó à versão mais recente da árvore.
	 * @param value Valor a ser adicionado.
	 */
	 void Insert(const T& value);

	/** Remoção de valor na ARN.
	 * Busca por **algum** nó com valor igual \p val e remove este nó da versão mais recente
	 * da árvore. Retorna um ponteiro para o valor do nó removido, ou nulo se não existe tal nó.
	 * @param value Valor a ser removido.
	 */
	 const T* Remove(const T& value);

	/** Versão atual da ARN.
	 * @returns Quantas operações de modificação já foram realizadas na estrutura.
	 */
	inline int current();
	///@}

	/** @name Campos, construção e acesso
	 */
	///@{
	/** Raízes de todas as versões.
	 * `#roots[i]` é a raiz da ARN em sua versão i. O vetor é inicializado com apenas um elemento
	 * com valor nulo, pois a estrutura começa em sua versão 0, vazia.
	 */
	std::vector<Node<T>*> roots;
	

	/** Construtor.
	 * Constrói a estrutura com apenas uma versão 0 vazia.
	 */
	RedBlackTree();
	/** Destrutor.
	 * O destrutor apaga também todos os nós criados pela estrutura.
	 */
	~RedBlackTree();

	/** Retorna um filho de um nó em dada versão.
	 * @param u Um nó da ARN cuja versão inclui \p version, ou seja,
	 * \p version ∈ `[u.version, u.copy.version - 1]`.
	 * @param side Lado do filho desejado.
	 * @param version Versão desejada.
	 * @returns O filho \p side de \p u na versão \p version.
	 * @remarks Essa função é utilizada em operações de acesso.
	 */
	Node<T>* Child(Node<T> *u, bool side, int version);
	///@}

	/** @name Funções auxiliares de modificação
	 * As seguintes funções são auxiliares e usadas internamente durante operações de
	 * modificação. Estas funções sempre recebem um nó `u`, e é necessário que `u` tenha sido
	 * criado na atual operação (de acesso ou modificação) ou tenha sido ativo
	 * no **início** desta operação.
	 *
	 * Todas as funções terão então a pre-condição: "u ou sua cópia são ativos".
	 * Como sempre queremos modificar ou acessar a versão mais nova do nó `u`, essa condição
	 * garante que estas funções precisem apenas modificar `u` ou sua cópia.
	 */
	///@{

	/** Versão ativa de um nó.
	 * @param u Um nó da ARN.
	 * @returns Versão ativa de \p u.
	 * @pre \p u ou sua cópia são ativos.
	 */
	Node<T>* Active(Node<T> *u);

	/** Retorna um filho de um nó na versão atual.
	 * @param u Um nó da ARN.
	 * @param side Lado desejado do filho.
	 * @pre \p u ou sua cópia são ativos.
	 * @see Child
	 */
	Node<T>* Child(Node<T> *u, bool side);

	/** Modificação de filho de um nó.
	 * Modifica o lado \p side de \p u para apontar para \p v.
	 *
	 * @param u Um nó da ARN.
	 * @param side Lado do nó a ser modificado.
	 * @param v Novo valor para ponteiro.
	 * @pre \p u ou sua cópia são ativos.
	 * @pre \p v ou sua cópia são ativos.
	 * @remark Não é possível inferir \p side a partir de \p u e \p v apenas comparando seus
	 * valores, já que \p pode ser nulo ou a árvore pode ter muitos nós com o mesmo valor.
	 * @warning Esta modificação pode causar a cópia ou modificação de \p u e seus ancestrais.
	 */
	void Modify(Node<T> *u, bool side, Node<T> *v);

	/** Copia um nó.
	 * Cria uma cópia de \p u com as versões mais atuais de seus campos. Usado por #Modify.
	 * @param u Um nó da ARN.
	 * @return Uma cópia de \p u.
	 * @pre \p u deve ser um nó ativo.
	 * @warning Esta modificação pode causar a cópia ou modificação dos ancestrais de \p u.
	 */
	Node<T> *Copy(Node<T> *u);

	/** Rotaciona um nó.
	 * Rotaciona \p u de forma que seu filho de lado \p side tome seu lugar, enquanto mantém
	 * as propriedades de uma árvore binária.
	 * @param u Um nó da ARN.
	 * @param side Lado da rotação.
	 * @pre \p u ou sua cópia são ativos.
	 * @pre \p u deve ter um filho de lado \p side na versão mais atual.
	 * @warning Esta rotação pode causar a cópia ou modificação de \p u, seu filho de lado
	 * \p side e de seus ancestrais.
	 * @warning A rotação não mantém as propriedades rubro-negras.
	 */
	void Rotate(Node<T> *u, bool side);

	/** Subsitui um nó.
	 * A subárvore de \p u é substituida pela subárvore de \p x, isto é, \p x é desconectado
	 * de seu pai, se existir, e então troca-se o filho do pai de \p u de \p u para \p x.
	 * @param u, x Nós da ARN.
	 * @pre \p u ou sua cópia são ativos.
	 * @pre \p x ou sua cópia são ativos.
	 * @warning A substituição pode causar a cópia ou modificação de \p u, \p x, e seus
	 * ancestrais.
	 */
	void Transplant(Node<T> *u, Node<T> *x);

	/** Menor elemento em uma subárvore na versão atual.
	 * @param u Um nó da ARN.
	 * @returns Menor elemento na subárvore de u, na versão atual.
	 * @pre \p u ou sua cópia são ativos.
	 */
	Node<T>* MinElement(Node<T> *u);

	/** Arruma violações causadas por remoção.
	 * @param y, side Nó da ARN e lado tal que os caminhos até links nulos que passam por \p y
	 * em direção à \p side tem um nó preto a menos que os outros.
	 * @pre \p y ou sua cópia são ativos.
	 * @warning A função pode causar a cópia ou modificação de \p y e outros nós.
	 */
	void AddBlack(Node<T> *y, bool side);

	///@}

};

#include "RedBlackTree.tpp"

#endif // REDBLACKTREE_HPP_
