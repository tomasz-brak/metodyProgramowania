#pragma once
#include <string>

void generateLCG(unsigned long long seed, int n, unsigned long long m,
                 unsigned long long a, unsigned long long c,
                 const std::string &filename);

void generateACG(unsigned long long seed, int n, const std::string &filename);

void calculateLCGParams(long long Xmax, unsigned long long &m,
                        unsigned long long &a, unsigned long long &c);
