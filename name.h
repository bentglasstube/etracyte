#pragma once

#include <random>
#include <vector>
#include <string>

class Name {
  public:

    Name(unsigned int seed) : rng_(seed) {}
    std::string generate();

  private:

    std::mt19937 rng_;

    char random_from_vector(const std::vector<char>& vector);

    std::string render(const std::string& s) const;
    std::string syllable();

    bool acceptable(const std::string& s) const;

    static const std::vector<char> kConsonants;
    static const std::vector<char> kVowels;
    static const std::vector<char> kSibilants;
};
