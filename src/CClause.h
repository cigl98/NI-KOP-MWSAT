#include <vector>

#include "CVariable.h"

class CClause {
    public:
        CClause(const CVariable &var1, const CVariable &var2, const CVariable &var3) : m_var1(var1), m_var2(var2), m_var3(var3) {}

        bool getValue() {
            return m_var1.getValue() || m_var2.getValue() || m_var3.getValue();
        }

        std::vector<CVariable> getVariables() {
            std::vector<CVariable> vars {m_var1, m_var2, m_var3};
            return vars;
        }

        void setAssignment(const std::vector<int> &assignment) {
            bool var1Assignment = static_cast<bool>(assignment[m_var1.getId() - 1]);
            bool var2Assignment = static_cast<bool>(assignment[m_var2.getId() - 1]);
            bool var3Assignment = static_cast<bool>(assignment[m_var3.getId() - 1]);
            m_var1.setAssignment(var1Assignment);
            m_var2.setAssignment(var2Assignment);
            m_var3.setAssignment(var3Assignment);
        }

        void print(std::ostream &os) const {
            m_var1.print(os);
            m_var2.print(os);
            m_var3.print(os);
            os << 0 << std::endl;
        }

    private:
        CVariable m_var1, m_var2, m_var3;
};