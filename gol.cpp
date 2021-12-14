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
                save = argv[i+1];
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-g") || (arg == "--generations")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                generations =  atoi(argv[i+1]);
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-m") || (arg == "--measure")) {
            measure = true;
        }
    }
}

void gol::setup(){
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

    height = board.size() - 1;
    width = board[0].size() - 1;
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

bool gol::getMeasure() const {
    return measure;
}

int gol::getReturnFlag() const {
    return return_flag;
}
//Count alive neighbours, if the cell is at a border wrap around
int32_t gol::getAliveNeighbours(int m, int n) {
    int result = 0;
    //Neigbours most of the time
    if(m > 0 && m < height && n> 0 && n < width) {
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[m + 1][n + 1] == 'x';
        neighbours[2] = board[m + 1][n] == 'x';
        neighbours[3] = board[m + 1][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[m - 1][n - 1] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][n + 1] == 'x';
    }else

    //Special case m = 0 / n> 0 && n<width Top Row
    if(m == 0 && n> 0 && n<width){
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[m + 1][n + 1] == 'x';
        neighbours[2] = board[m + 1][n] == 'x';
        neighbours[3] = board[m + 1][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[height][n - 1] == 'x';
        neighbours[6] = board[height][n] == 'x';
        neighbours[7] = board[height][n + 1] == 'x';
    }else

    //Special case m = height / n> 0 && n<width Bottom Row
    if(m == height && n> 0 && n<width){
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[0][n + 1] == 'x';
        neighbours[2] = board[0][n] == 'x';
        neighbours[3] = board[0][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[m - 1][n - 1] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][n + 1] == 'x';
    }else

    //Special case m = m > 0 && m< height / n == 0 left Row
    if(m > 0 && m< height && n == 0){
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[m + 1][n + 1] == 'x';
        neighbours[2] = board[m + 1][n] == 'x';
        neighbours[3] = board[m + 1][width] == 'x';
        neighbours[4] = board[m][width] == 'x';
        neighbours[5] = board[m - 1][width] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][n + 1] == 'x';
    }else

    //Special case m = m > 0 && m< height / n == width right Row
    if(m > 0 && m< height && n == width){
        neighbours[0] = board[m][0] == 'x';
        neighbours[1] = board[m + 1][0] == 'x';
        neighbours[2] = board[m + 1][n] == 'x';
        neighbours[3] = board[m + 1][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[m - 1][n - 1] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][0] == 'x';
    }else

    //Super special case 0/0
    if(m == 0 && n == 0){
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[m+1][n + 1] == 'x';
        neighbours[2] = board[m+1][n] == 'x';
        neighbours[3] = board[m+1][width] == 'x';
        neighbours[4] = board[m][width] == 'x';
        neighbours[5] = board[height][width] == 'x';
        neighbours[6] = board[height][n] == 'x';
        neighbours[7] = board[height][n + 1] == 'x';
    }else

    //Super special case 0/n
    if(m == 0 && n == width){
        neighbours[0] = board[m][0] == 'x';
        neighbours[1] = board[m + 1][0] == 'x';
        neighbours[2] = board[m + 1][n] == 'x';
        neighbours[3] = board[m + 1][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[height][n - 1] == 'x';
        neighbours[6] = board[height][n] == 'x';
        neighbours[7] = board[height][0] == 'x';
    }else

    //Super special case left lower corner m/0
    if(m == height && n == 0){
        neighbours[0] = board[m][n + 1] == 'x';
        neighbours[1] = board[0][n + 1] == 'x';
        neighbours[2] = board[0][n] == 'x';
        neighbours[3] = board[0][width] == 'x';
        neighbours[4] = board[m][width] == 'x';
        neighbours[5] = board[m - 1][width] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][n + 1] == 'x';
    }else

    //Super special case right lower corner m/n
    if(m == height && n == width){
        neighbours[0] = board[m][0] == 'x';
        neighbours[1] = board[0][0] == 'x';
        neighbours[2] = board[0][n] == 'x';
        neighbours[3] = board[0][n - 1] == 'x';
        neighbours[4] = board[m][n - 1] == 'x';
        neighbours[5] = board[m - 1][n - 1] == 'x';
        neighbours[6] = board[m - 1][n] == 'x';
        neighbours[7] = board[m - 1][0] == 'x';
    }

    for (bool neighbour : neighbours){
        if(neighbour)
            result++;
    }

    return result;
}

