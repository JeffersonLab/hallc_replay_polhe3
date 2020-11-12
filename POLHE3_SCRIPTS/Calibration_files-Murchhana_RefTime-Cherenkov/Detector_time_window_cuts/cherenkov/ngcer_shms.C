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




void ngcer_shms(){
  

//Declare root file
char run_num[20];
cout<<"Enter SHMS Run number:"<<endl;
cin>> run_num;

TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_%s_200000.root",run_num);//A1N
//TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_default_%s_-1.root",run_num);//D2N
TFile *fileIN = new TFile(filename);

//Declare Tree

TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries

Long64_t nentries;
nentries = T->GetEntries();

// Acquire the branches

Int_t npmt=4;


Double_t ngcer_goodAdcTdcDiffTime[30],ngcer_counter[30],ngcer_mult[30],ngcer_adcPulseTime[30],ngcer_goodAdcPulseAmp[30],ngcer_goodAdcPulseInt[30];;
Double_t starttime;
Int_t ndata_ngcer_pulseint;


T->SetBranchAddress("P.hod.starttime", &starttime);
T->SetBranchAddress("P.ngcer.adcPulseTime", &ngcer_adcPulseTime);

T->SetBranchAddress("Ndata.P.ngcer.adcPulseInt", &ndata_ngcer_pulseint);     //hit list
T->SetBranchAddress("P.ngcer.adcCounter", &ngcer_counter);                   //pmts

T->SetBranchAddress("P.ngcer.goodAdcMult", &ngcer_mult);

T->SetBranchAddress("P.ngcer.goodAdcTdcDiffTime",&ngcer_goodAdcTdcDiffTime);  // events that pass the detector time window cuts

T->SetBranchAddress("P.ngcer.goodAdcPulseAmp", &ngcer_goodAdcPulseAmp); 
T->SetBranchAddress("P.ngcer.goodAdcPulseInt", &ngcer_goodAdcPulseInt); 


// Declare histos

TH1F *ng_histo_good[npmt];
TH1F *ng_histo_goodMult[npmt];
TH1F *ng_histoRaw[npmt];
TH1F *ng_histoRawMult[npmt];
TH1F *ng_multiplicity[npmt];
TH2F *goodDiffTime_goodPulseInt[npmt];
TH2F *goodDiffTime_goodPulseAmp[npmt];


for(Int_t i=1; i<npmt+1; i++){
   TString name=Form("hNCer%d",i);
   TString nameMult=Form("hNCerMult%d",i);
   TString nameRaw=Form("hNCerRaw%d",i);
   TString nameRawMult=Form("hNCerRawMult%d",i);
   TString title=Form("SHMS Noble Gas Cherenkov AdcTdcTimeDiff: PMT %d",i);
   TString mtitle=Form("SHMS NGCER Multiplicity Plot: PMT %d",i);
   ng_multiplicity[i]=new TH1F("ng_multiplicity",mtitle,200,0,6);
   ng_histo_good[i]=new TH1F(name,title,200,-100,100);
   ng_histo_goodMult[i]=new TH1F(nameMult,title,200,-100,100);
   ng_histoRaw[i]=new TH1F(nameRaw,title,200,-100,100);
   ng_histoRawMult[i]=new TH1F(nameRawMult,title,200,-100,100);

   TString title2=Form("goodPulseInt vs goodDiffTime: PMT %d",i);
   TString title3=Form("goodPulseAmp vs goodDiffTime: PMT %d",i);
   goodDiffTime_goodPulseInt[i]=new TH2F("goodDiffTime_goodPulseInt",title2,400,-100,250,400,0,200);
   goodDiffTime_goodPulseAmp[i]=new TH2F("goodDiffTime_goodPulseAmp",title3,400,-100,250,400,0,300);
}


// Loop of entries in tree



for(Long64_t ievent=0; ievent < nentries; ievent++){                                      //loop over no. of entries
 T->GetEntry(ievent);

 for (Int_t counter=1; counter< npmt+1; counter++){
  ng_multiplicity[counter]->Fill(ngcer_mult[counter-1]);                                  //Fill multiplicity histogram 
  ng_histo_good[counter]->Fill(ngcer_goodAdcTdcDiffTime[counter-1]);                   //Fill histogram with events that pass the detector time window cuts: good variable
  if(ngcer_mult[counter-1]==1){
     ng_histo_goodMult[counter]->Fill(ngcer_goodAdcTdcDiffTime[counter-1]);                //Fill histogram with events that pass the detector time window cuts: good variable
     goodDiffTime_goodPulseInt[counter]->Fill(ngcer_goodAdcTdcDiffTime[counter-1],ngcer_goodAdcPulseInt[counter-1]);
     goodDiffTime_goodPulseAmp[counter]->Fill(ngcer_goodAdcTdcDiffTime[counter-1],ngcer_goodAdcPulseAmp[counter-1]);
     }
 }

 for (Int_t nhit=0; nhit < ndata_ngcer_pulseint; nhit++){                                 //loop over hits
     for(Int_t counter=1; counter<npmt+1; counter++){                                     //loop over pmts/counter
       if(ngcer_counter[nhit]==counter){                                                  //associate right it to right pmt
	 ng_histoRaw[counter]->Fill(starttime-ngcer_adcPulseTime[nhit]);                  //Fill time difference histogram  
         if(ngcer_mult[counter-1]==1){
	 ng_histoRawMult[counter]->Fill(starttime-ngcer_adcPulseTime[nhit]);               //Fill time difference histogram  
	 }
       }
     }
 }
}



//Find largest histogram peak to set Y scale

  Float_t adcmax=0;
  Float_t temp=0;
  for(Int_t i=1;i<npmt+1;i++)
    {
      temp=ng_histoRaw[i]->GetMaximum();
      if (temp>adcmax)adcmax=temp;
    }
  cout << "Max was: "<<adcmax<<endl;



//Create Reference Cut Lines

  TLine *upper_old=new TLine(80,0,80,adcmax);
  TLine *lower_old=new TLine(-20,0,-20,adcmax);

  TLine *upper_new1=new TLine(50,0,50,adcmax);
  TLine *lower_new1=new TLine(18,0,18,adcmax);

  TLine *upper_new2=new TLine(70,0,70,adcmax);
  TLine *lower_new2=new TLine(28,0,28,adcmax);

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
 c1->Divide(2,2);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c1->cd(counter);
  gPad->SetLogy();
  ng_multiplicity[counter]->Draw();
 }
 c1->Update();
 // c1->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_time_window_cuts/cherenkov/SHMS_plots/SHMS_%s_ngcer_Multiplicity.pdf",run_num));


TCanvas *c2=new TCanvas("c2","c2",1000,700);
 c2->Divide(2,2);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c2->cd(counter);
  gPad->SetLogy();
  ng_histo_good[counter]->GetYaxis()->SetRangeUser(0.2,170000);

  ng_histo_good[counter]->SetLineColor(9);
  // ng_histo_good[counter]->SetFillStyle(3144);
  // ng_histo_good[counter]->SetFillColor(9);

  ng_histo_goodMult[counter]->SetLineColor(2);
  ng_histo_goodMult[counter]->SetLineWidth(2);
  // ng_histo_goodMult[counter]->SetFillStyle(3018);
  // ng_histo_goodMult[counter]->SetFillColor(2);

  ng_histoRaw[counter]->SetLineColor(1);
  //ng_histoRaw[counter]->SetFillStyle(3395);
  // ng_histoRaw[counter]->SetFillColor(1);

  ng_histoRawMult[counter]->SetLineColor(6);
  ng_histoRawMult[counter]->SetLineWidth(2);
  //ng_histoRawMult[counter]->SetFillStyle(3005);
  // ng_histoRawMult[counter]->SetFillColor(6);

  ng_histoRaw[counter]->Draw();
  ng_histoRawMult[counter]->Draw("SAME");
  ng_histo_good[counter]->Draw("SAME");
  ng_histo_goodMult[counter]->Draw("SAME");
 
 // Draw cut lines
  lower_old->Draw("same");
  upper_old->Draw("same");
  
  /* if(counter==4){
    lower_new2->Draw("same");
    upper_new2->Draw("same");
  }
  else{
    lower_new1->Draw("same");
    upper_new1->Draw("same");
  }
  */

  TLegend *leg = new TLegend(.1,.52,.53,.9);
  leg->AddEntry(ng_histoRaw[1],"ngcer_AdcPulseTime - hodo_starttime (no cut)","l");
  leg->AddEntry(ng_histoRawMult[1],"ngcer_AdcPulseTime - hodo_starttime (mult==1)","l");
  leg->AddEntry(ng_histo_good[1],"ngcer_GoodAdcTdcTimeDiff (no cut)","l");
  leg->AddEntry(ng_histo_goodMult[1],"ngcer_GoodAdcTdcTimeDiff (mult==1)","l");
 
  leg->Draw();
  
 
 }

 c2->Update();
 //c2->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_time_window_cuts/cherenkov/SHMS_plots/SHMS_%s_ngcer_TimeWindowCut.pdf",run_num)); 



TCanvas *c3=new TCanvas("c3","c3",1000,700);
 c3->Divide(2,2);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c3->cd(counter);
  gPad->SetLogz();
  goodDiffTime_goodPulseInt[counter]->Draw("colz");
 }
 c3->Update();
 c3->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/SHMS_plots/SHMS_%s_ngcer_goodDiffTime_vs_goodPulseInt.pdf",run_num));


TCanvas *c4=new TCanvas("c4","c4",1000,700);
 c4->Divide(2,2);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c4->cd(counter);
  gPad->SetLogz();
  goodDiffTime_goodPulseAmp[counter]->Draw("colz");
 }
 c4->Update();
 c4->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/Detector_plots/cherenkov/SHMS_plots/SHMS_%s_ngcer_goodDiffTime_vs_goodPulseAmp.pdf",run_num));
return 0;
}





























