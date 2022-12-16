#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Grid Size
#define X_SIZE 8
#define Y_SIZE 8

// Key Codes
#define K_UP 'w'
#define K_DOWN 's'
#define K_LEFT 'a'
#define K_RIGHT 'd'

// Directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define NONE 0

// Input Length
#define MAX_INPUT_LEN 32

// Colors
#define B_RED "\e[1;31m"
#define B_GREEN "\e[1;32m"
#define B_WHITE "\e[1;37m"
#define B_YELLOW "\e[1;33m"
#define C_RESET "\e[0m"

// Skins
#define S_SNAKE '*'
#define S_FRUIT 'O'
#define S_DEDGE '|'
#define S_PROMPT ':'

// Score
#define SCORE_PER_FRUIT 25

// Structures
typedef struct Vector2{
	int x;
	int y;
} Vector2;

typedef struct Entity{
	char skin;
	Vector2 position;
} Entity;

// Constructors
Vector2 new_Vector2( int x, int y );

// Checkers
int IsActiveEntity( Entity e );
int IsColliding( Entity a, Entity b );
int IsAtPosition( Entity a, int x, int y );

// Display Functions
char** InitializeDisplay();
void Display( char** display );
void ClearDisplay( char** display );
void DisplayScore( int score );
void DisplayPrompt();

// Draw Functions
void DrawEntity( char** display, Entity* entity );
void DrawEntities( char** display, Entity* entities );

// Fruit Functions
Entity* InitializeFruit( Entity* fruit, char skin );
Vector2 PickFruitPosition( Entity* snake );

// Snake Functions
Entity* InitializeSnake( int headPosX, int headPosY, char skin );
void MoveSnake( Entity* snake, int direction );
void GrowSnake( Entity* snake );

int main(){
	// Seed random number generator
	srand(time(NULL));
	
	// Initialize display
	char** display = InitializeDisplay();
	
	// Initialize snake
	Entity* snake = InitializeSnake(X_SIZE / 2, Y_SIZE / 2, '*');
	
	// Set last move direction (used to disallow moving snake into itself)
	int lastMoveDirection = NONE;

	// Initialize fruit
	Entity* fruit = InitializeFruit(snake, 'O');

	// Initialize score
	int score = 0;
	
	// Run all display functions once to show game before taking input
	DrawEntity(display, fruit);
	DrawEntities(display, snake);

	DisplayScore(score);
	Display(display);
	DisplayPrompt();

	// Begin main loop
	int running = 1;
	while (running){		
		// Safely collect input by grabbing first character of entire line from stdin such that the line is removed from the buffer
		char inputLine[MAX_INPUT_LEN];
		char inputKey = 0;
		
		fgets(inputLine, MAX_INPUT_LEN, stdin);
		inputKey = inputLine[0];

		// Match input to move snake
		switch (inputKey){
			case K_UP:
				MoveSnake(snake, UP);
				break;
			case K_DOWN:
				MoveSnake(snake, DOWN);
				break;
			case K_LEFT:
				MoveSnake(snake, LEFT);
				break;
			case K_RIGHT:
				MoveSnake(snake, RIGHT);
				break;
			default:
				break;
		}
		
		// Check if snake has hit a wall (head is out of bounds)
		if (snake[0].position.x < 0 || snake[0].position.x >= X_SIZE || snake[0].position.y < 0 || snake[0].position.y >= Y_SIZE){
			ClearDisplay(display);
			puts(B_RED "Snake has hit a wall!" C_RESET);
			printf("Your score:" B_YELLOW " %d\n" C_RESET, score);
			return 1;
		}
		
		// Check if snake head collides with any of its nodes
		for (int i = 1; IsActiveEntity(snake[i]); ++i){
			if (IsColliding(snake[0], snake[i])){
				ClearDisplay(display);
				puts(B_RED "Snake crashed into itself!" C_RESET);
				printf("Your score:" B_YELLOW " %d\n" C_RESET, score);
				return 1;
			}
		};
		
		// Check if snake head is colliding with fruit; if so, grow snake and move fruit and add score
		if (IsAtPosition(snake[0], fruit->position.x, fruit->position.y)){
			fruit->position = PickFruitPosition(snake);
			GrowSnake(snake);
			score += SCORE_PER_FRUIT;
		}	

		// Clear display
		ClearDisplay(display);

		// Draw snake and fruit
		DrawEntity(display, fruit);
		DrawEntities(display, snake);
		
		// Display!
		DisplayScore(score);
		Display(display);
		DisplayPrompt();
	}

	return 0;
}

Vector2 new_Vector2(int x, int y){
	Vector2 n;
	
	n.x = x;
	n.y = y;
	
	return n;
}

int IsActiveEntity(Entity e){
	// Active entities are those whose x and y coordinates are both -1
	return !(e.position.x == -1 && e.position.y == -1);
}

int IsColliding(Entity a, Entity b){
	// Are entities a and b colliding? (Do they have the same position)
	return (a.position.x == b.position.x && a.position.y == b.position.y);
}

int IsAtPosition(Entity a, int x, int y){
	// Is entity a at position?
	return (a.position.x == x && a.position.y == y);
}

char** InitializeDisplay(){
	// Allocate columns
	char** columns = (char**)malloc(Y_SIZE * sizeof(char*));
	
	// Allocate rows
	for (int y = 0; y < Y_SIZE; ++y){
		columns[y] = (char*)malloc(X_SIZE * sizeof(char));
		
		// Assign empty space to rows
		for (int x = 0; x < X_SIZE; ++x){
			columns[y][x] = ' ';
		}
	}

	return columns;
}

void Display(char** display){		
	// Show display
	for (int y = 0; y < Y_SIZE; ++y){
		// Draw left bound
		printf(B_WHITE "%c" C_RESET, S_DEDGE);
		
		// Draw row
		for (int x = 0; x < X_SIZE; ++x){
			// Check if char matches skin of snake or fruit!
			switch (display[y][x]){
				case S_SNAKE:
					printf(B_GREEN " %c " C_RESET, S_SNAKE);
					break;
				case S_FRUIT:
					printf(B_RED " %c " C_RESET, S_FRUIT);
					break;
				default:
					printf(" %c ", display[y][x]);
			}	
		}
		
		// Draw right bound and newline
		printf(B_WHITE "%c" C_RESET "\n", S_DEDGE);
	}
}

void ClearDisplay(char** display){
	// Clear the terminal; WARNING, this is NOT portable! Does not work on Windows.
	system("clear");
	
	// Make every entry in the display empty space
	for (int y = 0; y < Y_SIZE; ++y){
		for (int x = 0; x < X_SIZE; ++x){
			display[y][x] = ' ';
		}
	}
}

void DisplayScore(int score){
	// Display current score :)
	printf("[ SCORE: " B_YELLOW "%d" C_RESET " ]\n", score);
}

void DisplayPrompt(){
	// Print a prompt before user enters their command
	printf(B_YELLOW "%c" C_RESET, S_PROMPT);
}

void DrawEntity(char** display, Entity* entity){
	// Add single dynamic entity to display
	display[entity->position.y][entity->position.x] = entity->skin;
}

void DrawEntities(char** display, Entity* entities){
	// Add entities to display using an icon
	for (int i = 0; IsActiveEntity(entities[i]); ++i){
		display[entities[i].position.y][entities[i].position.x] = entities[i].skin;
	}
}

Entity* InitializeFruit(Entity* snake, char skin){
	// Allocate fruit
	Entity* fruit = (Entity*)malloc(sizeof(Entity));

	// Pick random position for fruit
	fruit->position = PickFruitPosition(snake);
	fruit->skin = skin;

	return fruit;
}

Vector2 PickFruitPosition(Entity* snake){
	// Generate table of positions
	Vector2 positions[X_SIZE * Y_SIZE];

	// Go over every position and add it to table IF it doesn't overlap with the snake 
	int i = 0;
	for (int y = 0; y < Y_SIZE; ++y){
		for (int x = 0; x < X_SIZE; ++x){
			int isOccupied = 0;
			for (int j = 0; IsActiveEntity(snake[j]); ++j){
				// If current (x, y) matches any of snake's (x, y), this position is occupied and we do not need to perform any other checks
				if (IsAtPosition(snake[j], x, y)){
					isOccupied = 1;
					break;
				}
			}
		
			// Check if position is not occupied, if so, add to the list!
			if (!isOccupied){
				positions[i] = new_Vector2(x, y);
				i++;
			}
		}
	}

	// Pick random position and return it
	return positions[rand() % i];
}

Entity* InitializeSnake(int headPosX, int headPosY, char skin){
	int snakeLength = X_SIZE * Y_SIZE;

	// Allocate snake nodes to heap; max theoretical size is size of grid, declared above
	Entity* snake = (Entity*)malloc(snakeLength * sizeof(Entity));

	// Every snake node's position is negative; this means snake node is not active
	for (int i = 0; i < snakeLength; ++i){
		snake[i].position.x = -1;
		snake[i].position.y = -1;
		snake[i].skin = skin;
	}

	// Initialize head of snake
	snake[0].position.x = headPosX;
	snake[0].position.y = headPosY;

	// Return snake array
	return snake;
}

void MoveSnake(Entity* snake, int direction){
	// Determine dx and dy from direction
	int dx = 0;
	int dy = 0;

	switch (direction){
		case UP:
			dx = 0;
			dy = -1;
			break;
		case DOWN:
			dx = 0;
			dy = 1;
			break;
		case LEFT:
			dx = -1;
			dy = 0;
			break;
		case RIGHT:
			dx = 1;
			dy = 0;
			break;
		default:
			return;
	}
	
	// Check if we aren't moving in opposite direction (towards the node after the head; the neck)
	int neckExists = IsActiveEntity(snake[1]);
	int dxOverlap = (snake[1].position.x == snake[0].position.x + dx);
       	int dyOverlap = (snake[1].position.y == snake[0].position.y + dy);	
	
	if (neckExists && dxOverlap && dyOverlap){
		return;
	}

	// Move head node first
	int lastPosX = snake[0].position.x;
	int lastPosY = snake[0].position.y;
	
	snake[0].position.x += dx;
	snake[0].position.y += dy;

	for (int i = 1; IsActiveEntity(snake[i]); ++i){
		// Move every node to the position of the node ahead of it before it was moved
		int cacheX = snake[i].position.x;
		int cacheY = snake[i].position.y;

		snake[i].position.x = lastPosX;
		snake[i].position.y = lastPosY;
	
		lastPosX = cacheX;
		lastPosY = cacheY;
	}
}

void GrowSnake(Entity* snake){
	// Get to first inactive snake node
	int i = 0;
	while (IsActiveEntity(snake[i])){
		i++;
	}
	
	// TODO Figure out direction of new snake node; for now, just place it at left of previous node
	snake[i].position.x = snake[i - 1].position.x - 1;
	snake[i].position.y = snake[i - 1].position.y;
}
