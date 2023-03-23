#pragma once
#include <vector>
#include <ostream>
#include <numeric>
#include <cmath>

#include "CClause.h"

class CFormula {
    public:
        CFormula(const std::vector<CClause> &clauses, const std::vector<int> &weights, const int variablesNr, const int clausesNr)
        : m_clauses(clauses), m_weights(weights), m_variablesNr(variablesNr), m_clausesNr(clausesNr) {
            int max = m_weights[0];
            for (size_t i = 1; i < m_weights.size(); ++i) {
                if (m_weights[i] > max) {
                    max = m_weights[i];
                }
            }
            m_weightsSum = max;
        }

        std::vector<CClause> getClauses() { return m_clauses; }

        void print(std::ostream &os) const {
            os << "p mwcnf " << m_variablesNr << " " << m_clausesNr << std::endl;
            os << "w ";

            for (int m_weight : m_weights) {
                os << m_weight << " ";
            }
            os << 0 << std::endl;

            for (const CClause &clause : m_clauses) {
                clause.print(os);
            }
        }

        bool evaluateAssignment(const std::vector<int> &assignment, int &clausesSatisfiedNr, int &weightsSum, int &variablesNr, int &clausesNr) {
            variablesNr = m_variablesNr;
            clausesNr = m_clausesNr;
            weightsSum = countSolutionWeightsSum(assignment);
            bool res = true;
            int currentClausesSatisfiedNumber = 0;

            for(CClause &clause : m_clauses) {
                clause.setAssignment(assignment);
                if (clause.getValue()) {
                    currentClausesSatisfiedNumber +=1;
                } else {
                    res = false;
                }
            }

            clausesSatisfiedNr = currentClausesSatisfiedNumber;
            return res;
        }

        int getVariablesNr() const {
            return m_variablesNr;
        }

        int getClausesNr() const {
                return m_clausesNr;
            }

        int getWeightsSum() const {
            return m_weightsSum;
        }

private:
        int countSolutionWeightsSum(const std::vector<int> &assignment) const {
            int weightsSum = 0;
            for (int i = 0; i < m_variablesNr; ++i) {
                if (assignment[i] != 0) {
                    weightsSum += m_weights[i];
                }
            }

            return weightsSum;
        }

        std::vector<CClause> m_clauses;
        std::vector<int> m_weights;

        int m_variablesNr, m_clausesNr, m_weightsSum;
};