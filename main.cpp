#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

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

typedef struct {
	int player1ID;
	int player2ID;
	char character1[MAX_LEN_STR];
	char character2[MAX_LEN_STR];
	int winnerID;
} Match;

typedef struct MatchNode {
	Match match;
	struct MatchNode* next;
	struct MatchNode* prev;
} MatchNode;

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
//Game
void registerMatch(PlayerNode*, CharacterNode*, struct MatchNode**);
MatchNode* createMatchNode(Match);
void saveMatchesToFile(struct MatchNode*);
void printAllMatches(struct MatchNode*);
PlayerNode* findPlayerByID(PlayerNode*, int);
CharacterNode* findCharacterByName(CharacterNode*, const char*);
CharacterNode* getRandomAvailableCharacter(CharacterNode*, char[], char[]);
void saveCharactersToFile(CharacterNode*);
void freeCharacters(CharacterNode*);
/*
......... MAIN ....................
*/
int main(){
	PlayerNode *headPlayer = NULL;
	CharacterNode *headCharacter = NULL;
	MatchNode *matchList = NULL;
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	savePlayersToFile(headPlayer);
	printAllPlayers(headPlayer);

	loadCharactersFromFile(&headCharacter);
	registerMatch(headPlayer, headCharacter, &matchList);
	saveMatchesToFile(matchList);
	printAllMatches(matchList);
	
	freePlayers(headPlayer);
	freeCharacters(headCharacter);
	return 0;
}

/*
......... PLAYER FUNCTIONS ........
*/
int lastCharacterID=0;
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

//-------------------------------------------------//
MatchNode* createMatchNode(Match match) {
	MatchNode* newNode = (MatchNode*)malloc(sizeof(MatchNode));
	newNode->match = match;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

PlayerNode* findPlayerByID(PlayerNode* head, int id) {
	while (head != NULL) {
		if (head->player.id == id)
			return head;
		head = head->next;
	}
	return NULL;
}

CharacterNode* findCharacterByName(CharacterNode* head, const char* name) {
	while (head != NULL) {
		if (strcmp(head->character.name, name) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}

CharacterNode* getRandomAvailableCharacter(CharacterNode* head, char excluded1[], char excluded2[]) {
	CharacterNode* candidates[100];
	int count = 0;
	while (head != NULL) {
		if (strcmp(head->character.name, excluded1) != 0 &&
			strcmp(head->character.name, excluded2) != 0) {
			candidates[count++] = head;
		}
		head = head->next;
	}
	if (count == 0) return NULL;
	int index = rand() % count;
	return candidates[index];
}

void registerMatch(PlayerNode* players, CharacterNode* characters, MatchNode** matchList) {
	if (players == NULL || players->next == NULL || characters == NULL) {
		printf("No hay suficientes jugadores o personajes para iniciar una partida.\n");
		return;
	}

	int id1, id2;
	printf("\nIngrese el ID del Jugador 1: ");
	scanf("%d", &id1);
	printf("Ingrese el ID del Jugador 2: ");
	scanf("%d", &id2);
	cleanBuffer();

	if (id1 == id2) {
		printf("No puedes enfrentar al mismo jugador.\n");
		return;
	}

	PlayerNode* p1 = findPlayerByID(players, id1);
	PlayerNode* p2 = findPlayerByID(players, id2);

	if (!p1 || !p2) {
		printf("Uno o ambos jugadores no existen.\n");
		return;
	}

	srand(time(NULL));
	CharacterNode* char1 = getRandomAvailableCharacter(characters, "", "");
	CharacterNode* char2 = getRandomAvailableCharacter(characters, char1->character.name, "");

	if (!char1 || !char2) {
		printf("No hay suficientes personajes unicos.\n");
		return;
	}

	printf("\n%s jugará con %s\n", p1->player.nickname, char1->character.name);
	printf("%s jugará con %s\n", p2->player.nickname, char2->character.name);

	int winner;
	do {
		printf("Ingrese ID del ganador (%d o %d): ", id1, id2);
		scanf("%d", &winner);
	} while (winner != id1 && winner != id2);

	// Actualizar estadísticas
	if (winner == id1) {
		p1->player.victories++;
		char1->character.victories++;
	} else {
		p2->player.victories++;
		char2->character.victories++;
	}

	Match match = {id1, id2, "", "", winner};
	strcpy(match.character1, char1->character.name);
	strcpy(match.character2, char2->character.name);

	MatchNode* newMatch = createMatchNode(match);
	if (*matchList == NULL) {
		*matchList = newMatch;
	} else {
		MatchNode* current = *matchList;
		while (current->next != NULL)
			current = current->next;
		current->next = newMatch;
		newMatch->prev = current;
	}

	printf("Partida registrada exitosamente.\n");
}

void saveMatchesToFile(MatchNode* head) {
	FILE* file = fopen("matches.txt", "w");
	if (!file) {
		perror("No se pudo abrir el archivo de partidas");
		return;
	}

	while (head != NULL) {
		fprintf(file, "%d|%d|%s|%s|%d\n",
			head->match.player1ID,
			head->match.player2ID,
			head->match.character1,
			head->match.character2,
			head->match.winnerID);
		head = head->next;
	}

	fclose(file);
	printf("Partidas guardadas en 'matches.txt'\n");
}

void printAllMatches(MatchNode* head) {
	while (head != NULL) {
		printf("\nPARTIDA:\n");
		printf("Jugador 1 (ID %d) con %s\n", head->match.player1ID, head->match.character1);
		printf("Jugador 2 (ID %d) con %s\n", head->match.player2ID, head->match.character2);
		printf("Ganador: Jugador ID %d\n", head->match.winnerID);
		head = head->next;
	}
}
