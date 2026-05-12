#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE       1
#define FALSE      0

#define LINHA      8
#define COLUNA     8
#define PECAS_COR 12
#define PECA_P    'P'
#define PECA_B    'B'
#define SEM_PECA  ' '



void vExibirTabuleiro();
void vIniciaTabuleiro();
void vTrocarTurnos   ();
char *piSelecionarPeca(int sequencia);

char gacTabuleiro[LINHA][COLUNA];
char cTurno = PECA_B;



//====================================================
int main(){
    char szEntrada[64];

    vIniciaTabuleiro();//declara o tabuleiro
    while (TRUE)
    {
        do{//game loop
        vExibirTabuleiro();//mostra o tabuleiro no terminal
        
        printf("DIGITE 'Q' PARA SAIR:\n");
        fgets(szEntrada, sizeof(szEntrada), stdin);
        if(szEntrada[0] == 'q' || szEntrada[0] == 'Q')
            return 0;

        }while (atoi(szEntrada) < 1 || atoi(szEntrada) > 12);
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
//====================================================


char *piSelecionarPeca(int sequencia){

    int ii;
    int jj;
    int ik = 0;

    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++){
            if(cTurno == gacTabuleiro[ii][jj]) ik++;
            if(sequencia == ik)
                return &gacTabuleiro[ii][jj];
        }
    }

    return NULL;
}
void vTrocarTurnos(){
    cTurno = (cTurno == PECA_P) ? PECA_B: PECA_P; 
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
    printf("\n");
    for(ii = 0; ii<LINHA; ii++){
        for(jj = 0; jj<COLUNA; jj++){
            if(cTurno == gacTabuleiro[ii][jj])
                printf("|%02i|", ik++);
            else
                printf("|%c%c|", gacTabuleiro[ii][jj], ' ');

        }
        printf("\n");
    }


    printf("TURNO DAS: %s\n", (cTurno == PECA_P)? "PRETAS": "BRANCAS");

}