#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H
#include <string>

class ParticleType {
 public:
  ParticleType(const std::string name, const double mass, const int charge);

  std::string GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual double GetWidth() const;
  virtual void Print() const;

 private:
  const std::string name_;
  const double mass_;
  const int charge_;
};

#endif