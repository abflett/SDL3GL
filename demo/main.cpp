#include <iostream>

#include "Engine.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Starting Demo..."
              << "\n";

    ige::Engine engine;
    engine.Run();

    return EXIT_SUCCESS;
}