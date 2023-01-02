#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>

#include "ParticleType.h"
#include "ResonanceType.h"
// #include "TRandom.h"

class Particle {
 public:
  // Particle() = default;
  Particle(std::string name, double px = 0., double py = 0., double pz = 0.);
  static void AddParticle(std::string name, double mass, int charge,
                          double width = 0.);
  void SetIndex(int index);
  void SetIndex(std::string name);
  static void PrintParticleTypes();
  void PrintParticle() const;
  int GetIndex() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  std::string GetName() const;
  int GetCharge() const;
  void SetP(double px = 0., double py = 0., double pz = 0.);
  double GetEnergy() const;
  double GetInvariantMass(Particle const &p) const;
  int Decay2body(Particle &dau1, Particle &dau2) const;
  // Particle GenerateParticle(double px = 0, double py = 0, double pz = 0);

 private:
  static constexpr int maxNumParticleType_ = 10;
  static std::vector<ParticleType *> particleType_;
  static int nParticleType_;
  int index_;
  double px_;
  double py_;
  double pz_;
  static int FindParticle(std::string name);
  void Boost(double bx, double by, double bz);
};

#endif