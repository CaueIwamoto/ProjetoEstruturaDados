//Bibliotecas:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //Usando para registrar data e horário atual nos arquivos txt


//Estrutura dia, mês e ano na parte do cadastro:
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

//Estrutura nome, idade, RG e data de nascimento do paciente:
typedef struct Paciente {
    char nome[100];
    int idade;
    char rg[15];
    Data entrada;
    struct Paciente* proximo;
} Paciente;

Paciente* lista = NULL;

//Busca RG paciente:
Paciente* buscarPorRG(const char* rg);

//Estrutura fila comum:
typedef struct NoFila {
    Paciente* paciente;
    struct NoFila* proximo;
} NoFila;

typedef struct {
    NoFila* head;
    NoFila* tail;
} Fila;

Fila filaAtendimento = {NULL, NULL};


//Estrutura fila prioritária (HEAP):
#define MAX_HEAP 20
Paciente* heap[MAX_HEAP];
int tamanhoHeap = 0;

//Função trocar:
void trocar(Paciente** a, Paciente** b) {
    Paciente* temp = *a;
    *a = *b;
    *b = temp;
}

//Função subirHeap:
void subirHeap(int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        if (heap[idx]->idade > heap[pai]->idade) {
            trocar(&heap[idx], &heap[pai]);
            idx = pai;
        } else break;
    }
}

//Função descerHeap:
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

//ESTRUTURA DE DESFAZER:
typedef struct Operacao {
    char tipo[20];          
    char rg[15];// RG do paciente
    struct Operacao* prox;
} Operacao;

Operacao* topoPilha = NULL;

//Função registrar operação:
void registrarOperacao(const char* tipo, const char* rg) {
    Operacao* nova = (Operacao*) malloc(sizeof(Operacao));
    strcpy(nova->tipo, tipo);
    strcpy(nova->rg, rg);
    nova->prox = topoPilha;
    topoPilha = nova;
}



//Função desfazer última operação feita:
void desfazerUltimaOperacao() {
    if (topoPilha == NULL) {
        printf("Nenhuma operacao para desfazer.\n");
        return;
    }

    Operacao* op = topoPilha;
    printf("Deseja desfazer a operacao: %s %s? (s/n): ", op->tipo, op->rg);
    char resp;
    scanf(" %c", &resp);
    getchar();

    if (resp == 's' || resp == 'S') {
        if (strcmp(op->tipo, "ENFILEIRAR") == 0) {
            // Remove o paciente do início da fila
            desenfileirar(&filaAtendimento);
            printf("Operacao desfeita com sucesso.\n");
        }
    }

    topoPilha = topoPilha->prox;
    free(op);
}


//Função mostrar histórico das operações:
void mostrarOperacoes() {
    Operacao* atual = topoPilha;
    if (!atual) {
        printf("Nenhuma operacao registrada.\n");
        return;
    }

    printf("--- Historico de Operacoes ---\n");
    while (atual != NULL) {
        printf("Operacao: %s | RG: %s\n", atual->tipo, atual->rg);
        atual = atual->prox;
    }
}


//Função para salvar em arquivo txt os dados do paciente:
void salvarPacientes() {
    time_t agora = time(NULL);
    struct tm* tm_info = localtime(&agora);

    char nomeArquivo[100];
    strftime(nomeArquivo, sizeof(nomeArquivo), "pacientes_%Y%m%d_%H%M%S.txt", tm_info);

    FILE* f = fopen(nomeArquivo, "w");
    if (!f) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    Paciente* atual = lista;
    while (atual) {
        fprintf(f, "%s;%d;%s;%d;%d;%d\n", atual->nome, atual->idade, atual->rg,
                atual->entrada.dia, atual->entrada.mes, atual->entrada.ano);
        atual = atual->proximo;
    }

    fclose(f);
    printf("Pacientes salvos no arquivo: %s\n", nomeArquivo);
}

//Função para carregar na memória os dados paciente pelo arquivo txt criado:
void carregarPacientes() {
    char nomeArquivo[100];
    printf("Digite o nome do arquivo a ser carregado no sistema: ");
    fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
    //Remove o \n do final:
    strtok(nomeArquivo, "\n"); 

    FILE* f = fopen(nomeArquivo, "r");
    if (!f) {
        printf("Arquivo \"%s\" não encontrado.\n", nomeArquivo);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        Paciente* novo = (Paciente*) malloc(sizeof(Paciente));
        sscanf(linha, "%[^;];%d;%[^;];%d;%d;%d", novo->nome, &novo->idade, novo->rg,
               &novo->entrada.dia, &novo->entrada.mes, &novo->entrada.ano);
        novo->proximo = lista;
        lista = novo;
    }

    fclose(f);
    printf("Pacientes carregados com sucesso.\n");
}




//Último item do menu (8):
void mostrarSobre() {
    printf("\n--- Sobre o Projeto ---\n");
    printf("Nome do aluno: Cauê Iwamoto Meira\n");
    printf("Ciclo: Quarto ciclo\n");
    printf("Curso: Ciência da Computação\n");
    printf("Disciplina: Estrutura de Dados\n");
    printf("Data: 19/05/2025\n");
}

//Função prioridade aviso limite:
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

//Função aviso se fila estiver vazia:
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

//Função fila prioritária, se ela estiver fazia:
void mostrarHeap() {
    if (tamanhoHeap == 0) {
        printf("Fila prioritária vazia.\n");
        return;
    }

    for (int i = 0; i < tamanhoHeap; i++) {
        printf("Nome: %s | Idade: %d | RG: %s\n", heap[i]->nome, heap[i]->idade, heap[i]->rg);
    }
}

//Função que exibe o menu do atendimento prioritário:
void menuPrioritario() {
    int opcao;
    do {
        printf("\n--- Menu Atendimento Prioritário ---\n");
        printf("1. Enfileirar paciente prioritário\n");
        printf("2. Desenfileirar paciente prioritário\n");
        printf("3. Mostrar fila prioritária\n");
        printf("0. Voltar\n");
        printf("\n");
        printf("Digite uma das opções do menu: ");
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

//Estrutura de parte da pesquisa. Árvore não binária de busca:
typedef struct NodoABB {
    Paciente* dados;
    int chave;
    struct NodoABB* esq;
    struct NodoABB* dir;
} NodoABB;

NodoABB* raizAno = NULL;
NodoABB* raizMes = NULL;
NodoABB* raizDia = NULL;
NodoABB* raizIdade = NULL;


//Função genérica para inserção de chave (faz parte para organização em mostrar os registros por ano, idade, mês e dia):
NodoABB* inserirPorChave(NodoABB* raiz, Paciente* p, int chave) {
    if (raiz == NULL) {
        NodoABB* novo = (NodoABB*) malloc(sizeof(NodoABB));
        novo->dados = p;
        novo -> chave = chave;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (chave < chaveDoPaciente(raiz)) //Para pegar corretamente o mês, ano, dia e idade do paciente
        raiz->esq = inserirPorChave(raiz->esq, p, chave);
    else
        raiz->dir = inserirPorChave(raiz->dir, p, chave);

    return raiz;
}

//Função que extrai a chave corretamente do paciente (ano, idade, mês e dia):
int chaveDoPaciente(NodoABB* nodo) {
    return nodo->chave;
}

//Quarto item, exibição em ordem:
void exibirEmOrdem(NodoABB* raiz) {
    if (raiz == NULL) return;
    exibirEmOrdem(raiz->esq);
    Paciente* p = raiz->dados;
    printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%04d\n",
           p->nome, p->idade, p->rg,
           p->entrada.dia, p->entrada.mes, p->entrada.ano);
    exibirEmOrdem(raiz->dir);
}



//Primeira parte do item 1., registrar paciente:
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

    printf("Data de nascimento (DD MM AAAA): ");
    scanf("%d %d %d", &novo->entrada.dia, &novo->entrada.mes, &novo->entrada.ano);
    getchar();

    novo->proximo = NULL;

    return novo;
}

//Ao finalizar cadastro, paciente cadastrado com sucesso final item 1-1.:
void inserirPaciente() {
    Paciente* novo = criarPaciente();
    if (novo != NULL) {
        novo->proximo = lista;
        lista = novo;
        printf("Paciente cadastrado com sucesso!\n");
    }
}

//Busca paciente por RG:
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
        printf("\nNome: %s\nIdade: %d\nRG: %s\nData de nascimento: %02d/%02d/%04d\n",
               p->nome, p->idade, p->rg, p->entrada.dia, p->entrada.mes, p->entrada.ano);
    } else {
        printf("Paciente nao encontrado.\n");
    }
}

//Função para atualizar os dados de um paciente através de seu RG:
void atualizarPaciente() {
    char rg[15];
    printf("Informe o RG do paciente a ser atualizado do sistema: ");
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

        printf("Nova data de nascimento (DD MM AAAA): ");
        scanf("%d %d %d", &p->entrada.dia, &p->entrada.mes, &p->entrada.ano);
        getchar();

        printf("Paciente atualizado com sucesso.\n");
    } else {
        //Se o número do RG não coincidir:
        printf("Paciente nao encontrado.\n");
    }
}

//Função para remover paciente através de seu número de RG:
void removerPaciente() {
    char rg[15];
    printf("Informe o RG do paciente a ser removido do sistema: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente *atual = lista, *anterior = NULL;
    while (atual != NULL && strcmp(atual->rg, rg) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    //Se o número do RG não coincidir:
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

//Função exibe lista dos pacientes:
void listarPacientes() {
    Paciente* atual = lista;
    if (atual == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    while (atual != NULL) {
        printf("\nNome: %s\nIdade: %d\nRG: %s\nData de nascimento: %02d/%02d/%04d\n",
               atual->nome, atual->idade, atual->rg,
               atual->entrada.dia, atual->entrada.mes, atual->entrada.ano);
        atual = atual->proximo;
    }
}

//Função do menu de cadastro:
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
        printf("\n");
        printf("Digite uma das opções do menu: ");
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

//Função enfileirar:
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

//Função para adicionar na fila comum, de acordo com o RG do paciente:
void adicionarNaFila() {
    char rg[15];
    printf("Informe o RG do paciente a ser enfileirado: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    //Caso o RG não bata:
    if (p == NULL) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    enfileirar(&filaAtendimento, p);
    printf("Paciente enfileirado para atendimento.\n");
    
    registrarOperacao("ENFILEIRAR", rg);
}

//Função para remover paciente da fila:
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

//Função para exibir fila atual:
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

//Função do menu da fila:
void menuFila() {
    int opcao;
    do {
        printf("\n--- Menu de Atendimento (Fila) ---\n");
        printf("1. Enfileirar paciente\n");
        printf("2. Desenfileirar paciente\n");
        printf("3. Mostrar fila\n");
        printf("0. Voltar ao menu principal\n");
        printf("\n");
        printf("Digite uma das opções do menu: ");
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

//Função inserir árvore:
void inserirNaArvore() {
    char rg[15];
    printf("Informe o RG do paciente: ");
    fgets(rg, 15, stdin);
    strtok(rg, "\n");

    Paciente* p = buscarPorRG(rg);
    //Caso não encontre o RG:
    if (!p) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    raizAno   = inserirPorChave(raizAno, p, p->entrada.ano);
    raizMes   = inserirPorChave(raizMes, p, p->entrada.mes);
    raizDia   = inserirPorChave(raizDia, p, p->entrada.dia);
    raizIdade = inserirPorChave(raizIdade, p, p->idade);

    printf("Paciente inserido nas arvores de pesquisa.\n");
}

//Menu do item 4:
void menuPesquisa() {
    int opcao;
    do {
        printf("\n--- Menu de Pesquisa (Árvore Binária) ---\n");
        printf("1. Inserir paciente na árvore\n");
        printf("2. Mostrar por ano de entrada\n");
        printf("3. Mostrar por mês de entrada\n");
        printf("4. Mostrar por dia de entrada\n");
        printf("5. Mostrar por idade\n");
        printf("0. Voltar ao menu principal\n");
        printf("\n");
        printf("Digite umas das opções do menu: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirNaArvore(); break;
            case 2: exibirEmOrdem(raizAno); break;
            case 3: exibirEmOrdem(raizMes); break;
            case 4: exibirEmOrdem(raizDia); break;
            case 5: exibirEmOrdem(raizIdade); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}



//main:
int main() {
    int opcao;
    do {
        printf("\n=== Sistema de Gerenciamento de Saúde ===\n");
        printf("1. Cadastro de pacientes\n");
        printf("2. Atendimento (fila comum)\n");
        printf("3. Atendimento prioritario (heap)\n");
        printf("4. Pesquisa (árvore binária)\n");
        printf("5. Desfazer operacoes\n");
        printf("6. Carregar pacientes\n");
        printf("7. Salvar pacientes\n");
        printf("8. Sobre\n");
        printf("0. Sair\n");
        printf("\n");
        printf("Digite uma das opções do menu: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: menuCadastro(); break;
            case 2: menuFila(); break;
            case 3: menuPrioritario(); break;
            case 4: menuPesquisa(); break;
            case 5: {
                int subop;
                printf("\n1. Mostrar operacoes\n2. Desfazer ultima operacao\nEscolha: ");
                scanf("%d", &subop);
                getchar();
            if (subop == 1) mostrarOperacoes();
                else if (subop == 2) desfazerUltimaOperacao();
                    break;
}
            case 6: carregarPacientes(); break;
            case 7: salvarPacientes(); break;
            case 8: mostrarSobre(); break;
            case 0: printf("Encerrando o sistema...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
