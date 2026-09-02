#include "poly/poly.h"

int fase=0;

int inicios[6][2]={{5,5},{10,4},{8,6},{4,4},{7,4},{2,6}};
int paddings[6][2]={{2,-2},{0,0},{2,-1},{1,-1},{1,-3},{2,-3}};
int joex;
int joey;

int qtdburacos;
int lenghtburacos[6]={4,6,6,3,6,6};
int buracos[8][2];

int qtdparedes;
int lenghtparedes[6]={26,34,36,24,33,34};
int paredes[40][2];

int caixas[8][2];

void init(){
	joex=inicios[fase][0];
	joey=inicios[fase][1];

	qtdburacos=lenghtburacos[fase];
	for(int a=0;a<8;a++)buracos[a][0]=-1;
	for(int a=0;a<8;a++)buracos[a][1]=-1;
	if(fase==0){
		int arrayburacos[4][2]={{5,3},{7,5},{3,6},{5,8}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	if(fase==1){
		int arrayburacos[6][2]={{8,7},{7,7},{6,7},{5,7},{7,6},{8,6}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	if(fase==2){
		int arrayburacos[6][2]={{3,5},{3,6},{3,7},{8,3},{8,4},{8,5}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	if(fase==3){
		int arrayburacos[3][2]={{6,3},{7,4},{6,6}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	if(fase==4){
		int arrayburacos[6][2]={{5,8},{6,7},{6,9},{7,8},{8,9},{9,8}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	if(fase==5){
		int arrayburacos[6][2]={{4,6},{4,7},{5,6},{5,7},{6,6},{6,7}};
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][0]=arrayburacos[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)buracos[a][1]=arrayburacos[a][1];
	}
	
	qtdparedes=lenghtparedes[fase];
	for(int a=0;a<40;a++)paredes[a][0]=-1;
	for(int a=0;a<40;a++)paredes[a][1]=-1;
	if(fase==0){
		int arrayparedes[26][2]={{4,2},{5,2},{6,2},{6,3},{6,4},{7,4},{8,4},{8,5},{8,6},{7,6},{6,6},{6,7},{6,8},{6,9},{5,9},{4,9},{4,8},{4,7},{3,7},{2,7},{2,6},{2,5},{3,5},{4,5},{4,4},{4,3}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	if(fase==1){
		int arrayparedes[34][2]={{8,2},{9,2},{10,2},{11,2},{11,3},{11,4},{11,5},{10,5},{9,5},{8,5},{7,5},{9,6},{9,7},{9,8},{8,8},{7,8},{6,8},{5,8},{4,8},{4,7},{4,6},{4,5},{3,5},{2,5},{2,4},{2,3},{2,2},{3,2},{4,2},{4,1},{5,1},{6,1},{7,1},{8,1}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	if(fase==2){
		int arrayparedes[36][2]={{3,2},{4,2},{5,2},{6,2},{7,2},{8,2},{9,2},{10,2},{10,3},{10,4},{10,5},{9,5},{9,6},{9,7},{8,7},{7,7},{6,7},{6,8},{5,8},{4,8},{3,8},{2,8},{1,8},{0,8},{0,7},{0,6},{0,5},{0,4},{0,3},{1,3},{2,3},{3,3},{4,5},{5,5},{6,5},{7,5}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	if(fase==3){
		int arrayparedes[24][2]={{3,2},{4,2},{5,2},{6,2},{7,2},{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{8,8},{7,8},{6,8},{5,8},{4,8},{3,8},{3,7},{3,6},{3,5},{3,4},{3,3},{5,4},{5,6}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	if(fase==4){
		int arrayparedes[33][2]={{5,3},{6,3},{7,3},{8,3},{9,3},{9,4},{9,5},{9,6},{9,7},{10,7},{10,8},{10,9},{9,9},{9,10},{8,10},{7,10},{6,10},{5,10},{4,10},{3,10},{2,10},{2,9},{2,8},{2,7},{2,6},{3,6},{4,6},{5,6},{5,5},{5,4},{7,7},{5,9},{7,9}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	if(fase==5){
		int arrayparedes[34][2]={{4,3},{5,3},{6,3},{7,3},{8,3},{8,4},{9,4},{9,5},{9,6},{9,7},{9,8},{9,9},{8,9},{7,9},{6,9},{6,10},{5,10},{4,10},{3,10},{2,10},{2,9},{1,9},{1,8},{1,7},{1,6},{1,5},{1,4},{2,4},{3,4},{4,4},{3,6},{3,7},{7,6},{7,7}};
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][0]=arrayparedes[a][0];
		for(int a=0;a<lenghtparedes[fase];a++)paredes[a][1]=arrayparedes[a][1];
	}
	
	for(int a=0;a<8;a++)caixas[a][0]=-1;
	for(int a=0;a<8;a++)caixas[a][1]=-1;
	if(fase==0){
		int arraycaixas[4][2]={{5,4},{6,5},{5,6},{4,6}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
	if(fase==1){
		int arraycaixas[6][2]={{8,3},{8,4},{6,5},{5,5},{6,3},{4,4}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
	if(fase==2){
		int arraycaixas[6][2]={{2,6},{3,4},{4,6},{5,4},{6,6},{7,4}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
	if(fase==3){
		int arraycaixas[3][2]={{7,4},{6,5},{6,6}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
	if(fase==4){
		int arraycaixas[6][2]={{4,8},{6,5},{6,8},{7,5},{8,5},{8,7}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
	if(fase==5){
		int arraycaixas[6][2]={{3,5},{5,5},{5,6},{5,7},{5,8},{7,8}};
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][0]=arraycaixas[a][0];
		for(int a=0;a<lenghtburacos[fase];a++)caixas[a][1]=arraycaixas[a][1];
	}
}

void desenhaquadrado(int sel){
	int m=sel%15,n=sel/15;
	int background[8]={
	(m*20)+2,(n*20)+10,
	(m*20)+19,(n*20)+10,
	(m*20)+19,(n*20)+30,
	(m*20)+2,(n*20)+30};
	fillpoly(background,4);
}

void desenhaburacos(int m,int n){
	m=m+paddings[fase][0];
	n=n+paddings[fase][1];
	int background[8]={
	(m*20)+2,(n*20)+12,
	(m*20)+19,(n*20)+12,
	(m*20)+19,(n*20)+28,
	(m*20)+2,(n*20)+28};
	fillpoly(background,4);
}

void desenhaparedes(int m,int n){
	m=m+paddings[fase][0];
	n=n+paddings[fase][1];
	int background[8]={
	(m*20)+2,(n*20)+12,
	(m*20)+19,(n*20)+12,
	(m*20)+19,(n*20)+28,
	(m*20)+2,(n*20)+28};
	fillpoly(background,4);
}

void desenhacaixas(int m,int n){
	m=m+paddings[fase][0];
	n=n+paddings[fase][1];
	int background[8]={
	(m*20)+2,(n*20)+12,
	(m*20)+19,(n*20)+12,
	(m*20)+19,(n*20)+28,
	(m*20)+2,(n*20)+28};
	fillpoly(background,4);
}

void desenhajoe(){
	int m=joex;
	int n=joey;
	m=m+paddings[fase][0];
	n=n+paddings[fase][1];
	int head[8] = {
		(m*20)+8,11+(n*20),
		(m*20)+12,11+(n*20),
		(m*20)+12,14+(n*20),
		(m*20)+8,14+(n*20)};
	fillpoly(head,4); //cabeça do Joe
	int arms[8] = {
		(m*20)+4,15+(n*20),
		(m*20)+16,15+(n*20),
		(m*20)+16,17+(n*20),
		(m*20)+4,17+(n*20)};
	fillpoly(arms,4); //braços do Joe
	int body[8] = {
		(m*20)+9,17+(n*20),
		(m*20)+12,17+(n*20),
		(m*20)+12,22+(n*20),
		(m*20)+9,22+(n*20)};
	fillpoly(body,4); //tronco
	int rightLeg[8] = {
		(m*20)+8,22+(n*20),
		(m*20)+10,22+(n*20),
		(m*20)+10,29+(n*20),
		(m*20)+8,29+(n*20)};
	fillpoly(rightLeg,4);
	int leftLeg[8] = {
		(m*20)+11,22+(n*20),
		(m*20)+13,22+(n*20),
		(m*20)+13,29+(n*20),
		(m*20)+11,29+(n*20)};
	fillpoly(leftLeg,4);
}

int last=0;

int main(){
	setvideomode(); //inicia modo grafico
	init();
	while(!keystate(KEY_ESCAPE)){ //o jogo sempre continua while/enquanto o user não pressiona a tecla ESC/escape
		if(keystate(KEY_1)){fase=0;init();}
		if(keystate(KEY_2)){fase=1;init();}
		if(keystate(KEY_3)){fase=2;init();}
		if(keystate(KEY_4)){fase=3;init();}
		if(keystate(KEY_5)){fase=4;init();}
		if(keystate(KEY_6)){fase=5;init();}

		if(last==0&&(keystate(KEY_LEFT)||keystate(KEY_UP)||keystate(KEY_RIGHT)||keystate(KEY_DOWN))){
			int nadaatras=-1,caixaatras=-1,nadaatrasatras=-1;
			if(keystate(KEY_UP))
			{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex&&paredes[x][1]==joey-1)nadaatras=x;
				for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex&&caixas[x][1]==joey-1)caixaatras=x;
				if(nadaatras==-1&&caixaatras==-1)joey--;
				else if(nadaatras==-1)
				{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex&&paredes[x][1]==joey-2)nadaatrasatras=x;
					for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex&&caixas[x][1]==joey-2)nadaatrasatras=x;
					if(nadaatrasatras==-1)
					{	joey--;
						caixas[caixaatras][1]--;
					}
				}
			}
			if(keystate(KEY_DOWN))
			{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex&&paredes[x][1]==joey+1)nadaatras=x;
				for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex&&caixas[x][1]==joey+1)caixaatras=x;
				if(nadaatras==-1&&caixaatras==-1)joey++;
				else if(nadaatras==-1)
				{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex&&paredes[x][1]==joey+2)nadaatrasatras=x;
					for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex&&caixas[x][1]==joey+2)nadaatrasatras=x;
					if(nadaatrasatras==-1)
					{	joey++;
						caixas[caixaatras][1]++;
					}
				}
			}
			if(keystate(KEY_LEFT))
			{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex-1&&paredes[x][1]==joey)nadaatras=x;
				for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex-1&&caixas[x][1]==joey)caixaatras=x;
				if(nadaatras==-1&&caixaatras==-1)joex--;
				else if(nadaatras==-1)
				{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex-2&&paredes[x][1]==joey)nadaatrasatras=x;
					for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex-2&&caixas[x][1]==joey)nadaatrasatras=x;
					if(nadaatrasatras==-1)
					{	joex--;
						caixas[caixaatras][0]--;
					}
				}
			}
			if(keystate(KEY_RIGHT))
			{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex+1&&paredes[x][1]==joey)nadaatras=x;
				for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex+1&&caixas[x][1]==joey)caixaatras=x;
				if(nadaatras==-1&&caixaatras==-1)joex++;
				else if(nadaatras==-1)
				{	for(int x=0;x<qtdparedes;x++)if(paredes[x][0]==joex+2&&paredes[x][1]==joey)nadaatrasatras=x;
					for(int x=0;x<qtdburacos;x++)if(caixas[x][0]==joex+2&&caixas[x][1]==joey)nadaatrasatras=x;
					if(nadaatrasatras==-1)
					{	joex++;
						caixas[caixaatras][0]++;
					}
				}
			}
			last=4;
		}
		else if(last>0)last--;

		setcolor(0);
		for(int c=0;c<135;c++)desenhaquadrado(c);
		setcolor(1);
		for(int c=0;c<qtdparedes;c++)desenhaparedes(paredes[c][0],paredes[c][1]);
		setcolor(4);
		for(int c=0;c<qtdburacos;c++)desenhaburacos(buracos[c][0],buracos[c][1]);
		setcolor(3);
		for(int c=0;c<qtdburacos;c++)desenhacaixas(caixas[c][0],caixas[c][1]);
		setcolor(2);
		desenhajoe();

		setcolor(1);
		blitInt(1,0,5);
		blitInt(2,0,20);
		blitInt(3,0,35);
		blitInt(4,0,50);
		blitInt(5,0,65);
		blitInt(6,0,80);

		setcolor(4);
		blitInt(fase+1,0,(fase*15)+5);

		sleep(20); //delay de 20 milissegndos
		waitvbl(); //processamentos da biblioteca poly
	}
	return 0;
}
