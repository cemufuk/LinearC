#include "Console.h"
#include "Matrix.h"
#include "Menu.h"
#include "Utils.h"

Memory* init_mem(void)
{
	Memory* memory = calloc(1, sizeof(Memory));

	if (!memory)
		return NULL;

	// Men�y� olu�tur
	{
		memory->home.title = "LineerC | Mustafa Nesin & Cem Ufuk Yilmaz";

		memory->home.options[0] = "Cikis";
		memory->home.options[1] = "Yeni Matris Tanimla";
		memory->home.options[2] = "Matris Listesi";
		memory->home.options[3] = "Islemler";
		memory->home.options[4] = "Denklem Cozucu";
		memory->home.options[5] = "Kaydet";
		memory->home.options[6] = "Yukle";

		memory->home.functions[0] = NULL;
		memory->home.functions[1] = menu_define;
		memory->home.functions[2] = menu_list;
		memory->home.functions[3] = menu_console;
		memory->home.functions[4] = menu_equation;
		memory->home.functions[5] = menu_save;
		memory->home.functions[6] = menu_read;
	}

	// Komutlar� olu�tur
	{
		int c = -1;

		CMD_INIT("return", NULL);
		CMD_INIT("clear", cmd_clear);

		#pragma region Matrix
		CMD_INIT_M("t", cmd_transpose);
		CMD_INIT_M("inv", cmd_inverse);
		CMD_INIT_M("det", cmd_determinant);
		CMD_INIT_M("adj", cmd_adjoint);
		CMD_INIT_M2("r", "rank", cmd_rank);
		BEGIN_CMD_INIT; /* pow */ {
			memory->commands[c].name = "pow";
			memory->commands[c].function = cmd_power;
			memory->commands[c].returns = true;
			memory->commands[c].paramcount = 2;
			memory->commands[c].params[0] = CMD_MATRIX_PARAM;
			memory->commands[c].params[1] = CMD_SCALAR_PARAM;
		}
		BEGIN_CMD_INIT; /* pow */ {
			memory->commands[c].name = "pow";
			memory->commands[c].function = cmd_power;
			memory->commands[c].returns = true;
			memory->commands[c].paramcount = 2;
			memory->commands[c].params[0] = CMD_SCALAR_PARAM;
			memory->commands[c].params[1] = CMD_SCALAR_PARAM;
		}
		#pragma endregion

		#pragma region Math
		CMD_INIT2("deg", "degree", cmd_deg);
		CMD_INIT2("rad", "radian", cmd_rad);
		CMD_INIT_0("pi", cmd_pi);
		CMD_INIT_0("e", cmd_e);
		CMD_INIT_S("sqrt", cmd_sqrt);
		CMD_INIT_S("ln", cmd_ln);
		CMD_INIT_S("log", cmd_log);
		BEGIN_CMD_INIT; /* log */ {
			memory->commands[c].name = "log";
			memory->commands[c].function = cmd_log;
			memory->commands[c].returns = true;
			memory->commands[c].paramcount = 2;
			memory->commands[c].params[0] = CMD_SCALAR_PARAM;
			memory->commands[c].params[1] = CMD_SCALAR_PARAM;
		}
		CMD_INIT_S("sin", cmd_sin);
		CMD_INIT_S("cos", cmd_cos);
		CMD_INIT_S("tan", cmd_tan);
		CMD_INIT_S("cot", cmd_cot);
		CMD_INIT_S("sec", cmd_sec);
		CMD_INIT_S("csc", cmd_csc);
		CMD_INIT_S("sinh", cmd_sinh);
		CMD_INIT_S("cosh", cmd_cosh);
		CMD_INIT_S("tanh", cmd_tanh);
		CMD_INIT_S("coth", cmd_coth);
		CMD_INIT_S("sech", cmd_sech);
		CMD_INIT_S("csch", cmd_csch);
		CMD_INIT_S2("asin", "arcsin", cmd_asin);
		CMD_INIT_S2("acos", "arccos", cmd_acos);
		CMD_INIT_S2("atan", "arctan", cmd_atan);
		CMD_INIT_S2("acot", "arccot", cmd_acot);
		CMD_INIT_S2("asec", "arcsec", cmd_asec);
		CMD_INIT_S2("acsc", "arccsc", cmd_acsc);
		CMD_INIT_S2("asinh", "arcsinh", cmd_asinh);
		CMD_INIT_S2("acosh", "arccosh", cmd_acosh);
		CMD_INIT_S2("atanh", "arctanh", cmd_atanh);
		CMD_INIT_S2("acoth", "arccoth", cmd_acoth);
		CMD_INIT_S2("asech", "arcsech", cmd_asech);
		CMD_INIT_S2("acsch", "arccsch", cmd_acsch);
		#pragma endregion
	}

	return memory;
}

void free_mem(Memory* memory)
{
	mem_remove_all(memory);
	free(memory);
}

Node* mem_new(Memory* memory, char name, uint8_t rows, uint8_t cols, float* data)
{
	Matrix* matrix = mx_new(rows, cols, data);
	if (!matrix)
		return NULL;

	Node* node = mem_add(memory, name, matrix);
	if (!node)
	{
		free(matrix);
		return NULL;
	}

	return node;
}

Node* mem_add(Memory* memory, char name, Matrix* matrix)
{
	if (!memory)
		return NULL;

	Node* node = calloc(1, sizeof(Node));
	if (!node)
		return NULL;

	node->name = name;
	node->matrix = matrix;

	if (memory->tail)
	{
		memory->tail->next = node;
		node->prev = memory->tail;
	}

	return memory->tail = node;
}

Node* mem_query(Memory* memory, char name)
{
	if (!memory)
		return NULL;

	Node* node = memory->tail;
	while (node)
	{
		if (node->name == name)
			break;
		node = node->prev;
	}
	return node;
}

void mem_remove(Memory* memory, Node* node)
{
	if (!memory || !node)
		return;

	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		memory->tail = node->prev;

	mx_free(node->matrix);
	free(node);
}

void mem_remove_all(Memory* memory)
{
	while (memory->tail)
		mem_remove(memory, memory->tail);
}

int mem_save(Memory* memory)
{
	FILE* file = fopen("save.bin", "wb");

	if (!file)
		return -1;

	int p;
	uint8_t count = 0, row, col;
	fseek(file, sizeof(count), SEEK_SET);

	Node* node = memory->tail;

	while (node)
	{
		fwrite(&node->name, 1, 1, file);
		fwrite(&node->matrix->rows, 1, 1, file);
		fwrite(&node->matrix->cols, 1, 1, file);

		for (row = 0, p = 0, count++; row < node->matrix->rows; row++)
			for (col = 0; col < node->matrix->cols; col++, p++)
				fwrite(node->matrix->data + p, sizeof(float), 1, file);

		node = node->prev;
	}

	fseek(file, 0, SEEK_SET);
	fwrite(&count, sizeof(count), 1, file);
	fclose(file);

	return count;
}

int mem_read(Memory* memory)
{
	FILE* file = fopen("save.bin", "rb");

	if (!file)
		return -1;

	mem_remove_all(memory);

	uint8_t count;
	if (fread(&count, sizeof(count), 1, file) != 1)
		return -1;

	char name;
	int p;
	uint8_t rows, cols, row, col;
	float* data;

	for (uint8_t c = 0; c < count; c++)
	{
		if (fread(&name, sizeof(name), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		if (fread(&rows, sizeof(rows), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		if (fread(&cols, sizeof(cols), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		data = malloc(rows * cols * sizeof(float));
		if (!data)
		{
			mem_remove_all(memory);
			return -1;
		}

		for (row = 0, p = 0; row < rows; row++)
			for (col = 0; col < cols; col++, p++)
			{
				if (fread(data + p, sizeof(float), 1, file) != 1)
				{
					free(data);
					mem_remove_all(memory);
					return -1;
				}
			}

		mem_new(memory, name, rows, cols, data);
	}

	fclose(file);
	return count;
}