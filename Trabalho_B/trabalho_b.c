#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/* ===== STRUCT ===== */

typedef struct {
    int protocolo;
    char local[50];
    char tipo[30];
    char horario[20];
} Chamada;

typedef struct {
    Chamada dados[MAX];
    int topo; /* indice do topo; -1 = pilha vazia */
} Pilha;

/* ===== INICIALIZAR ===== */

void inicializar(Pilha *p) {
    p->topo = -1;
}

/* ===== VAZIA / CHEIA ===== */

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX - 1;
}

/* ===== PUSH ===== */

int push(Pilha *p, Chamada c) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia. Capacidade maxima: %d chamadas.\n", MAX);
        return 0;
    }
    p->topo++;
    p->dados[p->topo] = c;
    return 1;
}

/* ===== POP ===== */

int pop(Pilha *p, Chamada *saida) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nenhuma chamada para atender.\n");
        return 0;
    }
    *saida = p->dados[p->topo];
    p->topo--;
    return 1;
}

/* ===== PEEK ===== */

int peek(Pilha *p, Chamada *saida) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nenhuma chamada registrada.\n");
        return 0;
    }
    *saida = p->dados[p->topo];
    return 1;
}

/* ===== LISTAR (topo -> base) ===== */

void listar(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhuma chamada registrada.\n");
        return;
    }
    printf("\n%-4s %-10s %-30s %-20s %-10s\n", "#", "Protocolo", "Local", "Tipo", "Horario");
    printf("----------------------------------------------------------------------\n");
    int pos = 1;
    for (int i = p->topo; i >= 0; i--) {
        printf("%-4d %-10d %-30s %-20s %-10s\n",
               pos,
               p->dados[i].protocolo,
               p->dados[i].local,
               p->dados[i].tipo,
               p->dados[i].horario);
        pos++;
    }
    printf("----------------------------------------------------------------------\n");
    printf("Total: %d chamada(s)\n", p->topo + 1);
}

/* ===== VERIFICAR PROTOCOLO DUPLICADO ===== */

int protocoloExiste(Pilha *p, int protocolo) {
    for (int i = 0; i <= p->topo; i++) {
        if (p->dados[i].protocolo == protocolo)
            return 1;
    }
    return 0;
}

/* ===== SALVAR CSV (base -> topo, para recarregar na ordem certa) ===== */

void salvarCSV(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nada para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("dados_b.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(arquivo, "protocolo;local;tipo;horario\n");
    for (int i = 0; i <= p->topo; i++) {
        fprintf(arquivo, "%d;%s;%s;%s\n",
                p->dados[i].protocolo,
                p->dados[i].local,
                p->dados[i].tipo,
                p->dados[i].horario);
    }

    fclose(arquivo);
    printf("Dados salvos em dados_b.csv com sucesso.\n");
}

/* ===== CARREGAR CSV ===== */

void carregarCSV(Pilha *p) {
    FILE *arquivo = fopen("dados_b.csv", "r");
    if (arquivo == NULL) {
        printf("Arquivo dados_b.csv nao encontrado. Iniciando com pilha vazia.\n");
        return;
    }

    char linha[150];
    fgets(linha, sizeof(linha), arquivo); /* pula cabecalho */

    int carregados = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (pilhaCheia(p)) {
            printf("Pilha atingiu capacidade maxima durante o carregamento.\n");
            break;
        }

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

/* ===== LER STRING ===== */

void lerString(char *destino, int tamanho, const char *mensagem) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

/* ===== MENU ===== */

void exibirMenu() {
    printf("\n========================================\n");
    printf("  CHAMADAS DE EMERGENCIA (VETOR)        \n");
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

    carregarCSV(&pilha);

    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1: {
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia. Capacidade maxima: %d chamadas.\n", MAX);
                    break;
                }

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
                if (strlen(nova.local) == 0) { printf("Local nao pode ser vazio.\n"); break; }

                lerString(nova.tipo, sizeof(nova.tipo), "Tipo da ocorrencia: ");
                if (strlen(nova.tipo) == 0) { printf("Tipo nao pode ser vazio.\n"); break; }

                lerString(nova.horario, sizeof(nova.horario), "Horario (HH:MM): ");
                if (strlen(nova.horario) == 0) { printf("Horario nao pode ser vazio.\n"); break; }

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
                Chamada topoChamada;
                printf("\n--- Consultar Topo (Peek) ---\n");
                if (peek(&pilha, &topoChamada)) {
                    printf("Proxima chamada a atender:\n");
                    printf("  Protocolo : %d\n", topoChamada.protocolo);
                    printf("  Local     : %s\n", topoChamada.local);
                    printf("  Tipo      : %s\n", topoChamada.tipo);
                    printf("  Horario   : %s\n", topoChamada.horario);
                }
                break;
            }

            case 4: {
                printf("\n--- Lista de Chamadas (Topo -> Base) ---\n");
                listar(&pilha);
                break;
            }

            case 5:
                salvarCSV(&pilha);
                break;

            case 6:
                inicializar(&pilha);
                carregarCSV(&pilha);
                break;

            case 0:
                salvarCSV(&pilha);
                printf("Saindo... Ate logo!\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
