#include "Player.h"



int main(){
	int seed=time(NULL);
	srand48(seed);
	C4board game;


	// Sets up the game by asking for types and names of players
	std::string input1, input2, p1name, p2name;
	std::cout<<"Welcome to Connect4\nIs Player 1 a (h)uman or (c)omputer? ";
	std::cin>>input1;
	while(input1!="h" && input1!="c" && input1!="human" && input1!="computer"){
		std::cout<<"ERROR: Please try again\nIs Player 1 a (h)uman or (c)omputer? ";
		std::cin>>input1;
	}
	std::cout<<"Is Player 2 a (h)uman or (c)omputer? ";
	std::cin>>input2;	
	while(input2!="h" && input2!="c" && input2!="human" && input2!="computer"){
		std::cout<<"ERROR: Please try again\nIs Player 2 a (h)uman or (c)omputer? ";
		std::cin>>input2;
	}

	// Gets names of humans and converts all inputs to single letters h or c	
	if(input1=="human" || input1=="h"){
		input1="h";
		std::cout<<"Please enter a name for Player 1: ";
		std::cin>>p1name;	
	}
	if(input2=="human" || input2=="h"){
		std::cout<<"Please enter a name for Player 2: ";
		std::cin>>p2name;	
		input2="h";
	}
	if(input1=="computer"){input1="c";}
	if(input2=="computer"){input2="c";}

		
	// human v human game
	if(input1=="h" && input2=="h"){
		Human A(p1name);
		Human B(p2name);
		for(int i=0;i<21;++i){
			game.print();
			game.insert_animate(A.move(game),1);
			if(game.is_winner(1,true)){break;}
			game.insert_animate(B.move(game),2);
			if(game.is_winner(2,true)){break;}
			if(i==20){std::cout<<"The match is a draw\n";}
		}
	}

	// human vs AI game
	else if(input1=="h" && input2=="c"){
		Human A(p1name);
		AIBasic B(2);
		for(int i=0;i<21;++i){
			game.print();
			game.insert_animate(A.move(game),1);
			if(game.is_winner(1,true)){break;}
			game.insert_animate(B.move(game),2);
			if(game.is_winner(2,true)){break;}
			if(i==20){std::cout<<"The match is a draw\n";}
		}
	}

	// AI vs Human game
	else if(input1=="c" && input2=="h"){
		AIBasic A(1);
		Human B(p2name);
		for(int i=0;i<21;++i){
			game.print();
			game.insert_animate(A.move(game),1);
			if(game.is_winner(1,true)){break;}
			game.insert_animate(B.move(game),2);
			if(game.is_winner(2,true)){break;}
			if(i==20){std::cout<<"The match is a draw\n";}
		}
	}

	// AI vs AI game
	else if(input1=="c" && input2=="c"){
		AIBasic A(1);
		AIBasic B(2);
		for(int i=0;i<21;++i){
			game.print();
			game.insert_animate(A.move(game),1);
			if(game.is_winner(1,true)){break;}
			game.insert_animate(B.move(game),2);
			if(game.is_winner(2,true)){break;}
			if(i==20){std::cout<<"The match is a draw\n";}
		}
	}
}
