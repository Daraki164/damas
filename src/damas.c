#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <damas.h>



char gacTabuleiro[LINHA][COLUNA];
char gcTurno = PECA_B;



//====================================================
int main(){
    int  iMovimentos;

    char szEntrada[64];
    char *pOrigem;

    vIniciaTabuleiro();//declara o tabuleiro
    while(TRUE)//game loop
    {
        do{
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

            vApagarMovimentosPossiveis(pOrigem);
       
        }while(TRUE);

        //seleciona outra peça sem for uma invalida(sem movimentos)
        if(!iMovimentos){
            vLimparSelecao(pOrigem); 
            continue; 
        }

        vMoverPeca(pOrigem, pcDestino(atoi(szEntrada)));
   
        vChecarDamas();
        vTrocarTurnos();
        system("clear"); 
    }
    
    
    return 0;
}
//=================cotinuar==================================
//logiga de escolha chumbada9
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

char *pcDestino(int escolha){  
    int ii;
    int jj;

    for(ii = 0; ii<LINHA; ii++)
        for(jj = 0; jj<LINHA; jj++)
            if(gacTabuleiro[ii][jj] == ('0' + escolha)) return &gacTabuleiro[ii][jj];
    
    return NULL;
}
//goto
//função recursiva
int iMovimentosPossiveis(char *origem, int iQtdMovimentos){
    char *pcInicioTabuleiro = &gacTabuleiro[0][0];
    char *pcFinalTabuleiro  = &gacTabuleiro[7][7];
    char *pcDiagonal1       = NULL;
    char *pcDiagonal2       = NULL;

    int iBorda = (origem - pcInicioTabuleiro) % 8;
    int iDiag1 = COLUNA + 1;
    int iDiag2 = COLUNA - 1;
    

    if(gcTurno == PECA_B){

        pcDiagonal1 = (origem - iDiag1);
        pcDiagonal2 = (origem - iDiag2);

        if(pcDiagonal1 < pcInicioTabuleiro);
        else if(iBorda != 0 && *pcDiagonal1 == SEM_PECA)
            *pcDiagonal1 = '0'+iQtdMovimentos++;
        else if(*pcDiagonal1 == PECA_P && *origem != PECA_P){
            iBorda = (pcDiagonal1 - pcInicioTabuleiro) % 8;
            if(iBorda != 0 && *(pcDiagonal1 - iDiag1) == SEM_PECA)
                *(pcDiagonal1 - iDiag1) = '0'+iQtdMovimentos++;
        }
        if(pcDiagonal2 < pcInicioTabuleiro);
        else if(iBorda != 7 && *(pcDiagonal2) == SEM_PECA)
            *(pcDiagonal2) = '0'+iQtdMovimentos++;
        else if(*(pcDiagonal2) == PECA_P && *origem != PECA_P){
            iBorda = (pcDiagonal2 - pcInicioTabuleiro) % 8;
            if(iBorda != 7 && *(pcDiagonal2 - iDiag2) == SEM_PECA)
                *(pcDiagonal2 - iDiag2) = '0'+iQtdMovimentos++;
        }
    }
    else{
        pcDiagonal1 = (origem + iDiag1);
        pcDiagonal2 = (origem + iDiag2);

        if(pcDiagonal2 > pcFinalTabuleiro);
        else if(iBorda != 0 && *(pcDiagonal2) == SEM_PECA)
            *(pcDiagonal2) = '0'+iQtdMovimentos++;
        else if(*(pcDiagonal2) == PECA_B && *origem != PECA_B){
            iBorda = (pcDiagonal2 - pcInicioTabuleiro) % 8;
            if(iBorda != 0 && *(pcDiagonal2 + iDiag2) == SEM_PECA)
                *(pcDiagonal2 + iDiag2) = '0'+iQtdMovimentos++;
        } 
        if(pcDiagonal1 > pcFinalTabuleiro);
        else if(iBorda != 7 && *(pcDiagonal1) == SEM_PECA)
            *(pcDiagonal1) = '0'+iQtdMovimentos++;
        else if(*(pcDiagonal1) == PECA_B && *origem != PECA_B){
            iBorda = (pcDiagonal1 - pcInicioTabuleiro) % 8;
            if(iBorda != 7 && *(pcDiagonal1 + iDiag1) == SEM_PECA)
                *(pcDiagonal1 + iDiag1) = '0'+iQtdMovimentos++;
        }
    }
    return iQtdMovimentos - 1;
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

void vApagarPecasInimigas(char *origem, char *destino){
    int iDistancia = abs(destino - origem);

    if(iDistancia > (COLUNA + 1)){
        if(gcTurno == PECA_B){
            if(destino == (origem - (COLUNA - 1) * 2))
                *(origem - (COLUNA - 1)) = SEM_PECA; 
            else if(destino == (origem - (COLUNA + 1) * 2))
                *(origem - (COLUNA + 1)) = SEM_PECA;
        }
        else{
            if(destino == (origem + (COLUNA - 1) * 2))
                *(origem + (COLUNA - 1)) = SEM_PECA; 
            else if(destino == (origem + (COLUNA + 1) * 2))
                *(origem + (COLUNA + 1)) = SEM_PECA;
        }
    }   
}

void vMoverPeca(char *origem, char *destino){
    vApagarMovimentosPossiveis(origem);
    *destino = gcTurno;
    *origem  = SEM_PECA;
    vApagarPecasInimigas(origem, destino);
    


}

void vLimparSelecao(char *peca){
    *peca = gcTurno;
}

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
                gacTabuleiro[ii][jj] = 'X';
                return &gacTabuleiro[ii][jj];
            }
        }
    }

    return NULL;
}

void vTrocarTurnos(){
    gcTurno = (gcTurno == PECA_P) ? PECA_B: PECA_P; 
}

char cObterConteudoCasa(char cCor, int iL, int iC){
    if(iL == 3 || iL == 4) return SEM_PECA;
   
    //linha par
    if((iL % 2) == 0)
        return (iC % 2) == 0 ? SEM_PECA: cCor;
    else
        return (iC % 2) == 0 ? cCor  : SEM_PECA;
    
}

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
            else
                printf("|%c%c|", gacTabuleiro[ii][jj], ' ');
        
        printf("\n");       
    }
}

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
/*

|  ||01||  ||02||  ||03||  ||04|
|05||  ||06||  ||07||  ||08||  |
|  ||  ||  ||09||  ||10||  ||11|
|12||  ||  ||  ||  ||  ||  ||B |
|  ||x ||  ||B ||  ||  ||  ||  |
|  ||  ||  ||  ||B ||  ||B ||  |
|  ||B ||  ||B ||  ||B ||  ||B |
|B ||  ||B ||  ||B ||  ||B ||  |

*/