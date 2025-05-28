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

typedef struct {
	int id;
	char name[MAX_LEN_STR];
	int victories;
} Character;

typedef struct CharacterNode {
	Character character;
	struct CharacterNode* next;
	struct CharacterNode* prev;
} CharacterNode;

//Prototypes
//General
void freePlayers(PlayerNode*);
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
	freePlayers(headPlayer);
	return 0;
}
/*
Character management

*/
Character createCharacter(CharacterNode* head) {
    Character newChar;
    newChar.id = ++lastCharacterID;
	char name[MAX_LEN_STR];
    int exists;
    do {
        exists = 0;
        printf("\nDame el nombre del personaje: ");
        fgets(name, MAX_LEN_STR, stdin);
        name[strcspn(name, "\n")] = 0;

        CharacterNode* current = head;
        while (current != NULL) {
            if (strcmp(current->character.name, name) == 0) {
                exists = 1;
                printf("Ese nombre ya existe. Intenta otro.\n");
                break;
            }
            current = current->next;
        }
    } while (exists);

    strcpy(newChar.name, name);
    newChar.victories = 0;

    return newChar;
}
CharacterNode* createCharacterNode(Character character) {
    CharacterNode* newNode = (CharacterNode*)malloc(sizeof(CharacterNode));
    newNode->character = character;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}
void addCharacter(CharacterNode** head) {
    Character newChar = createCharacter(*head);
    CharacterNode* newNode = createCharacterNode(newChar);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    CharacterNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
    newNode->prev = current;
}
void printAllCharacters(CharacterNode* head) {
    CharacterNode* current = head;
    while (current != NULL) {
        printf("\nPERSONAJE:\n");
        printf("Id: %d\n", current->character.id);
        printf("Nombre: %s\n", current->character.name);
        printf("Victorias: %d\n", current->character.victories);
        current = current->next;
    }
}
void saveCharactersToFile(CharacterNode* head) {
    FILE* file = fopen("characters.txt", "w");
    if (file == NULL) {
        perror("No se pudo abrir archivo para personajes");
        return;
    }

    CharacterNode* current = head;
    while (current != NULL) {
        fprintf(file, "%d|%s|%d\n",
                current->character.id,
                current->character.name,
                current->character.victories);
        current = current->next;
    }

    fclose(file);
    printf("Personajes guardados en 'characters.txt'\n");
}
void loadCharactersFromFile(CharacterNode** head) {
    FILE* file = fopen("characters.txt", "r");
    if (file == NULL) {
        printf("Archivo de personajes no encontrado.\n");
        return;
    }

    Character tmpChar;
    while (fscanf(file, "%d|%49[^|]|%d\n",
                  &tmpChar.id, tmpChar.name, &tmpChar.victories) == 3) {
        if (tmpChar.id > lastCharacterID)
            lastCharacterID = tmpChar.id;
        CharacterNode* newNode = createCharacterNode(tmpChar);

        if (*head == NULL) {
            *head = newNode;
        } else {
            CharacterNode* current = *head;
            while (current->next != NULL)
                current = current->next;
            current->next = newNode;
            newNode->prev = current;
        }
    }

    fclose(file);
    printf("Personajes cargados desde 'characters.txt'\n");
}
void freeCharacters(CharacterNode* head) {
    CharacterNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    printf("Memoria de personajes liberada.\n");
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
	}while(newPlayer.age<=0);
	newPlayer.victories = 0;
	
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
Liberar memoria
*/
void freePlayers(PlayerNode* head) {
    PlayerNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    printf("\nMemoria liberada correctamente.\n");
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
			current->player.victories);
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
