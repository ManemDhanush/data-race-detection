#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

class VectorClock {
private:

public:
    std::vector<int> vector;
    // Default constructor
    VectorClock() = default;

    // Constructor with number of threads
    VectorClock(int num_threads) : vector(num_threads, 0) {}

    // Constructor with number of threads and initial values
    VectorClock(int num_threads, const std::vector<int>& values) {
        if (values.empty()) {
            vector.resize(num_threads, 0);
        } else {
            assert(num_threads == values.size());
            vector = values;
        }
    }

    // Increment function
    VectorClock& increment(int index) {
        if (index >= 0 && index < vector.size()) {
            vector[index]++;
        }
        return *this;
    }

    // Overload [] operator for getting elements
    int operator[](size_t index) const {
        return vector[index];
    }

    // Overload [] operator for setting elements
    int& operator[](size_t index) {
        return vector[index];
    }

    // Overload + operator for joining two VectorClocks
    VectorClock operator+(const VectorClock& other) const {
        std::vector<int> joined_vec(vector.size());
        std::transform(vector.begin(), vector.end(), other.vector.begin(), joined_vec.begin(), [](int a, int b) { return std::max(a, b); });
        return VectorClock(static_cast<int>(joined_vec.size()), joined_vec);
    }

    // Overload <= operator to compare VectorClocks
    bool operator<=(const VectorClock& other) const {
        return std::equal(vector.begin(), vector.end(), other.vector.begin(), std::less_equal<>());
    }

    // Friend function for ostream to print VectorClock
    friend std::ostream& operator<<(std::ostream& os, const VectorClock& vc) {
        os << '[';
        for (size_t i = 0; i < vc.vector.size(); i++) {
            os << vc.vector[i];
            if (i < vc.vector.size() - 1) os << ", ";
        }
        os << ']';
        return os;
    }
};