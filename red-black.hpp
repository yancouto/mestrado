#include <vector>
#include <climits>

/* Implementação de uma árvore rubro-negra parcialmente persistente utilizando o método de Node
 * copying. A estrutura não usará ponteiros de pai, então cada nó (que não é a raiz) tem 1 ponteiro
 * para ele, e assim é necessário apenas 1 campo extra.
 */


/* Estrutura que representa um nó da ABB que armazena objetos do tipo T. */
template<class T> struct Node {
	// Filho esquerdo (0) e direito (1)
	Node *child[2];
	// Filho adicional, e o seu tempo de criação
	Node *extra; int extraTimestamp;
	// Cor do nó. Este campo não é persistente (só é válido para nós atuais)
	bool red;
	// Valor armazenado no nó
	T value;
	// Pai do nó (só é válido para nós atuais)
	Node *parent;

	Node(const T& val) : extra(nullptr), extraTimestamp(-1), red(true), value(val) {
		child[0] = child[1] = nullptr;
	}

	// Retorna o filho side do nó, considerando o filho adicional, se possível.
	Node* childAt(bool side, int time=INT_MAX);
	/* Muda o filho apropriado para v, possivelmente propagando essa mudança, e retorna o ponteiro
	 * para o nó que atualmente representa este nó. time é o tempo atual. */
	Node* change(Node *v, int time);
};

/* Arvore rubro negra que armazena objetos do tipo T.
 * T deve ser comparável (possuir operator <) */
template<class T> struct RedBlackTree {
	// Versão 0 é a árvore vazia
	std::vector<Node<T>*> roots;

	RedBlackTree() { roots.push_back(nullptr); }

	// Ignora a versão 0
	inline int versionCount() { return roots.size() - 1; }

	/* Procura pelo objeto val na ABB criada com tempo time, e retorna um ponteiro constante para
	 * o objeto, se for encontrado, e nullptr caso contrário.
	 * Restrições: 0 <= time < version_count() */
	const T* find(int time, const T& val);

	/* Insere o objeto val na ABB em sua versão mais atual, e retorna o tempo da nova versão.
	 * Tempo: O(lg(tamanho da ABB)) */
	int insert(const T& val);

	/* Remove o objeto val na ABB em sua versão mais atual, e retorna o tempo da nova versão.
	 * Tempo: O(lg(tamanho da ABB)) */
	int erase(const T& val);
};
