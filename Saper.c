#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct{
	char odkryte;
	int ukryte;
}pole;

void setBombs(pole[][8], int);
void updateGame(pole[][8],int);
void odkryjZera(pole[][8],int*,int,int);
void odkryjPole(pole[][8],int,int,int,int*,bool*);
void changeColor(int);
void pokazWyniki();
void przegrana();
void wygrana(int);

int main(){
	//Inicjalizacja gry
	pole tablica[8][8];
	int startTimer = time(NULL);
	for(int x=0;x<8;x++){
		for(int y=0;y<8;y++){
			tablica[x][y].ukryte=0;
			tablica[x][y].odkryte='#';
		}
	}
	setBombs(tablica, 10);
	//Toczenie sie gry
	bool przegrales = false;
	int wynik = 0;
	while(!przegrales&&wynik<54){
		updateGame(tablica,1);
		char input[50];
		scanf("%49s",input);
			if(strstr(input,"quit")){
			przegrales=true;
		}
		if(strstr(input,"win")){ //Komenda do testowania
			wynik=100;
		}
		if(strstr(input,"highscores")){
			pokazWyniki();
			przegrales=true;
		}
		int z = 0;
		if(input[0]=='X'){
			z+=1;
		}
		int x = input[0+z]-65;
		int y = input[1+z]-97;
		odkryjPole(tablica,x,y,z,&wynik,&przegrales);
		if(wynik>=54){
			system("clear");
			updateGame(tablica,0);
			int czas = time(NULL)-startTimer;
			wygrana(czas);
		}
	}
	return(0);
}

void setBombs(pole tablica[][8], int ilebomb){
	srand(time(NULL));
	while(ilebomb>0){
		int x = rand()%8;
		int y = rand()%8;
		if(tablica[x][y].ukryte!=9){
			for(int z=-1;z<=1;z++){
				for(int c=-1;c<=1;c++){
					if(x+z>=0&&x+z<=7&&y+c>=0&&y+c<=7){
						if(tablica[x+z][y+c].ukryte!=9){
							tablica[x+z][y+c].ukryte+=1;
						}
					}
				}
			}
			tablica[x][y].ukryte=9;
			ilebomb-=1;
		}
	}
}

void updateGame(pole tablica[][8],int wyswietlKomendy){
	system("clear");
	printf("   ");
	for(int x=0;x<8;x++){
		changeColor(0);
		printf("%3c",65+x);
	}
	for(int y=0;y<8;y++){
		for(int x=0;x<8;x++){
			if(x==0){
				changeColor(0);
				printf("\n%3c",97+y);
			}
			if(tablica[x][y].odkryte!='#'){
				changeColor(tablica[x][y].odkryte-'0'+1);
				if(tablica[x][y].odkryte=='X'){
					changeColor(6);
				}
			}
			else{
				changeColor(0);
			}
			printf("%3c",tablica[x][y].odkryte);
		}
	}
	changeColor(0);
	if(wyswietlKomendy){
		printf("\n\nWpisz komende:\nAby odkryc pole wpisz koordynaty np: 'Dc'\nAby oflagowac jako mine wpisz X przed koordynatem np: 'XAb'\nAby zobaczyc najlepsze wyniki wpisz 'highscores'\nAby wyjsc wpisz 'quit'\n\n\n\n");
	}
}

void odkryjZera(pole tablica[][8],int* wynik,int x,int y){
	for(int z=-1;z<=1;z++){
		for(int c=-1;c<=1;c++){
			if(x+z>=0&&x+z<=7&&y+c>=0&&y+c<=7){
				if(!(tablica[x+z][y+c].odkryte!='#')){
					tablica[x+z][y+c].odkryte=tablica[x+z][y+c].ukryte + '0';
					*wynik+=1;
					if(tablica[x+z][y+c].ukryte==0){
						odkryjZera(tablica,wynik,x+z,y+c);
					}
				}
			}
		}
	}
}

void odkryjPole(pole tablica[][8],int x,int y,int z,int* wynik,bool* przegrales){
	if(z==0){
		if(tablica[x][y].odkryte=='#'){
			if(tablica[x][y].ukryte!=9){
				tablica[x][y].odkryte=tablica[x][y].ukryte+'0';
				*wynik+=1;
				if(tablica[x][y].ukryte==0){
					odkryjZera(tablica,wynik,x,y);
				}
			}
			else{
				*przegrales=true;
				przegrana();
			}
		}
	}
	else{
		if(tablica[x][y].odkryte=='#'){
			tablica[x][y].odkryte='X';
		}
		else if(tablica[x][y].odkryte=='X'){
			tablica[x][y].odkryte='#';
		}
	}
}

void changeColor(int id){
	switch(id){
		case 0:
			printf("\033[0;37m"); //Biały
			break;
		case 1:
			printf("\033[1;32m"); //Zielony
			break;
		case 2:
			printf("\033[1;34m"); //Jasno niebieski
			break;
		case 3:
			printf("\033[1;35m"); //Fioletowy
			break;
		case 4:
			printf("\033[1;33m"); //Zolty
			break;
		case 5:
			printf("\033[1;36m"); //Turkusowy
			break;
		case 6:
			printf("\033[1;31m"); //Czerwony
			break;
	}
}

void przegrana(){
	system("clear");
	printf("Przegrales");
}

void wygrana(int czas){
	char imie[30];
	printf("\n\n\nWygrales w %d sekund!\nWpisz swoje imie zeby zostac dodanym do tablicy wynikow:\n",czas);
	scanf("%29s",imie);
	FILE * wyniki;
	wyniki = fopen("wyniki.txt","a");
	fprintf(wyniki,"%s - %d sekund.\n",imie,czas);
	fclose(wyniki);
}

void pokazWyniki(){
	system("clear");
	FILE * wyniki;
	wyniki = fopen("wyniki.txt","r+");
	printf("Tabela wynikow:\n\n\n");
	if(wyniki!=NULL){
		int x=1;
		while(1){
			char wynik[50] = "";
			char* outcome = fgets(wynik,49,wyniki);
			if(strlen(wynik)>3){
				printf("%d. %s",x,wynik);
			}
			x+=1;
			if(outcome == NULL){
				break;
			}
		}
		printf("\n");
	}
	else{
		printf("Nikt jeszcze nie wygral, badz pierwszy!\n");
	}
}
