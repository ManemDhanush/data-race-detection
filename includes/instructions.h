#include <iostream>
#include <string>

class Instruction {
public:
    virtual ~Instruction() {}  // Virtual destructor to ensure proper cleanup
    virtual int getThreadId() const = 0;
    virtual std::string getLocation() const = 0;
    virtual std::string toString() const = 0;  // For verbose output
    virtual void print(std::ostream& os) const = 0;
};


class Read : public Instruction {
private:
    int thread_id;
    std::string location;

public:
    Read(int id, std::string loc);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};

class Write : public Instruction {
    int thread_id;
    std::string location;
public:
    Write(int id, std::string loc);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};


class Acquire : public Instruction {
    int thread_id;
    std::string location;
public:
    Acquire(int id, std::string loc);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};


class Release : public Instruction {
    int thread_id;
    std::string location;
public:
    Release(int id, std::string loc);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};

class AtomicLoad : public Instruction{
private:
    int thread_id;
    std::string atomic_obj;
public:
    AtomicLoad(int id, std::string objName);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};

class AtomicStore : public Instruction{
private:
    int thread_id;
    std::string atomic_obj;
public:
    AtomicStore(int id, std::string objName);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};

class AtomicRMW : public Instruction{
private:
    int thread_id;
    std::string atomic_obj;
public:
    AtomicRMW(int id, std::string objName);
    int getThreadId() const override;
    std::string getLocation() const override;
    std::string toString() const override;
    void print(std::ostream& os) const override;
};

std::ostream& operator<<(std::ostream& os, const Instruction& instr);