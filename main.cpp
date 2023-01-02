#include <cmath>
#include <iostream>

#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"
#include "TFile.h"
#include "TH1F.h"
#include "TRandom.h"

int Main() {
  /*ParticleType e{"Elettrone", 1.2, -1};
  ResonanceType k{"Instable Particle", 2.3, 2, 3};

  ParticleType* array[2]{&e, &k};

  std::cout << "Nome elettrone: " << e.GetName() << '\n';
  std::cout << "Massa elettrone: " << e.GetMass() << '\n';
  std::cout << "Carica elettrone: " << e.GetCharge() << '\n';
  std::cout << '\n';
  std::cout << "Nome Particella instabile: " << k.GetName() << '\n';
  std::cout << "Massa Particella instabile: " << k.GetMass() << '\n';
  std::cout << "Carica Particella instabile: " << k.GetCharge() << '\n';
  std::cout << "Width Particella instabile: " << k.GetWidth() << '\n';
  std::cout << '\n';
  for(int i{0}; i < 2; ++i){
      array[i]->Print();
  }*/

  Particle::AddParticle("Pion+", 0.13957, 1);
  Particle::AddParticle("Pion-", 0.13957, -1);
  Particle::AddParticle("Kaon+", 0.49367, 1, 1);
  Particle::AddParticle("Kaon-", 0.49367, 1, -1);
  Particle::AddParticle("Proton+", 0.93827, 1);
  Particle::AddParticle("Proton-", 0.93827, -1);
  Particle::AddParticle("K*", 0.89166, 0, 0.050);

  gRandom->SetSeed();

  TH1F* particleDistribution = new TH1F("h1", "Particle Distribution", 7, 0, 7);
  TH1F* phiDistribution =
      new TH1F("h2", "Phi Angle Distribution", 1000, 0, 2 * M_PI);
  TH1F* thetaDistribution =
      new TH1F("h3", "Theta Angle Distribution", 1000, 0, M_PI);
  TH1F* impulseDistribution =
      new TH1F("h4", "Impulse Distribution", 1000, 0, 4);
  TH1F* traverseImpulseDistribution =
      new TH1F("h5", "Traverse Impulse Distribution", 1000, 0, 4);
  TH1F* energyDistribution = new TH1F("h6", "Energy Distribution", 1000, 0, 2);

  TH1F* totalInvMass = new TH1F("h7", "Total Invariant Mass ", 1000, 0, 4);
  TH1F* invMassOppositeCharge =
      new TH1F("h8", "Invariant Mass of opposite charge particles", 1000, 0, 4);
  TH1F* invMassSameCharge =
      new TH1F("h9", "Invariant Mass of same charge particles", 1000, 0, 4);
  TH1F* invMassKPOpposite = new TH1F(
      "h10", "Invariant Mass of Kaon/Pion with opposite charge", 1000, 0, 4);
  TH1F* invMassKPSame = new TH1F(
      "h11", "Invariant of Mass Kaon/Pion with same charge", 1000, 0, 4);
  TH1F* invMassDaughters =
      new TH1F("h12", "Invariant Mass of daughters of decay", 1000, 0, 2);

  invMassOppositeCharge->Sumw2();
  invMassSameCharge->Sumw2();
  invMassKPOpposite->Sumw2();
  invMassKPSame->Sumw2();

  std::vector<Particle> eventParticles;
  std::vector<Particle> daughters;
  for (size_t i = 0; i < 1E5; i++) {
    eventParticles.clear();

    for (size_t j = 0; j != 100; j++) {
      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double p_module = gRandom->Exp(1);
      double px = p_module * cos(phi) * sin(theta);
      double py = p_module * sin(phi) * sin(theta);
      double pz = p_module * cos(theta);
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

      Particle particle(name, px, py, pz);

      phiDistribution->Fill(phi);
      thetaDistribution->Fill(theta);
      impulseDistribution->Fill(p_module);
      traverseImpulseDistribution->Fill(p_module * sin(theta));
      energyDistribution->Fill(particle.GetEnergy());

      if (particle.GetName() == "K*") {
        double probability = gRandom->Rndm();

        Particle p = Particle("Pion+");
        Particle k = Particle("Kaon-");

        if (probability > 0.5) {
          p.SetIndex("Pion-");
          k.SetIndex("Kaon+");
        }

        eventParticles.push_back(particle);

        particle.Decay2body(p, k);

        eventParticles.push_back(p);
        eventParticles.push_back(k);

        invMassDaughters->Fill(p.GetInvariantMass(k));

        particleDistribution->Fill(particle.GetIndex());
      } else {
        particleDistribution->Fill(particle.GetIndex());
        eventParticles.push_back(particle);
      }
    }

    /*for (size_t j = 0; j < eventParticles.size() - 1; j++) {
      for (size_t k = j + 1; k < eventParticles.size(); k++) {
        double invMass = eventParticles[k].GetInvariantMass(eventParticles[j]);

        const auto first = eventParticles[k];
        const std::string first_type =
            first.GetName().substr(0, first.GetName().size() - 1);
        const auto second = eventParticles[j];
        const std::string second_type =
            second.GetName().substr(0, second.GetName().size() - 1);

        totalInvMass->Fill(invMass);

        if (first.GetCharge() * second.GetCharge() > 0) {
          invMassSameCharge->Fill(invMass);

          if ((first_type == "Pion" && second_type == "Kaon") ||
              (first_type == "Kaon" && second_type == "Pion"))
            invMassKPSame->Fill(invMass);

        } else if (first.GetCharge() * second.GetCharge() < 0) {
          invMassOppositeCharge->Fill(invMass);

          if ((first_type == "Pion" && second_type == "Kaon") ||
              (first_type == "Kaon" && second_type == "Pion"))
            invMassKPOpposite->Fill(invMass);
        }
      }
    }*/

    for (size_t j{0}; j < eventParticles.size() - 1; ++j) {
      for (size_t k{j + 1}; k < eventParticles.size(); ++k) {
        double invMass = eventParticles[k].GetInvariantMass(eventParticles[j]);

        totalInvMass->Fill(invMass);

        if (eventParticles[j].GetCharge() * eventParticles[k].GetCharge() > 0) {
          invMassSameCharge->Fill(invMass);
          if ((eventParticles[j].GetName() == "Pion+" &&
               eventParticles[k].GetName() == "Kaon+") ||
              (eventParticles[j].GetName() == "Pion-" &&
               eventParticles[k].GetName() == "Kaon-") ||
              (eventParticles[k].GetName() == "Pion+" &&
               eventParticles[j].GetName() == "Kaon+") ||
              (eventParticles[k].GetName() == "Pion-" &&
               eventParticles[j].GetName() == "Kaon-")) {
            invMassKPSame->Fill(invMass);
          }
        }
        if (eventParticles[j].GetCharge() * eventParticles[k].GetCharge() < 0) {
          invMassOppositeCharge->Fill(invMass);
          if ((eventParticles[j].GetName() == "Pion+" &&
               eventParticles[k].GetName() == "Kaon-") ||
              (eventParticles[j].GetName() == "Pion-" &&
               eventParticles[k].GetName() == "Kaon+") ||
              (eventParticles[k].GetName() == "Pion+" &&
               eventParticles[j].GetName() == "Kaon-") ||
              (eventParticles[k].GetName() == "Pion-" &&
               eventParticles[j].GetName() == "Kaon+")) {
            invMassKPOpposite->Fill(invMass);
          }
        }
      }
    }
  }

  TH1F* difference1 = new TH1F("h13",
                               "Difference between Invariant Mass of particles "
                               "with opposite charge and same charge",
                               1000, 0, 4);
  difference1->Add(invMassOppositeCharge, invMassSameCharge, 1, -1);

  TH1F* difference2 = new TH1F("h14",
                               "Difference between Invariant Mass of Kaon and "
                               "Pion with opposite and same charge",
                               1000, 0, 4);
  difference2->Add(invMassKPOpposite, invMassKPSame, 1, -1);

  TFile* file = new TFile("Analysis.root", "RECREATE");
  particleDistribution->Write();
  phiDistribution->Write();
  thetaDistribution->Write();
  impulseDistribution->Write();
  traverseImpulseDistribution->Write();
  energyDistribution->Write();
  totalInvMass->Write();
  invMassOppositeCharge->Write();
  invMassSameCharge->Write();
  invMassKPOpposite->Write();
  invMassKPSame->Write();
  invMassDaughters->Write();
  difference1->Write();
  difference2->Write();
  file->Close();

  return 0;
}