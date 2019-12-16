#pragma once
#define CON_BUFFER_SIZE	255
#define MIN_MATRIX_SIZE	1
#define MAX_MATRIX_SIZE	10
#define MAX_PARAM_COUNT	5

typedef struct Memory	Memory;
typedef struct Matrix	Matrix;
typedef struct Menu		Menu;
typedef struct Command	Command;
typedef struct Parsed	Parsed;
typedef struct Node		Node;

#define MENU_PARAMS	Memory* memory
#define CMD_PARAMS	Memory* memory, Parsed* parsed

struct Matrix
{
	int rows;
	int cols;
	float* data;
};

struct Parsed
{
	char* name;
	char argcount;
	char* args[MAX_PARAM_COUNT];
};

struct Command
{
	char* name; // K���k harflerden ve alt �izgi karakterinden olu�mal�d�r.
	char* help;
	void (*function)(CMD_PARAMS);
};

struct Menu
{
	char* title;
	char* options[10];
	void (*functions[10])(MENU_PARAMS);
};

struct Node
{
	Node* prev;
	Node* next;
	Matrix* matrix;
	char name;
};

struct Memory
{
	Node* tail;
	Matrix* matrix;
	Menu* home;
	Command** commands;
};

Memory* init_mem();
void free_mem(Memory* memory);

Node* mem_new(Memory* memory, char name, int rows, int cols, float* data);
Node* mem_add(Memory* memory, char name, Matrix* matrix);
Node* mem_query(Memory* memory, char name);
void mem_remove(Memory* memory, Node* node);