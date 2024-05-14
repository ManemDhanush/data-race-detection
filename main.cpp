#include "includes/run.h"

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