//
// Created by Alexander Dickbauer on 12.12.2021.
//

#include "gol.h"
#include <fstream>
gol::gol(int argc, char **argv) {
    if (argc < 4) {
        show_usage(argv[0]);
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return_flag = 1;
        } else if ((arg == "-l") || (arg == "--load")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                load = argv[i+1]; // Increment 'i' so we don't get the argument as the next argv[i].
                infile.open(argv[i+1], std::ios::in);
                if (!infile) {
                    std::cerr << "No file found" << std::endl;
                    return_flag = 1;
                }
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        } else if ((arg == "-s") || (arg == "--save")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                save = argv[i+1]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-g") || (arg == "--generations")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                generations =  atoi(argv[i+1]); // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-m") || (arg == "--measure")) {
            measure = true;
        }
    }
    std::cout << load << "\n"
              << save << "\n"
              << generations << "\n"
              << measure << std:: endl;
}

void gol::setup(){
    loadBoard();
}

void gol::computation() {
    /**
    *  ‘x’ – live cell, ‘.’ – dead cell
    *
    * Rule 1: Any dead cell with exactly three living neighbours becomes a live cell
    * Rule 2: Any live cell with two or three living neighbours stay alive
    * Rule 3: Any live cell with fewer than two living neighbours dies
    * Rule 4: Any live cell with more than three living neighbours dies
    *
    * Consider: {0, 0} is a neighbour of {m, n} on a m × n sized grid
    */
    std::vector<std::vector<char>> computation_board = board;
    int neighbours;
    bool cellAlive;
    char result;

    for (auto i = 0; i < generations; i++){
        for (auto m= 0; m <= height; m++){
            for(auto n=0; n<= width; n++){
                neighbours = getAliveNeighbours(m, n);
                cellAlive = board[m][n] == 'x';

                //If cell is dead
                if(!cellAlive){
                    //Rule 1
                    if(neighbours == 3){
                        result = 'x';
                    }else{
                        result = '.';
                    }
                }

                //If cell is alive
                if(cellAlive){
                    //Rule 2
                    if(neighbours == 2 || neighbours == 3){
                        result = 'x';
                    }else
                        //Rule 3 and 4
                        if(neighbours < 2 || neighbours > 3){
                        result = '.';
                    }
                }

                computation_board[m][n] = result;

            }
        }
        //After all cells where visited store the computation_board as the new board
        board = computation_board;
    }
}

void gol::finalization() {
    saveBoard();
}


void gol::show_usage(const std::string& name)
{
    std::cerr << "Usage: " << name
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-l, --load FilePath\tSpecify the source path\n"
              << "\t-s, --save FilePath\tSpecify the destination path\n"
              << "\t-g, --generations Integer\tSpecify the generations for game of life (optional, default = 1)\n"
              << "\t-m, --measure\tSpecify if execution should be timed (optional, default=false)\n"
              << std::endl;
}

void gol::saveBoard(){
    std::cout << "Saving at "<< save << std::endl;
    outfile.open(save, std::ios::out);
    outfile << board.size() << "," << board[0].size() << std::endl;
    for (auto & i : board)
    {
        for (char j : i)
        {
            outfile << j;
        }
        outfile << std::endl;
    }
    outfile.close();
}

bool gol::getMeasure() const {
    return measure;
}

void gol::loadBoard() {
    std::string str;
    int i = 0;
    while (std::getline(infile, str)) {
        if(i == 0) { //Skip first line
            i = 1;
            continue;
        }
        std::vector<char> v(str.begin(), str.end());
        board.push_back(v);
    }
    infile.close();

    height = board.size();
    width = board[0].size();
}

int gol::getReturnFlag() const {
    return return_flag;
}

int32_t gol::getAliveNeighbours(int m, int n) {

    std::vector<bool> results;

    //Neigbours most of the time
    if(m > 0 && m< height && n> 0 && n<width) {
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Special case m = 0 / n> 0 && n<width Top Row <-- Todo
    if(m == 0 && n> 0 && n<width){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Special case m = height / n> 0 && n<width Bottom Row <-- Todo
    if(m == height && n> 0 && n<width){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Special case m = m > 0 && m< height / n == 0 left Row <-- Todo
    if(m > 0 && m< height && n == 0){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Special case m = m > 0 && m< height / n == width left Row <-- Todo
    if(m > 0 && m< height && n == width){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Super special case 0/0
    if(m == 0 && n == 0){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[height][n + 1] == 'x');
        results.push_back(board[height][n] == 'x');
        results.push_back(board[height][width] == 'x');
        results.push_back(board[m][width] == 'x');
        results.push_back(board[m - 1][width] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }
    //Super special case 0/n <-- TODO
    if(m == 0 && n == width){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Super special case m/0 <-- TODO
    if(m == height && n == 0){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }

    //Super special case m/n <-- TODO
    if(m == height && n == width){
        results.push_back(board[m][n + 1] == 'x');
        results.push_back(board[m + 1][n + 1] == 'x');
        results.push_back(board[m + 1][n] == 'x');
        results.push_back(board[m + 1][n - 1] == 'x');
        results.push_back(board[m][n - 1] == 'x');
        results.push_back(board[m - 1][n - 1] == 'x');
        results.push_back(board[m - 1][n] == 'x');
        results.push_back(board[m - 1][n + 1] == 'x');
    }


    return std::count(results.begin(), results.end(), true);
}

