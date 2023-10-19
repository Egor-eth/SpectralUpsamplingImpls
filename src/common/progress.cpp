#include "progress.h"
#include <iostream>

void init_progress_bar() {
    std::cout << std::endl;
}

void finish_progress_bar() {

}

void print_progress(float p) 
{
    const int bar_length = 100;
    int progress_points = bar_length * p;
    std::cout << "\033[F[";
    std::cout << std::string(progress_points, '*') + std::string(bar_length - progress_points - 1, ' ');
    std::cout << "]\n";
}