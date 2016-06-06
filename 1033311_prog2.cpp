#include<iostream>
#include<ctime>
using namespace std;
#define MAX_STACK_SIZE 1000 /*maximum stack size*/
typedef struct {
	short int row;
	short int col;
	short int dir;
} element;
element stack[MAX_STACK_SIZE];

typedef struct {
	short int vert;
	short int horiz;
} coordinate;
bool stack_full(int *top){
	if (*top >= MAX_STACK_SIZE - 1){
		cerr << "stack is full" << endl;
		return 1;
	}
	else
		return 0;
}
bool stack_empty(int *top){
	if (*top < 0){
		cerr << "stack is emty" << endl;
		return 1;
	}
	else
		return 0;
}
void push(int *top, element item){
	if (stack_full(top) == 1){
		exit(true);
	}
	else
		stack[++*top] = item;
}
element pop(int *top){
	if (stack_empty(top) == 1){
		exit(true);
	}
	else
		return stack[(*top)--];
}

void path(){
	int m, n;
	cout << "input your number of columns and rows of the maze" << endl;
	cin >> m >> n;
	int **maze = NULL;   
	maze = new int*[m + 2];
	for (int i = 0; i < m + 2; i++) maze[i] = new int[n + 2];   //the origin maze with 1 frame, initialize the matrix
	for (int i = 0; i < m + 2; i++){
		for (int j = 0; j < n + 2; j++){
			maze[i][j] = 1;
		}
	}
	srand(time(0));
	for (int i = 1; i < m + 1; i++){	//randomly change the matrix
		for (int j = 1; j < n + 1; j++){
			maze[i][j] = rand() % 2;
		}
	}
	maze[1][1] = 0; maze[m][n] = 0;
	for (int i = 0; i < m + 2; i++){
		for (int j = 0; j < n + 2; j++){
			cout << maze[i][j];
		}
		cout << endl;
	}

	int **mark = NULL;     // to mark where have been passed
	mark = new int*[m + 2];
	for (int i = 0; i < m + 2; i++) mark[i] = new int[n + 2];
	for (int i = 0; i < m + 2; i++){
		for (int j = 0; j < n + 2; j++){
			mark[i][j] = 0;
		}
	}

	coordinate move[8];    // there is eight ways to move, the number of move donates the change of the coordianate 
	move[0].vert = -1;  move[0].horiz =  0;
	move[1].vert = -1;  move[1].horiz =  1;
	move[2].vert =  0;  move[2].horiz =  1;
	move[3].vert =  1;  move[3].horiz =  1;
	move[4].vert =  1;  move[4].horiz =  0;
	move[5].vert =  1;  move[5].horiz = -1;
	move[6].vert =  0;  move[6].horiz = -1;
	move[7].vert = -1;  move[7].horiz = -1;
		
	int exit_col = n, exit_row = m;
	int row, col, next_row, next_col, dir, found = false;
	int top = 0;
	element position;
	mark[1][1] = 1;
	stack[0].row = 1, stack[0].col = 1; stack[0].dir = 0;
	while (top > -1 && !found){                                   // start finding path
		position = pop(&top);
		row = position.row, col = position.col, dir = position.dir;
		while (dir < 8 && !found){
			next_row = row + move[dir].vert;
			next_col = col + move[dir].horiz;
			if (next_row == exit_row && next_col == exit_col) //found the terminal
				found = true;
			else if (!maze[next_row][next_col] && !mark[next_row][next_col]){ 
				mark[next_row][next_col] = 1;// mark the position that already being visited
				position.row = row, position.col = col, position.dir = dir++;
				push(&top, position);//put the path into stack
				row = next_row, col = next_col, dir = 0;
			}
			else
				dir++; //try another direction
		}
	}

	if (found){
		cout << "The path is:" << endl;
		cout << "Row\tColumn" << endl;
		cout << "---------------" << endl;
		for (int i = 0; i <= top; i++)
			cout << stack[i].row << "\t" << stack[i].col << endl;
		cout << row << "\t" << col << endl;
		cout << exit_row << "\t" << exit_col << endl;
	}
	else
		cout << "This maze does not have a path" << endl;

}

int main(){
	path();
	system("pause");
	return 0;
}