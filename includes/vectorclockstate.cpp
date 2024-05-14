#include "vectorclock.h"
#include <unordered_map>
#include <string>
#include <iostream>

class VectorClockState {
private:
    std::vector<VectorClock> C;
    std::unordered_map<std::string, VectorClock> L, R, W;

public:
    // Constructor
    VectorClockState(const std::vector<VectorClock>& c, 
                     const std::unordered_map<std::string, VectorClock>& l,
                     const std::unordered_map<std::string, VectorClock>& r,
                     const std::unordered_map<std::string, VectorClock>& w)
        : C(c), L(l), R(r), W(w) {}

    // const VectorClock& getC(int index) const { return C[index]; }
    // const VectorClock& getL(const std::string& key) const { return L.at(key); }
    // const VectorClock& getR(const std::string& key) const { return R.at(key); }
    // const VectorClock& getW(const std::string& key) const { return W.at(key); }

    // Update a specific VectorClock in the vector C
    void updateC(int index, const VectorClock& newClock) {
        if (index >= 0 && index < C.size()) {
            C[index] = newClock;
        }
    }

    // Update a specific VectorClock in the map L
    void updateL(const std::string& key, const VectorClock& newClock) {
        L[key] = newClock;
    }

    // Update a specific entry in the map R
    void updateR(const std::string& key, int index, int value) {
        if (R.find(key) != R.end() && index >= 0 && index < R[key].vector.size()) {
            R[key].vector[index] = value;
        }
    }

    // Update a specific entry in the map W
    void updateW(const std::string& key, int index, int value) {
        if (W.find(key) != W.end() && index >= 0 && index < W[key].vector.size()) {
            W[key].vector[index] = value;
        }
    }

    // Accessor methods to get references (consider the safety of these operations)
    VectorClock& getC(int index) { return C.at(index); }
    const VectorClock& getL(const std::string& key) const { return L.at(key); }
    VectorClock& getR(const std::string& key) { return R[key]; }
    VectorClock& getW(const std::string& key) { return W[key]; }



    // Overload << operator for printing
    friend std::ostream& operator<<(std::ostream& os, const VectorClockState& vcs) {
        os << "\nC: ";
        for (const auto& vc : vcs.C) os << vc << ", ";
        os << "\nL: ";
        for (const auto& pair : vcs.L) os << "{" << pair.first << ": " << pair.second << "}, ";
        os << "\nR: ";
        for (const auto& pair : vcs.R) os << "{" << pair.first << ": " << pair.second << "}, ";
        os << "\nW: ";
        for (const auto& pair : vcs.W) os << "{" << pair.first << ": " << pair.second << "}";
        return os;
    }
};
