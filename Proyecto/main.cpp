#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// DEFINICION DE TECLAS
#define RIGHT 77
#define LEFT 75
#define DOWN 80 
#define UP 72
#define ENTER 13
#define ESC 27
#define SPACE 2

#define MAX_LEN_STR 50

// HEADERS CREADOS
#include "gotoxy.h"
#include "menu.h"

// ESTRUCTURAS
typedef struct {
    int id;
    char name[MAX_LEN_STR];
    char nickname[MAX_LEN_STR];
    int age;
    int victories;
} Player;

typedef struct PlayerNode {
    Player player;
    struct PlayerNode* next;
    struct PlayerNode* prev;
} PlayerNode;

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

// PROTOTIPOS
void cleanBuffer();
void freePlayers(PlayerNode*);
void freeCharacters(CharacterNode*);

Player createPlayer();
PlayerNode* createPlayerNode(Player);
void addPlayer(PlayerNode**);
void savePlayersToFile(PlayerNode*);
void printAllPlayers(PlayerNode*);
PlayerNode* findPlayerByID(PlayerNode*, int);

CharacterNode* findCharacterByName(CharacterNode*, const char*);
void loadCharactersFromFile(CharacterNode**);
void saveMatchesToFile(MatchNode*);
MatchNode* createMatchNode(Match);
void registerMatch(PlayerNode*, CharacterNode*, MatchNode**);

void topPlayers(PlayerNode*);
void topCharacters(CharacterNode*);

// GLOBALES
int lastPlayerID = 0;

// FUNCION PRINCIPAL
int main() {
    PlayerNode* headPlayer = NULL;
    CharacterNode* headCharacter = NULL;
    MatchNode* matchList = NULL;

    loadCharactersFromFile(&headCharacter);

    int op = 0;
    int x = 3, y = 5;

    getch();
    do {
        presenta_menu(x, y);
        op = scroll_menu(x, y, 4);
        system("cls");

        switch (op) {
            case 1:
                addPlayer(&headPlayer);
                addPlayer(&headPlayer);
                savePlayersToFile(headPlayer);
                break;

            case 2:
                registerMatch(headPlayer, headCharacter, &matchList);
                saveMatchesToFile(matchList);
                break;

            case 3:
                printf("\n--- ESTADISTICAS ---\n");
                printAllPlayers(headPlayer);
                topPlayers(headPlayer);
                topCharacters(headCharacter);
                getch();
                break;

            case 4:
                printf("Saliendo...\n");
                getch();
                break;
        }
    } while (op != 4);

    freePlayers(headPlayer);
    freeCharacters(headCharacter);
    return 0;
}

// FUNCIONES

void cleanBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freePlayers(PlayerNode* head) {
    PlayerNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    printf("\nMemoria de jugadores liberada.\n");
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

Player createPlayer() {
    Player p;
    p.id = ++lastPlayerID;
    printf("Nombre: ");
    fgets(p.name, MAX_LEN_STR, stdin);
    strtok(p.name, "\n");

    printf("Nickname: ");
    fgets(p.nickname, MAX_LEN_STR, stdin);
    strtok(p.nickname, "\n");

    printf("Edad: ");
    scanf("%d", &p.age);
    cleanBuffer();

    p.victories = 0;
    return p;
}

PlayerNode* createPlayerNode(Player p) {
    PlayerNode* newNode = (PlayerNode*)malloc(sizeof(PlayerNode));
    newNode->player = p;
    newNode->next = newNode->prev = NULL;
    return newNode;
}

void addPlayer(PlayerNode** head) {
    Player p = createPlayer();
    PlayerNode* newNode = createPlayerNode(p);

    if (*head == NULL) {
        *head = newNode;
    } else {
        PlayerNode* tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = newNode;
        newNode->prev = tmp;
    }
}

void savePlayersToFile(PlayerNode* head) {
    FILE* f = fopen("players.txt", "w");
    if (!f) return;

    while (head) {
        fprintf(f, "%d;%s;%s;%d;%d\n", head->player.id,
            head->player.name,
            head->player.nickname,
            head->player.age,
            head->player.victories);
        head = head->next;
    }
    fclose(f);
}

void printAllPlayers(PlayerNode* head) {
    printf("\n--- Lista de jugadores ---\n");
    while (head) {
        printf("ID: %d | Nombre: %s | Nickname: %s | Edad: %d | Victorias: %d\n",
            head->player.id,
            head->player.name,
            head->player.nickname,
            head->player.age,
            head->player.victories);
        head = head->next;
    }
}

void loadCharactersFromFile(CharacterNode** head) {
    FILE* f = fopen("characters.txt", "r");
    if (!f) return;

    Character ch;
    while (fscanf(f, "%d;%[^;];%d\n", &ch.id, ch.name, &ch.victories) == 3) {
        CharacterNode* newNode = (CharacterNode*)malloc(sizeof(CharacterNode));
        newNode->character = ch;
        newNode->next = NULL;
        newNode->prev = NULL;

        if (*head == NULL) {
            *head = newNode;
        } else {
            CharacterNode* tmp = *head;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = newNode;
            newNode->prev = tmp;
        }
    }
    fclose(f);
}

PlayerNode* findPlayerByID(PlayerNode* head, int id) {
    while (head) {
        if (head->player.id == id)
            return head;
        head = head->next;
    }
    return NULL;
}

CharacterNode* findCharacterByName(CharacterNode* head, const char* name) {
    while (head) {
        if (strcmp(head->character.name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

MatchNode* createMatchNode(Match m) {
    MatchNode* newNode = (MatchNode*)malloc(sizeof(MatchNode));
    newNode->match = m;
    newNode->next = newNode->prev = NULL;
    return newNode;
}

void registerMatch(PlayerNode* pList, CharacterNode* cList, MatchNode** matchList) {
    Match m;
    printf("ID del jugador 1: ");
    scanf("%d", &m.player1ID);
    printf("ID del jugador 2: ");
    scanf("%d", &m.player2ID);
    cleanBuffer();

    printf("Nombre del personaje de jugador 1: ");
    fgets(m.character1, MAX_LEN_STR, stdin);
    strtok(m.character1, "\n");

    printf("Nombre del personaje de jugador 2: ");
    fgets(m.character2, MAX_LEN_STR, stdin);
    strtok(m.character2, "\n");

    printf("ID del ganador: ");
    scanf("%d", &m.winnerID);
    cleanBuffer();

    PlayerNode* winner = findPlayerByID(pList, m.winnerID);
    if (winner) winner->player.victories++;

    CharacterNode* ch1 = findCharacterByName(cList, m.character1);
    if (ch1) ch1->character.victories++;

    CharacterNode* ch2 = findCharacterByName(cList, m.character2);
    if (ch2) ch2->character.victories++;

    MatchNode* newNode = createMatchNode(m);
    if (*matchList == NULL) {
        *matchList = newNode;
    } else {
        MatchNode* tmp = *matchList;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = newNode;
        newNode->prev = tmp;
    }
}

void saveMatchesToFile(MatchNode* head) {
    FILE* f = fopen("matches.txt", "w");
    if (!f) return;

    while (head) {
        fprintf(f, "%d;%d;%s;%s;%d\n",
            head->match.player1ID,
            head->match.player2ID,
            head->match.character1,
            head->match.character2,
            head->match.winnerID);
        head = head->next;
    }
    fclose(f);
}

// ESTADISTICAS

void topPlayers(PlayerNode* head) {
    int count = 0;
    PlayerNode* arr[100];

    while (head) {
        arr[count++] = head;
        head = head->next;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (arr[j]->player.victories > arr[i]->player.victories) {
                PlayerNode* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    printf("\nTop 10 jugadores con mas victorias:\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%d. %s (%s) - %d victorias\n", i + 1,
            arr[i]->player.name,
            arr[i]->player.nickname,
            arr[i]->player.victories);
    }
}

void topCharacters(CharacterNode* head) {
    int count = 0;
    CharacterNode* arr[100];

    while (head) {
        arr[count++] = head;
        head = head->next;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (arr[j]->character.victories > arr[i]->character.victories) {
                CharacterNode* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    printf("\nTop 5 personajes con mas victorias:\n");
    for (int i = 0; i < count && i < 5; i++) {
        printf("%d. %s - %d victorias\n", i + 1,
            arr[i]->character.name,
            arr[i]->character.victories);
    }
}

