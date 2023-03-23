#include <string>
#include <vector>

#include "CFormula.h"

class CInputParser {
    public:
        CInputParser();
        CInputParser(const std::string &directory);
        std::vector<CFormula> parseInstances();
        CFormula parseInstance(const std::string &instanceFile);

    private:
        void loadFileNamesFromDirectory(const std::string &directory);
        void parseSize(int &clausesNr, int &variablesNr, std::stringstream &line);
        CClause parseClause(std::stringstream &line);
        std::vector<int> parseWeights(std::stringstream &line);

        std::vector<std::string> instance_files;
};