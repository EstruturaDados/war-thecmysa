// ============================================================================
//                   PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//                            Camilly Almeida :)
// ============================================================================

// Bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -----------------------------------------
// Constantes globais
// -----------------------------------------
#define MAX_TERRITORIOS 5
#define MAX_STRING 30
#define TOTAL_MISSOES 2

// -----------------------------------------
// Estrutura principal do território
// -----------------------------------------
typedef struct {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int tropas;
} Territorio;

// -----------------------------------------
// Protótipos das funções
// -----------------------------------------

// Setup / memória
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Interface
void exibirMapa(const Territorio* mapa);
void exibirMenuPrincipal();
void exibirMissao(int missao);

// Lógica principal
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int atacante, int defensor, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, const char* corJogador, int missao);

// Util
void limparBufferEntrada();

// ============================================================================
// Função Principal
// ============================================================================
int main() {
    srand(time(NULL));   // sorteio dos dados e da missão

    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    const char corJogador[] = "Vermelho";
    int missao = sortearMissao();
    int opcao;

    do {
        system("clear || cls");
        exibirMapa(mapa);
        exibirMissao(missao);

        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;

            case 2:
                if (verificarVitoria(mapa, corJogador, missao)) {
                    printf("\n🎉 Missao concluida! Voce venceu!\n");
                    opcao = 0;
                } else {
                    printf("\nA missao ainda nao foi completada.\n");
                }
                break;

            case 0:
                printf("\nEncerrando jogo...\n");
                break;

            default:
                printf("\nOpcao invalida.\n");
        }

        printf("\nPressione ENTER para continuar...");
        limparBufferEntrada();
        getchar();

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// ============================================================================
// Implementação das Funções
// ============================================================================

// Aloca o vetor de territórios
Territorio* alocarMapa() {
    return (Territorio*) calloc(MAX_TERRITORIOS, sizeof(Territorio));
}

// Preenche com dados básicos
void inicializarTerritorios(Territorio* mapa) {
    char nomes[5][MAX_STRING] = {
        "Alaska", "Groenlandia", "Brasil", "Argentina", "Egito"
    };

    char cores[5][MAX_STRING] = {
        "Azul", "Verde", "Vermelho", "Amarelo", "Preto"
    };

    int tropasIniciais[5] = {5, 3, 4, 2, 6};

    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = tropasIniciais[i];
    }
}

// Libera memória alocada
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Exibe o mapa atual
void exibirMapa(const Territorio* mapa) {
    printf("=========================================\n");
    printf("           ESTADO ATUAL DO MAPA\n");
    printf("=========================================\n");

    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("[%d] %-12s | Dono: %-10s | Tropas: %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("=========================================\n\n");
}

// Menu principal
void exibirMenuPrincipal() {
    printf("\n--- MENU ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n\n");
}

// Mostra a missão atual
void exibirMissao(int missao) {
    printf("\n--- MISSAO ATUAL ---\n");
    if (missao == 0)
        printf("Destruir o exercito Verde.\n");
    else
        printf("Conquistar 3 territorios.\n");
}

// Menu de ataque
void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int a, d;

    printf("\nDigite o numero do territorio atacante: ");
    scanf("%d", &a);
    printf("Digite o numero do territorio defensor: ");
    scanf("%d", &d);

    simularAtaque(mapa, a - 1, d - 1, corJogador);
}

// Lógica da batalha
void simularAtaque(Territorio* mapa, int a, int d, const char* corJogador) {
    if (a < 0 || a >= MAX_TERRITORIOS || d < 0 || d >= MAX_TERRITORIOS || a == d) {
        printf("\nAtaque invalido.\n");
        return;
    }

    if (strcmp(mapa[a].cor, corJogador) != 0) {
        printf("\nVoce so pode atacar com territorios seus.\n");
        return;
    }

    if (mapa[a].tropas < 1) {
        printf("\nO territorio selecionado nao tem tropas suficientes.\n");
        return;
    }

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\nDado ataque: %d | Dado defesa: %d\n", dadoA, dadoD);

    if (dadoA >= dadoD) {
        mapa[d].tropas--;

        printf("Defensor perdeu 1 tropa!\n");

        if (mapa[d].tropas <= 0) {
            printf("Territorio CONQUISTADO!\n");
            strcpy(mapa[d].cor, corJogador);
            mapa[d].tropas = 1;
            mapa[a].tropas--;
        }
    } else {
        printf("Ataque falhou.\n");
    }
}

// Sorteia a missão
int sortearMissao() {
    return rand() % TOTAL_MISSOES;
}

// Verifica se a missão foi concluída
int verificarVitoria(const Territorio* mapa, const char* corJogador, int missao) {
    if (missao == 0) {
        for (int i = 0; i < MAX_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0)
                return 0;
        }
        return 1; // verde destruído
    }

    if (missao == 1) {
        int cont = 0;
        for (int i = 0; i < MAX_TERRITORIOS; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0)
                cont++;

        return cont >= 3;
    }

    return 0;
}

// Limpa buffer stdin
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
