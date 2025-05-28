int scroll_menu(int posX, int posY, int cantOpc);
void presenta_menu(int posX, int posY)
{
 	 system("cls");
 	 SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),15);
	 gotoxy(posX,posY);printf("1. SELECCION");
	 gotoxy(posX,++posY);printf("2. REGISTRO DE PARTIDAS");
	 gotoxy(posX,++posY);printf("3. ESTADISTICAS");
	 gotoxy(posX,++posY);printf("4. SALIR");	 	 
}

int scroll_menu(int posX, int posY, int cantOpc)
{
 char tecla='\0';
 int op=1,y=posY;
 
 posX=posX-2;
 SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),10);	
 gotoxy(posX,posY); 
 printf(">>");
 do{
 	tecla=getch(); 	
 	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0);
 	gotoxy(posX,y);
	printf(">>");
 	if(tecla==80 && op<cantOpc)// 'P' 80 ABAJO
 	{ 	y++;
		op++;
 	}
 	if(tecla==72 && op>1)//'H' ARRIBA
 	{ 	y--;
		op--;
 	}
 	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),10);
 	gotoxy(posX,y);
	printf(">>");
 }while(tecla!=27 && tecla!=13);//ESC 27   ENTER 13
 return (op);
}

