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
#include <TH2.h>
#include <TF1.h>
#include <cstdlib>
#include "TStyle.h"
#include "TColor.h"

using namespace std;

static const double degtorad = 3.141592653589793/180.;


void cer_hms(){
  

//Declare root file

char run_num[20];
cout<<"Enter HMS Run number:"<<endl;
cin>> run_num;

TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_all_%s_200000.root",run_num);
TFile *fileIN = new TFile(filename);



//Declare Tree

TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries

Long64_t nentries;
nentries = T->GetEntries();

// Acquire the branches

Int_t npmt=2;

Double_t cer_goodAdcTdcDiffTime[30],cer_counter[30],cer_mult[30],cer_adcPulseTime[30],cer_goodAdcPulseAmp[30],cer_goodAdcPulseInt[30];
Double_t starttime;
Int_t ndata_cer_pulseint;

T->SetBranchAddress("H.hod.starttime", &starttime);
T->SetBranchAddress("H.cer.adcPulseTime", &cer_adcPulseTime);

T->SetBranchAddress("Ndata.H.cer.adcPulseInt", &ndata_cer_pulseint); 
T->SetBranchAddress("H.cer.adcCounter", &cer_counter);

T->SetBranchAddress("H.cer.goodAdcMult", &cer_mult);

T->SetBranchAddress("H.cer.goodAdcTdcDiffTime",&cer_goodAdcTdcDiffTime);

T->SetBranchAddress("H.cer.goodAdcPulseAmp", &cer_goodAdcPulseAmp); 
T->SetBranchAddress("H.cer.goodAdcPulseInt", &cer_goodAdcPulseInt); 

// Declare histos

TH1F *histo_good[npmt];
TH1F *histo_goodMult[npmt];
TH1F *histoRaw[npmt];
TH1F *histoRawMult[npmt];
TH1F *multiplicity[npmt];
TH2F *goodDiffTime_goodPulseInt[npmt];
TH2F *goodDiffTime_goodPulseAmp[npmt];

for(Int_t i=1; i<npmt+1; i++){
   TString name=Form("hCer%d",i);
   TString nameMult=Form("hCerMult%d",i);
   TString nameRaw=Form("hCerRaw%d",i);
   TString nameRawMult=Form("hCerRawMult%d",i);
   TString title=Form("HMS Cherenkov AdcTdcTimeDiff: PMT %d",i);
   TString mtitle=Form("HMS CER Multiplicity Plot: PMT %d",i);
   multiplicity[i]=new TH1F("multiplicity",mtitle,200,0,6);
   histo_good[i]=new TH1F(name,title,400,-100,300);
   histo_goodMult[i]=new TH1F(nameMult,title,400,-100,300);
   histoRaw[i]=new TH1F(nameRaw,title,400,-100,300);
   histoRawMult[i]=new TH1F(nameRawMult,title,400,-100,300);
   /* histo_good[i]=new TH1F(name,title,250,50,200);
   histo_goodMult[i]=new TH1F(nameMult,title,250,50,200);
   histoRaw[i]=new TH1F(nameRaw,title,250,50,200);
   histoRawMult[i]=new TH1F(nameRawMult,title,250,50,200);*/

   TString title2=Form("goodPulseInt vs goodDiffTime: PMT %d",i);
   TString title3=Form("goodPulseAmp vs goodDiffTime: PMT %d",i);
   goodDiffTime_goodPulseInt[i]=new TH2F("goodDiffTime_goodPulseInt",title2,400,-100,250,400,0,200);
   goodDiffTime_goodPulseAmp[i]=new TH2F("goodDiffTime_goodPulseAmp",title3,400,-100,250,400,0,300);

}


// Loop of entries in tree


for(Long64_t ievent=0; ievent < nentries; ievent++){                                   //loop over no. of entries
 T->GetEntry(ievent);

for (Int_t counter=1; counter< npmt+1; counter++){
  multiplicity[counter]->Fill(cer_mult[counter-1]);                                  //Fill multiplicity histogram 
  histo_good[counter]->Fill(cer_goodAdcTdcDiffTime[counter-1]);                  //Fill histogram with events that pass the detector time window cuts: good variable
 if(cer_mult[counter-1]==1){
     histo_goodMult[counter]->Fill(cer_goodAdcTdcDiffTime[counter-1]);                //Fill histogram with events that pass the detector time window cuts: good variable
     goodDiffTime_goodPulseInt[counter]->Fill(cer_goodAdcTdcDiffTime[counter-1],cer_goodAdcPulseInt[counter-1]);
     goodDiffTime_goodPulseAmp[counter]->Fill(cer_goodAdcTdcDiffTime[counter-1],cer_goodAdcPulseAmp[counter-1]);
 } 
}



 for (Int_t nhit=0; nhit < ndata_cer_pulseint; nhit++){                               //loop over hits
   for(Int_t counter=1; counter<npmt+1; counter++){                                     //loop over pmts/counter
     if(cer_counter[nhit]==counter){                                                  //associate right it to right pmt
       histoRaw[counter]->Fill(starttime-cer_adcPulseTime[nhit]);                  //Fill time difference histogram  
       if(cer_mult[counter-1]==1){
	 histoRawMult[counter]->Fill(starttime-cer_adcPulseTime[nhit]);            //Fill time differnece histogram with multiplicity=1
       }
     }
   }
 }
}



//Find largest histogram peak to set Y scale
  Float_t adcmax=0;
  Float_t temp=0;
  for(Int_t i=0;i<npmt;i++)
    {
      temp=histoRaw[i]->GetMaximum();
      if (temp>adcmax)adcmax=temp;
    }
  cout << "Max was: "<<adcmax<<endl;

  //Create Reference Cut Line

  TLine *lower_old=new TLine(60,0,60,adcmax);
  TLine *upper_old=new TLine(200,0,200,adcmax);

  TLine *upper_new1=new TLine(160,0,160,adcmax);
  TLine *lower_new1=new TLine(100,0,100,adcmax);

  TLine *upper_new2=new TLine(160,0,160,adcmax);
  TLine *lower_new2=new TLine(120,0,120,adcmax);


  lower_old->SetLineStyle(2);
  lower_old->SetLineColor(kBlack);
  upper_old->SetLineStyle(2);
  upper_old->SetLineColor(kBlack);

  lower_new1->SetLineStyle(2);
  lower_new1->SetLineColor(kRed);
  lower_new1->SetLineWidth(2);
  upper_new1->SetLineStyle(2);
  upper_new1->SetLineColor(kRed);
  upper_new1->SetLineWidth(2);

  lower_new2->SetLineStyle(2);
  lower_new2->SetLineColor(kRed);
  lower_new2->SetLineWidth(2);
  upper_new2->SetLineStyle(2);
  upper_new2->SetLineColor(kRed);
  upper_new2->SetLineWidth(2);



//Draw histograms

TCanvas *c1=new TCanvas("c1","c1",1000,700);
 c1->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c1->cd(counter);
  gPad->SetLogy();
  multiplicity[counter]->Draw();
 }
 c1->Update();
  c1->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/HMS_plots/HMS_%s_cer_Multiplicity.pdf",run_num));


TCanvas *c2=new TCanvas("c2","c2",1000,700);
 c2->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c2->cd(counter);
  gPad->SetLogy();

  //histo_good[counter]->GetYaxis()->SetRangeUser(0.2,170000);
  histo_good[counter]->SetLineColor(9);
  histo_goodMult[counter]->SetLineColor(6);
  histo_goodMult[counter]->SetLineWidth(2);

  histoRaw[counter]->SetLineColor(1);
  histoRawMult[counter]->SetLineColor(2);
  histoRawMult[counter]->SetLineWidth(2);
 
  histoRaw[counter]->Draw();
  histoRawMult[counter]->Draw("SAME");
 
  histo_good[counter]->Draw("SAME");
  histo_goodMult[counter]->Draw("same");

  // Draw cut lines
  lower_old->Draw("same");
  upper_old->Draw("same");
  
  /* if(counter==2){
    lower_new2->Draw("same");
    upper_new2->Draw("same");
  }
  else{
    lower_new1->Draw("same");
    upper_new1->Draw("same");
    }*/
    

  TLegend *leg = new TLegend(.1,.6,.5,.9); 

  leg->AddEntry(histoRaw[1],"cer_AdcPulseTime - hodo_starttime (no cut)","l");
  leg->AddEntry(histoRawMult[1],"cer_AdcPulseTime - hodo_starttime (mult==1)","l");
  leg->AddEntry(histo_good[1],"cer_GoodAdcTdcTimeDiff (no cut)","l");
  leg->AddEntry(histo_goodMult[1],"cer_GoodAdcTdcTimeDiff (mult==1)","l");
 
  leg->Draw(); 
 }

 c2->Update(); 
 c2->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/HMS_plots/HMS_%s_cer_TimeWindowCut.pdf",run_num));





TCanvas *c3=new TCanvas("c3","c3",1000,700);
 c3->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c3->cd(counter);
  gPad->SetLogz();
  goodDiffTime_goodPulseInt[counter]->Draw("colz");
 }
 c3->Update();
 c3->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/HMS_plots/HMS_%s_cer_goodDiffTime_vs_goodPulseInt.pdf",run_num));


TCanvas *c4=new TCanvas("c4","c4",1000,700);
 c4->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c4->cd(counter);
  gPad->SetLogz();
  goodDiffTime_goodPulseAmp[counter]->Draw("colz");
 }
 c4->Update();
 c4->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/HMS_plots/HMS_%s_cer_goodDiffTime_vs_goodPulseAmp.pdf",run_num));
return 0;
}
 





























