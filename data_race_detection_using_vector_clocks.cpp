#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <string>
#include <memory>


// ----------------------------------------------------------------------------
// Classes for the instructions in the program
// Different Types of Instructions
// 1. Read
// 2. Write
// 3. Acquire Lock
// 4. Release Lock
// ----------------------------------------------------------------------------

class Instruction {
public:
    virtual ~Instruction() {}
    virtual int getThreadId() const = 0;
    virtual std::string getLocation() const = 0;
    virtual std::string toString() const = 0;
    virtual void print(std::ostream& os) const = 0;
};

class Read : public Instruction {
private:
    int thread_id;
    std::string location;

public:
    Read(int id, std::string loc) : thread_id(id), location(std::move(loc)) {}
    int getThreadId() const { return thread_id; }
    std::string getLocation() const override { return location; }

    std::string toString() const override {
        return "Read(" + std::to_string(thread_id) + ", " + location + ")";
    }

    void print(std::ostream& os) const override {
        os << "Read(" << thread_id << ", " << location << ")";
    }
};

class Write : public Instruction {
private:
    int thread_id;
    std::string location;

public:
    Write(int id, std::string loc) : thread_id(id), location(std::move(loc)) {}
    int getThreadId() const { return thread_id; }
    std::string getLocation() const override { return location; }

    std::string toString() const override {
        return "Write(" + std::to_string(thread_id) + ", " + location + ")";
    }

    void print(std::ostream& os) const override {
        os << "Write(" << thread_id << ", " << location << ")";
    }
};

class Acquire : public Instruction{
private:
    int thread_id;
    std::string lock;

public:
    Acquire(int id, std::string lockName) : thread_id(id), lock(std::move(lockName)) {}
    int getThreadId() const { return thread_id; }
    std::string getLock() const { return lock; }
    std::string getLocation() const override { return lock; }

    std::string toString() const override {
        return "Acquire(" + std::to_string(thread_id) + ", " + lock + ")";
    }

    void print(std::ostream& os) const override {
        os << "Acquire(" << thread_id << ", " << lock << ")";
    }
};

class Release : public Instruction {
private:
    int thread_id;
    std::string lock;

public:
    Release(int id, std::string lockName) : thread_id(id), lock(std::move(lockName)) {}
    int getThreadId() const { return thread_id; }
    std::string getLock() const { return lock; }
    std::string getLocation() const override { return lock; }

    std::string toString() const override {
        return "Release(" + std::to_string(thread_id) + ", " + lock + ")";
    }

    void print(std::ostream& os) const override {
        os << "Release(" << thread_id << ", " << lock << ")";
    }
};

class AtomicLoad : public Instruction{
private:
    int thread_id;
    std::string atomic_obj;

public:
    AtomicLoad(int id, std::string objName) : thread_id(id), atomic_obj(std::move(objName)) {}
    int getThreadId() const { return thread_id; }
    std::string getAtomicObj() const { return atomic_obj; }
    std::string getLocation() const override { return atomic_obj; }

    std::string toString() const override {
        return "AtomicLoad(" + std::to_string(thread_id) + ", " + atomic_obj + ")";
    }

    void print(std::ostream& os) const override {
        os << "AtomicLoad(" << thread_id << ", " << atomic_obj << ")";
    }
};

class AtomicStore : public Instruction{
private:
    int thread_id;
    std::string atomic_obj;

public:
    AtomicStore(int id, std::string objName) : thread_id(id), atomic_obj(std::move(objName)) {}
    int getThreadId() const { return thread_id; }
    std::string getAtomicObj() const { return atomic_obj; }
    std::string getLocation() const override { return atomic_obj; }

    std::string toString() const override {
        return "AtomicStore(" + std::to_string(thread_id) + ", " + atomic_obj + ")";
    }

    void print(std::ostream& os) const override {
        os << "AtomicStore(" << thread_id << ", " << atomic_obj << ")";
    }
};

class AtomicRMW : public Instruction {
private:
    int thread_id;
    std::string atomic_obj;

public:
    AtomicRMW(int id, std::string objName) : thread_id(id), atomic_obj(std::move(objName)) {}
    int getThreadId() const { return thread_id; }
    std::string getAtomicObj() const { return atomic_obj; }
    std::string getLocation() const override { return atomic_obj; }

    std::string toString() const override {
        return "AtomicRMW(" + std::to_string(thread_id) + ", " + atomic_obj + ")";
    }

    void print(std::ostream& os) const override {
        os << "AtomicRMW(" << thread_id << ", " << atomic_obj << ")";
    }
};

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    instr.print(os);
    return os;
}


// End of Instructions



// ------------------------------ VectorClock -------------------------------
// VectorClock class for vector clocks
// Used in the implementation of the data race detector
// Stores the vector clock for each thread
// -------------------------------------------------------------------------



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


// End of VectorClock


// ------------------------------ VectorClockState -------------------------------
// VectorClockState class
// Stores the vector clocks for each thread
// -------------------------------------------------------------------------------



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


// End of VectorClockState


// ----------------------------- Initial VectorClockState -------------------------------

VectorClockState initialVectorClockState(int num_threads, const std::vector<std::string>& locks, 
                                         const std::vector<std::string>& atomic_objects, 
                                         const std::vector<std::string>& shared_locations) {
    std::vector<VectorClock> C(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        C[i] = VectorClock(num_threads);
        C[i].increment(i);
    }

    std::unordered_map<std::string, VectorClock> L;
    for (const auto& l : locks) {
        L[l] = VectorClock(num_threads);
    }
    for (const auto& ao : atomic_objects) {
        L[ao] = VectorClock(num_threads);
    }

    std::unordered_map<std::string, VectorClock> R, W;
    for (const auto& loc : shared_locations) {
        R[loc] = VectorClock(num_threads);
        W[loc] = VectorClock(num_threads);
    }

    return VectorClockState(C, L, R, W);
}


// End of Initial VectorClockState


// ----------------------------- Race -------------------------------
// Overload << operator for printing
// Different Types of Races
// 1. Read-Write Race
// 2. Write-Write Race
// 3. Read-Write-Write Race
// -------------------------------------------------------------------



class Race {
public:
    virtual ~Race() = default;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Race& race) {
    race.print(os);
    return os;
}

class ReadWriteRace : public Race {
private:
    int u;
    int t;
    std::string x;

public:
    ReadWriteRace(int u, int t, std::string x) : u(u), t(t), x(std::move(x)) {}
    void print(std::ostream& os) const override {
        os << "ReadWriteRace(" << u << ", " << t << ", " << x << ")";
    }
};

class WriteWriteRace : public Race {
private:
    int u;
    int t;
    std::string x;

public:
    WriteWriteRace(int u, int t, std::string x) : u(u), t(t), x(std::move(x)) {}
    void print(std::ostream& os) const override {
        os << "WriteWriteRace(" << u << ", " << t << ", " << x << ")";
    }
};

class WriteReadRace : public Race {
private:
    int u;
    int t;
    std::string x;

public:
    WriteReadRace(int u, int t, std::string x) : u(u), t(t), x(std::move(x)) {}
    void print(std::ostream& os) const override {
        os << "WriteReadRace(" << u << ", " << t << ", " << x << ")";
    }
};

int findRacyThread(const VectorClock& location_vec, const VectorClock& clock_vec) {
    for (size_t i = 0; i < location_vec.vector.size(); ++i) {
        if (location_vec.vector[i] > clock_vec.vector[i]) {
            return i;  // Returning the index of the racy thread
        }
    }

    // If no racy thread is found, assert or throw an exception
    throw std::runtime_error("Could not find racy thread");  // Use an exception to handle error
}


// End of Race


// ----------------------------- Run Algorithm -------------------------------


std::tuple<VectorClockState, std::unique_ptr<Race>> run(VectorClockState& state, const std::vector<std::shared_ptr<Instruction>>& program, bool verbose = false) {
    for (const auto& instr : program) {
        int t = instr->getThreadId();
        std::string x = instr->getLocation();

        if (auto read = dynamic_cast<Read*>(instr.get())) {
            if (!(state.getW(x) <= state.getC(t))) {
                int u = findRacyThread(state.getW(x), state.getC(t));
                auto race = std::make_unique<WriteReadRace>(u, t, x);
                if (verbose) {
                    std::cout << "!!! " << *race << " when executing " << read->toString() << " !!!" << std::endl;
                }
                return std::make_tuple(state, std::move(race));
            }
            state.updateR(x, t, state.getC(t)[t]);
        } else if (auto write = dynamic_cast<Write*>(instr.get())) {
            if (!(state.getW(x) <= state.getC(t))) {
                int u = findRacyThread(state.getW(x), state.getC(t));
                auto race = std::make_unique<WriteWriteRace>(u, t, x);
                if (verbose) {
                    std::cout << "!!! " << *race << " when executing " << write->toString() << " !!!" << std::endl;
                }
                return std::make_tuple(state, std::move(race));
            } else if (!(state.getR(x) <= state.getC(t))) {
                int u = findRacyThread(state.getR(x), state.getC(t));
                auto race = std::make_unique<ReadWriteRace>(u, t, x);
                if (verbose) {
                    std::cout << "!!! " << *race << " when executing " << write->toString() << " !!!" << std::endl;
                }
                return std::make_tuple(state, std::move(race));
            }
            state.updateW(x, t, state.getC(t)[t]);
        } else if (auto acquire = dynamic_cast<Acquire*>(instr.get())) {
            state.updateC(t, state.getC(t) + state.getL(acquire->getLock()));
        } else if (auto release = dynamic_cast<Release*>(instr.get())) {
            state.updateL(release->getLock(), state.getC(t));
            state.getC(t).increment(t);
        } else if (auto atomicStore = dynamic_cast<AtomicStore*>(instr.get())) {
            state.updateL(atomicStore->getAtomicObj(), state.getC(t));
            state.getC(t).increment(t);
        } else if (auto atomicLoad = dynamic_cast<AtomicLoad*>(instr.get())) {
            state.updateC(t, state.getC(t) + state.getL(atomicLoad->getAtomicObj()));
        } else if (auto atomicRMW = dynamic_cast<AtomicRMW*>(instr.get())) {
            auto D = state.getC(t) + state.getL(atomicRMW->getAtomicObj());
            state.updateL(atomicRMW->getAtomicObj(), D);
            state.updateC(t, D);
            state.getC(t).increment(t);
        } else {
            throw std::invalid_argument("Unknown instruction type");
        }

        if (verbose) {
            std::cout << *instr << " : " << state << std::endl;
        }
    }
    return std::make_tuple(state, nullptr);
}

void ReadWriteRaceExample() {
    int threads = 2;
    std::vector<std::string> locks;
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"x"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Read>(0, "x"),   // Thread 0 reads 'x'
        std::make_shared<Write>(1, "x")   // Thread 1 writes to 'x' simultaneously
    };

    std::cout << "----------------------Running ReadWriteRaceExample---------------------------------------" << std::endl;
    run(state, program, true);

    std::cout << "-------------------------End of ReadWriteRaceExample--------------------------" << std::endl;
    
}

void SolveReadWriteRaceExample() {
    int threads = 2;
    std::vector<std::string> locks = {"m"};
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"x"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Acquire>(0, "m"),
        std::make_shared<Read>(0, "x"),
        std::make_shared<Release>(0, "m"),
        std::make_shared<Acquire>(1, "m"),
        std::make_shared<Write>(1, "x"),
        std::make_shared<Release>(1, "m")
    };


    std::cout << "----------------------Running SolveReadWriteRaceExample---------------------------------------" << std::endl;
    run(state, program, true);

    std::cout << "-------------------------End of SolveReadWriteRaceExample--------------------------" << std::endl;
    
}

void WriteWriteRaceExample() {
    int threads = 2;
    std::vector<std::string> locks;
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"y"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Write>(0, "y"),  // Thread 0 writes to 'y'
        std::make_shared<Write>(1, "y")   // Thread 1 writes to 'y' simultaneously
    };

    std::cout << "----------------------Running WriteWriteRaceExample---------------------------------------" << std::endl;
    run(state, program, true);

    std::cout << "-------------------------End of WriteWriteRaceExample--------------------------" << std::endl;
    

}

void SolveWriteWriteRaceExample() {
    int threads = 2;
    std::vector<std::string> locks = {"n"};
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"y"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Acquire>(0, "n"),
        std::make_shared<Write>(0, "y"),
        std::make_shared<Release>(0, "n"),
        std::make_shared<Acquire>(1, "n"),
        std::make_shared<Write>(1, "y"),
        std::make_shared<Release>(1, "n")
    };


    std::cout << "----------------------Running SolveWriteWriteRaceExample---------------------------------------" << std::endl;
    run(state, program, true);

    std::cout << "-------------------------End of SolveWriteWriteRaceExample--------------------------" << std::endl;
    
}

void WriteReadRaceExample() {
    int threads = 2;
    std::vector<std::string> locks;
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"z"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Write>(0, "z"),  // Thread 0 writes to 'z'
        std::make_shared<Read>(1, "z")   // Thread 1 reads from 'z' simultaneously
    };

    std::cout << "----------------------Running WriteReadRaceExample---------------------------------------" << std::endl;
    run(state, program, true);

    std::cout << "-------------------------End of WriteReadRaceExample--------------------------" << std::endl; 
}

void SolveWriteReadRaceExample() {
    int threads = 2;
    std::vector<std::string> locks = {"p"};
    std::vector<std::string> atomic_objects;
    std::vector<std::string> shared_locations = {"z"};

    auto state = initialVectorClockState(threads, locks, atomic_objects, shared_locations);
    std::vector<std::shared_ptr<Instruction>> program = {
        std::make_shared<Acquire>(0, "p"),
        std::make_shared<Write>(0, "z"),
        std::make_shared<Release>(0, "p"),
        std::make_shared<Acquire>(1, "p"),
        std::make_shared<Read>(1, "z"),
        std::make_shared<Release>(1, "p")
    };
    std::cout << "----------------------Running SolveWriteReadRaceExample---------------------------------------" << std::endl;

    run(state, program, true);

    std::cout << "-------------------------End of SolveWriteReadRaceExample--------------------------" << std::endl;

}







int main() {

    ReadWriteRaceExample();
    SolveReadWriteRaceExample();
    WriteWriteRaceExample();
    SolveWriteWriteRaceExample();
    WriteReadRaceExample();
    SolveWriteReadRaceExample();

}
