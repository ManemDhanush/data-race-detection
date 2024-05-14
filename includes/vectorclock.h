#include <vector>
#include <algorithm>
#include <iostream>

class VectorClock {
public:
    std::vector<int> vector;
    VectorClock();
    VectorClock(int num_threads);
    VectorClock(int num_threads, const std::vector<int>& values);

    VectorClock& increment(int index);
    int operator[](size_t index) const;
    int& operator[](size_t index);
    VectorClock operator+(const VectorClock& other) const;
    bool operator<=(const VectorClock& other) const;

    friend std::ostream& operator<<(std::ostream& os, const VectorClock& vc);
};