#include "Particle.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>  //for RAND_MAX
#include <iostream>

std::vector<ParticleType*> Particle::particleType_;
int Particle::nParticleType_ = 0;

int Particle::FindParticle(std::string name) {
  auto it = std::find_if(
      particleType_.begin(), particleType_.end(),
      [=](ParticleType* particle) { return particle->GetName() == name; });

  int result = std::distance(particleType_.begin(), it);

  if (it != particleType_.end()) {
    return result;
  } else {
    std::cout << "\n ** PARTICLE " << name << "NOT FOUND ** \n";
    return -1;
  }
}

Particle::Particle(std::string name, double px, double py, double pz)
    : px_(px), py_(py), pz_(pz) {
  index_ = Particle::FindParticle(name);
}

int Particle::GetIndex() const { return index_; }

void Particle::AddParticle(std::string name, double mass, int charge,
                           double width) {
  ParticleType* new_particle;
  if (particleType_.size() > maxNumParticleType_) return;
  if (width > 0)
    new_particle = new ResonanceType{name, mass, charge, width};
  else
    new_particle = new ParticleType{name, mass, charge};

  particleType_.push_back(new_particle);
}

void Particle::SetIndex(int index) { index_ = index; }
void Particle::SetIndex(std::string name) { index_ = FindParticle(name); }
void Particle::PrintParticleTypes() {
  std::for_each(particleType_.begin(), particleType_.end(),
                [=](ParticleType* particle) { particle->Print(); });
}
void Particle::PrintParticle() const {
  std::cout << "Index: " << index_ << '\n';
  std::cout << "Name " << particleType_[index_]->GetName() << '\n';
  std::cout << "Px: " << px_ << '\n';
  std::cout << "Py: " << py_ << '\n';
  std::cout << "Pz: " << pz_ << '\n';
}

double Particle::GetPx() const { return px_; }
double Particle::GetPy() const { return py_; }
double Particle::GetPz() const { return pz_; }
double Particle::GetMass() const { return particleType_[index_]->GetMass(); }
std::string Particle::GetName() const {
  return particleType_[index_]->GetName();
}
int Particle::GetCharge() const { return particleType_[index_]->GetCharge(); }

void Particle::SetP(double px, double py, double pz) {
  px_ = px;
  py_ = py;
  pz_ = pz;
}

double Particle::GetEnergy() const {
  return std::sqrt(GetMass() * GetMass() +
                   (GetPx() * GetPx() + GetPy() * GetPy() + GetPz() * GetPz()));
}

double Particle::GetInvariantMass(Particle& p) const {
  /*double sumP = pow(GetPx() + p.GetPx(), 2) + pow(GetPy() + p.GetPy(), 2) +
                pow(GetPz() + p.GetPz(), 2);*/
  return std::sqrt(pow(GetEnergy() + p.GetEnergy(), 2) -
                   (pow(GetPx() + p.GetPx(), 2) + pow(GetPy() + p.GetPy(), 2) +
                    pow(GetPz() + p.GetPz(), 2)));
}

int Particle::Decay2body(Particle& dau1, Particle& dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (index_ > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += particleType_[index_]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(px_ * px_ + py_ * py_ + pz_ * pz_ + massMot * massMot);

  double bx = px_ / energy;
  double by = py_ / energy;
  double bz = pz_ / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz) {
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * px_ + by * py_ + bz * pz_;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  px_ += gamma2 * bp * bx + gamma * bx * energy;
  py_ += gamma2 * bp * by + gamma * by * energy;
  pz_ += gamma2 * bp * bz + gamma * bz * energy;
}

/*Particle GenerateParticle(double px, double py, double pz) {
  double x = gRandom->Rndm();

  std::string name;

  if (x <= 0.4) {
    name = "Pion+";
  } else if (x <= 0.8) {
    name = "Pion-";
  } else if (x <= 0.85) {
    name = "Kaon+";
  } else if (x <= 0.9) {
    name = "Kaon-";
  } else if (x <= 0.945) {
    name = "Proton+";
  } else if (x <= 0.99) {
    name = "Proton-";
  } else {
    name = "K*";
  }

  return Particle(name, px, py, pz);
}*/
