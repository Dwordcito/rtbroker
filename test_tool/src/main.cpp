#include "rtBroker.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2) return -1;
    try
    {
        RTBroker broker;
        broker.getData(argv[1]);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
