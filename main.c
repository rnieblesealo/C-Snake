#include <stdio.h>
#include <stdlib.h>

#define X_SIZE 10
#define Y_SIZE 10

typedef struct Entity{
	int posX;
	int posY;
} Entity;

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

int IsActiveEntity(Entity e){
	// Active entities are those whose x and y coordinates are both -1
	return (e.posX == -1 && e.posY == -1) ? 0 : 1;
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
		puts("Here!");
		DISPLAY[entities[i].posY][entities[i].posX] = icon;
	}
}

void Display(char** DISPLAY){
	// Show display
	for (int y = 0; y < Y_SIZE; ++y){
		for (int x = 0; x < X_SIZE; ++x){
			printf(" %c ", DISPLAY[y][x]);
		}
		putchar('\n');
	}
}

int main(){
	// For now, all this terminal really does is print out the snake. Neat!
	Entity* snake = InitializeSnake(X_SIZE / 2, Y_SIZE / 2);

	char** DISPLAY = InitializeDisplay();
	
	DisplayEntities(DISPLAY, snake, '*');
	Display(DISPLAY);

	return 0;
}
