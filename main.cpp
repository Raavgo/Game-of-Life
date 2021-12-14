#include <iostream>
#include <string>
#include <vector>
#include "gol.h"
#include "Timing.h"


int main(int argc, char* argv[])
{
   Timing* timing = Timing::getInstance();
   gol game(argc, argv);

   if(game.getReturnFlag() != 0)
       return 1;

   if(game.getMeasure())
       timing->startSetup();

   game.setup();

    if(game.getMeasure())
        timing->stopSetup();

    if(game.getMeasure())
        timing->startComputation();

    if(game.getMeasure())
        timing->stopComputation();

    if(game.getMeasure())
        timing->startFinalization();

    game.saveBoard();

    if(game.getMeasure())
        timing->stopFinalization();

    if(game.getMeasure()) {
        std::string output = timing->getResults();
        std::cout << output<< std::endl;
    }

   return 0;
}


