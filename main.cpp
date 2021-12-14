#include <iostream>
#include <string>
#include "gol.h"
#include "Timing.h"


int main(int argc, char* argv[])
{
    //Init Timing and GameOfLife Instance
    Timing* timing = Timing::getInstance();
    gol game(argc, argv);

    //If something went wrong while parsing exit
    if(game.getReturnFlag() != 0)
        return 1;

   //Start setup time measurement if the --measure flag is set
    if(game.getMeasure())
       timing->startSetup();

   //Load the file into a vector<vector<char>> and store width and height of it
    game.setup();

    //Stop setup time measurement if the --measure flag is set
    if(game.getMeasure())
        timing->stopSetup();

    //Start computation time measurement if the --measure flag is set
    if(game.getMeasure())
        timing->startComputation();

    //Iterate over all cells of the board, count the neighbours, and apply the gol rules
    game.computation();

    //Stop computation time measurement if the --measure flag is set
    if(game.getMeasure())
        timing->stopComputation();

    //Start finalization time measurement if the --measure flag is set
    if(game.getMeasure())
        timing->startFinalization();

    //Save the board to a file
    game.finalization();

    //Stop finalization time measurement if the --measure flag is set
    if(game.getMeasure())
        timing->stopFinalization();

    //Output the timing results if the --measure flag is set
    if(game.getMeasure()) {
        std::cout << timing->getResults() << std::endl;
    }

   return 0;
}


