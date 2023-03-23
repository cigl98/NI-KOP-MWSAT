#include <cmath>
#include <iostream>


#include "CSimulatedAnnealingSolver.h"
#include "CRandomAssignmentGenerator.h"

CSimulatedAnnealingSolver::CSimulatedAnnealingSolver(const CFormula &formula) : m_formula(formula) {}

CSimulatedAnnealingSolver::CSimulatedAnnealingSolver(const CFormula &formula, double initialTemperature,
                                                     int equilibriumValue, double coolingFactor)
                                                     : m_formula(formula), m_initialTemperature(initialTemperature),
                                                     m_equilibriumValue(equilibriumValue),
                                                     m_coolingFactor(coolingFactor),
                                                     m_currentTemperature(initialTemperature) {}

TState CSimulatedAnnealingSolver::tryNewState(const TState &state, double &acceptedStatesNr) {
    TState newState = generateRandomNeighbor(state);
    if (newState.betterThan(state)) {
        acceptedStatesNr++;
        return newState;
    }
    int d = newState.howMuchWorseThan(state);
    if (CRandomAssignmentGenerator::generateRandomRealNumberInRangeFromZero(1.0) < std::exp(-d / m_currentTemperature)) {
        acceptedStatesNr++;
        return newState;
    }
    return state;
}

 std::vector<int> CSimulatedAnnealingSolver::flipBitAtRandomPosition(const std::vector<int> &assignment) const {
     int pos = CRandomAssignmentGenerator::generateRandomNumberInRangeFromZero(static_cast<int>(assignment.size() - 1));
    std::vector<int> newAssignment = assignment;
    newAssignment[pos] == 1 ? newAssignment[pos] = 0 : newAssignment[pos] = 1;
    return newAssignment;
}

std::vector<int> CSimulatedAnnealingSolver::flipBitAtPosition(const std::vector<int> &assignment, int pos) const {
    std::vector<int> newAssignment = assignment;
    newAssignment[pos] == 1 ? newAssignment[pos] = 0 : newAssignment[pos] = 1;
    return newAssignment;
}

TState CSimulatedAnnealingSolver::generateRandomInitialState() {
    int clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr;
    bool formulaSatisfied;
    std::vector<int> assignment;

    assignment = CRandomAssignmentGenerator::generateRandomAssignment(m_formula.getVariablesNr());
    formulaSatisfied = m_formula.evaluateAssignment(assignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
    weightsSum = m_formula.getWeightsSum();
    return TState(formulaSatisfied, clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr, assignment);
}

TState CSimulatedAnnealingSolver::generateZeroInitialState() {
    int clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr;
    bool formulaSatisfied;
    std::vector<int> assignment(m_formula.getVariablesNr(), 0);

    formulaSatisfied = m_formula.evaluateAssignment(assignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
    weightsSum = m_formula.getWeightsSum();
    return TState(formulaSatisfied, clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr, assignment);
}

TState CSimulatedAnnealingSolver::generateNeighborByClauses(const TState &state) {
    bool clauseWasChanged = false;
    std::vector<int> newAssignment;
    for (int i = 0; i < std::ceil(m_formula.getClausesNr() / 10); ++i) {
        int randomClauseIdx = CRandomAssignmentGenerator::generateRandomNumberInRangeFromZero(m_formula.getClausesNr() - 1);
        CClause randomClause = m_formula.getClauses()[randomClauseIdx];
        if (!randomClause.getValue()) {
            clauseWasChanged = true;
            std::vector<CVariable> variables = randomClause.getVariables();
            int randomVariableIdx = variables[CRandomAssignmentGenerator::generateRandomNumberInRangeFromZero(2)].getId();
            newAssignment = flipBitAtPosition(state.m_assignment, randomVariableIdx - 1);
            break;
        }
    }

    if (!clauseWasChanged) {
        newAssignment = flipBitAtRandomPosition(state.m_assignment);
    }

    int clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr;
    bool formulaSatisfied = m_formula.evaluateAssignment(newAssignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
    weightsSum = m_formula.getWeightsSum();
    return TState(formulaSatisfied, clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr, newAssignment);
}

TState CSimulatedAnnealingSolver::generateNeighborByVariables(const TState &state) {
    int bestClausesSatisfiedNr = -1;
    std::vector<int> bestAssignment;

    for (int i = 0; i < std::ceil(m_formula.getVariablesNr() / 10); ++i) {
        std::vector<int> newAssignment = flipBitAtRandomPosition(state.m_assignment);
        int clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr;
        bool formulaSatisfied = m_formula.evaluateAssignment(newAssignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
        if (clausesSatisfiedNr > bestClausesSatisfiedNr) {
            bestClausesSatisfiedNr = clausesSatisfiedNr;
            bestAssignment = newAssignment;
        }
    }

    int clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr;
    bool formulaSatisfied = m_formula.evaluateAssignment(bestAssignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
    weightsSum = m_formula.getWeightsSum();
    return TState(formulaSatisfied, clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr, bestAssignment);
}

TState CSimulatedAnnealingSolver::generateRandomNeighbor(const TState &state) {
    std::vector<int> newAssignment = flipBitAtRandomPosition(state.m_assignment);
    int clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr;
    bool formulaSatisfied = m_formula.evaluateAssignment(newAssignment, clausesSatisfiedNr, solutionWeightsSum, variablesNr, clausesNr);
    weightsSum = m_formula.getWeightsSum();
    return TState(formulaSatisfied, clausesSatisfiedNr, solutionWeightsSum, weightsSum, variablesNr, clausesNr, newAssignment);
}



double CSimulatedAnnealingSolver::cool(double currentTemperature) {
    return currentTemperature * m_coolingFactor;
}

TSolution CSimulatedAnnealingSolver::runSimulatedAnnealing() {
    TState currentState = generateRandomInitialState();
    TState best = currentState;
    std::vector<int> iterationCosts;
    iterationCosts.push_back(currentState.cost());
  
    int equilibrium = m_equilibriumValue;
    while (m_currentTemperature > 1) {
        double generatedStatesNr = 0, acceptedStatesNr = 0;
        while (equilibrium > 0) {
            currentState = tryNewState(currentState, acceptedStatesNr);
            generatedStatesNr++;
            if (currentState.betterThan(best)) {
                best = currentState;
            }
            equilibrium--;
        }

        iterationCosts.push_back(currentState.cost());
        equilibrium = m_equilibriumValue;

        m_currentTemperature = cool(m_currentTemperature);
        double acceptRatio = acceptedStatesNr / generatedStatesNr;


        if (acceptRatio <= 0.01) {
            break;
        }
    }

    TSolution solution = TSolution(best.m_formulaSatisfied, best.m_variablesNr, best.m_clausesNr,
                                   best.m_solutionWeightsSum, best.m_assignment, iterationCosts);
    return solution;
}
