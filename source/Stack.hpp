#ifndef STACK_HPP_
#define STACK_HPP_


/* Implementação de uma pilha totalmente persistente. */
template<class T> struct Stack {
	// Valor armazenado
	T value;
	// Próximo nó da lista ligada
	Stack *next;

	Stack(const T& val, Stack *nx) : value(val), next(nx) {}
	Stack() : next(nullptr) {} // pilha vazia

	// Retorna o primeiro elemento da pilha, sem modificá-la.
	const T& top() { return value; }
	// Remove o primeiro elemento da pilha, e retorna a nova pilha.
	Stack* pop() { return next; }
	// Insere o elemento val no início da pilha, e retorna a nova pilha.
	Stack* push(const T& val) { return new Stack(val, this); }
};

#endif
