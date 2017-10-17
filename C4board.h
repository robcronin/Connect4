#include <iostream>
#include <unistd.h>

class C4board{
	private:
		int *board;
		int rows;
		int cols;
		int moves_made;
	public:

		C4board();					// constructor
		~C4board();					// destructor
		C4board(const C4board& board_in);		// deep copy
		C4board& operator=(const C4board& board_in);	// deep assignment

		int noRows();					// returns number of rows
		int noCols();					// number of cols
		int noMoves();					// number of moves

		void refresh();					// resets the board
		bool legal(const int entry);			// checks if move legal
		int insert(const int entry, const int player);	// inserts move
		int insert_animate(const int entry, const int player);	// insert w/ animation
		void remove(const int entry);			// removes from column
		void print();					// prints board
		void highlight(const int player, const int r, const int c, const int dir);	// highlights winner
		bool is_winner(const int player, bool hl);	// checks if "player" won
		bool is_winning_move(const int entry, int player);	// checks if a move will win
};
