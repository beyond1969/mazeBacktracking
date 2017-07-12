/* made by Hyeokjung Kim			*/
/* e-mail : beyond1969@gamil.com	*/

#include <iostream>
#include <fstream>
/* movement */
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'
#define STAY 'S'

using namespace std;

// info for a tile
typedef struct maze_block_info {
	int left;
	int right;
	int up;
	int down;
	int pX, pY;
	bool isLocated[15][15];
} st_mazeBlk;

/* functions */
void print_answer(int latestTurn);
void mark_location();
bool check_overlap();
bool check_state(int state);
void move_minotaur();
void promising(int latestTurn, char move);
void move_theseus(int turn);

/* map maker */
int mazeXSize = 0;
int mazeYSize = 0;
int exitX = -1;
int exitY = -1;
st_mazeBlk** mazeMap = NULL;

/* controller */
st_mazeBlk* theseus = NULL;
st_mazeBlk* minotaur = NULL;

char* arr_theseus_moves = NULL;
int puzzle;
bool only_one_answer;

int main() {
	puzzle = 1;
	ifstream inputFile;
	int tX, tY;
	int mX, mY;
	
	while (true) {
		arr_theseus_moves = new char[1000]();
		only_one_answer = true;
		switch (puzzle) {
		case 1:
			inputFile.open("input1.txt");
			break;
		case 2:
			inputFile.open("input2.txt");
			break;
		case 3:
			inputFile.open("input3.txt");
			break;
		case 4:
			inputFile.open("input4.txt");
			break;
		case 5:
			inputFile.open("input5.txt");
			break;
		default:
			cout << "exit program..." << endl;
			system("pause");
			return 0;
		}

		if (!inputFile.is_open()) {
			//system("cls");
			cout << "open file error! : " << puzzle << endl;
			puzzle++;
			continue;
		}

		/* input and make maze*/
		char c_mazeXSize[3], c_mazeYSize[2], c_mazeBlk[5];
		char c_left[2], c_right[2], c_up[2], c_down[2];
		char c_theseus[5], c_minotaur[5], c_exit[5];
		char c_pX[3], c_pY[3];
		/// input maze x size
		inputFile.getline(c_mazeXSize, 3);
		mazeXSize = atoi(c_mazeXSize);
		/// input maze y size
		inputFile.getline(c_mazeYSize, 2);
		mazeYSize = atoi(c_mazeYSize);
		/// make Map with dynamic array
		mazeMap = new st_mazeBlk*[mazeYSize];
		for (int i = 0; i < mazeYSize; i++) {
			mazeMap[i] = new st_mazeBlk[mazeXSize];
			memset(mazeMap[i], NULL, sizeof(st_mazeBlk)*mazeXSize);
		}
		/// input maze 1 x 1 block info
		for (int y = 0; y < mazeYSize; y++) {
			for (int x = 0; x < mazeXSize; x++) {
				inputFile.getline(c_mazeBlk, 5);
				c_left[0] = c_mazeBlk[0];
				c_right[0] = c_mazeBlk[1];
				c_up[0] = c_mazeBlk[2];
				c_down[0] = c_mazeBlk[3];

				mazeMap[y][x].left = atoi(c_left);
				mazeMap[y][x].right = atoi(c_right);
				mazeMap[y][x].up = atoi(c_up);
				mazeMap[y][x].down = atoi(c_down);
				mazeMap[y][x].pX = x;
				mazeMap[y][x].pY = y;
				mazeMap[y][x].isLocated[y][x] = false;
				//cout << mazeMap[y][x].pX << mazeMap[y][x].pY << mazeMap[y][x].left << mazeMap[y][x].right << mazeMap[y][x].up << mazeMap[y][x].down << endl;
			}
		}
		/* initialize Theseus, Minotaur and exit block location */
		inputFile.getline(c_theseus, 5);
		c_pX[0] = c_theseus[0]; c_pX[1] = c_theseus[1];
		c_pY[0] = c_theseus[2]; c_pY[1] = c_theseus[3];
		tX = atoi(c_pX); tY = atoi(c_pY);
		inputFile.getline(c_minotaur, 5);
		c_pX[0] = c_minotaur[0]; c_pX[1] = c_minotaur[1];
		c_pY[0] = c_minotaur[2]; c_pY[1] = c_minotaur[3];
		mX = atoi(c_pX); mY = atoi(c_pY);
		theseus = &mazeMap[tY][tX];
		minotaur = &mazeMap[mY][mX];
		inputFile.getline(c_exit, 5);
		c_pX[0] = c_exit[0]; c_pX[1] = c_exit[1];
		c_pY[0] = c_exit[2]; c_pY[1] = c_exit[3];
		exitX = atoi(c_pX); exitY = atoi(c_pY);
		// map making end
		
		//cout << minotaur->left << minotaur->right << minotaur->up << minotaur->down << endl;
		//cout << " start : " << theseus->pX << theseus->pY << "," << minotaur->pX << minotaur->pY << endl;

		// start solving...
		mark_location();
		move_theseus(0);

		/* delete map */
		for (int i = 0; i < mazeYSize; i++) {
			delete[] mazeMap[i];
		}
		delete[] mazeMap;
		delete[] arr_theseus_moves;
		inputFile.close();

		puzzle++;
	}
	/*
	for (int i = 0; i < 15; i++) {
		delete[] isLocatedTheseus[i];
		delete[] isLocatedMinotaur[i];
	}
	delete[] isLocatedTheseus;
	delete[] isLocatedMinotaur;
	delete[] arr_theseus_moves;
	*/
}

bool check_state(int state) {
	if(state == 0)
		return ((theseus->pX == minotaur->pX) && (theseus->pY == minotaur->pY));
	else
		return ((theseus->pX == exitX) && (theseus->pY == exitY));
}

void move_minotaur() {
	if ((minotaur->pX > theseus->pX) && (minotaur->left != 1)) {
		minotaur=&mazeMap[minotaur->pY][minotaur->pX-1];
		//cout << "mino left : " << minotaur->pX << minotaur->pY << endl;
		return;
	}
	if ((minotaur->pX < theseus->pX) && (minotaur->right != 1)) {
		minotaur = &mazeMap[minotaur->pY][minotaur->pX+1];
		//cout << "mino right : " << minotaur->pX << minotaur->pY << endl;
		return;
	}
	if ((minotaur->pY > theseus->pY) && (minotaur->up != 1)) {
		minotaur = &mazeMap[minotaur->pY-1][minotaur->pX];
		//cout << "mino up : " << minotaur->pX << minotaur->pY << endl;
		return;
	}
	if ((minotaur->pY < theseus->pY) && (minotaur->down != 1)) {
		minotaur = &mazeMap[minotaur->pY+1][minotaur->pX];
		//cout << "mino down : " << minotaur->pX << minotaur->pY << endl;
		return;
	}
}

void promising(int latestTurn, char move) {

	arr_theseus_moves[latestTurn] = move;
	if (check_state(0)) {
		//cout << "dead" << endl;
		return;
	}
	if (check_state(1)) {
		//cout << "wow" << endl;
		if (only_one_answer == true) {
			only_one_answer = false;
			print_answer(latestTurn);
		}
		return;
	}

	/* minotaur's move */
	move_minotaur();
	if (check_state(0)) {
		//cout << "dead" << endl;
		return;
	}
	move_minotaur();
	if (check_state(0)) {
		//cout << "dead" << endl;
		return;
	}
	/* minotaur's move end*/

	if (check_overlap()) {
		//cout << "wari" << endl;
		return;
	}
	mark_location();
	move_theseus(latestTurn + 1);
}

void move_theseus_left(int latestTurn) {
	if (theseus->left != 1) {
		theseus = &mazeMap[theseus->pY][theseus->pX-1];
		//cout << " left move : " << theseus->pX << theseus->pY << "," << minotaur->pX << minotaur->pY << endl;
		promising(latestTurn, LEFT);
	}
	return;
}
void move_theseus_right(int latestTurn) {
	if (theseus->right != 1) {
		theseus = &mazeMap[theseus->pY][theseus->pX+1];
		//cout << " right move : " << theseus->pX << theseus->pY << "," << minotaur->pX << minotaur->pY << endl;
		promising(latestTurn, RIGHT);
	}
	return;
}
void move_theseus_up(int latestTurn) {
	if (theseus->up != 1) {
		theseus = &mazeMap[theseus->pY-1][theseus->pX];
		//cout << " up move : " << theseus->pX << theseus->pY << "," << minotaur->pX << minotaur->pY << endl;
		promising(latestTurn, UP);
	}
	return;
}
void move_theseus_down(int latestTurn) {
	if (theseus->down != 1) {
		theseus = &mazeMap[theseus->pY+1][theseus->pX];
		//cout << " down move : " << theseus->pX << theseus->pY << "," << minotaur->pX << minotaur->pY << endl;
		promising(latestTurn, DOWN);
	}
	return;
}
void move_theseus_stay(int latestTurn) {
	promising(latestTurn, STAY);
	return;
}
void move_theseus(int latestTurn) {
	st_mazeBlk* tempTheseus;
	st_mazeBlk* tempMinotaur;

	tempTheseus = theseus;
	tempMinotaur = minotaur;
	//cout << "before move : " << tempTheseus->pX << tempTheseus->pY << "," << tempMinotaur->pX << tempMinotaur->pY << endl;
	/* try every moves... */
	move_theseus_left(latestTurn);
	theseus = tempTheseus; minotaur = tempMinotaur;
	move_theseus_right(latestTurn);
	theseus = tempTheseus; minotaur = tempMinotaur;
	move_theseus_up(latestTurn);
	theseus = tempTheseus; minotaur = tempMinotaur;
	move_theseus_down(latestTurn);
	theseus = tempTheseus; minotaur = tempMinotaur;
	move_theseus_stay(latestTurn);
	theseus = tempTheseus; minotaur = tempMinotaur;
	/* moves end */
}

void print_answer(int latestTurn) {
	cout << "PUZZLE " << puzzle << endl;
	for (int i = 0; i <= latestTurn; i++) {
		cout << arr_theseus_moves[i] << " ";
	}
	cout << endl;
	cout << "theseus : " << theseus->pX << "," << theseus->pY << endl;
	cout << "minotaur : " << minotaur->pX << "," << minotaur->pY << endl;
}

void mark_location() {
	theseus->isLocated[minotaur->pY][minotaur->pX] = true;
}
bool check_overlap() {
	return (theseus->isLocated[minotaur->pY][minotaur->pX]);
}