[![Build Status](https://travis-ci.org/yancouto/mestrado.svg?branch=master)](https://travis-ci.org/yancouto/mestrado)
[![PDF Link](https://img.shields.io/badge/thesis-pdf-brightgreen.svg)](https://yancouto.github.io/mestrado/thesis.pdf)
[![Documentação](https://img.shields.io/badge/documentation-doxygen-brightgreen.svg)](https://yancouto.github.io/mestrado/)

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
compilá-los basta usar `make` nesta pasta (e ter pdflatex e as bibliotecas necessárias :D). Uma
versão atualizada do `.pdf` fica [neste link](https://yancouto.github.io/mestrado/thesis.pdf).

### Implementações:
As implementações feitas estão na pasta `source`. A documentação para as estruturas implementadas
fica [neste link](https://yancouto.github.io/mestrado/). Para testar as implementações, basta
rodar o comando `make` nessa pasta.

Os testes são feitos usando [googletest](https://github.com/google/googletest/).
Pode ser necessário rodar `git submodule update --init --recursive` quando se clona o repositório
pela primeira vez, se a pasta `source/test/gtest/` estiver vazia.
