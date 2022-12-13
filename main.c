#include <stdio.h>
#include <stdlib.h>

#define X_SIZE 10
#define Y_SIZE 10

#define K_UP 'w'
#define K_DOWN 's'
#define K_LEFT 'a'
#define K_RIGHT 'd'

typedef struct Entity{
	int posX;
	int posY;
} Entity;

int IsActiveEntity(Entity e){
	// Active entities are those whose x and y coordinates are both -1
	return (e.posX == -1 && e.posY == -1) ? 0 : 1;
}

Entity* InitializeSnake(int headPosX, int headPosY){
	int snakeLength = X_SIZE * Y_SIZE;

	// Allocate snake nodes to heap; max theoretical size is size of grid, declared above
	Entity* snake = (Entity*)malloc(snakeLength * sizeof(Entity));

	// Every snake node's position is negative; this means snake node is not active
	for (int i = 0; i < snakeLength; ++i){
		snake[i].posX = -1;
		snake[i].posY = -1;
	}

	// Initialize head of snake
	snake[0].posX = headPosX;
	snake[0].posY = headPosY;

	// Return snake array
	return snake;
}

void MoveSnake(Entity* snake, int dx, int dy){
	// Move head node first
	int lastPosX = snake[0].posX;
	int lastPosY = snake[0].posY;
	
	snake[0].posX += dx;
	snake[0].posY += dy;

	for (int i = 1; IsActiveEntity(snake[i]); ++i){
		// Move every node to the position of the node ahead of it before it was moved
		int cacheX = snake[i].posX;
		int cacheY = snake[i].posY;

		snake[i].posX = lastPosX;
		snake[i].posY = lastPosY;
	
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
	snake[i].posX = snake[i - 1].posX - 1;
	snake[i].posY = snake[i - 1].posY;
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

void ClearDisplay(char** DISPLAY){
	// Clear the terminal; WARNING, this is NOT portable! Does not work on Windows.
	system("clear");
	
	// Make every entry in the display empty space
	for (int y = 0; y < Y_SIZE; ++y){
		for (int x = 0; x < X_SIZE; ++x){
			DISPLAY[y][x] = ' ';
		}
	}
}

void DisplayEntities(char** DISPLAY, Entity* entities, char icon){
	// Add entities to display using an icon
	for (int i = 0; IsActiveEntity(entities[i]); ++i){
		DISPLAY[entities[i].posY][entities[i].posX] = icon;
	}
}

void Display(char** DISPLAY){		
	// Show display
	for (int y = 0; y < Y_SIZE; ++y){
		// Draw left bound
		putchar('|');
		
		// Draw row
		for (int x = 0; x < X_SIZE; ++x){
			printf(" %c ", DISPLAY[y][x]);
		}
		
		// Draw right bound and newline
		puts("|");
	}
}

int main(){
	// Initialize display
	char** DISPLAY = InitializeDisplay();
	
	// Initialize snake
	Entity* snake = InitializeSnake(X_SIZE / 2, Y_SIZE / 2);

	// Run display once, begin by drawing snake
	DisplayEntities(DISPLAY, snake, '*');

	// Show display
	Display(DISPLAY);
		
	// Begin main loop
	int running = 1;
	while(running){
		// Safely collect input by grabbing first character of entire line from stdin such that the line is removed from the buffer
		char line[1000];
		char input;
		
		fgets(line, 1000, stdin);
		input = line[0];

		// Match input to move snake
		switch (input){
			case K_UP:
				MoveSnake(snake, 0, -1);
				break;
			case K_DOWN:
				MoveSnake(snake, 0, 1);
				break;
			case K_LEFT:
				MoveSnake(snake, -1, 0);
				break;
			case K_RIGHT:
				MoveSnake(snake, 1, 0);
				break;
			default:
				GrowSnake(snake); // TODO Remove, this is for testing
				break;
		}

		// Clear display
		ClearDisplay(DISPLAY);
		
		// Draw snake
		DisplayEntities(DISPLAY, snake, '*');
		
		// Display!
		Display(DISPLAY);
	}

	return 0;
}
