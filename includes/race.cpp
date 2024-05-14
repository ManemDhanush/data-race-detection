#include <iostream>
#include <string>
#include "instructions.h"

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

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    instr.print(os);  // Call the virtual print method
    return os;
}