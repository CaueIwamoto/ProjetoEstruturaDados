#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =================== ESTRUTURAS ===================== //
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Paciente {
    char nome[100];
    int idade;
    char rg[15];
    Data entrada;
    struct Paciente* proximo;
} Paciente;

Paciente* lista = NULL;

// =================== FUNÇÕES DE CADASTRO ===================== //
Paciente* criarPaciente() {
    Paciente* novo = (Paciente*) malloc(sizeof(Paciente));
    if (novo == NULL) {
        printf("Erro de alocação de memória!\n");
        return NULL;
    }

    printf("Nome: ");
    fgets(novo->nome, 100, stdin);
    strtok(novo->nome, "\n");

    printf("Idade: ");
    scanf("%d", &novo->idade);
    getchar();

    printf("RG: ");
    fgets(novo->rg, 15, stdin);
    strtok(novo->rg, "\n");

    printf("Data de entrada (dd mm aaaa): ");
    scanf("%d %d %d", &novo->entrada.dia, &novo->entrada.mes, &novo->entrada.ano);
    getchar();

    novo->proximo = NULL;

    return novo;
}

void inserirPaciente() {
    Paciente* novo = criarPaciente();
    if (novo != NULL) {
        novo->proximo = lista;
        lista = novo;
        printf("Paciente cadastrado com sucesso!\n");
    }
}

void listarPacientes() {
    Paciente* atual = lista;
    if (atual == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    while (atual != NULL) {
        printf("\nNome: %s\n", atual->nome);
        printf("Idade: %d\n", atual->idade);
        printf("RG: %s\n", atual->rg);
        printf("Data de entrada: %02d/%02d/%04d\n",
               atual->entrada.dia, atual->entrada.mes, atual->entrada.ano);
        atual = atual->proximo;
    }
}

void menuCadastro() {
    int opcao;
    do {
        printf("\n--- Menu de Cadastro ---\n");
        printf("1. Cadastrar novo paciente\n");
        printf("2. Mostrar lista de pacientes\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                inserirPaciente();
                break;
            case 2:
                listarPacientes();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

// =================== MENU GERAL ===================== //
void menuPrincipal() {
    int opcao;
    do {
        printf("\n=== Sistema de Atendimento Médico ===\n");
        printf("1. Cadastro de pacientes\n");
        printf("2. Atendimento (fila comum)\n");
        printf("3. Atendimento prioritário (heap)\n");
        printf("4. Pesquisa (árvore binária)\n");
        printf("5. Desfazer operação (pilha)\n");
        printf("6. Carregar/Salvar dados\n");
        printf("7. Sobre\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                menuCadastro();
                break;
            case 2:
                printf("Função de atendimento comum ainda não implementada.\n");
                break;
            case 3:
                printf("Função de atendimento prioritário ainda não implementada.\n");
                break;
            case 4:
                printf("Função de pesquisa ainda não implementada.\n");
                break;
            case 5:
                printf("Função de desfazer ainda não implementada.\n");
                break;
            case 6:
                printf("Função de carregar/salvar ainda não implementada.\n");
                break;
            case 7:
                printf("\nDesenvolvedor: [Seu nome aqui]\nCiclo: 1º\nCurso: Ciência da Computação\nDisciplina: Estrutura de Dados\nData: 2025\n");
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);
}

// =================== MAIN ===================== //
int main() {
    menuPrincipal();
    return 0;
}
