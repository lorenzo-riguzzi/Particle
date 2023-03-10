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

  TCanvas* c1 = new TCanvas("Data About Generated Particles", "c1", 1900, 1000);
  TCanvas* c2 = new TCanvas("Traverse Impulse and Energy", "c2", 1900, 1000);
  TCanvas* c3 = new TCanvas("Invariant Mass Histograms", "c3", 1900, 1000);
  TCanvas* c4 = new TCanvas("Invariant Mass of K*", "c4", 1900, 1000);

  gStyle->SetOptStat(1110);
  gStyle->SetOptFit(111);

  c1->Divide(2, 2);
  c2->Divide(2, 1);
  c3->Divide(3, 2);
  c4->Divide(3, 1);

  TF1* f1 = new TF1("f1", "[0]", 0, 2 * M_PI);
  TF1* f2 = new TF1("f2", "[0]*exp(-[1]*x)", 0, 4);
  TF1* f3 = new TF1("f3", "gaus", 0, 4);

  c1->cd(1);
  particleDistribution->SetFillColor(kBlue + 2);
  particleDistribution->GetXaxis()->SetBinLabel(1, "Pion+");
  particleDistribution->GetXaxis()->SetBinLabel(2, "Pion-");
  particleDistribution->GetXaxis()->SetBinLabel(3, "Kaon+");
  particleDistribution->GetXaxis()->SetBinLabel(4, "Kaon-");
  particleDistribution->GetXaxis()->SetBinLabel(5, "Proton+");
  particleDistribution->GetXaxis()->SetBinLabel(6, "Proton-");
  particleDistribution->GetXaxis()->SetBinLabel(7, "K*");
  particleDistribution->GetYaxis()->SetTitle("Entries");
  particleDistribution->GetYaxis()->SetTitleOffset(1.4);
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
  phiDistribution->SetFillColor(kBlue + 2);
  phiDistribution->GetXaxis()->SetTitle("Angle (radians)");
  phiDistribution->GetYaxis()->SetTitle("Entries");
  phiDistribution->GetYaxis()->SetTitleOffset(1.6);
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
  thetaDistribution->SetFillColor(kBlue + 2);
  thetaDistribution->GetXaxis()->SetTitle("Angle (radians)");
  thetaDistribution->GetYaxis()->SetTitle("Entries");
  thetaDistribution->GetYaxis()->SetTitleOffset(1.6);
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
  impulseDistribution->SetFillColor(kBlue + 2);
  impulseDistribution->GetXaxis()->SetTitle("Impulse (GeV/c)");
  impulseDistribution->GetYaxis()->SetTitle("Entries");
  impulseDistribution->GetYaxis()->SetTitleOffset(1.6);
  impulseDistribution->Draw("HISTO");
  impulseDistribution->Fit("f2", "R");
  f2->Draw("SAME");
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

  c2->cd(1);
  traverseImpulseDistribution->SetFillColor(kBlue + 2);
  traverseImpulseDistribution->GetXaxis()->SetTitle("Traverse impulse (GeV/c)");
  traverseImpulseDistribution->GetYaxis()->SetTitle("Entries");
  traverseImpulseDistribution->GetYaxis()->SetTitleOffset(1.4);
  traverseImpulseDistribution->Draw("HISTO, SAME");
  c2->cd(2);
  energyDistribution->SetFillColor(kBlue + 2);
  energyDistribution->GetXaxis()->SetTitle("Energy (GeV)");
  energyDistribution->GetYaxis()->SetTitle("Entries");
  energyDistribution->GetYaxis()->SetTitleOffset(1.4);
  energyDistribution->Draw();

  c3->cd(1);
  totalInvMass->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  totalInvMass->GetYaxis()->SetTitle("Entries");
  totalInvMass->GetYaxis()->SetTitleOffset(1.4);
  totalInvMass->Draw();
  c3->cd(2);
  invMassSameCharge->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  invMassSameCharge->GetYaxis()->SetTitle("Entries");
  invMassSameCharge->GetYaxis()->SetTitleOffset(1.4);
  invMassSameCharge->Draw();
  c3->cd(3);
  invMassOppositeCharge->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  invMassOppositeCharge->GetYaxis()->SetTitle("Entries");
  invMassOppositeCharge->GetYaxis()->SetTitleOffset(1.4);
  invMassOppositeCharge->Draw();
  c3->cd(4);
  invMassKPSame->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  invMassKPSame->GetYaxis()->SetTitle("Entries");
  invMassKPSame->GetYaxis()->SetTitleOffset(1.4);
  invMassKPSame->Draw();
  c3->cd(5);
  invMassKPOpposite->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  invMassKPOpposite->GetYaxis()->SetTitle("Entries");
  invMassKPOpposite->GetYaxis()->SetTitleOffset(1.4);
  invMassKPOpposite->Draw();

  c4->cd(1);
  invMassDaughters->SetFillColor(kBlue + 2);
  invMassDaughters->SetTitleSize(0.028, "y");
  invMassDaughters->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  invMassDaughters->GetYaxis()->SetTitle("Entries");
  invMassDaughters->GetYaxis()->SetTitleOffset(2);
  invMassDaughters->Fit("f3");
  invMassDaughters->Draw("E");
  invMassDaughters->Draw("HISTO, SAME");
  invMassDaughters->Draw("HISTO");
  invMassDaughters->Draw("E, P, SAME");
  // f3->Draw("Same");
  std::cout << '\n'
            << "---------"
            << "Daughters Invariant Mass"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(1)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(2)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';

  c4->cd(2);
  // difference1->GetYaxis()->SetRangeUser(0, 4000);
  difference1->GetXaxis()->SetRangeUser(0, 2);
  difference1->SetTitleSize(0.028, "y");
  difference1->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  difference1->GetYaxis()->SetTitle("Entries");
  difference1->GetYaxis()->SetTitleOffset(2);
  difference1->Fit("f3");
  difference1->Draw("E");
  difference1->Draw("HISTO, SAME");
  difference1->Draw("HISTO");
  difference1->Draw("E, P, SAME");
  // f3->Draw("SAME");
  gPad->Update();
  TPaveStats* st1 = (TPaveStats*)difference1->FindObject("stats");
  st1->SetOptStat(0000);
  std::cout << '\n'
            << "---------"
            << "Difference 1"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(1)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(2)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';

  c4->cd(3);
  difference2->GetXaxis()->SetRangeUser(0, 2);
  difference2->SetTitleSize(0.028, "y");
  difference2->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  difference2->GetYaxis()->SetTitle("Entries");
  difference2->GetYaxis()->SetTitleOffset(2);
  difference2->Fit("f3");
  difference2->Draw("E");
  difference2->Draw("HISTO, SAME");
  difference2->Draw("HISTO");
  difference2->Draw("E, P, SAME");
  gPad->Update();
  TPaveStats* st2 = (TPaveStats*)difference2->FindObject("stats");
  st2->SetOptStat(0000);
  std::cout << '\n'
            << "---------"
            << "Difference 2"
            << "---------" << '\n';
  std::cout << " Amplitude: " << f3->GetParameter(0) << " +/- "
            << f3->GetParError(0) << '\n';
  std::cout << " Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(1)
            << '\n';
  std::cout << " STDEV: " << f3->GetParameter(2) << " +/- "
            << f3->GetParError(2)
            << "\n X^2/NDF: " << f3->GetChisquare() / f3->GetNDF()
            << "\n Probability: " << f3->GetProb() << '\n'
            << '\n';
}