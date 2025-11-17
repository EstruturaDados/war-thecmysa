// ============================================================================
//                   PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//                           Camilly Almeida :)
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 5
#define MAX_STRING 30
#define TOTAL_MISSOES 2

// -----------------------------------------
// Estrutura de Território
// -----------------------------------------
typedef struct {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int tropas;
} Territorio;

// -----------------------------------------
// Protótipos
// -----------------------------------------
void tutorial(void);
void cadastrarTerritorios(Territorio *mapa);
void exibirMapa(const Territorio *mapa);
void exibirMenu(void);
void exibirMissao(int missao);

void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *mapa, int atacante, int defensor);

int sortearMissao(void);
int verificarVitoria(const Territorio *mapa, int missao);

void limparBufferEntrada(void);
void lerString(char *buffer, int tamanho);

// ============================================================================
// Tutorial Inicial
// ============================================================================
void tutorial(void) {
    printf("\n=========================================================\n");
    printf("                BEM-VINDO AO WAR ESTRUTURADO!\n");
    printf("=========================================================\n");
    printf("Este é um jogo de estratégia inspirado no clássico WAR.\n\n");

    printf("🎯 OBJETIVO DO JOGO:\n");
    printf("- Cada território possui nome, cor e quantidade de tropas.\n");
    printf("- Você terá uma missão secreta para vencer.\n\n");

    printf("📘 COMO FUNCIONA (RESUMO):\n");
    printf("1) Você irá CADASTRAR os 5 territórios (nome, cor e tropas).\n");
    printf("2) O sistema sorteia sua MISSÃO.\n");
    printf("3) No MENU, escolha:\n");
    printf("   1 - ATACAR: escolha atacante e defensor\n");
    printf("   2 - VERIFICAR MISSÃO: checa se venceu\n");
    printf("   0 - SAIR\n\n");

    printf("⚔️ SISTEMA DE ATAQUE (REGRAS SIMPLES):\n");
    printf("- Cada lado rola um dado (1 a 6).\n");
    printf("- Se o ATACANTE empata ou vence -> defensor perde 1 tropa.\n");
    printf("- Se o defensor chega a 0 tropas -> território é conquistado.\n");
    printf("- NOVA REGRA: quando o atacante vence, ele GANHA +1 tropa (bônus).\n");
    printf("- Para atacar, o território atacante precisa ter pelo menos 2 tropas.\n\n");

    printf("🏆 MISSÕES POSSÍVEIS:\n");
    printf("1) Destruir o exército VERDE.\n");
    printf("2) Conquistar 3 territórios da mesma cor (baseado no território 1).\n\n");

    printf("Pressione ENTER para continuar...");
    getchar();
}

// ============================================================================
// MAIN
// ============================================================================
int main(void) {
    srand((unsigned) time(NULL));

    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria.\n");
        return 1;
    }

    tutorial();                 // mostra o tutorial no inicio
    cadastrarTerritorios(mapa); // Nível Novato: cadastro estático (5 territorios)

    int missao = sortearMissao(); // Nível Mestre: sorteio de missão
    int opcao = -1;

    do {
        /* Limpar a tela é opcional e nem sempre funciona em todos ambientes.
           Mantive comentado para evitar problemas em alguns terminais.
           Para ativar, descomente a linha abaixo. */
        /* system("clear || cls"); */

        exibirMapa(mapa);
        exibirMissao(missao);
        exibirMenu();

        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            opcao = -1;
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;

            case 2:
                if (verificarVitoria(mapa, missao)) {
                    printf("\n🎉 MISSÃO COMPLETA! Você venceu!\n");
                    opcao = 0; // encerra após vitória
                } else {
                    printf("\nA missão ainda não foi concluída.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}

// ============================================================================
// Cadastro – Nível Novato
// ============================================================================
void cadastrarTerritorios(Territorio *mapa) {
    printf("\n=== CADASTRO DOS TERRITÓRIOS (5) ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; ++i) {
        printf("\nTerritório %d:\n", i + 1);

        printf("Nome: ");
        lerString(mapa[i].nome, MAX_STRING);

        printf("Cor do exército: ");
        lerString(mapa[i].cor, MAX_STRING);

        printf("Número de tropas (inteiro >= 0): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            printf("Entrada inválida. Digite um numero inteiro >= 0: ");
            limparBufferEntrada();
        }
        limparBufferEntrada();
    }
    printf("\nCadastro finalizado. Vamos jogar!\n\n");
}

// ============================================================================
// Exibição do mapa e menus
// ============================================================================
void exibirMapa(const Territorio *mapa) {
    printf("\n=========================================\n");
    printf("               MAPA ATUAL\n");
    printf("=========================================\n");
    for (int i = 0; i < MAX_TERRITORIOS; ++i) {
        printf("[%d] %-18s | Dono: %-10s | Tropas: %2d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=========================================\n\n");
}

void exibirMenu(void) {
    printf("--- MENU ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n\n");
}

void exibirMissao(int missao) {
    printf("--- SUA MISSÃO ---\n");
    if (missao == 0) {
        printf("Destruir o exército VERDE.\n");
    } else {
        printf("Conquistar 3 territórios da mesma cor (baseado no território 1).\n");
    }
    printf("-------------------\n\n");
}

// ============================================================================
// Ataque – Nível Aventureiro (com bônus de +1 para atacante em vitória)
// ============================================================================
void faseDeAtaque(Territorio *mapa) {
    int orig, dest;

    printf("\n=== FASE DE ATAQUE ===\n");
    exibirMapa(mapa);

    printf("Escolha o número do território ATACANTE (1-%d): ", MAX_TERRITORIOS);
    if (scanf("%d", &orig) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Escolha o número do território DEFENSOR (1-%d): ", MAX_TERRITORIOS);
    if (scanf("%d", &dest) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    simularAtaque(mapa, orig - 1, dest - 1);
}

void simularAtaque(Territorio *mapa, int a, int d) {
    // valida índices
    if (a < 0 || a >= MAX_TERRITORIOS || d < 0 || d >= MAX_TERRITORIOS) {
        printf("Ataque inválido: índice fora do intervalo.\n");
        return;
    }
    if (a == d) {
        printf("Ataque inválido: atacante e defensor são o mesmo território.\n");
        return;
    }

    // valida tropas mínimas
    if (mapa[a].tropas < 2) {
        printf("Ataque inválido: o território atacante precisa ter pelo menos 2 tropas.\n");
        return;
    }

    // rolagem de dados
    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\n🎲 Ataque: %d  |  Defesa: %d\n", dadoA, dadoD);

    if (dadoA >= dadoD) {
        // atacante vence (empates favorecem o atacante)
        // atacante ganha +1 tropa de bônus
        mapa[a].tropas += 1;
        printf("\n🟢 VITÓRIA DO ATAQUE! Atacante ganha +1 tropa de bônus.\n");

        // defensor perde 1 tropa
        mapa[d].tropas -= 1;
        printf("➖ Defensor perdeu 1 tropa (restam %d).\n", mapa[d].tropas > 0 ? mapa[d].tropas : 0);

        // se defensor zerou, conquista e ocupa
        if (mapa[d].tropas <= 0) {
            printf("\n🏴 TERRITÓRIO CONQUISTADO: %s\n", mapa[d].nome);
            // assumir cor do atacante
            strncpy(mapa[d].cor, mapa[a].cor, MAX_STRING - 1);
            mapa[d].cor[MAX_STRING - 1] = '\0';
            // defensor passa a ter 1 tropa (ocupação)
            mapa[d].tropas = 1;
            // atacante envia 1 tropa para ocupar (se tiver)
            if (mapa[a].tropas > 0) {
                mapa[a].tropas -= 1;
                printf("📦 1 tropa enviada para ocupar o território.\n");
            }
        }
    } else {
        // defensor vence
        mapa[a].tropas -= 1;
        if (mapa[a].tropas < 0) mapa[a].tropas = 0;
        printf("\n🔴 ATAQUE FALHOU! Atacante perdeu 1 tropa (restam %d).\n", mapa[a].tropas);
    }
}

// ============================================================================
// Missões – Nível Mestre
// ============================================================================
int sortearMissao(void) {
    return rand() % TOTAL_MISSOES; // 0 ou 1
}

int verificarVitoria(const Territorio *mapa, int missao) {
    if (!mapa) return 0;

    if (missao == 0) {
        // destruir exército "Verde"
        for (int i = 0; i < MAX_TERRITORIOS; ++i) {
            if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0)
                return 0; // ainda existe verde
        }
        return 1; // verde destruído
    } else {
        // conquistar 3 territórios da mesma cor (usa cor do território 0 como referência)
        const char *corRef = mapa[0].cor;
        int cont = 0;
        for (int i = 0; i < MAX_TERRITORIOS; ++i) {
            if (strcmp(mapa[i].cor, corRef) == 0)
                cont++;
        }
        return (cont >= 3) ? 1 : 0;
    }
}

// ============================================================================
// Utilitários de entrada
// ============================================================================
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}

// lê uma string com fgets e remove '\n'
void lerString(char *buffer, int tamanho) {
    if (!buffer || tamanho <= 0) return;
    if (fgets(buffer, tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}
