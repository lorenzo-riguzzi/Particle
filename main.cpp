#include <cmath>
#include <iostream>

#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"
#include "TFile.h"
#include "TH1F.h"
#include "TRandom.h"

int Main() {
  Particle::AddParticle("Pion+", 0.13957, 1);
  Particle::AddParticle("Pion-", 0.13957, -1);
  Particle::AddParticle("Kaon+", 0.49367, 1);
  Particle::AddParticle("Kaon-", 0.49367, -1);
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

  for (size_t i = 0; i < 1E5; ++i) {
    eventParticles.clear();

    for (size_t j = 0; j != 100; ++j) {
      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double p_module = gRandom->Exp(1);
      double px = p_module * cos(phi) * sin(theta);
      double py = p_module * sin(phi) * sin(theta);
      double pz = p_module * cos(theta);
      double x = gRandom->Rndm();

      Particle particle;
      particle.SetP(px, py, pz);

      if (x <= 0.4) {
        particle.SetIndex("Pion+");
      } else if (x <= 0.8) {
        particle.SetIndex("Pion-");
      } else if (x <= 0.85) {
        particle.SetIndex("Kaon+");
      } else if (x <= 0.9) {
        particle.SetIndex("Kaon-");
      } else if (x <= 0.945) {
        particle.SetIndex("Proton+");
      } else if (x <= 0.99) {
        particle.SetIndex("Proton-");
      } else {
        particle.SetIndex("K*");
      }

      phiDistribution->Fill(phi);
      thetaDistribution->Fill(theta);
      impulseDistribution->Fill(p_module);
      traverseImpulseDistribution->Fill(p_module * sin(theta));
      energyDistribution->Fill(particle.GetEnergy());

      if (particle.GetName() == "K*") {
        double prob = gRandom->Rndm();

        particleDistribution->Fill(particle.GetIndex());
        eventParticles.push_back(particle);

        Particle pi;
        Particle k;

        pi.SetIndex("Pion+");
        k.SetIndex("Kaon-");

        if (prob > 0.5) {
          pi.SetIndex("Pion-");
          k.SetIndex("Kaon+");
        }

        particle.Decay2body(pi, k);

        eventParticles.push_back(pi);
        eventParticles.push_back(k);

        invMassDaughters->Fill(pi.GetInvariantMass(k));
      } else {
        particleDistribution->Fill(particle.GetIndex());
        eventParticles.push_back(particle);
      }
    }

    for (size_t j{0}; j < eventParticles.size(); ++j) {
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

    /* std::string name;

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

       particleDistribution->Fill(particle.GetIndex());
       //eventParticles.push_back(particle);

       Particle p = Particle("Pion+");
       Particle k = Particle("Kaon-");

       if (probability > 0.5) {
         p.SetIndex("Pion-");
         k.SetIndex("Kaon+");
       }

       particle.Decay2body(p, k);

       eventParticles.push_back(p);
       eventParticles.push_back(k);

       invMassDaughters->Fill(p.GetInvariantMass(k));
     } else {
       particleDistribution->Fill(particle.GetIndex());
       eventParticles.push_back(particle);
     }
   }

   for (size_t j{0}; j < eventParticles.size(); ++j) {
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
   }*/
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