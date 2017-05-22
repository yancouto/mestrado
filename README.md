## Estruturas de dados avançadas

### Tópicos:
- Estruturas persistentes
	- Pilha
	- Fila
	- Deque
	- Técnicas gerais
	- Árvore rubro-negra
- Estruturas retroativas
- Árvores de busca binária adaptativas
- Algoritmos de Level Ancestor e Lowest Common Ancestor

### Dissertação:
Os arquivos `.tex` necessários para criar o pdf da dissertação estão na pasta `thesis`. Para
compilá-los basta usar `pdflatex main.tex` nesta pasta (e ter as bibliotecas necessárias :D). Uma
versão atualizada do `.pdf` fica [neste link](https://www.ime.usp.br/~yancouto/stuff/tese.pdf).

### Implementações:
As implementações feitas estão na pasta `source`. Para testá-las, basta rodar o comando `make`
nessa pasta.

Os testes são feitos usando [googletest](https://github.com/google/googletest/).
Pode ser necessário rodar `git submodule update --init --recursive` quando se clona o repositório
pela primeira vez, se a pasta `source/test/gtest/` estiver vazia.
