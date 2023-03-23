#include <iostream>
#include <string>
#include <chrono>

#include "CInputParser.h"
#include "CSimulatedAnnealingSolver.h"

std::string getNextArgument(char **argv, int argc, int idx) {
    if (idx >= argc) {
        std::cerr << "Missing argument." << std::endl;
        return "";
    }
    return argv[++idx];
}

void parseArguments(int argc, char ** argv, std::string &file, int &tc, int &ec, double &cf, bool &l) {
    std::string arg;
    for (int i = 1; i < argc; ++i) {
        arg = argv[i];
        if (arg == "-f") {
            file = getNextArgument(argv, argc, i);
        }

        if (arg == "-tc") {
            tc = std::stoi(getNextArgument(argv, argc, i));
        }

        if (arg == "-ec") {
            ec = std::stoi(getNextArgument(argv, argc, i));
        }

        if (arg == "-cf") {
            cf = std::stod(getNextArgument(argv, argc, i));
        }

        if (arg == "-l") {
            l = true;
        }
    }
}

int main(int argc, char **argv) {
    std::string file;
    int t, e;
    double cf;
    bool l = false;

    if (argc == 1) {
        std::cerr << "Missing argument." << std::endl;
        return 1;
    }

    parseArguments(argc, argv, file, t, e, cf, l);

    CInputParser inputParser;
    CFormula formula = inputParser.parseInstance(file);
    CSimulatedAnnealingSolver solver(formula, t, e, cf);

    auto start = std::chrono::system_clock::now();
    TSolution solution = solver.runSimulatedAnnealing();
    solution.m_longPrint = l;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> totalTime = end - start;

    if (l) {
        std::cout << "Total time: " << totalTime.count() << std::endl;
    }

    std::cout << solution;
    return 0;
}
