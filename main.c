// =================== BIBLIOTECAS ===================== //
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

// =================== FUNÇÕES DE LISTA (CADASTRO) ===================== //
Paciente* criarPaciente() {
    Paciente* novo = (Paciente*) malloc(sizeof(Paciente));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
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

Paciente* buscarPorRG(const char* rg) {
    Paciente* atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->rg, rg) == 0)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

void consultarPaciente() {
    char rg[15];
    printf("Informe o RG do paciente: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    if (p) {
        printf("\nNome: %s\nIdade: %d\nRG: %s\nData de entrada: %02d/%02d/%04d\n",
               p->nome, p->idade, p->rg, p->entrada.dia, p->entrada.mes, p->entrada.ano);
    } else {
        printf("Paciente nao encontrado.\n");
    }
}

void atualizarPaciente() {
    char rg[15];
    printf("Informe o RG do paciente a atualizar: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    if (p) {
        printf("Novo nome: ");
        fgets(p->nome, 100, stdin);
        strtok(p->nome, "\n");

        printf("Nova idade: ");
        scanf("%d", &p->idade);
        getchar();

        printf("Nova data de entrada (dd mm aaaa): ");
        scanf("%d %d %d", &p->entrada.dia, &p->entrada.mes, &p->entrada.ano);
        getchar();

        printf("Paciente atualizado com sucesso.\n");
    } else {
        printf("Paciente nao encontrado.\n");
    }
}

void removerPaciente() {
    char rg[15];
    printf("Informe o RG do paciente a remover: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente *atual = lista, *anterior = NULL;
    while (atual != NULL && strcmp(atual->rg, rg) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("Paciente removido com sucesso.\n");
}

void listarPacientes() {
    Paciente* atual = lista;
    if (atual == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    while (atual != NULL) {
        printf("\nNome: %s\nIdade: %d\nRG: %s\nData de entrada: %02d/%02d/%04d\n",
               atual->nome, atual->idade, atual->rg,
               atual->entrada.dia, atual->entrada.mes, atual->entrada.ano);
        atual = atual->proximo;
    }
}

void menuCadastro() {
    int opcao;
    do {
        printf("\n--- Menu de Cadastro ---\n");
        printf("1. Cadastrar novo paciente\n");
        printf("2. Consultar paciente pelo RG\n");
        printf("3. Atualizar dados de paciente\n");
        printf("4. Remover paciente\n");
        printf("5. Mostrar lista de pacientes\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirPaciente(); break;
            case 2: consultarPaciente(); break;
            case 3: atualizarPaciente(); break;
            case 4: removerPaciente(); break;
            case 5: listarPacientes(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// =================== FUNÇÃO PRINCIPAL ===================== //
int main() {
    int opcao;
    do {
        printf("\n=== Sistema de Atendimento Médico ===\n");
        printf("1. Cadastro de pacientes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: menuCadastro(); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
