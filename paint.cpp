/*
   Arquivo : retangulos.cpp
   Descricao : Programa que possui funcionalidades para o desenho de figuras geometricas 
               apresentando a possibilidade de escolher entre varias figuras, cores,
			   opces de preenchimento e de borda alem da possibilidade de salvar o arquivo
			   para continuar a edicao posteriormente.
*/
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

// ************************** Parametros de configuracao **************************

#define TAM_MAX 1000000       // Numero maximo de posicoes no arranjo
#define PI 3.1415926535898     // Valor aproximado da constante PI

// ************************** Figuras geometricas disponiveis *********************

#define RETANGULO 1
#define RETA 2
#define PONTO 3
#define CIRCULO 4
#define TRIANGULO 5
#define LOSANGO 6
#define PENTAGONO 7
#define HEXAGONO 8
#define HEPTAGONO 9

// ************************** Cores utilizadas ******************

GLfloat r2, g2, b2;             // Variaveis responsaveis por armazenar as configuracoes de cores

//Cores definidas na paleta de cores

#define MARROM 0
#define CINZA 1
#define BRANCO 2
#define ROSA 3
#define AZUL_CLARO 4
#define AMARELO 5
#define AZUL 6
#define VERDE 7
#define VERMELHO 8
#define PRETO 9

// Macros utilizadas para definir a cor corrente

#define USANDO_MARROM       r2 = 0.7; g2 = 0.5; b2 = 0.3; glColor3f (r2, g2, b2);
#define USANDO_CINZA        r2 = 0.4; g2 = 0.4; b2 = 0.4; glColor3f (r2, g2, b2);
#define USANDO_BRANCO       r2 = 1.0; g2 = 1.0; b2 = 1.0; glColor3f (r2, g2, b2);
#define USANDO_ROSA         r2 = 1.0; g2 = 0.0; b2 = 1.0; glColor3f (r2, g2, b2);
#define USANDO_AZUL_CLARO   r2 = 0.0; g2 = 1.0; b2 = 1.0; glColor3f (r2, g2, b2);
#define USANDO_AMARELO      r2 = 1.0; g2 = 1.0; b2 = 0.0; glColor3f (r2, g2, b2);
#define USANDO_AZUL         r2 = 0.0; g2 = 0.0; b2 = 1.0; glColor3f (r2, g2, b2);
#define USANDO_VERDE        r2 = 0.0; g2 = 1.0; b2 = 0.0; glColor3f (r2, g2, b2);
#define USANDO_VERMELHO     r2 = 1.0; g2 = 0.0; b2 = 0.0; glColor3f (r2, g2, b2);
#define USANDO_PRETO        r2 = 0.0; g2 = 0.0; b2 = 0.0; glColor3f (r2, g2, b2);

// Cores com funcoes especiais no sistema como plano de fundo ou destaque de botao

#define COR_ATUAL 10
#define CINZA_CLARO 11
#define COR_FUNDO 12

#define USANDO_COR_ATUAL      glColor3f (r2, g2, b2);
#define USANDO_CINZA_CLARO    glColor3f (0.8, 0.8, 0.8);
#define USANDO_COR_FUNDO      glColor3f (0.2, 0.5, 0.8);

// ******************* Definicao de slots para salvar arquivos *******************

#define SAVE_0 0
#define SAVE_1 1
#define SAVE_2 2
#define SAVE_3 3
#define SAVE_4 4
#define SAVE_5 5
#define SAVE_6 6
int file = SAVE_6;           // Inicializacao do indicador de arquivo com o valor default

// ******************************** Opcoes do menu *******************************

#define NADA -1              // Nenhuma opcao selecionada ( pode ser eliminado )          
#define PRIMITIVA 0          // Opcao de primitivas
#define PREENCHIMENTO 1      // Opcao de preenchimento
#define CHEIO 1
#define VAZIO 2
#define CONTORNO 2           // Opcao pelo tipo de borda
#define CONTINUO 1           // Submenu de contorno 
#define TRACEJADO 2          // Submenu de contorno
#define PONTILHADO 3         // Submenu de contorno
#define LIVRE 3              // Opcao por desenho livre
#define LINHAS 4             // Opcao por LINHAS a linha ( futuramente vai virar largura da linha )
#define PONTOS 5             // Opcao por PONTOS a linha ( futuramente vai virar largura do ponto )
#define DESFAZER 6           // Opcao por desfazer
#define REFAZER 7            // Opcao por refazer
#define LIMPAR 8             // Opcao por limpar
#define AREA 9               // Opcao pela borracha
#define LOAD 10              // Opcao por carregar a figura do slot padrao
#define STORE 11             // Opcao por salvar a figura no slot padrao

int option = NADA;           // Opcao selecionada inicialmente
int sub_option = CONTINUO;   // Tipo de linha ( continua ou tracejada )
bool desenhaLIVRE = false;   // Desenha livre
bool existePontoZero = false;// Determina se o ponto zero foi nicializado para desenhar com mao livre
GLfloat xZero = 0;           // X inicial
GLfloat yZero = 0;           // Y inicial
int aux;                     // Inteiro auxiliar para salvar o estado de uma variavel

// ***************** Inicializacao das variaveis globais utilizadas no sistema *****************

// Dimensoes originais da janela - serao alteradas pela funcao reshape
// se o tamanho da janela for alterado.
int window_w = 800, window_h = 600; 

// Define uma figura de um tipo e com seus atributos particulares
struct Figura {
	GLint tipo;           // Tipo da figura ( retangulo, circulo, ponto e linha )
	GLfloat x1, y1;       // Ponto inicial
	GLfloat x2, y2;       // Ponto final
	GLfloat R;            // Componente vermelha da cor
	GLfloat G;            // Componente verde da cor  
	GLfloat B;            // Componente azul da cor
	int preenchido;       // Indica se a figura esta preenchida
	int linha;            // Indica se a borda eh continua ou tracejada ou pontilhada
	GLint espessura;      // Indica a espessura da borda
	GLint tamanhoPonto;       // Indica o tamanho do ponto
} Figuras[TAM_MAX];       // Vetor de figuras

int n_figuras = 0;        // Numero de figuras atualmente
int n_limiteRefazer = 0;  // Numero maximo de figuras para refazer

Figura Atual;             // Figura que esta sendo desenhada atualmente

bool selecionada = false; // Indica se alguma figura foi encontrada no ponto indicado pra calcular a area
GLfloat xSel = 0;         // Coordenada X do ponto em que se deseja pegar uma figura para calcular a area
GLfloat ySel = 0;         // Coordenada Y do ponto em que se deseja pegar uma figura para calcular a area

int linha = 0;                      // Tipo de linha utilizado como continuo
int preenchido = VAZIO ;            // Preenchimento inicializado como VAZIO
GLint espessura = 1;                // Espessura da linha inicializada como o minimo
GLint tamanhoPonto = 4;             // Tamanho do ponto
GLint modoDesenho = 1;              // Modo de desenho inicializado

GLfloat win, r, g, b;               // Variaveis auxiliares ( janela, cor, etc )
GLint view_w, view_h, primitiva=-1;  // Variaveis auxiliares ( janela, cor, etc )

GLfloat fatorW = 13.0;              // Fator de ajuste horizontal da tela
GLfloat fatorH = 10.0;              // Fator de ajuste vertical da tela

bool salvar = false;      // Indica que a opcao salvar nao foi selecionada         
bool carregar = false;    // Indica que a opcao carregar nao foi selecionada
bool sair = false;        // Indica que a opcao sair nao foi selecionada
bool limparTudo = false;  // Indica que a opcao limpar tudo nao foi selecionada
bool first = true;        // Evita que seja feito um primeiro desenho com valores desconhecidos de X2 e Y2
bool evitaBug = false;    // Evita inserir imagem no array quando clicar para escolher uma cor
bool desenhando = false;  // Indica se alguma figura esta sendo desenhada (se o botao esquerdo do mouse esta pressionado)


// ############################### FUNCOES UTILIZADAS NO SISTEMA ################################

// Calcula a area da figura selecionada
void calcularArea (Figura sel);

// Funcao responsavel por limpar a tela
void clear ();

// Criacao do Menu
void CriaMenu();

// Desenha um retangulo preenchido na tela dados os pontos inicial e final e a cor corrente
void desenhaRetanguloFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um retangulo vazio na tela dados os pontos inicial e final e a cor corrente
void desenhaRetanguloEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um triangulo na tela dados os pontos inicial e final
void desenhaTrianguloFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um triangulo na tela dados os pontos inicial e final
void desenhaTrianguloEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha uma reta a partir das coordenadas de dois pontos
void desenhaReta( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha uma seta a partir dos seus quatro pontos
void desenhaSeta( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);

// Desenha duas retas que se cruzam
void desenhaX( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha duas retas perpendiculares
void desenhaCruz( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um circulo preenchido
void desenhaCirculoFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um circulo vazio
void desenhaCirculoEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

// Desenha um poligono regular com o numero de vertices dado tendo como centro o primeiro ponto
void desenhaPoligonoEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 , int points  );

// Desenha um poligono preenchido
void desenhaPoligonoFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 , int points  );

// Desenha um ponto com as coordenadas  x e y e o tamanho 6.0
void desenhaPonto( GLfloat x, GLfloat y, GLfloat tamanho );

// Desfaz o desenho de uma figura ou a acao de limpar a tela
void desfazer ();

// Desenha as figuras na tela
void display(void);

// Callback para gerenciar cliques do mouse
void HandleMouse(int button, int state, int x, int y);

void menuPontos ( void );

void menuPreenchimento ( void );

void menuLinhas ( void );

// Determina o ponto final do retangulo sendo desenhado no momento.
// Esta funcao eh rotulada como "glutMotionFunc", que eh chamada
// sempre que o mouse eh movido com algum botao pressionado.
void HandleMouseMotion(int x, int y);

void HandleMouseMotion2(int x, int y);

// Funcao init
void init (void) ;

// Funcao utilizada para carregar uma figura salva anteriormente
void load ();

// Processamento das opcoes de load
void MenuCarregar(int op);

// Define a forma como o menu sera desenhado na tela
void menuCor ( void );

// Utiliza teclado
void HandleKeyboard(unsigned char key, int x, int y);

// Desenho do menu de contorno
void menuContorno ( void );

// Desenha o menu de Opcoes
void menuOpcoes ( void );

// Gerenciamento do menu com as opções de cores           
void MenuPrimitiva(int op);

// Funcao responsavel por desenhar o menu de primitivas
void menuPrimitivas ( void );

// Gerenciamento do menu principal           
void MenuPrincipal(int op);

// Processamento das opcoes de salvamento
void MenuSalvar(int op);

// Indica se o ponto selecionado pertence a alguma figura desenhada
bool pertence ( GLfloat x, GLfloat y, Figura F);

// Refaz o desenho de uma figura
void refazer ();

// Chamada pela GLUT quando a janela eh redimensionada.
// O codigo a seguir altera o sistema de coordenadas sempre
// que a janela eh redimensionada, mantendo as dimensoes dos objetos.
void reshape(int w, int h);

// Funcao utilizada para salvar as imagens desenhadas atualmente na tela
void save ();

// Seleciona a ultima figura desenhada que contem o ponto indicado
void selecionaFigura ( GLfloat x, GLfloat y );

// Troca os valores de duas variaveis do tipo GLfloat
void trocaGLfloat ( GLfloat &A, GLfloat &B );


// ######################################## MAIN #############################################


/*  Define o tamanho inicial e a posição  da janela e o modo de "display" 
 *  (single buffer e RGB).  Abre a janela com o título Retangulos 
 *  Chama as rotinas de inicialização  
 *  Registra as funções de callback.
 *  Entra no loop principal e processa os eventos.
 */

// Funcao principal. Inicia a execucao do programa
int main(int argc, char** argv){
   preenchido = !preenchido; // Muda o valor da variavel para desenhar figuras vazias	
   cout << "\n\n\tSeja bem vindo ao FreePaint !" << endl;
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (window_w, window_h); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("FreePaint");
   init ();
   glutKeyboardFunc(HandleKeyboard);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(HandleMouse);
   glutMotionFunc(HandleMouseMotion);
   glutPassiveMotionFunc(HandleMouseMotion2);
      
   glutMainLoop();
   return 0;   
}


// ############################# IMPLEMENTACAO DAS FUNCOES ######################################


// Define a forma como o menu sera desenhado na tela
void menuCor ( void ){
   for(int i=0; i<10; i++ ) {
       // Desenha na cor escolhida
	   switch(i) {
	      case MARROM:
	    	   USANDO_MARROM;
	    	   break;
	    	case CINZA: 
	    	   USANDO_CINZA;
	    	   break;
	    	case 2: 
	    	   USANDO_BRANCO;
	    	   break;
	    	case 3:
			   USANDO_ROSA;
	    	   break;
	    	case 4: 
	    	   USANDO_AZUL_CLARO;
	    	   break;
	    	case 5: 
	    	   USANDO_AMARELO;
	    	   break;
	    	case 6: 
	    	   USANDO_AZUL;
	    	   break;
	    	case 7: 
	    	   USANDO_VERDE;
	    	   break;
	    	case 8: 
	    	   USANDO_VERMELHO;
	    	   break;
	    	case 9: 
	    	   USANDO_PRETO;
	    	   break;
		    default:
		      USANDO_PRETO;
	   }
	    
	   // Desenha os retangulos
	   if ( r == r2 && g == g2 && b == b2 ){
	      USANDO_COR_FUNDO;
	    	desenhaRetanguloFull( 0.0, 0.0 + window_h / fatorH * i, 
            window_w / fatorW , window_h / fatorH + window_h /fatorH * i );
		     
		   glColor3f (r2, g2, b2);
		   
         desenhaRetanguloFull( 10.0, 0.0 + window_h / fatorH * i + 10, 
            window_w / fatorW - 10 , window_h / fatorH + window_h /fatorH * i - 10 );
	   }
	    
	   else
		   desenhaRetanguloFull( 0.0, 0.0 + window_h / fatorH * i, 
            window_w / fatorW , window_h / fatorH + window_h /fatorH * i );
   }
    
   glColor3f (0.0, 0.0, 0.0);

   // Desenha os retangulos que formam a grade
   for( int i=9; i>=0; i-- ) {	    	   
	   glLineWidth(4.0f);
      
		desenhaRetanguloEmpty( 0.0, 0.0 + window_h / fatorH * i, 
         window_w / fatorW , window_h / fatorH + window_h /fatorH * i );
		
      glLineWidth(1.0f);
	}
}

// Desenha um retangulo preenchido na tela dados os pontos inicial e final e a cor corrente
void desenhaRetanguloFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
	
	// Troca valores das coordenadas para facilitar o desenho
  	if ( x2 < x1 ) trocaGLfloat ( x1, x2 );
  	if ( y2 > y1 ) trocaGLfloat ( y1, y2 );
  	
	// Poligono preenchido
	glBegin(GL_POLYGON);
	  	glVertex2f(x1,y1);
	  	glVertex2f(x2,y1);
	  	glVertex2f(x2,y2);
	  	glVertex2f(x1,y2);
	glEnd();  
}

// Menu de opcoes
void menuOpcoes ( void ){

	// Plano de fundo
	glColor3f (0.9, 0.9, 0.9);
	for(int i=0; i<12; i++ ) {  
	   if ( i == 0 && option == PRIMITIVA )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 1 && option == PREENCHIMENTO )	glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 2 && option == CONTORNO )	    glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 3 && option == LIVRE )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 4 && option == LINHAS )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 5 && option == PONTOS )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 6 && option == DESFAZER )	    glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 7 && option == REFAZER )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 8 && option == LIMPAR )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 9 && option == AREA )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 10 && option == LOAD )	        glColor3f (0.6, 0.6, 0.6);
	   else if ( i == 11 && option == STORE )	        glColor3f (0.6, 0.6, 0.6);
	   
      // Desenha cor de fundo normal ( nao selecionado )		
		else glColor3f (0.9, 0.9, 0.9);	
		desenhaRetanguloFull( window_w / fatorW + window_w / fatorW * i, window_h, 
         2 * window_w / fatorW + window_w/ fatorW * i, window_h - window_h/10 );
	}
   
	// Desenha grade usando retangulos nao preenchidos
	glColor3f (0.0, 0.0, 0.0);
	for(int i=0; i<12; i++ ) { 
	   // Desenha os retangulos
	   glLineWidth(4.0f);
         
      desenhaRetanguloEmpty( window_w / fatorW + window_w / fatorW * i, window_h, 
         2 * window_w / fatorW + window_w/ fatorW * i, window_h - window_h/10 );
		
      glLineWidth(1.0f);	
		
		if ( i == PRIMITIVA ){
			
			glLineWidth(6.0f);
					
			desenhaPoligonoFull ( window_w / fatorW + i *window_w /fatorW + window_w / fatorW/2, window_h - window_h/fatorH/2,
            window_w / fatorW + i *window_w /fatorW + window_w / fatorW/2, window_h - 5, 6 );
			
			glLineWidth(1.0f);
			
		}	
		
		if ( i == PREENCHIMENTO ){
			glLineWidth(6.0f);
			
			desenhaRetanguloEmpty ( window_w / fatorW + i *window_w /fatorW + 10, window_h - 10,
            window_w / fatorW + (i +1) *window_w /fatorW - 10, window_h -window_h/fatorH + 10 );
			
			desenhaTrianguloFull ( window_w / fatorW + i *window_w /fatorW + 10, window_h - 10,
            window_w / fatorW + (i +1) *window_w /fatorW - 10, window_h -window_h/fatorH + 10 );

			glLineWidth(1.0f);
		}	
		
		
		if ( i == CONTORNO ){
			glLineWidth(6.0f);
			
			// Continua
			/*
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10, window_h-window_h/fatorH/4,
              window_w / fatorW + (i +1) *window_w /fatorW - 10, window_h-window_h/fatorH/4);
			*/
			
			// Tracejada
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10, window_h-2*window_h/fatorH/4 ,
				window_w / fatorW + i *window_w /fatorW + 10 + window_w /fatorW/7 ,window_h-2*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 2*window_w /fatorW/7, window_h-2*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 3*window_w /fatorW/7 ,window_h-2*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 4*window_w /fatorW/7, window_h-2*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 5*window_w /fatorW/7 ,window_h-2*window_h/fatorH/4 );
			
			// Pontilhada
			 /*
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10, window_h-3*window_h/fatorH/4 ,
				window_w / fatorW + i *window_w /fatorW + 10 + window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 2*window_w /fatorW/15, window_h-3*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 3*window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 4*window_w /fatorW/15, window_h-3*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 5*window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 6*window_w /fatorW/15, window_h-3*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 7*window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
		
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 8*window_w /fatorW/15, window_h-3*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 9*window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10 + 10*window_w /fatorW/15, window_h-3*window_h/fatorH/4,
				window_w / fatorW + i *window_w /fatorW + 10 + 11*window_w /fatorW/15 ,window_h-3*window_h/fatorH/4 );
            */
			glLineWidth(1.0f);
		}	
		
		if ( i == LIVRE ){
			glLineWidth(6.0f);
         
            desenhaReta ( window_w / fatorW + i *window_w /fatorW + 10, window_h - 10,
               window_w / fatorW + (i +1) *window_w /fatorW - window_w /fatorW/1.7, window_h -window_h/fatorH + 10 );
			
			desenhaReta ( window_w / fatorW + (i +1) *window_w /fatorW - window_w /fatorW/1.7, window_h -window_h/fatorH + 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/2, window_h - 10);
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + window_w /fatorW/2, window_h - 10,
            window_w / fatorW + (i +1) *window_w /fatorW - 10, window_h -window_h/fatorH + 10  );
		
			glLineWidth(1.0f);
		}				
		 
		if ( i == LINHAS ){
			
			glLineWidth(6.0f);
		    desenhaReta ( window_w / fatorW + window_w / fatorW * i + 10, window_h - window_h/fatorH/2,
			   window_w / fatorW + window_w / fatorW * (i + 1) -10, window_h - window_h/fatorH/2);
	    	glLineWidth(1.0f);
                       
			
		} 
		
		if ( i == PONTOS ){
			glLineWidth(6.0f);
			
			desenhaPonto ( window_w / fatorW + window_w / fatorW * i + window_w / fatorW/2, window_h - window_h/fatorH/2, 8 );
                       
			glLineWidth(1.0f);
		} 
		 
		if ( i == DESFAZER ){
			glLineWidth(6.0f);
         
			desenhaSeta ( window_w / fatorW + window_w / fatorW * i + 10, window_h - window_h/fatorH + window_h/fatorH/2,
			   window_w / fatorW + window_w / fatorW * (i + 1) - 10, window_h - window_h/fatorH + window_h/fatorH/2,
			      window_w / fatorW + window_w / fatorW * (i + 1) - window_w / fatorW/2,window_h -  10,
		            window_w / fatorW + window_w / fatorW * (i + 1) - window_w / fatorW/2, window_h - window_h/fatorH + 10 ); 
                  
			glLineWidth(1.0f);
		}
		
		if ( i == REFAZER ){
			glLineWidth(6.0f);
         
			desenhaSeta ( window_w / fatorW + window_w / fatorW * (i + 1) - 10, window_h - window_h/fatorH + window_h/fatorH/2,
            window_w / fatorW + window_w / fatorW * i + 10, window_h - window_h/fatorH + window_h/fatorH/2,
			      window_w / fatorW + window_w / fatorW * (i + 1) - window_w / fatorW/2, window_h -  10,
		            window_w / fatorW + window_w / fatorW * (i + 1) - window_w / fatorW/2, window_h - window_h/fatorH + 10  );
			
         glLineWidth(1.0f);
		}
		
		if ( i == LIMPAR ){
			glLineWidth(6.0f);
         
			desenhaX ( window_w / fatorW + window_w / fatorW * (i) + 10, window_h - window_h/fatorH + 10,
			   2 * window_w / fatorW + window_w / fatorW * (i) - 10, window_h - 10 );
            
			glLineWidth(1.0f);
		}
      
		if ( i == AREA ){
			glLineWidth(6.0f);
			
			desenhaReta ( window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h -window_h/fatorH + 10);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - 10,
            window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h -window_h/fatorH + 10);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - window_h/fatorH/2,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - window_h/fatorH/2);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - 10);
	
         glColor3f (0.0, 0.0, 0.0);
		}	
		
		if ( i == LOAD ){
			glLineWidth(6.0f);
			
			
		desenhaReta ( window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - 10,
           window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h -window_h/fatorH + 10);
           
        desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - window_h/fatorH+10,
           window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h -window_h/fatorH + 10);
            
		   glLineWidth(1.0f);
		}
		
		if ( i == STORE ){
			glLineWidth(6.0f);
         
		 desenhaReta ( window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h -window_h/fatorH/2);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h -window_h/fatorH/2,
            window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h -window_h/fatorH + 10);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - window_h/fatorH/2,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - window_h/fatorH/2);
            
         desenhaReta ( window_w / fatorW + (i+1 ) *window_w /fatorW - window_w /fatorW/4, window_h - 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h - 10);
            
         desenhaReta ( window_w / fatorW + (i+1)*window_w /fatorW - window_w /fatorW/4, window_h -window_h/fatorH + 10,
            window_w / fatorW + i *window_w /fatorW + window_w /fatorW/4, window_h -window_h/fatorH + 10);
                  
			glLineWidth(1.0f);
		}
	}	
}

// Menu salvar
void MenuSalvar(int op){
   switch(op) {
      case 0: file = SAVE_0; break;
      case 1: file = SAVE_1; break;
      case 2: file = SAVE_2; break;
      case 3: file = SAVE_3; break;
      case 4: file = SAVE_4; break;
      case 5: file = SAVE_5; break;
      case 6: file = SAVE_6; break;
   }
   save();
   glutPostRedisplay();
}   

// Determina o ponto final do retangulo sendo desenhado no momento.
// Esta funcao eh rotulada como "glutMotionFunc", que eh chamada
// sempre que o mouse eh movido com algum botao pressionado.
void HandleMouseMotion(int x, int y) {
/* Os valores de x e y retornados pelo clique do mouse 
   sao tomados em relacao ao canto superior esquerdo da 
   janela e correspondem ao numero de pixels na janela,
	e portanto precisam ser ajustados para o sistema de coordenadas
	definido para o desenho. 
	Neste caso, como a funcao reshape ajusta o sistema
	de coordenadas de acordo com a janela, basta inverter o
	valor de y, considerando a origem no canto inferior 
	esquerdo da janela. Obs: em outras aplicacoes este ajuste
	pode se tornar um pouco mais complexo. */

	y = window_h - y;
	
	// Atualiza o valor das coordenadas do ponto 2 com as coordenadas do ponto atual	
	if (desenhando) {
		Atual.x2 = x;
		Atual.y2 = y;
		
		if ( Atual.x2 < window_w / fatorW )
		   Atual.x2 = window_w / fatorW;
		if ( Atual.y2 < 0 )
		   Atual.y2 = 0;
	}	

	glutPostRedisplay(); // Chama a funcao display
}

// dgsdfgfdgfdhfdhghgfhsfadawfds
void HandleMouseMotion2(int x, int y) {
	y = window_h - y;
	
	xSel = x;
	ySel = y;
	
	if ( existePontoZero == false ){
		xZero = xSel;
		yZero = ySel;
		
		existePontoZero = true;
	}
	
	// Se o modo de desenho livre estiver selecionado
	if ( desenhaLIVRE == true && option == LIVRE ){
		
		// Se houver uma distancia minima entre os pontos inicial e final
		if ( sqrt ( (xZero - xSel)*(xZero - xSel) +  (yZero - ySel)*(yZero - ySel)) > 2 ){
		
			Figuras[n_figuras].tipo = RETA;
			Figuras[n_figuras].x1 = xZero;
			Figuras[n_figuras].y1 = yZero;
			Figuras[n_figuras].x2 = xSel;
			Figuras[n_figuras].y2 = ySel;
					   
			Figuras[n_figuras].R = r;
			Figuras[n_figuras].G = g;
			Figuras[n_figuras].B = b;
				  	   
			Figuras[n_figuras].linha = CONTINUO;
			Figuras[n_figuras].espessura = espessura;
			Figuras[n_figuras].tamanhoPonto = 0;
			Figuras[n_figuras].preenchido = 0;
			
			n_figuras++;
			n_limiteRefazer = n_figuras;
			limparTudo = false;
				    
			first = true;
			
			// Modifica ponto inicial para a proxima reta
			xZero = xSel;
			yZero = ySel;	
		}	  	
	}
	
    
	glutPostRedisplay(); // Chama a funcao display
}

// Troca os valores de duas variaveis do tipo GLfloat
void trocaGLfloat ( GLfloat &A, GLfloat &B ){
	GLfloat aux = A;
	A = B;
	B = aux;
}

// Desenha um retangulo vazio na tela dados os pontos inicial e final e a cor corrente
void desenhaRetanguloEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
  	
	// Troca valores das coordenadas para facilitar o desenho
  	if ( x2 < x1 ) trocaGLfloat ( x1, x2 );
  	if ( y2 > y1 ) trocaGLfloat ( y1, y2 );
  	
	// Poligono vazio
	glBegin(GL_LINE_STRIP);
	glPolygonMode (GL_FRONT, GL_LINE);
	  	glVertex2f(x1,y1);
	  	glVertex2f(x2,y1);
	  	glVertex2f(x2,y2);
	  	glVertex2f(x1,y2);
	  	glVertex2f(x1,y1);
	glEnd();  
}

// Desenha um triangulo na tela dados os pontos inicial e final
void desenhaTrianguloFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
  	// Desenha um retângulo com a cor corrente
  	
  	if ( x2 < x1 ) trocaGLfloat ( x1, x2 );
  	if ( y2 > y1 ) trocaGLfloat ( y1, y2 );
  	
	glBegin (GL_POLYGON);
	  	glVertex2f(x1,y1);
	  	glVertex2f(x2,y1);
	  	glVertex2f(x2,y2);
	glEnd();
}

// Desenha um triangulo na tela dados os pontos inicial e final
void desenhaTrianguloEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
  	// Desenha um retângulo com a cor corrente
  	
  	if ( x2 < x1 ) trocaGLfloat ( x1, x2 );
  	if ( y2 > y1 ) trocaGLfloat ( y1, y2 );
  	
	glBegin(GL_LINE_STRIP);
	glPolygonMode (GL_FRONT, GL_LINE);
	  	glVertex2f(x1,y1);
	  	glVertex2f(x2,y1);
	  	glVertex2f(x2,y2);
	  	glVertex2f(x1,y1);
	glEnd();   
}

// Desenha uma reta a partir das coordenadas de dois pontos
void desenhaReta( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
	glBegin(GL_LINES);
	// coordenadas inicial e final da linha
      glVertex2f(x1,y1);  
	   glVertex2f(x2,y2);    
	glEnd(); 
}

// Desenha uma seta a partir das coordenadas dos quatro pontos que a formam
void desenhaSeta( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4){
  	desenhaReta ( x1, y1, x2, y2 );
  	desenhaReta ( x1, y1, x3, y3 );
  	desenhaReta ( x1, y1, x4, y4 );
}

// Desenha duas retas cruzadas 
void desenhaX( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
  	desenhaReta ( x1, y1, x2, y2 );
  	desenhaReta ( x1, y2, x2, y1 );
}

// Desenha duas retas perpendiculares
void desenhaCruz( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
  	desenhaReta ( x1, y1, x1 + 2*(x2-x1), y1 );
  	desenhaReta ( x2, y2, x2, y2 - 2*(y2-y1) );
}

// Desenha um circulo preenchido
void desenhaCirculoFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
	GLint circle_points = 50; 
	GLfloat angle;
	GLfloat raio;
	
	raio = sqrt ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	
	glTranslatef(x1,y1,0);
	glBegin(GL_POLYGON);               	    
	for (int j = 0; j <= circle_points; j++) {    
		angle = 2*PI*j/circle_points; 
		glVertex2f(raio * cos(angle), raio * sin(angle)); 
	
	} 
	glEnd();
	glTranslatef(-x1,-y1,0);
}

// Desenha um circulo apenas com as bordas
void desenhaCirculoEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 ){
	GLint circle_points = 50; 
	GLfloat angle;
	GLfloat raio;
	
	raio = sqrt ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	
	glTranslatef(x1,y1,0);
	glBegin(GL_LINE_STRIP);
	glPolygonMode (GL_FRONT, GL_LINE);               	    
	for (int j = 0; j <= circle_points; j++) {    
		angle = 2*PI*j/circle_points; 
		glVertex2f(raio * cos(angle), raio * sin(angle)); 
	
	} 
	glEnd();
	glTranslatef(-x1,-y1,0);
}

// Desenha um ponto com as coordenadas  x e y e o tamanho 6.0
void desenhaPonto( GLfloat x, GLfloat y, GLfloat tamanho  ){
	glPointSize(tamanho);
	glBegin(GL_POINTS);
      glVertex2f(x, y); 
   glEnd();
}

// Desenha um poligono regular com o numero de vertices dado tendo como centro o primeiro ponto
void desenhaPoligonoEmpty( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 , int points  ){
	GLint circle_points = points; 
	GLfloat angle;
	GLfloat raio;
	
	raio = sqrt ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	
	glTranslatef(x1,y1,0);
	glBegin(GL_LINE_STRIP);
	glPolygonMode (GL_FRONT, GL_LINE);               	    
	for (int j = 0; j <= circle_points; j++) {    
		angle = 2*PI*j/circle_points; 
		glVertex2f(raio * cos(angle), raio * sin(angle)); 
	
	} 
	
	glEnd();
	glTranslatef(-x1,-y1,0);
}

// Desenha um poligono de n vertices preenchido
void desenhaPoligonoFull( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 , int points  ){
	GLint circle_points = points; 
	GLfloat angle;
	GLfloat raio;
	
	raio = sqrt ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	
	glTranslatef(x1,y1,0);
	glBegin(GL_POLYGON);                	    
	for (int j = 0; j <= circle_points; j++) {    
		angle = 2*PI*j/circle_points; 
		glVertex2f(raio * cos(angle), raio * sin(angle)); 
	
	} 
	
	glEnd();
	glTranslatef(-x1,-y1,0);
}

// Desfaz o desenho da ultima figura ou a acao de limpar a tela 
void desfazer (){
	if ( limparTudo == true ){               
       n_figuras = n_limiteRefazer;
       limparTudo = false;
    }
               
	else if ( n_figuras > 0 )
       n_figuras--;
}

// Refaz uma figura desfeita
void refazer (){
    if ( n_figuras < n_limiteRefazer)
        n_figuras++;
}

// Funcao responsavel por limpar a tela
void clear (){
   // Significa que nao existem figuras para serem desenhadas
   n_figuras = 0; 
	
   // Indica que a tela foi limpa. Isto eh util para desfazer a limpeza 	
   limparTudo = true;
}

// Funcao utilizada para salvar as imagens desenhadas atualmente na tela
void save (){
	
	ofstream arquivo;
	
	// Seleciona o arquivo correto
	switch ( file ){
	   case SAVE_0:
	      arquivo.open ( "save.txt");
	      break;
	   case SAVE_1:
	      arquivo.open ( "save1.txt");
	      break;
	   case SAVE_2:
	      arquivo.open ( "save2.txt");
	      break;
	   case SAVE_3:
	      arquivo.open ( "save3.txt");
	      break;
	   case SAVE_4:
	      arquivo.open ( "save4.txt");
	      break;
	   case SAVE_5:
	      arquivo.open ( "save5.txt");
	      break;
	   case SAVE_6:
	      arquivo.open ( "save6.txt");
	      break;
	}
	
	if ( arquivo.fail() ){
		//printf("\n\n\tFalha na abertura do arquivo !");
		cout << "\n\n\tFalha na abertura do arquivo !" << endl;
	}
		
	arquivo << n_figuras;
	arquivo << "\n\n";
	for ( int i=0; i<n_figuras; i++ ){
		arquivo << Figuras[i].tipo << " ";
		arquivo << Figuras[i].x1 << " ";
		arquivo << Figuras[i].x2 << " ";
		arquivo << Figuras[i].y1 << " ";
		arquivo << Figuras[i].y2 << " ";
		arquivo << Figuras[i].R << " ";
		arquivo << Figuras[i].G << " ";
		arquivo << Figuras[i].B<< " ";
		arquivo << Figuras[i].espessura << " ";
		arquivo << Figuras[i].tamanhoPonto << " ";
		arquivo << Figuras[i].linha << " ";
		arquivo << Figuras[i].preenchido << " ";
		arquivo << "\n\n";
	}
	   
	arquivo.close ( );
}

// Funcao utilizada para carregar uma figura salva anteriormente
void load (){
	
	ifstream arquivo;
	
	// Seleciona o arquivo correto
	switch ( file ){
	   case SAVE_0:
	      arquivo.open ( "save.txt");
	      break;
	   case SAVE_1:
	      arquivo.open ( "save1.txt");
	      break;
	   case SAVE_2:
	      arquivo.open ( "save2.txt");
	      break;
	   case SAVE_3:
	      arquivo.open ( "save3.txt");
	      break;
	   case SAVE_4:
	      arquivo.open ( "save4.txt");
	      break;
	   case SAVE_5:
	      arquivo.open ( "save5.txt");
	      break;
	   case SAVE_6:
	      arquivo.open ( "save6.txt");
	      break;
	}
	
	if ( arquivo.fail() ){
		//printf("\n\n\tFalha na abertura do arquivo !");
		cout << "\n\n\tFalha na abertura do arquivo !" << endl;
	}
	
	arquivo >> n_figuras;
	for ( int i=0; i<n_figuras; i++ ){
		arquivo >> Figuras[i].tipo ;
		arquivo >> Figuras[i].x1 ;
		arquivo >> Figuras[i].x2 ;
		arquivo >> Figuras[i].y1 ;
		arquivo >> Figuras[i].y2 ;
		arquivo >> Figuras[i].R ;
		arquivo >> Figuras[i].G ;
		arquivo >> Figuras[i].B;
		arquivo >> Figuras[i].espessura ;
		arquivo >> Figuras[i].tamanhoPonto ;
		arquivo >> Figuras[i].linha ;
		arquivo >> Figuras[i].preenchido ;
	}
	arquivo.close ( ); 
}



// Calcula a area da figura selecionada
void calcularArea (Figura F){
	GLfloat area = 0;
	GLfloat Raio;
	
	if ( F.tipo == RETANGULO ){
		area = abs( F.x2 - F.x1 ) * abs( F.y2 - F.y1 );
		cout << "\n\n\tArea do retangulo = " << area << endl;
		//printf ( "\n\n\tArea do retangulo = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == CIRCULO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = PI * Raio * Raio;
		cout << "\n\n\tArea do circulo = " << area << endl;
		//printf ( "\n\n\tArea do circulo = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == TRIANGULO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = 3 * 0.5 * Raio*Raio * sin (120/57.3);
		cout << "\n\n\tArea do triangulo = " << area << endl;
		//printf ( "\n\n\tArea do triangulo = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == LOSANGO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = 4 * 0.5 * Raio*Raio * sin (90/57.3);
		cout << "\n\n\tArea do losango = " << area << endl;
		//printf ( "\n\n\tArea do losango = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == PENTAGONO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = 5 * 0.5 * Raio*Raio * sin (72/57.3);
		cout << "\n\n\tArea do pentagono = " << area << endl;
		//printf ( "\n\n\tArea do pentagono = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == HEXAGONO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = 6 * 0.5 * Raio*Raio * sin (60/57.3);
		cout << "\n\n\tArea do hexagono = " << area << endl;
		//printf ( "\n\n\tArea do hexagono = ");
		//printf ( "%f", area);
	}
	
	else if ( F.tipo == HEPTAGONO ){
		Raio = sqrt ( ( F.x2 - F.x1 ) *( F.x2 - F.x1 ) + ( F.y2 - F.y1 )*( F.y2 - F.y1 ));
		area = 7 * 0.5 * Raio*Raio * sin (51/57.3);
	    cout << "\n\n\tArea do heptagono = " << area << endl;
		//printf ( "\n\n\tArea do heptagono = ");
		//printf ( "%f", area);
	}
	else
	   cout << "\n\n\tNenhum objeto encontrado !" << endl;	
	   //printf ( "\n\n\tNenhum objeto encontrado !");
}

// Indica se o ponto selecionado pertence a alguma figura desenhada
bool pertence ( GLfloat x, GLfloat y, Figura F){
	
	if ( F.tipo == RETANGULO ){
		
		if ( F.x2 < F.x1 ) trocaGLfloat ( F.x2 , F.x1 );
		
		if ( F.y2 < F.y1 ) trocaGLfloat ( F.y2 , F.y1 );
		
		if ( x >= F.x1 && x <= F.x2 && y >= F.y1 && y <= F.y2)
		   return true;
		
		else
		   return false;
	}
	
	else if ( F.tipo == CIRCULO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	
	else if ( F.tipo == TRIANGULO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	
	else if ( F.tipo == LOSANGO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	else if ( F.tipo == PENTAGONO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	else if ( F.tipo == HEXAGONO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	else if ( F.tipo == HEPTAGONO ){
		if ( sqrt ((x - F.x1)*(x - F.x1) + (y - F.y1)*(y - F.y1)) < sqrt ((F.x2 - F.x1)*(F.x2 - F.x1) + (F.y2 - F.y1)*(F.y2 - F.y1))){
		   return true;
		}
		else
		   return false;
	}
	
	else return false;
	
}

// Seleciona a ultima figura desenhada que contem o ponto indicado
void selecionaFigura ( GLfloat x, GLfloat y ){
	selecionada = false;
	for ( int i = n_figuras-1; i >= 0; i-- ){
		if ( pertence ( x, y, Figuras[i])){
		   calcularArea (Figuras[i]);
		   selecionada = true;
		   break;
	    }
	}
	
	if ( selecionada == false ){
	   cout << "\n\n\tNenhum objeto encontrado !" << endl;
	   //printf ( "\n\n\tNenhum objeto encontrado !");
	}
	
	//glFlush();
}

// Gerenciamento do menu com as opções de cores           
void MenuPrimitiva(int op){
   switch(op) {
            case 0:
                     primitiva = RETANGULO;
                     break;
            case 1:
                     primitiva = RETA;
                     break;
            case 2:
                     primitiva = PONTO;
                     break;
            case 3:
                     primitiva = CIRCULO;
                     break;

    }
    glutPostRedisplay();
}   

// Gerenciamento das opcoes de load dos desenhos
void MenuCarregar(int op){
   switch(op) {
            case 0:
                     file = SAVE_0;
                     break;
            case 1:
                     file = SAVE_1;
                     break;
            case 2:
                     file = SAVE_2;
                     break;
            case 3:
                     file = SAVE_3;
                     break;
            case 4:
                     file = SAVE_4;
                     break;
            case 5:
                     file = SAVE_5;
                     break;
            case 6:
                     file = SAVE_6;
                     break;

    }
    load();
    glutPostRedisplay();
}   

// Gerenciamento do menu principal           
void MenuPrincipal(int op){
   switch(op) {
            case 0:
                     desfazer ();
                    
                     break;
            case 1:
                     refazer ();
                     
                     break;
            case 2:
                     clear ();
                     
                     break;
            case 3:
                     exit (0); 
					                  
                     break;
                        
			case 4:
				 selecionaFigura ( xSel, ySel );
             break;
    }
    glutPostRedisplay();
}  

// Criacao do Menu
void CriaMenu() {
    int menu,submenu1,submenu2,submenu3;

    submenu1 = glutCreateMenu(MenuPrimitiva);
    glutAddMenuEntry("Retangulo",0);
    glutAddMenuEntry("Reta",1);
    glutAddMenuEntry("Ponto",2);
    glutAddMenuEntry("Circulo",3);

    submenu2 = glutCreateMenu(MenuSalvar);
    glutAddMenuEntry("Save 1",0);
    glutAddMenuEntry("Save 2",1);
    glutAddMenuEntry("Save 3",2);
    glutAddMenuEntry("Save 4",3);
    glutAddMenuEntry("Save 5",4);
    glutAddMenuEntry("Save 6",5);
    
    submenu3 = glutCreateMenu(MenuCarregar);
    glutAddMenuEntry("Load 1",0);
    glutAddMenuEntry("Load 2",1);
    glutAddMenuEntry("Load 3",2);
    glutAddMenuEntry("Load 4",3);
    glutAddMenuEntry("Load 5",4);
    glutAddMenuEntry("Load 6",5);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Desenhar",submenu1);
    glutAddMenuEntry("Desfazer",0);
    glutAddMenuEntry("Refazer",1);
    glutAddMenuEntry("Limpar tudo",2);
    glutAddMenuEntry("Sair",3);
    glutAddMenuEntry("Calcular Área",4);
    glutAddSubMenu("Salvar",submenu2);
    glutAddSubMenu("Carregar",submenu3);   
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);    
    glFlush();
    
}

// Funcao responsavel pela inicializacao
void init (void) {
    // Define a cor de background da janela 
    glClearColor (1.0, 1.0, 1.0, 0.0);

    // Define o sistema de visualização - tipo de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_w, 0.0, window_h, -1.0, 1.0);
    
    //strcpy(texto, "(0,0)");
    
    CriaMenu();
}

// Callback para gerenciar cliques do mouse
void HandleMouse(int button, int state, int x, int y){
	
    // Ajusta as coordenadas do mouse 
    // (veja o comentario na funcao HandleMouseMotion)
	y = window_h - y;

    //switch (button){
    	//case GLUT_LEFT_BUTTON :
   if (button == GLUT_LEFT_BUTTON){
   	
		if (state == GLUT_DOWN) { // Se a acao for "apertar o botao do mouse"...
		
		   if ( option == AREA) selecionaFigura ( xSel, ySel );

		   if( x > 0 && x < window_w / fatorW ){
		   	  evitaBug = true;
		   	
		      if ( y > 0 && y < window_h / 10 ){
			     r = 0.7f; g = 0.5f; b = 0.3f;	
             }
             
             else if ( y > 0 && y < 2 * window_h / 10 ){
			     r = 0.4f; g = 0.4f; b = 0.4f;	
             }
             
             else if ( y > 0 && y < 3 * window_h / 10 ){
			     r = 1.0f; g = 1.0f; b = 1.0f;	
             }
             
             else if ( y > 0 && y < 4 * window_h / 10 ){
			     r = 1.0f; g = 0.0f; b = 1.0f;	
             }             
             
             else if ( y > 0 && y < 5 * window_h / 10 ){
			     r = 0.0f; g = 1.0f; b = 1.0f;	
             }  

             else if ( y > 0 && y < 6 * window_h / 10 ){
			     r = 1.0f; g = 1.0f; b = 0.0f;	
             }
             
             else if ( y > 0 && y < 7 * window_h / 10 ){
			     r = 0.0f; g = 0.0f; b = 1.0f;	
             }
             
             else if ( y > 0 && y < 8 * window_h / 10 ){
			     r = 0.0f; g = 1.0f; b = 0.0f;	
             }             
             
             else if ( y > 0 && y < 9 * window_h / 10 ){
			     r = 1.0f; g = 0.0f; b = 0.0f;	
             } 			            
		   	
             else if ( y > 0 && y < 10 * window_h / 10 ){
			     r = 0.0f; g = 0.0f; b = 0.0f;	
             } 		   	
		   }
		   
// **************************************************************************************
		
		   else if( y < window_h && y > window_h - window_h / fatorH ){
		   	  evitaBug = true;
		   	
		      if ( x > window_w / fatorW && x < 2 * window_w / fatorW ){
		      	 
		      	 if ( option == PRIMITIVA )
		      	     option = NADA;
				 else 
				     option = PRIMITIVA;
	
             }
             
             else if ( x > window_w / fatorW && x < 3 * window_w / fatorW  ){
             	 if ( option == PREENCHIMENTO )
             	    option = NADA;
             	    
             	 else{
             	    preenchido = !preenchido;
             	    option = PREENCHIMENTO;
                }
             }
             
             else if ( x > window_w / fatorW && x < 4 * window_w / fatorW ){
			       if ( option == CONTORNO ) 
			          option = NADA;
			       
					 else   
			          option = CONTORNO; 
             }
             
             else if ( x > window_w / fatorW && x < 5 * window_w / fatorW ){	
                if ( option == LIVRE ){
			          option = NADA;
			          desenhaLIVRE = false;
			         
				}
			          
			    else{			    
			    	option = LIVRE;
			    	primitiva = NADA;
			    }    
             }             
             
             else if ( x > window_w / fatorW && x < 6 * window_w / fatorW ){
             	 if ( option == LINHAS )
                   option = NADA;
                   
                else{
                   option = LINHAS;
                   primitiva = RETA;
               }
             }  

             else if ( x > window_w / fatorW && x < 7 * window_w / fatorW ){
                if ( option == PONTOS )
                   option = NADA;
                   
                else{
                   option = PONTOS; 
				   primitiva = PONTO;
		     	}
             }
             
             else if ( x > window_w / fatorW && x < 8 * window_w / fatorW ){
			     option = DESFAZER;
			     desfazer ();
             }
             
             else if ( x > window_w / fatorW && x < 9 * window_w / fatorW ){
			     option = REFAZER;
			     refazer ();
				 
             }             
             
             else if ( x > window_w / fatorW && x < 10 * window_w / fatorW ){
             	 option = LIMPAR;
			     clear ();	
             } 			            
		   	
             else if ( x > window_w / fatorW && x < 11 * window_w / fatorW ){
                if ( option == PONTOS )
                   option = AREA;
                   
                else
                   option = AREA; 
             } 	
			 else if ( x > window_w / fatorW && x < 12 * window_w / fatorW ){
			     option = LOAD;
			     file = SAVE_6;
			     load ();
             } 	 
			 else if ( x > window_w / fatorW && x < 13 * window_w / fatorW ){
			 	 option = STORE;
			 	 file = SAVE_6;
			 	 save ();	
             }   	
		   	
		   }
// ****************************************************************************************


           else if( x > window_w / fatorW && x < 2 * window_w / fatorW && option == PRIMITIVA ){
           	     evitaBug = true;
           		if ( y < window_h - window_h / fatorH && y > window_h - 2 * window_h / fatorH ){
           			primitiva = RETANGULO;
           			option = NADA;
           		}
           		           	
				else if ( y < window_h - 2 * window_h / fatorH && y > window_h - 3 * window_h / fatorH ){
           			primitiva = RETA;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 3 * window_h / fatorH && y > window_h - 4 * window_h / fatorH ){
           			primitiva = PONTO;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 4 * window_h / fatorH && y > window_h - 5 * window_h / fatorH ){
           			primitiva = CIRCULO;
           			option = NADA;
           		}

           		else if ( y < window_h - 5 * window_h / fatorH && y > window_h - 6 * window_h / fatorH ){
           			primitiva = TRIANGULO;
           			option = NADA;
           		}

           		else if ( y < window_h - 6 * window_h / fatorH && y > window_h - 7 * window_h / fatorH ){
           			primitiva = LOSANGO;
           			option = NADA;
           		}

           		else if ( y < window_h - 7 * window_h / fatorH && y > window_h - 8 * window_h / fatorH ){
           			primitiva = PENTAGONO;
           			option = NADA;
           		}

           		else if ( y < window_h - 8 * window_h / fatorH && y > window_h - 9 * window_h / fatorH ){
           			primitiva = HEXAGONO;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 9 * window_h / fatorH && y > window_h - 10 * window_h / fatorH ){
           			primitiva = HEPTAGONO;
           			option = NADA;
           		}
           }
           
           // Opcao de preenchimento
           else if( x > 2*window_w / fatorW && x < 3 * window_w / fatorW && option == PREENCHIMENTO ){
           	    evitaBug = true;
           		if ( y < window_h - window_h / fatorH && y > window_h - 2 * window_h / fatorH ){
           			preenchido = true;
           			option = NADA;
           		}
           		           	
				else if ( y < window_h - 2 * window_h / fatorH && y > window_h - 3 * window_h / fatorH ){
           			preenchido = false;
           			option = NADA;
           		}
           }
           
           // Opcao de contorno
           else if( x > 3*window_w / fatorW && x < 4 * window_w / fatorW && option == CONTORNO ){
           	     evitaBug = true;
           		if ( y < window_h - window_h / fatorH && y > window_h - 2 * window_h / fatorH ){
           			linha = CONTINUO;
           			option = NADA;
           		}
           		           	
				else if ( y < window_h - 2 * window_h / fatorH && y > window_h - 3 * window_h / fatorH ){
           			linha = TRACEJADO;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 3 * window_h / fatorH && y > window_h - 4 * window_h / fatorH ){
           			linha = PONTILHADO;
           			option = NADA;
           		}
           }
		   
		    // Opcoes de largura de linhas
		    else if( x > 5*window_w / fatorW && x < 6 * window_w / fatorW && option == LINHAS ){
           	     evitaBug = true;
           		if ( y < window_h - window_h / fatorH && y > window_h - 2 * window_h / fatorH ){
           			espessura = 1;
           			option = NADA;
           		}
           		           	
				else if ( y < window_h - 2 * window_h / fatorH && y > window_h - 3 * window_h / fatorH ){
           			espessura = 2;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 3 * window_h / fatorH && y > window_h - 4 * window_h / fatorH ){
           			espessura = 3;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 4 * window_h / fatorH && y > window_h - 5 * window_h / fatorH ){
           			espessura = 4;
           			option = NADA;
           		}

           		else if ( y < window_h - 5 * window_h / fatorH && y > window_h - 6 * window_h / fatorH ){
           			espessura = 5;
           			option = NADA;
           		}

           		else if ( y < window_h - 6 * window_h / fatorH && y > window_h - 7 * window_h / fatorH ){
           			espessura = 6;
           			option = NADA;
           		}
           }
           
           // Opcoes do tamanho do ponto
		    else if( x > 6*window_w / fatorW && x < 7 * window_w / fatorW && option == PONTOS ){
           	     evitaBug = true;
           		if ( y < window_h - window_h / fatorH && y > window_h - 2 * window_h / fatorH ){
           			tamanhoPonto = 3;
           			option = NADA;
           		}
           		           	
				else if ( y < window_h - 2 * window_h / fatorH && y > window_h - 3 * window_h / fatorH ){
           			tamanhoPonto = 4;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 3 * window_h / fatorH && y > window_h - 4 * window_h / fatorH ){
           			tamanhoPonto = 5;
           			option = NADA;
           		}
           		
           		else if ( y < window_h - 4 * window_h / fatorH && y > window_h - 5 * window_h / fatorH ){
           			tamanhoPonto = 6;
           			option = NADA;
           		}

           		else if ( y < window_h - 5 * window_h / fatorH && y > window_h - 6 * window_h / fatorH ){
           			tamanhoPonto = 7;
           			option = NADA;
           		}

           		else if ( y < window_h - 6 * window_h / fatorH && y > window_h - 7 * window_h / fatorH ){
           			tamanhoPonto = 8;
           			option = NADA;
           		}
           }
		   
		   else if ( x > window_w / fatorW && y < window_h - window_h / fatorH) {
              // Determina o ponto inicial do retangulo sendo desenhado
			  Atual.x1 = x;
		      Atual.y1 = y;

			  Atual.R = r;
			  Atual.G = g;
			  Atual.B = b;
			
			  Atual.tipo = primitiva;
			  Atual.preenchido = preenchido;
			  Atual.espessura = espessura;
			  Atual.tamanhoPonto = tamanhoPonto;
			  Atual.linha = linha;
			  
			  // Inicializa o ponto 2 com as coordenadas do ponto 1
			  // Isso impede que uma reta seja desenhada apenas com um click
			  // que usaria coordenadas lixo do Ponto 2
			 
		      Atual.x2 = x;
		      Atual.y2 = y;
			
			  desenhando = true;
			  
			  if ( desenhaLIVRE == true ){
				 desenhaLIVRE = false;
				 existePontoZero = false;
		      }
			
			  else if ( option == LIVRE ){
			     desenhaLIVRE = true;
		      }
			  

	       }

			  
			
		}
		else if (state == GLUT_UP) { // Se a acao for "soltar o botao do mouse"...

		    //desenhaLIVRE = false;
		    
		    existePontoZero = false;
		    
		    
		    if ( evitaBug == false ){
				// Finaliza o desenho do retangulo, adicionando o "atual" no vetor.
				desenhando = false;
				Atual.tipo = primitiva;
				Atual.preenchido = preenchido;
				Atual.espessura = espessura;
				Atual.tamanhoPonto = tamanhoPonto;
				Atual.linha = linha;
				
				
				   if (sqrt((Atual.x1 - Atual.x2)*(Atual.x1 - Atual.x2)+(Atual.y1 - Atual.y2)*(Atual.y1 - Atual.y2)) > 5 || primitiva == PONTO){
				      Figuras[n_figuras] = Atual;
				
				      n_figuras++;
				      n_limiteRefazer = n_figuras;
				      limparTudo = false;
			    
			          first = true;
				   }
		   }
		   evitaBug = false;
      }	
   } 
   glutPostRedisplay(); // Chama a funcao display
}
	
// Desenha as figuras na tela
void display(void) {
    // Limpa a janela
    glClear (GL_COLOR_BUFFER_BIT);   

    // Desenhar todos as figuras do vetor figuras[]
	for (int i=0; i<n_figuras; i++) {
		 
		 glLineWidth(Figuras[i].espessura);  
			
			if ( Figuras[i].linha == TRACEJADO ){
				 glEnable(GL_LINE_STIPPLE);
                 glLineStipple(2, 0xF0F0);
			}    
			else if ( Figuras[i].linha == PONTILHADO ){
				 glEnable(GL_LINE_STIPPLE);
                 glLineStipple(1, 0xF0F0);
			}  
			else{
				glDisable(GL_LINE_STIPPLE);
			}
			
			glColor3f (Figuras[i].R, Figuras[i].G, Figuras[i].B); 
		switch(Figuras[i].tipo) { 
		   case RETANGULO :  
            if (Figuras[i].preenchido)	      
		         desenhaRetanguloFull ( Figuras[i].x1, Figuras[i].y1 ,Figuras[i].x2 , Figuras[i].y2);
		      else
		           desenhaRetanguloEmpty ( Figuras[i].x1, Figuras[i].y1 ,Figuras[i].x2 , Figuras[i].y2);
		      break;
		      
		    case RETA :  
			  desenhaReta ( Figuras[i].x1,Figuras[i].y1 ,Figuras[i].x2 ,Figuras[i].y2 );
		      break;
		      
		    case PONTO :     
		      desenhaPonto( Figuras[i].x1, Figuras[i].y1, Figuras[i].tamanhoPonto );
		      break;
		      
		      
		    case CIRCULO :  
		      if (Figuras[i].preenchido)
		           desenhaCirculoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2 );
		      else
		           desenhaCirculoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2 );
              break;
               
            case TRIANGULO :
		      if (Figuras[i].preenchido)
		           desenhaPoligonoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 3 );
		      else
		           desenhaPoligonoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 3 );
              break;
              
            case LOSANGO :
		      if (Figuras[i].preenchido)
		           desenhaPoligonoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 4 );
		      else
		           desenhaPoligonoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 4 );
              break;
                        
			case PENTAGONO :
		      if (Figuras[i].preenchido)
		           desenhaPoligonoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 5 );
		      else
		           desenhaPoligonoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 5 );
              break;
              
            case HEXAGONO : 
		      if (Figuras[i].preenchido)
		           desenhaPoligonoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 6 );
		      else
		           desenhaPoligonoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 6 );
              break;
              
            case HEPTAGONO :
		      if (Figuras[i].preenchido)
		           desenhaPoligonoFull ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 7 );
		      else
		           desenhaPoligonoEmpty ( Figuras[i].x1, Figuras[i].y1,Figuras[i].x2 ,Figuras[i].y2, 7 );
              break;
        }   
		glDisable(GL_LINE_STIPPLE);	
		

	}
	
	// Se existir, desenhar a figura "atual"
	if (desenhando) {
        if ( first == true ){
        	first = false;
        }
        else {

		    if ( Atual.linha == TRACEJADO ){
				 glEnable(GL_LINE_STIPPLE);
                 glLineStipple(2, 0xF0F0);
			}    
			else if ( Atual.linha == PONTILHADO ){
				 glEnable(GL_LINE_STIPPLE);
                 glLineStipple(1, 0xF0F0);
			} 
			else{
				glDisable(GL_LINE_STIPPLE);
			}   
			
			glColor3f (Atual.R, Atual.G, Atual.B);
			
		    Atual.espessura = espessura;
		    glLineWidth(Atual.espessura);
		    
			switch(primitiva) {
			   // Desenha um retangulo	
			   case RETANGULO:	
			      Atual.preenchido = preenchido;
			      if (Atual.preenchido)
			          desenhaRetanguloFull ( Atual.x1, Atual.y1 ,Atual.x2 , Atual.y2);
			      else
			          desenhaRetanguloEmpty ( Atual.x1, Atual.y1 ,Atual.x2 , Atual.y2);
			      break;
			      
			   case RETA:	     
			      desenhaReta ( Atual.x1,Atual.y1 ,Atual.x2 ,Atual.y2 );
			      break;
			      
			   case CIRCULO: 
			      if (Atual.preenchido)
			          desenhaCirculoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2 );	
			      else
			          desenhaCirculoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2 );	
			      break;
			      
			      
			   case TRIANGULO :
			      if (Atual.preenchido)
			           desenhaPoligonoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 3 );
			      else
			           desenhaPoligonoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 3 );
	              break;
	              
			   case LOSANGO :
			      if (Atual.preenchido)
			           desenhaPoligonoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 4 );
			      else
			           desenhaPoligonoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 4 );
	              break;
	              
	        case PENTAGONO :
			      if (Atual.preenchido)
			           desenhaPoligonoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 5 );
			      else
			           desenhaPoligonoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 5 );
	              break;
	              
	        case HEXAGONO :
			      if (Atual.preenchido)
			           desenhaPoligonoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 6 );
			      else
			           desenhaPoligonoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 6 );
	              break;
	              
	        case HEPTAGONO :
			      if (Atual.preenchido)
			           desenhaPoligonoFull ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 7 );
			      else
			           desenhaPoligonoEmpty ( Atual.x1, Atual.y1,Atual.x2 ,Atual.y2, 7 );
	              break;
	        }
	        glDisable(GL_LINE_STIPPLE);
        }
	}
    
    menuCor(); // Exibe o submenu em cima do desenho
    menuOpcoes();
    menuPrimitivas();
	menuPreenchimento();
    menuContorno();
    menuLinhas();
    menuPontos();
    glutSwapBuffers(); // Usado em conjunto com GL_DOUBLE    
}

// Desenho do menu de Primitivas
void menuPrimitivas ( void ){
	// Submenu para primitivas
		
	if ( option == PRIMITIVA ){

        USANDO_CINZA_CLARO;
        for( int i=1; i<=9; i++ )
	       desenhaRetanguloFull( window_w / fatorW, window_h - i * window_h / fatorH, 2 * window_w / fatorW, 
	          window_h - ( i + 1 ) * window_h / fatorH );
	    
	    USANDO_PRETO;
	    
	    for( int i=1; i<=9; i++ ) {
	    	
	        glLineWidth(4.0f);
    	    desenhaRetanguloEmpty( window_w / fatorW, window_h - i * window_h / fatorH, 2 * window_w / fatorW, 
	            window_h - ( i + 1 ) * window_h / fatorH );
		    glLineWidth(1.0f);
		    
		    switch ( i ){
		    	case RETANGULO:
		    		glLineWidth(4.0f);
		    		desenhaRetanguloEmpty( window_w / fatorW + 10, window_h - i * window_h / fatorH - 10, 
					2 * window_w / fatorW -10, window_h - (i + 1) * window_h / fatorH + 10 );
		    		glLineWidth(1.0f);
		    		
		    		break;
		    	case RETA:
		    		glLineWidth(4.0f);
		    		desenhaReta( window_w / fatorW + 10, window_h - i * window_h / fatorH - 10, 
					2 * window_w / fatorW -10, window_h - (i + 1) * window_h / fatorH + 10 );
		    		glLineWidth(1.0f);
		    		
		    		break;
		    	case PONTO:
		    		desenhaPonto( window_w / fatorW + window_w / fatorW/2, window_h - i * window_h / fatorH - window_h / fatorH/2, 6.0 );
		    		
		    		break;
		    	case CIRCULO:
		    		glLineWidth(4.0f);
		    		desenhaCirculoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10 );
		    		glLineWidth(1.0f);
		    		
		    		break;
		    	case 5:
		    		glLineWidth(4.0f);
		    		desenhaPoligonoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10 , 3 );
					glLineWidth(1.0f);
					
		    		break;
		    	case 6:
		    		glLineWidth(4.0f);
		    		desenhaPoligonoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10, 4 );
					glLineWidth(1.0f);
		    		
		    		break;
		    	case 7:
		    		glLineWidth(4.0f);
		    		desenhaPoligonoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10, 5 );
					glLineWidth(1.0f);
		    		
		    		break;
		    	case 8:
		    		glLineWidth(4.0f);
		    		desenhaPoligonoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10, 6 );
					glLineWidth(1.0f);
		    		
		    		break;
		    	case 9:
		    		glLineWidth(4.0f);
		    		desenhaPoligonoEmpty( window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - window_h / fatorH / 2, 
					window_w / fatorW + window_w / fatorW / 2, window_h - i * window_h / fatorH - 10, 7 );
					glLineWidth(1.0f);
		    		
		    		break;	
		    }
	    }
	}
}


// Desenho do menu de Primitivas
void menuPreenchimento ( void ){
	// Submenu para primitivas
		
	if ( option == PREENCHIMENTO ){

        USANDO_CINZA_CLARO;
        for( int i=1; i<=2; i++ )
	       desenhaRetanguloFull( 2 * window_w / fatorW, window_h - i * window_h / fatorH, 3 * window_w / fatorW, 
	          window_h - ( i + 1 ) * window_h / fatorH );
	    
	    USANDO_PRETO;
	    
	    for( int i=1; i<=2; i++ ) {
	    	
	        glLineWidth(4.0f);
    	    desenhaRetanguloEmpty( 2 * window_w / fatorW, window_h - i * window_h / fatorH, 3 * window_w / fatorW, 
	            window_h - ( i + 1 ) * window_h / fatorH );
		    glLineWidth(1.0f);
		    
		    switch ( i ){
		    	case CHEIO:
		    		glLineWidth(4.0f);
		    		desenhaRetanguloFull( 2*window_w / fatorW + 10, window_h - i * window_h / fatorH - 10, 
					   3 * window_w / fatorW -10, window_h - (i + 1) * window_h / fatorH + 10 );
		    		glLineWidth(1.0f);
		    		
		    		break;
		    	case VAZIO:
		    		glLineWidth(4.0f);
		    		desenhaRetanguloEmpty( 2*window_w / fatorW + 10, window_h - i * window_h / fatorH - 10, 
					   3 * window_w / fatorW -10, window_h - (i + 1) * window_h / fatorH + 10 );
		    		glLineWidth(1.0f);
		    		
		    		break;
		    }
	    }
	}
}

// Desenho do menu de Primitivas
void menuLinhas ( void ){
	// Submenu para primitivas
		
	if ( option == LINHAS ){

        USANDO_CINZA_CLARO;
        for( int i=1; i<=6; i++ )
	       desenhaRetanguloFull( 5 * window_w / fatorW, window_h - i * window_h / fatorH, 6 * window_w / fatorW, 
	          window_h - ( i + 1 ) * window_h / fatorH );
	    
	    USANDO_PRETO;
	    
	    for( int i=1; i<=6; i++ ) {
	    	
	        glLineWidth(4.0f);
    	    desenhaRetanguloEmpty( 5 * window_w / fatorW, window_h - i * window_h / fatorH, 6 * window_w / fatorW, 
	            window_h - ( i + 1 ) * window_h / fatorH );
		    glLineWidth(1.0f);
		    
		    switch ( i ){
		    	case 1:
		    		glLineWidth(1.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
	    		    glLineWidth(1.0f);
		    		break;
		    	case 2:
		    		glLineWidth(2.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
		    		6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		glLineWidth(1.0f);
		    		break;
		    	case 3:
		    		glLineWidth(3.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
		    		6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		glLineWidth(1.0f);
		    		break;
		    	case 4:
		    		glLineWidth(4.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
		    		6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		glLineWidth(1.0f);
		    		break;
		    	case 5:
		    		glLineWidth(5.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
		    		6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		glLineWidth(1.0f);
		    		break;
		    	case 6:
		    		glLineWidth(6.0f);
		    		desenhaReta ( 5*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
		    		6*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		glLineWidth(1.0f);
		    		break;
		    }
	    }
	}
}

// Desenho do menu de Pontos
void menuPontos ( void ){
	// Submenu para primitivas
		
	if ( option == PONTOS ){

        USANDO_CINZA_CLARO;
        for( int i=1; i<=6; i++ )
	       desenhaRetanguloFull( 6 * window_w / fatorW, window_h - i * window_h / fatorH, 7 * window_w / fatorW, 
	          window_h - ( i + 1 ) * window_h / fatorH );
	    
	    USANDO_PRETO;
	    
	    for( int i=1; i<=6; i++ ) {
	    	
	        glLineWidth(4.0f);
    	    desenhaRetanguloEmpty( 6 * window_w / fatorW, window_h - i * window_h / fatorH, 7 * window_w / fatorW, 
	            window_h - ( i + 1 ) * window_h / fatorH );
		    glLineWidth(1.0f);
		    
		    switch ( i ){
		    	case 1:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 3 );
		    		break;
		    	case 2:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 4 );
		    		break;
		    	case 3:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 5 );
		    		break;
		    	case 4:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 6 );
		    		break;
		    	case 5:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 7 );
		    		break;
		    	case 6:		    		
		    		desenhaPonto( 6*window_w / fatorW + window_w / fatorW/2, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2, 8 );
		    		break;
		    }
	    }
	}
}

// Desenho do menu de Primitivas
void menuContorno ( void ){
	// Submenu para primitivas
		
	if ( option == CONTORNO ){

        USANDO_CINZA_CLARO;
        for( int i=1; i<=3; i++ )
	       desenhaRetanguloFull( 3 * window_w / fatorW, window_h - i * window_h / fatorH, 4 * window_w / fatorW, 
	          window_h - ( i + 1 ) * window_h / fatorH );
	    
	    USANDO_PRETO;
	    
	    for( int i=1; i<=3; i++ ) {
	    	
	        glLineWidth(4.0f);
    	    desenhaRetanguloEmpty( 3 * window_w / fatorW, window_h - i * window_h / fatorH, 4 * window_w / fatorW, 
	            window_h - ( i + 1 ) * window_h / fatorH );
		    glLineWidth(1.0f);
		    
		    switch ( i ){
		    	case CONTINUO:
		    		glLineWidth(6.0f);
		    		desenhaReta ( 3*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					4*window_w / fatorW - 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
	    		    glLineWidth(1.0f);
		    		
		    		break;
		    	case TRACEJADO:
		    		glLineWidth(6.0f);
		    		desenhaReta ( 3*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + window_w / fatorW/7, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		
		    		desenhaReta ( 3*window_w / fatorW + 10 + 2*window_w / fatorW/7, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 3*window_w / fatorW/7, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
				    desenhaReta ( 3*window_w / fatorW + 10 + 4*window_w / fatorW/7, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 5*window_w / fatorW/7, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
					glLineWidth(1.0f);
					
		    		break;
		    	case PONTILHADO:
		    		glLineWidth(6.0f);
		    		desenhaReta ( 3*window_w / fatorW + 10, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		
		    		desenhaReta ( 3*window_w / fatorW + 10 + 2*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 3*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
				    desenhaReta ( 3*window_w / fatorW + 10 + 4*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 5*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
					desenhaReta ( 3*window_w / fatorW + 10 + 6*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 7*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
					desenhaReta ( 3*window_w / fatorW + 10 + 8*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 9*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
		    		
		    		desenhaReta ( 3*window_w / fatorW + 10 + 10*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2,
					3*window_w / fatorW + 10 + 11*window_w / fatorW/15, window_h - (i+1) * window_h / fatorH+window_h / fatorH/2 );
					
					glLineWidth(1.0f);
					break;
		    }
	    }
	}
}

// Chamada pela GLUT quando a janela eh redimensionada.
// O codigo a seguir altera o sistema de coordenadas sempre
// que a janela eh redimensionada, mantendo as dimensoes dos objetos.
void reshape(int w, int h){
	window_w = w;
	window_h = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport (0, 0, window_w, window_h);
	glOrtho(0.0, window_w, 0.0, window_h, -1.0, 1.0);
	
}

// Callback para gerenciar eventos de teclado
void HandleKeyboard(unsigned char key, int x, int y){
	switch (key) {
		
		case 67: // C
		case 99: // c
			linha = CONTINUO;
			break;
			
		case 84:  // T
		case 116: // t
			linha = TRACEJADO;
			break;
			
		case 80:  // P
		case 112: // p
			linha = PONTILHADO;
			break;
	
	    case 70:  // F
		case 102: // f
		    preenchido = !preenchido;
			break;
	}
  glutPostRedisplay();
}




