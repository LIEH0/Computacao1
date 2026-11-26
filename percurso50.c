#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//------------------------------- CONSTANTES
#define LINHAS        5
#define COLUNAS       10
#define CASA_FINAL    50
#define MAX_JOGADAS   1000

#define USAR_CORES    1

#define JOGADOR_A 0
#define JOGADOR_B 1
#define ROBO_C    2

//---------------------------- VARIAVEIS GLOBAIS
int tabuleiro[LINHAS][COLUNAS];
int cartas[3][3];
int posicoes[3];
int vez;
int vencedor;
int totalJogadas;

// Situacao atual de cada participante (-1 = ainda nao jogou)
int ultimaCarta[3];
int ultimaSaida[3];
int ultimaChegada[3];
int ultimoAtalho[3];

int histQuem[MAX_JOGADAS];
int histCarta[MAX_JOGADAS];
int histSaida[MAX_JOGADAS];
int histChegada[MAX_JOGADAS];
int histAtalho[MAX_JOGADAS];
int histFinal[MAX_JOGADAS];

int numeroArquivo;
char nomeArquivo[80];
char mensagemVitoria[250];

char letras[3] = { 'A', 'B', 'C' };
const char *nomes[3]    = { "Jogador 1", "Jogador 2", "Robo" };
const char *nomesTxt[3] = { "Jogador A", "Jogador B", "Robo C" };
const char *coresNomes[3] = { "azul", "amarelo", "roxo" };

int atalhoOrigem[6]  = { 2, 17, 22, 14, 30, 47 };
int atalhoDestino[6] = { 10, 32, 41, 7, 21, 15 };

const char *corParticipante[3] = { "\033[1;34m", "\033[1;33m", "\033[1;35m" };
const char *corVerde    = "\033[1;32m";
const char *corVermelho = "\033[1;31m";
const char *corReset    = "\033[0m";

//--------------------------------- DADOS

int converterNumero(char linha[]) {
	int i = 0;
	int numero = 0;
	int digitos = 0;

	while (linha[i] == ' ' || linha[i] == '\t') {
		i++;
	}
	while (linha[i] >= '0' && linha[i] <= '9') {
		if (digitos < 6) {
			numero = numero * 10 + (linha[i] - '0');
		}
		digitos++;
		i++;
	}
	if (digitos == 0 || digitos > 6) {
		return -1;
	}
	while (linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\r') {
		i++;
	}
	if (linha[i] != '\n' && linha[i] != '\0') {
		return -1;
	}
	return numero;
}

int lerNumero(char pergunta[], int minimo, int maximo) {
	char linha[100];
	int numero;

	while (1) {
		printf("%s", pergunta);
		fflush(stdout);
		if (fgets(linha, sizeof(linha), stdin) == NULL) {
			printf("\nEntrada encerrada. Fim do programa.\n");
			exit(0);
		}
		int temNovaLinha = 0;
		for (int i = 0; linha[i] != '\0'; i++) {
			if (linha[i] == '\n') {
				temNovaLinha = 1;
			}
		}
		if (temNovaLinha == 0) {
			int c = getchar();
			while (c != '\n' && c != EOF) {
				c = getchar();
			}
		}
		numero = converterNumero(linha);
		if (numero >= minimo && numero <= maximo) {
			return numero;
		}
		printf("Entrada invalida. Digite um numero de %d a %d.\n", minimo, maximo);
	}
}
//-------------------------- TABULEIRO E ATALHOS
void inicializarTabuleiro() {
	for (int faixa = 0; faixa < LINHAS; faixa++) {
		int linha = LINHAS - 1 - faixa;
		for (int p = 0; p < COLUNAS; p++) {
			int coluna;
			if (faixa % 2 == 0) {
				coluna = COLUNAS - 1 - p;
			} else {
				coluna = p;
			}
			tabuleiro[linha][coluna] = faixa * COLUNAS + p + 1;
		}
	}
}

int indiceAtalho(int casa) {
	for (int i = 0; i < 6; i++) {
		if (casa == atalhoOrigem[i]) {
			return i;
		}
	}
	return -1;
}

int verificarAtalho(int quem) {
    int i = indiceAtalho(posicoes[quem]);

    if (i < 0) {
        return 0;
    }

    posicoes[quem] = atalhoDestino[i];

    return atalhoDestino[i];
}

/* Retorna o indice de quem chegou na casa 50, ou -1 se ninguem chegou */
int verificarVitoria() {
	for (int i = 0; i < 3; i++) {
		if (posicoes[i] >= CASA_FINAL) {
			return i;
		}
	}
	return -1;
}

/* ========================================================================== */
/*  CARTAS                                                                     */
/* ========================================================================== */

/* Coloca 1,2,3,4,5,6,0,0,0 em um vetor e embaralha (Fisher-Yates);
 * depois copia o vetor para a matriz 3x3.                                   */
void embaralharCartas() {
	int valores[9] = { 1, 2, 3, 4, 5, 6, 0, 0, 0 };

	for (int i = 8; i > 0; i--) {
		int j = rand() % (i + 1);        /* sorteia um indice de 0 a i */
		int guardado = valores[i];       /* troca valores[i] com valores[j] */
		valores[i] = valores[j];
		valores[j] = guardado;
	}
	for (int k = 0; k < 9; k++) {
		cartas[k / 3][k % 3] = valores[k];   /* k/3 = linha, k%3 = coluna */
	}
}

/* ========================================================================== */
/*  TEXTOS AUXILIARES                                                          */
/* ========================================================================== */

/* "-" se o valor for negativo (ainda nao jogou), senao o proprio numero */
void numeroTexto(int valor, char texto[]) {
	if (valor < 0) {
		strcpy(texto, "-");
	} else {
		snprintf(texto, 12, "%d", valor);
	}
}

/* "Nenhum", "14 -> 7" ou "-" (quando ainda nao jogou) */
void atalhoTexto(int chegada, int destino, char texto[]) {
	if (chegada < 0) {
		strcpy(texto, "-");
	} else if (destino == 0) {
		strcpy(texto, "Nenhum");
	} else {
		snprintf(texto, 20, "%d -> %d", chegada, destino);
	}
}

/* ========================================================================== */
/*  TELA DO TERMINAL                                                           */
/* ========================================================================== */

void mostrarCabecalho() {
	printf("\n========================================\n");
	printf("             PERCURSO 50\n");
	printf("========================================\n");
	if (vencedor < 0) {
		printf("Rodada %d | Vez do %s%s (%c)%s\n", totalJogadas / 3 + 1,
		       corParticipante[vez], nomes[vez], letras[vez], corReset);
	} else {
		printf("FIM DA PARTIDA\n");
	}
}

void mostrarBordaTabuleiro() {
	for (int c = 0; c < COLUNAS; c++) {
		printf("+-----");
	}
	printf("+\n");
}

/* Cada casa tem duas linhas: o numero (com + ou - se for atalho) e as letras
 * dos participantes que estao nela.                                         */
void mostrarTabuleiro() {
	printf("\nTABULEIRO\n");
	for (int l = 0; l < LINHAS; l++) {
		mostrarBordaTabuleiro();

		/* linha 1: numero da casa */
		for (int c = 0; c < COLUNAS; c++) {
			int casa = tabuleiro[l][c];
			int i = indiceAtalho(casa);
			char marca = ' ';
			const char *cor = "";
			if (i >= 0 && i < 3) {
				marca = '+';
				cor = corVerde;
			}
			if (i >= 3)          {
				marca = '-';
				cor = corVermelho;
			}
			printf("| %s%02d%c%s ", cor, casa, marca, corReset);
		}
		printf("|\n");

		/* linha 2: letras dos participantes */
		for (int c = 0; c < COLUNAS; c++) {
			int casa = tabuleiro[l][c];
			int quantos = 0;
			printf("| ");
			for (int i = 0; i < 3; i++) {
				if (posicoes[i] == casa) {
					printf("%s%c%s", corParticipante[i], letras[i], corReset);
					quantos++;
				}
			}
			for (int k = quantos; k < 4; k++) {
				printf(" ");
			}
		}
		printf("|\n");
	}
	mostrarBordaTabuleiro();

	/* casa 0: fica fora das 50 casas */
	printf("Inicio (casa 0): ");
	for (int i = 0; i < 3; i++) {
		if (posicoes[i] == 0) {
			printf("%s%c%s ", corParticipante[i], letras[i], corReset);
		}
	}
	printf("\n");
}

void mostrarLegenda() {
	printf("\nLEGENDA\n");
	for (int i = 0; i < 3; i++) {
		printf("  %s%c%s = %s (%s)\n", corParticipante[i], letras[i], corReset, nomes[i], coresNomes[i]);
	}
	printf("  %s+%s atalho positivo: 2->10, 17->32, 22->41\n", corVerde, corReset);
	printf("  %s-%s atalho negativo: 14->7, 30->21, 47->15\n", corVermelho, corReset);
}

/* Mostra as 9 cartas. So a carta "revelada" (0 a 8) mostra o valor; -1 = nenhuma */
void mostrarCartas(int revelada) {
	printf("\nCARTAS (posicao:valor)\n");
	for (int l = 0; l < 3; l++) {
		printf("  ");
		for (int c = 0; c < 3; c++) {
			int posicao = l * 3 + c;
			if (posicao == revelada) {
				printf("[%d:%d] ", posicao + 1, cartas[l][c]);
			} else {
				printf("[%d:?] ", posicao + 1);
			}
		}
		printf("\n");
	}
}

/* Tabela com a situacao atual dos tres participantes */
void mostrarJogadores() {
	char carta[12], saida[12], chegada[12], atalho[20];

	printf("\nSITUACAO DOS PARTICIPANTES\n");
	printf("+---------+-------+-------+----------------+-----------+---------------+\n");
	printf("| Jogador | Carta | Saida | Chegada normal | Atalho    | Posicao final |\n");
	printf("+---------+-------+-------+----------------+-----------+---------------+\n");
	for (int i = 0; i < 3; i++) {
		numeroTexto(ultimaCarta[i], carta);
		numeroTexto(ultimaSaida[i], saida);
		numeroTexto(ultimaChegada[i], chegada);
		atalhoTexto(ultimaChegada[i], ultimoAtalho[i], atalho);

		printf("| %s%-7c%s | %5s | %5s | %14s | %-9s | %13d |\n",
		       corParticipante[i], letras[i], corReset, carta, saida, chegada, atalho, posicoes[i]);
	}
	printf("+---------+-------+-------+----------------+-----------+---------------+\n");
}

/* Historico completo: todas as jogadas da partida, separadas por rodada */
void mostrarHistorico() {
	char atalho[20];

	printf("\nHISTORICO COMPLETO\n");
	if (totalJogadas == 0) {
		printf("  Nenhuma jogada ainda.\n");
	}
	for (int j = 0; j < totalJogadas; j++) {
		int quem = histQuem[j];

		if (j % 3 == 0) {
			printf("  --- RODADA %d ---\n", j / 3 + 1);
		}
		atalhoTexto(histChegada[j], histAtalho[j], atalho);
		printf("  %s%c%s: saiu da casa %d | carta %d | chegada normal %d | atalho %s | final %d",
		       corParticipante[quem], letras[quem], corReset,
		       histSaida[j], histCarta[j], histChegada[j], atalho, histFinal[j]);
		if (histCarta[j] == 0) {
			printf(" (perdeu a vez)");
		}
		if (histFinal[j] >= CASA_FINAL) {
			printf(" (VENCEU)");
		}
		printf("\n");
	}
}

/* Mostra tudo na ordem pedida: cabecalho, tabuleiro, legenda, cartas, tabela, historico */
void mostrarSituacao(int revelada) {
	mostrarCabecalho();
	mostrarTabuleiro();
	mostrarLegenda();
	mostrarCartas(revelada);
	mostrarJogadores();
	mostrarHistorico();
}

/* ========================================================================== */
/*  JOGADA                                                                     */
/* ========================================================================== */

/* Executa a jogada da carta escolhida (0 a 8) pelo participante da vez */
void realizarJogada(int escolha) {
	int quem = vez;
	int valor = cartas[escolha / 3][escolha % 3];
	int saida = posicoes[quem];
	int chegada;
	int destino = 0;

	if (valor > 0) {                                 /* zero: nao anda */
		posicoes[quem] = posicoes[quem] + valor;
		if (posicoes[quem] > CASA_FINAL) {
			posicoes[quem] = CASA_FINAL;             /* nunca passa de 50 */
		}
	}
	chegada = posicoes[quem];

	if (valor > 0 && chegada < CASA_FINAL) {         /* atalho: so se parou na origem */
		destino = verificarAtalho(quem);
	}

	/* guarda a situacao atual do participante */
	ultimaCarta[quem] = valor;
	ultimaSaida[quem] = saida;
	ultimaChegada[quem] = chegada;
	ultimoAtalho[quem] = destino;

	/* guarda no historico */
	histQuem[totalJogadas] = quem;
	histCarta[totalJogadas] = valor;
	histSaida[totalJogadas] = saida;
	histChegada[totalJogadas] = chegada;
	histAtalho[totalJogadas] = destino;
	histFinal[totalJogadas] = posicoes[quem];
	totalJogadas++;

	/* mostra o resultado na tela */
	printf("\n>>> Carta escolhida: posicao %d\n", escolha + 1);
	mostrarCartas(escolha);
	printf("\n%s%s (%c)%s: ", corParticipante[quem], nomes[quem], letras[quem], corReset);
	if (valor == 0) {
		printf("carta 0! Nao avanca nenhuma casa e perde a vez (continua na casa %d).\n", saida);
	} else {
		printf("carta %d! Saiu da casa %d e chegou na casa %d.\n", valor, saida, chegada);
		if (destino != 0) {
			if (destino > chegada) {
				printf("    Atalho POSITIVO! Vai da casa %d para a casa %d.\n", chegada, destino);
			} else {
				printf("    Atalho NEGATIVO! Volta da casa %d para a casa %d.\n", chegada, destino);
			}
		}
	}

	vencedor = verificarVitoria();
}

/* Um turno completo: cartas novas, mostra a situacao, escolha, jogada */
void jogarTurno() {
	char pergunta[100];
	int escolha;

	embaralharCartas();                              /* embaralha de novo a cada turno */

	printf("\n\n################################################\n");
	mostrarSituacao(-1);                             /* cartas todas escondidas */

	if (vez == ROBO_C) {
		escolha = rand() % 9;                        /* o robo escolhe ao acaso */
		printf("\nO Robo (C) esta escolhendo uma carta... escolheu a posicao %d.\n", escolha + 1);
	} else {
		snprintf(pergunta, sizeof(pergunta), "\n%s (%c), escolha uma posicao de 1 a 9: ", nomes[vez], letras[vez]);
		escolha = lerNumero(pergunta, 1, 9) - 1;
	}

	realizarJogada(escolha);
	vez = (vez + 1) % 3;                             /* ordem fixa: A -> B -> C -> A ... */
}

/* ========================================================================== */
/*  ARQUIVO TXT                                                                */
/* ========================================================================== */

/* Salva o historico da partida em historico_percurso50_N.txt.
 * Procura o primeiro numero livre para nunca sobrescrever um arquivo existente.
 * Retorna 1 se salvou, 0 se deu erro.                                        */
int salvarHistoricoTXT() {
	FILE *arquivo;
	char atalho[20];

	/* procura um nome de arquivo que ainda nao exista */
	while (1) {
		snprintf(nomeArquivo, sizeof(nomeArquivo), "historico_percurso50_%d.txt", numeroArquivo);
		arquivo = fopen(nomeArquivo, "r");
		if (arquivo == NULL) {
			break;                                   /* nao existe: pode usar */
		}
		fclose(arquivo);
		numeroArquivo++;
	}

	arquivo = fopen(nomeArquivo, "w");
	if (arquivo == NULL) {
		return 0;
	}

	/* cabecalho */
	fprintf(arquivo, "========================================\n");
	fprintf(arquivo, "             PERCURSO 50\n");
	fprintf(arquivo, "          HISTORICO DA PARTIDA\n");
	fprintf(arquivo, "========================================\n\n");
	fprintf(arquivo, "Partida numero: %d\n", numeroArquivo);
	fprintf(arquivo, "Total de jogadas: %d\n", totalJogadas);
	fprintf(arquivo, "Resultado final: %s chegou a casa %d na rodada %d.\n",
	        nomesTxt[vencedor], CASA_FINAL, (totalJogadas - 1) / 3 + 1);
	fprintf(arquivo, "Vencedor: %s\n\n", nomesTxt[vencedor]);

	/* historico separado por rodadas */
	for (int j = 0; j < totalJogadas; j++) {
		int quem = histQuem[j];

		if (j % 3 == 0) {
			fprintf(arquivo, "RODADA %d\n\n", j / 3 + 1);
		}
		atalhoTexto(histChegada[j], histAtalho[j], atalho);
		fprintf(arquivo, "%s:\n", nomesTxt[quem]);
		fprintf(arquivo, "  Carta escolhida: %d\n", histCarta[j]);
		fprintf(arquivo, "  Casa de saida: %d\n", histSaida[j]);
		fprintf(arquivo, "  Chegada normal: %d\n", histChegada[j]);
		fprintf(arquivo, "  Atalho: %s\n", atalho);
		fprintf(arquivo, "  Posicao final: %d\n", histFinal[j]);
		if (histCarta[j] == 0) {
			fprintf(arquivo, "  Resultado: Perdeu a vez.\n");
		}
		if (histFinal[j] >= CASA_FINAL) {
			fprintf(arquivo, "  Resultado: Chegou a casa %d e venceu.\n", CASA_FINAL);
		}
		fprintf(arquivo, "\n");

		if (j % 3 == 2 || j == totalJogadas - 1) {   /* fim da rodada */
			fprintf(arquivo, "----------------------------------------\n\n");
		}
	}

	/* resultado final */
	fprintf(arquivo, "RESULTADO FINAL\n\n");
	fprintf(arquivo, "Vencedor: %s\n\n", nomesTxt[vencedor]);
	fprintf(arquivo, "========================================\n");
	fprintf(arquivo, "          FIM DA PARTIDA\n");
	fprintf(arquivo, "========================================\n");

	if (fclose(arquivo) != 0) {
		return 0;
	}
	return 1;
}

/* ========================================================================== */
/*  PARTIDA E MENU                                                             */
/* ========================================================================== */

/* Zera tudo para uma partida nova (o Jogador A sempre comeca) */
void reiniciarPartida() {
	for (int i = 0; i < 3; i++) {
		posicoes[i] = 0;
		ultimaCarta[i] = -1;
		ultimaSaida[i] = -1;
		ultimaChegada[i] = -1;
		ultimoAtalho[i] = 0;
	}
	totalJogadas = 0;
	vencedor = -1;
	vez = JOGADOR_A;
	mensagemVitoria[0] = '\0';       /* apaga a mensagem da partida anterior */
}

/* Joga uma partida inteira e, no fim, salva o historico */
void jogarPartida() {
	reiniciarPartida();

	while (vencedor < 0 && totalJogadas < MAX_JOGADAS) {
		jogarTurno();
	}

	if (vencedor < 0) {              /* nao deve acontecer: o historico encheu */
		snprintf(mensagemVitoria, sizeof(mensagemVitoria),
		         "Partida encerrada: limite de %d jogadas do historico.", MAX_JOGADAS);
		return;
	}

	/* situacao final e mensagem de vitoria */
	printf("\n\n################################################\n");
	mostrarSituacao(-1);
	printf("\n****************************************\n");
	printf("  %s%s (%c) VENCEU A PARTIDA!%s\n", corParticipante[vencedor], nomes[vencedor], letras[vencedor], corReset);
	printf("****************************************\n");

	if (salvarHistoricoTXT() == 1) {
		printf("\nHistorico salvo com sucesso no arquivo: %s\n", nomeArquivo);
		snprintf(mensagemVitoria, sizeof(mensagemVitoria),
		         "Ultima partida: %s (%c) venceu!\nHistorico salvo em: %s",
		         nomes[vencedor], letras[vencedor], nomeArquivo);
		numeroArquivo++;             /* a proxima partida usa o proximo numero */
	} else {
		printf("\nERRO: nao foi possivel salvar o arquivo %s.\n", nomeArquivo);
		snprintf(mensagemVitoria, sizeof(mensagemVitoria),
		         "Ultima partida: %s (%c) venceu!\nERRO: o historico nao pode ser salvo.",
		         nomes[vencedor], letras[vencedor]);
	}
}

void mostrarMenu() {
	printf("\n========================================\n");
	printf("             PERCURSO 50\n");
	printf("========================================\n");
	if (mensagemVitoria[0] != '\0') {
		printf("\n%s\n", mensagemVitoria);
	}
	printf("\n1 - Iniciar partida\n");
	printf("2 - Sair\n\n");
}

void inicializarJogo() {
	srand((unsigned int)time(NULL));                 /* inicia os sorteios (uma vez so) */

	if (USAR_CORES == 0) {                           /* sem cores: todos os codigos viram texto vazio */
		corParticipante[0] = "";
		corParticipante[1] = "";
		corParticipante[2] = "";
		corVerde = "";
		corVermelho = "";
		corReset = "";
	}
	inicializarTabuleiro();
	numeroArquivo = 1;
	mensagemVitoria[0] = '\0';
	vencedor = -1;
}

int main() {
	int opcao;

	inicializarJogo();

	while (1) {
		mostrarMenu();
		opcao = lerNumero("Escolha uma opcao: ", 1, 2);
		if (opcao == 1) {
			jogarPartida();
		} else {
			return 0;
		}
	}
}
