#include "run.h"
#include "instructions.h"
#include "vectorclockstate.h"
#include "race.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <string>
#include <memory>

int findRacyThread(const VectorClock& location_vec, const VectorClock& clock_vec) {
    for (size_t i = 0; i < location_vec.vector.size(); ++i) {
        if (location_vec.vector[i] > clock_vec.vector[i]) {
            return i;  // Returning the index of the racy thread
        }
    }

    // If no racy thread is found, assert or throw an exception
    throw std::runtime_error("Could not find racy thread");  // Use an exception to handle error
}


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
            state.updateC(t, state.getC(t) + state.getL(acquire->getLocation()));
        } else if (auto release = dynamic_cast<Release*>(instr.get())) {
            state.updateL(release->getLocation(), state.getC(t));
            state.getC(t).increment(t);
        } else if (auto atomicStore = dynamic_cast<AtomicStore*>(instr.get())) {
            state.updateL(atomicStore->getLocation(), state.getC(t));
            state.getC(t).increment(t);
        } else if (auto atomicLoad = dynamic_cast<AtomicLoad*>(instr.get())) {
            state.updateC(t, state.getC(t) + state.getL(atomicLoad->getLocation()));
        } else if (auto atomicRMW = dynamic_cast<AtomicRMW*>(instr.get())) {
            auto D = state.getC(t) + state.getL(atomicRMW->getLocation());
            state.updateL(atomicRMW->getLocation(), D);
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
