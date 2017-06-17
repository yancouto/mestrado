/** @file
 * Implementação de uma árvore rubro-negra parcialmente persistente (ARBPP).
 * Feita utilizando o método de %Node copying, por Driscoll et. al. \cite DriscollSST1989 ,
 * e baseado na implementação de Cormen et. al. \cite CormenRedBlack
 */

#ifndef REDBLACKTREE_HPP_
#define REDBLACKTREE_HPP_

/// Nó da árvore rubro-negra parcialmente persistente.
/**
 * O nó armazena um valor do tipo T.
 * @see RedBlackTree mais info
 */
template<class T> class Node {
	public:

	/** @name Campos originais
	 * Campos existentes em um nó de uma ARB não-persistente.
	 */
	///@{
	/// Nós filhos do nó.
	/** 0 é o nó esquerdo e 1 o direito. */
	Node *child[2];

	/// Cor do nó (vermelho ou preto)
	/** Este campo só armazena a cor de nós ativos, ou seja, nós com `#copy = nullptr`.
	 * Caso o nó não seja ativo, o valor é inválido e deve ser ignorado.
	 */
	bool red;

	/// Valor armazenado pelo nó.
	/**
	 * Como este é um nó de uma árvore de busca binária, vale que os valores dos nós em sua
	 * subárvore esquerda (descendentes de `#child[0]`) tem valor menor ou igual à `#value`,
	 * e valores de nós em sua subárvore direita tem valor maior ou igual à `#value`.
	 */
	T value;
	///@}
	
	/** @name Campos persistentes
	 * Campos usados para manter o nó persistente.
	 */
	///@{
	/// Tempo de criação do nó.
	/** Índice da operação de modificação na qual o nó foi criado. */
	int timestamp;

	/// Cópia do nó (se não é ativo).
	/**
	 * Operações de modificação podem causar a cópia de nós. Nesse caso, o nó que foi criado
	 * pela cópia deste é armazenado neste campo.
	 *
	 * Se `#copy = null`, o nó é ativo e é válido para as versões a partir da versão #timestamp
	 * até a mais atual. Caso contrário, o nó é valido para as versões a partir de #timestamp
	 * até `copy.timestamp - 1`.
	 */
	Node *copy;

	/// Nó pai do nó.
	/**
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

	/// Ponteiro extra.
	/**
	 * Se #extraTimestamp = -1, esse campo é inválido e deve ser ignorado. Caso contrário,
	 * representa o ponteiro extra do nó. Seu valor pode ser nulo. O campo #extraSide indica
	 * se este é um ponteiro esquerdo ou direito.
	 */
	Node<T> *extra;

	/// Lado do ponteiro extra.
	/** Se #extraTimestamp = -1, esse campo é inválido e deve ser ignorado. Caso contrário,
	 * representa o lado do ponteiro extra.
	 * @see #extra
	 * @remark É necessário armazenar este valor pois o ponteiro extra pode ser nulo, ou
	 * podem existir valores repetidos na árvore, e nesses casos não é possível inferir o
	 * lado do ponteiro apenas comparando seu valor com #value.
	 */
	bool extraSide;

	/// Tempo de criação do ponteiro extra.
	/**
	 * O campo armazena a versão em que o ponteiro extra foi adicionado ao nó, ou `-1` se este
	 * nó não tem ponteiro extra.
	 */
	int extraTimestamp;

	///@}
};

#endif // REDBLACKTREE_HPP_
