#include <iostream>
#include <string>

class Race {
public:
    virtual ~Race() {}
    virtual void print(std::ostream& os) const = 0;
};

class ReadWriteRace : public Race {
    int u, t;
    std::string x;
public:
    ReadWriteRace(int u, int t, std::string x);
    void print(std::ostream& os) const override;
};

class WriteWriteRace : public Race {
    int u, t;
    std::string x;
public:
    WriteWriteRace(int u, int t, std::string x);
    void print(std::ostream& os) const override;
};

class WriteReadRace : public Race {
    int u, t;
    std::string x;
public:
    WriteReadRace(int u, int t, std::string x);
    void print(std::ostream& os) const override;
};

std::ostream& operator<<(std::ostream& os, const Race& race);
