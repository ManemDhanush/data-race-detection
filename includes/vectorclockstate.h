#include "vectorclock.h"
#include <unordered_map>
#include <string>
#include <iostream>

class VectorClockState {
    std::vector<VectorClock> C;
    std::unordered_map<std::string, VectorClock> L, R, W;
public:
    // VectorClockState(int num_threads, const std::vector<std::string>& locks,
    //                  const std::vector<std::string>& atomic_objects,
    //                  const std::vector<std::string>& shared_locations);

    VectorClockState(const std::vector<VectorClock>& c, 
                     const std::unordered_map<std::string, VectorClock>& l,
                     const std::unordered_map<std::string, VectorClock>& r,
                     const std::unordered_map<std::string, VectorClock>& w) {}

    void updateC(int index, const VectorClock& newClock);
    void updateL(const std::string& key, const VectorClock& newClock);
    void updateR(const std::string& key, int index, int value);
    void updateW(const std::string& key, int index, int value);

    VectorClock& getC(int index);
    const VectorClock& getL(const std::string& key);
    VectorClock& getR(const std::string& key);
    VectorClock& getW(const std::string& key);

    friend std::ostream& operator<<(std::ostream& os, const VectorClockState& vcs);
};