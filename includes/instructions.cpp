#include <string>
#include <iostream>


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