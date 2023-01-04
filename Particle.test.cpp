#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.h"

#include <iostream>

#include "doctest.h"

TEST_CASE("Testing ParticleType and ResonanceType") {
  ParticleType p{"Pion+", 0.13957, 1};
  ResonanceType k{"K*", 0.89166, 0, 0.050};

  ParticleType* array[2]{&p, &k};

  std::cout << "Name Pion+: " << p.GetName() << '\n';
  std::cout << "Mass Pion+: " << p.GetMass() << '\n';
  std::cout << "Charge Pion+: " << p.GetCharge() << '\n';
  std::cout << '\n';
  std::cout << "Name K*: " << k.GetName() << '\n';
  std::cout << "Mass K*: " << k.GetMass() << '\n';
  std::cout << "Charge K*: " << k.GetCharge() << '\n';
  std::cout << "Width K*: " << k.GetWidth() << '\n';
  std::cout << '\n';
  std::cout << "Printing the array" << '\n';
  for (int i{0}; i < 2; ++i) {
    array[i]->Print();
  }
  std::cout << '\n';

  CHECK(p.GetName() == "Pion+");
  CHECK(p.GetMass() == 0.13957);
  CHECK(p.GetCharge() == 1);
  CHECK(k.GetName() == "K*");
  CHECK(k.GetMass() == 0.89166);
  CHECK(k.GetCharge() == 0);
  CHECK(k.GetWidth() == 0.050);
}

TEST_CASE("Testing Particle") {
  Particle::AddParticle("Pion+", 0.13957, 1);
  Particle::AddParticle("Pion-", 0.13957, -1);
  Particle::AddParticle("K*", 0.89166, 0, 0.050);
  Particle::AddParticle("Pion+", 0.13957, 1);

  std::cout << "Printing Particle Types" << '\n';
  Particle::PrintParticleTypes();
  std::cout << '\n';

  Particle p1;
  Particle p2;
  p1.SetIndex(0);
  p2.SetIndex("K*");

  CHECK(p1.GetIndex() == 0);
  CHECK(p1.GetName() == "Pion+");
  CHECK(p1.GetMass() == 0.13957);
  CHECK(p1.GetCharge() == 1);
  CHECK(p1.GetWidth() == 0);
  CHECK(p2.GetIndex() == 2);
  CHECK(p2.GetName() == "K*");
  CHECK(p2.GetMass() == 0.89166);
  CHECK(p2.GetCharge() == 0);
  CHECK(p2.GetWidth() == 0.050);

  p1.SetP(1, 2, 3);
  p2.SetP(4, 5, 6);

  std::cout << "Printing p1" << '\n';
  p1.PrintParticle();
  std::cout << '\n';
  std::cout << "Printing p2" << '\n';
  p2.PrintParticle();
  std::cout << '\n';

  CHECK(p1.GetPx() == 1);
  CHECK(p1.GetPy() == 2);
  CHECK(p1.GetPz() == 3);
  CHECK(p2.GetPx() == 4);
  CHECK(p2.GetPy() == 5);
  CHECK(p2.GetPz() == 6);

  CHECK(p1.GetEnergy() == doctest::Approx(3.744).epsilon(0.001));
  CHECK(p2.GetEnergy() == doctest::Approx(8.820).epsilon(0.001));
  CHECK(p1.GetInvariantMass(p2) == doctest::Approx(1.69).epsilon(0.01));
  CHECK(p2.GetInvariantMass(p1) == doctest::Approx(1.69).epsilon(0.01));
}