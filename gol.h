//
// Created by Alexander Dickbauer on 12.12.2021.
//
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>

#ifndef GOL_GOL_H
#define GOL_GOL_H


class gol {
    private:
        std::string load;
        std::string save;
        int32_t generations = 1;
        bool measure = false;
        static void show_usage(std::string name);
        int32_t return_flag = 0;
        int32_t witdh;
        int32_t height;
        std::ifstream  infile;
        std::fstream  outfile;
        std::vector<std::vector<char>> board;
        void loadBoard();

    public:
        gol(int argc, char **argv);
        void setup();
        void computation();
        void finalization();
        int32_t getGenerations();
        bool getMeasure();
        void saveBoard();


};


#endif //GOL_GOL_H
