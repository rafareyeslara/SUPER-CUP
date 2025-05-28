#include <stdio.h>
#include <conio.h>
#include<time.h>
#include<stdlib.h>
#include <windows.h>

//DEFINICION DE TECLAS
#define RIGHT 77
#define LEFT 75
#define DOWN 80 
#define UP 72
#define ENTER 13
#define ESC 27
#define SPACE 2
//HEADERS CREADOS 
#include "gotoxy.h"
#include "menu.h"

int main(){
int op=0; 
int x=3,y=5;

getch();
do{
	presenta_menu(x,y);
	op=scroll_menu(x,y,4);
	system("cls");
	switch(op)
	{
		case 1:printf("SELECCION");break;
	 	case 2:printf("REGISTRO DE PARTIDAS");break;
		case 3:printf("ESTADISTICAS");getch();break;
	 	case 4:printf("SALIR");getch();break;
	}
	
 }while(op!=4);
	return 0;
}


