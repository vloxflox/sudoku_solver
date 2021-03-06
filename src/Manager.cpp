#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>


#include "Sudoku.h"
#include "Manager.h"

Manager::Manager(){}

void Manager::init_lists(){
	std::string line;
	std::string puzzle;
	std::string answer;

	std::ifstream file ("./input/sudoku.csv");
	
	while (file && getline(file, line)){

		puzzle = line.substr(0,81);
		answer = line.substr(82,81);

		puzzle_list.push_back(puzzle);
		answer_list.push_back(answer);

	}

}

void Manager::answer_test_list(){

	std::vector<int> not_matching_list;

	std::cout << "solve how many puzzles?" << std::endl;

	int puzzle_count;

	std::cin >> puzzle_count;

	if( puzzle_count > puzzle_list.size() ) puzzle_count = puzzle_list.size();

	clear_screen();


	for(int i = 0; i < puzzle_count ; i++){
		
		std::cout<< "solving " << i << std::endl;

		Sudoku sudoku( puzzle_list.at(i) );

		sudoku.solve();

		if(sudoku.get_answer() != answer_list.at(i))
			not_matching_list.push_back(i);
	}

	clear_screen();

	int unmatching_count = not_matching_list.size();

	std::cout<< "tested " << puzzle_count << " puzzles" << std::endl;
	if( unmatching_count == 0 ){
		std::cout << "every puzzle got the expected answer" << std::endl;
	}else{
		std::cout << unmatching_count << " puzzles showed unexpected answers at lines: ";
		for(int i = 0; i < unmatching_count; i++){
			std::cout << not_matching_list.at(i) << "," << std::endl;
		}
	}
}


void Manager::time_test_list(){

	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - begin;
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

	int puzzle_count;

	std::cout << "solve how many puzzles?" << std::endl;

	std::cin >> puzzle_count;

	if( puzzle_count > puzzle_list.size() ) puzzle_count = puzzle_list.size();

	clear_screen();


	for(int i = 0; i < puzzle_count ; i++){
		
		std::cout<< "solving " << i << std::endl;

		Sudoku *sudoku = new Sudoku( puzzle_list.at(i) );

		//inicializa a contagem em milissegundos para resolver o sudoku
		begin = std::chrono::high_resolution_clock::now();

		sudoku->solve();

		end = std::chrono::high_resolution_clock::now();    
    	dur = end - begin;
    	ms = std::chrono::duration_cast<std::chrono::microseconds >(dur).count();

    	times_list.push_back( (int)ms );

    	delete sudoku;

	}

}

void Manager::export_time_csv(){

	std::ofstream file ("./output/time_record.csv");

	for(int i = 0; i < times_list.size(); i++){
		file << times_list.at(i)<< std::endl;
	}

	std::cout << "Puzzles solved. Results stored at the \"output\" folder" << std::endl;
}

void Manager::solve_random(){
	srand (time(NULL));
	int selected = rand() % puzzle_list.size();

	Sudoku sudoku(puzzle_list.at(selected));
	sudoku.show_sudoku();

	auto begin = std::chrono::high_resolution_clock::now();

	sudoku.solve();

	auto end = std::chrono::high_resolution_clock::now();    
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::microseconds >(dur).count();

	sudoku.print_answer();
	bool is_matching = (sudoku.get_answer() == answer_list.at(selected) );
	char choice;


	std::cout << "Solving time : " << (int)ms << " microseconds " << std::endl;
	if(is_matching){
		std::cout << "The solution found matches the expected one" << std::endl;
	}else{
		std::cout << "The solution found doesn't match the expected one" << std::endl;
	}

}


void Manager::display(){
	clear_screen();
	std::cout << "Loading..." << std::endl;
	clear_screen();

	init_lists();


	int option;


	do{
		std::cout <<
		"1 - Solve a random puzzle from the list\n"<<
		"2 - Solve list and report time\n"<<
		"3 - Check if answers match the expected ones\n"<<
		"4 - Exit\n";

		std::cin >> option;
		clear_screen();

		switch(option){
			case 1:
				solve_random();
				break;
			case 2:
				time_test_list();
				clear_screen();
				export_time_csv();
				break;
			case 3:
				answer_test_list();
				break;
			default:
				return;
		}

		std::cout << "Press enter to continue" << std::endl;
		std::cin.ignore();
		std::getchar();
		clear_screen();


	}while(true);

}

void Manager::clear_screen(){
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
}