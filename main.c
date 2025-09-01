#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITULO 50
#define MAX_AUTOR 20

#define MAX_NOME 20
#define MAX_CPF 13
#define MAX_EMAIL 20

#define MAX_DATA 15
#define STATUS_EMPRESTADO 0
#define STATUS_DEVOLVIDO 1

#define MEMORIAEXTRA 5

typedef struct livro
{
    int idLivro;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int anoPublicacao;
    int disponivel;
} Livro;

typedef struct usuario
{
    int idUsuario;
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char email[MAX_EMAIL];
} Usuario;

typedef struct emprestimo
{
    int idEmprestimo;
    Livro livro;
    Usuario usuario;
    char dataRetirada[MAX_DATA];
    char dataDevolucao[MAX_DATA];
    int status;
} Emprestimo;

Livro *livro = NULL;
Usuario *usuario = NULL;
Emprestimo *emprestimo = NULL;

int capacidadeLivro = 0, totalLivro = 0;
int capacidadeUsuario = 0, totalUsuario = 0;
int capacidadeEmprestimo = 0, totalEmprestimo = 0;

void menu();
void limpaBuffer();

void gerenciarLivros();
void cadastrarLivro();
void atualizarLivro();
void consultarLivro();
void removerLivro();
void listarTodosLivros();
Livro* buscaLivroId(int id);
Livro* buscaLivroTitulo(char *titulo);

void gerenciarUsuarios();
void cadastrarUsuario();
void atualizarUsuario();
void consultarUsuario();
void removerUsuario();
void listarTodosUsuarios();
Usuario* buscaUsuarioId(int id);
Usuario* buscaUsuarioCpf(char *cpf);
void imprimeCpfFormatado(char *cpf);

void gerenciarEmprestimos();
void cadastrarEmprestimo();
void atualizarEmprestimo();
void consultarEmprestimo();
void removerEmprestimo();
void listarTodosEmprestimos();
Emprestimo* buscaEmprestimoId(int id);
void gerarRelatorioEmprestimo();

void carregarDados();
void salvarDados();


int main()
{
    livro = NULL;
    usuario = NULL;
    emprestimo = NULL;
    totalLivro = 0;
    totalUsuario = 0;
    totalEmprestimo = 0;

    carregarDados();

    menu();

    salvarDados();

    free(livro);
    free(usuario);
    free(emprestimo);

    return 0;
}

void menu()
{
    int opcao;
    do
    {
        printf("=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar Livros.\n");
        printf("2. Gerenciar Usuarios.\n");
        printf("3. Gerenciar Emprestimos.\n");
        printf("4. Sair do programa.\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch (opcao)
        {
        case 1:
            gerenciarLivros();
            break;
        case 2:
            gerenciarUsuarios();
            break;
        case 3:
            gerenciarEmprestimos();
            break;
        case 4:
            printf("\nEncerrando o programa. Ate logo!\n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n\n");
        }
    }
    while (opcao != 4);
}

void limpaBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void salvarDados()
{
    FILE *flivro = fopen("livros.bin", "wb");
    FILE *fusuario = fopen("usuarios.bin", "wb");
    FILE *femprestimo = fopen("emprestimos.bin", "wb");

    if(flivro == NULL || fusuario == NULL || femprestimo == NULL)
    {
        printf("Erro ao salvar os arquivos.\n");
        return;
    }

    fwrite(&totalLivro, sizeof(int), 1, flivro);
    fwrite(livro, sizeof(Livro), totalLivro, flivro);

    fwrite(&totalUsuario, sizeof(int), 1, fusuario);
    fwrite(usuario, sizeof(Usuario), totalUsuario, fusuario);

    fwrite(&totalEmprestimo, sizeof(int), 1, femprestimo);
    fwrite(emprestimo, sizeof(Emprestimo), totalEmprestimo, femprestimo);

    fclose(flivro);
    fclose(fusuario);
    fclose(femprestimo);
}

void carregarDados()
{
    FILE *flivro = NULL;
    FILE *fusuario = NULL;
    FILE *femprestimo = NULL;

    flivro = fopen("livros.bin", "rb");
    if (flivro != NULL)
    {
        fread(&totalLivro, sizeof(int), 1, flivro);
        if (totalLivro > 0)
        {
            capacidadeLivro = totalLivro + MEMORIAEXTRA;
            livro = (Livro*)realloc(livro, capacidadeLivro * sizeof(Livro));
            fread(livro, sizeof(Livro), totalLivro, flivro);
        }
        fclose(flivro);
    }
    else
    {
        livro = (Livro*)malloc(MEMORIAEXTRA * sizeof(Livro));
        if (livro == NULL)
        {
            printf("Erro: memoria insuficiente para livros!\n");
            return;
        }
        capacidadeLivro = MEMORIAEXTRA;
    }

    fusuario = fopen("usuarios.bin", "rb");
    if (fusuario != NULL)
    {
        fread(&totalUsuario, sizeof(int), 1, fusuario);
        if (totalUsuario > 0)
        {
            capacidadeUsuario = totalUsuario + MEMORIAEXTRA;
            usuario = (Usuario*)realloc(usuario, capacidadeUsuario * sizeof(Usuario));
            fread(usuario, sizeof(Usuario), totalUsuario, fusuario);
        }
        fclose(fusuario);
    }
    else
    {
        usuario = (Usuario*)malloc(MEMORIAEXTRA * sizeof(Usuario));
        if (usuario == NULL)
        {
            printf("Erro: memoria insuficiente para usuarios!\n");
            return;
        }
        capacidadeUsuario = MEMORIAEXTRA;
    }

    femprestimo = fopen("emprestimos.bin", "rb");
    if (femprestimo != NULL)
    {
        fread(&totalEmprestimo, sizeof(int), 1, femprestimo);
        if (totalEmprestimo > 0)
        {
            capacidadeEmprestimo = totalEmprestimo + MEMORIAEXTRA;
            emprestimo = (Emprestimo*)realloc(emprestimo, capacidadeEmprestimo * sizeof(Emprestimo));
            fread(emprestimo, sizeof(Emprestimo), totalEmprestimo, femprestimo);
        }
        fclose(femprestimo);
    }
    else
    {
        emprestimo = (Emprestimo*)malloc(MEMORIAEXTRA * sizeof(Emprestimo));
        if (emprestimo == NULL)
        {
            printf("Erro: memoria insuficiente para emprestimos!\n");
            return;
        }
        capacidadeEmprestimo = MEMORIAEXTRA;
    }

}

void gerenciarLivros()
{
    int opcao;

    do
    {
        printf("\n=== GERENCIAMENTO DE LIVROS ===\n");
        printf("1. Cadastrar novo livro.\n");
        printf("2. Editar livro existente.\n");
        printf("3. Consultar livro por ID\n");
        printf("4. Remover livro.\n");
        printf("5. Listar todos os livros.\n");
        printf("6. Voltar ao menu principal.\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch (opcao)
        {
        case 1:
            cadastrarLivro();
            break;
        case 2:
            atualizarLivro();
            break;
        case 3:
            consultarLivro();
            break;
        case 4:
            removerLivro();
            break;
        case 5:
            listarTodosLivros();
            break;
        case 6:
            printf("\nRetornando ao menu principal...\n\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }

    }
    while (opcao != 6);
}

Livro* buscaLivroId(int id)
{
    for(int i = 0; i < totalLivro; i++)
    {
        if(livro[i].idLivro == id)
        {
            return &livro[i];
        }
    }
    return NULL;
}

Livro* buscaLivroTitulo(char *titulo)
{
    for(int i = 0; i < totalLivro; i++)
    {
        if(strcmp(livro[i].titulo, titulo) == 0)
        {
            return &livro[i];
        }
    }
    return NULL;
}

void cadastrarLivro()
{
    if(totalLivro >= capacidadeLivro)
    {
        capacidadeLivro += MEMORIAEXTRA;

        Livro *vetLivro = (Livro*)realloc(livro, capacidadeLivro * sizeof(Livro));
        if(vetLivro == NULL)
        {
            printf("Erro: memoria insuficiente!\n");
            return;
        }
        livro = vetLivro;
    }

    Livro novoLivro;
    novoLivro.idLivro = totalLivro + 1;

    printf("\n=== CADASTRAR LIVRO ===\n");

    int validaTitulo = 0;
    do
    {
        printf("Titulo: ");
        fgets(novoLivro.titulo, sizeof(novoLivro.titulo), stdin);

        if(strchr(novoLivro.titulo, '\n') == NULL)
        {
            limpaBuffer();
            printf("Erro: O titulo e muito longo (limite 50 caracteres).\n");
        }
        else
        {
            novoLivro.titulo[strcspn(novoLivro.titulo, "\n")] = '\0';
            if (strlen(novoLivro.titulo) == 0)
            {
                printf("Erro: O titulo nao pode ser vazio.\n");
            }
            else if (buscaLivroTitulo(novoLivro.titulo) != NULL)
            {
                printf("Erro: Ja existe um livro com este titulo.\n");
            }
            else
            {
                validaTitulo = 1;
            }
        }

    }
    while(!validaTitulo);

    int validaAutor = 0;
    do
    {
        printf("Autor: ");
        fgets(novoLivro.autor, sizeof(novoLivro.autor), stdin);

        if(strchr(novoLivro.autor, '\n') == NULL)
        {
            limpaBuffer();
            printf("Erro: O nome do autor e muito longo (limite 20 caracteres).\n");
        }
        else
        {
            novoLivro.autor[strcspn(novoLivro.autor, "\n")] = '\0';
            if (strlen(novoLivro.autor) == 0)
            {
                printf("Erro: O autor nao pode ser vazio.\n");
            }
            else
            {
                validaAutor = 1;
            }
        }
    }
    while(!validaAutor);

    printf("Ano de Publicacao: ");
    scanf("%d", &novoLivro.anoPublicacao);
    limpaBuffer();

    novoLivro.disponivel = 1;
    livro[totalLivro++] = novoLivro;

    printf("\nLivro cadastrado com sucesso! (ID: %d)\n", novoLivro.idLivro);
}

void atualizarLivro()
{
    if(totalLivro >= 1)
    {
        int id;
        printf("Digite o ID do livro a ser editado: ");
        scanf("%d", &id);
        limpaBuffer();

        int indexAux = 0;
        int existeLivro = 0;
        char novoTitulo[MAX_TITULO];
        char novoAutor[MAX_AUTOR];
        int novoAno;

        for(int i = 0; i < totalLivro; i++)
        {
            if(livro[i].idLivro == id)
            {
                indexAux = i;
                existeLivro = 1;
            }
        }

        if(existeLivro)
        {
            printf("\nDados atuais do livro ID %d: \n", livro[indexAux].idLivro);
            printf("Titulo: %s | Autor: %s | Ano: %d\n",
                   livro[indexAux].titulo, livro[indexAux].autor, livro[indexAux].anoPublicacao);


            printf("\nNovo titulo: ");
            fgets(novoTitulo, MAX_TITULO, stdin);
            novoTitulo[strcspn(novoTitulo, "\n")] = '\0';

            if(strlen(novoTitulo) > 0)
            {
                strcpy(livro[indexAux].titulo, novoTitulo);
            }

            printf("Novo autor: ");
            fgets(novoAutor, MAX_AUTOR, stdin);
            novoAutor[strcspn(novoAutor, "\n")] = '\0';

            if(strlen(novoAutor) > 0)
            {
                strcpy(livro[indexAux].autor, novoAutor);
            }

            printf("Novo ano de publicacao: ");
            scanf("%d", &novoAno);
            limpaBuffer();

            if(novoAno != 0)
            {
                livro[indexAux].anoPublicacao = novoAno;
            }

            printf("\nLivro atualizado com sucesso!\n\n");
        }
        else
        {
            printf("Erro: Livro com ID nao encontrado.\n", id);
        }
    }
    else
    {
        printf("\nNenhum livro cadastrado para editar.\n\n");
    }
}

void consultarLivro()
{
    if(totalLivro >= 1)
    {
        int id;
        printf("Digite o ID do livro a ser consultado: ");
        scanf("%d", &id);
        limpaBuffer();

        int existeLivro = 0;

        for(int i = 0; i < totalLivro; i++)
        {
            if(livro[i].idLivro == id)
            {
                printf("ID: %d | Titulo: %s | Autor: %s | Ano: %d | Disponibilidade: ",
                       livro[i].idLivro, livro[i].titulo, livro[i].autor, livro[i].anoPublicacao);
                if(livro[i].disponivel)
                {
                    printf("Disponivel\n");
                }
                else
                {
                    printf("Indisponivel\n");
                }
                existeLivro = 1;
            }
        }

        if (!existeLivro)
        {
            printf("\nLivro com ID %d nao encontrado.\n", id);
        }
    }
    else
    {
        printf("\nNenhum livro cadastrado para consultar.\n\n");
    }

}

void removerLivro()
{
    if(totalLivro >= 1)
    {
        int id;
        printf("Digite o ID do livro a ser removido: ");
        scanf("%d", &id);
        limpaBuffer();

        int indexDelete = 0;
        int existeId = 0;
        for(int i = 0; i < totalLivro; i++)
        {
            if(livro[i].idLivro == id)
            {
                indexDelete = i;
                existeId = 1;
            }
        }

        if(existeId == 0)
        {
            printf("\nErro: Livro com ID: %d nao encontrado.\n", id);
            return;
        }

        for(int i = 0; i < totalEmprestimo; i++)
        {
            if(emprestimo[i].livro.idLivro == livro[indexDelete].idLivro)
            {
                printf("\nErro: Livro presente em um emprestimo, nao pode ser removido!\n");
                return;
            }
        }

        if(livro[indexDelete].disponivel == 0)
        {
            printf("\nErro: Livro emprestado, nao pode ser removido!\n");
            return;
        }

        livro[indexDelete] = livro[totalLivro - 1];
        totalLivro--;
        printf("\nLivro removido com sucesso!\n");
    }
    else
    {
        printf("\nNenhum livro cadastrado para remover.\n\n");
    }
}

void listarTodosLivros()
{
    if(totalLivro >= 1)
    {
        printf("\n=== LISTA DE LIVROS CADASTRADOS ===\n");
        for(int i = 0; i < totalLivro; i++)
        {
            printf("ID: %d | Titulo: %s | Autor: %s | Ano: %d | Disponibilidade: ",
                   livro[i].idLivro, livro[i].titulo, livro[i].autor, livro[i].anoPublicacao);
            if(livro[i].disponivel)
            {
                printf("Disponivel\n");
            }
            else
            {
                printf("Indisponivel\n");
            }
        }
    }
    else
    {
        printf("\nNenhum livro cadastrado.\n");
    }
}

void gerenciarUsuarios()
{
    int opcao;

    do
    {
        printf("\n=== GERENCIAMENTO DE USUARIOS ===\n");
        printf("1. Cadastrar novo usuario.\n");
        printf("2. Editar usuario existente.\n");
        printf("3. Consultar usuario por id.\n");
        printf("4. Remover usuario.\n");
        printf("5. Listar todos os usuarios.\n");
        printf("6. Voltar ao menu principal.\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch (opcao)
        {
        case 1:
            cadastrarUsuario();
            break;
        case 2:
            atualizarUsuario();
            break;
        case 3:
            consultarUsuario();
            break;
        case 4:
            removerUsuario();
            break;
        case 5:
            listarTodosUsuarios();
            break;
        case 6:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }

    }
    while (opcao != 6);
}

Usuario* buscaUsuarioId(int id)
{
    for(int i = 0; i < totalUsuario; i++)
    {
        if(usuario[i].idUsuario == id)
        {
            return &usuario[i];
        }
    }
    return NULL;
}

Usuario* buscaUsuarioCpf(char *cpf)
{
    for(int i = 0; i < totalUsuario; i++)
    {
        if(strcmp(usuario[i].cpf, cpf) == 0)
        {
            return &usuario[i];
        }
    }
    return NULL;
}

void imprimeCpfFormatado(char *cpf)
{
    if(strlen(cpf) == 11)
    {
        printf("%c%c%c.%c%c%c.%c%c%c-%c%c",
               cpf[0], cpf[1], cpf[2],
               cpf[3], cpf[4], cpf[5],
               cpf[6], cpf[7], cpf[8],
               cpf[9], cpf[10]);
    }
    else
    {
        printf("%s", cpf);
    }
}

void cadastrarUsuario()
{
    if(totalUsuario >= capacidadeUsuario)
    {
        capacidadeUsuario += MEMORIAEXTRA;

        Usuario *vetUsuario = (Usuario*)realloc(usuario, capacidadeUsuario * sizeof(Usuario));
        if(vetUsuario == NULL)
        {
            printf("Erro: memoria insuficiente!\n");
            return;
        }
        usuario = vetUsuario;
    }

    Usuario novoUsuario;
    novoUsuario.idUsuario = totalUsuario + 1;

    printf("\n=== CADASTRAR USUARIO ===\n");

    int validaNome = 0;
    do
    {
        printf("Nome: ");
        fgets(novoUsuario.nome, sizeof(novoUsuario.nome), stdin);


        if(strchr(novoUsuario.nome, '\n') == NULL)
        {
            limpaBuffer();
            printf("Erro: O nome do usuario e muito longo (limite 20 caracteres).\n");
        }
        else
        {
            novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';
            if(strlen(novoUsuario.nome) == 0)
            {
                printf("Erro: O nome nao pode ser vazio.\n");
            }
            else
            {
                validaNome = 1;
            }
        }
    }
    while(!validaNome);

    int validaCpf = 0;

    do
    {
        printf("CPF: ");
        fgets(novoUsuario.cpf, sizeof(novoUsuario.cpf), stdin);

        if(strchr(novoUsuario.cpf, '\n') == NULL)
        {
            limpaBuffer();
            printf("Erro: CPF e muito longo (limite 11 caracteres).\n");
        }
        else
        {
            novoUsuario.cpf[strcspn(novoUsuario.cpf, "\n")] = '\0';
            if(strlen(novoUsuario.cpf) == 0)
            {
                printf("Erro: O CPF nao pode ser vazio.\n");
            }
            else if(buscaUsuarioCpf(novoUsuario.cpf) != NULL)
            {
                printf(("Erro: CPF ja cadastrado.\n"));
            }
            else
            {
                validaCpf = 1;
            }
        }
    }
    while(!validaCpf);

    int validaEmail = 0;
    do
    {
        printf("Email: ");
        fgets(novoUsuario.email, sizeof(novoUsuario.email), stdin);

        if(strchr(novoUsuario.email, '\n') == NULL)
        {
            limpaBuffer();
            printf("Erro: O email do usuario e muito longo (limite 20 caracteres).\n");

        }
        else
        {
            novoUsuario.email[strcspn(novoUsuario.email, "\n")] = '\0';
            if(strlen(novoUsuario.email) == 0)
            {
                printf("Erro: O email nao pode ser vazio.\n");
            }
            else
            {
                validaEmail = 1;
            }
        }
    }
    while(!validaEmail);

    usuario[totalUsuario++] = novoUsuario;

    printf("\nUsuario cadastrado com sucesso! (ID: %d)\n", novoUsuario.idUsuario);
}


void atualizarUsuario()
{
    if(totalUsuario >= 1)
    {
        int id;
        printf("Digite o ID do usuario a ser editado: ");
        scanf("%d", &id);
        limpaBuffer();

        int indexAux = 0;
        int existeUsuario = 0;
        char novoNome[MAX_NOME];
        char novoEmail[MAX_EMAIL];

        for(int i = 0; i < totalUsuario; i++)
        {
            if(usuario[i].idUsuario == id)
            {
                indexAux = i;
                existeUsuario = 1;
            }
        }

        if(existeUsuario)
        {
            printf("Dados atuais do usuario ID %d: \n", usuario[indexAux].idUsuario);
            printf("Nome: %s | CPF: ",
                   usuario[indexAux].nome);
            imprimeCpfFormatado(usuario[indexAux].cpf);
            printf(" | Email: %s\n", usuario[indexAux].email);


            printf("\nNovo nome: ");
            fgets(novoNome, MAX_NOME, stdin);
            novoNome[strcspn(novoNome, "\n")] = '\0';

            if(strlen(novoNome) > 0)
            {
                strcpy(usuario[indexAux].nome, novoNome);
            }

            printf("Novo email: ");
            fgets(novoEmail, MAX_EMAIL, stdin);
            novoEmail[strcspn(novoEmail, "\n")] = '\0';

            if(strlen(novoEmail) > 0)
            {
                strcpy(usuario[indexAux].email, novoEmail);
            }

            printf("\nUsuario atualizado com sucesso!\n\n");
        }
        else
        {
            printf("Erro: Usuario com ID nao encontrado.\n", id);
        }
    }
    else
    {
        printf("\nNenhum usuario cadastrado para editar.\n\n");
    }
}

void consultarUsuario()
{
    int id;
    printf("Digite o ID do usuario a ser consultado: ");
    scanf("%d", &id);
    limpaBuffer();

    int existeUsuario = 0;

    for(int i = 0; i < totalUsuario; i++)
    {
        if(usuario[i].idUsuario == id)
        {
            printf("ID: %d | Nome: %s | CPF: %s | Email: %s ",
                   usuario[i].idUsuario, usuario[i].nome, usuario[i].cpf, usuario[i].email);
            existeUsuario = 1;
        }
    }
    if(!existeUsuario)
    {
        printf("\nUsuario com ID %d nao encontrado.\n", id);
    }
}

void removerUsuario()
{
    if(totalUsuario >= 1)
    {
        int id;
        printf("Digite o ID do usuario a ser removido: ");
        scanf("%d", &id);
        limpaBuffer();

        int indexDelete = 0;
        int existeId = 0;
        for(int i = 0; i < totalUsuario; i++)
        {
            if(usuario[i].idUsuario == id)
            {
                indexDelete = i;
                existeId = 1;
            }
        }

        if(existeId == 0)
        {
            printf("\nErro: Usuario com ID: %d nao encontrado.\n", id);
            return;
        }

        for(int i = 0; i < totalEmprestimo; i++)
        {
            if(emprestimo[i].usuario.idUsuario == usuario[indexDelete].idUsuario)
            {
                printf("\nErro: Usuario com emprestimos registrados, nao pode ser removido!\n");
                return;
            }
        }

        usuario[indexDelete] = usuario[totalUsuario - 1];
        totalUsuario--;
        printf("\nUsuario removido com sucesso!\n");
        return;
    }
    else
    {
        printf("\nNenhum usuario cadastrado para remover.\n\n");
    }
}

void listarTodosUsuarios()
{
    if(totalUsuario >= 1)
    {
        printf("\n=== LISTA DE USUARIOS CADASTRADOS ===\n");
        for(int i = 0; i < totalUsuario; i++)
        {
            printf("ID: %d | Nome: %s | CPF: ",
                   usuario[i].idUsuario, usuario[i].nome);
            imprimeCpfFormatado(usuario[i].cpf);
            printf(" | Email: %s\n", usuario[i].email);
        }
    }
    else
    {
        printf("\nNenhum usuario cadastrado.\n");
    }
}

void gerenciarEmprestimos()
{
    int opcao;

    do
    {
        printf("\n=== GERENCIAMENTO DE EMPRESTIMOS ===\n");
        printf("1. Registrar novo emprestimo.\n");
        printf("2. Editar emprestimo existente.\n");
        printf("3. Consultar emprestimo.\n");
        printf("4. Remover emprestimo.\n");
        printf("5. Gerar relatorio.\n");
        printf("6. Voltar ao menu principal.\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch (opcao)
        {
        case 1:
            cadastrarEmprestimo();
            break;
        case 2:
            atualizarEmprestimo();
            break;
        case 3:
            consultarEmprestimo();
            break;
        case 4:
            removerEmprestimo();
            break;
        case 5:
            gerarRelatorioEmprestimo();
            break;
        case 6:
            printf("Retornando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
    while (opcao != 6);
}

void cadastrarEmprestimo()
{
    if(totalEmprestimo >= capacidadeEmprestimo)
    {
        capacidadeEmprestimo += MEMORIAEXTRA;

        Emprestimo *vetEmprestimo = (Emprestimo*)realloc(emprestimo, capacidadeEmprestimo * sizeof(Emprestimo));
        if(vetEmprestimo == NULL)
        {
            printf("Erro: memoria insuficiente!\n");
            return;
        }
        emprestimo = vetEmprestimo;
    }

    printf("\n=== CADASTRAR EMPRESTIMO ===\n");

    char tituloLivro[MAX_TITULO];
    Livro* existeLivro = NULL;
    int validaTituloLivro = 0;

    do
    {
        printf("Titulo do livro: ");
        fgets(tituloLivro, sizeof(tituloLivro), stdin);
        tituloLivro[strcspn(tituloLivro, "\n")] = '\0';

        if(strlen(tituloLivro) == 0)
        {
            printf("Erro: O nome nao pode ser vazio.\n");
        }
        else
        {
            existeLivro = buscaLivroTitulo(tituloLivro);

            if(existeLivro == NULL)
            {
                printf("Erro: titulo de livro nao encontrado.\n");
            }
            else if((*existeLivro).disponivel == 0)
            {
                printf("Erro: livro ja esta emprestado.\n");
                existeLivro = NULL;
            }
            else
            {
                validaTituloLivro = 1;
            }
        }
    }
    while(!validaTituloLivro);

    char cpfUsuario[MAX_CPF];
    Usuario* existeUsuario = NULL;
    int validaUsuarioCpf = 0;

    do
    {
        printf("CPF do usuario: ");
        fgets(cpfUsuario, sizeof(cpfUsuario), stdin);
        cpfUsuario[strcspn(cpfUsuario, "\n")] = '\0';

        if(strlen(cpfUsuario) == 0)
        {
            printf("Erro: O CPF nao pode ser vazio.\n");
        }
        else
        {
            existeUsuario = buscaUsuarioCpf((cpfUsuario));
            if(existeUsuario == NULL)
            {
                printf("Erro: CPF nao encontrado.\n");
            }
            else
            {
                validaUsuarioCpf = 1;
            }
        }
    }
    while(!validaUsuarioCpf);

    Emprestimo novoEmprestimo;
    novoEmprestimo.idEmprestimo = totalEmprestimo + 1;
    novoEmprestimo.livro = *existeLivro;
    novoEmprestimo.usuario = *existeUsuario;

    (*existeLivro).disponivel = 0;
    printf("Data de retirada (DD/MM/AAAA): ");
    fgets(novoEmprestimo.dataRetirada, sizeof(novoEmprestimo.dataRetirada), stdin);
    novoEmprestimo.dataRetirada[strcspn(novoEmprestimo.dataRetirada, "\n")] = '\0';
    limpaBuffer();

    novoEmprestimo.status = STATUS_EMPRESTADO;
    strcpy(novoEmprestimo.dataDevolucao, "N/A");

    emprestimo[totalEmprestimo++] = novoEmprestimo;

    printf("\nEmprestimo cadastrado com sucesso! (ID: %d)\n", novoEmprestimo.idEmprestimo);
}

void atualizarEmprestimo()
{
    if(totalEmprestimo >= 1)
    {
        int id;
        printf("\nDigite o ID do emprestimo a ser editado: ");
        scanf("%d", &id);
        limpaBuffer();

        Emprestimo* existeEmprestimo = buscaEmprestimoId(id);

        if(existeEmprestimo)
        {
            printf("Dados atuais do emprestimos Id %d \n", id);
            printf("Livro: %s\n", (*existeEmprestimo).livro.titulo);
            printf("Usuario: %s\n", (*existeEmprestimo).usuario.nome);
            printf("Data retirada: %s\n", (*existeEmprestimo).dataRetirada);
            printf("Data devolucao: %s\n", (*existeEmprestimo).dataDevolucao);
            printf("Status: ");
            if((*existeEmprestimo).status == STATUS_EMPRESTADO)
            {
                printf("Emprestado\n");
            }
            else
            {
                printf("Devolvido\n");
            }

            if((*existeEmprestimo).status == STATUS_EMPRESTADO)
            {
                printf("\nDigite a data de devolucao (DD/MM/AAAA): ");
                char dataDevolucao[MAX_DATA];
                fgets(dataDevolucao, sizeof(dataDevolucao), stdin);
                dataDevolucao[strcspn(dataDevolucao, "\n")] = '\0';
                limpaBuffer();

                if(strlen(dataDevolucao) > 0)
                {
                    strcpy((*existeEmprestimo).dataDevolucao, dataDevolucao);
                    (*existeEmprestimo).status = STATUS_DEVOLVIDO;

                    Livro* livroDevolvido = buscaLivroTitulo((*existeEmprestimo).livro.titulo);
                    if(livroDevolvido != NULL)
                    {
                        (*livroDevolvido).disponivel = 1;
                    }
                    printf("\nEmprestimo atualizado com sucesso!\n");
                }
            }
            else
            {
                printf("\nEmprestimo de livro ja devolvido e nao pode ser editado.\n\n");
            }
        }
        else
        {
            printf("Erro: Emprestimo com ID %d nao encontrado.\n", id);
        }
    }
    else
    {
        printf("\nNenhum emprestimo cadastrado para editar.\n\n");
    }
}

Emprestimo* buscaEmprestimoId(int id)
{
    for(int i = 0; i < totalEmprestimo; i++)
    {
        if(emprestimo[i].idEmprestimo == id)
        {
            return &emprestimo[i];
        }
    }
    return NULL;
}

void buscarEmprestimoTitulo(char* titulo)
{
    int existeEmprestimo = 0;
    for(int i = 0; i < totalEmprestimo; i++)
    {
        if(strcmp(emprestimo[i].livro.titulo, titulo) == 0)
        {
            printf("\n--- Informacoes do Emprestimo ---\n");
            printf("ID: %d | Livro: %s | Autor: %s | Usuario: %s | Data retirada: %s | Status: ",
                   emprestimo[i].idEmprestimo, emprestimo[i].livro.titulo, emprestimo[i].livro.autor,
                   emprestimo[i].usuario.nome, emprestimo[i].dataRetirada);
            if(emprestimo[i].status == STATUS_EMPRESTADO)
            {
                printf("Emprestado\n");
            }
            else
            {
                printf("Devolvido\n");
            }
            existeEmprestimo = 1;
        }
    }

    if(!existeEmprestimo)
    {
        printf("\nNenhum emprestimo encontrado para o livro '%s'.\n", titulo);
    }
}

void buscarEmprestimoCpf(char* cpf)
{
    int existeCpf = 0;
    for(int i = 0; i < totalEmprestimo; i++)
    {
        if(strcmp(emprestimo[i].usuario.cpf, cpf) == 0)
        {
            printf("\n--- Informacoes do Emprestimo ---\n");
            printf("ID: %d | Livro: %s | Autor: %s | Usuario: %s | Data retirada: %s | Status: ",
                   emprestimo[i].idEmprestimo, emprestimo[i].livro.titulo, emprestimo[i].livro.autor,
                   emprestimo[i].usuario.nome, emprestimo[i].dataRetirada);
            if(emprestimo[i].status == STATUS_EMPRESTADO)
            {
                printf("Emprestado\n");
            }
            else
            {
                printf("Devolvido\n");
            }
            existeCpf = 1;
        }
    }
    if(!existeCpf)
    {
        printf("\nNenhum emprestimo encontrado para o CPF '%s.\n", cpf);
    }
}

void consultarEmprestimo()
{
    if(totalEmprestimo >= 1)
    {
        int opcao;
        printf("\n=== CONSULTAR EMPRESTIMO ===\n");
        printf("1. Consultar por ID\n");
        printf("2. Consultar por titulo de livro\n");
        printf("3. Consultar por CPF do usuario\n");
        printf("\nEscolha uma opcao de consulta: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch(opcao)
        {
        case 1:
        {
            int id;
            printf("Digite o ID do emprestimo a ser consultado: ");
            scanf("%d", &id);
            limpaBuffer();

            Emprestimo* existeEmprestimo = buscaEmprestimoId(id);

            if(existeEmprestimo != NULL)
            {
                printf("\n--- Informacoes do Emprestimo ---\n");
                printf("ID do Emprestimo: %d\n", (*existeEmprestimo).idEmprestimo);
                printf("Data de Retirada: %s\n", (*existeEmprestimo).dataRetirada);
                printf("Data de Devolucao: %s\n", (*existeEmprestimo).dataDevolucao);
                printf("Status: ");
                if ((*existeEmprestimo).status == STATUS_EMPRESTADO)
                {
                    printf("Emprestado\n");
                }
                else
                {
                    printf("Devolvido\n");
                }
                printf("\n--- Informacoes do Livro ---\n");
                printf("Titulo: %s\n", (*existeEmprestimo).livro.titulo);
                printf("Autor: %s\n", (*existeEmprestimo).livro.autor);
                printf("Ano de Publicacao: %d\n", (*existeEmprestimo).livro.anoPublicacao);
                printf("\n--- Informacoes do Usuario ---\n");
                printf("Nome: %s\n", (*existeEmprestimo).usuario.nome);
                printf("CPF: ");
                imprimeCpfFormatado((*existeEmprestimo).usuario.cpf);
                printf("\nEmail: %s\n", (*existeEmprestimo).usuario.email);
            }
            else
            {
                printf("\nEmprestimo com ID %d nao encontrado.\n", id);
            }
            break;
        }
        case 2:
        {
            char titulo[MAX_TITULO];
            printf("Digite o titulo do livro a ser consultado: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            buscarEmprestimoTitulo(titulo);
            break;
        }
        case 3:
        {
            char cpf[MAX_CPF];
            printf("Digite o CPF do usuario a ser consultado: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = '\0';
            buscarEmprestimoCpf(cpf);
            break;
        }
        default:

            printf("\nOpcao invalida.\n");
        }
    }
}

void removerEmprestimo()
{
    if(totalEmprestimo >= 1)
    {
        int id;
        printf("\nDigite o ID do emprestimo a ser removido: ");
        scanf("%d", &id);
        limpaBuffer();

        int indexDelete = 0;
        int existeId = 0;
        for(int i = 0; i < totalEmprestimo; i++)
        {
            if(emprestimo[i].idEmprestimo == id)
            {
                indexDelete = i;
                existeId = 1;
            }
        }

        if(existeId == 0)
        {
            printf("\nErro: Emprestimo com ID: %d nao encontrado.\n", id);
            return;
        }

        Livro* atualizaLivro = buscaLivroTitulo(emprestimo[indexDelete].livro.titulo);
        if(atualizaLivro != NULL)
        {
            (*atualizaLivro).disponivel = 1;
        }
        emprestimo[indexDelete] = emprestimo[totalEmprestimo - 1];
        totalEmprestimo--;
        printf("\nEmprestimo com ID %d removido com sucesso!\n", id);
    }
    else
    {
        printf("\nNenhum emprestimo cadastrado para remover.\n\n");
    }
}

void gerarRelatorioEmprestimo()
{
    if(totalEmprestimo >= 1)
    {
        int opcaoRelatorio;
        FILE *arquivoRelatorio = NULL;
        char filtro[MAX_TITULO];
        Livro* livroRelatorio = NULL;
        Usuario* usuarioRelatorio = NULL;

        printf("\n=== GERAR RELATORIO DE EMPRESTIMOS ===\n");
        printf("Escolha o tipo de relatorio:\n");
        printf("1. Emprestimos ativos.\n");
        printf("2. Emprestimos por usuario (CPF).\n");
        printf("3. Emprestimos por livro (titulo).\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcaoRelatorio);
        limpaBuffer();

        switch(opcaoRelatorio)
        {
        case 1:
            if(totalEmprestimo >= 1)
            {
                int emprestimoAtivo = 0;
                for(int i = 0; i < totalEmprestimo; i++)
                {
                    if(emprestimo[i].status == STATUS_EMPRESTADO)
                    {
                        emprestimoAtivo = 1;
                    }
                }

                if(emprestimoAtivo)
                {
                    arquivoRelatorio = fopen("relatorioAtivos.txt", "w");
                    if(arquivoRelatorio == NULL)
                    {
                        printf("Erro ao criar o arquivo de relatorio.\n");
                    }
                    fprintf(arquivoRelatorio, "RELATORIO DE EMPRESTIMOS ATIVOS\n");
                    fprintf(arquivoRelatorio, "=================================\n\n");
                    for(int i = 0; i < totalEmprestimo; i++)
                    {
                        if(emprestimo[i].status == STATUS_EMPRESTADO)
                        {
                            fprintf(arquivoRelatorio, "ID: %d\n", emprestimo[i].idEmprestimo);
                            fprintf(arquivoRelatorio, "  Livro: %s\n", emprestimo[i].livro.titulo);
                            fprintf(arquivoRelatorio, "  Autor: %s\n", emprestimo[i].livro.autor);
                            fprintf(arquivoRelatorio, "  Usuario: %s\n", emprestimo[i].usuario.nome);
                            fprintf(arquivoRelatorio, "  CPF: %s\n", emprestimo[i].usuario.cpf);
                            fprintf(arquivoRelatorio, "  Data de Retirada: %s\n", emprestimo[i].dataRetirada);
                            fprintf(arquivoRelatorio, "---------------------------------\n");
                        }
                    }
                    printf("\nRelatorio de emprestimos ativos gerado com sucesso!\n");
                }
                else
                {
                    printf("\nNao ha emprestimos ativos para gerar o relatorio.\n");
                }
            }
            else
            {
                printf("\nNao ha registros de emprestimos.\n\n");
            }
            break;
        case 2:
            do
            {
                printf("Digite o CPF do usuario para o relatorio: ");
                fgets(filtro, sizeof(filtro), stdin);
                filtro[strcspn(filtro, "\n")] = '\0';

                usuarioRelatorio = buscaUsuarioCpf(filtro);
                if(usuarioRelatorio == NULL)
                {
                    printf("\nErro: Nao existe usuario com o CPF %s.\n", filtro);
                }
            }
            while(usuarioRelatorio == NULL);

            int emprestimoUsuario = 0;
            for(int i = 0; i < totalEmprestimo; i++)
            {
                if(strcmp(emprestimo[i].usuario.cpf, filtro) == 0)
                {
                    emprestimoUsuario = 1;
                }
            }

            if(emprestimoUsuario)
            {
                arquivoRelatorio = fopen("relatorioUsuario.txt", "w");
                if(arquivoRelatorio == NULL)
                {
                    printf("Erro ao criar o arquivo de relatorio.\n");
                }
                fprintf(arquivoRelatorio, "RELATORIO DE EMPRESTIMOS - USUARIO CPF: %s\n", filtro);
                fprintf(arquivoRelatorio, "=================================================\n\n");
                for(int i = 0; i < totalEmprestimo; i++)
                {
                    if (strcmp(emprestimo[i].usuario.cpf, filtro) == 0)
                    {
                        fprintf(arquivoRelatorio, "Emprestimo ID: %d\n", emprestimo[i].idEmprestimo);
                        fprintf(arquivoRelatorio, "  Livro: %s\n", emprestimo[i].livro.titulo);
                        fprintf(arquivoRelatorio, "  Autor: %s\n", emprestimo[i].livro.autor);
                        fprintf(arquivoRelatorio, "  Data de Retirada: %s\n", emprestimo[i].dataRetirada);
                        fprintf(arquivoRelatorio, "  Status: ");
                        if (emprestimo[i].status == STATUS_EMPRESTADO)
                        {
                            fprintf(arquivoRelatorio, "Emprestado\n");
                        }
                        else
                        {
                            fprintf(arquivoRelatorio, "Devolvido\n");
                        }
                        fprintf(arquivoRelatorio, "---------------------------------\n");
                    }
                }
                printf("\nRelatorio de emprestimos por usuario gerado com sucesso!\n");
            }
            else
            {
                printf("\nNao ha emprestimos para o usuario com o CPF %s.\n", filtro);
            }
            break;
        case 3:
            do
            {
                printf("Digite o titulo do livro para o relatorio: ");
                fgets(filtro, sizeof(filtro), stdin);
                filtro[strcspn(filtro, "\n")] = '\0';

                livroRelatorio = buscaLivroTitulo(filtro);
                if(livroRelatorio == NULL)
                {
                    printf("\nErro: Nao existe livro com o titulo %s.\n", filtro);
                }
            }
            while(livroRelatorio == NULL);

            int emprestimoLivro = 0;
            for(int i = 0; i < totalEmprestimo; i++)
            {
                if(strcmp(emprestimo[i].livro.titulo, filtro) == 0)
                {
                    emprestimoLivro = 1;
                }
            }

            if(emprestimoLivro)
            {
                arquivoRelatorio = fopen("relatorioLivro.txt", "w");
                if(arquivoRelatorio == NULL)
                {
                    printf("Erro ao criar o arquivo de relatorio.\n");
                }

                fprintf(arquivoRelatorio, "RELATORIO DE EMPRESTIMOS PARA O LIVRO: %s\n", filtro);
                fprintf(arquivoRelatorio, "========================================================\n\n");

                for (int i = 0; i < totalEmprestimo; i++)
                {
                    if (strcmp(emprestimo[i].livro.titulo, filtro) == 0)
                    {
                        fprintf(arquivoRelatorio, "Emprestimo ID: %d\n", emprestimo[i].idEmprestimo);
                        fprintf(arquivoRelatorio, "  Usuario: %s\n", emprestimo[i].usuario.nome);
                        fprintf(arquivoRelatorio, "  CPF: %s\n", emprestimo[i].usuario.cpf);
                        fprintf(arquivoRelatorio, "  Data de Retirada: %s\n", emprestimo[i].dataRetirada);
                        fprintf(arquivoRelatorio, "  Status: ");
                        if (emprestimo[i].status == STATUS_EMPRESTADO)
                        {
                            fprintf(arquivoRelatorio, "Emprestado\n");
                        }
                        else
                        {
                            fprintf(arquivoRelatorio, "Devolvido\n");
                        }
                        fprintf(arquivoRelatorio, "---------------------------------\n");
                    }
                }
                printf("\nRelatorio de emprestimos por livro gerado com sucesso!\n");
            }
            else
            {
                printf("\nNao ha emprestimos para o livro com titulo %s.\n", filtro);
            }
            break;

        default :
            printf("\nOpcao invalida.\n");
            return;
        }

        if(arquivoRelatorio != NULL)
        {
            fclose(arquivoRelatorio);
        }
    }
    else
    {
        printf("\nNenhum emprestimo cadastrado para gerar relatorio.\n");
    }
}
