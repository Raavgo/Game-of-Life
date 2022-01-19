//
// Created by Alexander Dickbauer on 12.12.2021.
//

#include "gol.h"
#include <fstream>
#include <omp.h>
#include <vector>
#include <string>
#include <sstream>

int toInt(std::string s){
    int i;
    std::stringstream in_s(s);
    in_s >> i;

    return i;
}
std::vector<std::string> SplitString(
        std::string str,
        std::string delimeter)
{
    std::vector<std::string> splittedStrings = {};
    size_t pos = 0;

    while ((pos = str.find(delimeter)) != std::string::npos)
    {
        std::string token = str.substr(0, pos);
        if (token.length() > 0)
            splittedStrings.push_back(token);
        str.erase(0, pos + delimeter.length());
    }

    if (str.length() > 0)
        splittedStrings.push_back(str);
    return splittedStrings;
}

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
                std::cerr << "--load option requires one argument." << std::endl;
                return_flag = 1;
            }
        } else if ((arg == "-s") || (arg == "--save")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                save = argv[i+1];
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--save option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-g") || (arg == "--generations")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                generations =  atoi(argv[i+1]);
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--generations option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-me") || (arg == "--measure")) {
            measure = true;
        }else if ((arg == "-t") || (arg == "--threads")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                threads =  atoi(argv[i+1]);
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }else if ((arg == "-mo") || (arg == "--mode")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                std::string omp = "omp";
                std::string seq = "seq";

                if(omp.compare(argv[i+1]) == 0) {
                    threads = atoi(argv[i + 1]);
                    parralel_flag = true;
                }

                if(seq.compare(argv[i+1]) == 0){
                    parralel_flag = false;
                }
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--destination option requires one argument." << std::endl;
                return_flag = 1;
            }
        }
    }
}

void gol::setup(){
    std::string str;
    int i = -1;
    while (std::getline(infile, str)) {
        if(i == -1) { //Skip first line
            i = 0;
            width = toInt(SplitString(str, ",")[0]);
            height = toInt(SplitString(str, ",")[1]);
            continue;
        }
        std::vector<char> v(str.begin(), str.end());
        board.push_back(v);
    }
    infile.close();
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
    std::vector<std::vector<char>> ingrid = board;

    #pragma omp parallel num_threads(threads) shared(ingrid, board) if (parralel_flag)
    for (auto i = 0; i < generations; i++){
        #pragma omp for schedule(auto)
        for (auto t= 0; t < height*width; t++){

               int m = t / width;
               int n = t % width;

               int neigh = getAliveNeighbours(m, n);
               bool cellAlive = board[m][n] == 'x';
               char result;

                //If cell is dead
                if(!cellAlive){
                    //Rule 1
                    if(neigh == 3){
                        result = 'x';
                    }else{
                        result = '.';
                    }
                }

                //If cell is alive
                if(cellAlive){
                    //Rule 2
                    if(neigh == 2 || neigh == 3){
                        result = 'x';
                    }else
                        //Rule 3 and 4
                    if(neigh < 2 || neigh > 3){
                        result = '.';
                    }
                }

                ingrid[m][n] = result;
        }
        //After all cells where visited store the ingrid as the new board
        board = ingrid;
    }
}

void gol::finalization() {
    outfile.open(save, std::ios::out);
    outfile << width << "," << height << std::endl;

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
              << "\t-mo, --mode Mode\tSpecify the mode to run possible options (seq, omp)\n"
              << "\t-t, --threads Threads\tSpecify the amount of threads to run on\n"
              << "\t-l, --load FilePath\tSpecify the source path\n"
              << "\t-s, --save FilePath\tSpecify the destination path\n"
              << "\t-g, --generations Integer\tSpecify the generations for game of life (optional, default = 1)\n"
              << "\t-me, --measure\tSpecify if execution should be timed (optional, default=false)\n"
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

    size_t up =    m == 0? height-1  : m-1;
    size_t down =  m == height-1? 0  : m+1;
    size_t left =  n == 0 ? width-1  : n-1;
    size_t right = n == width-1 ? 0  : n+1;

    neighbours[0] = board [m][right] == 'x';
    neighbours[1] = board [up][right] == 'x';
    neighbours[2] = board [up][n] == 'x';
    neighbours[3] = board [up][left] == 'x';
    neighbours[4] = board [m][left] == 'x';
    neighbours[5] = board [down][left] == 'x';
    neighbours[6] = board [down][n] == 'x';
    neighbours[7] = board [down][right] == 'x';

    for (bool neighbour : neighbours){
        if(neighbour)
            result++;
    }

    return result;
}
