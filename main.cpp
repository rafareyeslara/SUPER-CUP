#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEN_STR 50

typedef struct{
	int id;
	char name[MAX_LEN_STR];
	char nickname[MAX_LEN_STR];
	int age;
	int victories;
}Player;

typedef struct PlayerNode{
	Player player;
	PlayerNode *next;
	PlayerNode *prev;
}PlayerNode;

//Prototypes
//General
void cleanBuffer();
//Player
Player createPlayer();
PlayerNode* createPlayerNode(Player);
void addPlayer(PlayerNode**);
void savePlayersToFile(PlayerNode*);
void printAllPlayers(PlayerNode*);
/*
......... MAIN ....................
*/
int main(){
	PlayerNode *headPlayer = NULL;
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	savePlayersToFile(headPlayer);
	printAllPlayers(headPlayer);
	return 0;
}

/*
......... PLAYER FUNCTIONS ........
*/

// Variable global para autoincrementar ID
int lastPlayerID = 0;

Player createPlayer(){
	Player newPlayer;
	newPlayer.id = ++lastPlayerID;
	
	printf("\nDame el nombre del jugador:");
	fgets(newPlayer.name, MAX_LEN_STR, stdin);
	newPlayer.name[strcspn(newPlayer.name, "\n")] = 0;
	printf("\nDame el nickname sin espacios: ");
	scanf("%s", newPlayer.nickname);
	do{
		printf("\nDame edad: ");
		scanf("%d", &newPlayer.age);
		if(newPlayer.age<=0){
			printf("Edad invalida, intenta nuevamente.\n");
		}
	}while(newPlayer.age<=0)
	newPlayer.points = 0;
	
	cleanBuffer();
	return newPlayer;
}

PlayerNode* createPlayerNode(Player newPlayer){
    PlayerNode *newPlayerNode;
	newPlayerNode = (PlayerNode*)malloc(
							sizeof(PlayerNode));
    newPlayerNode->player = newPlayer;
    newPlayerNode->next = NULL;
    newPlayerNode->prev = NULL;
    return newPlayerNode;
}

void addPlayer(PlayerNode **headPlayer){
	Player newPlayer = createPlayer();
    PlayerNode *newPlayerNode = createPlayerNode(
									newPlayer);
    if(*headPlayer==NULL){
        *headPlayer = newPlayerNode;
        return;
    }
    PlayerNode *lastNode = *headPlayer;
    while(lastNode->next != NULL){
        lastNode = lastNode->next;
    }
    lastNode->next = newPlayerNode;
    newPlayerNode->prev = lastNode;
}

/*
Clean buffer
*/
void cleanBuffer(){
	int c;
	while ((c = getchar()) != '\n' 
							&& c != EOF); 
}

/*
Save the players to a file with | as the separator.
*/
void savePlayersToFile(PlayerNode *headPlayer) {
    FILE* file = fopen("players.txt", "w");
    if (file == NULL) {
        perror("Error en archivo\n");
        return;
    }

    PlayerNode* current = headPlayer;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%d|%d\n", 
			current->player.id,
			current->player.name,
			current->player.nickname,
			current->player.age,
			current->player.points);
        current = current->next;
    }

    fclose(file);
    printf("Jugadores guardados en 'players.txt'\n");
}

void printAllPlayers(PlayerNode *headPlayer){
	PlayerNode* current = headPlayer;
    while (current != NULL) {
    	printf("\nJUGADOR:\n");
        printf("Id: %d\n",current->player.id);
        printf("Nombre: %s\n",
						current->player.name);
		printf("Nickname: %s\n",
						current->player.nickname);
		printf("Edad: %d\n",
						current->player.age);
		printf("Victorias: %d\n",
						current->player.victories);
        current = current->next;
    }
}
