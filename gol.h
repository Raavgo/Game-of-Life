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
        //Internal variables needed to compute the gol logic
        bool neighbours[8];
        std::string save;
        int32_t generations = 1;
        bool measure = false;
        int32_t return_flag = 0;
        size_t width{};
        size_t height{};
        std::ifstream infile;
        std::fstream outfile;
        std::vector<std::vector<char>> board;
        int threads = 1;
        bool parralel_flag;

    //Privat functions
        static void show_usage(const std::string& name);
        int32_t getAliveNeighbours(int m, int n);

    public:
        //Public functions needed for the main loop
        gol(int argc, char **argv);
        void setup();
        void computation();
        void finalization();

        [[nodiscard]] bool getMeasure() const;
        [[nodiscard]] int getReturnFlag() const;
};


#endif //GOL_GOL_H
