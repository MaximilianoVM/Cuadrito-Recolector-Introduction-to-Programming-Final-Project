/* 

////////////////////// 	J	U	E	G	O		P	R	O	Y	E	C	T	O	//////////////////////

*/

#include "miniwin.h"
#include <sstream>
#include <windows.h>
#include <stdlib.h>     
#include <time.h>       

using namespace miniwin;
using namespace std;

////////////////////// 		E  S  T  R  U  C  T  U  R  A  S		//////////////////////

struct Juego{
    int gordo    = 800;
    int alto     = 600;
    
    int vidas    = 3;
    int score    = 0;  //Puntaje
    int buenas   = 0;  //buenas existiendo
    int malas	 = 1;  //malas debe haber ahora (cambia)
    int mbuenas  = 0;  //muybuenas debe haber ahora (cambia)
    int bolast 	 = 2;  //buenas totales en pantalla
    int buenasHay =0;  //buenas existiendo (cambia)
    int mbuenasHay=0;  //muy buenas existiendo (camibia)
    int malasHay  =0;  //malas existiendo (cambia)
    
};

struct Caja{
	int x           = 0; // X y Y SE MUEVEN
    int y           = 600;
    int largo       = 60; //gordo
    int ancho       = 60; //alto
};

struct BolasBuenas{
	int x           = 20; // X y Y SE MUEVEN
    int y           = 65;
    int radio       = 12;
};

struct BolasMalas{
	int x           = 400; // X y Y SE MUEVEN
    int y           = 65;
    int sentido     = 1; //por defecto (varía)
    int radio       = 10;
    int velocidad 	= 5; //cambia dependiendo del puntaje y si tomó amarillas
};

struct BolasMuyBuenas{
	int x           = 400; // X y Y SE MUEVEN (cambian)
    int y           = 65;
    int radio       = 15;
};


////////////////////// 		P  R  O  T  O  T  I  P  O  S		//////////////////////

void pintaBanner(struct Juego, struct BolasMalas[]);           												 				 // Pinta tablero

void pintaObjetos(struct Caja, struct BolasBuenas[], struct BolasMalas[], struct BolasMuyBuenas &, struct Juego);            // Imprime objetos en pantalla 

void mueveCaja(struct Caja*, struct Juego);        																			 // Detecta eventos de teclado (MUEVE LA CAJA)      

void cambiaValores(struct BolasBuenas[] , struct BolasMalas[] , struct BolasMuyBuenas & , struct Juego &, struct Caja &);	 // Mueve las bolitas, buenas yMbuenas caen y malas rebotan

void generaBuenas(struct BolasBuenas[] , struct Juego &);																	 // le da sus valores de posicion (aleatorios) a las bolas buenas que existan (2)

void generaMalas(struct BolasMalas[] , struct Juego &);																		 // le da sus valores de posicion (aleatorios) a las bolas malas que vayan apareciendo (varía)

void generaMBuenas(struct BolasMuyBuenas& , struct Juego &);																 // le da sus valores de posicion (aleatorios) a la bola muy buena 

void detectaColision(struct Caja, struct BolasBuenas[], struct BolasMalas[], struct BolasMuyBuenas &, struct Juego &);		 // Detecta el contacto de las bolas con la caja y determina lo que ocurrirá dependiendo de la bola

void eliminaBuenas(struct BolasBuenas[] , struct Juego &,  struct BolasMuyBuenas &);										 // Una vez que tocan el punto mas bajo, les da un nuevo valor de posición a buenas y mbuenas (estas ultimas desaparecen)



////////////////////// 		M	A	I	N		//////////////////////



int main(){
	
	srand(time(NULL)); 
	
	int prob=0 ; 					//inicializa
	
	struct Juego  game;
	struct Caja caja;
	struct BolasBuenas bBuenas[2]; //solo habrá 2
	struct BolasMalas bMalas[25];  //límite (es imposible sobrevivir eso) pero puede variar
	struct BolasMuyBuenas bmBuenas; //solo habrá una cuando se alcance cierto puntaje (multiplos de ---)
	int t;  						//tecla
	
	vredimensiona(game.gordo, game.alto);  //pantalla
	
	
	////////////////////////////	INICIO	  ////////////////////////////
	
	do{
		color(ROSACAJA);												//una caja en el inicio de adorno
    	rectangulo_lleno(370, 200, 370 + caja.largo, 200-caja.ancho);	
    	refresca();
    	
    	stringstream txtStream;
    	
    	color(AZULBOLA);
    	
    	txtStream.str("");
    	txtStream <<"-Recolecta las bolitas rosas mientras evitas recolectar las azules-";
    	texto(190, 250, txtStream.str());

    	txtStream.str("");
    	txtStream <<"-Cada 10 puntos aparecerá otra azul y aumentarán su velocidad a menos que tomes la amarilla-";
    	texto(90, 280, txtStream.str());
    	
    	txtStream.str("");
    	txtStream <<"-La amarilla además te dará una vida extra-";
    	texto(260, 310, txtStream.str());
    	
    	refresca();
    	
    	color(AMARILLO);
    	    
		txtStream.str("");
    	txtStream <<"CUADRITO RECOLECTOR";
    	texto(320, 100, txtStream.str());
    	
    	txtStream.str("");
    	txtStream <<"¡ pulsa ESPACIO para iniciar !";
    	texto(305, 450, txtStream.str());
    	
    	color(ROSABOLA);
    	
    	txtStream.str("");
    	txtStream <<"BRISSA ELIZEL SICAIROS GONZALEZ";
    	texto(550, 550, txtStream.str());
    	
    	txtStream.str("");
    	txtStream <<"MAXIMILIANO VACA MONTEJANO";
    	texto(550, 565, txtStream.str());
    	
    	refresca();
    	
		t=tecla();
		
		espera(50);
    	
	}while(t != ESPACIO);
	
	
	////////////////////////////	JUEGO	////////////////////////////
    
    do{
    	
    	
    	borra();
    	
    	pintaBanner(game, bMalas);  
    	
    	
    	pintaObjetos(caja, bBuenas, bMalas,bmBuenas, game);		//es importante que estos dos estén aquí
    	detectaColision(caja, bBuenas, bMalas, bmBuenas, game);
    	
    	
    	mueveCaja(&caja, game);
    	 	
    	 	
		//////////////---	APARECE BUENAS  ---//////////////
		
		prob = rand()% 400 + 1 ;				//genera un numero aleatorio hasta 400
		
		if(prob<5 && game.buenas<game.bolast){  // si ese numero es menor a 5, genera una buena, PARA QUE SALGAN SEPARADAS
		
			game.buenas++;
			
		}
		
		
		//////////////---	METAS DE PUNTOS "NIVELES"  ---//////////////
			
			if(game.score%10 == 0 && game.score>0){  		//CADA 10 PUNTOS (multiplos de 10) el 0 tambien cumple condición: por eso &&
				
					game.malas++;								//> aumenta el numero de malas 
					
					for(int i=0; i<game.malas ; i++){			//> aumenta la velocidad a todas
						bMalas[i].velocidad ++;
					}
					game.score++; 								//> punto extra por llegar a meta
					game.mbuenas=1;								//> sale una amarilla con el poder de revertir lo malo y suma una vida si se agarra
			}
	 	
	 	
	 	//////////////---	GENERA  ---////////////// (a partir de saber cuantas deben existir)
		
		generaBuenas(bBuenas, game);
		
		generaMalas(bMalas, game);
		
		generaMBuenas(bmBuenas, game);
		
		//////////////---	MUEVE Y ACTUALIZA  ---//////////////
		
		cambiaValores(bBuenas, bMalas, bmBuenas, game, caja); // MUEVE
    	
    	eliminaBuenas(bBuenas , game, bmBuenas);
    	
    	
    	refresca();
    	espera(1);
    	
	}while (game.vidas > 0);
	
	////////////////////////////	SI MUERE  ////////////////////////////
	
	if(game.vidas<=0){
		stringstream txtStream;
		
		borra();
		
		color(AMARILLO);
    	txtStream.str("");
    	txtStream <<"has perdido, se mas cuidadoso y procura tomar las bolas amarillas.";
    	texto(190, 200, txtStream.str());
    	
    	txtStream.str("");
    	txtStream <<"PUNTAJE: "<< game.score;
    	texto(330, 310, txtStream.str());
    	
    	refresca();
    	
	}
	
	
	
    return 0;
}


////////////////////// 		F  U  C  I  O  N  E  S		//////////////////////


void pintaBanner(struct Juego game, struct BolasMalas m[0]){		//solo necesitamos saber la velocidad de una bola mala; son iguales
    stringstream txtStream;
    
    color(ROSACAJA);
    rectangulo_lleno(0, 0, game.gordo, 50);
    color(AMARILLO);
    txtStream.str("");
    txtStream << "VELOCIDAD MALAS: " << m[0].velocidad << "   		VIDAS: " << game.vidas << "  		SCORE: " << game.score;
    texto(240, 50, txtStream.str());

}

void pintaObjetos(struct Caja caja, struct BolasBuenas b[2], struct BolasMalas m[2],struct BolasMuyBuenas &mb , struct Juego game){
    
    //////////////---	BUENAS  ---//////////////
    
    for(int i=0 ; i<game.buenas ; i++){  //desde i al numero de buenas que debe haber 
    	
    	color(ROSABOLA);
    	circulo_lleno(b[i].x, b[i].y, b[i].radio);

	}
	
	//////////////---	MALAS  ---//////////////
	
	for(int i=0 ; i<game.malas ; i++){  //desde i al numero de malas que debe haber 
    
    	color(AZULBOLA);
    	circulo_lleno(m[i].x, m[i].y, m[i].radio);
	}
	
	//////////////---	MUY BUENAS  ---//////////////
	
	if(game.mbuenas>0){					//si debe haber algúna (no siempre será así)
		
		color(AMARILLO);
    	circulo_lleno(mb.x, mb.y, mb.radio);
		
	}
	
	//////////////---	CAJA  ---//////////////
    
    color(ROSACAJA);
    rectangulo_lleno(caja.x, caja.y, caja.x + caja.largo, caja.y-caja.ancho);
    
    
}

void mueveCaja(struct Caja *caja, struct Juego game){
	
	int t = tecla();
	
	switch(t){
		
		case DERECHA:   if((*caja).x + (*caja).largo < game.gordo){
			(*caja).x+=20;
			
		}break;
                        
        case IZQUIERDA: if((*caja).x > 0){
        	(*caja).x-=20;

		}break;
		
        case ARRIBA:    
			if((*caja).y == 600){
        		do{
        		(*caja).y-=1;
				}
				while((*caja).y != 550);
				//
			}
			else if((*caja).y == 550){
				do{
        		(*caja).y-=1;
				}
				while((*caja).y != 500);
			}	
		break;
		
		case ABAJO:    
			if((*caja).y == 550){
        		do{
        		(*caja).y+=1;
				}
				while((*caja).y != 600);
			}
				
			else if((*caja).y == 500){
				do{
        		(*caja).y+=1;
				}
				while((*caja).y != 550);
			}
        	//
		break;
	}
	
}

void generaBuenas(struct BolasBuenas b[2] , struct Juego &game){
	
	if(game.buenas < game.bolast){								//si no exceden el límite
		for(int i=game.buenasHay ; i<game.buenas ; i++){		//desde el numero de buenas que HAY hasta el que DEBE HABER
		
			b[i].x = rand()% 785 + 15 ;							//les da valores de posición (x aleatoria)
			b[i].y =  65; 										//y fija para que caiga de arriba
			
			game.buenasHay ++;									//señala que ya HAY una mas (cambia el valor original)
		
		}
	}
	
}

void generaMBuenas(struct BolasMuyBuenas &mb , struct Juego &game){
	
	if(game.mbuenas==1){										//si se requiere una (solo puede haber una)
		for(int i=game.mbuenasHay ; i<game.mbuenas ; i++){		//desde el numero de muy buenas que HAY hasta el que DEBE HABER
			
			mb.x = rand()% 785 + 15 ;							//les da valores de posición (x aleatoria)
			mb.y =  65; 										// y fija para que caiga de arriba
			
			game.mbuenasHay++;									//señala que ya HAY una mas (cambia el valor original)
		}
	}
	
}
	
void generaMalas(struct BolasMalas m[25] , struct Juego &game){  

	for(int i=game.malasHay ; i<game.malas ; i++){   		   //desde el numero de malas que HAY hasta el que DEBE HABER
	
		m[i].x = rand()% 785 + 15 ;								//les da valores de posición aleatorios (que no aparezcan muy abajo)
		m[i].y = rand()% 300 + 50; 
		m[i].sentido = rand()% 4 +1;							//les da un sentido aleatorio
		 
		
		game.malasHay++;										//señala que ya HAY una mas (cambia el valor original)
	}
	
}



void cambiaValores(struct BolasBuenas b[2], struct BolasMalas m[25], struct BolasMuyBuenas &mb, struct Juego &game, struct Caja &caja){
	
	////////////////////////////////// - BUENAS - //////////////////////////////////
	
	
	for(int i=0 ; i<game.bolast ; i++){				
		
		b[i].y += 5;						//las buenas solo caen, con una velocidad 5 (fija)
		
	}
	
	
	////////////////////////////////// - MALAS - //////////////////////////////////
	
	
	/* se intentó que rebotaran en la caja pero no se logró, este intento se puede ver de forma comentada */
	
	for(int i=0 ; i<game.malas ; i++){ 					//para cada una de las malas verificar lo siguiente
		
		switch(m[i].sentido){
			case 1: // (1) arriba-izq
					m[i].x-=m[i].velocidad; 
					m[i].y-=m[i].velocidad;
					
					//////////////---	REBOTE  ---//////////////
					
					if(m[i].x<=0+10) m[i].sentido=2;  //lado
					else if(m[i].y<=50+10) m[i].sentido=4; //techo
					
					//////////////---	EN CAJA (DESCARTADO)  ---//////////////
					
					/*
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ //techo a caja
						m[i].sentido=4;
						game.vidas --;
					}
					
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ //lado a caja
						m[i].sentido=2;
						game.vidas --;
					}
					*/
					
					break; 
					
			case 2: // (2) arriba-der
					m[i].x+=m[i].velocidad; 
					m[i].y-=m[i].velocidad; 
					
					//////////////---	REBOTE  ---//////////////
					
					if(m[i].x>=800-10) m[i].sentido=1; 
					else if(m[i].y<=50+10) m[i].sentido=3; 
					
					//////////////---	EN CAJA (DESCARTADO)  ---//////////////
					
					/*
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=3;
						game.vidas --;
					}
					
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=1;
						game.vidas --;
					}
					*/
					
					break; 
					
			case 3: // (3) abajo-der
					m[i].x+=m[i].velocidad; 
					m[i].y+=m[i].velocidad; 
					
					//////////////---	REBOTE  ---//////////////
					
					if(m[i].x>=800-10) m[i].sentido=4; 
					else if(m[i].y>=600-10) m[i].sentido=2;
					
					//////////////---	EN CAJA (DESCARTADO)  ---//////////////
					
					/*
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=2;
						game.vidas --;
					}
					
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=4;
						game.vidas --;
					}
					*/
					break; 
					
			case 4: // (4) abajo-izq
					m[i].x-=m[i].velocidad; 
					m[i].y+=m[i].velocidad; 
					
					//////////////---	REBOTE  ---//////////////
					
					if(m[i].x<=0+10) m[i].sentido=3;
					else if(m[i].y>=600-10) m[i].sentido=1;
					
					//////////////---	EN CAJA (DESCARTADO)  ---//////////////
					
					/*
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=1;
						game.vidas --;
					}
					
					else if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){ 
						m[i].sentido=3;
						game.vidas --;
					}
					*/
					break; 
		}
				
		}
		
		////////////////////////////////// - MUY BUENAS - //////////////////////////////////
		
		
		if(game.mbuenas==1){
			mb.y+=6;				//solo es una y cae con una velocidad de 6 (fija) y mayora las buenas para mayor dificultad
		}
	
	
}

void detectaColision(struct Caja caja, struct BolasBuenas b[2], struct BolasMalas m[25], struct BolasMuyBuenas &mb, struct Juego &game){
	
	////////////////////////////////// - BUENAS - //////////////////////////////////
	
	
	for(int i=0 ; i<game.buenas ; i++){
		if(b[i].y >= caja.y-caja.ancho &&  b[i].y<=caja.y  &&  b[i].x >= caja.x  &&  b[i].x <= caja.x+caja.largo){	//			si toca al cubo

			game.score ++;
			
			
			// "animación" confirma que se recolectó la bolita
			
			color(VERDE);
			rectangulo_lleno(caja.x+10, caja.y-10, caja.x + caja.largo-10, caja.y-caja.ancho+10);
			refresca();
			espera(70);
			
			
			// la regresa con valores aleatorios de x a la parte superior
			
			
			b[i].y= 50;
			b[i].x = rand()% 785 + 15 ;
			
			
			//game.buenasHay--;	trae problemas: suma puntos de mas
			
		}
					
	}
	
	////////////////////////////////// - MALAS - //////////////////////////////////
	
	
	for(int i=0 ; i<game.malas ; i++){
		if(m[i].y >= caja.y-caja.ancho &&  m[i].y<=caja.y  &&  m[i].x >= caja.x  &&  m[i].x <= caja.x+caja.largo){	//			si toca al cubo 

			game.vidas --;
			
			// "animación" confirma que se recolectó la bolita
			
			color(ROJO);
			rectangulo_lleno(caja.x+10, caja.y-10, caja.x + caja.largo-10, caja.y-caja.ancho+10);
			refresca();
			espera(500);
			
			// la regresa con valores aleatorios de x a la parte superior
			
			/*
			m[i].y= 50;
			m[i].x = rand()% 785 + 15 ;
			*/
			
			game.malasHay--; //elimina una bolita de las que hay (se generará otra para cumplir con las que debe haber)
				
			
		}
					
	}
	
	
	////////////////////////////////// - MUY BUENAS - //////////////////////////////////
	
	
	if(mb.y >= caja.y-caja.ancho &&  mb.y<=caja.y  &&  mb.x >= caja.x  &&  mb.x <= caja.x+caja.largo){	//			si toca al cubo
	
			game.mbuenasHay=0;  																			//indica que ya HAY muy buenas
			
			for(int i=game.mbuenasHay ; i<game.mbuenas ; i++){
				game.mbuenas=0;																				//indica que ya NO DEBE HABER muy buenas
				
				
				// "animación" confirma que se recolectó la bolita:
				
				color(AMARILLO);
				rectangulo_lleno(caja.x+10, caja.y-10, caja.x + caja.largo-10, caja.y-caja.ancho+10);
				refresca();
				espera(1000);
				
				//efectos de la bolita:
				game.vidas ++;
				game.malas --;
				for(int i=0; i<game.malas ; i++){
					m[i].velocidad --;			//resta velocidad a TODAS
				}
				
			}
		}
}


void eliminaBuenas(struct BolasBuenas b[20] , struct Juego &game, struct BolasMuyBuenas &mb){
	
		//////////////---	BUENAS  ---//////////////
		
		for(int i=0 ; i<game.buenas ; i++){     			//evalúa para cada bolita buena
			
			if(b[i].y >= game.alto){						//si llega al piso
				
				b[i].y= 50;									//lo regresa para arriba
				b[i].x = rand()% 785 + 15 ;					//con valor aleatorio en x
	
			}	
		}
		
		//////////////---	MUY BUENAS  ---//////////////
		
		if(mb.y >= game.alto){
			game.mbuenas =0;				//si llega al piso desaparece (debe haber=0)
		}
		
	
}
