#include "poly/poly.h"
#include <math.h>

// Arrays que armazenam os fatores de movimento em X e Y para cada um dos 36 graus de rotação
// Usados para criar movimento suave em 10° increments (360°/36 = 10° por índice)
int fatorx[36]={9,8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8};
int fatory[36]={0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1};

// Matriz que representa a pista do jogo (17 linhas x 23 colunas)
// 1 = estrada/área navegável, 0 = fora da pista
int pista[17][23]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},{0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},{0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0},{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

// Variáveis globais da posição e orientação do carro
int px,py, // Coordenadas X e Y da posição do carro no mapa (em pixels)
 angulo; // Ângulo de rotação do carro (de 0 a 35 graus de inclinação)

// Função de inicialização - reseta a posição e rotação do carro
void init(){
	px=1320; // Posição inicial X (off-screen, necessário para o cálculo de viewport)
	py=1136; // Posição inicial Y (off-screen, necessário para o cálculo de viewport)
	angulo=0; // Orientação inicial: 0° (apontando para cima)
}

int main(){
	init(); // Inicializa o carro
	setvideomode(); // Configura o modo de vídeo gráfico

	while(!keystate(KEY_ESCAPE)){ // loop principal do jogo - executa até pressionar ESC
		setcolor(0);
		bar(0,0,320,200);

		// Calcula a posição de cada bloco da pista em relação à câmera (centralized em px, py)
		setcolor(21);
		for(int x=0;x<17;x++)
			for(int y=0;y<23;y++)
				if(pista[x][y]==1) // se for parte da estrada
					bar(-px+(80*y),-py+(80*x),80,80); // desenha um bloco de 80x80 pixels, ajustando pela posição do carro

		// Desenha o carro como um polígono (quadrilátero)
		setcolor(17);
		int bordas[8]={
			// Cálculo de 4 pontos (vértices) do carro em relação ao ângulo
			// Cada ponto é calculado usando seno/cosseno com offset angular
			// Centro do carro está em (160, 100) na tela
			160+(sin((((angulo+15)%36)*10)*0.0174)*14),100+(cos((((angulo+15)%36)*10)*0.0174)*14), // Vértice 1
			160+(sin((((angulo+21)%36)*10)*0.0174)*14),100+(cos((((angulo+21)%36)*10)*0.0174)*14), // Vértice 2
			160+(sin((((angulo+33)%36)*10)*0.0174)*14),100+(cos((((angulo+33)%36)*10)*0.0174)*14), // Vértice 3
			160+(sin((((angulo+3)%36)*10)*0.0174)*14),100+(cos((((angulo+3)%36)*10)*0.0174)*14) // Vértice 4
		};
		fillpoly(bordas,4); // Desenha o polígono preenchido (4 vértices)

		// Controles do carro
		if(keystate(KEY_LEFT))angulo=(angulo+1)%36; // Girar à esquerda (incrementa ângulo)
		if(keystate(KEY_RIGHT))angulo=(angulo+35)%36; // Girar à direita (decrementa ângulo, mesmo que -1 em módulo 36)

		// Verifica colisão e movimento do carro
		// Verifica se está dentro dos limites da tela e se a próxima posição é válida (pista[x][y]==1)
		if(py>-20&&pista[((py+20)/80)+1][(px/80)+2]==1){ // move o carro para frente na direção do ângulo atual
			py-=fatorx[angulo]; // Ajusta Y baseado no fator de ângulo
			px+=fatory[angulo]; // Ajusta X baseado no fator de ângulo
		}
		else init(); // se colidir ou sair da pista, reinicia a posição

		sleep(30); // Aguarda 30ms (controla a velocidade do jogo)
		waitvbl(); // Aguarda sincronização vertical (sync com monitor)
	}

	return 0;
}