// Coriolis.cpp : Defines the entry point for the application.
//

#include "App.h"
#include <cstdio>

int main()
{
    try
    {
        App app;
        app.Run();
    }
    catch(const std::exception& e)
    {
        fprintf(stderr, "Error: %s\n", e.what());
        return -1;
    }

    return 0;
}
