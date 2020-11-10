#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TROOT.h>
#include <TMatrixDBase.h>
#include <TMath.h>
#include <TChain.h>
#include <TH1F.h>
#include <TF1.h>
#include <cstdlib>
#include "TStyle.h"
#include "TColor.h"

using namespace std;

static const double degtorad = 3.141592653589793/180.;

void formatit_2dd(TH2F *histo, TString bar, TString xax, TString yax, Int_t flag){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  

  histo->SetLabelSize(0.035, "x");
  histo->SetLabelSize(0.035, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.3);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  //  histo->SetLineColor(kPink+10);
  //  histo->SetLineWidth(3);
  //  histo->SetLineStyle(2);
  histo->Draw("colz");  
 
 if (flag == 0){

  auto leg = new TLegend(0.2,0.85,0.35,0.95);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kAzure+7);
  leg->SetBorderSize(0);
  leg->AddEntry(histo,bar,"");
  leg->Draw("same"); 

 }

}

void formatit_1h(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  //  histo->SetFillStyle(3315);
  histo->SetLineWidth(3);
  histo->SetLineStyle(1);
  histo->SetLineColor(kOrange+7);
  //  histo->SetFillColor(kOrange+10);
  histo->Draw("");  

  auto leg1 = new TLegend(0.15,0.85,0.65,0.95);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kOrange+7);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

  /*  if (flag == 0){

    TLine *linex = new TLine(30.,0.,30.,600.);
    TLine *liney = new TLine(0.,30.,600.,30.);
    linex->SetLineColor(kOrange+7);
    liney->SetLineColor(kOrange+7);
    linex->Draw("same");
    liney->Draw("same");

  }

  TF1 *gaus1 = new TF1("gaus1","gaus",xmin,xmax);
  gaus1->SetLineColor(kMagenta-9);
  gaus1->SetLineStyle(2);
  gaus1->SetLineWidth(5);
  histo->Fit("gaus1","R");
  gaus1->Draw("same"); 
  
  Float_t mean = gaus1->GetParameter(1);
  Float_t err_mean = gaus1->GetParError(1);
  Float_t sigma = gaus1->GetParameter(2); 
  
  TString legis_1, legis_2;
  legis_1 = Form("#mu(#pi) = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);
  
  leg = new TLegend(0.55,0.9,0.85,0.93);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kMagenta-9);
  leg->SetBorderSize(0);
  leg->AddEntry(histo,legis_1,"");
  leg->Draw("same"); 

  cout << " mean sigma:  " << mean << " " << sigma << endl; 
  dbfilee << pmt << " " << mean << " " << err_mean << "\n";
  //  cout << " file name is: " << &dbfilee << endl; */

}

void formatit_1h_same(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  histo->SetFillStyle(3356);
  histo->SetLineStyle(2);
  histo->SetLineWidth(1);
  histo->SetLineColor(kBlue-7);
  histo->SetFillColor(kBlue-7);
  histo->Draw("same");  

  auto leg1 = new TLegend(0.15,0.77,0.65,0.87);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kBlue-7);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

}


void formatit_1h_same2(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  histo->SetFillStyle(3356);
  histo->SetLineStyle(2);
  histo->SetLineWidth(3);
  histo->SetLineColor(kBlack);
  //  histo->SetFillColor(kAzure-3);
  histo->Draw("same");  

  auto leg1 = new TLegend(0.15,0.67,0.65,0.77);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kBlack);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

}

void formatit_1h_same3(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  histo->SetFillStyle(3490);
  histo->SetLineStyle(3);
  histo->SetLineWidth(3);
  histo->SetLineColor(kPink+7);
  histo->SetFillColor(kPink+7);
  histo->Draw("same");  

  auto leg1 = new TLegend(0.15,0.57,0.65,0.67);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kPink+7);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

}

void formatit_1h_same4(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  //  histo->SetFillStyle(3244);
  histo->SetLineStyle(1);
  histo->SetLineWidth(4);
  histo->SetLineColor(kTeal+1);
  //  histo->SetFillColor(kTeal+1);
  histo->Draw("same");  

  auto leg1 = new TLegend(0.15,0.47,0.65,0.57);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kTeal+1);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

}

void formatit_1h_same5(TH1F *histo, TString xax, TString yax, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.21);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(0.13);
  gPad->SetLogy();

  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetYaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitleOffset(1.1);
  histo->GetYaxis()->SetTitle(yax);
  histo->GetXaxis()->SetTitle(xax);
  //  histo->SetFillStyle(3244);
  histo->SetLineStyle(4);
  histo->SetLineWidth(3);
  histo->SetLineColor(kRed+2);
  //  histo->SetFillColor(kTeal+1);
  histo->Draw("same");  

  auto leg1 = new TLegend(0.15,0.47,0.65,0.57);
  leg1->SetFillColor(-1);
  leg1->SetFillStyle(0);
  leg1->SetTextSize(0.055);
  leg1->SetTextColor(kRed+2);
  leg1->SetBorderSize(0);
  leg1->AddEntry(histo,bar,"");
  leg1->Draw("same"); 

}

int main(){
  
  TFile *myfile;
  
  TChain chain("T");
  
  cout << "what a fuck? " << endl; 

  chain.Add("/lustre19/expphy/cache/hallc/E12-10-002/simona/ROOTfiles/shms_replay_production_all_2552_1500000.root");

  
  Double_t ngcer_pulseint[30], ngcer_counter[30], ngcer_errflag[30], ngcer_gooddifftime[30], beta;
  Int_t ndata_ngcer_pulseint, ndata_ngcer_mult;

  Double_t ngcer, etotnorm, premean, preneg, prepos, goodint[30];
  Double_t inside, starttime, ngcer_pulseamp[30], ngcer_pulsetime[30];
  Double_t ngcer_goodpulseamp[30], ngcer_goodpulsetime[30], ngcer_goodpulseint[30];
  
  Double_t reftime_amp, ngcer_mult[30], delta, xfp, yfp, xpfp,ypfp, reftime;

  chain.SetBranchAddress("Ndata.P.ngcer.adcPulseInt", &ndata_ngcer_pulseint);

  chain.SetBranchAddress("P.ngcer.adcCounter", &ngcer_counter[0]);
  chain.SetBranchAddress("P.ngcer.adcPulseInt", &ngcer_pulseint[0]);
  chain.SetBranchAddress("P.ngcer.adcPulseAmp", &ngcer_pulseamp[0]);
  chain.SetBranchAddress("P.ngcer.adcPulseTime", &ngcer_pulsetime[0]);

  chain.SetBranchAddress("P.ngcer.goodAdcPulseInt", &ngcer_goodpulseint[0]);
  chain.SetBranchAddress("P.ngcer.goodAdcPulseAmp", &ngcer_goodpulseamp[0]);
  chain.SetBranchAddress("P.ngcer.goodAdcTdcDiffTime", &ngcer_gooddifftime[0]);

  chain.SetBranchAddress("P.ngcer.adcErrorFlag", &ngcer_errflag[0]);

  chain.SetBranchAddress("Ndata.P.ngcer.goodAdcMult", &ndata_ngcer_mult);
  chain.SetBranchAddress("P.ngcer.goodAdcMult", &ngcer_mult[0]);

  chain.SetBranchAddress("P.cal.etottracknorm", &etotnorm);
  chain.SetBranchAddress("P.ngcer.npeSum", &ngcer);
  chain.SetBranchAddress("P.hod.betanotrack", &beta);
  chain.SetBranchAddress("P.dc.InsideDipoleExit", &inside);

  chain.SetBranchAddress("P.hod.starttime", &starttime);
  chain.SetBranchAddress("P.gtr.dp", &delta);

  chain.SetBranchAddress("P.dc.x_fp", &xfp);
  chain.SetBranchAddress("P.dc.y_fp", &yfp);

  chain.SetBranchAddress("P.dc.xp_fp", &xpfp);
  chain.SetBranchAddress("P.dc.yp_fp", &ypfp);
  chain.SetBranchAddress("P.hod.betanotrack", &beta);

  chain.SetBranchAddress("T.shms.pFADC_TREF_ROC2_adcPulseAmpRaw", &reftime_amp);
  chain.SetBranchAddress("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw", &reftime);


  TH1F* int_ngcer[9];
  TH1F* int_goodngcer[9];
  TH1F* amp_ngcer[9];
  TH1F* delta_per_pmt[9];

  TH1F* amp_pmt4only;

  TH1F* pmt0_01crosstalk;
  TH1F* pmt1_01crosstalk;

  TH1F* amp_goodngcer[9];
  TH1F* time_ngcer[9];
  TH1F* time_ngcer_calo[9];
  TH1F* time_ngcer_calo_reftime[9];
  TH1F* time_ngcer_delta[9];
  TH1F* time_ngcer_pmt1;
  TH1F* time_ngcer_pmt1_from4;
  TH1F* time_ngcer_pmt2;
  TH1F* time_ngcer_pmt3;
  TH1F* time_ngcer_pmt4;

  TH1F* time_ngcer_pmt4_inside;
  TH1F* time_ngcer_pmt4_tref;

  TH1F* time_goodngcer[9];
  TH1F* time_ngcer_reftime[9];

  TH2F* time_1vs2;
  TH2F* time_ngcer_pmt1_xy;
  TH2F* time_ngcer_pmt1_xy_delta;

  TH2F* time_ngcer_pmt3_xy;
  TH2F* time_ngcer_pmt3_xy_w;
  TH2F* time_ngcer_pmt3_dv;
  TH2F* time_ngcer_pmt3_xy_delta;
  TH2F* time_ngcer_pmt3_xy_w_delta;
  TH2F* time_ngcer_pmt3_dv_delta;

  TH2F* time_ngcer_pmt2_xy;
  TH2F* time_ngcer_pmt2_xy_w;
  TH2F* time_ngcer_pmt2_dv;
  TH2F* time_ngcer_pmt2_xy_delta;
  TH2F* time_ngcer_pmt2_xy_w_delta;
  TH2F* time_ngcer_pmt2_dv_delta;

  TH2F* time_ngcer_pmt1_xy_w;
  TH2F* time_ngcer_pmt1_dv;
  TH2F* time_ngcer_pmt1_xy_w_delta;
  TH2F* time_ngcer_pmt1_dv_delta;

  TH2F* time_ngcer_pmt4_xy;
  TH2F* time_ngcer_pmt4_xy_w;
  TH2F* time_ngcer_pmt4_dv;
  TH2F* time_ngcer_pmt4_xy_delta;
  TH2F* time_ngcer_pmt4_xy_w_delta;
  TH2F* time_ngcer_pmt4_dv_delta;

  TH2F* time_ngcer_pmt4_xp;
  TH2F* time_ngcer_pmt4_xp_w;
  TH2F* time_ngcer_pmt4_dvxp;
  TH2F* time_ngcer_pmt4_xp_delta;
  TH2F* time_ngcer_pmt4_xp_w_delta;
  TH2F* time_ngcer_pmt4_dvxp_delta;

  TH2F* time_ngcer_pmt3_xp;
  TH2F* time_ngcer_pmt3_xp_w;
  TH2F* time_ngcer_pmt3_dvxp;

  TH2F* time_ngcer_pmt3_xp_delta;
  TH2F* time_ngcer_pmt3_xp_w_delta;
  TH2F* time_ngcer_pmt3_dvxp_delta;

  TH2F* time_ngcer_pmt2_xp;
  TH2F* time_ngcer_pmt2_xp_w;
  TH2F* time_ngcer_pmt2_dvxp;
  TH2F* time_ngcer_pmt2_xp_delta;
  TH2F* time_ngcer_pmt2_xp_w_delta;
  TH2F* time_ngcer_pmt2_dvxp_delta;

  TH2F* time_ngcer_pmt1_xp;
  TH2F* time_ngcer_pmt1_xp_w;
  TH2F* time_ngcer_pmt1_dvxp;

  TH2F* time_ngcer_pmt1_only_xy;
  TH2F* time_ngcer_pmt1_only_xy_w;
  TH2F* time_ngcer_pmt1_only_dv;

  TH2F* time_ngcer_pmt2_only_xy;
  TH2F* time_ngcer_pmt2_only_xy_w;
  TH2F* time_ngcer_pmt2_only_dv;

  TH2F* time_ngcer_pmt3_only_xy;
  TH2F* time_ngcer_pmt3_only_xy_w;
  TH2F* time_ngcer_pmt3_only_dv;

  TH2F* time_ngcer_pmt4_only_xy;
  TH2F* time_ngcer_pmt4_only_xy_w;
  TH2F* time_ngcer_pmt4_only_dv;


  TH2F* time_ngcer_pmt1_xp_delta;
  TH2F* time_ngcer_pmt1_xp_w_delta;
  TH2F* time_ngcer_pmt1_dvxp_delta;

  TH1F* time_ngcer_pmt1_1ddelta;
  TH1F* time_ngcer_pmt2_1ddelta;
  TH1F* time_ngcer_pmt3_1ddelta;
  TH1F* time_ngcer_pmt4_1ddelta;

  for(Int_t mm = 0; mm < 8; mm++){
    int_ngcer[mm] = new TH1F("","",100,0.,100.);
    int_goodngcer[mm] = new TH1F("","",100,0.,100.);

    amp_ngcer[mm] = new TH1F("","",500,0.,600.);
    amp_goodngcer[mm] = new TH1F("","",500,0.,600.);

    time_ngcer[mm] = new TH1F("","",300,-80.,20.);
    time_ngcer_calo[mm] = new TH1F("","",300,-80.,20.);
    time_ngcer_calo_reftime[mm] = new TH1F("","",300,-80.,20.);
    time_ngcer_delta[mm] = new TH1F("","",300,-80.,20.);
    time_goodngcer[mm] = new TH1F("","",300,-80.,20.);
    time_ngcer_reftime[mm] = new TH1F("","",300,-80.,20.);
    delta_per_pmt[mm] = new TH1F("","",100,-30.,40.);

  }
  
  time_1vs2 = new TH2F("","",700,-200.,200.,700,-200.,200.);
  time_ngcer_pmt1 = new TH1F("","",300,-80.,20.);

  time_ngcer_pmt1_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);

  time_ngcer_pmt1_only_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_only_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_only_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_only_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_only_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_only_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_only_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_only_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_only_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_only_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_only_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_only_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);


  time_ngcer_pmt1_xy_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_xy_w_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt1_dv_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);


  time_ngcer_pmt2_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_xy_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_xy_w_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt2_dv_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);

  time_ngcer_pmt3_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_xy_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_xy_w_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt3_dv_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);

  time_ngcer_pmt4_xy = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_xy_w = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_dv = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_xy_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_xy_w_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);
  time_ngcer_pmt4_dv_delta = new TH2F("","",140,-40.,40.,150,-60.,60.);

  time_ngcer_pmt4_xp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt4_xp_w = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt4_dvxp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt4_xp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt4_xp_w_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt4_dvxp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt3_xp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt3_xp_w = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt3_dvxp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt3_xp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt3_xp_w_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt3_dvxp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt2_xp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt2_xp_w = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt2_dvxp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt2_xp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt2_xp_w_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt2_dvxp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt1_xp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt1_xp_w = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt1_dvxp = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt1_xp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt1_xp_w_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);
  time_ngcer_pmt1_dvxp_delta = new TH2F("","",100,-0.2,0.2,150,-60.,60.);

  time_ngcer_pmt1_from4 = new TH1F("","",300,-80.,20.);
  time_ngcer_pmt2 = new TH1F("","",300,-80.,20.);
  time_ngcer_pmt3 = new TH1F("","",300,-80.,20.);
  time_ngcer_pmt4 = new TH1F("","",300,-80.,20.);
  time_ngcer_pmt4_inside = new TH1F("","",300,-80.,20.);

  time_ngcer_pmt4_tref = new TH1F("","",2000,0.,7000.);

  amp_pmt4only = new TH1F("","",500,0.,600.);

  pmt0_01crosstalk = new TH1F("","",500,0.,600.);
  pmt1_01crosstalk = new TH1F("","",500,0.,600.);

  time_ngcer_pmt1_1ddelta = new TH1F("","",100,-30.,40.);
  time_ngcer_pmt2_1ddelta = new TH1F("","",100,-30.,40.);
  time_ngcer_pmt3_1ddelta = new TH1F("","",100,-30.,40.);
  time_ngcer_pmt4_1ddelta = new TH1F("","",100,-30.,40.);

  ofstream outfilemore("pmt3_34_crosstalk.txt");
  ofstream outfilemore1("pmt4_34_crosstalk.txt");

  Int_t nentries = chain.GetEntries();
  //  for(Int_t i=0; i < nentries; i++){
  for(Int_t i=0; i < 800000; i++){
    
    chain.GetEntry(i);
    
    // this is to check crosstalk 
    
    if (ngcer_mult[3] == 1. && ngcer_mult[2] == 1. && ngcer_mult[0] == 0 && ngcer_mult[1] == 0){
      
      for(Int_t nhit = 0; nhit < ndata_ngcer_pulseint; nhit++){ 

	Double_t timediff2 = starttime - ngcer_pulsetime[nhit];

	if (delta > -10. && delta < 22.){

	  if (ngcer_counter[nhit] == 3 && ngcer_pulseamp[nhit] > 0.){
	    outfilemore << i << setw(20) << nhit << setw(20) << ngcer_counter[nhit] << setw(20) << ngcer_pulseint[nhit] << setw(20) << ngcer_pulseamp[nhit] << setw(20) << timediff2 << endl; 
	  }
	  if (ngcer_counter[nhit] == 4 && ngcer_pulseamp[nhit] > 0.){
	    outfilemore1 << i << setw(20) << nhit << setw(20) << ngcer_counter[nhit] << setw(20) << ngcer_pulseint[nhit] << setw(20) << ngcer_pulseamp[nhit] << setw(20) << timediff2 << endl; 
	  }

	}
	
      }
    }
    // end of checking crosstalk


   for (Int_t counter = 1; counter < 5; counter++){

      //      if(etotnorm > 0.7){

       int_goodngcer[counter]->Fill(ngcer_goodpulseint[counter-1]);
       amp_goodngcer[counter]->Fill(ngcer_goodpulseamp[counter-1]);
       time_goodngcer[counter]->Fill(ngcer_gooddifftime[counter-1]);
       //      }
    } 

    for(Int_t nhit = 0; nhit < ndata_ngcer_pulseint; nhit++){  
      
      for (Int_t counter = 1; counter < 5; counter++){

	if(ngcer_counter[nhit] == counter){

	  // INT and AMP per counter for all hits per event; no other cuts
	  int_ngcer[counter]->Fill(ngcer_pulseint[nhit]);
	  amp_ngcer[counter]->Fill(ngcer_pulseamp[nhit]);

	  // TIME DIFF per counter for all hits per event; no other cuts
	  Double_t timediff = starttime - ngcer_pulsetime[nhit];
	  time_ngcer[counter]->Fill(timediff);
	  
	  if (etotnorm > 0.7){
	    // TIME DIFF per counter for all hits per event but with a CALO cut to select ELECTRONS
	    time_ngcer_calo[counter]->Fill(timediff);
	  
	    if(reftime > 2500.){
	      time_ngcer_calo_reftime[counter]->Fill(timediff);
	    }
	  }
	  
	  // TIME DIFF per counter for all hits per event but with a cut to avoid corrupted REF TIMES
	  if (reftime_amp > 1.){
	    time_ngcer_reftime[counter]->Fill(timediff);
	  }

	  // TIME DIFF per counter for all hits per event but with a DELTA cut
	  if (delta > -10. && delta < 22.){
	    time_ngcer_delta[counter]->Fill(timediff);
	  }

	  // fill in delta distr

	  delta_per_pmt[counter]->Fill(delta);

	  // all this is for stuff in the small peak for PMT 1
	  if(ngcer_counter[nhit] == 1 && timediff > -45. && timediff < -30. && ngcer_pulseamp[nhit] > 0.){
	    Double_t xcer = xfp - 89.1 * xpfp;
	    Double_t ycer = yfp - 89.1 * ypfp;
	    
	    // x vs y with AMP as weight
	    time_ngcer_pmt1_xy->Fill(ycer,xcer);
	    time_ngcer_pmt1_xy_w->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt1_dv->Divide(time_ngcer_pmt1_xy_w,time_ngcer_pmt1_xy);
	    
	    // x vs y with AMP as weight but with a DELTA cut
	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt1_xy_delta->Fill(ycer,xcer);
	      time_ngcer_pmt1_xy_w_delta->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt1_dv_delta->Divide(time_ngcer_pmt1_xy_w_delta,time_ngcer_pmt1_xy_delta);
	    }	    
	    // x vs xpfp with AMP as weight
	    time_ngcer_pmt1_xp->Fill(xpfp,xfp);
	    time_ngcer_pmt1_xp_w->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt1_dvxp->Divide(time_ngcer_pmt1_xp_w,time_ngcer_pmt1_xp);
	    
	    // x vs xpfp with AMP as weight but with DELTA cut
	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt1_xp_delta->Fill(xpfp,xfp);
	      time_ngcer_pmt1_xp_w_delta->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt1_dvxp_delta->Divide(time_ngcer_pmt1_xp_w_delta,time_ngcer_pmt1_xp_delta);
	    }
	   
	    // Multiplicity distr. for PMT 2 when PMT 1 had stuff in small peak
	    

	    // 1 D delta distribution in the small peak
	    time_ngcer_pmt1_1ddelta->Fill(delta);
	    
	  }


	  if(ngcer_counter[nhit] == 2 && timediff > -37. && timediff < -26. && ngcer_pulseamp[nhit] > 0.){
	    Double_t xcer = xfp - 89.1 * xpfp;
	    Double_t ycer = yfp - 89.1 * ypfp;

	    time_ngcer_pmt2_xy->Fill(ycer,xcer);
	    time_ngcer_pmt2_xy_w->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt2_dv->Divide(time_ngcer_pmt2_xy_w,time_ngcer_pmt2_xy);

	    // X vs Y with DELTA cut
	    if (delta > -10. && delta < 22.){
	      time_ngcer_pmt2_xy_delta->Fill(ycer,xcer);
	      time_ngcer_pmt2_xy_w_delta->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt2_dv_delta->Divide(time_ngcer_pmt2_xy_w_delta,time_ngcer_pmt2_xy_delta);
	    }
	    
	    time_ngcer_pmt2_xp->Fill(xpfp,xfp);
	    time_ngcer_pmt2_xp_w->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt2_dvxp->Divide(time_ngcer_pmt2_xp_w,time_ngcer_pmt2_xp);

	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt2_xp_delta->Fill(xpfp,xfp);
	      time_ngcer_pmt2_xp_w_delta->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt2_dvxp_delta->Divide(time_ngcer_pmt2_xp_w_delta,time_ngcer_pmt2_xp_delta);
	    }

	    time_ngcer_pmt2_1ddelta->Fill(delta);
	  }


	  if(ngcer_counter[nhit] == 3 && timediff > -38. && timediff < -28. && ngcer_pulseamp[nhit] > 0.){
	    Double_t xcer = xfp - 89.1 * xpfp;
	    Double_t ycer = yfp - 89.1 * ypfp;

	    time_ngcer_pmt3_xy->Fill(ycer,xcer);
	    time_ngcer_pmt3_xy_w->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt3_dv->Divide(time_ngcer_pmt3_xy_w,time_ngcer_pmt3_xy);

	    // X vs Y with DELTA
	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt3_xy_delta->Fill(ycer,xcer);
	      time_ngcer_pmt3_xy_w_delta->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt3_dv_delta->Divide(time_ngcer_pmt3_xy_w_delta,time_ngcer_pmt3_xy_delta);
	    }
	    
	    time_ngcer_pmt3_xp->Fill(xpfp,xfp);
	    time_ngcer_pmt3_xp_w->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt3_dvxp->Divide(time_ngcer_pmt3_xp_w,time_ngcer_pmt3_xp);

	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt3_xp_delta->Fill(xpfp,xfp);
	      time_ngcer_pmt3_xp_w_delta->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt3_dvxp_delta->Divide(time_ngcer_pmt3_xp_w_delta,time_ngcer_pmt3_xp_delta);
	    }

	    time_ngcer_pmt3_1ddelta->Fill(delta);
	  }
	  if(ngcer_counter[nhit] == 4 && timediff > -40. && timediff < -32. && ngcer_pulseamp[nhit] > 0.){
	    Double_t xcer = xfp - 89.1 * xpfp;
	    Double_t ycer = yfp - 89.1 * ypfp;

	    time_ngcer_pmt4_xy->Fill(ycer,xcer);
	    time_ngcer_pmt4_xy_w->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt4_dv->Divide(time_ngcer_pmt4_xy_w,time_ngcer_pmt4_xy);

	    // X vs Y with DELTA
	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt4_xy_delta->Fill(ycer,xcer);
	      time_ngcer_pmt4_xy_w_delta->Fill(ycer,xcer,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt4_dv_delta->Divide(time_ngcer_pmt4_xy_w,time_ngcer_pmt4_xy);
	    }

	    time_ngcer_pmt4_xp->Fill(xpfp,xfp);
	    time_ngcer_pmt4_xp_w->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt4_dvxp->Divide(time_ngcer_pmt4_xp_w,time_ngcer_pmt4_xp);

	    if(delta > -10. && delta < 22.){
	      time_ngcer_pmt4_xp_delta->Fill(xpfp,xfp);
	      time_ngcer_pmt4_xp_w_delta->Fill(xpfp,xfp,ngcer_pulseamp[nhit]);
	      time_ngcer_pmt4_dvxp_delta->Divide(time_ngcer_pmt4_xp_w_delta,time_ngcer_pmt4_xp_delta);
	    }

	    time_ngcer_pmt4_1ddelta->Fill(delta);
	    time_ngcer_pmt4_tref->Fill(reftime);



	  }
	  

	  if(ngcer_counter[nhit] == 1 && ngcer_mult[1] == 0. && ngcer_mult[2] == 0. && ngcer_mult[3] == 0. && ngcer_pulseamp[nhit] > 0.5){
	    Double_t xcerr = xfp - 89.1 * xpfp;
	    Double_t ycerr = yfp - 89.1 * ypfp;
	    time_ngcer_pmt1->Fill(timediff);
	    time_ngcer_pmt1_only_xy->Fill(ycerr,xcerr);
	    time_ngcer_pmt1_only_xy_w->Fill(ycerr,xcerr,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt1_only_dv->Divide(time_ngcer_pmt1_only_xy_w,time_ngcer_pmt1_only_xy);
	  }
	  if(ngcer_counter[nhit] == 2 && ngcer_mult[0] == 0. && ngcer_mult[2] == 0. && ngcer_mult[3] == 0. && ngcer_pulseamp[nhit] > 0.){
	    Double_t xcerr = xfp - 89.1 * xpfp;
	    Double_t ycerr = yfp - 89.1 * ypfp;
	    time_ngcer_pmt2->Fill(timediff);
	    time_ngcer_pmt2_only_xy->Fill(ycerr,xcerr);
	    time_ngcer_pmt2_only_xy_w->Fill(ycerr,xcerr,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt2_only_dv->Divide(time_ngcer_pmt2_only_xy_w,time_ngcer_pmt2_only_xy);
	  }
	  if(ngcer_counter[nhit] == 3 && ngcer_mult[0] == 0. && ngcer_mult[1] == 0. && ngcer_mult[3] == 0. && ngcer_pulseamp[nhit] > 0.5){
	    Double_t xcerr = xfp - 89.1 * xpfp;
	    Double_t ycerr = yfp - 89.1 * ypfp;
	    time_ngcer_pmt3->Fill(timediff);
	    time_ngcer_pmt3_only_xy->Fill(ycerr,xcerr);
	    time_ngcer_pmt3_only_xy_w->Fill(ycerr,xcerr,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt3_only_dv->Divide(time_ngcer_pmt3_only_xy_w,time_ngcer_pmt3_only_xy);
	  }

	  if(ngcer_counter[nhit] == 1 && ngcer_mult[1] == 0. && ngcer_mult[2] == 0.){
	    time_ngcer_pmt1_from4->Fill(timediff);

	  }

	  if(ngcer_counter[nhit] == 4 && ngcer_mult[0] == 0. && ngcer_mult[1] == 0. && ngcer_mult[2] == 0. && ngcer_pulseamp[nhit] > 0.5){
	    Double_t xcerr = xfp - 89.1 * xpfp;
	    Double_t ycerr = yfp - 89.1 * ypfp;
	    time_ngcer_pmt4->Fill(timediff);
	    time_ngcer_pmt4_only_xy->Fill(ycerr,xcerr);
	    time_ngcer_pmt4_only_xy_w->Fill(ycerr,xcerr,ngcer_pulseamp[nhit]);
	    time_ngcer_pmt4_only_dv->Divide(time_ngcer_pmt4_only_xy_w,time_ngcer_pmt4_only_xy);

	    if (timediff > -39. && timediff < -31.){
	    amp_pmt4only->Fill(ngcer_pulseamp[nhit]);
	    }
	    if(inside == 1){
	      time_ngcer_pmt4_inside->Fill(timediff);
	    }
	    
	  }


	  /*	  if (i > 98461 && i < 98469){

		  cout << "ev, hit, counter, pulsetime, starttime, timediff: " << i << " " << nhit << " " << ngcer_counter[nhit] << " " << ngcer_pulsetime[nhit] << " " << starttime << " " << timediff << endl; 

		  } */

	} // if counter = ..
	
      } // closes the counter loop
      
    } // closes the hit per entry loop
    
  } // closes the entry loop


  //  gStyle->SetOptStat(kTrue);
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  //  gStyle->SetPalette(kCMYK);
  //  gStyle->SetPalette(kBrownCyan);
  gStyle->SetPalette(kMint);
  //  gStyle->SetPalette(kCandy);
  
  gStyle->SetOptStat(0);


  // Pulse integral all and good

  TCanvas *c12 = new TCanvas("c12","",900,900);
  c12->Divide(2,2);
  for(Int_t mmm = 1; mmm < 5; mmm++){
    c12->cd(mmm);
    formatit_1h(int_ngcer[mmm],"ADC Pulse Integral (pC)","Events","all hits");
    formatit_1h_same(int_goodngcer[mmm],"","","good hits");
  }
  c12->Print("ngcer_int.png");
 
  // llllllllllllllllllllllllllllllllllllllll
 
   // Pulse amplitude all and good

  TCanvas *c13 = new TCanvas("c13","",900,900);
  c13->Divide(2,2);
  for(Int_t mmm = 1; mmm < 5; mmm++){
    c13->cd(mmm);
    formatit_1h(amp_ngcer[mmm],"ADC Pulse Amplitude (mV)","Events","all hits");
    formatit_1h_same(amp_goodngcer[mmm],"","","good hits");
    if (mmm == 4){
      //      formatit_1h_same4(amp_pmt4only,"","","PMT 4 only");
      //     formatit_1h_same4(time_ngcer_pmt1_from4,"","","PMT 1, 4");
     }
  }
  c13->Print("ngcer_amp.png");
 
  // llllllllllllllllllllllllllllllllllllllll

   // Pulse time all and good

  TCanvas *c14 = new TCanvas("c14","",900,900);
  c14->Divide(2,2);
  for(Int_t mmm = 1; mmm < 5; mmm++){
    c14->cd(mmm);
    formatit_1h(time_ngcer[mmm],"StartTime - ADC Pulse Time (ns)","Events","all hits");
    formatit_1h_same3(time_ngcer_calo[mmm],"","","cal > 0.7");
    formatit_1h_same2(time_ngcer_reftime[mmm],"","","refamp cut");
    formatit_1h_same4(time_ngcer_delta[mmm],"","","delta cut");
    //    formatit_1h_same5(time_ngcer_calo_reftime[mmm],"","","reftime cut");

    if (mmm == 1){
     formatit_1h_same(time_ngcer_pmt1,"","","PMT 1 only");
     }
    if (mmm == 2){
     formatit_1h_same(time_ngcer_pmt2,"","","PMT 2 only");
     }
    if (mmm == 3){
     formatit_1h_same(time_ngcer_pmt3,"","","PMT 3 only");
     }
    if (mmm == 4){
     formatit_1h_same(time_ngcer_pmt4,"","","PMT 4 only");
     //     formatit_1h_same4(time_ngcer_pmt4_inside,"","","PMT 4, inside");
     }

  }
  c14->Print("ngcer_time_reftime.png"); 
 
  // llllllllllllllllllllllllllllllllllllllll

   // Y vs X for small peak

  TCanvas *c15 = new TCanvas("c15","",900,900);
  c15->Divide(2,2);
  c15->cd(1);
  formatit_2dd(time_ngcer_pmt1_dv,"PMT 1 (small peak)","Y at Cher (cm)","X at Cher (cm)",0);
  c15->cd(2);
  formatit_2dd(time_ngcer_pmt2_dv,"PMT 2 (small peak)","Y at Cher (cm)","X at Cher (cm)",0);
  c15->cd(3);
  formatit_2dd(time_ngcer_pmt3_dv,"PMT 3 (small peak)","Y at Cher (cm)","X at Cher (cm)",0);
  c15->cd(4);
  formatit_2dd(time_ngcer_pmt4_dv,"PMT 4 (small peak)","Y at Cher (cm)","X at Cher (cm)",0);

  c15->Print("ngcer_xycer_smallpeak.png");

   // Y vs X for small peak with delta cut

  TCanvas *c25 = new TCanvas("c25","",900,900);
  c25->Divide(2,2);
  c25->cd(1);
  formatit_2dd(time_ngcer_pmt1_dv_delta,"PMT 1 (small peak), delta cut","Y at Cher (cm)","X at Cher (cm)",0);
  c25->cd(2);
  formatit_2dd(time_ngcer_pmt2_dv_delta,"PMT 2 (small peak), delta cut","Y at Cher (cm)","X at Cher (cm)",0);
  c25->cd(3);
  formatit_2dd(time_ngcer_pmt3_dv_delta,"PMT 3 (small peak), delta cut","Y at Cher (cm)","X at Cher (cm)",0);
  c25->cd(4);
  formatit_2dd(time_ngcer_pmt4_dv_delta,"PMT 4 (small peak), delta cut","Y at Cher (cm)","X at Cher (cm)",0);

  c25->Print("ngcer_xycer_smallpeak_deltacut.png");


   // Y vs X for small peak with delta cut

  TCanvas *c35 = new TCanvas("c35","",900,900);
  c35->Divide(2,2);
  c35->cd(1);
  formatit_2dd(time_ngcer_pmt1_only_dv,"PMT 1 only","Y at Cher (cm)","X at Cher (cm)",0);
  c35->cd(2);
  formatit_2dd(time_ngcer_pmt2_only_dv,"PMT 2 only","Y at Cher (cm)","X at Cher (cm)",0);
  c35->cd(3);
  formatit_2dd(time_ngcer_pmt3_only_dv,"PMT 3 only","Y at Cher (cm)","X at Cher (cm)",0);
  c35->cd(4);
  formatit_2dd(time_ngcer_pmt4_only_dv,"PMT 4 only","Y at Cher (cm)","X at Cher (cm)",0);

  c35->Print("ngcer_xycer_perpmtonly.png");


   // delta for small peak

  TCanvas *c16 = new TCanvas("c16","",900,900);
  c16->Divide(2,2);
  c16->cd(1);
  formatit_1h(delta_per_pmt[1],"Delta (%)","Events","PMT 1, all hits/events");
  formatit_1h_same(time_ngcer_pmt1_1ddelta,"","","PMT 1, small peak");
  c16->cd(2);
  formatit_1h(delta_per_pmt[2],"Delta (%)","Events","PMT 2, all hits/events");
  formatit_1h_same(time_ngcer_pmt2_1ddelta,"","","PMT 2, small peak");
  c16->cd(3);
  formatit_1h(delta_per_pmt[3],"Delta (%)","Events","PMT 3, all hits/events");
  formatit_1h_same(time_ngcer_pmt3_1ddelta,"","","PMT 3, small peak");
  c16->cd(4);
  formatit_1h(delta_per_pmt[4],"Delta (%)","Events","PMT 4, all hits/events");
  formatit_1h_same(time_ngcer_pmt4_1ddelta,"","","PMT 4, small peak");

  c16->Print("ngcer_delta_smallpeak_andall.png");

  // ---------------------------------------

  TCanvas *c17 = new TCanvas("c17","",900,900);
  c17->Divide(2,2);

  c17->cd(1);
  formatit_2dd(time_ngcer_pmt1_dvxp,"PMT 1 (small peak), no cut","XP_FP (rad)","X at Cher (cm)",0);
  c17->cd(2);
  formatit_2dd(time_ngcer_pmt2_dvxp,"PMT 2 (small peak), no cut","XP_FP (rad)","X at Cher (cm)",0);
  c17->cd(3);
  formatit_2dd(time_ngcer_pmt3_dvxp,"PMT 3 (small peak), no cut","XP_FP (rad)","X at Cher (cm)",0);
  c17->cd(4);
  formatit_2dd(time_ngcer_pmt4_dvxp,"PMT 4 (small peak), no cut","XP_FP (rad)","X at Cher (cm)",0);

  c17->Print("ngcer_xpfpcer_smallpeak.png");

  //////////////////////////

  /*  TCanvas *c18 = new TCanvas("c18","",900,900);
  c18->Divide(2,2);
  c18->cd(4);
  formatit_1h(time_ngcer_pmt4_tref,"","","PMT 1, tdiff: (-42,-31)"); */

 //////////////////////////

  TCanvas *c19 = new TCanvas("c19","",900,900);
  c19->Divide(2,2);

  c19->cd(1);
  formatit_2dd(time_ngcer_pmt1_dvxp_delta,"PMT 1 (small peak), delta cut","XP_FP (rad)","X at Cher (cm)",0);
  c19->cd(2);
  formatit_2dd(time_ngcer_pmt2_dvxp_delta,"PMT 2 (small peak), delta cut","XP_FP (rad)","X at Cher (cm)",0);
  c19->cd(3);
  formatit_2dd(time_ngcer_pmt3_dvxp_delta,"PMT 3 (small peak), delta cut","XP_FP (rad)","X at Cher (cm)",0);
  c19->cd(4);
  formatit_2dd(time_ngcer_pmt4_dvxp_delta,"PMT 4 (small peak), delta cut","XP_FP (rad)","X at Cher (cm)",0);

  c19->Print("ngcer_xpfpcer_delta_smallpeak.png");




  return 0;
  
}
 
