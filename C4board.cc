#include "C4board.h"

// constructor
C4board::C4board():board{new int[42]}, rows{6}, cols{7}, moves_made{0}{
	for(int i=0;i<42;++i){board[i]=0;}	
}

// destructor
C4board::~C4board(){delete[] board;}

// deep copy constructor
C4board::C4board(const C4board& board_in):rows{6}, cols{7}, moves_made{board_in.moves_made}{
	if(board_in.board!=0){
		board=new int[42];
		for(int i=0;i<42;++i){
			board[i]=board_in.board[i];
		}
	}
	else{
		board=0;
	}
}

// deep assignment
C4board& C4board::operator=(const C4board& board_in){
	if(this== &board_in){return *this;}

	int *b=new int[42];
	std::copy(board_in.board, board_in.board+42,b);

	delete[] this->board;
	board=b;
	this->rows=6;
	this->cols=7;
	return *this;
}

int C4board::noRows(){return rows;}
int C4board::noCols(){return cols;}
int C4board::noMoves(){return moves_made;}

void C4board::refresh(){
	for(int i=0;i<rows*cols;++i){board[i]=0;}
	moves_made=0;
}
	
// checks if a move is legal by checking the top row
bool C4board::legal(const int entry){
	if(board[entry-1]==0){return true;}
	else{return false;}
}

// finds a spot for a piece to go and enters it
int C4board::insert(const int entry, const int player){
	if(legal(entry) && moves_made<rows*cols){
		for(int i=rows-1;i>=0;--i){
			if(board[i*cols+entry-1]==0){
				board[i*cols+entry-1]=player;
				moves_made++;
				return i;
			}
		}
	}
	else{
		std::cout<<"ERROR ERROR ERROR\n";
	}
	return -1;
}

// inserts but prints to simulate dropping of the piece (assumes only called on legal move)
int C4board::insert_animate(const int entry, const int player){
	for(int i=0;i<rows;++i){
		// places it in the next available row
		board[i*cols+entry-1]=player;
		usleep(75000);
		print();

		// if its the final row or the next row is full then thats the position and break
		if(i==rows-1 || board[(i+1)*cols+entry-1]!=0){
			moves_made++;			
			return i;
		}
		// otherwise clears that row
		board[i*cols+entry-1]=0;
	}
	return -1;
}

// loops through and removes the first stone in a given column
void C4board::remove(const int entry){
	for(int i=0;i<rows;++i){
		if(board[i*cols+entry-1]!=0){
			board[i*cols+entry-1]=0;
			moves_made--;
			return;
		}
	}
}
		
// prints the board in colour based on which player is where
void C4board::print(){
	system("clear");
	std::cout<<"\x1b[2;37m";
	for(int i=0;i<cols;++i){
		std::cout<<i+1<<"  ";
	}
	std::cout<<"\n";
	for(int i=0;i<cols*3-2;++i){std::cout<<"-";}
	std::cout<<"\n";
	for(int i=0;i<rows;++i){
		for(int j=0;j<cols;++j){
			switch(board[i*cols+j]){
				case 0:
					std::cout<<"\x1b[2;37mo  ";
					break;
				case 1:
					std::cout<<"\x1b[1;91m@  ";
					break;
				case 2:
					std::cout<<"\x1b[2;93m@  ";
					break;		
				// case 3 is for highlighting
				case 3:
					std::cout<<"\x1b[1;92m@  ";
					break;
			}
		}
		std::cout<<"\n";
	}
	std::cout<<"\x1b[0m";	// resets the colour
}

// highlight function that highlights 4 in a row from a given spot in a given direction
void C4board::highlight(const int player, const int r, const int c, const int dir){
	int tr, tc;
	int loops=20;	// "flashes" loop number of times

	// dir 1 is for 4 in a row on a row (goes backwards from start point)
	if(dir==1){
		for(int j=0;j<loops;++j){
			tr=r, tc=c;
			// changes to 3s (so they will be green in print)
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=3;
				--tc;
			}
			print();					// prints
			std::cout<<"Player "<<player<<" won!\n";	// says who won
			usleep(100000);					// waits between prints
			tr=r, tc=c;					// resets start point
			
			// changes back to player colour
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=player;
				--tc;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
		}
	}
	// similar for vertical 4 in a row
	else if(dir==2){
		for(int j=0;j<loops;++j){
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=3;
				++tr;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=player;
				++tr;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
		}
	}
	// similar for diagonal up to the right
	else if(dir==3){
		for(int j=0;j<loops;++j){
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=3;
				++tr;--tc;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=player;
				++tr;--tc;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
		}
	}
	// similar for diagonal down to the right
	else if(dir==4){
		for(int j=0;j<loops;++j){
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=3;
				--tr;--tc;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
			tr=r, tc=c;
			for(int i=0;i<4;i++){
				board[tr*cols+tc]=player;
				--tr;--tc;
			}
			print();
			std::cout<<"Player "<<player<<" won!\n";
			usleep(100000);
		}
	}
}

// checks if a given player has won based on the board (will highlight winner if requested)
bool C4board::is_winner(const int player, bool hl){
	int counter=0;
	// checks rows
	for(int i=rows-1;i>=0;--i){
		counter=0;
		for(int j=0;j<cols-3+counter;++j){
			// if each spot is a certain player then it bumps the counter
			if(board[i*cols+j]==player){
				counter++;
				// if the counter hits 4 then it highlights if requested and returns true
				if(counter==4){
					if(hl){
						highlight(player, i, j, 1);
					}
					return true;
				}
			}
			// else resets the counter
			else{counter=0;}
		}
	}
					
	// similarly checks cols
	for(int j=0;j<cols;++j){
		counter=0;
		for(int i=rows-1;i>=3-counter;--i){
			if(board[i*cols+j]==player){
				counter++;
				if(counter==4){
					if(hl){
						highlight(player, i, j, 2);
					}
					return true;
				}
			}
			else{counter=0;}
		}
	}

	// similarly checks diag up
	// 	first starting from the left wall
	for(int i=rows-1;i>=3;--i){
		counter=0;
		for(int j=0;i-j>=0;++j){
			if(board[(i-j)*cols+j]==player){
				counter++;
				if(counter==4){
					if(hl){
						highlight(player, i-j, j, 3);
					}
					return true;
				}
			}
			else{counter=0;}
		}
	}
	//	then from bottom row
	for(int j=1;j<=3;++j){
		counter=0;
		for(int i=rows-1;i-j+1>=0;--i){
			if(board[i*cols+j+rows-i-1]==player){
				counter++;
				if(counter==4){
					if(hl){
						highlight(player, i, j+rows-i-1, 3);
					}
					return true;
				}
			}
			else{counter=0;}
		}
	}

	// similarly checks diag down
	// 	first from left wall
	for(int i=0;i<=2;++i){
		counter=0;
		for(int j=0;i+j<rows;++j){
			if(board[(i+j)*cols+j]==player){
				counter++;
				if(counter==4){
					if(hl){
						highlight(player, i+j, j, 4);
					}
					return true;
				}
			}
			else{counter=0;}
		}
	}
	// 	then from top row
	for(int j=1;j<=3;++j){
		counter=0;
		for(int i=0;i+j<cols;++i){
			if(board[i*cols+i+j]==player){
				counter++;
				if(counter==4){
					if(hl){
						highlight(player, i, i+j, 4);
					}
					return true;
				}
			}
			else{counter=0;}
		}
	}
	return false;
}	

// checks if a given move will win for a given player
bool C4board::is_winning_move(const int entry, int player){
	bool winner=false;
	// checks it is legal
	if(legal(entry)){
		insert(entry, player);			// enters the move
		winner=is_winner(player,false);		// checks if that wins
		remove(entry);				// removes the move
	}
	return winner;					// returns true or false
	
}	
