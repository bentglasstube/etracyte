#include "name.h"

#include <algorithm>
#include <sstream>

std::string Name::generate() {
  std::uniform_real_distribution<double> n(0, 0.999);
  const double v = n(rng_);
  const size_t length = 4 + (size_t)(v * v * 6);

  while (true) {
    std::stringstream name;
    while (name.str().length() < length) {
      name << syllable();
    }

    const std::string result = name.str();
    if (acceptable(result)) return render(result);
  }
}

std::string Name::syllable() {
  std::uniform_real_distribution<double> r(0, 0.999);

  std::stringstream syl;

  const double p = r(rng_);

  if (p < 0.70) {
    syl << random_from_vector(kConsonants);
    syl << random_from_vector(kVowels);
    syl << random_from_vector(kConsonants);
  } else if (p < 0.85) {
    syl << random_from_vector(kVowels);
    syl << random_from_vector(kConsonants);
  } else {
    syl << random_from_vector(kConsonants);
    syl << random_from_vector(kVowels);
  }

  return syl.str();
}

bool Name::acceptable(const std::string& name) const {
  bool prev = name[0] < 'a';
  for (size_t i = 1; i < name.length(); ++i) {
    if (prev && name[i] < 'a') return false;
    prev = name[i] < 'a';
  }
  return true;
}

std::string Name::render(const std::string& s) const {
  std::stringstream o;

  for (size_t i = 0; i < s.length(); ++i) {
    switch (s[i]) {
      case 'A': o << "ai"; break;
      case 'C': o << "ch"; break;
      case 'E': o << "ei"; break;
      case 'I': o << "ie"; break;
      case 'N': o << "ng"; break;
      case 'O': o << "or"; break;
      case 'S': o << "sh"; break;
      case 'U': o << "ou"; break;
      default: o << s[i]; break;
    }
  }

  std::string result = o.str();

  if (result[0] == '\'') result = result.substr(1);
  if (result[result.length() - 1] == '\'') result = result.substr(0, result.length() - 1);

  result[0] = std::toupper(result[0]);
  return result;
}


char Name::random_from_vector(const std::vector<char>& vector) {
  std::uniform_real_distribution<double> r(0, 0.999);
  const double i = r(rng_);
  return vector[(int)(i * i * vector.size())];
}


const std::vector<char> Name::kConsonants({'s', 'r', 'n', 't', 'l', 'k', 'd', 'g', 'p', 'm', 'b', 'S', 'C', '\'', 'v', 'z', 'y'});
const std::vector<char> Name::kVowels({'e', 'a', 'i', 'o', 'u', 'I', 'U', 'A', 'E', 'O'});
