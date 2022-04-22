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

void MinimaxPlayer::utility(OthelloBoard* b,int& val)
{
		if(b->count_score('X') <= b->count_score('O'))
		{
			val = 1;
		}
		else
		{
			val = -1;
		}
}

//I wrote 2 versions of successor, one that just returned board states, 
//and one that returned board states and the rows and cols for each state.
void MinimaxPlayer::successor(OthelloBoard* b, vector<OthelloBoard*>& sub, char sign)
{
	for(int i = 0; i < b->get_num_cols(); i++)
	{
		for(int j = 0; j < b->get_num_rows(); j++)
		{
			if(b->is_legal_move(i, j, sign))
			{
				OthelloBoard* c = b;
				c->play_move(i, j, sign);
				sub.push_back(c);
			}
		}
	}
}


//This fuction will only ever be used to calulate the first branches of the tree so it doesn't need a symbol flag
void MinimaxPlayer::successor_with_dec(OthelloBoard* b, vector<OthelloBoard*>& sub, vector<vector<int>>& moves)
{
	for(int i = 0; i < b->get_num_cols(); i++)
	{
		for(int j = 0; j < b->get_num_rows(); j++)
		{
			if(b->is_legal_move(i, j, symbol))
			{
				OthelloBoard* c = b;
				c->play_move(i, j, symbol);
				c->display();
				sub.push_back(c);
				vector<int> a;
				a.push_back(i);
				a.push_back(j);
				moves.push_back(a);
			}
		}
	}
	std::cout << "returning dec\n";
}

void MinimaxPlayer::minimax_dec(OthelloBoard* b,int& col, int& row)
{
	vector<OthelloBoard*> sub;
	vector<vector<int>> moves;
	int tar = 0;
	int lead = 0;
	successor_with_dec(b, sub, moves);
	for(int i = 0; i < sub.size(); i++)
	{
		int j = min_val(sub[i]);
		std::cout << "Min played\n";
		if(j > lead)
		{
			lead = j;
			tar = i;
		}
	}
	col = moves[tar][0];
	row = moves[tar][1];
}

int MinimaxPlayer::max_val(OthelloBoard* b)
{
	int v = 0;
	if(!b->has_legal_moves_remaining('X') && !b->has_legal_moves_remaining('O'))
	{
		utility(b , v);
		return v;
	}
	vector<OthelloBoard*> sub; 
	successor(b, sub, 'X');
	for(int i = 0; i < sub.size(); i++)
	{
		v = std::max(v, min_val(sub[i]));
	}
	return v;
}

int MinimaxPlayer::min_val(OthelloBoard* b)
{
	int v = 0;
	if(!b->has_legal_moves_remaining('X') && !b->has_legal_moves_remaining('O'))
	{
		utility(b , v);
		return v;
	}
	vector<OthelloBoard*> sub; 
	successor(b, sub, 'O');
	for(int i = 0; i < sub.size(); i++)
	{
		v = std::max(v, min_val(sub[i]));
	}
	return v;

}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	minimax_dec(b, col, row);
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
