#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;
int move_list[100][2];
char** flip(char** board, int row, int col, char player, int length) {
	// declare a list of positions of discs that will be flipped
	// e.g. {{0,1}, {0,2}} means disc at location board[0][1] & board[0][2] will be flipped
	int discs_to_flip[60][2];
	for (int i = 0; i < 60; i++)
		for (int j = 0; j < 2; j++)
			discs_to_flip[i][j] = -1;
	int counter = 0;
	char otherPlayer = (player == 'b') ? 'w' : 'b';

	// use deltas to find all 8 surrounding positions
	int surroundingPosDeltas[8][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 }, // 3 positions above
	{ 0, -1 },{ 0, 1 }, // 2 positions on same row
	{ 1, -1 },{ 1, 0 },{ 1, 1 } }; // 3 positions below

								   // for every delta representing a neighboring position...
	for (auto deltas : surroundingPosDeltas) {
		// cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';

		// save what row/col currently on
		int curr_row = row + deltas[0];
		int curr_col = col + deltas[1];

		// ignore if this goes off of the board
		if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
			continue;


		// save character in this position
		char char_in_pos = board[curr_row][curr_col];

		// use this variable to save whether or not a line of pieces should be flipped
		int flip_this_direction = 0;

		// if the character in this delta position is the opponent's piece...
		if (char_in_pos == otherPlayer) {
			// cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

			// continue in this delta position until the next character is no longer the opponent's or you go off the board
			while (char_in_pos == otherPlayer) {
				curr_row += deltas[0];
				curr_col += deltas[1];

				// check to see if new position is off board
				if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
					break;

				// save the character
				char_in_pos = board[curr_row][curr_col];
			}

			// if the player's piece is found after traversing over the opponent's piece(s), we know we will be flipping
			if (char_in_pos == player)
				flip_this_direction = 1;

			// if we found out we should be flipping...
			if (flip_this_direction == 1) {
				// save current position
				curr_row = row + deltas[0];
				curr_col = col + deltas[1];
				char_in_pos = board[curr_row][curr_col];

				// traverse over the opponent's pieces, while saving the positions to the big list to be flipped later
				while (char_in_pos == otherPlayer) {
					// cout << "flipping [" << curr_row << ", " << curr_col << "]\n";
					discs_to_flip[counter][0] = curr_row;
					discs_to_flip[counter][1] = curr_col;
					counter++;
					curr_row += deltas[0];
					curr_col += deltas[1];

					// save next character
					char_in_pos = board[curr_row][curr_col];
				}

			}
		}
	}

	// after we've collecting the row/col of all discs to flipped, flip them to the current player's color/character
	for (int i = 0; discs_to_flip[i][0] != -1; i++) {
		//cout << discs_to_flip[i][0] << " " << discs_to_flip[i][1] << endl;
		//cout << player << endl;
		board[discs_to_flip[i][0]][discs_to_flip[i][1]] = player;
		//cout << board[discs_to_flip[i][0]][discs_to_flip[i][1]] << endl;
	}
	return board;
}
char** makeMove(char** board, int row, int col, char player, int length) {
	// cout << "Updating row: " << row << " col: " << col << '\n';
	// set provided row/col position to the player's character piece
	board[row][col] = player;

	// flip discs from resulting move
	board = flip(board, row, col, player, length);
	//cout << board[3][3] << endl;
	return board;
}
char Player_choice() {
	cout << "Enter 'b' to play as black or 'w' to play as white: ";
	string selected_player;
	// loop until user makes a valid choice of player
	while (true)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> selected_player;
		//cout << selected_player<<endl;
		char input = selected_player[0];
		if (input != 'w' && input != 'b') {
			cout << "\nInvalid input: \"Enter 'b' to be black or 'w' to be white. \n";
			continue;
		}
		break;
	}

	char player_char = selected_player[0];
	cout << "You have chosen to play as " << ((player_char == 'w') ? "white" : "black") << "!\n\n";

	return player_char;
}
struct Map {
	int empty_cells;
	int turn;
	int map_size;
};
struct Player {
	int num_choices;
	int mode;
	int win_cells;
};
int isFlippable(char** board, int row, int col, char player) {
	char otherPlayer = (player == 'b') ? 'w' : 'b';

	// Check all 8 surround positions
	int surroundingPosDeltas[8][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 }, // 3 positions above
	{ 0, -1 },{ 0, 1 }, // 2 positions on same row
	{ 1, -1 },{ 1, 0 },{ 1, 1 } }; // 3 positions below

								   // for every delta of the surrounding positions
	for (auto deltas : surroundingPosDeltas) {

		// skip if the position is off of game board
		if (row + deltas[0] > 7 || row + deltas[0] < 0 || col + deltas[1] > 7 || col + deltas[1] < 0) {
			continue;
		}

		// cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';
		char char_in_pos = board[row + deltas[0]][col + deltas[1]]; // grab the character in that spot

																	// if the character in this delta spot is the opponent's piece...
		if (char_in_pos == otherPlayer) {
			// save spot's row and col #
			int curr_row = row + deltas[0];
			int curr_col = col + deltas[1];

			// cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

			//continue along this delta trajectory until you stop seeing the opponent's pieces
			while (char_in_pos == otherPlayer) {
				curr_row += deltas[0];
				curr_col += deltas[1];

				// check to see if new position is off board
				if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
					break;

				// save the next character
				char_in_pos = board[curr_row][curr_col];
			}

			// if the player's piece is seen after one (+more) of the opponent's pieces, the original move is a flippable one
			if (char_in_pos == player)
				return 1;
		}
	}

	// if no flippable spot is found after checking all surrounding positions, the original move is not a flippable one
	return 0;
}
void calculateLegalMoves(char** board, char player, int length, struct Player *player0) {
	// declare main move list

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 2; j++)
			move_list[i][j] = -1;
	}
	int counter = 0;
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			// first make sure the spot is empty
			if (board[i][j] == '-') {

				// check to see if placing a piece there will flip one (+more) of the opponent's pieces
				if (isFlippable(board, i, j, player)) {

					// if so, create a 2-element vector representative of the move and push it to the big move list
					move_list[counter][0] = i;
					move_list[counter][1] = j;
					counter++;
				}

			}
		}
	}
	player0->num_choices = counter;
}
void printLegalMoves() {
	for (int i = 0; move_list[i][0] != -1; i++) {
		for (int j = 0; j<1; j++)
			cout << "(" << move_list[i][j] << "," << move_list[i][j + 1] << ")  ";
	}
	cout << endl;
}
// return a list of all the moves available to white
char** firstValue(char** board, int length) {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			board[i][j] = '-';
		}
	}

	board[(length / 2) - 1][(length / 2) - 1] = 'w'; board[(length / 2) - 1][(length / 2)] = 'b';
	board[(length / 2)][(length / 2) - 1] = 'b'; board[(length / 2)][(length / 2)] = 'w';
	return board;
}
int Check_size(struct Map map) {
	if (map.empty_cells == 0) {
		return 1;
	}
	else
		return 0;
}
int getScore(char** board, char player) {
	int total = 0;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			if (board[i][j] == player)
				total += 1;

	return total;
}
void print(char**Board, int length) {
	cout << "   ";
	for (int i = 0; i < length; i++) {
		cout << i;
		cout << "  ";
	}
	cout << "\n";
	for (int i = 0; i < length; ++i) {
		cout << (i) << "  ";
		for (int j = 0; j < length; ++j) {
			cout << Board[i][j] << "  ";
		}
		cout << '\n';
	}
}
int Check_OK(char** board, int row, int col, char player, int length) {
	int proposedMove[2] = { row, col };
	//    for (int i : proposedMove) {
	//         cout << i << ' ';
	//    }

	//This error should NOT occur, as the regex pattern validates the user's input
	if (row > length || row < 0 || col > length || col < 0)
		throw  range_error{ "Check_OK()" };

	// Make sure position is empty
	if (board[row][col] != '-') {

		return 0;
	}

	for (int i = 0; move_list[i][0] != -1; i++) {
		if (move_list[i][0] == row && move_list[i][1] == col) {
			return 1;
		}
	}

	return 0;
}
void CalculateLegalMoves(char** board, char player, int length, struct Player *player1, struct Player *player2) {
	if (player == 'w') {
		calculateLegalMoves(board, player, length, player1);
	}
	else {
		calculateLegalMoves(board, player, length, player2);
	}
}
void Show_states(struct Player player1, struct Player player2, char player) {
	if (player == 'w')
		cout << "you have " << player1.num_choices << " legal move to play\n";
	else
		cout << "you have " << player2.num_choices << " legal move to play\n";
}
void print_winner(struct Player player1, struct Player player2) {
	if (player1.win_cells > player2.win_cells)
		cout << "WHITE WON!\n";
	if (player1.win_cells < player2.win_cells)
		cout << "BLACK WON!\n";
	if (player1.win_cells == player2.win_cells)
		cout << "DRAW!\n";
}
int main()
{

	while (1) {
		cout << "if you want to start the game input 1\n";
		cout << "if you want to  change the grounds setting input 2\n";
		cout << "if you want to exit input 3\n";
		int n = 0;
		cin >> n;
		char** Board;
		int total_moves = 0;
		char player = 'b';
		if (n == 1) {
			struct Map map1;
			map1.empty_cells = 60;
			map1.map_size = 64;
			system("CLS");
			Board = (char**)malloc(8 * sizeof(char**));
			for (int i = 0; i < 8; i++)
				Board[i] = (char*)malloc(8 * sizeof(char*));
			Board = firstValue(Board, 8);
			cout << "if you want to play with another player input 1\n";
			cout << "if you want to play with PC input 2\n";
			int AI = 0;
			cin >> AI;
			if (AI == 1) {
				struct Player player1;
				player1.mode = 0;
				player1.num_choices = 0;
				struct Player player2;
				player2.mode = 0;
				player2.num_choices = 0;

				system("CLS");
				char player1_choice = Player_choice();
				char player2_choice = ((player1_choice == 'w') ? 'b' : 'w');
				while (Check_size(map1) == 0) {
					print(Board, 8);
					CalculateLegalMoves(Board, player, 8, &player1, &player2);
					cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
					Show_states(player1, player2, player);
					printLegalMoves();
					CalculateLegalMoves(Board, 'b', 8, &player1, &player2);
					if (player == 'b' && player2.num_choices == 0) {
						cout << "Black is out of moves, PASS to White.\n";
						player = 'w';
						continue;
					}
					CalculateLegalMoves(Board, 'w', 8, &player1, &player2);
					if (player == 'w' && player1.num_choices == 0) {
						cout << "White is out of moves, PASS to Black.\n";
						player = 'b';
						continue;
					}
					if (player == 'b') {
						CalculateLegalMoves(Board, 'b', 8, &player1, &player2);
					}
					else {
						CalculateLegalMoves(Board, 'w', 8, &player1, &player2);
					}
					cout << ((player == 'w') ? "White's Move: " : "Black's Move: ");
					cout << "just to be sure enter twice in a row" << endl;
					string user_input;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//cin.getline(user_input, 3);
					getline(cin, user_input);
					//cin >> user_input;
					cout << "You entered: " << user_input[0]-'0'<<" "<< user_input[2] - '0' << '\n';
					int first_number = user_input[0] - '0';
					int second_number = user_input[2] - '0';
					if (first_number<-1 || first_number>8 || second_number<-1 || second_number>8 || user_input[1] != ' ') {
						cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
						cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
						continue;
					}

				/*	if (!regex_match(user_input, move_input_pattern)) {
						cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
						cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
						continue;
					}*/

					// user_input = [<some num>, " ", <some num>], nums will be at indices 0 and 2
					// subtract '0's ascii value (48) from the user nums to get the real integer
					int row = user_input[0] - '0';
					int col = user_input[2] - '0';
					try {
						if (Check_OK(Board, row, col, player, 8) == 1) {
							Board = makeMove(Board, row, col, player, 8);
							map1.empty_cells--;
						}
						else {
							cout << "Illegal move! Try again.\n";
							continue;
						}
					}
					catch (range_error& e) {
						cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
						return 1;
					}

					total_moves += 1;
					int white_total = getScore(Board, 'w');
					player1.win_cells = white_total;
					int black_total = getScore(Board, 'b');
					player2.win_cells = black_total;
					//print(Board, 8);
					cout << "Black total: " << black_total << '\n';
					cout << "White total: " << white_total << "\n\n";

					// Switch players
					player = (player == 'w') ? 'b' : 'w';
				}
				print_winner(player1, player2);
			}
			if (AI == 2) {
				system("CLS");
				char player_choice = Player_choice();
				char AI_choice = ((player_choice == 'w') ? 'b' : 'w');
				struct Player player1;
				player1.num_choices = 0;
				struct Player player2;
				player2.num_choices = 0;
				if (player_choice == 'w') {
					player1.mode = 0;
					player2.mode = 1;
				}
				else {
					player1.mode = 1;
					player2.mode = 0;
				}
				while (Check_size(map1) == 0) {
					//print(Board, 8);
					CalculateLegalMoves(Board, player, 8, &player1, &player2);
					//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
					//printLegalMoves();


					CalculateLegalMoves(Board, 'b', 8, &player1, &player2);
					if (player == 'b' && player2.num_choices == 0) {
						cout << "Black is out of moves, PASS to White.\n";
						player = 'w';
						continue;
					}
					CalculateLegalMoves(Board, 'w', 8, &player1, &player2);
					if (player == 'w' && player1.num_choices == 0) {
						cout << "White is out of moves, PASS to Black.\n";
						player = 'b';
						continue;
					}
					if (player == 'b') {
						//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						CalculateLegalMoves(Board, 'b', 8, &player1, &player2);
					}
					else {
						//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						CalculateLegalMoves(Board, 'w', 8, &player1, &player2);
					}
					total_moves += 1;
					int white_total = getScore(Board, 'w');
					player1.win_cells = white_total;
					int black_total = getScore(Board, 'b');
					player2.win_cells = black_total;
					//print(Board, 8);
					cout << "Black total: " << black_total << '\n';
					cout << "White total: " << white_total << "\n\n";
					print(Board, 8);
					cout << endl;
					if (player == player_choice) {
						cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						Show_states(player1, player2, player);
						printLegalMoves();

						string user_input;

						while (true) {
							// Print input prompt
							cout << ((player == 'w') ? "Your move (w): " : "Your move (b): ");
							cin.ignore();
							getline(cin, user_input);
							int first_number = user_input[0] - '0';
							int second_number = user_input[2] - '0';
							if (first_number<-1 || first_number>8 || second_number<-1 || second_number>8 || user_input[1] != ' ') {
								cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
								cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
								continue;
							}

							else {
								// user_input = [<some num>, " ", <some num>], nums will be at indices 0 and 2
								// subtract '0's ascii value (48) from the user nums to get the real integer
								int row = user_input[0] - '0';
								int col = user_input[2] - '0';

								try {
									// if the inserted move is legal, make the move
									if (Check_OK(Board, row, col, player, 8) == 1) {
										Board = makeMove(Board, row, col, player, 8);
										map1.empty_cells--;
									}
									else {
										cout << "Illegal move! Try again.\n";
										continue;
									}
								}
								catch (range_error& e) {
									cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
									return 1;
								}
								player = (player == 'w') ? 'b' : 'w';
								break;
							}
						}
					}
					else {
						CalculateLegalMoves(Board, player, 8, &player1, &player2);
						int counter = 0;
						for (int i = 0; move_list[i][0] != -1; i++) {
							counter++;
						}
						int ai_choice = rand() % counter;
						int row = move_list[ai_choice][0];
						int col = move_list[ai_choice][1];
						try {
							// if the inserted move is legal, make the move
							if (Check_OK(Board, row, col, player, 8) == 1) {
								Board = makeMove(Board, row, col, player, 8);
								map1.empty_cells--;
							}
							else {
								cout << "Illegal move! Try again.\n";
								continue;
							}
						}
						catch (range_error& e) {
							cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
							return 1;
						}
						player = (player == 'w') ? 'b' : 'w';
					}
				}
				print_winner(player1, player2);
			}
		}
		if (n == 2) {
			struct Player player1;
			player1.mode = 0;
			player1.num_choices = 0;
			struct Player player2;
			player2.mode = 0;
			player2.num_choices = 0;
			cout << "please enter the length you want (4 - 20)\n";
			int length = 0;
			cin >> length;
			struct Map map2;
			map2.empty_cells = (length*length) - 4;
			map2.map_size = length*length;
			char** Board;
			Board = (char**)malloc(length * sizeof(char**));
			for (int i = 0; i < length; i++)
				Board[i] = (char*)malloc(length * sizeof(char*));
			Board = firstValue(Board, length);
			cout << "if you want to play with another player input 1\n";
			cout << "if you want to play with PC input 2\n";
			int AI = 0;
			cin >> AI;
			if (AI == 1) {
				system("CLS");
				char player1_choice = Player_choice();
				char player2_choice = ((player1_choice == 'w') ? 'b' : 'w');
				while (Check_size(map2) == 0) {
					print(Board, length);
					CalculateLegalMoves(Board, player, length, &player1, &player2);
					cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
					Show_states(player1, player2, player);
					printLegalMoves();
					CalculateLegalMoves(Board, 'b', length, &player1, &player2);
					if (player == 'b' && move_list[0][0] == -1) {
						cout << "Black is out of moves, PASS to White.\n";
						player = 'w';
						continue;
					}
					CalculateLegalMoves(Board, 'w', length, &player1, &player2);
					if (player == 'w' && move_list[0][0] == -1) {
						cout << "White is out of moves, PASS to Black.\n";
						player = 'b';
						continue;
					}
					if (player == 'b') {
						CalculateLegalMoves(Board, 'b', length, &player1, &player2);
					}
					else {
						CalculateLegalMoves(Board, 'w', length, &player1, &player2);
					}
					cout << ((player == 'w') ? "White's Move: " : "Black's Move: ");

					string user_input;
					cin.ignore();
					getline(cin, user_input);
					cout << "You entered: " << user_input << '\n';

					int first_number = user_input[0] - '0';
					int second_number = user_input[2] - '0';
					if (first_number<-1 || first_number>8 || second_number<-1 || second_number>8 || user_input[1] != ' ') {
						cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
						cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
						continue;
					}


					// user_input = [<some num>, " ", <some num>], nums will be at indices 0 and 2
					// subtract '0's ascii value (48) from the user nums to get the real integer
					int row = user_input[0] - '0';
					int col = user_input[2] - '0';
					try {
						if (Check_OK(Board, row, col, player, length) == 1) {
							Board = makeMove(Board, row, col, player, length);
							map2.empty_cells--;
						}
						else {
							cout << "Illegal move! Try again.\n";
							continue;
						}
					}
					catch (range_error& e) {
						cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
						return 1;
					}

					total_moves += 1;
					int white_total = getScore(Board, 'w');
					player1.win_cells = white_total;
					int black_total = getScore(Board, 'b');
					player2.win_cells = black_total;
					//print(Board, 8);
					cout << "Black total: " << black_total << '\n';
					cout << "White total: " << white_total << "\n\n";

					// Switch players
					player = (player == 'w') ? 'b' : 'w';
				}
				print_winner(player1, player2);
			}
			if (AI == 2) {
				system("CLS");
				char player_choice = Player_choice();
				char AI_choice = ((player_choice == 'w') ? 'b' : 'w');
				struct Player player1;
				player1.num_choices = 0;
				struct Player player2;
				player2.num_choices = 0;
				if (player_choice == 'w') {
					player1.mode = 0;
					player2.mode = 1;
				}
				else {
					player1.mode = 1;
					player2.mode = 0;
				}
				while (Check_size(map2) == 0) {
					//print(Board, 8);
					CalculateLegalMoves(Board, player, length, &player1, &player2);
					//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
					//printLegalMoves();
					CalculateLegalMoves(Board, 'b', length, &player1, &player2);
					if (player == 'b' && move_list[0][0] == -1) {
						cout << "Black is out of moves, PASS to White.\n";
						player = 'w';
						continue;
					}
					CalculateLegalMoves(Board, 'w', length, &player1, &player2);
					if (player == 'w' && move_list[0][0] == -1) {
						cout << "White is out of moves, PASS to Black.\n";
						player = 'b';
						continue;
					}
					if (player == 'b') {
						//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						CalculateLegalMoves(Board, 'b', length, &player1, &player2);
					}
					else {
						//cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						CalculateLegalMoves(Board, 'w', length, &player1, &player2);
					}
					total_moves += 1;
					int white_total = getScore(Board, 'w');
					player1.win_cells = white_total;
					int black_total = getScore(Board, 'b');
					player2.win_cells = black_total;
					//print(Board, 8);
					cout << "Black total: " << black_total << '\n';
					cout << "White total: " << white_total << "\n\n";
					print(Board, length);
					cout << endl;
					if (player == player_choice) {
						cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
						printLegalMoves();

						string user_input;
						while (true) {
							// Print input prompt
							cout << ((player == 'w') ? "Your move (w): " : "Your move (b): ");
							cin.ignore();
							getline(cin, user_input);
							int first_number = user_input[0] - '0';
							int second_number = user_input[2] - '0';
							if (first_number<-1 || first_number>8 || second_number<-1 || second_number>8 || user_input[1] != ' ') {
								cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
								cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
								continue;
							}

							else {
								// user_input = [<some num>, " ", <some num>], nums will be at indices 0 and 2
								// subtract '0's ascii value (48) from the user nums to get the real integer
								int row = user_input[0] - '0';
								int col = user_input[2] - '0';

								try {
									// if the inserted move is legal, make the move
									if (Check_OK(Board, row, col, player, length) == 1) {
										Board = makeMove(Board, row, col, player, length);
										map2.empty_cells--;
									}
									else {
										cout << "Illegal move! Try again.\n";
										continue;
									}
								}
								catch (range_error& e) {
									cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
									return 1;
								}
								player = (player == 'w') ? 'b' : 'w';
								break;
							}
						}
					}
					else {
						CalculateLegalMoves(Board, player, length, &player1, &player2);
						int counter = 0;
						for (int i = 0; move_list[i][0] != -1; i++) {
							counter++;
						}
						int ai_choice = rand() % counter;
						int row = move_list[ai_choice][0];
						int col = move_list[ai_choice][1];
						try {
							// if the inserted move is legal, make the move
							if (Check_OK(Board, row, col, player, length) == 1) {
								Board = makeMove(Board, row, col, player, length);
								map2.empty_cells--;
							}
							else {
								cout << "Illegal move! Try again.\n";
								continue;
							}
						}
						catch (range_error& e) {
							cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
							return 1;
						}
						player = (player == 'w') ? 'b' : 'w';
					}
				}
				print_winner(player1, player2);
			}
		}
		if (n == 3) {
			break;
		}
	}
	system("PAUSE");
	return 0;
}

