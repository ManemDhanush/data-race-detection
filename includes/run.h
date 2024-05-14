#ifndef RUN_H
#define RUN_H

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

std::tuple<VectorClockState, std::unique_ptr<Race>> run(VectorClockState& state, const std::vector<std::shared_ptr<Instruction>>& program, bool verbose);

// int initialVectorClockState(const VectorClock& location_vec, const VectorClock& clock_vec);

VectorClockState initialVectorClockState(int num_threads, const std::vector<std::string>& locks, 
                                         const std::vector<std::string>& atomic_objects, 
                                         const std::vector<std::string>& shared_locations);

#endif