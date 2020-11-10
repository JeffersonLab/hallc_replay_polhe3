 #include <ROOT/RDataFrame.hxx>
#include <TCanvas.h>
#include <TH1D.h>
#include <string>

const double kCalorimeterMinimumEPCut = .8;

// Draft of a program to conduct electron-pion ratio estimation for SHMS runs by hand. 
// https://logbooks.jlab.org/entry/3650663
// https://hallcweb.jlab.org/DocDB/0010/001053/001/wisewordsoncheranalysis.pdf
// murchhanaroy@uky.edu
// August 2020 




// cdaq full replay dir.
const std::string kReplayDir =
    "/home/cdaq/polhe3/production/hallc_replay/ROOTfiles";

int electron_pion_ratio(int run = 10986, int nevents = -1) {

  ROOT::RDataFrame d{"T", Form("%s/shms_replay_production_default_%i_%i.root",
                               kReplayDir.c_str(), run, nevents)};


// Cuts to make sure we only look at good tracks, and define an extra useful
  // variable for the calorimeter shower-only E/P
  auto d_good_track =
      d.Filter("-10 < P.gtr.dp && P.gtr.dp < 22") // delta cut
          .Filter("fabs(P.gtr.y) < 4")            // target y position
          .Filter("fabs(P.gtr.th) < .06")         // target x' (theta)
          .Filter("P.cal.etottracknorm < 2")      // remove nonsensical values
          .Define("P_cal_eshowertracknorm",       // shower E/P
                  "P.cal.etottracknorm - P.cal.eprtracknorm")
          .Filter("P_cal_eshowertracknorm > .02"); // remove low energy
                                                   // electrons and positrons
                                                   // from dipole frame

  // Make a histogram for the #photo-electrons in the Cherenkov and the
  // calorimeter energy deposition (both in 1D and 2D)
  auto hngc = d_good_track.Histo1D(
      {"hngc", "SHMS NGC #pe;SHMS NGC #pe;count", 100, 0, 25},
      "P.ngcer.npeSum");

  auto hcal_all = d_good_track.Histo1D(
      {"hcal_all", "SHMS Cal E/P;SHMS Cal E/P;count", 100, 0, 2},
      "P.cal.etottracknorm");

  auto hcal_all_2d = d_good_track.Histo2D(
      {"hcal_all_2d",
       "SHMS preshower versus shower E/P;SHMS Shower E/P;SHMS Pre E/P", 50, 0,
       2, 50, 0, 2},
      "P_cal_eshowertracknorm", "P.cal.eprtracknorm");

  // Now we want to select particles we now for sure are electrons. Therefor we
  // apply a super-strict cut on the SHMS NGC of #pe > 5
  auto d_electron = d_good_track.Filter("P.ngcer.npeSum > 2");

  // Histogram of what the calorimeter response looks like with this cut
  auto hcal_electron = d_electron.Histo1D(
      {"hcal_electron", "SHMS Cal E/P for electrons;SHMS Cal E/P;count", 100, 0,
       2},
      "P.cal.etottracknorm");

  auto hcal_electron_2d =
      d_electron.Histo2D({"hcal_electron_2d",
                          "SHMS preshower versus shower E/P for "
                          "electrons;SHMS Shower E/P;SHMS Pre E/P",
                          50, 0, 2, 50, 0, 2},
                         "P_cal_eshowertracknorm", "P.cal.eprtracknorm");

  // For the pion rejection we need to do something similar. For this example,
  // we assume everything that is not an electron is a pion (ignoring kaons and
  // protons)
  auto d_pion = d_good_track.Filter("P.ngcer.npeSum < 0.1");

  // Histogram of what the calorimeter response looks like with this cut
  auto hcal_pion = d_pion.Histo1D(
      {"hcal_pion", "SHMS Cal E/P for pions;SHMS Cal E/P;count", 100, 0, 2},
      "P.cal.etottracknorm");

  auto hcal_pion_2d =
      d_pion.Histo2D({"hcal_pion_2d",
                      "SHMS preshower versus shower E/P for pions;SHMS "
                      "Shower E/P;SHMS Pre E/P",
                      50, 0, 2, 50, 0, 2},
                     "P_cal_eshowertracknorm", "P.cal.eprtracknorm");

  // Calculate the efficiency for our calorimeter cut: count how many electrons
  // we have and how many survive our calorimeter cut. The cut value is defined
  // at the top of this file.
  const std::string calorimeter_electron_cut =
      Form("P.cal.etottracknorm > %f", kCalorimeterMinimumEPCut);

  auto d_electron_cal_cut = d_electron.Filter(calorimeter_electron_cut);

  auto n_electrons = d_electron.Count();
  auto n_electrons_cal_cut = d_electron_cal_cut.Count();

  // To calculate the pion rejection factor, we need the total number of pions
  // and the number of pions that survive our cut
  auto d_pion_cal_cut = d_pion.Filter(calorimeter_electron_cut);

  auto n_pions = d_pion.Count();
  auto n_pions_cal_cut = d_pion_cal_cut.Count();

  // Everything useful is defined. Let's now make plots and calculate efficiency
  // and rejection!

  // Tell ROOT we want to plot the histogram titles
  gStyle->SetOptTitle(1);

  // A canvas for us to draw our results on
  TCanvas* c = new TCanvas("c", "Calorimeter PID Studies", 800, 800);
  // We subdivide the canvas in a 2x2 grid of panes for our figures
  c->Divide(2,  // 2 columns
            2); // 2 rows
  // We make a multi-page PDF file, so let's start by opening our file (notice
  // the opening bracket)
  c->Print(
      Form("calorimeter_pid_shms_%i_%.2f.pdf[", run, kCalorimeterMinimumEPCut));

  // PAGE 1
  // First we want to plot the Cherenkov and calorimeter spectra for all tracks
  {
    c->cd(1); // left top
    hngc->Draw();
    c->cd(2); // right top
    TPaveText t{0., 0.4, 1.0, 0.8, "NB NDC"};
    t.SetFillColor(kWhite);
    t.AddText("Raw spectra (without any PID cuts)");
    t.Draw();
    c->cd(3); // left bottom
    hcal_all->Draw();
    c->cd(4);                  // right bottom
    hcal_all_2d->Draw("colz"); // colz option plots a color scale z-axis
    // print this page
    c->Print(Form("calorimeter_pid_shms_%i_%.2f.pdf", run,
                  kCalorimeterMinimumEPCut));
  }

  // PAGE 2
  // Electron detection efficiency
  {
    c->cd(1); // left top
    hngc->Draw();
    // Add a line to show our Cherenkov cut of #pe > 5
    TLine l1{5, 0, 5, hngc->GetMaximum()};
    l1.SetLineStyle(2); // Dashed line
    l1.Draw();
    c->cd(2); // right top
    TPaveText t{0., 0.4, 1.0, 0.8, "NB NDC"};
    t.SetFillColor(kWhite);
    t.AddText(Form("Total number of electrons: %i", (int)*n_electrons));
    t.AddText("(Cherenkov cut: #pe > 5)");
    t.AddText(Form("Number of electrons after calorimeter cut: %i",
                   (int)*n_electrons_cal_cut));
    t.AddText(Form("(Calorimeter cut: E/P > %.2f)", kCalorimeterMinimumEPCut));
    t.AddText(Form("Calorimeter Efficiency: %.2f%%",
                   *n_electrons_cal_cut * 100. / *n_electrons));
    t.Draw();
    c->cd(3); // left bottom
    hcal_electron->Draw();
    // Add a line for the E/P cut we did
    TLine l2{kCalorimeterMinimumEPCut, 0, kCalorimeterMinimumEPCut,
             hcal_electron->GetMaximum()};
    l2.SetLineStyle(2); // Dashed line
    l2.Draw();
    c->cd(4);                       // right bottom
    hcal_electron_2d->Draw("colz"); // colz option plots a color scale z-axis
    // Add a line for the calorimeter cuts we did
    TLine l3{kCalorimeterMinimumEPCut, 0, 0, kCalorimeterMinimumEPCut};
    l3.SetLineStyle(2); // Dashed line
    l3.Draw();
    c->Print(Form("calorimeter_pid_shms_%i_%.2f.pdf", run,
                  kCalorimeterMinimumEPCut));
  }
  // PAGE 3
  // Pion rejection
  {
    c->cd(1); // left top
    hngc->Draw();
    // Add a line to show our Cherenkov cut of #pe < .1
    TLine l1{.1, 0, .1, hngc->GetMaximum()};
    l1.SetLineStyle(2); // Dashed line
    l1.Draw();
    c->cd(2); // right top
    TPaveText t{0., 0.4, 1.0, 0.8, "NB NDC"};
    t.SetFillColor(kWhite);
    t.AddText(Form("Total number of pions: %i", (int)*n_pions));
    t.AddText("(Cherenkov cut: #pe < .1)");
    t.AddText(Form("Number of pions after calorimeter cut: %i",
                   (int)*n_pions_cal_cut));
    t.AddText(Form("(Calorimeter cut: E/P > %.2f)", kCalorimeterMinimumEPCut));
    t.AddText(
        Form("Pion rejection factor: %.2f", *n_pions * 1. / *n_pions_cal_cut));
    t.Draw();
    c->cd(3); // left bottom
    hcal_pion->Draw();
    // Add a line for the E/P cut we did
    TLine l2{kCalorimeterMinimumEPCut, 0, kCalorimeterMinimumEPCut,
             hcal_pion->GetMaximum()};
    l2.SetLineStyle(2); // Dashed line
    l2.Draw();
    c->cd(4);                   // right bottom
    hcal_pion_2d->Draw("colz"); // colz option plots a color scale z-axis
    // Add a line for the calorimeter cuts we did
    TLine l3{kCalorimeterMinimumEPCut, 0, 0, kCalorimeterMinimumEPCut};
    l3.SetLineStyle(2); // Dashed line
    l3.Draw();
    c->Print(Form("calorimeter_pid_shms_%i_%.2f.pdf", run,
                  kCalorimeterMinimumEPCut));
  }

  // Close our multi-page PDF file
  c->Print(
      Form("calorimeter_pid_shms_%i_%.2f.pdf]", run, kCalorimeterMinimumEPCut));

  // That's all!
  return 0;
}

