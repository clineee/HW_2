/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
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

int MinimaxPlayer::utility(OthelloBoard* b)
{
	return b->count_score('O') - b->count_score('X');
}

//I wrote 2 versions of successor, one that just returned board states, 
//and one that returned board states and the rows and cols for each state.
vector<OthelloBoard> MinimaxPlayer::successor(OthelloBoard* b, char sign)
{
	vector<OthelloBoard> sub;
	for(int i = 0; i < b->get_num_cols(); i++)
	{
		for(int j = 0; j < b->get_num_rows(); j++)
		{
			if(b->is_legal_move(i, j, sign))
			{
				OthelloBoard c = *b;
				c.play_move(i, j, sign);
				sub.push_back(c);
			}
		}
	}
	return sub;
}


//This fuction will only ever be used to calulate the first branches of the tree so it doesn't need a symbol flag
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

int MinimaxPlayer::max_val(OthelloBoard b)
{
	int v = -17;
	if(!b.has_legal_moves_remaining('X') && !b.has_legal_moves_remaining('O'))
	{
		v = utility(&b);

		//For debugging
		std::cout << "returned max term: " << v << '\n';

	}
	else if(!b.has_legal_moves_remaining('X'))
	{
		v = min_val(b);

		//For debugging
		std::cout << "returned min no turn: " << v << '\n';

	}
	vector<OthelloBoard> sub = successor(&b,'X');
	for(int i = 0; i < sub.size(); i++)
	{
		int g = min_val(sub[0]);

		//For debugging
		std::cout << "Max v: " << v << " g: " << g << '\n';

		v = std::max(v, g);
	}
	return v;
}

int MinimaxPlayer::min_val(OthelloBoard b)
{
	int v = 17;
	if(!b.has_legal_moves_remaining('X') && !b.has_legal_moves_remaining('O'))
	{
		v = utility(&b);

		//For debugging
		std::cout << "returned min term: " << v << '\n';

	}
	else if(!b.has_legal_moves_remaining('O'))
	{
		v = max_val(b);

		//For debugging
		std::cout << "returned max no turn: " << v << '\n';

	}
	vector<OthelloBoard> sub = successor(&b,'O');
	for(int i = 0; i < sub.size(); i++)
	{
		int g = max_val(sub[i]);

		//For debugging
		std::cout << "Min v: " << v << " g: " << g << '\n';
		
		v = std::min(v, g);
	}
	return v;
}

void MinimaxPlayer::minimax_dec(OthelloBoard* b, int& col, int& row)
{
	vector<vector<int>> moves;

	//For debugging
	vector <int> test;

	vector<OthelloBoard> sub = successor_with_dec(b, moves);
	int tar = 0;
	int lead = -18;
	int  v = -19;
	for(int i = 0; i < sub.size(); i++)
	{
		v = min_val(sub[i]);

		//For debugging
		test.push_back(v);

		lead = std::max(v, lead);
		if(lead == v)
		{
			tar = i;
		}
	}
	col = moves[tar][0];
	row = moves[tar][1];

	//For debugging
	std::cout << "\033[1;31mSuccessors: " << sub.size() << '\n';
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
	std::cout << "Final val: " << lead << "\033[0m\n";
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	minimax_dec(b, col, row);
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}