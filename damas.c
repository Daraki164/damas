#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FALSE      0
#define TRUE       1

#define LINHA      8
#define COLUNA     8
#define PECAS_COR 12
#define PECA_P    'P'
#define PECA_B    'B'
#define SEM_PECA  ' '

int iMovimentosPossiveis       (char *origem, int iQtdMovimentos);

void vExibirTabuleiroTurno     ();
void vIniciaTabuleiro          ();
void vTrocarTurnos             ();
void vExibirTabuleiro          ();
void vMoverPeca                (char *origem, char *destino);
void vApagarMovimentosPossiveis(char *origem);
void vLimparSelecao            (char *peca);
void vApagarPecasInimigas      (char *origem, char *destino);

char *piSelecionarPeca         (int sequencia);
char *pcDestino                (int escolha);


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
            
            if(atoi(szEntrada) >= 1 && atoi(szEntrada) < iMovimentos) break;

            vApagarMovimentosPossiveis(pOrigem);
       
        }while(TRUE);

        //seleciona outra peça sem for uma invalida(sem movimentos)
        if(!iMovimentos){
            vLimparSelecao(pOrigem); 
            continue; 
        }

        vMoverPeca(pOrigem, pcDestino(atoi(szEntrada)));
        vExibirTabuleiro();

        // *(piSelecionarPeca(atoi(szEntrada)) - COLUNA) = 'X';

//===================continuar====================================
        // printf("%c | 0X%X\n", *piSelecionarPeca(atoi(szEntrada)), piSelecionarPeca(atoi(szEntrada)));
        // fgets(szEntrada, sizeof(szEntrada), stdin);


/*===============================================
        TODO: função de log
        /*freopen("Damas.log", "a", stdout);
        fflush(stdout);
        fclose(stdout);
        1 |B | 0X9579E068
        1 |P | 0X9579E041
===============================================*/


        vTrocarTurnos();
        system("clear"); 
    }
    
    
    return 0;
}
//=================cotinuar==================================
//logiga de escolha chumbada9


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

    int iBorda = (origem - pcInicioTabuleiro) % 8;
    

    if(gcTurno == PECA_B){
        if(iBorda != 0 && *(origem - (COLUNA + 1)) == SEM_PECA)
            *(origem - (COLUNA + 1)) = '0'+iQtdMovimentos++;
        else if(*(origem - (COLUNA + 1)) == PECA_P && *origem != PECA_P)
            iQtdMovimentos = iMovimentosPossiveis(origem - (COLUNA + 1), iQtdMovimentos);
            //iQtdMovimentos = iMovimentosPossiveis(origem - (COLUNA + 1), iQtdMovimentos);
        if(iBorda != 7 && *(origem - (COLUNA - 1)) == SEM_PECA)
            *(origem - (COLUNA - 1)) = '0'+iQtdMovimentos++;
        else if(*(origem - (COLUNA - 1)) == PECA_P && *origem != PECA_P)
            iQtdMovimentos = iMovimentosPossiveis(origem - (COLUNA - 1), iQtdMovimentos);
    }
    else{
        if(iBorda != 0 && *(origem + (COLUNA - 1)) == SEM_PECA)
            *(origem + (COLUNA - 1)) = '0'+iQtdMovimentos++;
        else if(*(origem + (COLUNA - 1)) == PECA_B && *origem != PECA_B)
            iQtdMovimentos = iMovimentosPossiveis(origem + (COLUNA - 1), iQtdMovimentos); 
        if(iBorda != 7 && *(origem + (COLUNA + 1)) == SEM_PECA)
            *(origem + (COLUNA + 1)) = '0'+iQtdMovimentos++;
        else if(*(origem + (COLUNA + 1)) == PECA_B && *origem != PECA_B)
            iQtdMovimentos = iMovimentosPossiveis(origem + (COLUNA + 1), iQtdMovimentos);
    }

    return iQtdMovimentos;
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
            if(gcTurno == gacTabuleiro[ii][jj]) ik++;
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
            printf("|%c%c|", gacTabuleiro[ii][jj], ' ');
            
        printf("\n");       
    }
    printf("\n");
    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++){
            if(gcTurno == gacTabuleiro[ii][jj])
                printf("|%02i|", ik++);
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