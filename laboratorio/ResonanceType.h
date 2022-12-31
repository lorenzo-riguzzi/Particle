#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H
#include "ParticleType.h"

class ResonanceType : public ParticleType {
 public:
  ResonanceType(std::string name, const double mass, const int charge,
                const double width);
  double GetWidth() const override;
  void Print() const override;

 private:
  const double width_;
};

#endif