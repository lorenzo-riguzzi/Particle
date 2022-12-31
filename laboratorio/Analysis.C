/*#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TH1F.h"
#include "TMath.h"
#include "TROOT.h"*/

void Analysis() {
  TFile* file = new TFile("Analysis.root");
  TH1F* particleDistribution = (TH1F*)file->Get("h1");
  TH1F* phiDistribution = (TH1F*)file->Get("h2");
  TH1F* thetaDistribution = (TH1F*)file->Get("h3");
  TH1F* impulseDistribution = (TH1F*)file->Get("h4");
  TH1F* traverseImpulseDistribution = (TH1F*)file->Get("h5");
  TH1F* energyDistribution = (TH1F*)file->Get("h6");
  TH1F* totalInvMass = (TH1F*)file->Get("h7");
  TH1F* invMassOppositeCharge = (TH1F*)file->Get("h8");
  TH1F* invMassSameCharge = (TH1F*)file->Get("h9");
  TH1F* invMassKPOpposite = (TH1F*)file->Get("h10");
  TH1F* invMassKPSame = (TH1F*)file->Get("h11");
  TH1F* invMassDaughters = (TH1F*)file->Get("h12");
  TH1F* difference1 = (TH1F*)file->Get("h13");
  TH1F* difference2 = (TH1F*)file->Get("h14");

  TCanvas* c1 = new TCanvas("Data About Generated Particles", "c1", 1000, 1000);
  TCanvas* c2 = new TCanvas("Data Analysis", "c2", 1000, 1000);
  TCanvas* c3 = new TCanvas("Histograms sums", "c3", 1000, 1000);

  c1->Divide(3, 2);
  c2->Divide(3, 2);
  c3->Divide(2, 1);

  TF1* f1 = new TF1("f1", "[0]", 0, 2 * M_PI);
  TF1* f2 = new TF1("f2", "[0]*exp(-[1]*x)", 0, 4);
  TF1* f3 = new TF1("f3", "gaus", 0, 4);

  c1->cd(1);
  particleDistribution->Draw();
  std::cout << '\n'
            << "---------"
            << "Particle Distribution"
            << "---------" << '\n';
  for (Int_t i{1}; i < 8; ++i) {
    std::cout << " Bin " << i << ": " << particleDistribution->GetBinContent(i)
              << " +/- " << particleDistribution->GetBinError(i) << '\n';
  }
  std::cout << '\n';
  c1->cd(2);
  phiDistribution->Draw("HISTO");
  phiDistribution->Fit("f1");
  f1->Draw("SAME");
  std::cout << '\n'
            << "---------"
            << "Phi Distribution"
            << "---------" << '\n';
  std::cout << " Mean: " << f1->GetParameter(0) << " +/- " << f1->GetParError(0)
            << "\n X^2/NDF: " << f1->GetChisquare() / f1->GetNDF()
            << "\n Probability: " << f1->GetProb() << '\n'
            << '\n';
  c1->cd(3);
  thetaDistribution->Draw("HISTO");
  thetaDistribution->Fit("f1");
  f1->Draw("SAME");
  std::cout << '\n'
            << "---------"
            << "Theta Distribution"
            << "---------" << '\n';
  std::cout << " Mean: " << f1->GetParameter(0) << " +/- " << f1->GetParError(0)
            << "\n X^2/NDF: " << f1->GetChisquare() / f1->GetNDF()
            << "\n Probability: " << f1->GetProb() << '\n'
            << '\n';
  c1->cd(4);
  impulseDistribution->Fit("f2");
  impulseDistribution->Draw("HISTO, SAME");
  std::cout << '\n'
            << "---------"
            << "Impulse Distribution"
            << "---------" << '\n';
  std::cout << " Parameter 0: " << f2->GetParameter(0) << " +/- "
            << f2->GetParError(0) << '\n';
  std::cout << " Parameter 1: " << f2->GetParameter(1) << " +/- "
            << f2->GetParError(1)
            << "\n X^2/NDF: " << f2->GetChisquare() / f2->GetNDF()
            << "\n Probability: " << f2->GetProb() << '\n'
            << '\n';
  c1->cd(5);
  traverseImpulseDistribution->Draw("HISTO, SAME");
  c1->cd(6);
  energyDistribution->Draw();

  c2->cd(1);
  totalInvMass->Draw();
  c2->cd(2);
  invMassSameCharge->Draw();
  c2->cd(3);
  invMassOppositeCharge->Draw();
  c2->cd(4);
  invMassKPSame->Draw();
  c2->cd(5);
  invMassKPOpposite->Draw();
  c2->cd(6);
  invMassDaughters->Fit("f3");
  invMassDaughters->Draw("HISTO, SAME");
  std::cout << '\n'
            << "---------"
            << "Daughters Invariant Mass"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(0)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(0)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';

  c3->cd(1);
  //difference1->GetYaxis()->SetRangeUser(0, 5000);
  difference1->Fit("f3");
  difference1->Draw("HISTO, SAME");
  std::cout << '\n'
            << "---------"
            << "Difference 1"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(0)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(0)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';

  c3->cd(2);
  //difference2->GetYaxis()->SetRangeUser(0, 2600);
  difference2->Fit("f3");
  difference2->Draw("HISTO, SAME");
  std::cout << '\n'
            << "---------"
            << "Difference 2"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(0)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(0)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';
}