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

void ref_time_plots_SHMS()
{
Int_t nbin1=500; //200 before
Int_t nbin2=500;  //100 before
Int_t nbin3=1000;

 TFile *fileIN = new TFile("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_11538_200000.root"); // A1N
//TFile *fileIN = new TFile("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_default_11369_-1.root"); //D2N full replay default
  //TFile *fileIN = new TFile("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_10886_-1.root"); //D2N full replay full branch
  auto T1 =  (TTree*)fileIN->Get("T");

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  //gStyle->SetStatY(0.9);
  //gStyle->SetStatX(0.9);
  // gStyle->SetStatW(0.);
  // gStyle->SetStatH(0.1); 
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.015);


  Int_t pFADC_cut=4200;//4200 for A1n
  Int_t  pT1_cut=3400;//3400 for A1n
 Int_t  pDCREF_cut=14400;

 Int_t  pFADC_y_max=40000;
 Int_t  pT1_y_max=40000;
 Int_t  pDCREF_y_max=100000;



  // Create lines to show the reference time cuts
  TLine *pFADC_line=new TLine(pFADC_cut,0,pFADC_cut,pFADC_y_max); 
  TLine *pT1_line=new TLine(pT1_cut,0,pT1_cut,pT1_y_max); 
  TLine *pDCREF_line=new TLine(pDCREF_cut,0,pDCREF_cut,pDCREF_y_max);

  pFADC_line->SetLineStyle(9);
  pFADC_line->SetLineColor(kRed);
  pFADC_line->SetLineWidth(2);
  pT1_line->SetLineStyle(9);
  pT1_line->SetLineColor(kRed);
  pT1_line->SetLineWidth(2);
  pDCREF_line->SetLineStyle(9);
  pDCREF_line->SetLineColor(kRed);
  pDCREF_line->SetLineWidth(2);
  
  
  auto c1= new TCanvas("c1","SHMS Reference Times Plots FADC (CAL,CER,HODO)");
  gPad->SetLogy();
  
  TH1F *pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut=new TH1F("pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut","pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1-3",nbin1,0,6000);
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw>>pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut");
  
  TH1F *pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1=new TH1F("pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1","pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1-3",nbin1,0,6000);
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw>>pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1","T.shms.pFADC_TREF_ROC2_adcMultiplicity==1");
  
  TH1F *pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2=new TH1F("pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2","pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1-3",nbin1,0,6000);
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw>>pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2","T.shms.pFADC_TREF_ROC2_adcMultiplicity==2");
  
  TH1F *pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3=new TH1F("pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3","pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1-3",nbin1,0,6000);
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw>>pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3","T.shms.pFADC_TREF_ROC2_adcMultiplicity==3");  
  
  pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut->SetLineColor(1); 
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1->SetLineColor(2);
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2->SetLineColor(3);
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3->SetLineColor(4);
  pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut->Draw();
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1->Draw("SAME");
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2->Draw("SAME");
  pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3->Draw("SAME");
  pFADC_line->Draw("SAME");

  auto legend = new TLegend(0.1,0.7,0.4,0.9);
  // legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
   legend->AddEntry(pFADC_TREF_ROC2_adcPulseTimeRaw_no_cut,"no multiplicity cuts","l");
   legend->AddEntry(pFADC_TREF_ROC2_adcPulseTimeRaw_cut_1,"T.shms.pFADC_TREF_ROC2_adcMultiplicity==1","l");
   legend->AddEntry(pFADC_TREF_ROC2_adcPulseTimeRaw_cut_2,"T.shms.pFADC_TREF_ROC2_adcMultiplicity==2","l");
   legend->AddEntry(pFADC_TREF_ROC2_adcPulseTimeRaw_cut_3,"T.shms.pFADC_TREF_ROC2_adcMultiplicity==3","l");
   legend->Draw();
   
   c1->Update();
   



 
  auto c2= new TCanvas("c2","SHMS Reference Times Plots TDC (HODO)");
  c2->Divide(2,1);
  c2->cd(1);
  gPad->SetLogy();
  TH1F *pT1_tdcTimeRaw_cut_1=new TH1F("pT1_tdcTimeRaw_cut_1","pT1_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT1_tdcTimeRaw>>pT1_tdcTimeRaw_cut_1","T.shms.pT1_tdcMultiplicity==1");
  
  TH1F *pT1_tdcTimeRaw_cut_2=new TH1F("pT1_tdcTimeRaw_cut_2","pT1_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT1_tdcTimeRaw>>pT1_tdcTimeRaw_cut_2","T.shms.pT1_tdcMultiplicity==2");
  
  TH1F *pT1_tdcTimeRaw_cut_3=new TH1F("pT1_tdcTimeRaw_cut_3","pT1_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT1_tdcTimeRaw>>pT1_tdcTimeRaw_cut_3","T.shms.pT1_tdcMultiplicity==3");

  pT1_tdcTimeRaw_cut_1->SetLineColor(2);
  pT1_tdcTimeRaw_cut_2->SetLineColor(3);
  pT1_tdcTimeRaw_cut_3->SetLineColor(4);
  pT1_tdcTimeRaw_cut_1->Draw();
  pT1_tdcTimeRaw_cut_2->Draw("SAME");
  pT1_tdcTimeRaw_cut_3->Draw("SAME");
  pT1_line->Draw("SAME");
 auto legend1 = new TLegend(0.1,0.7,0.4,0.9);
 
   legend1->AddEntry(pT1_tdcTimeRaw_cut_1,"T.shms.pT1_tdcMultiplicity==1","l");
   legend1->AddEntry(pT1_tdcTimeRaw_cut_2,"T.shms.pT1_tdcMultiplicity==2","l");
   legend1->AddEntry(pT1_tdcTimeRaw_cut_3,"T.shms.pT1_tdcMultiplicity==3","l");
   legend1->Draw();
  
 pT1_tdcTimeRaw_cut_1->SetAxisRange(0.1,5000,"X");
  pT1_tdcTimeRaw_cut_1->SetAxisRange(0.1,10000000,"Y");

  c2->cd(2);
  gPad->SetLogy();
  TH1F *pT2_tdcTimeRaw_cut_1=new TH1F("pT2_tdcTimeRaw_cut_1","pT2_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT2_tdcTimeRaw>>pT2_tdcTimeRaw_cut_1","T.shms.pT2_tdcMultiplicity==1");
  
  TH1F *pT2_tdcTimeRaw_cut_2=new TH1F("pT2_tdcTimeRaw_cut_2","pT2_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT2_tdcTimeRaw>>pT2_tdcTimeRaw_cut_2","T.shms.pT2_tdcMultiplicity==2");
  
  TH1F *pT2_tdcTimeRaw_cut_3=new TH1F("pT2_tdcTimeRaw_cut_3","pT2_tdcTimeRaw_cut_1-3",nbin2,0,5000);
  T1->Draw("T.shms.pT2_tdcTimeRaw>>pT2_tdcTimeRaw_cut_3","T.shms.pT2_tdcMultiplicity==3");

  pT2_tdcTimeRaw_cut_1->SetLineColor(2);
  pT2_tdcTimeRaw_cut_2->SetLineColor(3);
  pT2_tdcTimeRaw_cut_3->SetLineColor(4);
  pT2_tdcTimeRaw_cut_1->Draw();
  pT2_tdcTimeRaw_cut_2->Draw("SAME");
  pT2_tdcTimeRaw_cut_3->Draw("SAME");
  pT1_line->Draw("SAME");
  auto legend2 = new TLegend(0.1,0.7,0.4,0.9);
 
   legend2->AddEntry(pT2_tdcTimeRaw_cut_1,"T.shms.pT2_tdcMultiplicity==1","l");
   legend2->AddEntry(pT2_tdcTimeRaw_cut_2,"T.shms.pT2_tdcMultiplicity==2","l");
   legend2->AddEntry(pT2_tdcTimeRaw_cut_3,"T.shms.pT2_tdcMultiplicity==3","l");
   legend2->Draw();
 pT2_tdcTimeRaw_cut_1->SetAxisRange(0.1,5000,"X");
  pT2_tdcTimeRaw_cut_1->SetAxisRange(0.1,10000000,"Y");
  c2->Update();


  auto c3= new TCanvas("c3","SHMS Reference Times Plots TDC (DC)");
  gPad->SetLogy();
  
  TH1F *pDCREF1_tdcTimeRaw=new TH1F("pDCREF1_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF1_tdcTimeRaw>>pDCREF1_tdcTimeRaw_cut_1");
  TH1F *pDCREF2_tdcTimeRaw=new TH1F("pDCREF2_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF2_tdcTimeRaw>>pDCREF2_tdcTimeRaw");
  TH1F *pDCREF3_tdcTimeRaw=new TH1F("pDCREF3_tdcTimeRaw","pDCREF3-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF3_tdcTimeRaw>>pDCREF3_tdcTimeRaw");
  TH1F *pDCREF4_tdcTimeRaw=new TH1F("pDCREF4_tdcTimeRaw","pDCREF4-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF4_tdcTimeRaw>>pDCREF4_tdcTimeRaw");
  TH1F *pDCREF5_tdcTimeRaw=new TH1F("pDCREF5_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF5_tdcTimeRaw>>pDCREF5_tdcTimeRaw");
  TH1F *pDCREF6_tdcTimeRaw=new TH1F("pDCREF6_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF6_tdcTimeRaw>>pDCREF6_tdcTimeRaw");
  TH1F *pDCREF7_tdcTimeRaw=new TH1F("pDCREF7_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF7_tdcTimeRaw>>pDCREF7_tdcTimeRaw");
  TH1F *pDCREF8_tdcTimeRaw=new TH1F("pDCREF8_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF8_tdcTimeRaw>>pDCREF8_tdcTimeRaw");
  TH1F *pDCREF9_tdcTimeRaw=new TH1F("pDCREF9_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF9_tdcTimeRaw>>pDCREF9_tdcTimeRaw");
  TH1F *pDCREF10_tdcTimeRaw=new TH1F("pDCREF10_tdcTimeRaw","pDCREF1-10_tdcTimeRaw",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF10_tdcTimeRaw>>pDCREF10_tdcTimeRaw");

  pDCREF1_tdcTimeRaw->SetAxisRange(0.1,17000,"X");
  pDCREF1_tdcTimeRaw->SetAxisRange(0.1,1000000,"Y");

  pDCREF1_tdcTimeRaw->SetLineColor(45);
  pDCREF2_tdcTimeRaw->SetLineColor(46);
  pDCREF3_tdcTimeRaw->SetLineColor(47);
  pDCREF4_tdcTimeRaw->SetLineColor(48);
  pDCREF5_tdcTimeRaw->SetLineColor(49);
  pDCREF6_tdcTimeRaw->SetLineColor(40);
  pDCREF7_tdcTimeRaw->SetLineColor(41);
  pDCREF8_tdcTimeRaw->SetLineColor(42);
  pDCREF9_tdcTimeRaw->SetLineColor(43);
  pDCREF10_tdcTimeRaw->SetLineColor(44);

  pDCREF1_tdcTimeRaw->Draw();
  pDCREF2_tdcTimeRaw->Draw("SAME");
  pDCREF3_tdcTimeRaw->Draw("SAME");
  pDCREF4_tdcTimeRaw->Draw("SAME");
  pDCREF6_tdcTimeRaw->Draw("SAME");
  pDCREF7_tdcTimeRaw->Draw("SAME");
  pDCREF8_tdcTimeRaw->Draw("SAME");
  pDCREF9_tdcTimeRaw->Draw("SAME");
  pDCREF10_tdcTimeRaw->Draw("SAME");
  pDCREF_line->Draw("SAME");

  auto legend3 = new TLegend(0.1,0.6,0.2,0.9);
   legend3->AddEntry(pDCREF1_tdcTimeRaw,"pDCREF1","l");
   legend3->AddEntry(pDCREF2_tdcTimeRaw,"pDCREF2","l");
   legend3->AddEntry(pDCREF3_tdcTimeRaw,"pDCREF3","l");
   legend3->AddEntry(pDCREF4_tdcTimeRaw,"pDCREF4","l");
   legend3->AddEntry(pDCREF5_tdcTimeRaw,"pDCREF5","l");
   legend3->AddEntry(pDCREF6_tdcTimeRaw,"pDCREF6","l");
   legend3->AddEntry(pDCREF7_tdcTimeRaw,"pDCREF7","l");
   legend3->AddEntry(pDCREF8_tdcTimeRaw,"pDCREF8","l");
   legend3->AddEntry(pDCREF9_tdcTimeRaw,"pDCREF9","l");
   legend3->AddEntry(pDCREF10_tdcTimeRaw,"pDCREF10","l");
   legend3->Draw();
   c3->Update(); 


  auto c4= new TCanvas("c4","SHMS Reference Times Plots TDC (DC)");
  gPad->SetLogy();

  TH1F *pDCREF1_tdcTimeRaw_cut_1=new TH1F("pDCREF1_tdcTimeRaw_cut_1","pDCREF1_tdcTimeRaw_cut_1-3",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF1_tdcTimeRaw>>pDCREF1_tdcTimeRaw_cut_1","T.shms.pDCREF1_tdcMultiplicity==1");
  TH1F *pDCREF1_tdcTimeRaw_cut_2=new TH1F("pDCREF1_tdcTimeRaw_cut_2","pDCREF1_tdcTimeRaw_cut_1-3",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF1_tdcTimeRaw>>pDCREF1_tdcTimeRaw_cut_2","T.shms.pDCREF1_tdcMultiplicity==2");
  TH1F *pDCREF1_tdcTimeRaw_cut_3=new TH1F("pDCREF1_tdcTimeRaw_cut_3","pDCREF1_tdcTimeRaw_cut_1-3",nbin3,0,17000);
  T1->Draw("T.shms.pDCREF1_tdcTimeRaw>>pDCREF1_tdcTimeRaw_cut_3","T.shms.pDCREF1_tdcMultiplicity==3");

  pDCREF1_tdcTimeRaw_cut_1->SetAxisRange(0.1,17000,"X");
  pDCREF1_tdcTimeRaw_cut_1->SetAxisRange(0.1,100000000,"Y");
  pDCREF1_tdcTimeRaw_cut_1->SetLineColor(1);
  pDCREF1_tdcTimeRaw_cut_2->SetLineColor(2);
  pDCREF1_tdcTimeRaw_cut_3->SetLineColor(3);
  pDCREF1_tdcTimeRaw_cut_1->Draw();
  pDCREF1_tdcTimeRaw_cut_2->Draw("SAME");
  pDCREF1_tdcTimeRaw_cut_3->Draw("SAME");
  pDCREF_line->Draw("SAME");

  auto legend4 = new TLegend(0.1,0.7,0.35,0.9);
   legend4->AddEntry(pDCREF1_tdcTimeRaw_cut_1,"T.shms.pDCREF1_tdcMultiplicity==1","l");
   legend4->AddEntry(pDCREF1_tdcTimeRaw_cut_2,"T.shms.pDCREF1_tdcMultiplicity==2","l");
   legend4->AddEntry(pDCREF1_tdcTimeRaw_cut_3,"T.shms.pDCREF1_tdcMultiplicity==3","l");
   legend4->Draw();

   c4->Update(); 
 return 0;
  
}





















































  /*  
  c1->Divide(2,1);
  c1->cd(1);	
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw>>pFADC_TREF_ROC2_adcPulseTimeRaw");

  c1->cd(2);
  T1->Draw("T.shms.pFADC_TREF_ROC2_adcMultiplicity>>pFADC_TREF_ROC2_adcMultiplicity");
  
  c1->cd(3);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseTime>>hFADC_TREF_ROC1_adcPulseTime");

  c1->cd(4);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseAmpRaw>>hFADC_TREF_ROC1_adcPulseAmpRaw");

  c1->cd(5);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseAmp>>hFADC_TREF_ROC1_adcPulseAmp");
 
  c1->cd(6);	
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseIntRaw>>hFADC_TREF_ROC1_adcPulseIntRaw");
 
  c1->cd(7);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPulseInt>>hFADC_TREF_ROC1_adcPulseInt");

  c1->cd(8);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPedRaw>>hFADC_TREF_ROC1_adcPedRaw");

  c1->cd(9);
  T1->Draw("T.hms.hFADC_TREF_ROC1_adcPed>>hFADC_TREF_ROC1_adcPed");
  
  c1->Modified();
  c1->Update();
  

  

  auto c2= new TCanvas("c1","HMS Reference Times Plots TDC (HODO)");
  c2->Divide(2,2);
  
  c2->cd(1);
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
 
  
  /*

 auto c3= new TCanvas("c3","HMS Reference Times Plots TDC (DC)");

  c3->Divide(2,1);
  c3->cd(1);
  TH1F *hDCREF1_tdcTimeRaw=new TH1F("hDCREF1_tdcTimeRaw","hDCREF1,5_tdcTimeRaw",100,0,24000);
  TH1F *hDCREF5_tdcTimeRaw=new TH1F("hDCREF5_tdcTimeRaw","hDCREF1,5_tdcTimeRaw",100,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw");
  T1->Draw("T.hms.hDCREF5_tdcTimeRaw>>hDCREF5_tdcTimeRaw");
  hDCREF1_tdcTimeRaw->SetLineColor(2);
  hDCREF1_tdcTimeRaw->Draw();
  hDCREF5_tdcTimeRaw->SetLineColor(3);
  hDCREF5_tdcTimeRaw->Draw("SAME");
  auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
   legend1->AddEntry(hDCREF1_tdcTimeRaw,"T.hms.hDCREF1_tdcTimeRaw","l");
   legend1->AddEntry(hDCREF5_tdcTimeRaw,"T.hms.hDCREF5_tdcTimeRaw","l");
   legend1->Draw();
  
  
  c3->cd(2);
  TH1F *hDCREF1_tdcMultiplicity=new TH1F("hDCREF1_tdcMultiplicity","hDCREF1,5_tdcMultiplicity",100,0,4);
  TH1F *hDCREF5_tdcMultiplicity=new TH1F("hDCREF5_tdcMultiplicity","hDCREF1,5_tdcMultiplicity",100,0,4);
  T1->Draw("T.hms.hDCREF1_tdcMultiplicity>>hDCREF1_tdcMultiplicity");
  T1->Draw("T.hms.hDCREF5_tdcMultiplicity>>hDCREF5_tdcMultiplicity");
  hDCREF1_tdcMultiplicity->SetLineColor(2);
  hDCREF1_tdcMultiplicity->Draw();
  hDCREF5_tdcMultiplicity->SetLineColor(3);
  hDCREF5_tdcMultiplicity->Draw("SAME");
  auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
   legend2->AddEntry(hDCREF1_tdcMultiplicity,"T.hms.hDCREF1_tdcMultiplicity","l");
   legend2->AddEntry(hDCREF5_tdcMultiplicity,"T.hms.hDCREF5_tdcMultiplicity","l");
   legend2->Draw();
  

  c3->Modified();
  c3->Update();
  */

  /*
  auto c3= new TCanvas("c3","HMS Reference Times Plots TDC (DC)");
  c3->Divide(2,2); 
  c3->cd(1);
  TH1F *hDCREF1_tdcTimeRaw_cut_1=new TH1F("hDCREF1_tdcTimeRaw_cut_1","hDCREF1_tdcTimeRaw_cut_1",100,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_1","T.hms.hDCREF1_tdcMultiplicity==1");
  
  TH1F *hDCREF5_tdcTimeRaw_cut_1=new TH1F("hDCREF5_tdcTimeRaw_cut_1","hDCREF5_tdcTimeRaw_cut_1",100,0,24000);
  T1->Draw("T.hms.hDCREF5_tdcTimeRaw>>hDCREF5_tdcTimeRaw_cut_1","T.hms.hDCREF5_tdcMultiplicity==1");
   
  hDCREF1_tdcTimeRaw_cut_1->SetLineColor(2);
  hDCREF5_tdcTimeRaw_cut_1->SetLineColor(3);
  hDCREF1_tdcTimeRaw_cut_1->Draw();
  hDCREF5_tdcTimeRaw_cut_1->Draw("SAME");
  auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
   legend1->AddEntry(hDCREF1_tdcTimeRaw_cut_1,"T.hms.hDCREF1_tdcMultiplicity==1","l");
   legend1->AddEntry(hDCREF5_tdcTimeRaw_cut_1,"T.hms.hDCREF5_tdcMultiplicity==1","l");
   legend1->Draw();
  


  c3->cd(2);
  TH1F *hDCREF1_tdcTimeRaw_cut_2=new TH1F("hDCREF1_tdcTimeRaw_cut_2","hDCREF1_tdcTimeRaw_cut_2",100,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_2","T.hms.hDCREF1_tdcMultiplicity==2");
  
  TH1F *hDCREF5_tdcTimeRaw_cut_2=new TH1F("hDCREF5_tdcTimeRaw_cut_2","hDCREF5_tdcTimeRaw_cut_2",100,0,24000);
  T1->Draw("T.hms.hDCREF5_tdcTimeRaw>>hDCREF5_tdcTimeRaw_cut_2","T.hms.hDCREF5_tdcMultiplicity==2");

  hDCREF1_tdcTimeRaw_cut_2->SetLineColor(2);
  hDCREF5_tdcTimeRaw_cut_2->SetLineColor(3);
  hDCREF1_tdcTimeRaw_cut_2->Draw();
  hDCREF5_tdcTimeRaw_cut_2->Draw("SAME");
  auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
   legend2->AddEntry(hDCREF1_tdcTimeRaw_cut_2,"T.hms.hDCREF1_tdcMultiplicity==2","l");
   legend2->AddEntry(hDCREF5_tdcTimeRaw_cut_2,"T.hms.hDCREF5_tdcMultiplicity==2","l");
   legend2->Draw();
  
 
  c3->cd(3);
  TH1F *hDCREF1_tdcTimeRaw_cut_3=new TH1F("hDCREF1_tdcTimeRaw_cut_3","hDCREF1_tdcTimeRaw_cut_3",100,0,24000);
  T1->Draw("T.hms.hDCREF1_tdcTimeRaw>>hDCREF1_tdcTimeRaw_cut_3","T.hms.hDCREF1_tdcMultiplicity==3");

  TH1F *hDCREF5_tdcTimeRaw_cut_3=new TH1F("hDCREF5_tdcTimeRaw_cut_3","hDCREF5_tdcTimeRaw_cut_3",100,0,24000);
  T1->Draw("T.hms.hDCREF5_tdcTimeRaw>>hDCREF5_tdcTimeRaw_cut_3","T.hms.hDCREF5_tdcMultiplicity==3");
 
  hDCREF1_tdcTimeRaw_cut_3->SetLineColor(2);
  hDCREF5_tdcTimeRaw_cut_3->SetLineColor(3);
  hDCREF1_tdcTimeRaw_cut_3->Draw();
  hDCREF5_tdcTimeRaw_cut_3->Draw("SAME");
  auto legend3 = new TLegend(0.1,0.7,0.48,0.9);
   legend3->AddEntry(hDCREF1_tdcTimeRaw_cut_3,"T.hms.hDCREF1_tdcMultiplicity==3","l");
   legend3->AddEntry(hDCREF5_tdcTimeRaw_cut_3,"T.hms.hDCREF5_tdcMultiplicity==3","l");
   legend3->Draw();

  c3->Modified();
  c3->Update(); 
  
  
 
  auto T2 =  (TTree*)fileIN->Get("TSH;1");
  auto c1= new TCanvas("c1","HMS Scaler Rate");
  T2->Draw("H.pTRIG1.scalerRate>>scalerRate");
  */
 
