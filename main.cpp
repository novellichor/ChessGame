#include <SFML/Graphics.hpp>

#define LENGTH 7
#define BLACK_PAWN 1
#define WHITE_PAWN -1
#define BLACK_ROOK 2
#define WHITE_ROOK -2
#define BLACK_KNIGHT 3
#define WHITE_KNIGHT -3
#define BLACK_BISHOP 4
#define WHITE_BISHOP -4
#define BLACK_QUEEN 5
#define WHITE_QUEEN -5
#define BLACK_KING 6
#define WHITE_KING -6

using namespace sf;

struct pos {
	int x, y;
} oldPos, whiteKing, blackKing, transformWHITE, transformBLACK;

int size = 100;  //size of window is 800x800, and chess board is 8x8
int isMoving;  //flag for when a chess piece is moving

// chess board initial setup
int board[8][8] = {
	2, 3, 4, 5, 6, 4, 3, 2,
	1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
    -1,-1,-1,-1,-1,-1,-1,-1,
	-2,-3,-4,-5,-6,-4,-3,-2
};

int rightWhiteRookM, leftWhiteRookM, whiteKingFirstMove;
int rightBlackRookM, leftBlackRookM, blackKingFirstMove;

int move;  // 0 for white is moving, 1 for black is moving

int checkWhite, checkBlack;

int transformationWhite, transformationBlack;   //flag for transforming chess piece color
												// transformationWhite is black->white 
												// transformationBlack is white->black 

// Functions for moving chess pieces. Returns 1 if valid chess move
//		movePawnW(), movePawnB(), moveRookW(), moveRookB(), moveBishopW(),  
//		moveBishopB(), moveQueenW(), moveQueenB(), moveKnightW(), moveKnightB()
int movePawnW(int oldx, int oldy, int newx, int newy) {
	if(oldPos.y == 6) { //if at row 6 (original spot)
		if ((newy == oldy - 1 && newx == oldx && board[oldy - 1][oldx] == 0) || 
			(newy == oldy - 2 && newx == oldx && board[oldy - 1][oldx] == 0 && 
				board[oldy - 2][oldx] == 0)) { //if attempted to move forward 1 or 2 steps
			return 1;
		}
	}
	else if (newy == oldy - 1 && newx == oldx && board[oldy - 1][oldx] == 0) { //if not at row 6 but attempted to move forward 1 step
		return 1;
	}
	if (board[oldy - 1][oldx - 1] > 0) { //if there is a black chess piece in up-left diagonal by 1 step
		if (newy == oldy - 1 && newx == oldx - 1) { //if attempted to move up-left diagnoal by 1 step
			return 1;
		}
	}
	if (board[oldy - 1][oldx + 1] > 0) { //if there is a black chess piece in up-right diagonal by 1 step
		if (newy == oldy - 1 && newx == oldx + 1) { //if attempted to move up-right diagnoal by 1 step
			return 1;
		}
	}
}

int movePawnB(int oldx, int oldy, int newx, int newy) {
	if (oldPos.y == 1) { //if at row 1 (original spot)
		if ((newy == oldy + 1 && newx == oldx && board[oldy + 1][oldx] == 0) || 
			(newy == oldy + 2 && newx == oldx && board[oldy + 1][oldx] == 0 && 
				board[oldy + 2][oldx] == 0)) {//if attempted to move forward 1 or 2 steps
			return 1;
		}
	}
	else if (newy == oldy + 1 && newx == oldx && board[oldy + 1][oldx] == 0) {//if not at row 6 but attempted to move forward 1 step
		return 1;
	}
	if (board[oldy + 1][oldx - 1] > 0) { //if there is a black chess piece in up-right diagonal by 1 step
		if (newy == oldy + 1 && newx == oldx - 1) {//if attempted to move up-right diagnoal by 1 step
			return 1;
		}
	}
	if (board[oldy + 1][oldx + 1] > 0) {//if there is a black chess piece in up-left diagonal by 1 step
		if (newy == oldy + 1 && newx == oldx + 1) {//if attempted to move up-left diagnoal by 1 step
			return 1;
		}
	}
}

int moveRookW(int oldx, int oldy, int newx, int newy) {
	for (int i = oldx - 1; i >= 0; i--) { //move to left
		if (board[oldy][i] >= 0 && (newx == i && newy == oldy)) {//if there is a black chess piece along x axis or it is a free space, and player attempts to move left
			return 1;
		}
		else if(board[oldy][i] != 0) { //if there is a white chess piece along x axis
			break;
		}
	}

	for (int i = oldy - 1; i >= 0; i--) //move up
	{
		if (board[i][oldx] >= 0 && (newy == i && newx == oldx))//if there is a black chess piece along y axis or it is a free space, and player attempts to move up
		{
			return 1;
		}
		else if (board[i][oldx] != 0)//if there is a white chess piece along y axis
		{
			break;
		}
	}
	for (int i = oldx + 1; i <= 7; i++) //move to right
	{
		if (board[oldy][i] >= 0 && (newy == oldy && newx == i))//if there is a black chess piece along x axis or it is a free space, and player attempts to move right
		{
			return 1;
		}
		else if (board[oldy][i] != 0)//if there is a white chess piece along x axis
		{
			break;
		}
	}
	for (int i = oldy + 1; i <= 7; i++) //move down
	{
		if (board[i][oldx] >= 0 && (newy == i && newx == oldx))//if there is a black chess piece along y axis or it is a free space, and player attempts to move down
		{
			return 1;
		}
		else if (board[i][oldx] != 0)//if there is a white chess piece along y axis
		{
			break;
		}
	}
	return 0;
}

int moveRookB(int oldx, int oldy, int newx, int newy)
{
	for (int i = oldx - 1; i >= 0; i--) //move to left
	{
		if (board[oldy][i] <= 0 && (newx == i && newy == oldy))//if there is a white chess piece along x axis or it is a free space, and player attempts to move left
		{
			return 1;
		}
		else if (board[oldy][i] != 0)//if there is a black chess piece along x axis
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) //move up
	{
		if (board[i][oldx] <= 0 && (newy == i && newx == oldx))//if there is a white chess piece along y axis or it is a free space, and player attempts to move up
		{
			return 1;
		}
		else if (board[i][oldx] != 0)//if there is a black chess piece along y axis
		{
			break;
		}
	}
	for (int i = oldx + 1; i <= 7; i++) //move to right
	{
		if (board[oldy][i] <= 0 && (newy == oldy && newx == i))//if there is a white chess piece along x axis or it is a free space, and player attempts to move right
		{
			return 1;
		}
		else if (board[oldy][i] != 0)//if there is a black chess piece along x axis
		{
			break;
		}
	}
	for (int i = oldy + 1; i <= 7; i++) //move down
	{
		if (board[i][oldx] <= 0 && (newy == i && newx == oldx))//if there is a white chess piece along y axis or it is a free space, and player attempts to move down
		{
			return 1;
		}
		else if (board[i][oldx] != 0)//if there is a black chess piece along y axis
		{
			break;
		}
	}
	return 0;
}

int moveBishopW(int oldx, int oldy, int newx, int newy)
{
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-left
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-right
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // move down-left
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // move down-right
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int moveBishopB(int oldx, int oldy, int newx, int newy)
{
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-left
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-right
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // move down-left
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // move down-right
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int moveQueenW(int oldx, int oldy, int newx, int newy)
{
	for (int i = oldx - 1; i >= 0; i--) // move left
	{
		if (board[oldy][i] >= 0 && (newx == i && newy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // move up
	{
		if (board[i][oldx] >= 0 && (newy == i && newx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i <= 7; i++) // move right
	{
		if (board[oldy][i] >= 0 && (newy == oldy && newx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i <= 7; i++) // move down
	{
		if (board[i][oldx] >= 0 && (newy == i && newx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-left
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-right
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // move down-left
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // move down-right
	{
		if (board[i][j] >= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int moveQueenB(int oldx, int oldy, int newx, int newy)
{
	for (int i = oldx - 1; i >= 0; i--) // move left
	{
		if (board[oldy][i] <= 0 && (newx == i && newy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // move up
	{
		if (board[i][oldx] <= 0 && (newy == i && newx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i <= 7; i++) // move right
	{
		if (board[oldy][i] <= 0 && (newy == oldy && newx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i <= 7; i++) // move down
	{
		if (board[i][oldx] <= 0 && (newy == i && newx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-left
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // move up-right
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // move down-left
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // move down-right
	{
		if (board[i][j] <= 0 && (newy == i && newx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int moveKnightW(int oldx, int oldy, int newx, int newy)
{
	if (oldy - 2 >= 0 && oldx - 1 >= 0 && newy == oldy - 2 && newx == oldx - 1 && board[newy][newx] >= 0)
	{
		return 1; // move up 1 in L shape
	}
	if (oldy - 2 >= 0 && oldx + 1 < LENGTH && newy == oldy - 2 && newx == oldx + 1 && board[newy][newx] >= 0)
	{
		return 1; // move up 2 in L shape
	}
	if (oldy - 1 >= 0 && oldx + 2 < LENGTH && newy == oldy - 1 && newx == oldx + 2 && board[newy][newx] >= 0)
	{
		return 1; // move right 1 in L shape
	}
	if (oldy + 1 >= 0 && oldx + 2 < LENGTH && newy == oldy + 1 && newx == oldx + 2 && board[newy][newx] >= 0)
	{
		return 1; // move right 2 in L shape
	}
	if (oldy + 2 < LENGTH && oldx + 1 < LENGTH && newy == oldy + 2 && newx == oldx + 1 && board[newy][newx] >= 0)
	{
		return 1; // move down 1 in L shape
	}
	if (oldy + 2 < LENGTH && oldx - 1 >= 0 && newy == oldy + 2 && newx == oldx - 1 && board[newy][newx] >= 0)
	{
		return 1; // move down 2 in L shape
	}
	if (oldy + 1 < LENGTH && oldx - 2 >= 0 && newy == oldy + 1 && newx == oldx - 2 && board[newy][newx] >= 0)
	{
		return 1; // move left 1 in L shape
	}
	if (oldy - 1 >= 0 && oldx - 2 >= 0 && newy == oldy - 1 && newx == oldx - 2 && board[newy][newx] >= 0)
	{
		return 1; //move left 2 in L shape
	}
	return 0;
}

int moveKnightB(int oldx, int oldy, int newx, int newy)
{
	if (oldy - 2 >= 0 && oldx - 1 >= 0 && newy == oldy - 2 && newx == oldx - 1 && board[newy][newx] <= 0)
	{
		return 1; // move up 1 in L shape
	}
	if (oldy - 2 >= 0 && oldx + 1 < LENGTH && newy == oldy - 2 && newx == oldx + 1 && board[newy][newx] <= 0)
	{
		return 1; //  move up 2 in L shape
	}
	if (oldy - 1 >= 0 && oldx + 2 < LENGTH && newy == oldy - 1 && newx == oldx + 2 && board[newy][newx] <= 0)
	{
		return 1; // move right 1 in L shape
	}
	if (oldy + 1 >= 0 && oldx + 2 < LENGTH && newy == oldy + 1 && newx == oldx + 2 && board[newy][newx] <= 0)
	{
		return 1; // move right 2 in L shape
	}
	if (oldy + 2 < LENGTH && oldx + 1 < LENGTH && newy == oldy + 2 && newx == oldx + 1 && board[newy][newx] <= 0)
	{
		return 1; //  move down 1 in L shape
	}
	if (oldy + 2 < LENGTH && oldx - 1 >= 0 && newy == oldy + 2 && newx == oldx - 1 && board[newy][newx] <= 0)
	{
		return 1; //  move down 2 in L shape
	}
	if (oldy + 1 < LENGTH && oldx - 2 >= 0 && newy == oldy + 1 && newx == oldx - 2 && board[newy][newx] <= 0)
	{
		return 1; // move left 1 in L shape
	}
	if (oldy - 1 >= 0 && oldx - 2 >= 0 && newy == oldy - 1 && newx == oldx - 2 && board[newy][newx] <= 0)
	{
		return 1; // move left 2 in L shape
	}
	return 0;
}

// Functions for potential checks to the opposing king. Returns 1 if valid check
//		PawnWCheck(), PawnBCheck(), RookWCheck(), RookBCheck(), BishopWCheck(), BishopBCheck(), 
//		KnightWCheck(), KnightBCheck(), QueenWCheck(), QueenBCheck(), KingWCheck(), KingBCheck()
int PawnWCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (board[oldy - 1][oldx - 1] >= 0)//if black chess piece can be captured by one space diagonal left
	{
		if (oldy - 1 == kingy && oldx - 1 == kingx)
		{
			return 1;
		}
	}
	if (board[oldy - 1][oldx + 1] >= 0)//if black chess piece can be captured by one space diagonal right
	{
		if (oldy - 1 == kingy && oldx + 1 == kingx)
		{
			return 1;
		}
	}
	return 0;
}

int PawnBCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (board[oldy + 1][oldx - 1] <= 0)
	{
		if (oldy + 1 == kingy && oldx - 1 == kingx)
		{
			return 1;
		}
	}
	if (board[oldy + 1][oldx + 1] <= 0)
	{
		if (oldy + 1 == kingy && oldx + 1 == kingx)
		{
			return 1;
		}
	}
	return 0;
}

int RookWCheck(int oldx, int oldy, int kingx, int kingy)
{
	for (int i = oldx - 1; i >= 0; i--) // left
	{
		if (board[oldy][i] >= 0 && (kingx == i && kingy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // up
	{
		if (board[i][oldx] >= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i < LENGTH; i++) // right
	{
		if (board[oldy][i] >= 0 && (kingy == oldy && kingx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i < LENGTH; i++) // move down
	{
		if (board[i][oldx] >= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	return 0;
}

int RookBCheck(int oldx, int oldy, int kingx, int kingy)
{
	for (int i = oldx - 1; i >= 0; i--) // left
	{
		if (board[oldy][i] <= 0 && (kingx == i && kingy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // up
	{
		if (board[i][oldx] <= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i < LENGTH; i++) // right
	{
		if (board[oldy][i] <= 0 && (kingy == oldy && kingx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i < LENGTH; i++) // move down
	{
		if (board[i][oldx] <= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	return 0;
}

int BishopWCheck(int oldx, int oldy, int kingx, int kingy)
{
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // left up
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // right up
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // left down
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // right down
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int BishopBCheck(int oldx, int oldy, int kingx, int kingy)
{
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // left up
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // right up
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // left down
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i <= 7; i++)  // right down
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int KnightWCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (oldy - 2 >= 0 && oldx - 1 >= 0 && kingy == oldy - 2 && kingx == oldx - 1 && board[kingy][kingx] >= 0)
	{
		return 1; // left up
	}
	if (oldy - 2 >= 0 && oldx + 1 < LENGTH && kingy == oldy - 2 && kingx == oldx + 1 && board[kingy][kingx] >= 0)
	{
		return 1; // right up
	}
	if (oldy - 1 >= 0 && oldx + 2 < LENGTH && kingy == oldy - 1 && kingx == oldx + 2 && board[kingy][kingx] >= 0)
	{
		return 1; // right 1
	}
	if (oldy + 1 >= 0 && oldx + 2 < LENGTH && kingy == oldy + 1 && kingx == oldx + 2 && board[kingy][kingx] >= 0)
	{
		return 1; // right 2
	}
	if (oldy + 2 < LENGTH && oldx + 1 < LENGTH && kingy == oldy + 2 && kingx == oldx + 1 && board[kingy][kingx] >= 0)
	{
		return 1; // down 1
	}
	if (oldy + 2 < LENGTH && oldx - 1 >= 0 && kingy == oldy + 2 && kingx == oldx - 1 && board[kingy][kingx] >= 0)
	{
		return 1; // down 2
	}
	if (oldy + 1 < LENGTH && oldx - 2 >= 0 && kingy == oldy + 1 && kingx == oldx - 2 && board[kingy][kingx] >= 0)
	{
		return 1; // left 1
	}
	if (oldy - 1 >= 0 && oldx - 2 >= 0 && kingy == oldy - 1 && kingx == oldx - 2 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	return 0;
}

int KnightBCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (oldy - 2 >= 0 && oldx - 1 >= 0 && kingy == oldy - 2 && kingx == oldx - 1 && board[kingy][kingx] <= 0)
	{
		return 1; // left up
	}
	if (oldy - 2 >= 0 && oldx + 1 < LENGTH && kingy == oldy - 2 && kingx == oldx + 1 && board[kingy][kingx] <= 0)
	{
		return 1; // right up
	}
	if (oldy - 1 >= 0 && oldx + 2 < LENGTH && kingy == oldy - 1 && kingx == oldx + 2 && board[kingy][kingx] <= 0)
	{
		return 1; // right 1
	}
	if (oldy + 1 >= 0 && oldx + 2 < LENGTH && kingy == oldy + 1 && kingx == oldx + 2 && board[kingy][kingx] <= 0)
	{
		return 1; // right 2
	}
	if (oldy + 2 < LENGTH && oldx + 1 < LENGTH && kingy == oldy + 2 && kingx == oldx + 1 && board[kingy][kingx] <= 0)
	{
		return 1; // down 1
	}
	if (oldy + 2 < LENGTH && oldx - 1 >= 0 && kingy == oldy + 2 && kingx == oldx - 1 && board[kingy][kingx] <= 0)
	{
		return 1; // down 2
	}
	if (oldy + 1 < LENGTH && oldx - 2 >= 0 && kingy == oldy + 1 && kingx == oldx - 2 && board[kingy][kingx] <= 0)
	{
		return 1; // left 1
	}
	if (oldy - 1 >= 0 && oldx - 2 >= 0 && kingy == oldy - 1 && kingx == oldx - 2 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	return 0;
}

int QueenWCheck(int oldx, int oldy, int kingx, int kingy)
{
	for (int i = oldx - 1; i >= 0; i--) // left
	{
		if (board[oldy][i] >= 0 && (kingx == i && kingy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // up
	{
		if (board[i][oldx] >= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i < LENGTH; i++) // right
	{
		if (board[oldy][i] >= 0 && (kingy == oldy && kingx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i < LENGTH; i++) // down
	{
		if (board[i][oldx] >= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // left up
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // right up
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // left down
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i < LENGTH; i++)  // right down
	{
		if (board[i][j] >= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int QueenBCheck(int oldx, int oldy, int kingx, int kingy)
{
	for (int i = oldx - 1; i >= 0; i--) // left
	{
		if (board[oldy][i] <= 0 && (kingx == i && kingy == oldy))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy - 1; i >= 0; i--) // up
	{
		if (board[i][oldx] <= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	for (int i = oldx + 1; i < LENGTH; i++) // right
	{
		if (board[oldy][i] <= 0 && (kingy == oldy && kingx == i))
		{
			return 1;
		}
		else if (board[oldy][i] != 0)
		{
			break;
		}
	}
	for (int i = oldy + 1; i < LENGTH; i++) // down
	{
		if (board[i][oldx] <= 0 && (kingy == i && kingx == oldx))
		{
			return 1;
		}
		else if (board[i][oldx] != 0)
		{
			break;
		}
	}
	int j = oldx - 1;
	for (int i = oldy - 1; i >= 0; i--) // left up
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy - 1; i >= 0; i--) // right up
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	j = oldx - 1;
	for (int i = oldy + 1; i <= 7; i++) // left down
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j--;
	}
	j = oldx + 1;
	for (int i = oldy + 1; i < LENGTH; i++)  // right down
	{
		if (board[i][j] <= 0 && (kingy == i && kingx == j))
		{
			return 1;
		}
		else if (board[i][j] != 0)
		{
			break;
		}
		j++;
	}
	return 0;
}

int KingWCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (oldx - 1 >= 0 && oldy - 1 >= 0 && kingy == oldy - 1 && kingx == oldx - 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldy - 1 >= 0 && kingx == oldx && kingy == oldy - 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldy - 1 >= 0 && oldx + 1 < LENGTH && kingx == oldx + 1 && kingy == oldy - 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldx + 1 < LENGTH && kingy == oldy && kingx == oldx + 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldx + 1 < LENGTH && oldy + 1 < LENGTH && kingy == oldy + 1 && kingx == oldx + 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldy + 1 < LENGTH && kingy == oldy + 1 && kingx == oldx && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldx - 1 >= 0 && oldy + 1 < LENGTH && kingx == oldx - 1 && kingy == oldy + 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	if (oldx - 1 >= 0 && kingy == oldy && kingx == oldx - 1 && board[kingy][kingx] <= 0)
	{
		return 1;
	}
	return 0;
}

int KingBCheck(int oldx, int oldy, int kingx, int kingy)
{
	if (oldx - 1 >= 0 && oldy - 1 >= 0 && kingy == oldy - 1 && kingx == oldx - 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldy - 1 >= 0 && kingx == oldx && kingy == oldy - 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldy - 1 >= 0 && oldx + 1 < LENGTH && kingx == oldx + 1 && kingy == oldy - 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldx + 1 < LENGTH && kingy == oldy && kingx == oldx + 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldx + 1 < LENGTH && oldy + 1 < LENGTH && kingy == oldy + 1 && kingx == oldx + 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldy + 1 < LENGTH && kingy == oldy + 1 && kingx == oldx && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldx - 1 >= 0 && oldy + 1 < LENGTH && kingx == oldx - 1 && kingy == oldy + 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	if (oldx - 1 >= 0 && kingy == oldy && kingx == oldx - 1 && board[kingy][kingx] >= 0)
	{
		return 1;
	}
	return 0;
}

//  Functions that check whether the king has been checked by calling the above check functions
//		BlackKingCheck(), WhiteKingCheck()
int BlackKingCheck(int posKingx, int posKingy)
{
	int ok = 0;
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			if (board[i][j] < 0)
			{
				if (board[i][j] == WHITE_PAWN)
				{
					ok = PawnWCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == WHITE_ROOK)
				{
					ok = RookWCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == WHITE_KNIGHT)
				{
					ok = KnightWCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == WHITE_BISHOP)
				{
					ok = BishopWCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == WHITE_QUEEN)
				{
					ok = QueenWCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == WHITE_KING)
				{
					ok = KingWCheck(j, i, posKingx, posKingy);
				}
				if (ok == 1)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

int WhiteKingCheck(int posKingx, int posKingy)
{
	int ok = 0;
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			if (board[i][j] > 0)
			{
				if (board[i][j] == BLACK_PAWN)
				{
					ok = PawnBCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == BLACK_ROOK)
				{
					ok = RookBCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == BLACK_KNIGHT)
				{
					ok = KnightBCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == BLACK_BISHOP)
				{
					ok = BishopBCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == BLACK_QUEEN)
				{
					ok = QueenBCheck(j, i, posKingx, posKingy);
				}
				if (board[i][j] == BLACK_KING)
				{
					ok = KingBCheck(j, i, posKingx, posKingy);
				}
				if (ok == 1)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

//  Functions for moving the king chess pieces. Returns 1 if valid chess move
//		moveBlackKing(), moveWhiteKing()
int moveBlackKing(int oldx, int oldy, int newx, int newy)
{
	if (oldx - 1 >= 0 && oldy - 1 >= 0 && newy == oldy - 1 && newx == oldx - 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx - 1, oldy - 1);
		if (ok == 1)
		{
			return 1;  // left up
		}
	}
	if (oldy - 1 >= 0 && newx == oldx && newy == oldy - 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx, oldy - 1);
		if (ok == 1)
		{
			return 1; // up
		}
	}
	if (oldy - 1 >= 0 && oldx + 1 < LENGTH && newx == oldx + 1 && newy == oldy - 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx + 1, oldy - 1);
		if (ok == 1)
		{
			return 1; // right up
		}
	}
	if (oldx + 1 < LENGTH && newy == oldy && newx == oldx + 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx + 1, oldy);
		if (ok == 1)
		{
			return 1; // right
		}
	}
	if (oldx + 1 < LENGTH && oldy + 1 < LENGTH && newy == oldy + 1 && newx == oldx + 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx + 1, oldy + 1);
		if (ok == 1)
		{
			return 1; // right down
		}
	}
	if (oldy + 1 < LENGTH && newy == oldy + 1 && newx == oldx && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx, oldy + 1);
		if (ok == 1)
		{
			return 1; // down
		}
	}
	if (oldx - 1 >= 0 && oldy + 1 < LENGTH && newx == oldx - 1 && newy == oldy + 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx - 1, oldy + 1);
		if (ok == 1)
		{
			return 1; // left down
		}
	}
	if (oldx - 1 >= 0 && newy == oldy && newx == oldx - 1 && board[newy][newx] <= 0)
	{
		int ok = BlackKingCheck(oldx - 1, oldy);
		if (ok == 1)
		{
			return 1; // left
		}
	}
	// left
	if (rightBlackRookM == 0 && blackKingFirstMove == 0 && board[0][5] == 0 && board[0][6] == 0 && newy == 0 && newx == 6)
	{
		int ok = BlackKingCheck(4, 0);
		if (ok == 1)
		{
			ok = BlackKingCheck(5, 0);
			if (ok == 1)
			{
				ok = BlackKingCheck(6, 0);
				if (ok == 1)
				{
					blackKingFirstMove = 1;
					rightBlackRookM = 1;
					board[0][7] = 0;
					board[0][5] = BLACK_ROOK;
					return 1;
				}
			}
		}
	}
	if (leftBlackRookM == 0 && blackKingFirstMove == 0 && board[0][3] == 0 && board[0][2] == 0 && board[0][1] == 0 && newy == 0 && newx == 2)
	{
		int ok = BlackKingCheck(4, 0);
		if (ok == 1)
		{
			ok = BlackKingCheck(3, 0);
			if (ok == 1)
			{
				ok = BlackKingCheck(2, 0);
				if (ok == 1)
				{
					ok = BlackKingCheck(1, 0);
					if (ok == 1)
					{
						blackKingFirstMove = 1;
						leftBlackRookM = 1;
						board[0][0] = 0;
						board[0][3] = BLACK_ROOK;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int moveWhiteKing(int oldx, int oldy, int newx, int newy)
{
	if (oldx - 1 >= 0 && oldy - 1 >= 0 && newy == oldy - 1 && newx == oldx - 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx - 1, oldy - 1);
		if (ok == 1)
		{
			return 1;  // left up
		}
	}
	if (oldy - 1 >= 0 && newx == oldx && newy == oldy - 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx, oldy - 1);
		if (ok == 1)
		{
			return 1; // up
		}
	}
	if (oldy - 1 >= 0 && oldx + 1 < LENGTH && newx == oldx + 1 && newy == oldy - 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx + 1, oldy - 1);
		if (ok == 1)
		{
			return 1; // right up
		}
	}
	if (oldx + 1 < LENGTH && newy == oldy && newx == oldx + 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx + 1, oldy);
		if (ok == 1)
		{
			return 1; // right
		}
	}
	if (oldx + 1 < LENGTH && oldy + 1 < LENGTH && newy == oldy + 1 && newx == oldx + 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx + 1, oldy + 1);
		if (ok == 1)
		{
			return 1; // right down
		}
	}
	if (oldy + 1 < LENGTH && newy == oldy + 1 && newx == oldx && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx, oldy + 1);
		if (ok == 1)
		{
			return 1; // down
		}
	}
	if (oldx - 1 >= 0 && oldy + 1 < LENGTH && newx == oldx - 1 && newy == oldy + 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx - 1, oldy + 1);
		if (ok == 1)
		{
			return 1; // right down
		}
	}
	if (oldx - 1 >= 0 && newy == oldy && newx == oldx - 1 && board[newy][newx] >= 0)
	{
		int ok = WhiteKingCheck(oldx - 1, oldy);
		if (ok == 1)
		{
			return 1; // left
		}
	}
	// right
	if (whiteKingFirstMove == 0 && rightWhiteRookM == 0 && board[7][5] == 0 && board[7][6] == 0 && newy == 7 && newx == 6)
	{
		int ok = 1;
		ok = WhiteKingCheck(4, 7);
		if (ok == 1)
		{
			ok = WhiteKingCheck(5, 7);
			if (ok == 1)
			{
				ok = WhiteKingCheck(6, 7);
				if (ok == 1)
				{
					board[7][7] = 0;
					board[7][5] = WHITE_ROOK;
					whiteKingFirstMove = 1;
					rightWhiteRookM = 1;
					return 1;
				}
			}
		}
	}
	//left
	if (whiteKingFirstMove == 0 && rightWhiteRookM == 0 && board[7][3] == 0 && board[7][2] == 0 && board[7][1] == 0 && newy == 7 && newx == 2)
	{
		int ok = 1;
		ok = WhiteKingCheck(4, 7);
		if (ok == 1)
		{
			ok = WhiteKingCheck(3, 7);
			if (ok == 1)
			{
				ok = WhiteKingCheck(2, 7);
				if (ok == 1)
				{
					ok = WhiteKingCheck(1, 7);
					if (ok == 1)
					{
						board[7][0] = 0;
						board[7][3] = WHITE_ROOK;
						whiteKingFirstMove = 1;
						leftWhiteRookM = 1;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

//  Functions that get the position of the king chess piece
//		posWhiteKing(), posBlackKing()
void posWhiteKing()
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			if (board[i][j] == WHITE_KING)
			{
				whiteKing.x = j;
				whiteKing.y = i;
				break;
			}
		}
	}
}

void posBlackKing()
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			if (board[i][j] == BLACK_KING)
			{
				blackKing.y = i;
				blackKing.x = j;
				break;
			}
		}
	}
}

int main() {
	RenderWindow window(VideoMode(800, 800), "Chess Game");
	Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;

	t1.loadFromFile("images/board.png");
	t2.loadFromFile("images/BlackPawn.png");
	t3.loadFromFile("images/WhitePawn.png");
	t4.loadFromFile("images/WhiteRook.png");
	t5.loadFromFile("images/BlackRook.png");
	t6.loadFromFile("images/WhiteKnight.png");
	t7.loadFromFile("images/BlackKnight.png");
	t8.loadFromFile("images/WhiteBishop.png");
	t9.loadFromFile("images/BlackBishop.png");
	t10.loadFromFile("images/WhiteQueen.png");
	t11.loadFromFile("images/BlackQueen.png");
	t12.loadFromFile("images/BlackKing.png");
	t13.loadFromFile("images/WhiteKing.png");
	t14.loadFromFile("images/TransformationWhite.png");
	t15.loadFromFile("images/TransformationBlack.png");

	Sprite table(t1);
	Sprite BlackPawn(t2);
	Sprite WhitePawn(t3);
	Sprite WhiteRook(t4);
	Sprite BlackRook(t5);
	Sprite WhiteKnight(t6);
	Sprite BlackKnight(t7);
	Sprite WhiteBishop(t8);
	Sprite BlackBishop(t9);
	Sprite WhiteQueen(t10);
	Sprite BlackQueen(t11);
	Sprite BlackKing1(t12);
	Sprite WhiteKing1(t13);
	Sprite WhiteTransformation(t14);
	Sprite BlackTransformation(t15);
	Sprite MoveImages;  //assigned a value when moving a chess piece

	float dx = 0, dy = 0;	//amount that it was moved by on the board
	int noMovedPiece = 0;   //the chess piece that was moved

	while(window.isOpen()) {
		Vector2i pos = Mouse::getPosition(window);
		int x = pos.x / size;
		int y = pos.y / size;
		Event e;

		while(window.pollEvent(e)) {
			if(e.type == Event::Closed) {
				window.close();
			}
			window.clear();
			if(e.type == Event::MouseButtonPressed) {
				if(e.key.code == Mouse::Left) {
					if (transformationWhite == 1) {
						if (pos.y >= transformWHITE.y * size && pos.y <= (transformWHITE.y + 1) * size
							&& pos.x >= transformWHITE.x * size && pos.x <= (transformWHITE.x + 1) * size) {
							int xx = pos.x % 100, yy = pos.y % 100;
							if (xx < 50 && yy < 50 && xx > 0 && yy > 0)
							{
								board[transformWHITE.y][transformWHITE.x] = WHITE_ROOK;
								transformationWhite = 0;
							}
							if (xx > 50 && xx < 100 && yy < 50 && yy > 0)
							{
								board[transformWHITE.y][transformWHITE.x] = WHITE_QUEEN;
								transformationWhite = 0;
							}
							if (xx > 50 && xx < 100 && yy>50 && yy < 100)
							{
								board[transformWHITE.y][transformWHITE.x] = WHITE_KNIGHT;
								transformationWhite = 0;
							}
							if (xx < 50 && xx>0 && yy > 50 && y < 100)
							{
								board[transformWHITE.y][transformWHITE.x] = WHITE_BISHOP;
								transformationWhite = 0;
							}
							if (transformationWhite == 0)
							{
								posBlackKing();
								int h = BlackKingCheck(blackKing.x, blackKing.y);
								if (h == 0)
								{
									checkBlack = 1;
								}
							}
						}
					}
					if (transformationBlack == 1)
					{
						if (pos.y >= transformBLACK.y * size && pos.y <= (transformBLACK.y + 1) * size && 
							pos.x >= transformBLACK.x * size && pos.x <= (transformBLACK.x + 1) * size)
						{
							int xx = pos.x % 100, yy = pos.y % 100;
							if (xx < 50 && yy < 50 && xx > 0 && yy > 0)
							{
								board[transformBLACK.y][transformBLACK.x] = BLACK_ROOK;
								transformationBlack = 0;
							}
							if (xx > 50 && xx < 100 && yy < 50 && yy > 0)
							{
								board[transformBLACK.y][transformBLACK.x] = BLACK_QUEEN;
								transformationBlack = 0;
							}
							if (xx > 50 && xx < 100 && yy>50 && yy < 100)
							{
								board[transformBLACK.y][transformBLACK.x] = BLACK_KNIGHT;
								transformationBlack = 0;
							}
							if (xx < 50 && xx>0 && yy > 50 && y < 100)
							{
								board[transformBLACK.y][transformBLACK.x] = BLACK_BISHOP;
								transformationBlack = 0;
							}
							if (transformationBlack == 0)
							{
								posWhiteKing();
								int h = WhiteKingCheck(whiteKing.x, whiteKing.y);
								if (h == 0)
								{
									checkWhite = 1;
								}
							}
						}
					}

					if(board[y][x] != 0) { //if clicking on a chess piece
						dx = pos.x - x * size;
						dy = pos.y - y * size;
						if(board[y][x] == BLACK_PAWN && move == 1) {
							noMovedPiece = BLACK_PAWN;
							MoveImages = BlackPawn;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_PAWN && move == 0) {
							noMovedPiece = WHITE_PAWN;
							MoveImages = WhitePawn;
							board[y][x] = 0;
						}
						if(board[y][x] == BLACK_ROOK && move == 1) {
							noMovedPiece = BLACK_ROOK;
							MoveImages = BlackRook;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_ROOK && move == 0) {
							noMovedPiece = WHITE_ROOK;
							MoveImages = WhiteRook;
							board[y][x] = 0;
						}
						if(board[y][x] == BLACK_KNIGHT && move == 1) {
							noMovedPiece = BLACK_KNIGHT;
							MoveImages = BlackKnight;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_KNIGHT && move == 0) {
							noMovedPiece = WHITE_KNIGHT;
							MoveImages = WhiteKnight;
							board[y][x] = 0;
						}
						if(board[y][x] == BLACK_BISHOP && move == 1) {
							noMovedPiece = BLACK_BISHOP;
							MoveImages = BlackBishop;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_BISHOP && move == 0) {
							noMovedPiece = WHITE_BISHOP;
							MoveImages = WhiteBishop;
							board[y][x] = 0;
						}
						if(board[y][x] == BLACK_QUEEN && move == 1) {
							noMovedPiece = BLACK_QUEEN;
							MoveImages = BlackQueen;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_QUEEN && move == 0) {
							noMovedPiece = WHITE_QUEEN;
							MoveImages = WhiteQueen;
							board[y][x] = 0;
						}
						if(board[y][x] == BLACK_KING && move == 1) {
							noMovedPiece = BLACK_KING;
							MoveImages = BlackKing1;
							board[y][x] = 0;
						}
						if(board[y][x] == WHITE_KING && move == 0) {
							noMovedPiece = WHITE_KING;
							MoveImages = WhiteKing1;
							board[y][x] = 0;
						}
						if (board[y][x] == 0) {
							isMoving = 1;
							oldPos.x = x;
							oldPos.y = y;
						}
					}
				}
			}
			if (e.type == Event::MouseButtonReleased) {
				if (e.key.code == Mouse::Left) {
					int ok = 2;
					if (noMovedPiece == WHITE_PAWN && isMoving == 1)
					{
						ok = movePawnW(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == BLACK_PAWN && isMoving == 1)
					{
						ok = movePawnB(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == WHITE_ROOK && isMoving == 1)
					{
						ok = moveRookW(oldPos.x, oldPos.y, x, y);
						if (ok == 1 && leftWhiteRookM == 0 && oldPos.y == 7 && oldPos.x == 0)
						{
							leftWhiteRookM = 1;
						}
						if (ok == 1 && rightWhiteRookM == 0 && oldPos.y == 7 && oldPos.x == 7)
						{
							rightWhiteRookM = 1;
						}
					}
					if (noMovedPiece == BLACK_ROOK && isMoving == 1)
					{
						ok = moveRookB(oldPos.x, oldPos.y, x, y);
						if (ok == 1 && rightBlackRookM == 0 && oldPos.y == 0 && oldPos.x == 7)
						{
							rightBlackRookM = 1;
						}
						if (ok == 1 && leftBlackRookM == 0 && oldPos.y== 0 && oldPos.x == 0)
						{
							leftBlackRookM = 1;
						}
					}
					if (noMovedPiece == WHITE_BISHOP && isMoving == 1)
					{
						ok = moveBishopW(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == BLACK_BISHOP && isMoving == 1)
					{
						ok = moveBishopB(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == WHITE_QUEEN && isMoving == 1)
					{
						ok = moveQueenW(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == BLACK_QUEEN && isMoving == 1)
					{
						ok = moveQueenB(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == WHITE_KNIGHT && isMoving == 1)
					{
						ok = moveKnightW(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == BLACK_KNIGHT && isMoving == 1)
					{
						ok = moveKnightB(oldPos.x, oldPos.y, x, y);
					}
					if (noMovedPiece == BLACK_KING && isMoving == 1)
					{
						ok = moveBlackKing(oldPos.x, oldPos.y, x, y);
						if (ok == 1 && blackKingFirstMove == 0)
						{
							blackKingFirstMove = 1;
						}
					}
					if (noMovedPiece == WHITE_KING && isMoving == 1)
					{
						ok = moveWhiteKing(oldPos.x, oldPos.y, x, y);
						if (ok == 1 && whiteKingFirstMove == 0)
						{
							whiteKingFirstMove = 1;
						}
					}
					if (ok == 1)
					{
						int tmp = board[y][x];
						board[y][x] = noMovedPiece;
						if (y == 0 && noMovedPiece == WHITE_PAWN)
						{
							transformationWhite = 1;
							transformWHITE.x = x;
							transformWHITE.y = y;
							board[y][x] = 0;
						}
						if (y == 7 && noMovedPiece == BLACK_PAWN)
						{
							transformationBlack = 1;
							transformBLACK.x = x;
							transformBLACK.y = y;
							board[y][x] = 0;
						}
						if (move == 0) 
						{
							if (checkWhite == 1)
							{
								posWhiteKing();
								int s = WhiteKingCheck(whiteKing.x, whiteKing.y);
								if (s == 0)
								{
									board[oldPos.y][oldPos.x] = noMovedPiece;
									board[y][x] = tmp;
								}
								else
								{
									checkWhite = 0;
									posBlackKing();
									int sah = BlackKingCheck(blackKing.x, blackKing.y);
									if (sah == 0)
									{
										checkBlack = 1;
									}
									move = 1;
								}
							}
							else
							{
								posWhiteKing();
								int sa = WhiteKingCheck(whiteKing.x, whiteKing.y);
								if (sa == 0)
								{
									board[oldPos.y][oldPos.x] = noMovedPiece;
									board[y][x] = tmp;
								}
								else
								{
									posBlackKing();
									int sah = BlackKingCheck(blackKing.x, blackKing.y);
									if (sah == 0)
									{
										checkBlack = 1;
									}
									move = 1;
								}
							}
						}
						else 
						{
							if (checkBlack == 1)
							{
								posBlackKing();
								int s = BlackKingCheck(blackKing.x, blackKing.y);
								if (s == 0)
								{
									board[oldPos.y][oldPos.x] = noMovedPiece;
									board[y][x] = tmp;
								}
								else
								{
									checkBlack = 0;
									posWhiteKing();
									int check = WhiteKingCheck(whiteKing.x, whiteKing.y);
									if (check == 0)
									{
										checkWhite = 1;
									}
									move = 0;
								}
							}
							else
							{
								posBlackKing();
								int sa = BlackKingCheck(blackKing.x, blackKing.y);
								if (sa == 0)
								{
									board[oldPos.y][oldPos.x] = noMovedPiece;
									board[y][x] = tmp;
								}
								else
								{
									posWhiteKing();
									int check = WhiteKingCheck(whiteKing.x, whiteKing.y);
									if (check == 0)
									{
										checkWhite = 1;
									}
									move = 0;
								}
							}
						}
					}
					else if (ok == 0)
					{
						board[oldPos.y][oldPos.x] = noMovedPiece;
					}
					isMoving = 0;

				}
			}
		}
		window.clear();
		window.draw(table);  //draw chess board using Sprite
		if (transformationWhite == 1) { //draw chess piece black->white using Sprite
			WhiteTransformation.setPosition(transformWHITE.x * size, transformWHITE.y * size);
			window.draw(WhiteTransformation);
		}
		if (transformationBlack == 1) { //draw chess piece white->black using Sprite
			BlackTransformation.setPosition(transformBLACK.x * size, transformBLACK.y * size);
			window.draw(BlackTransformation);
		}
		if (isMoving == 1) { //draws the moving chess piece
			MoveImages.setPosition(pos.x - dx, pos.y - dy);  
			window.draw(MoveImages);
		}
		for (int i = 0; i <= LENGTH; i++) {  //draw chess pieces onto chessboard using Sprite
			for (int j = 0; j <= LENGTH; j++) {
				if (board[i][j] != 0) {
					if (board[i][j] == BLACK_PAWN) {
						BlackPawn.setPosition(j* size, i* size);  
						window.draw(BlackPawn);
					}
					if (board[i][j] == WHITE_PAWN) {
						WhitePawn.setPosition(j* size, i* size);
						window.draw(WhitePawn);
					}
					if (board[i][j] == BLACK_ROOK) {
						BlackRook.setPosition(j* size, i* size);
						window.draw(BlackRook);
					}
					if (board[i][j] == WHITE_ROOK) {
						WhiteRook.setPosition(j* size, i* size);
						window.draw(WhiteRook);
					}
					if (board[i][j] == BLACK_KNIGHT) {
						BlackKnight.setPosition(j* size, i* size);
						window.draw(BlackKnight);
					}
					if (board[i][j] == WHITE_KNIGHT) {
						WhiteKnight.setPosition(j* size, i* size);
						window.draw(WhiteKnight);
					}
					if (board[i][j] == BLACK_BISHOP) {
						BlackBishop.setPosition(j* size, i* size);
						window.draw(BlackBishop);
					}
					if (board[i][j] == WHITE_BISHOP) {
						WhiteBishop.setPosition(j* size, i* size);
						window.draw(WhiteBishop);
					}
					if (board[i][j] == BLACK_QUEEN) {
						BlackQueen.setPosition(j* size, i* size);
						window.draw(BlackQueen);
					}
					if (board[i][j] == WHITE_QUEEN) {
						WhiteQueen.setPosition(j* size, i* size);
						window.draw(WhiteQueen);
					}
					if (board[i][j] == BLACK_KING) {
						BlackKing1.setPosition(j* size, i* size);
						window.draw(BlackKing1);
					}
					if (board[i][j] == WHITE_KING) {
						WhiteKing1.setPosition(j* size, i* size);
						window.draw(WhiteKing1);
					}
				}
			}
		}
		window.display();
	}
	return 0;
}