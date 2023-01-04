#include "ResonanceType.h"
#include <iostream>

ResonanceType::ResonanceType(const std::string name, const double mass,
                             const int charge, const double width)
    : ParticleType(name, mass, charge), width_(width) {}
double ResonanceType::GetWidth() const { return width_; }
void ResonanceType::Print() const {
  ParticleType::Print();
  std::cout << "Width: " << width_ << '\n';
}