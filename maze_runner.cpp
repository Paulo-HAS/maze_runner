#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <chrono>
#include <thread>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *pf = fopen("/workspaces/maze_runner/data/maze2.txt", "r");
	char ch;

	if (pf == NULL) {
        printf("Failed to open the file.\n");
        return initial_pos;
    }

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(pf, "%d %d", &num_rows, &num_cols);
	printf("Lines | Colluns: %d %d \n", num_rows, num_cols);
	// Aloca a matriz maze (malloc)
	maze = (char**)malloc(num_rows*sizeof(char*));

	for (int i = 0; i < num_rows; ++i)
		// Aloca cada linha da matriz
		*(maze+i) =(char*) malloc(num_cols*sizeof(char));
			/////alocar a matriz primeiro antes!!!!
	printf("Maze OK!\n");
	
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			ch = fgetc(pf);
            if (ch != '\n') {
                maze[i][j] = ch;
				if (ch == 'e') {
					initial_pos.i = i;
					initial_pos.j = j;
				}
            } else {
                --j;
            }
		}
	}
	fclose(pf);
	printf("Initial Position: (%d , %d)\n", initial_pos.i, initial_pos.j);


	using namespace std::this_thread;	//Breve tempo para checar inicialização do labirinto
    using namespace std::chrono; 

	sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(3));


	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	// Marcar a posição atual com o símbolo '.'
	if (maze[pos.i][pos.j] == 's') { // checando se a posisão atual é a saída
        return true;
    }
	maze[pos.i][pos.j] = '.';
	// Limpa a tela
	system("clear");
	// Imprime o labirinto
	print_maze();

	using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds

	sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + milliseconds(25));	//delay de animação (40 fps)
	
	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		cada uma delas no vetor valid_positions
			- pos.i, pos.j+1
			- pos.i, pos.j-1
			- pos.i+1, pos.j
			- pos.i-1, pos.j
		Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	*/
	

	pos_t pos_add;
	if(pos.i>0)
		if(maze[pos.i-1][pos.j] == 'x' || maze[pos.i-1][pos.j] == 's'){
			pos_add.i = pos.i-1;
			pos_add.j = pos.j;
			valid_positions.push(pos_add);
		}
	if(pos.j>0)
		if(maze[pos.i][pos.j-1] == 'x' || maze[pos.i][pos.j-1] == 's'){
			pos_add.i = pos.i;
			pos_add.j = pos.j-1;
			valid_positions.push(pos_add);
		}
	if(pos.i<num_rows-1)
		if(maze[pos.i+1][pos.j] == 'x' || maze[pos.i+1][pos.j] == 's'){
			pos_add.i = pos.i+1;
			pos_add.j = pos.j;
			valid_positions.push(pos_add);
		}
	if(pos.j<num_cols-1)
		if(maze[pos.i][pos.j+1] == 'x' || maze[pos.i][pos.j+1] == 's'){
			pos_add.i = pos.i;
			pos_add.j = pos.j+1;
			valid_positions.push(pos_add);
		}

	// Verifica se a pilha de posições nao esta vazia 
	//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
	// Caso contrario, retornar falso
	bool res = false;
	if (!valid_positions.empty()) {
		pos_t next_position = valid_positions.top();
		valid_positions.pop();
		res = walk(next_position);
	}
	return res;
}

int main(int argc, char* argv[]) {
	system("clear");
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	print_maze();
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	if(exit_found)
		printf("Exit found!\n");
	else	
		printf("No exit found!\n");
	return 0;
}
