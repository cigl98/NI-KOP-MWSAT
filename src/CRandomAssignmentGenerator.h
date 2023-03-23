#include <vector>
#include <random>


class CRandomAssignmentGenerator {
    public:
        static int generateRandomNumberInRangeFromZero(int max) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, max);
            return distrib(gen);
        }

    static double generateRandomRealNumberInRangeFromZero(double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distrib(0.0, max);
        return distrib(gen);
    }

        static std::vector<int> generateRandomAssignment(int size) {
            return generateRandomNumbersInRange(0, 1, size);
        }

    private:
        static std::vector<int> generateRandomNumbersInRange(int min, int max, int count) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(min, max);
            std::vector<int> generatedValues;

            for (int i = 0; i < count; ++i) {
                generatedValues.push_back(distrib(gen));
            }

            return generatedValues;
        }
};