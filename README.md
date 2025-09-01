# Sistema de Gerenciamento de Biblioteca

Este é um projeto prático para a disciplina de Algoritmos e Programação II, com o objetivo de criar um sistema de gerenciamento de biblioteca utilizando a linguagem C.

O programa permite o gerenciamento completo de livros, usuários e empréstimos, com persistência de dados em arquivos binários.

---

### Funcionalidades

O sistema possui um menu interativo que oferece as seguintes funcionalidades:

* **Gerenciamento de Livros**:
    * Cadastrar, editar, consultar e remover livros.
    * Listar todos os livros cadastrados.
    * Valida a unicidade do título e impede a remoção de livros que estão em empréstimos.

* **Gerenciamento de Usuários**:
    * Cadastrar, editar, consultar e remover usuários.
    * Listar todos os usuários cadastrados.
    * Valida a unicidade do CPF e impede a remoção de usuários com empréstimos registrados.

* **Gerenciamento de Empréstimos**:
    * Registrar novos empréstimos com base em livros e usuários já cadastrados.
    * Atualizar empréstimos (marcar como devolvido).
    * Consultar empréstimos por ID, título de livro ou CPF do usuário.
    * Gerar relatórios detalhados em arquivos de texto.

---

### Estrutura do Projeto

O programa é composto por três estruturas principais, que são salvas separadamente em arquivos binários para garantir a persistência dos dados:

* **`Livro`**: Armazena informações sobre os livros (ID, título, autor, ano de publicação e disponibilidade).
* **`Usuario`**: Armazena informações dos usuários (ID, nome, CPF e e-mail).
* **`Emprestimo`**: Estabelece a relação entre um `Livro` e um `Usuario`, registrando a data e o status do empréstimo.

O projeto utiliza alocação dinâmica de memória (`realloc`) para gerenciar os vetores de estruturas de forma eficiente, expandindo o espaço de memória conforme novos registros são adicionados.

---

### Como Compilar e Executar

O projeto foi desenvolvido em **C** e é compatível com o **CodeBlocks versão 25.03** no sistema operacional **MS-Windows**.

1.  Abra o arquivo de projeto (`.cbp`) no CodeBlocks.
2.  Compile o projeto (Build).
3.  Execute o programa (Run).

Os dados serão automaticamente salvos nos arquivos `livros.bin`, `usuarios.bin` e `emprestimos.bin` para uso futuro.

---
