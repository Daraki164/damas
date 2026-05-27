#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <damas.h>



char gacTabuleiro[LINHA][COLUNA];
char gcTurno = PECA_B;

int main(){
    int  iMovimentos;
    char szEntrada[64];
    char *pOrigem;

    vIniciaTabuleiro();

    // Loop principal do jogo
    while(TRUE){
        do{
            // Interface de seleção de peça
            system("clear");
            vExibirTabuleiroTurno();//mostra o tabuleiro no terminal
            
            printf("DIGITE 'Q' PARA SAIR:\n");
            fgets(szEntrada, sizeof(szEntrada), stdin);
            if(szEntrada[0] == 'q' || szEntrada[0] == 'Q')
                return 0;
        
        }while(atoi(szEntrada) < 1 || atoi(szEntrada) > 12);

        pOrigem = piSelecionarPeca(atoi(szEntrada));

        do{
            iMovimentos = 0;

            if((iMovimentos = iMovimentosPossiveis(pOrigem, 1)) == 0) break;

            vExibirTabuleiro();
            fgets(szEntrada, sizeof(szEntrada), stdin);
            
            if(atoi(szEntrada) >= 1 && atoi(szEntrada) <= iMovimentos) break;

            vApagarMovimentos();
       
        }while(TRUE);

        // Valida se a peça escolhida possui movimentos válidos
        if(!iMovimentos){
            vLimparSelecao(pOrigem); 
            continue; 
        }

        vMoverPeca(pOrigem, pcDestino(atoi(szEntrada)));
        
        if(iChecarVitoria())
            return 1;
        
        vChecarDamas  ();
        vTrocarTurnos ();
        system        ("clear"); 
    }
    
    
    return 0;
}

// Verifica se o jogador atual capturou todas as peças do oponente
int iChecarVitoria(){
    int ii;
    int jj;

    if (gcTurno == PECA_B) {
        for (ii = 0; ii < COLUNA; ii++) {
            for (jj = 0; jj < LINHA; jj++) {
                if (gacTabuleiro[ii][jj] == PECA_P || gacTabuleiro[ii][jj] == DAMA_P) {
                    return 0;
                }
            }
        }
        system("clear");
        vExibirTabuleiro();
        printf("VITORIA DAS BRANCAS\n");
        return 1;
    } else {
        for (ii = 0; ii < COLUNA; ii++) {
            for (jj = 0; jj < LINHA; jj++) {
                if (gacTabuleiro[ii][jj] == PECA_B || gacTabuleiro[ii][jj] == DAMA_B) {
                    return 0;
                }
            }
        }
        system("clear");
        vExibirTabuleiro();
        printf("VITORIA DAS PRETAS\n");
        return 1;
    }
}


// Promove peças à Dama quando atingem o extremo oposto do tabuleiro
void vChecarDamas(){
    int ii = 0;

    if(gcTurno == PECA_B){
        for(; ii < COLUNA; ii++)
            if(gacTabuleiro[0][ii] == PECA_B)
                gacTabuleiro[0][ii] = DAMA_B;
    }
    else{
        for(; ii < COLUNA; ii++)
            if(gacTabuleiro[7][ii] == PECA_P)
                gacTabuleiro[7][ii] = DAMA_P;
    }
}

// Localiza o ponteiro da casa destino baseado na escolha numérica do usuário
char *pcDestino(int escolha){  
    int ii;
    int jj;

    for(ii = 0; ii<LINHA; ii++)
        for(jj = 0; jj<LINHA; jj++)
            if(gacTabuleiro[ii][jj] == (char)escolha) return &gacTabuleiro[ii][jj];
    
    return NULL;
}

// Calcula movimentos possíveis para uma Dama, incluindo capturas com parada obrigatória
int iMovimentosDamas(char *origem, int iQtdMovimentos){
    char *pcInicioTabuleiro = &gacTabuleiro[0][0];
    char *pcFinalTabuleiro  = &gacTabuleiro[7][7];
    char *pcAlvo;
    int iColAtual, iColAlvo;
    int iDiag1 = COLUNA + 1; // Diagonal 9
    int iDiag2 = COLUNA - 1; // Diagonal 7

    // Superior Esquerda (Sobe 1 linha, volta 1 coluna = -9)
    pcAlvo = origem - iDiag1;
    while (pcAlvo >= pcInicioTabuleiro) {
        iColAtual = ((pcAlvo + iDiag1) - pcInicioTabuleiro) % 8;
        iColAlvo  = (pcAlvo - pcInicioTabuleiro) % 8;

        if (iColAlvo >= iColAtual) break;

        if (*pcAlvo == SEM_PECA) {
            *pcAlvo = (char)iQtdMovimentos++;
        } else {
            // Verifica possibilidade de captura
            if ((gcTurno == PECA_B && (*pcAlvo == PECA_P || *pcAlvo == DAMA_P)) ||
                (gcTurno == PECA_P && (*pcAlvo == PECA_B || *pcAlvo == DAMA_B))) {
                char *pcPulo = pcAlvo - iDiag1;
                if (pcPulo >= pcInicioTabuleiro) {
                    int iColPulo = (pcPulo - pcInicioTabuleiro) % 8;
                    if (iColPulo < iColAlvo && *pcPulo == SEM_PECA) {
                        *pcPulo = (char)iQtdMovimentos++;
                    }
                }
            }
            break; 
        }

        pcAlvo -= iDiag1;
    }

    // Inferior Esquerda
    pcAlvo = origem + iDiag2;
    while (pcAlvo <= pcFinalTabuleiro) {
        iColAtual = ((pcAlvo - iDiag2) - pcInicioTabuleiro) % 8;
        iColAlvo  = (pcAlvo - pcInicioTabuleiro) % 8;

        if (iColAlvo >= iColAtual) break;

        if (*pcAlvo == SEM_PECA) {
            *pcAlvo = (char)iQtdMovimentos++;
        } else {
            if ((gcTurno == PECA_B && (*pcAlvo == PECA_P || *pcAlvo == DAMA_P)) ||
                (gcTurno == PECA_P && (*pcAlvo == PECA_B || *pcAlvo == DAMA_B))) {
                char *pcPulo = pcAlvo + iDiag2;
                if (pcPulo <= pcFinalTabuleiro) {
                    int iColPulo = (pcPulo - pcInicioTabuleiro) % 8;
                    if (iColPulo < iColAlvo && *pcPulo == SEM_PECA) {
                        *pcPulo = (char)iQtdMovimentos++;
                    }
                }
            }
            break;
        }

        pcAlvo += iDiag2;
    }

    // Superior Direita
    pcAlvo = origem - iDiag2;
    while (pcAlvo >= pcInicioTabuleiro) {
        iColAtual = ((pcAlvo + iDiag2) - pcInicioTabuleiro) % 8;
        iColAlvo  = (pcAlvo - pcInicioTabuleiro) % 8;

        if (iColAlvo <= iColAtual) break;

        if (*pcAlvo == SEM_PECA) {
            *pcAlvo = (char)iQtdMovimentos++;
        } else {
            if ((gcTurno == PECA_B && (*pcAlvo == PECA_P || *pcAlvo == DAMA_P)) ||
                (gcTurno == PECA_P && (*pcAlvo == PECA_B || *pcAlvo == DAMA_B))) {
                char *pcPulo = pcAlvo - iDiag2;
                if (pcPulo >= pcInicioTabuleiro) {
                    int iColPulo = (pcPulo - pcInicioTabuleiro) % 8;
                    if (iColPulo > iColAlvo && *pcPulo == SEM_PECA) {
                        *pcPulo = (char)iQtdMovimentos++;
                    }
                }
            }
            break;
        }

        pcAlvo -= iDiag2;
    }

    // Inferior Direita
    pcAlvo = origem + iDiag1;
    while (pcAlvo <= pcFinalTabuleiro) {
        iColAtual = ((pcAlvo - iDiag1) - pcInicioTabuleiro) % 8;
        iColAlvo  = (pcAlvo - pcInicioTabuleiro) % 8;

        if (iColAlvo <= iColAtual) break;

        if (*pcAlvo == SEM_PECA) {
            *pcAlvo = (char)iQtdMovimentos++;
        } else {
            if ((gcTurno == PECA_B && (*pcAlvo == PECA_P || *pcAlvo == DAMA_P)) ||
                (gcTurno == PECA_P && (*pcAlvo == PECA_B || *pcAlvo == DAMA_B))) {
                char *pcPulo = pcAlvo + iDiag1;
                if (pcPulo <= pcFinalTabuleiro) {
                    int iColPulo = (pcPulo - pcInicioTabuleiro) % 8;
                    if (iColPulo > iColAlvo && *pcPulo == SEM_PECA) {
                        *pcPulo = (char)iQtdMovimentos++;
                    }
                }
            }
            break;
        }

        pcAlvo += iDiag1;
    }

    return iQtdMovimentos - 1;
}

// Identifica movimentos válidos para peças normais e delega para Dama se necessário
int iMovimentosPossiveis(char *origem, int iQtdMovimentos){
    char *pcInicioTabuleiro = &gacTabuleiro[0][0];
    char *pcFinalTabuleiro  = &gacTabuleiro[7][7];
    char *pcDiagonal1       = NULL;
    char *pcDiagonal2       = NULL;

    int iBorda = (origem - pcInicioTabuleiro) % 8;
    int iDiag1 = COLUNA + 1; //diagonal ESQ
    int iDiag2 = COLUNA - 1; //diagonal DIR
    
    // chama a função `iMovimentosDamas` para calcular seus movimentos.
    if(*origem == 'X')
        return iMovimentosDamas(origem, iQtdMovimentos);
    
    // Lógica para o turno das peças brancas (`PECA_B`).
    if(gcTurno == PECA_B){
        //==============vai para cima==================
        // Calcula os ponteiros para as casas diagonais superiores.
        // `pcDiagonal1`: Superior Esquerda (`origem - 9`).
        // `pcDiagonal2`: Superior Direita (`origem - 7`).
        pcDiagonal1 = (origem - iDiag1);
        pcDiagonal2 = (origem - iDiag2);
        //=============================================
        
        // Validação Superior Esquerda e capturas
        if (pcDiagonal1 >= pcInicioTabuleiro && (pcDiagonal1 - pcInicioTabuleiro) % 8 < iBorda) {
            if (*pcDiagonal1 == SEM_PECA)
                *pcDiagonal1 = (char)iQtdMovimentos++;
            else if (*pcDiagonal1 == PECA_P || *pcDiagonal1 == DAMA_P) {
                char *pcPulo = pcDiagonal1 - iDiag1;
                if (pcPulo >= pcInicioTabuleiro && (pcPulo - pcInicioTabuleiro) % 8 < (pcDiagonal1 - pcInicioTabuleiro) % 8)
                    if (*pcPulo == SEM_PECA) *pcPulo = (char)iQtdMovimentos++;
            }
        }

        // Validação Superior Direita e capturas
        if (pcDiagonal2 >= pcInicioTabuleiro && (pcDiagonal2 - pcInicioTabuleiro) % 8 > iBorda) {
            if (*pcDiagonal2 == SEM_PECA)
                *pcDiagonal2 = (char)iQtdMovimentos++;
            else if (*pcDiagonal2 == PECA_P || *pcDiagonal2 == DAMA_P) {
                char *pcPulo = pcDiagonal2 - iDiag2;
                if (pcPulo >= pcInicioTabuleiro && (pcPulo - pcInicioTabuleiro) % 8 > (pcDiagonal2 - pcInicioTabuleiro) % 8)
                    if (*pcPulo == SEM_PECA) *pcPulo = (char)iQtdMovimentos++;
            }
        }
    }
    else{
        //============vai para baixo==================
        // Lógica para o turno das peças pretas (`PECA_P`).
        // Calcula os ponteiros para as casas diagonais inferiores.
        // `pcDiagonal1`: Inferior Direita (`origem + 9`).
        // `pcDiagonal2`: Inferior Esquerda (`origem + 7`).
        pcDiagonal1 = (origem + iDiag1);
        pcDiagonal2 = (origem + iDiag2);
        //============================================

        // Validação Inferior Esquerda e capturas
        if (pcDiagonal2 <= pcFinalTabuleiro && (pcDiagonal2 - pcInicioTabuleiro) % 8 < iBorda) {
            if (*pcDiagonal2 == SEM_PECA)
                *pcDiagonal2 = (char)iQtdMovimentos++;
            else if (*pcDiagonal2 == PECA_B || *pcDiagonal2 == DAMA_B) {
                char *pcPulo = pcDiagonal2 + iDiag2;
                if (pcPulo <= pcFinalTabuleiro && (pcPulo - pcInicioTabuleiro) % 8 < (pcDiagonal2 - pcInicioTabuleiro) % 8)
                    if (*pcPulo == SEM_PECA) *pcPulo = (char)iQtdMovimentos++;
            }
        }

        // Validação Inferior Direita e capturas
        if (pcDiagonal1 <= pcFinalTabuleiro && (pcDiagonal1 - pcInicioTabuleiro) % 8 > iBorda) {
            if (*pcDiagonal1 == SEM_PECA)
                *pcDiagonal1 = (char)iQtdMovimentos++;
            else if (*pcDiagonal1 == PECA_B || *pcDiagonal1 == DAMA_B) {
                char *pcPulo = pcDiagonal1 + iDiag1;
                if (pcPulo <= pcFinalTabuleiro && (pcPulo - pcInicioTabuleiro) % 8 > (pcDiagonal1 - pcInicioTabuleiro) % 8)
                    if (*pcPulo == SEM_PECA) *pcPulo = (char)iQtdMovimentos++;
            }
        }
    }
    return iQtdMovimentos - 1;
}


// Limpa os marcadores numéricos de movimentos do tabuleiro
void vApagarMovimentos(){
    int ii;
    int jj;

    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++)
            if(gacTabuleiro[ii][jj] != PECA_B && gacTabuleiro[ii][jj] != PECA_P &&
               gacTabuleiro[ii][jj] != DAMA_B && gacTabuleiro[ii][jj] != DAMA_P &&
               gacTabuleiro[ii][jj] != 'x'    && gacTabuleiro[ii][jj] != 'X')
                gacTabuleiro[ii][jj] = SEM_PECA;

    }

}

void vApagarMovimentosPossiveis(char *origem){
    if(gcTurno == PECA_B){
        if(*(origem - (COLUNA + 1)) != PECA_B && *(origem - (COLUNA + 1)) != PECA_P)
            *(origem - (COLUNA + 1)) = SEM_PECA;
        else if(*(origem - (COLUNA + 1)) == PECA_P)
            vApagarMovimentosPossiveis((origem - (COLUNA + 1)));
        if(*(origem - (COLUNA - 1)) != PECA_B && *(origem - (COLUNA - 1)) != PECA_P)
            *(origem - (COLUNA - 1)) = SEM_PECA;
        else if(*(origem - (COLUNA - 1)) == PECA_P)
            vApagarMovimentosPossiveis((origem - (COLUNA - 1)));
    }
    else{
        if(*(origem + (COLUNA - 1)) != PECA_B && *(origem + (COLUNA - 1)) != PECA_P)
            *(origem + (COLUNA - 1)) = SEM_PECA;
        else if(*(origem + (COLUNA - 1)) == PECA_B)
            vApagarMovimentosPossiveis((origem + (COLUNA - 1)));
        if(*(origem + (COLUNA + 1)) != PECA_B && *(origem + (COLUNA + 1)) != PECA_P)
            *(origem + (COLUNA + 1)) = SEM_PECA;
        else if(*(origem + (COLUNA + 1)) == PECA_B)
            vApagarMovimentosPossiveis((origem + (COLUNA + 1)));
    }
}

// Remove peças inimigas entre a origem e o destino do movimento (captura)
void vApagarPecasInimigas(char *origem, char *destino){
    int iPasso;
    char *pcTemp;
    int iDistancia = abs(destino - origem);

    if (iDistancia % (COLUNA + 1) == 0) iPasso = COLUNA + 1;
    else if (iDistancia % (COLUNA - 1) == 0) iPasso = COLUNA - 1;
    else return;

    if (destino < origem) iPasso = -iPasso;

    pcTemp = origem + iPasso;
    while (pcTemp != destino) {
        if (*pcTemp == PECA_P || *pcTemp == DAMA_P || 
            *pcTemp == PECA_B || *pcTemp == DAMA_B) {
            *pcTemp = SEM_PECA;
        }
        pcTemp += iPasso;
    }
}

// Move a peça no tabuleiro, mantendo status de Dama se necessário
void vMoverPeca(char *origem, char *destino){
    vApagarMovimentos();

    if (*origem == 'X')
        *destino = (gcTurno == PECA_B) ? DAMA_B : DAMA_P;
    else
        *destino = gcTurno;

    *origem  = SEM_PECA;
    vApagarPecasInimigas(origem, destino);
}

// Restaura o caractere da peça após cancelamento da seleção
void vLimparSelecao(char *peca){
    if (*peca == 'X')
        *peca = (gcTurno == PECA_B) ? DAMA_B : DAMA_P;
    else
        *peca = gcTurno;
}

// Seleciona a peça no tabuleiro baseada na ordem sequencial exibida ao jogador
char *piSelecionarPeca(int sequencia){
    int ii;
    int jj;
    int ik = 0;

    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++){
            if(gcTurno == PECA_B 
               && (gacTabuleiro[ii][jj] == PECA_B || gacTabuleiro[ii][jj] == DAMA_B)) 
               ik++;
            else if(gcTurno == PECA_P
               && (gacTabuleiro[ii][jj] == PECA_P || gacTabuleiro[ii][jj] == DAMA_P))
               ik++;
            if(sequencia == ik){
                if(gacTabuleiro[ii][jj] == DAMA_B 
                    || gacTabuleiro[ii][jj] == DAMA_P)
                    gacTabuleiro[ii][jj] = 'X';
                else 
                    gacTabuleiro[ii][jj] = 'x';
                return &gacTabuleiro[ii][jj];
            }
        }
    }

    return NULL;
}

// Alterna o turno entre peças brancas e pretas
void vTrocarTurnos(){
    gcTurno = (gcTurno == PECA_P) ? PECA_B: PECA_P; 
}

// Determina o conteúdo inicial de cada casa do tabuleiro
char cObterConteudoCasa(char cCor, int iL, int iC){
    if(iL == 3 || iL == 4) return SEM_PECA;
   
    //linha par
    if((iL % 2) == 0)
        return (iC % 2) == 0 ? SEM_PECA: cCor;
    else
        return (iC % 2) == 0 ? cCor  : SEM_PECA;
    
}

// Inicializa o tabuleiro com as peças em suas posições iniciais
void vIniciaTabuleiro(){
    int ii;
    int jj;
    
    char peca = PECA_P;
    char conteudo;

    for(ii = 0; ii<LINHA; ii++){
        if(ii >= 5) peca = PECA_B;
        for(jj = 0; jj<COLUNA; jj++){
            gacTabuleiro[ii][jj] = cObterConteudoCasa(peca, ii, jj);
        }
    }
}

// Configuração de tabuleiro para testes rápidos
void vIniciarTabuleiroTeste(){
    int ii;
    int jj;

    for(ii = 0; ii < COLUNA; ii++)
        for(jj = 0; jj < LINHA; jj++)
            gacTabuleiro[ii][jj] = SEM_PECA;

    gacTabuleiro[4][4] = DAMA_P;
    gacTabuleiro[3][3] = DAMA_B;
}

// Exibe o estado atual do tabuleiro no terminal
void vExibirTabuleiro(){
    int ii;
    int jj;
    int ik = 1;

    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++)
            if(gacTabuleiro[ii][jj] == DAMA_B)
                printf("|BB|");
            else if(gacTabuleiro[ii][jj] == DAMA_P)
                printf("|PP|");
            else if(gacTabuleiro[ii][jj] > 0 && gacTabuleiro[ii][jj] < 32)
                printf("|%02i|", gacTabuleiro[ii][jj]);
            else
                printf("|%c%c|", gacTabuleiro[ii][jj], ' ');
        
        printf("\n");       
    }
}

// Exibe o tabuleiro com índices de seleção para o jogador do turno
void vExibirTabuleiroTurno(){
    int ii;
    int jj;
    int ik = 1;
    
    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++)
            if(gacTabuleiro[ii][jj] == DAMA_B)
                printf("|BB|");
            else if(gacTabuleiro[ii][jj] == DAMA_P)
                printf("|PP|");
            else if(gacTabuleiro[ii][jj] > 0 && gacTabuleiro[ii][jj] < 32)
                printf("|%02i|", gacTabuleiro[ii][jj]);
            else
                printf("|%c%c|", gacTabuleiro[ii][jj], ' ');
            
        printf("\n");       
    }
    printf("\n");
    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++){
            if(gcTurno == PECA_B 
               && (gacTabuleiro[ii][jj] == PECA_B || gacTabuleiro[ii][jj] == DAMA_B))
                printf("|%02i|", ik++);
            else if(gcTurno == PECA_P
               && (gacTabuleiro[ii][jj] == PECA_P || gacTabuleiro[ii][jj] == DAMA_P))
                printf("|%02i|", ik++);
            else if(gacTabuleiro[ii][jj] == DAMA_B)
                printf("|BB|");
            else if(gacTabuleiro[ii][jj] == DAMA_P)
                printf("|PP|");
            else
                printf("|%c%c|", gacTabuleiro[ii][jj], ' ');

        }
        printf("\n");
    }


    printf("TURNO DAS: %s\n", (gcTurno == PECA_P)? "PRETAS": "BRANCAS");
}