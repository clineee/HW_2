/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */

/*
Ethan Cline
04/25/2022

I left a bunch of commented out debugging code just because. 
It's all labeled as is so it shouldn't be to hard to sort out 
from my normal comments.

I also did some fixes that I found on the disscussion board for the 
main program so it wouldn't be a problem on valgrind
*/

#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"

using std::vector;

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

//My utility function evals on the difference of players number of points
//So if the human has more points then the function will return a negative
//value, otherwise it will return a positive value
int MinimaxPlayer::utility(OthelloBoard* b)
{
	//For debugging
	/*std::cout << "\033[1;35m";
	b->display();
	std::cout << "O score: " << b->count_score('O') << " X score: " << b->count_score('X') << "\033[0m\n";*/

	return b->count_score('O') - b->count_score('X');
}

//I wrote 2 versions of successor, one that just returned board states, 
//and one that returned board states and the rows and cols for each state.
//So this is the normal successor code that just returns board states.
vector<OthelloBoard> MinimaxPlayer::successor(OthelloBoard* b, char sign)
{
	vector<OthelloBoard> sub;
	
	//For debugging
	//std::cout << "\033[1;34mMid loop Successors[\n";

	for(int i = 0; i < b->get_num_cols(); i++)
	{
		for(int j = 0; j < b->get_num_rows(); j++)
		{
			if(b->is_legal_move(i, j, sign))
			{
				OthelloBoard c = *b;
				c.play_move(i, j, sign);	
				sub.push_back(c);

				//For debugging
				//c.display();
			}
		}
	}

	//For debugging
	//std::cout << "] End group.\033[0m\n";
	return sub;
}


//This fuction will only ever be used to calulate the first branches of the tree so it doesn't need a symbol flag
//This also returns the moves with the board states. I didn't want to track every move with the states, this program
//was mem hungry enough by itself 
vector<OthelloBoard> MinimaxPlayer::successor_with_dec(OthelloBoard* b, vector<vector<int>>& moves)
{
	vector<OthelloBoard> sub;
	for(int i = 0; i < b->get_num_cols(); i++)
	{
		for(int j = 0; j < b->get_num_rows(); j++)
		{
			if(b->is_legal_move(i, j, symbol))
			{
				OthelloBoard c = *b;
				c.play_move(i, j, symbol);
				sub.push_back(c);
				vector<int> a;
				a.push_back(i);
				a.push_back(j);
				moves.push_back(a);
			}
		}
	}
	return sub;
}

//This is a pretty standard tranlastion of the pseudocode, the only thing I would
//say is of note is the fact it checks for if the opponate gets multiple moves in
//a row, as that may be the only action that can be taken in the state
int MinimaxPlayer::max_val(OthelloBoard b)
{
	int v = -17; //lower than the min value, so I won't get any trash data
	if(!b.has_legal_moves_remaining('X') && !b.has_legal_moves_remaining('O'))
	{
		v = utility(&b);

		//For debugging
		//std::cout << "\033[1;35mreturned max term: " << v << "\033[0m\n";

	}
	else if(!b.has_legal_moves_remaining('X'))
	{
		v = min_val(b);

		//For debugging
		//std::cout << "returned min no turn: " << v << '\n';

	}
	else
	{
		vector<OthelloBoard> sub = successor(&b,'X');
		for(int i = 0; i < sub.size(); i++)
		{
			int g = min_val(sub[i]);

			//For debugging
			//std::cout << "Max v: " << v << " g: " << g << '\n';

			v = std::max(v, g);
		}
	}
	return v;
}

//Same as the max value, expect the mins and maxs are flipped
int MinimaxPlayer::min_val(OthelloBoard b)
{
	int v = 17; //higher than the max value, so I won't get any trash data
	if(!b.has_legal_moves_remaining('X') && !b.has_legal_moves_remaining('O'))
	{
		v = utility(&b);

		//For debugging
		//std::cout << "\033[1;35mreturned min term: " << v << "\033[0m\n";

	}
	else if(!b.has_legal_moves_remaining('O'))
	{
		v = max_val(b);

		//For debugging
		//std::cout << "returned max no turn: " << v << '\n';

	}
	else
	{
		vector<OthelloBoard> sub = successor(&b,'O');
		for(int i = 0; i < sub.size(); i++)
		{
			int g = max_val(sub[i]);

			//For debugging
			//std::cout << "Min v: " << v << " g: " << g << '\n';
			
			v = std::min(v, g);
		}
	}
	return v;
}

//So there is a little bit of funkyness going on here, but it does work as
//intended. I made it so that we just track the rows and coloumns of the first
//layer, so that made this function basically the first min call that is in the
//minimax dec function. That then made it so it would chose worse choices in the
//late game, so I had to put a limiter on it so it would never chose a value
//that would make it lose. It basically always choses to win by as a close margin
//aas it can though. Appearantly I made a program that likes to mess with people
void MinimaxPlayer::minimax_dec(OthelloBoard* b, int& col, int& row)
{
	vector<vector<int>> moves;

	//For debugging
	//vector <int> test;
	//vector<OthelloBoard> sub1 = successor(b, 'O');

	vector<OthelloBoard> sub = successor_with_dec(b, moves);
	int tar = 0;
	int  v = 18;
	for(int i = 0; i < sub.size(); i++)
	{
		int g = max_val(sub[i]);
		if(g <= 0 && v != 17)
		{
			g = 17;
		}
		//For debugging
		//test.push_back(g);

		v = std::min(v, g);
		if(v == g)
		{
			tar = i;
		}

		//For debugging
		//std::cout << "\033[1;31mbranch : " << i << " finished\033[0m\n";
	}
	col = moves[tar][0];
	row = moves[tar][1];
/*
	//For debugging
	std::cout << "\033[1;33mog board:\n";
	b->display();
	std::cout << "\033[0m\n";

	std::cout << "\033[1;34mSuccessors: " << sub1.size() << '\n';
	for(int i = 0; i < sub1.size(); i++)
	{
		sub1[i].display();
	}
	std::cout << "\033[0m\n";

	std::cout << "\033[1;31mSuccessors_with_dec: " << sub.size() << '\n';
	for(int i = 0; i < moves.size(); i++)
	{
		std::cout << "V value: " << test[i] << " cords: ";
		for(int h = 0; h < moves[i].size(); h++)
		{
			std::cout << moves[i][h] << ' ';
		}
		std::cout << '\n';
		sub[i].display();
	}
	std::cout << "Final val: " << v << "\033[0m\n";
	*/
}

//All this does is call the function.
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	minimax_dec(b, col, row);
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}