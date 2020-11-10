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


void hgcer_shms(){
  

//Declare root file

TFile *fileIN = new TFile("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_10793_200000.root");

//Declare Tree

TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries

Long64_t nentries;
nentries = T->GetEntries();

// Acquire the branches

Int_t npmt=4;

Double_t hgcer_goodAdcTdcDiffTime[npmt],hgcer_counter[npmt],hgcer_mult[npmt],hgcer_adcPulseTime[npmt];
Double_t starttime;


T->SetBranchAddress("P.hgcer.goodAdcTdcDiffTime",&hgcer_goodAdcTdcDiffTime);
T->SetBranchAddress("P.hgcer.goodAdcMult", &hgcer_mult);
T->SetBranchAddress("P.hgcer.adcCounter", &hgcer_counter);
T->SetBranchAddress("P.hod.starttime", &starttime);
T->SetBranchAddress("P.hgcer.adcPulseTime", &hgcer_adcPulseTime);




// Declare histos

TH1F *hg_histo[npmt];
TH1F *hg_histoMult[npmt];
TH1F *hg_histoRaw[npmt];
TH1F *hg_histoMultRaw[npmt];
TH1F *hg_multiplicity[npmt];

for(Int_t i=0; i<npmt; i++){
   TString name=Form("hHCer%d",i+1);
   TString nameMult=Form("hHCerMult%d",i+1);
   TString nameRaw=Form("hHCerRaw%d",i+1);
   TString nameMultRaw=Form("hHCerMultRaw%d",i+1);
   TString title=Form("SHMS Heavy gas Cherenkov AdcTdcTimeDiff: PMT %d",i+1);
   TString mtitle=Form("SHMS HGCER Multiplicity Plot: PMT %d",i+1);
   hg_multiplicity[i]=new TH1F("hg_multiplicity",mtitle,200,0,6);
   hg_histo[i]=new TH1F(name,title,200,-500,300);
   hg_histoMult[i]=new TH1F(nameMult,title,200,-500,300);
   hg_histoRaw[i]=new TH1F(nameRaw,title,200,-500,300);
   hg_histoMultRaw[i]=new TH1F(nameMultRaw,title,200,-500,300);
}


// Loop of entries in tree

for(Long64_t ievent=0; ievent < nentries; ievent++){
 T->GetEntry(ievent);
 for (Int_t ipmt=0; ipmt < npmt; ipmt++){
          hg_multiplicity[ipmt]->Fill(hgcer_mult[ipmt]);
	  hg_histo[ipmt]->Fill(hgcer_goodAdcTdcDiffTime[ipmt]);
           if(hgcer_mult[ipmt]==1.0){
             hg_histoMult[ipmt]->Fill(hgcer_goodAdcTdcDiffTime[ipmt]);
	   }
 }
 for (Int_t ipmt=0; ipmt < npmt; ipmt++){
	 hg_histoRaw[ipmt]->Fill(starttime-hgcer_adcPulseTime[ipmt]);
          if(hgcer_mult[ipmt]==1.0){
             hg_histoMultRaw[ipmt]->Fill(starttime-hgcer_adcPulseTime[ipmt]);
	   }
 }
}
TCanvas *c1=new TCanvas("c1","c1",1000,700);
 c1->Divide(2,2);
 for(Int_t ipmt = 0; ipmt < npmt; ipmt++){ 
  c1->cd(ipmt+1);
  gPad->SetLogy();
  hg_multiplicity[ipmt]->Draw();
 }
 c1->Update();


//Find largest histogram peak to set Y scale
  Float_t adcmax=0;
  Float_t temp=0;
  for(Int_t i=0;i<npmt;i++)
    {
      temp=hg_histoRaw[i]->GetMaximum();
      if (temp>adcmax)adcmax=temp;
    }
  cout << "Max was: "<<adcmax<<endl;

  //Create Reference Cut Line
  TLine *lower=new TLine(70,0,70,adcmax);
  TLine *upper=new TLine(0,0,0,adcmax);
  lower->SetLineStyle(2);
  lower->SetLineColor(kRed);
  upper->SetLineStyle(2);
  upper->SetLineColor(kRed);




TCanvas *c2=new TCanvas("c2","c2",1000,700);
 c2->Divide(2,2);
 for(Int_t ipmt = 0; ipmt < npmt; ipmt++){ 
  c2->cd(ipmt+1);
  gPad->SetLogy();
  hg_histo[ipmt]->SetLineColor(9);
  hg_histoMult[ipmt]->SetLineColor(6);
  hg_histoRaw[ipmt]->SetLineColor(1);
  hg_histoMultRaw[ipmt]->SetLineColor(2);
  hg_histo[ipmt]->Draw();
  hg_histoMult[ipmt]->Draw("SAME");
  hg_histoRaw[ipmt]->Draw("SAME");
  hg_histoMultRaw[ipmt]->Draw("SAME");
  // Draw cut lines
       lower->Draw("same");
       upper->Draw("same");
  TLegend *leg = new TLegend(.1,.62,.5,.9);
  leg->AddEntry(hg_histo[1],"hgcer_GoodAdcTdcTimeDiff (no cut)","l");
  leg->AddEntry(hg_histoMult[1],"hgcer_GoodAdcTdcTimeDiff (multiplicity==1)","l");
  leg->AddEntry(hg_histoRaw[1],"hgcer_AdcPulseTime - hodo_starttime","l");
  leg->AddEntry(hg_histoMultRaw[1],"hgcer_AdcPulseTime - hodo_starttime(multiplicity==1)","l");
  leg->Draw();
 
 }

 c2->Update(); 
return 0;
}
 






























/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//Using chain insted of TTree, mostly useful while handling more than one rootfile.


  TFile *fileIN;
  
  TChain chain("T");
  
  cout << "what a fuck? " << endl; 
  Double_t pFADC_TREF_ROC2_adcPulseTimeRaw;

  chain.Add("/u/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_10614_200000.root");
  chain.SetBranchAddress("T.shms.pFADC_TREF_ROC2_adcPulseTimeRaw",& pFADC_TREF_ROC2_adcPulseTimeRaw);
  
  TH1F *histogram=new TH1F("histogram","pFADC_TREF_ROC2_adcPulseTimeRaw",200,0,10000);
  Int_t entries=chain.GetEntries();
  for( Int_t i=0;i<entries;i++){
    chain.GetEntry(i);
    histogram->Fill(pFADC_TREF_ROC2_adcPulseTimeRaw);
  }
  histogram->Draw();

  cout<<entries<<endl;
 

  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
