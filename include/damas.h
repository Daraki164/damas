#define FALSE      0
#define TRUE       1

#define LINHA      8
#define COLUNA     8
#define PECAS_COR 12
#define PECA_P    'P'
#define DAMA_P    'Q'
#define PECA_B    'B'
#define DAMA_B    'C'
#define SEM_PECA  ' '

int iMovimentosPossiveis       (char *origem, int iQtdMovimentos);

void vExibirTabuleiroTurno     ();
void vIniciaTabuleiro          ();
void vTrocarTurnos             ();
void vExibirTabuleiro          ();
void vChecarDamas              ();
void vMoverPeca                (char *origem, char *destino);
void vApagarMovimentosPossiveis(char *origem);
void vLimparSelecao            (char *peca);
void vApagarPecasInimigas      (char *origem, char *destino);

char *piSelecionarPeca         (int sequencia);
char *pcDestino                (int escolha);