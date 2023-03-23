class CVariable {
    public:
        CVariable(int id, bool negated) : m_id(id), m_negated(negated) {}
        int getId() const { return m_id; }
        bool isNegated() const { return m_negated; }

        bool getValue() const {
            if (m_negated) {
                return !m_assignment;
            } else {
                return m_assignment;
            }
        }

        void setAssignment(bool assignment) {
            m_assignment = assignment;
        }

        void print(std::ostream &os) const {
            if(isNegated()) {
                os << -1 * m_id << " ";
            } else {
                os << m_id << " ";
            }
        }

    private:
        int m_id;
        bool m_assignment;
        bool m_negated;
};