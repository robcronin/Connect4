#include "C4board.h"


// virtual Player class with mandatory move function
class Player{
	protected:
	public:
		virtual int move(C4board& game)=0;		// move function required by all derived classes

};

// human class which reads in moves from stdin
class Human: public Player{
	private:
		std::string name;				// name of player

	public:
		Human(std::string p);				// constructor
		int move(C4board& game);			// Human move
			 	
};

// AI that works off a few basic strategies
class AIBasic: public Player{
	private:
		int AI_player;					// player of AI

	public:
		AIBasic(int p);					// constructor
		bool is_trap(C4board& game, int player);	// looks for a trap (2 ways of winning)
		int move(C4board& game);			// AI move
		int choose_move(C4board& game, int alt_player=0);	// function called by "move"
};

// Attempt at creating an alpha-beta prune algorithm
// Was too slow however
class ABPrune: public Player{
	private:
		int AI_player;					// AI player

	public:
		ABPrune(int p);					// constructor
		int abprune(C4board& game, int alpha, int beta, int nextp);	// Alpha-Beta prune method
		int move(C4board& game);			// ABPrune move
};

