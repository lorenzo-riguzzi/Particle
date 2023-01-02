#include "ParticleType.h"
#include <iostream>

ParticleType::ParticleType(const std::string name, const double mass,
                           const int charge)
    : name_(name), mass_(mass), charge_(charge) {}

std::string ParticleType::GetName() const { return name_; }
double ParticleType::GetMass() const { return mass_; }
int ParticleType::GetCharge() const { return charge_; }
double ParticleType::GetWidth() const { return 0; }

void ParticleType::Print() const {
  std::cout << "Name: " << name_ << '\n';
  std::cout << "Mass: " << mass_ << '\n';
  std::cout << "Charge: " << charge_ << '\n';
}