//
// Created by Alexander Dickbauer on 12.12.2021.
//

#include "gol.h"
#include <fstream>
#include <cstdio>
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
                infile.open(argv[i+1], std::ios::in);// = fopen(argv[i+1], "r");
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

}

void gol::finalization() {
    saveBoard();
}


void gol::show_usage(std::string name)
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
    for (unsigned int i = 0; i < board.size(); ++i)
    {
        for (unsigned int j = 0; j < board[i].size(); ++j)
        {
            outfile << board[i][j];
        }
        outfile << std::endl;
    }
    outfile.close();
}


int32_t gol::getGenerations() {
    return generations;
}

bool gol::getMeasure() {
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
}

