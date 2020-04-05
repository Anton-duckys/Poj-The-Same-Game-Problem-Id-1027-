#include<iostream>
#include<vector>
#include<string>
using namespace std;
/*
	To solve this problem I divided it into subproblems:
	1)Find the largest cluster with function find_cluster;
	2)Erase found cluster with function erase;
	3)Shift the remaining balls in each colum down with function down;
	4)Shift columns to the left if there are empty columns with function left;

	Then repeat these steps until the game is over.


*/
void find_cluster(int i, int j, vector<string>& board, vector<vector<bool>>& tabl, int& count, char q) {
	/*
	recursively tracking board to find clusters
	"tabl" is a matrix to check which cells are visited to prevent looping
	*/
	if (i >= 0 && i < board.size() && j >= 0 && j < board[0].size() && board[i][j] == q && tabl[i][j] == 0) {
		++count;
		tabl[i][j] = 1;
		find_cluster(i + 1, j, board, tabl, count, q);
		find_cluster(i, j - 1, board, tabl, count, q);
		find_cluster(i - 1, j, board, tabl, count, q);
		find_cluster(i, j + 1, board, tabl, count, q);

	}
}
void erase(int i, int j, vector<string>& board, char q) {
	/*
		recursively tracking board to erase found cluster with change each cell in cluster with '_'
	*/
	if (i >= 0 && i < board.size() && j >= 0 && j < board[0].size() && board[i][j] == q) {


		board[i][j] = '_';
		erase(i + 1, j, board, q);
		erase(i, j - 1, board, q);
		erase(i - 1, j, board, q);
		erase(i, j + 1, board, q);

	}
}

void down(vector<string>& board) {
	/*
	shift the remaining balls down
	*/
	for (int j = 0; j < board[0].size(); ++j) {
		for (int i = board.size() - 1; i >= 0; --i) {
			if (board[i][j] == '_') {
				int l = i - 1;
				for (l; l >= 0; --l) {
					if (board[l][j] != '_') {
						swap(board[l][j], board[i][j]);
						break;
					}
				}
			}
		}
	}
}
bool empty_col(int j, vector<string>& board) {
	/*
	check if the column is empty
	*/
	for (int i = 0; i < board.size(); ++i) {
		if (board[i][j] != '_')
			return false;

	}
	return true;
}
void swap_col(int j1, int j2, vector<string>& board) {
	/*
	swap not empty column with empty column
	*/
	for (int i = 0; i < board.size(); ++i) {
		swap(board[i][j1], board[i][j2]);
	}
}
void left(vector<string>& board) {

	/*
	shift all not empty columns to the left by swapping columns with function swap;
	*/
	for (int j = 0; j < board[0].size(); ++j) {
		if (empty_col(j, board)) {
			int tempj = j + 1;
			for (tempj; tempj < board[0].size(); ++tempj) {
				if (!empty_col(tempj, board)) {
					swap_col(j, tempj, board);
					break;
				}
			}
		}
	}

}
int remaining_balls(vector<string>& board) {

	/*
	function to count the remaining balls at the end of the game
	*/
	int remains = 0;
	for (auto el : board) {
		for (auto elem : el) {
			if (elem != '_') {

				++remains;
			}
		}
	}
	return remains;
}
void rgbgame(vector<string>& board) {
	/*
	function to solve problem
	*/
	int turn = 0;
	int score = 0;
	int remains;
	while (true) {
		int count;
		int max = 0;

		pair<int, int>coord{ 9,14 };
		char c;
		for (int i = board.size() - 1; i >= 0; --i) {
			for (int j = 0; j < board[0].size(); ++j) {

				if (board[i][j] != '_')
				{

					count = 0;
					vector<vector<bool>> tabl{ board.size(),vector<bool>(board[0].size()) };//matrix to check which cell is visited
					find_cluster(i, j, board, tabl, count, board[i][j]);
					if (count > max)
					{

						max = count;
						coord = { i,j };
						c = board[i][j];


					}
					else if (count == max && j < coord.second) {
						max = count;
						coord = { i,j };
						c = board[i][j];

					}
					else if (count == max && j<coord.second && i>coord.first) {
						max = count;
						coord = { i,j };
						c = board[i][j];
					}

				}

			}
		}
		if (max <= 1)// if maximum cluster consists of 1 ball or the board is empty this means the game is over
		{
			remains = remaining_balls(board);
			if (!remains)
				score += 1000;
			break;
		}
		else {
			++turn;
			score += (max - 2) * (max - 2);

			cout << "Move " << turn << " at (" << abs(coord.first - 10) << "," << coord.second + 1 << ") : removed " << max << " balls of color " << board[coord.first][coord.second] << ", got " << (max - 2) * (max - 2) << " points" << endl;
			erase(coord.first, coord.second, board, board[coord.first][coord.second]);
			down(board);
			left(board);

		}
	}
	cout << "Final score: " << score << ", with " << remains << " balls remaining." << endl << endl;

}


using namespace std;



int main() {

	int n;
	cout << "Enter number of games" << endl;
	cin >> n;
	vector<string> board(10);
	cin.ignore();
	for (int k = 0; k < n; ++k) {
		cout << "Enter the field for Game number " << k + 1 << endl;
		for (int i = 0; i < 10; ++i) {
			getline(cin, board[i]);
		}
		cout << "Game " << k + 1 << ": " << endl;
		rgbgame(board);

	}

	system("pause");
	return 0;
}
