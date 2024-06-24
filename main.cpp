#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "gl_canvas2d.h"


//variaveis globais
int screenWidth = 700, screenHeight = 700; //largura e altura inicial da tela
int alturaTabuleiro = 600;
int mouseX, mouseY;
int linhas = 6;
int colunas = 7;
int tabuleiro[6][7];
int c1 = 6, c2 = 6, c3 = 6, c4 = 6, c5 = 6, c6 = 6, c7 = 6;
int ganhador = 0; // sera 0 se ninguem ganhou, 1 se o vermelho ganhou, 2 se o amarelo ganhou, 3 empate
int playerAtual = 1; //1 = vermelho, 2 = amarelo
enum GameState {
    TELA_INICIO,
    TELA_JOGO,
    MENU_AJUDA,
    TELA_FIM,
    GAME_QUIT
};
enum GameState connect4 = TELA_INICIO;
enum GameState estadoAnterior; //usada para "lembrar" o menu de ajuda em qual tela estava
float anima = 700;
float raioQuit = 500;
float raioInit = 0;
int tamanhoCasa = 80;
int scoreVermelho = 0, scoreAmarelo = 0;
float rV = 1, gV = 0.18, bV = 0.18; // Cor vermelha
float rA = 1, gA = 0.8, bA = 0; // Cor amarela
float rR = 0.26, gR = 0.13, bR = 0.93; // Cor azul
float rL = 1, gL = 0.5, bL = 0; // Cor laranja


// Função para inicializar o tabuleiro, definindo todas as posições como 0 (vazio)
void inicializaTabuleiro() {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = 0;
        }
    }
}

void desenhaInicio() {
    CV::color(rR, gR, bR);
    CV::rect(285, 185, 415, 225);
    CV::rect(280, 180, 420, 230);
    CV::rect(230, 480, 470, 530);
    CV::rect(225, 475, 475, 535);
    CV::rect((screenWidth / 2) - 40 , screenHeight - 85, (screenWidth / 2) + 40, screenHeight - 55);
    CV::text(331, screenHeight - 75, "HELP", 13);
    CV::text(328, 202, "PLAY!", 15);
    CV::color(0,0,0);
    CV::text(253, 500, "Welcome to Connect4!", 15);
    CV::text(227, 400, "Select a player to start:", 13);
    CV::text(285, 245, "ENTER to play", 13);
    if (playerAtual == 1) {
        CV::circle((screenWidth / 2) - 45, 340, 35, 35);
    } else if (playerAtual == 2) {
        CV::circle((screenWidth / 2) + 45, 340, 35, 35);
    }
    CV::color(rV, gV, bV);
    CV::circleFill((screenWidth / 2) - 45, 340, 30, 30);
    CV::color(rA, gA, bA);
    CV::circleFill((screenWidth / 2) + 45, 340, 30, 30);
}

//Função que vai verificar quem ganhou o jogo
int verificaGanhador()
{
    //Verificar a vitória vertical
    for (int i = 0; i < colunas; i++) {
        for (int j = 0; j < (linhas - 3); j++) {
            if (tabuleiro[j][i] != 0 && tabuleiro[j][i] == tabuleiro[j + 1][i] &&
                tabuleiro[j + 1][i] == tabuleiro[j + 2][i] && tabuleiro[j + 2][i] == tabuleiro[j + 3][i]) {
                    return tabuleiro[j][i];
            }
        }
    }

    //Verificar a vitória horizontal
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < (colunas - 3); j++) {
            if (tabuleiro[i][j] != 0 && tabuleiro[i][j] == tabuleiro[i][j + 1] &&
                tabuleiro[i][j + 1] == tabuleiro[i][j + 2] && tabuleiro[i][j + 2] == tabuleiro[i][j + 3]) {
                    return tabuleiro[i][j];
            }
        }
    }

    //Verificar a vitória diagonal /
    for (int i = 0; i < (linhas - 3); i++) {
        for (int j = 0; j < (colunas - 3); j++) {
            if (tabuleiro[i][j] != 0 && tabuleiro[i][j] == tabuleiro[i + 1][j + 1] &&
                tabuleiro[i + 1][j + 1] == tabuleiro[i + 2][j + 2] && tabuleiro[i + 2][j + 2] == tabuleiro[i + 3][j + 3]) {
                    return tabuleiro[i][j];
            }
        }
    }

    //Verificar a vitória diagonal \

    for (int i = 0; i < (linhas - 3); i++) {
        for (int j = (colunas - 1); j > 2; j--) {
            if (tabuleiro[i][j] != 0 && tabuleiro[i][j] == tabuleiro[i + 1][j - 1] &&
                tabuleiro[i + 1][j - 1] == tabuleiro[i + 2][j - 2] && tabuleiro[i + 2][j - 2] == tabuleiro[i + 3][j - 3]) {
                    return tabuleiro[i][j];
            }
        }
    }

    //Verifica empate
    if (c1 == 0 && c2 == 0 && c3 == 0 && c4 == 0 && c5 == 0 && c6 == 0 && c7 == 0) {
        return 3;
    }

    //Retorna se ninguém ganhou
    return 0;
}

void trocaPlayer() {
    if (playerAtual == 1) {
        playerAtual = 2;
    } else {
        playerAtual = 1;
    }
}

void alocaPeca(int col) {
    if (col == 1 && c1 > 0) {
        tabuleiro[6 - c1][col - 1] = playerAtual;
        trocaPlayer();
        c1--;
    } else if (col == 2 && c2 > 0) {
        tabuleiro[6 - c2][col - 1] = playerAtual;
        trocaPlayer();
        c2--;
    } else if (col == 3 && c3 > 0) {
        tabuleiro[6 - c3][col - 1] = playerAtual;
        trocaPlayer();
        c3--;
    } else if (col == 4 && c4 > 0) {
        tabuleiro[6 - c4][col - 1] = playerAtual;
        trocaPlayer();
        c4--;
    } else if (col == 5 && c5 > 0) {
        tabuleiro[6 - c5][col - 1] = playerAtual;
        trocaPlayer();
        c5--;
    } else if (col == 6 && c6 > 0) {
        tabuleiro[6 - c6][col - 1] = playerAtual;
        trocaPlayer();
        c6--;
    } else if (col == 7 && c7 > 0) {
        tabuleiro[6 - c7][col - 1] = playerAtual;
        trocaPlayer();
        c7--;
    }
}

void desenhaBotoes(int margemX, int margemY) {
    for (int i = 0; i < colunas; i++) {
        char str[2];
        sprintf(str, "%d", i + 1);
        CV::color(rR, gR, bR);
        CV::rect(margemX + i * tamanhoCasa + 15, margemY + linhas * tamanhoCasa + 10, margemX + (i + 1) * tamanhoCasa - 15, margemY + (linhas + 1) * tamanhoCasa - 40);
        CV::color(0, 0, 0);
        CV::text(margemX + (i + 1) * tamanhoCasa - 44, margemY + (linhas + 1) * tamanhoCasa - 60, str, 13);
    }
}

// Função para imprimir o tabuleiro no console
void desenhaJogo() {
    int margemX = (screenWidth - (colunas * tamanhoCasa)) / 2;
    int margemY = (alturaTabuleiro - (linhas * tamanhoCasa)) / 2;
    CV::color(0.4, 0.4, 1); //Cor azul
    CV::rectFill(margemX, margemY, screenWidth - margemX, alturaTabuleiro - margemY);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == 0) {
                CV::color(1, 1, 1);
                CV::circleFill(margemX + (j + 1) * tamanhoCasa - (tamanhoCasa / 2), margemY + (i + 1) * tamanhoCasa - (tamanhoCasa / 2), 30, 30);
            } else if (tabuleiro[i][j] == 1) {
                CV::color(rV, gV, bV);
                CV::circleFill(margemX + (j + 1) * tamanhoCasa - (tamanhoCasa / 2), margemY + (i + 1) * tamanhoCasa - (tamanhoCasa / 2), 30, 30);
            } else if (tabuleiro[i][j] == 2) {
                CV::color(rA, gA, bA);
                CV::circleFill(margemX + (j + 1) * tamanhoCasa - (tamanhoCasa / 2), margemY + (i + 1) * tamanhoCasa - (tamanhoCasa / 2), 30, 30);
            }
        }
    }
    CV::color(rR, gR, bR);
    CV::rect((screenWidth / 2) - 40, screenHeight - 85, (screenWidth / 2) + 40, screenHeight - 55);

    CV::text(331, screenHeight - 75, "HELP", 13);
    CV::text(520, screenHeight - 45, "Board size:", 13);
    CV::rect(510, screenHeight - 85, 560, screenHeight - 55);
    CV::rect(580, screenHeight - 85, 630, screenHeight - 55);
    CV::text(530, screenHeight - 75, "-", 15);
    CV::text(600, screenHeight - 75, "+", 15);

    if (playerAtual == 1) {
        CV::color(rV, gV, bV);
        CV::text(70, screenHeight - 75, "Your turn, Red!", 15);
        CV::circleFill(230, screenHeight - 70, 10, 20);
    } else if (playerAtual == 2) {
        CV::color(rA, gA, bA);
        CV::text(70, screenHeight - 75, "Your turn, Yellow!", 15);
        CV::circleFill(260, screenHeight - 70, 10, 20);
    }


    desenhaBotoes(margemX, margemY);
}

//função menu ajuda
void desenhaMenuAjuda() {
    CV::color(0, 0, 0);
    CV::rect(50, 50, screenWidth - 50, screenHeight - 50);
    CV::text(90, screenHeight - 150, "1 - Select the Player who will start the match;", 13);
    CV::text(90, screenHeight - 190, "2 - Click ENTER or the Play button to start!", 13);
    CV::text(90, screenHeight - 230, "3 - Check the current player in the upper left", 13);
    CV::text(90, screenHeight - 250, "corner and, on your turn, click on the column", 13);
    CV::text(90, screenHeight - 270, "button in which you want to place your piece;", 13);
    CV::text(90, screenHeight - 310, "4 - Your objective is to place 4 pieces in a", 13);
    CV::text(90, screenHeight - 330, "straight line, either vertically, horizontally", 13);
    CV::text(90, screenHeight - 350, "or diagonally;", 13);
    CV::text(90, screenHeight - 390, "5 - Remember that the piece will be placed in", 13);
    CV::text(90, screenHeight - 410, "the lowest empty space of the selected column;", 13);
    CV::text(90, screenHeight - 450, "6 - When the objective is reached by one of the", 13);
    CV::text(90, screenHeight - 470, "players, or when there are no more free spaces,", 13);
    CV::text(90, screenHeight - 490, "the game will end;", 13);
    CV::text(90, screenHeight - 530, "7 - If you want to increase or decrease the size", 13);
    CV::text(90, screenHeight - 550, "of the board, click the + or - keys on the keyboard", 13);
    CV::text(90, screenHeight - 570, "or the buttons on the screen.", 13);
    CV::color(rR, gR, bR);
    CV::line(screenWidth - 100, screenHeight - 100, screenWidth - 70, screenHeight - 70);
    CV::line(screenWidth - 70, screenHeight - 100, screenWidth - 100, screenHeight - 70);
    CV::text(90, screenHeight - 100, "Help menu", 15);
}

void desenhaFim() {
    CV::color(1, 1, 1);
    CV::rectFill(150, 150, 550, 550);

    if (ganhador == 1) {
        CV::color(rV, gV, bV);
        CV::rect(150, 150, 550, 550);
        CV::rect(285, 260, 415, 300);
        CV::rect(285, 200, 415, 240);
        CV::rect(285, 445, 415, 485);
        CV::rect(280, 440, 420, 490);
        CV::color(0, 0, 0);
        CV::text(307, 460, "RED WINS!", 15);
    } else if (ganhador == 2) {
        CV::color(rA, gA, bA);
        CV::rect(150, 150, 550, 550);
        CV::rect(285, 260, 415, 300);
        CV::rect(285, 200, 415, 240);
        CV::rect(285, 445, 415, 485);
        CV::rect(280, 440, 420, 490);
        CV::color(0, 0, 0);
        CV::text(292, 460, "YELLOW WINS!", 15);
    } else if (ganhador == 3) {
        CV::color(rL, gL, bL);
        CV::rect(150, 150, 550, 550);
        CV::rect(285, 260, 415, 300);
        CV::rect(285, 200, 415, 240);
        CV::rect(285, 445, 415, 485);
        CV::rect(280, 440, 420, 490);
        CV::color(0, 0, 0);
        CV::text(293, 460, "IT'S A DRAW!", 15);
    }
    CV::color(0, 0, 0);
    CV::text(327, 400, "SCORE", 13);
    CV::text(288, 380, "Red.........", 13);
    CV::text(288, 360, "Yellow......", 13);
    char strp1[5];
    sprintf(strp1, "%d", scoreVermelho);
    char strp2[5];
    sprintf(strp2, "%d", scoreAmarelo);
    CV::text(408, 380, strp1, 13);
    CV::text(408, 360, strp2, 13);
    CV::text(297, 275, "PLAY AGAIN!", 15);
    CV::text(331, 215, "QUIT", 15);
}

int geraRandom() {
    return rand() % 701;
}

void animaFim() {
    if (ganhador == 1) {
        CV::color(rV, gV, bV);
    } else if (ganhador == 2) {
        CV::color(rA, gA, bA);
    }
    int x = geraRandom();
    int y = geraRandom();
    CV::circleFill(x, y, 5, 5);
}

//Chamada múltiplas vezes
void render() {
    CV::clear(1,1,1);
    switch (connect4) {
        case TELA_INICIO:
            CV::color(0, 0, 0);
            CV::rectFill(0, 0, 700, 700);
            if (raioInit <= 600) {
                raioInit += 0.5;
            }
            CV::color(1, 1, 1);
            CV::circleFill(screenWidth/2, screenHeight/2, raioInit, raioInit);
            if (raioInit >= 300) {
                desenhaInicio();
            }
            break;

        case TELA_JOGO:
            desenhaJogo();
            ganhador = verificaGanhador();
            if (ganhador == 1) {
                scoreVermelho++;
                connect4 = TELA_FIM;
            } else if (ganhador == 2) {
                scoreAmarelo++;
                connect4 = TELA_FIM;
            } else if (ganhador == 3) {
                connect4 = TELA_FIM;
            }
            break;

        case MENU_AJUDA:
            desenhaMenuAjuda();
            break;

        case TELA_FIM:
            if (ganhador == 1 || ganhador == 2) {
                for (int i = 0; i < 50; i++) {
                    animaFim();
                }
            }
            desenhaFim();
            break;

        case GAME_QUIT:
            if (scoreAmarelo < scoreVermelho) {
                CV::color(rV, gV, bV);
            } else if (scoreVermelho < scoreAmarelo) {
                CV::color(rA, gA, bA);
            } else {
                CV::color(0, 0, 0);
            }
            CV::rectFill(0, 0, 700, 700);
            raioQuit -= 1;
            CV::color(1, 1, 1);
            CV::circleFill(screenWidth/2, screenHeight/2, raioQuit, raioQuit);
            if (raioQuit == 0) {
                exit(0);
            }
            break;
    }
}

void keyboard(int key) {
    switch(key) {
        case 43:
            if (colunas < 7 ) {
                colunas++;
            }
            break;

        case 45:
            if (colunas > 4) {
                colunas--;
            }
            break;
        case 13:
            if (connect4 == TELA_INICIO) {
                connect4 = TELA_JOGO;
            }
            break;
    }
}

void keyboardUp(int key)
{
}

void botoesInicio() {
    if (mouseX >= 275 && mouseX <= 425 && mouseY >= 185  && mouseY <= 225) {
        connect4 = TELA_JOGO;
    }
    if (mouseX >= ((screenWidth / 2) - 40) && mouseX <= ((screenWidth / 2) + 40) && mouseY >= (screenHeight - 85)  && mouseY <= (screenHeight - 55)) {
        estadoAnterior = connect4;
        connect4 = MENU_AJUDA;
    }
    if (mouseX >= 275 && mouseX <= 335 && mouseY >= 310  && mouseY <= 370) {
        playerAtual = 1;
    }
    if (mouseX >= 365 && mouseX <= 425 && mouseY >= 310  && mouseY <= 370) {
        playerAtual = 2;
    }
}

void botoesJogo() {
    int margemX = (screenWidth - (colunas * tamanhoCasa)) / 2;
    int margemY = (alturaTabuleiro - (linhas * tamanhoCasa)) / 2;
    for (int i = 0; i < colunas; i++) {
        if (mouseX >= (margemX + i * tamanhoCasa + 15) && mouseX <= (margemX + (i + 1) * tamanhoCasa - 15) && mouseY >= (margemY + linhas * tamanhoCasa + 10) && mouseY <= (margemY + (linhas + 1) * tamanhoCasa - 40)) {
            alocaPeca(i + 1);
        }
    }
    if (mouseX >= ((screenWidth / 2) - 40) && mouseX <= ((screenWidth / 2) + 40) && mouseY >= (screenHeight - 85)  && mouseY <= (screenHeight - 55)) {
        estadoAnterior = connect4;
        connect4 = MENU_AJUDA;
    }
    if (mouseX >= 580 && mouseX <= 630 && mouseY >= (screenHeight - 85)  && mouseY <= (screenHeight - 55)) {
        if (colunas < 7 ) {
            colunas++;
        }
    }
    if (mouseX >= 510 && mouseX <= 560 && mouseY >= (screenHeight - 85)  && mouseY <= (screenHeight - 55)) {
        if (colunas > 4) {
            colunas--;
        }
    }
}

void botoesMenuAjuda() {
    if (mouseX >= (screenWidth - 100) && mouseX <= (screenWidth - 70) && mouseY >= (screenHeight - 100) && mouseY <= (screenHeight - 70)) {
        connect4 = estadoAnterior;
    }
}

void botoesFim() {
    if (mouseX >= 285 && mouseX <= 415 && mouseY >= 260 && mouseY <= 300) {
        c1 = 6; c2 = 6; c3 = 6; c4 = 6; c5 = 6; c6 = 6; c7 = 6;
        ganhador = 0;
        inicializaTabuleiro();
        connect4 = TELA_JOGO;
    }
    if (mouseX >= 285 && mouseX <= 415 && mouseY >= 200 && mouseY <= 240) {
        connect4 = GAME_QUIT;
    }
}

void mouse(int button, int state, int wheel, int direction, int x, int y) {

   mouseX = x;
   mouseY = y;
   printf("X:%d Y:%d\n", mouseX, mouseY);

   if (state == 0) {
       switch (connect4) {
            case TELA_INICIO:
                botoesInicio();
                break;

            case TELA_JOGO:
                botoesJogo();
                break;

            case MENU_AJUDA:
                botoesMenuAjuda();
                break;

            case TELA_FIM:
                botoesFim();
                break;
        }
    }
}

//Chamada uma vez
int main(void) {
   CV::init(&screenWidth, &screenHeight, "Trabalho da Amanda");

   srand(time(NULL));
   inicializaTabuleiro();

   CV::run();
}
