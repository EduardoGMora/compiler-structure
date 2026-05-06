#include "Regex.hpp"

Regex::Regex(Pattern* p) {
    pattern = p;
}

Regex::~Regex() {
    delete pattern;
    pattern = nullptr;
}

Pattern* Regex::getPattern() const {
    return pattern;
}