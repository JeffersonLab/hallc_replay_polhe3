#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <functional>
#include <numeric>


#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TF1.h"
#include "TPad.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

void ref_time_plots_HMS()

{
   TFile *fileIN = new TFile("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_all_04331_200000.root");
  // TFile *fileIN = new TFile("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_default_11398_-1.root"); //D2N full replay default

  auto T1 =  (TTree*)fileIN->Get("T");

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  //gStyle->SetStatY(0.9);
  //gStyle->SetStatX(0.9);
  // gStyle->SetStatW(0.);
  // gStyle->SetStatH(0.1); 
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.015);


  Int_t hFADC_cut=3400;//3400 for A1n
  Int_t  hT1_cut=1400;//1400 for A1n
  Int_t  hDCREF_cut=20000;

 Int_t  hFADC_y_max=40000;
 Int_t  hT1_y_max=40000;
 Int_t  hDCREF_y_max=100000;

  // Create lines to show the reference time cuts
  TLine *hFADC_line=new TLine(hFADC_cut,0,hFADC_cut,hFADC_y_max); 
  TLine *hT1_line=new TLine(hT1_cut,0,hT1_cut,hT1_y_max); 
  TLine *hDCREF_line=new TLine(hDCREF_cut,0,hDCREF_cut,hDCREF_y_max);

  hFADC_line->SetLineStyle(9);
  hFADC_line->SetLineColor(kRed);
  hFADC_line->SetLineWidth(2);
  hT1_line->SetLineStyle(9);
  hT1_line->SetLineColor(kRed);
  hT1_line->SetLineWidth(2);
  hDCREF_line->SetLineStyle(9);
  hDCREF_line->SetLineColor(kRed);
  hDCREF_line->SetLineWidth(2);
  

  
  auto c1= new TCanvas("c1","HMS Reference Times Plots FADC (CAL,CER,HODO)");
  gPad->SetLogy();

  TH1F *hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut=new TH1F("hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut","hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1-3",200,0,5000);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseTimeRaw>>hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut");
  
  TH1F *hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1=new TH1F("hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1","hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1-3",200,0,5000);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseTimeRaw>>hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1","T.hms.hFADC_TREF_ROC1_adcMultiplicity==1");
  
  TH1F *hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2=new TH1F("hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2","hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1-3",200,0,5000);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseTimeRaw>>hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2","T.hms.hFADC_TREF_ROC1_adcMultiplicity==2");
  
  TH1F *hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3=new TH1F("hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3","hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1-3",200,0,5000);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseTimeRaw>>hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3","T.hms.hFADC_TREF_ROC1_adcMultiplicity==3"); 
   
  
  hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut->SetLineColor(1);
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1->SetLineColor(2);
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2->SetLineColor(3);
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3->SetLineColor(4);
  hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut->Draw();
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1->Draw("SAME");
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2->Draw("SAME");
  hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3->Draw("SAME");

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  // legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
   legend->AddEntry(hFADC_TREF_ROC1_adcPulseTimeRaw_no_cut,"no multiplicity cuts","l");
   legend->AddEntry(hFADC_TREF_ROC1_adcPulseTimeRaw_cut_1,"T.hms.hFADC_TREF_ROC1_adcMultiplicity==1","l");
   legend->AddEntry(hFADC_TREF_ROC1_adcPulseTimeRaw_cut_2,"T.hms.hFADC_TREF_ROC1_adcMultiplicity==2","l");
   legend->AddEntry(hFADC_TREF_ROC1_adcPulseTimeRaw_cut_3,"T.hms.hFADC_TREF_ROC1_adcMultiplicity==3","l");
   legend->Draw();
hFADC_line->Draw("SAME");
  c1->Update();
  
  
  /*
  auto c2= new TCanvas("c1","HMS Reference Times Plots TDC (HODO)");
  c2->Divide(2,2);  
  c2->cd(1);
  gPad->SetLogy();
  T1->Draw("T.hms.hT1_tdcMultiplicity>>hT1_tdcMultiplicity");
  
  c2->cd(2);	
  T1->Draw("T.hms.hT1_tdcTimeRaw>>hT1_tdcTimeRaw");
 

  c2->cd(3);
  T1->Draw("T.hms.hT2_tdcMultiplicity>>hT2_tdcMultiplicity");
  
  c2->cd(4);
  T1->Draw("T.hms.hT2_tdcTimeRaw>>hT2_tdcTimeRaw");
 
  c2->Modified();
  c2->Update();
   */
  
  auto c2= new TCanvas("c2","HMS Reference Times Plots TDC (HODO)");

  gPad->SetLogy();
  TH1F *hT2_tdcTimeRaw_cut_1=new TH1F("hT2_tdcTimeRaw_cut_1","hT2_tdcTimeRaw_cut_1-3",200,0,2500);
  T1->Draw("T.hms.hT2_tdcTimeRaw>>hT2_tdcTimeRaw_cut_1","T.hms.hT2_tdcMultiplicity==1");
  
  TH1F *hT2_tdcTimeRaw_cut_2=new TH1F("hT2_tdcTimeRaw_cut_2","hT2_tdcTimeRaw_cut_1-3",200,0,2500);
  T1->Draw("T.hms.hT2_tdcTimeRaw>>hT2_tdcTimeRaw_cut_2","T.hms.hT2_tdcMultiplicity==2");
  
  TH1F *hT2_tdcTimeRaw_cut_3=new TH1F("hT2_tdcTimeRaw_cut_3","hT2_tdcTimeRaw_cut_1-3",200,0,2500);
  T1->Draw("T.hms.hT2_tdcTimeRaw>>hT2_tdcTimeRaw_cut_3","T.hms.hT2_tdcMultiplicity==3");

  hT2_tdcTimeRaw_cut_1->SetLineColor(2);
  hT2_tdcTimeRaw_cut_2->SetLineColor(3);
  hT2_tdcTimeRaw_cut_3->SetLineColor(4);
  hT2_tdcTimeRaw_cut_1->Draw();
  hT2_tdcTimeRaw_cut_2->Draw("SAME");
  hT2_tdcTimeRaw_cut_3->Draw("SAME");
  auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
 
   legend2->AddEntry(hT2_tdcTimeRaw_cut_1,"T.hms.hT2_tdcMultiplicity==1","l");
   legend2->AddEntry(hT2_tdcTimeRaw_cut_2,"T.hms.hT2_tdcMultiplicity==2","l");
   legend2->AddEntry(hT2_tdcTimeRaw_cut_3,"T.hms.hT2_tdcMultiplicity==3","l");
   legend2->Draw();

  hT1_line->Draw("SAME");
  hT2_tdcTimeRaw_cut_1->SetAxisRange(0.1,2500,"X");
  hT2_tdcTimeRaw_cut_1->SetAxisRange(0.1,1000000,"Y");
  c2->Modified();
  c2->Update();
  

 auto c3= new TCanvas("c3","HMS Reference Times Plots TDC (DC)");

  c3->Divide(2,1);
  c3->cd(1);

  gPad->SetLogy();
  TH1F *hDCREF1_tdcTimeRaw=new TH1F("hDCREF1_tdcTimeRaw","hDCREF1-5_tdcTimeRaw",1000,0,24000);
  TH1F *hDCREF2_tdcTimeRaw=new TH1F("hDCREF2_tdcTimeRaw","hDCREF1-5_tdcTimeRaw",1000,0,24000);
  TH1F *hDCREF3_tdcTimeRaw=new TH1F("hDCREF3_tdcTimeRaw","hDCREF1-5_tdcTimeRaw",1000,0,24000);
  TH1F *hDCREF4_tdcTimeRaw=new TH1F("hDCREF4_tdcTimeRaw","hDCREF1-5_tdcTimeRaw",1000,0,24000);
  TH1F *hDCREF5_tdcTimeRaw=new TH1F("hDCREF5_tdcTimeRaw","hDCREF1-5_tdcTimeRaw",1000,0,24000);
 
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw");
  T1->Draw("T.hms.hDCREF2_tdcTimeRaw>>hDCREF1_tdcTimeRaw");
  T1->Draw("T.hms.hDCREF3_tdcTimeRaw>>hDCREF1_tdcTimeRaw");
  T1->Draw("T.hms.hDCREF4_tdcTimeRaw>>hDCREF1_tdcTimeRaw");
  T1->Draw("T.hms.hDCREF5_tdcTimeRaw>>hDCREF5_tdcTimeRaw");
 
  hDCREF1_tdcTimeRaw->SetAxisRange(0.1,24000,"X");
  hDCREF1_tdcTimeRaw->SetAxisRange(0.1,100000000,"Y");

  hDCREF1_tdcTimeRaw->SetLineColor(45);
  hDCREF2_tdcTimeRaw->SetLineColor(46);
  hDCREF3_tdcTimeRaw->SetLineColor(47);
  hDCREF4_tdcTimeRaw->SetLineColor(48);
  hDCREF5_tdcTimeRaw->SetLineColor(49);
  

  hDCREF1_tdcTimeRaw->Draw();
  hDCREF2_tdcTimeRaw->Draw("SAME");
  hDCREF3_tdcTimeRaw->Draw("SAME");
  hDCREF4_tdcTimeRaw->Draw("SAME");
  hDCREF5_tdcTimeRaw->Draw("SAME");
 
  hDCREF_line->Draw("SAME");

  auto legend3 = new TLegend(0.1,0.6,0.2,0.9);
   legend3->AddEntry(hDCREF1_tdcTimeRaw,"hDCREF1","l");
   legend3->AddEntry(hDCREF2_tdcTimeRaw,"hDCREF2","l");
   legend3->AddEntry(hDCREF3_tdcTimeRaw,"hDCREF3","l");
   legend3->AddEntry(hDCREF4_tdcTimeRaw,"hDCREF4","l");
   legend3->AddEntry(hDCREF5_tdcTimeRaw,"hDCREF5","l");
  
   legend3->Draw();
   c3->Update();
  
  
  c3->cd(2);

  gPad->SetLogy();
  TH1F *hDCREF1_tdcMultiplicity=new TH1F("hDCREF1_tdcMultiplicity","hDCREF1_tdcMultiplicity",100,0,4);
 
  T1->Draw("T.hms.hDCREF1_tdcMultiplicity>>hDCREF1_tdcMultiplicity");
 
  hDCREF1_tdcMultiplicity->SetLineColor(2);
  hDCREF1_tdcMultiplicity->Draw();
 

  c3->Modified();
  c3->Update();
  
  
 
  auto c4= new TCanvas("c4","HMS Reference Times Plots TDC (DC)");
  gPad->SetLogy();
  TH1F *hDCREF1_tdcTimeRaw_cut_1=new TH1F("hDCREF1_tdcTimeRaw_cut_1","hDCREF1-5_tdcTimeRaw_cut_1-3",1000,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_1","T.hms.hDCREF1_tdcMultiplicity==1");
  TH1F *hDCREF1_tdcTimeRaw_cut_2=new TH1F("hDCREF1_tdcTimeRaw_cut_2","hDCREF1-5_tdcTimeRaw_cut_1-3",1000,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_2","T.hms.hDCREF1_tdcMultiplicity==2");
  TH1F *hDCREF1_tdcTimeRaw_cut_3=new TH1F("hDCREF1_tdcTimeRaw_cut_3","hDCREF1-5_tdcTimeRaw_cut_1-3",1000,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_3","T.hms.hDCREF1_tdcMultiplicity==3");


  hDCREF1_tdcTimeRaw_cut_1->SetAxisRange(0.1,24000,"X");
  hDCREF1_tdcTimeRaw_cut_1->SetAxisRange(0.1,100000000,"Y");
  hDCREF1_tdcTimeRaw_cut_1->SetLineColor(1);
  hDCREF1_tdcTimeRaw_cut_2->SetLineColor(2);
  hDCREF1_tdcTimeRaw_cut_3->SetLineColor(3);
  hDCREF1_tdcTimeRaw_cut_1->Draw();
  hDCREF1_tdcTimeRaw_cut_2->Draw("SAME");
  hDCREF1_tdcTimeRaw_cut_3->Draw("SAME");
  hDCREF_line->Draw("SAME");

  auto legend4 = new TLegend(0.1,0.7,0.35,0.9);
   legend4->AddEntry(hDCREF1_tdcTimeRaw_cut_1,"T.hms.hDCREF1_tdcMultiplicity==1","l");  
   legend4->AddEntry(hDCREF1_tdcTimeRaw_cut_2,"T.hms.hDCREF1_tdcMultiplicity==2","l");
   legend4->AddEntry(hDCREF1_tdcTimeRaw_cut_3,"T.hms.hDCREF1_tdcMultiplicity==3","l"); 
   legend4->Draw();

   c4->Modified();
   c4->Update(); 

  
 
  /*
  
 
  auto T2 =  (TTree*)fileIN->Get("TSH;1");
  auto c1= new TCanvas("c1","HMS Scaler Rate");
  T2->Draw("H.pTRIG1.scalerRate>>scalerRate");
  */
  return 0;
  
}
