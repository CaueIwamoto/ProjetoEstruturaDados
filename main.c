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

// =================== DECLARAÇÃO DE FUNÇÕES ===================== //
Paciente* buscarPorRG(const char* rg);

// =================== ESTRUTURA DE FILA ===================== //
typedef struct NoFila {
    Paciente* paciente;
    struct NoFila* proximo;
} NoFila;

typedef struct {
    NoFila* head;
    NoFila* tail;
} Fila;

Fila filaAtendimento = {NULL, NULL};

// =================== ESTRUTURA DE HEAP ===================== //
#define MAX_HEAP 20
Paciente* heap[MAX_HEAP];
int tamanhoHeap = 0;

void trocar(Paciente** a, Paciente** b) {
    Paciente* temp = *a;
    *a = *b;
    *b = temp;
}

void subirHeap(int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        if (heap[idx]->idade > heap[pai]->idade) {
            trocar(&heap[idx], &heap[pai]);
            idx = pai;
        } else break;
    }
}

void descerHeap(int idx) {
    int maior = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < tamanhoHeap && heap[esq]->idade > heap[maior]->idade)
        maior = esq;
    if (dir < tamanhoHeap && heap[dir]->idade > heap[maior]->idade)
        maior = dir;

    if (maior != idx) {
        trocar(&heap[idx], &heap[maior]);
        descerHeap(maior);
    }
}

void inserirPrioridade() {
    if (tamanhoHeap >= MAX_HEAP) {
        printf("Fila de prioridade cheia.\n");
        return;
    }

    char rg[15];
    printf("Informe o RG do paciente prioritario: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    if (!p) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    heap[tamanhoHeap] = p;
    subirHeap(tamanhoHeap);
    tamanhoHeap++;
    printf("Paciente enfileirado com prioridade.\n");
}

void removerPrioridade() {
    if (tamanhoHeap == 0) {
        printf("Fila de prioridade vazia.\n");
        return;
    }

    Paciente* p = heap[0];
    printf("Paciente %s removido da fila prioritária.\n", p->nome);
    heap[0] = heap[tamanhoHeap - 1];
    tamanhoHeap--;
    descerHeap(0);
}

void mostrarHeap() {
    if (tamanhoHeap == 0) {
        printf("Fila prioritária vazia.\n");
        return;
    }

    for (int i = 0; i < tamanhoHeap; i++) {
        printf("Nome: %s | Idade: %d | RG: %s\n", heap[i]->nome, heap[i]->idade, heap[i]->rg);
    }
}

void menuPrioritario() {
    int opcao;
    do {
        printf("\n--- Menu Atendimento Prioritário ---\n");
        printf("1. Enfileirar paciente prioritário\n");
        printf("2. Desenfileirar paciente prioritário\n");
        printf("3. Mostrar fila prioritária\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirPrioridade(); break;
            case 2: removerPrioridade(); break;
            case 3: mostrarHeap(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

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

void enfileirar(Fila* f, Paciente* paciente) {
    NoFila* novo = (NoFila*) malloc(sizeof(NoFila));
    novo->paciente = paciente;
    novo->proximo = NULL;

    if (f->tail != NULL)
        f->tail->proximo = novo;
    else
        f->head = novo;

    f->tail = novo;
}

void adicionarNaFila() {
    char rg[15];
    printf("Informe o RG do paciente a ser enfileirado: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    enfileirar(&filaAtendimento, p);
    printf("Paciente enfileirado para atendimento.\n");
}

void desenfileirar(Fila* f) {
    if (f->head == NULL) {
        printf("Fila vazia.\n");
        return;
    }

    NoFila* temp = f->head;
    f->head = f->head->proximo;

    if (f->head == NULL)
        f->tail = NULL;

    printf("Paciente %s desenfileirado.\n", temp->paciente->nome);
    free(temp);
}

void mostrarFila(Fila* f) {
    NoFila* atual = f->head;
    if (atual == NULL) {
        printf("Fila vazia.\n");
        return;
    }

    while (atual != NULL) {
        Paciente* p = atual->paciente;
        printf("Nome: %s | Idade: %d | RG: %s\n", p->nome, p->idade, p->rg);
        atual = atual->proximo;
    }
}

void menuFila() {
    int opcao;
    do {
        printf("\n--- Menu de Atendimento (Fila) ---\n");
        printf("1. Enfileirar paciente\n");
        printf("2. Desenfileirar paciente\n");
        printf("3. Mostrar fila\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: adicionarNaFila(); break;
            case 2: desenfileirar(&filaAtendimento); break;
            case 3: mostrarFila(&filaAtendimento); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

// =================== FUNÇÃO PRINCIPAL ===================== //
int main() {
    int opcao;
    do {
        printf("\n=== Sistema de Atendimento Médico ===\n");
        printf("1. Cadastro de pacientes\n");
        printf("2. Atendimento (fila comum)\n");
        printf("3. Atendimento prioritario (heap)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: menuCadastro(); break;
            case 2: menuFila(); break;
            case 3: menuPrioritario(); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
