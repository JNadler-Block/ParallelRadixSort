/*
 * Copyright (C) 2018-2022 David C. Harrison. All rights reserved.
 */

#include <vector>
#include <functional>

/*
 * Parallel Radix Sort with support for Most Significant Digit sorting only.
 * Multithreaded using up to CORES cpu cores.
 */
class ParallelRadixSort {
public:
    /*
     * Perform an in-place Most Significant Digit Radix Sort on each list of
     * unsigned integers in LISTS using no more that CORES cpu cores.
     */
    void msd(
        std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, 
        const unsigned int cores);
};