#define SUDOKU_LINES 9
#define CELL_COUNT 81

#include "Sudoku.h"
#include "Cell.h"
#include <iostream>
#include <string>



Sudoku::~Sudoku(){

	int rows = SUDOKU_LINES;
	int cols = SUDOKU_LINES;

	for(int r = rows; r < rows; r++){
		for(int c = cols; c < cols; c++){
			delete cell_matrix[r][c];
		}
	}


	for (int i = 0; i < rows; ++i)
    	delete [] cell_matrix[i];
    
    delete[] cell_matrix;
}


Sudoku::Sudoku(){}


void Sudoku::alocate_cell_matrix(){
	//initializes the matrix of pointers without assigning values to them
	int rows = SUDOKU_LINES;
	int cols = SUDOKU_LINES;

	this->cell_matrix = new Cell**[rows];
		for (int i = 0; i < rows; ++i)
			cell_matrix[i] = new Cell*[cols];
}


void Sudoku::initialize_values(std::string puzzle_input){
	//checks if string input is valid
	if(puzzle_input.size () != CELL_COUNT){
		std::cout << "puzzle input string must have 81 characters"<<std::endl;
	}

	//auxiliary values to convert from string structure to matrix structure
	int current_row;
	int current_col;

	for(int i = 0; i < CELL_COUNT; i++){
		//divides by 9 to get the line you're at
		current_row = i/SUDOKU_LINES;
		//modulum 9 gives the column you're at
		current_col = i%SUDOKU_LINES;

		//reads value from file and converts char to number
		//creates a pointer representing the cell initialized with the value
		cell_matrix[current_row][current_col] = new Cell(puzzle_input[i]-48);
	}
}

void Sudoku::set_adjacencies(){
	//sets which cells are adjacent (can't have same number)
	for(int row = 0;row < SUDOKU_LINES;row++){
		for(int col = 0;col < SUDOKU_LINES;col++){
			//adds all cells adjacent to a certain target Cell
			set_cell_rules(cell_matrix[row][col], row, col);
		}
	}
}

void Sudoku::set_cell_rules(Cell* target, int target_row, int target_col){
	//Add cells in the same row to the adjacency list
	for(int current_col = 0; current_col < SUDOKU_LINES; current_col++){
		if(current_col != target_col)
			//creates an adjacency to all the cells in the row (except by the target cell)
			target->create_edge(cell_matrix[target_row][current_col]);
	}

	//Add cells in the same collumn to the adjacency list
	for(int current_row = 0; current_row < SUDOKU_LINES; current_row++){
		if(current_row != target_row)
			//creates an adjacency to all the cells in the column (except by the target cell)
			target->create_edge(cell_matrix[current_row][target_col]);
	}
	
	//Block 0 1 or 2
	int current_block_row = target_row/3;
	int current_block_col = target_col/3;

	Cell* current_cell = new Cell();

	//"floors" the base row to get the firs cell of the block
	int base_row = current_block_row*3;
	int base_col = current_block_col*3;

	//row and col of the block
	int cell_block_row;
	int cell_block_col;

	//Traverse the rows of the block
	for(int inc_row = 0; inc_row < 3; inc_row++){
		//update value of the row
		cell_block_row = base_row + inc_row;

		//Traverse the columns of the block
		for(int inc_col = 0; inc_col < 3; inc_col++){
			//update value of the column
			cell_block_col = base_col + inc_col;
			//choses a new cell of the block
			current_cell = cell_matrix[cell_block_row][cell_block_col];

			//Check if the cell was already adjacent, if it wasn't makes it adjacent
			if(current_cell != target && !target->is_adjacent_to(current_cell)){
				target->create_edge(current_cell);
			}
		}
	}
}


void Sudoku::show_sudoku(){
	std::cout<<"Puzzle propsed: "<<std::endl;
	for(int i = 0; i<9; i++){
		for(int j=0;j<9;j++){
			std::cout<<cell_matrix[i][j]->get_value()<<" ";
		}
		std::cout<<std::endl;
	}
}

Sudoku::Sudoku(std::string puzzle_input){
	alocate_cell_matrix();
	initialize_values(puzzle_input);
	set_adjacencies();	
}

void Sudoku::solve(){
	test_cell(0);
}

Cell* Sudoku::index_to_cell(int current_index){
	//divides by 9 to get the line you're at
	int current_row = current_index/SUDOKU_LINES;
	//modulum 9 gives the column you're at
	int current_col = current_index%SUDOKU_LINES;

	//returns a pointer to the corresponding cell
	return cell_matrix[current_row][current_col];
}

void Sudoku::print_answer(){
	if(answer.size()!=81){
		std::cout<<"No answer found!"<<std::endl;
		return;
	}

	std::cout<<"Answer to the puzzle:"<<std::endl;

	for(int i = 0;i < 81; i++){
		if(i%9==0 && i != 0)std::cout<<std::endl;
		std::cout<<answer.at(i)<<" ";
	}
	std::cout<<std::endl;
}

bool Sudoku::test_cell(int current_index){
	//Stop condition
	if(current_index == 81){return true;}
	//Gets from index the cell to deal with
	Cell* current_cell = index_to_cell(current_index);
	//Checks if it is a tip (value can't be changed)
	if(current_cell->get_is_tip()){
		//Skips changing the values of tips
		if(test_cell(current_index + 1)){
			//Put the correct number in the front of the answer stack
			answer.insert(answer.begin(), current_cell->get_value());
			//tells the previous cell to do the same
			return true;
		}else{
			//The only difference is that tips don't change value after failing
			//they instantly backtrack if the next index find no answer
			return false;
		}

	}else{
		//Tries to find a legal number, if it finds, test next index
		//If it can't find any legal value it backtracks
		for(int guess_value = 1; guess_value < 10; guess_value++){
			//Changes current value searching for an answer
			current_cell->set_value(guess_value);
			//If the new value is valid, try to find an answer for the next index
			if( current_cell->is_legal() ){
				//tests recursively new answers if it finds, add it to the answer list
				if(test_cell(current_index + 1)){
					//Put the correct (current) guess in the front of the answer stack
					answer.insert(answer.begin(), guess_value);
					//tells the previous cell to do the same
					return true;
				}
				//otherwise just change the current number and try again
			}
			//number tried was not legal, or derivated no legal answer, try another number
			//If no number gave a valid answer, backtrack.
		}

		//Backtracking

		//Sets value back to zero in order to allow previous cells to try new values
		current_cell->set_value(0);

		//If the first node tried all numbers and were false, then there is no answer
		if(current_index == 0) std::cout << "no possible answer for this puzzle" << std::endl;
		//Signals previous node to change value or backtrack
		return false;
	}
	
}

std::string Sudoku::get_answer(){
	std::string str_answer;

	for(int i = 0; i < answer.size(); i++){
		str_answer += (char)(answer.at(i) + 48);
	}

	return str_answer;
}