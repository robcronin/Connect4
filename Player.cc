#include "Player.h"

// Constructor for Human Class
Human::Human(std::string p){name=p;}

// takes in an input from stdin
int Human::move(C4board& game){
	int ans;
	// Asks the player for an input
	std::cout<<name<<" please enter a move: ";
	std::cin >> ans;	// takes it in

	// ensures it is within the limits and is legal, else asks again
	while(ans<1 || ans>game.noCols() || game.legal(ans)==false){
		std::cout<<"ERROR\n"<<name<<" please enter a valid move between 1 and "<<game.noCols()<<": ";
		std::cin >> ans;	// takes it in
	std::cout<<ans<<"\n";
	}
	return ans;
	
}

// AIBasic constructor
AIBasic::AIBasic(int p):AI_player{p}{}

// function to check if a trap is set for a certain player
// 	i.e. they have two possible wins => guaranteed to win
bool AIBasic::is_trap(C4board& game, int player){
	int wins=0;
	// checks if each column provides a win for the player
	for(int j=1;j<=game.noCols();++j){
		if(game.is_winning_move(j,player)){
			wins++;
		}
		// if there is two possible moves then return true
		if(wins==2){
			return true;
		}
	}
	return false;
}

// AI_Basic move function
// 	note that alt_player is set to 0 by default
int AIBasic:: choose_move(C4board& game, int alt_player){
	int player;

	// Majority of the time we just want a move for AI
	if(alt_player==0){player=AI_player;}
	// sometimes we might want to see what the opp would play using the same strategy
	else{player=alt_player;}

	if(game.noMoves()<2){return game.noCols()/2+1;}	// starts with best strategy
	int opp= (player%2)+1;			// holder for the other player


	//*****************************************
	//************ INITIAL CHECKS *************
	//*****************************************

	// ATTACK: checks if AI can win
	for(int i=1;i<=game.noCols();i++){
		if(game.is_winning_move(i,player)){
			return i;
		}
	}

	// DEFENCE: blocks opp if they are about to win
	for(int i=1;i<=game.noCols();i++){
		if(game.is_winning_move(i,opp)){
			return i;
		}
	}

	//*****************************************
	//******** SETS UP COLUMN WEIGHTS *********
	//*****************************************

	// weighting based on centre columns being more valuable
	// For standard board the weights end up being [5,25,75,100,75,25,5]
	int *col_weights=new int[game.noCols()];
	int centre=game.noCols()/2;;

	// sets up middle cols odd number of cols
	if(game.noCols()%2==1){
		col_weights[centre]=100;
		col_weights[centre-1]=col_weights[centre+1]=75;
	}

	// else if even number
	else{
		centre--;
		col_weights[centre]=col_weights[centre+1]=100;
		col_weights[centre-1]=col_weights[centre+2]=75;
	}

	// fills in the rest based on decreasing fraction of last col
	int divider=3, weight;
	for(int i=centre-2;i>=0;--i){
		weight=col_weights[i+1]/divider;
		if(weight<1){weight=1;}	
		col_weights[i]=col_weights[game.noCols()-1-i]=weight;
		divider+=2;
	}
	
			
	// checks for non-legal moves, then makes that weight 0
	for(int i=1;i<=game.noCols();++i){
		if(game.legal(i)==false){col_weights[i-1]=0;}
	}

	
	// DEFENCE: Checks to avoid giving winning move to opp
	for(int i=1;i<=game.noCols();++i){
		if(game.legal(i)){
			// inserts each of AI's move
			game.insert(i,player);

			// checks if each AI move will give opp a winning move
			for(int j=1;j<=game.noCols();++j){
				if(game.is_winning_move(j,opp)){
					col_weights[i-1]=-100;	// sets the weight to -100 if so
					break;
				}
			}
			game.remove(i);		// undoes the move
		}
	}

	// DEFENCE: blocks a trap
	for(int i=1;i<=game.noCols();++i){
		if(col_weights[i-1]>0){		// checks through move still with positive weight
			game.insert(i,opp);	// inserts a move for the opp
			// if that move gives the opp a trap then block that move
			if(is_trap(game, opp)){
				game.remove(i);
				delete[] col_weights;
				return i;
			}
			game.remove(i);
		}
	}

	// DEFENCE: Avoid setting up an easy trap
	for(int i=1;i<=game.noCols();++i){
		if(col_weights[i-1]>0){
			game.insert(i,player);		// inserts a move for AI
			for(int j=1;j<=game.noCols();++j){
				if(game.legal(j)){
					game.insert(j,opp);	// checks each of opps moves
					if(is_trap(game,opp)){	// if its a trap
						col_weights[i-1]=-50;	// then -50 weight
					}
					game.remove(j);
				}			
			}
			game.remove(i);
		}
	}

	// this move is recursive so only run for original entryn for speed reasons
	if(alt_player==0){
		// DEFENCE: try to block opp from setting up win or trap
		for(int i=1;i<=game.noCols();++i){
			if(col_weights[i-1]>0){
				game.insert(i, player);			// inserts each of players moves
				int oppmove=choose_move(game, opp);
				if(oppmove!=-1){
					game.insert(oppmove, opp);	// inserts opp's move
					// sees what we would do in response
					int ourmove=choose_move(game, player);	
					// if we have a definite move
					if(ourmove!=-1 && !game.is_winning_move(ourmove, player)){
						// insert it
						game.insert(ourmove,player);
						// check if opp wins
						for(int k=1;k<=game.noCols();++k){
							// -20 weight if so
							if(game.is_winning_move(k, opp)){
								col_weights[i-1]=-20;
							}
						}
						game.remove(ourmove);
						// checks if a trap is on (-10 weight if so)
						if(is_trap(game, opp)){
							col_weights[i-1]=-10;
						}
					}
					game.remove(oppmove);
				}
				game.remove(i);
			}
		}
	}

	// ATTACK: looks for 3 in row with open ends (i.e. guaranteed win)
	// 	waits till the above defences have been checked to avoid gifting win to opp by 
	// 	trying to set up our own attack
	for(int i=1;i<=game.noCols();++i){
		if(col_weights[i-1]>0){
			game.insert(i,player);		// inserts a move for AI
			// if its a trap then return that move
			if(is_trap(game, player)){
				game.remove(i);
				delete[] col_weights;
				return i;
			}
			game.remove(i);
		}
	}

	// likewise both of these are recursive
	if(alt_player==0){	
		// ATTACK: try to force opp into setting up a win
		for(int i=1;i<=game.noCols();++i){
			if(col_weights[i-1]>0){
				game.insert(i,player);		// tries all our positive moves
				int oppmove=choose_move(game, opp);	// sees what opp would play
				if(oppmove!=-1){
					game.insert(oppmove,opp);
					for(int j=1;j<=game.noCols();++j){
						if(game.is_winning_move(j,player)){	// sees if we can win
							game.remove(oppmove);
							game.remove(i);
							delete[] col_weights;
							return i;
						}
					}
					game.remove(oppmove);
				}
				game.remove(i);
			}
		}	
	
		// ATTACK: try to force opp into allowing us to set a trap
		for(int i=1;i<=game.noCols();++i){
			if(col_weights[i-1]>0){
				game.insert(i,player);		// tries all our positive moves
				int oppmove=choose_move(game, opp);
				if(oppmove!=-1){
					game.insert(oppmove,opp);	// seew what opp would play
					int ourmove=choose_move(game, player);
					if(ourmove!=-1){
						game.insert(ourmove,player);
						if(is_trap(game, player)){	// sees if we can set a trap
							game.remove(ourmove);
							game.remove(oppmove);
							game.remove(i);
							delete[] col_weights;
							return i;
						}
						game.remove(ourmove);
					}
					game.remove(oppmove);
				}
				game.remove(i);
			}
		}	
	}


	// if no obvious move has been played:
	// then return a random move from positive weight moves by default 
	if(alt_player==0){
		int total_weight=0;
		for(int i=0;i<game.noCols();i++){
			if(col_weights[i]>0){
				total_weight+=col_weights[i];
			}
		}

		// randomly plays a move based on those moves
		double rand=drand48()*total_weight;
		double total=0;
		for(int i=1;i<=game.noCols();++i){
			if(col_weights[i-1]>0){
				total+=col_weights[i-1];
			}
			if(rand<total){
				delete[] col_weights;
				return i;
			}
		}
	
		// if all the moves are bad to play then it chooses the least bad move
		if(total_weight==0){
			int max=-101;
			int move=1;
			for(int i=1;i<=game.noCols();++i){
				if(col_weights[i-1]>max && col_weights[i-1]<0){
					max=col_weights[i-1];
					move=i;
				}
			}
			delete[] col_weights;
			return move;
				
		}
	}
	// else return -1 to indicate that player has no definite move
	else{delete[] col_weights;return -1;}
}

// Function from virtual class calls choose_move function
// 	needed a second one as choose_move has an extra arg
int AIBasic::move(C4board& game){
	return choose_move(game);
}


// Constructor for ABPrune class
ABPrune::ABPrune(int p):AI_player{p}{}


// attempt at alpha-beta pruning
// 	turned out to be too slow
// 	Adapted from http://blog.gamesolver.org/solving-connect-four/04-alphabeta/
// Returns a score for the current player based on how many moves they will have left when they win
// 	negative score if they will lose
// Only looks for a score in the window of alpha and beta, if the score is above or below either at any point then we prune the tree and return that score
int ABPrune::abprune(C4board& game, int alpha, int beta, int nextp){
	// checks if we've reached a draw
	if(game.noMoves()==42){return 0;}
	
	// if the game is won by the next player then returns the negative of how many moves the player has left 
	for(int i=1;i<=game.noCols();++i){
		if(game.is_winning_move(i,nextp)){
			return -(42-game.noMoves()+1)/2;
		}
	}

	// sets the max possible score
	int max=(42-game.noMoves()-1)/2;
	if(beta>max){beta=max;}		// if beta is bigger than the max then narrow the window by setting beta to this max
	if(alpha>=beta){return beta;}	// if the window has reversed itself then return beta

	// searches for most likely best columns first (i.e. centre columns) - more efficient
	int columns[7]={4,3,5,2,6,1,7};
	int col;
	// loops through the columns
	for(int i=0;i<game.noCols();++i){
		col=columns[i];		// sets the columns based on next best column


		if(game.legal(col)){
			// inserts for the other player
			game.insert(col,nextp);
			// calculates their score based on the inverse of the window
			int oppscore=abprune(game, -beta, -alpha, (nextp%2)+1);

			// updates beta and alpha based on this (-oppscore = myscore)
			if(-oppscore>beta){
				return -oppscore;	// returns if above window
			}
			if(-oppscore>alpha){alpha=-oppscore;}	// updates alpha
			game.remove(col);
		}
	}
	return alpha;		// returns alpha as a lower limit if it didn't return above
}

// ABPrune's move function
int ABPrune::move(C4board& game){
	// initilises alpha and beta outside the possible max and min scores
	int beta=50;
	int alpha=-beta;

	std::cout<<"AI Player "<<AI_player<<" is thinking\n";
	int max=-100;
	int pos=0;

	// if the AI can win it plays it
	for(int i=1;i<=game.noCols();i++){
		if(game.is_winning_move(i,AI_player)){
			return i;
		}
	}
	// or if it can block a win
	for(int i=1;i<=game.noCols();i++){
		if(game.is_winning_move(i,(AI_player%2)+1)){
			return i;
		}
	}

	// otherwise it tries to get scores for each column
	int columns[7]={4,3,5,2,6,1,7};
	int col;
	// tries the columns again from inside out
	for(int i=0;i<game.noCols();i++){
		col=columns[i];
		if(game.legal(col)){
			// adds in the new move by the player
			game.insert(col,AI_player);
			// gets the ab score
			int a=abprune(game,alpha,beta, (AI_player%2)+1);
			
			if(a>beta){return col;}		// if the score is bigger than the max it returns that score
			// otherwise updates alpha
			if(a>alpha){alpha=a;}
			game.remove(col);	// undoes the move
			if(a>max){max=a;pos=col;}	// updates the best possible move so far
		}
	}
	return pos;	// returns the best move found
}
