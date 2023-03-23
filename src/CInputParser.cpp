#include <filesystem>
#include <fstream>
#include <cmath>
#include <iostream>

#include "CInputParser.h"


CInputParser::CInputParser() = default;

CInputParser::CInputParser(const std::string &directory) {
    loadFileNamesFromDirectory(directory);
}

CFormula CInputParser::parseInstance(const std::string &instanceFile) {
    std::vector<CClause> clauses;
    std::vector<int> weights;
    int clausesNr, variablesNr;

    std::ifstream infile(instanceFile);
    std::string line;

    while (std::getline(infile, line)) {
        std::stringstream sstream(line);
        if (line.starts_with('c'))
            continue;

        if (line.starts_with('p')) {
            parseSize(clausesNr, variablesNr, sstream);
            continue;
        }

        if (line.starts_with('w')) {
            weights = parseWeights(sstream);
            continue;
        }

        clauses.push_back(parseClause(sstream));
    }

    return CFormula (clauses, weights, variablesNr, clausesNr);
}

std::vector<CFormula> CInputParser::parseInstances() {
    std::vector<CFormula> formulas;
    for (const std::string &instanceFile : instance_files) {
        formulas.push_back(parseInstance(instanceFile));
    }

    return formulas;
}

void CInputParser::loadFileNamesFromDirectory(const std::string &directory) {
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        instance_files.push_back(entry.path());
    }
}

void CInputParser::parseSize(int &clausesNr, int &variablesNr, std::stringstream &line) {
    std::string dummy;
    line >> dummy >> dummy;
    line >> variablesNr >> clausesNr;
}

CClause CInputParser::parseClause(std::stringstream &line) {
    int var1, var2, var3;
    line >> var1 >> var2 >> var3;

    CVariable cvar1(std::abs(var1), var1 < 0);
    CVariable cvar2(std::abs(var2), var2 < 0);
    CVariable cvar3(std::abs(var3), var3 < 0);

    return CClause(cvar1, cvar2, cvar3);
}

std::vector<int> CInputParser::parseWeights(std::stringstream &line) {
    char dummy;
    int n;
    std::vector<int> weights;

    line >> dummy >> n;
    while (n != 0) {
        weights.push_back(n);
        line >> n;
    }
    return weights;
}
