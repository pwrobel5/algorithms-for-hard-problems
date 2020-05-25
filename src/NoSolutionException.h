//
// Created by piotr on 24.05.2020.
//

#ifndef ALGORITHMS_FOR_HARD_PROBLEMS_NOSOLUTIONEXCEPTION_H
#define ALGORITHMS_FOR_HARD_PROBLEMS_NOSOLUTIONEXCEPTION_H

#include <exception>

class NoSolutionException : public std::exception {
public:
    const char * what () const noexcept override
    {
        return "No solution found";
    }
};


#endif //ALGORITHMS_FOR_HARD_PROBLEMS_NOSOLUTIONEXCEPTION_H
