#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== STRUCTS ===== */

typedef struct Chamada {
    int protocolo;
    char local[50];
    char tipo[30];
    char horario[20];
} Chamada;

typedef struct No {
    Chamada dados;
    struct No *proximo;
} No;

typedef struct {
    No *topo;
    int tamanho;
} Pilha;

/* ===== INICIALIZAR ===== */

void inicializar(Pilha *p) {
    p->topo = NULL;
    p->tamanho = 0;
}

/* ===== PILHA VAZIA ===== */

int pilhaVazia(Pilha *p) {
    return p->topo == NULL;
}

/* ===== PUSH ===== */

int push(Pilha *p, Chamada c) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return 0;
    }
    novo->dados = c;
    novo->proximo = p->topo;
    p->topo = novo;
    p->tamanho++;
    return 1;
}

/* ===== POP ===== */

int pop(Pilha *p, Chamada *saida) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nenhuma chamada para atender.\n");
        return 0;
    }
    No *remover = p->topo;
    *saida = remover->dados;
    p->topo = remover->proximo;
    free(remover);
    p->tamanho--;
    return 1;
}

/* ===== PEEK ===== */

int peek(Pilha *p, Chamada *saida) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nenhuma chamada registrada.\n");
        return 0;
    }
    *saida = p->topo->dados;
    return 1;
}

/* ===== LISTAR ===== */

void listar(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhuma chamada registrada.\n");
        return;
    }
    No *atual = p->topo;
    int pos = 1;
    printf("\n%-4s %-10s %-30s %-20s %-10s\n", "#", "Protocolo", "Local", "Tipo", "Horario");
    printf("----------------------------------------------------------------------\n");
    while (atual != NULL) {
        printf("%-4d %-10d %-30s %-20s %-10s\n",
               pos,
               atual->dados.protocolo,
               atual->dados.local,
               atual->dados.tipo,
               atual->dados.horario);
        atual = atual->proximo;
        pos++;
    }
    printf("----------------------------------------------------------------------\n");
    printf("Total: %d chamada(s)\n", p->tamanho);
}

/* ===== VERIFICAR PROTOCOLO DUPLICADO ===== */

int protocoloExiste(Pilha *p, int protocolo) {
    No *atual = p->topo;
    while (atual != NULL) {
        if (atual->dados.protocolo == protocolo)
            return 1;
        atual = atual->proximo;
    }
    return 0;
}

/* ===== SALVAR CSV ===== */

void salvarCSV(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nada para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("chamadas.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(arquivo, "protocolo;local;tipo;horario\n");

    /* Precisamos inverter a ordem para salvar da base ao topo,
       pois o CSV sera carregado com push, recompondo a pilha na ordem correta */
    int total = p->tamanho;
    No **vetor = (No **)malloc(total * sizeof(No *));
    if (vetor == NULL) {
        printf("Erro de memoria ao salvar.\n");
        fclose(arquivo);
        return;
    }

    No *atual = p->topo;
    for (int i = 0; i < total; i++) {
        vetor[i] = atual;
        atual = atual->proximo;
    }

    /* Escreve do ultimo para o primeiro (base -> topo) */
    for (int i = total - 1; i >= 0; i--) {
        fprintf(arquivo, "%d;%s;%s;%s\n",
                vetor[i]->dados.protocolo,
                vetor[i]->dados.local,
                vetor[i]->dados.tipo,
                vetor[i]->dados.horario);
    }

    free(vetor);
    fclose(arquivo);
    printf("Dados salvos em chamadas.csv com sucesso.\n");
}

/* ===== CARREGAR CSV ===== */

void carregarCSV(Pilha *p) {
    FILE *arquivo = fopen("chamadas.csv", "r");
    if (arquivo == NULL) {
        printf("Arquivo chamadas.csv nao encontrado. Iniciando com pilha vazia.\n");
        return;
    }

    char linha[150];
    fgets(linha, sizeof(linha), arquivo); /* pula cabecalho */

    int carregados = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';

        Chamada c;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        c.protocolo = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.local, token, sizeof(c.local) - 1);
        c.local[sizeof(c.local) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.tipo, token, sizeof(c.tipo) - 1);
        c.tipo[sizeof(c.tipo) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.horario, token, sizeof(c.horario) - 1);
        c.horario[sizeof(c.horario) - 1] = '\0';

        push(p, c);
        carregados++;
    }

    fclose(arquivo);

    if (carregados > 0)
        printf("%d chamada(s) carregada(s) do arquivo.\n", carregados);
    else
        printf("Arquivo vazio ou sem dados validos.\n");
}

/* ===== LIBERAR MEMORIA ===== */

void liberarPilha(Pilha *p) {
    No *atual = p->topo;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    p->topo = NULL;
    p->tamanho = 0;
}

/* ===== LER STRING SEGURA ===== */

void lerString(char *destino, int tamanho, const char *mensagem) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

/* ===== MENU ===== */

void exibirMenu() {
    printf("\n========================================\n");
    printf("   SISTEMA DE CHAMADAS DE EMERGENCIA   \n");
    printf("========================================\n");
    printf("  1. Registrar chamada (Push)\n");
    printf("  2. Atender chamada   (Pop)\n");
    printf("  3. Consultar topo    (Peek)\n");
    printf("  4. Listar todas\n");
    printf("  5. Salvar CSV\n");
    printf("  6. Carregar CSV\n");
    printf("  0. Sair\n");
    printf("========================================\n");
    printf("Opcao: ");
}

/* ===== MAIN ===== */

int main() {
    Pilha pilha;
    inicializar(&pilha);

    /* Carrega automaticamente ao iniciar */
    carregarCSV(&pilha);

    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar(); /* limpa buffer */

        switch (opcao) {

            case 1: {
                Chamada nova;
                printf("\n--- Registrar Chamada ---\n");

                printf("Protocolo: ");
                scanf("%d", &nova.protocolo);
                getchar();

                if (nova.protocolo <= 0) {
                    printf("Protocolo invalido. Deve ser um numero positivo.\n");
                    break;
                }

                if (protocoloExiste(&pilha, nova.protocolo)) {
                    printf("Erro: protocolo %d ja existe na pilha.\n", nova.protocolo);
                    break;
                }

                lerString(nova.local, sizeof(nova.local), "Local: ");
                if (strlen(nova.local) == 0) {
                    printf("Local nao pode ser vazio.\n");
                    break;
                }

                lerString(nova.tipo, sizeof(nova.tipo), "Tipo da ocorrencia: ");
                if (strlen(nova.tipo) == 0) {
                    printf("Tipo nao pode ser vazio.\n");
                    break;
                }

                lerString(nova.horario, sizeof(nova.horario), "Horario (HH:MM): ");
                if (strlen(nova.horario) == 0) {
                    printf("Horario nao pode ser vazio.\n");
                    break;
                }

                if (push(&pilha, nova))
                    printf("Chamada %d registrada com sucesso.\n", nova.protocolo);
                break;
            }

            case 2: {
                Chamada atendida;
                printf("\n--- Atender Chamada (Pop) ---\n");
                if (pop(&pilha, &atendida)) {
                    printf("Chamada atendida:\n");
                    printf("  Protocolo : %d\n", atendida.protocolo);
                    printf("  Local     : %s\n", atendida.local);
                    printf("  Tipo      : %s\n", atendida.tipo);
                    printf("  Horario   : %s\n", atendida.horario);
                }
                break;
            }

            case 3: {
                Chamada topo;
                printf("\n--- Consultar Topo (Peek) ---\n");
                if (peek(&pilha, &topo)) {
                    printf("Proxima chamada a atender:\n");
                    printf("  Protocolo : %d\n", topo.protocolo);
                    printf("  Local     : %s\n", topo.local);
                    printf("  Tipo      : %s\n", topo.tipo);
                    printf("  Horario   : %s\n", topo.horario);
                }
                break;
            }

            case 4: {
                printf("\n--- Lista de Chamadas (Topo -> Base) ---\n");
                listar(&pilha);
                break;
            }

            case 5: {
                salvarCSV(&pilha);
                break;
            }

            case 6: {
                liberarPilha(&pilha);
                inicializar(&pilha);
                carregarCSV(&pilha);
                break;
            }

            case 0: {
                salvarCSV(&pilha);
                liberarPilha(&pilha);
                printf("Saindo... Ate logo!\n");
                break;
            }

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}