#include <vector>

#include "CFormula.h"

struct TState {
    TState(bool formulaSatisfied, int clausesSatisfiedNr, int solutionWeightsSum, int weightsMax, int variablesNr, int clausesNr,
           const std::vector<int> &assignment) : m_formulaSatisfied(formulaSatisfied),
                                                 m_clausesSatisfiedNr(clausesSatisfiedNr), m_solutionWeightsSum(solutionWeightsSum),
                                                 m_weightsMax(weightsMax), m_variablesNr(variablesNr), m_clausesNr(clausesNr),
                                                 m_assignment(assignment) {}

    int cost() const {
        int x = (m_clausesNr - m_clausesSatisfiedNr) * (m_weightsMax);
        return m_solutionWeightsSum - x;
    }

    bool betterThan(const TState &state) const { return this->cost() > state.cost(); }
    int howMuchWorseThan(const TState &state) const { return state.cost() - this->cost(); }

    bool m_formulaSatisfied;
    int m_clausesSatisfiedNr;
    int m_solutionWeightsSum;
    int m_weightsMax;
    int m_variablesNr;
    int m_clausesNr;
    std::vector<int> m_assignment;
};

struct TSolution {
    bool m_formulaSatisfied;
    bool m_longPrint = false;
    int m_variablesNr;
    int m_clausesNr;
    int m_weightsSum;
    std::vector<int> m_assignment;
    std::vector<int> m_iteration_costs;

    TSolution(bool formulaSatisfied, int variablesNr, int clausesNr, int weightsSum,
              const std::vector<int> &assignment, const std::vector<int> &iterationCosts) : m_formulaSatisfied(
            formulaSatisfied), m_variablesNr(variablesNr), m_clausesNr(clausesNr), m_weightsSum(weightsSum),
                                                                                             m_assignment(assignment),
                                                                                             m_iteration_costs(
                                                                                                     iterationCosts) {}

    friend std::ostream & operator << (std::ostream &os, const TSolution &solution) {
        if (solution.m_longPrint) {
            os << "Variables: " << solution.m_variablesNr << " Clauses: " << solution.m_clausesNr << std::endl;
            os << "W: " << solution.m_weightsSum << std::endl;
            os << "F(Y): " << solution.m_formulaSatisfied << std::endl;
//            os << "=" << std::endl;
//            for (const int i : solution.m_iteration_costs) {
//                os << i << std::endl;
//            }
        } else {
            os << "wuf-" << solution.m_variablesNr << "-" << solution.m_clausesNr << " " << solution.m_weightsSum << " ";
            for (size_t i = 0; i < solution.m_assignment.size(); ++i) {
                if (solution.m_assignment[i] == 0) {
                    os << "-";
                }
                os << i + 1 << " ";
            }
            os << std::endl;
        }
        return os;
    }
};

class CSimulatedAnnealingSolver {
    public:
        CSimulatedAnnealingSolver(const CFormula &formula);
        CSimulatedAnnealingSolver(const CFormula &formula, double initialTemperature, int equilibriumValue,
                                  double coolingFactor);

        TSolution runSimulatedAnnealing();

    private:
        TState generateRandomInitialState();
        TState generateZeroInitialState();
        TState generateRandomNeighbor(const TState &state);
        TState generateNeighborByVariables(const TState &state);
        TState generateNeighborByClauses(const TState &state);
        TState tryNewState(const TState &state, double &acceptedStatesNr);
        std::vector<int> flipBitAtPosition(const std::vector<int> &assignment, int pos) const;
        std::vector<int> flipBitAtRandomPosition(const std::vector<int> &assignment) const;
        double cool(double currentTemperature);

        CFormula m_formula;
        double m_initialTemperature, m_currentTemperature, m_coolingFactor;
        int m_equilibriumValue;
};
