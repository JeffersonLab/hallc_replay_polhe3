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
#include <TLine.h>

using namespace std;

// Draft of a program to conduct ngcer calibration for SHMS by hand using "Method 1" from Simona. 
// https://logbooks.jlab.org/entry/3650663
// https://hallcweb.jlab.org/DocDB/0010/001053/001/wisewordsoncheranalysis.pdf
// murchhanaroy@uky.edu
// AUgust 2020 


void cer_calib_hms(){

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.01);
  

//Declare root file
char run_num[20];
cout<<"Enter HMS Run number (Including leading 0):"<<endl;
cin>> run_num;

//TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_all_%s_200000.root",run_num);
TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_default_%s_-1.root",run_num);
TFile *fileIN = new TFile(filename);


//Declare Tree
TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries
Long64_t nentries;
nentries = T->GetEntries();

// Acquire the branches
Int_t npmt=2;

//define the arrays we are going to put the branches in. 

 Double_t ngcer_goodAdcTdcDiffTime[4],ngcer_counter[30],ngcer_mult[30],ngcer_adcPulseTime[30],ngcer_goodAdcPulseAmp[4],ngcer_goodAdcPulseInt[4],ngcer_npe[4];
 Double_t cal_eprtracknorm, cal_etracknorm,ngcer_XatCer,ngcer_YatCer, delta; //good == array of 4. (This is what Brad said, he can probably explain more why). 
 Double_t starttime;
 Int_t ndata_ngcer_pulseint;
 
T->SetBranchAddress("H.hod.starttime", &starttime);
T->SetBranchAddress("H.cer.adcPulseTime", &ngcer_adcPulseTime);

T->SetBranchAddress("Ndata.H.cer.adcPulseInt", &ndata_ngcer_pulseint);     //hit list
T->SetBranchAddress("H.cer.adcCounter", &ngcer_counter);                   //pmts

T->SetBranchAddress("H.cer.goodAdcMult", &ngcer_mult);

T->SetBranchAddress("H.cer.goodAdcTdcDiffTime",&ngcer_goodAdcTdcDiffTime);  // events that pass the detector time window cuts

 T->SetBranchAddress("H.cer.goodAdcPulseAmp", &ngcer_goodAdcPulseAmp);
 T->SetBranchAddress("H.cer.goodAdcPulseInt", &ngcer_goodAdcPulseInt);

 T-> SetBranchAddress("H.cer.xAtCer",&ngcer_XatCer);
 T-> SetBranchAddress("H.cer.yAtCer",&ngcer_YatCer);
 T-> SetBranchAddress("H.cer.npe",&ngcer_npe);

 ////Energy variables for the shower and preshower
 T->SetBranchAddress("H.cal.etracknorm", &cal_etracknorm); // shower + preshower energy (I'm not exactly sure how these are "normalized" but these are the variables Brad and Xiaochao wanted me to use)
 T->SetBranchAddress("H.cal.eprtracknorm", &cal_eprtracknorm); // preshower energy
 T->SetBranchAddress("H.gtr.dp", &delta);
 





//// Declare histos and fits. 

 //Old code is counting from 1 so we need arrays of 5.

 TH1F *ng_goodAdcPulseAmp[5]; //no cuts, only the branch itself
 TH1F *ng_goodAdcPulseInt[5];
 
 TH1F *ng_goodAdcPulseAmp_multcut[5]; // multiplicity=1 cut applied
 TH1F *ng_goodAdcPulseInt_multcut[5];
 TH1F *ng_goodAdcPulseInt_multcut_fit[5];

 TH1F *ng_goodAdcPulseAmp_multcut_nonphys1[5]; // multiplicity=1 cut applied, only cal cut
 TH1F *ng_goodAdcPulseInt_multcut_nonphys1[5];

 TH1F *ng_goodAdcPulseAmp_multcut_nonphys2[5]; // multiplicity=1 cut applied, only xatcer, yatcer not equal 0
 TH1F *ng_goodAdcPulseInt_multcut_nonphys2[5];

 TH1F *ng_goodAdcPulseAmp_multcut_nonphys[5]; // multiplicity=1 cut applied
 TH1F *ng_goodAdcPulseInt_multcut_nonphys[5];

 TH1F *ng_goodAdcPulseAmp_multcut_single[5]; // multiplicity=1 cut applied
 TH1F *ng_goodAdcPulseInt_multcut_single[5];

 TH1F *ng_goodAdcTdcDiffTime[5]; // histo after cuts are applied 
 TH1F *ngcer_npe_histo[5];

 
 
 TF1 *gausFitnpe1, *gausFitnpe2;
 
 TF1 *gausFitAmp1, *gausFitAmp2;
 TF1 *gausFitInt1, *gausFitInt2;
TF1 *gausFitInt_zoom1, *gausFitInt_zoom2;


 

 // initializing histos that are for each PMT
 for(Int_t i=1; i<npmt+1; i++){
   TString namePulseAmp=Form("hNcerPulseAmp%d",i);  
   TString titlePulseAmp=Form("PMT%d: HMS CER goodAdcPulseAmp raw branch, Run %s",i,run_num);
   TString namePulseInt=Form("hNcerPulseInt%d",i);  
   TString titlePulseInt=Form("PMT%d: HMS CER goodAdcPulseInt raw branch, Run %s",i,run_num);


   TString namePulseAmp_multcut=Form("hNcerPulseAmp%d multcut",i);  
   TString titlePulseAmp_multcut=Form("PMT%d: HMS CER goodAdcPulseAmp raw brunch mult cut Run %s",i,run_num);
   TString namePulseInt_multcut=Form("hNcerPulseInt%d multcut",i);  
   TString titlePulseInt_multcut=Form("PMT%d: HMS CER goodAdcPulseInt raw branch mult cut %s",i,run_num);

   TString namePulseInt_multcut_fit=Form("hNcerPulseInt%d multcut_fit",i);  
   TString titlePulseInt_multcut_fit=Form("PMT%d: HMS CER goodAdcPulseInt raw branch mult cut %s",i,run_num);
   
   
   TString namePulseAmp_multcut_nonphys=Form("hNcerPulseAmp%d nonphyscut",i);  
   TString titlePulseAmp_multcut_nonphys=Form("PMT%d: HMS CER goodAdcPulseAmp non phys cut mult cut Run %s",i,run_num);
   TString namePulseInt_multcut_nonphys=Form("hNcerPulseInt%d nonphysut",i);  
   TString titlePulseInt_multcut_nonphys=Form("PMT%d: HMS CER goodAdcPulseInt non phys cut mult cut %s",i,run_num);

   TString namePulseAmp_multcut_nonphys1=Form("hNcerPulseAmp%d nonphys1cut",i);  
   TString titlePulseAmp_multcut_nonphys1=Form("PMT%d: HMS CER goodAdcPulseAmp non phys cut1 mult cut Run %s",i,run_num);
   TString namePulseInt_multcut_nonphys1=Form("hNcerPulseInt%d nonphys1cut",i);  
   TString titlePulseInt_multcut_nonphys1=Form("PMT%d: HMS CER goodAdcPulseInt non phys cut1 mult cut %s",i,run_num);

   TString namePulseAmp_multcut_nonphys2=Form("hNcerPulseAmp%d nonphys2cut",i);  
   TString titlePulseAmp_multcut_nonphys2=Form("PMT%d: HMS CER goodAdcPulseAmp non phys cut2 mult cut Run %s",i,run_num);
   TString namePulseInt_multcut_nonphys2=Form("hNcerPulseInt%d nonphys2cut",i);  
   TString titlePulseInt_multcut_nonphys2=Form("PMT%d: HMS CER goodAdcPulseInt non phys cut2 mult cut %s",i,run_num);

   TString namePulseAmp_multcut_single=Form("hNcerPulseAmp%d allcut",i);  
   TString titlePulseAmp_multcut_single=Form("PMT%d: HMS CER goodAdcPulseAmp non-physical cut, mult cut, single pmt %s",i,run_num);
   TString namePulseInt_multcut_single=Form("hNcerPulseInt%d allcut",i);  
   TString titlePulseInt_multcut_single=Form("PMT%d: HMS CER goodAdcPulseInt non-physical cut, mult cut, single pmt %s",i,run_num);
   
   ng_goodAdcPulseAmp[i]= new TH1F(namePulseAmp, titlePulseAmp,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt[i]= new TH1F(namePulseInt, titlePulseInt, 140, 0,70);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseInt_multcut_fit[i]= new TH1F(namePulseInt_multcut_fit, titlePulseInt_multcut_fit, 500,0,20);


   ng_goodAdcPulseAmp_multcut[i]= new TH1F(namePulseAmp_multcut, titlePulseAmp_multcut,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_multcut[i]= new TH1F(namePulseInt_multcut, titlePulseInt_multcut, 140,0,70);// (200 for 9740) (50 for 9753)
    
   ng_goodAdcPulseAmp_multcut_nonphys1[i]= new TH1F(namePulseAmp_multcut_nonphys1, titlePulseAmp_multcut_nonphys1,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_multcut_nonphys1[i]= new TH1F(namePulseInt_multcut_nonphys1, titlePulseInt_multcut_nonphys1, 140,0,70);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_multcut_nonphys2[i]= new TH1F(namePulseAmp_multcut_nonphys2, titlePulseAmp_multcut_nonphys2,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_multcut_nonphys2[i]= new TH1F(namePulseInt_multcut_nonphys2, titlePulseInt_multcut_nonphys2, 140,0,70);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_multcut_nonphys[i]= new TH1F(namePulseAmp_multcut_nonphys, titlePulseAmp_multcut_nonphys,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_multcut_nonphys[i]= new TH1F(namePulseInt_multcut_nonphys, titlePulseInt_multcut_nonphys, 140,0,70);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_multcut_single[i]= new TH1F(namePulseAmp_multcut_single, titlePulseAmp_multcut_single,240,0,120); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_multcut_single[i]= new TH1F(namePulseInt_multcut_single, titlePulseInt_multcut_single, 140,0,70);// (200 for 9740) (50 for 9753)

   ng_goodAdcTdcDiffTime[i]=new TH1F(Form("goodAdcTdcDiffTime PMT%d run %s",i,run_num),Form("goodAdcTdcDiffTime PMT%d run %s",i,run_num),150,-50,100); 

   ngcer_npe_histo[i]=new TH1F(Form("H.cer.npe[%d] run %s",i,run_num),Form("H.cer.npe[%d] run %s",i,run_num),100,0,10);
}

 




 /********************************************************************************************************************/
 // Begining of analysis on all the events and applying cuts

// Loop of entries in tree
 cout << "nentries= "<<nentries<<endl;
 for(Long64_t ievent=0; ievent < nentries; ievent++){   //can be lowered for testing                       

   //if (ievent % 10000 == 0){//printing progress updates to the terminal
   // cout<< "ievent= "<<ievent <<endl;
   // }

   T->GetEntry(ievent);
 
   //Filling up the RAW histos. Counting from 1. 
   for (int i=1; i<npmt+1;i++){ 
     ng_goodAdcPulseAmp[i]->Fill(ngcer_goodAdcPulseAmp[i-1]);
     ng_goodAdcPulseInt[i]->Fill(ngcer_goodAdcPulseInt[i-1]);
     ng_goodAdcTdcDiffTime[i]->Fill(ngcer_goodAdcTdcDiffTime[i-1]);
     if(ngcer_mult[i-1]==1){
       ng_goodAdcPulseAmp_multcut[i]->Fill(ngcer_goodAdcPulseAmp[i-1]); // only multiplicity cut on raw brunch
       ng_goodAdcPulseInt_multcut[i]->Fill(ngcer_goodAdcPulseInt[i-1]);
       ng_goodAdcPulseInt_multcut_fit[i]->Fill(ngcer_goodAdcPulseInt[i-1]);
       ngcer_npe_histo[i]->Fill(ngcer_npe[i-1]);	
     }
   }

  
 
   // now we ARE CHECKING if only one PMT had a singnal 
   for (Int_t counterNew=1; counterNew< npmt+1; counterNew++){  // counting from 1

     //apply mutiplicity cuts
     if(ngcer_mult[counterNew-1]==1){
       //// cut out any non-physical singnals. There is a line at x=y in the calorimeter that isn't physical, one at a time. 
        if(cal_eprtracknorm>0 && cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm){
	   ng_goodAdcPulseAmp_multcut_nonphys1[counterNew]->Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
	   ng_goodAdcPulseInt_multcut_nonphys1[counterNew]->Fill(ngcer_goodAdcPulseInt[counterNew-1]);
	}
	if(ngcer_YatCer != 0 && ngcer_XatCer != 0){
	   ng_goodAdcPulseAmp_multcut_nonphys2[counterNew]->Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
	   ng_goodAdcPulseInt_multcut_nonphys2[counterNew]->Fill(ngcer_goodAdcPulseInt[counterNew-1]);

	}

       //// cut out any non-physical singnals. There is a line at x=y in the calorimeter that isn't physical. 
	if(ngcer_YatCer != 0 && ngcer_XatCer != 0){
	 if(cal_eprtracknorm>0 && cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm){
	
     
	   Bool_t singleFlagAmp=kTRUE; // flags to help identify if there was singnal in only one PMT at a time
	   Bool_t singleFlagInt=kFALSE;

 
	   ng_goodAdcPulseAmp_multcut_nonphys[counterNew]->Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
	   ng_goodAdcPulseInt_multcut_nonphys[counterNew]->Fill(ngcer_goodAdcPulseInt[counterNew-1]);


	   if(ngcer_goodAdcPulseAmp[counterNew-1]>0)
	     {   
	       singleFlagAmp=kTRUE; // this assignement is redundent 
	       for (Int_t counter2=1; counter2 <npmt+1;counter2++){     
		 if (counterNew != counter2 && ngcer_goodAdcPulseAmp[counter2-1] >0){// loop over the other PMT's (that are not itself) and see if there was a signal
		   singleFlagAmp = kFALSE; // there was a signal in one of the other PMTs, so event was not in one single pmt
		 }
	       }
	       if (singleFlagAmp)
		 {// if event was in one single pmt
		   ng_goodAdcPulseAmp_multcut_single[counterNew]->Fill(ngcer_goodAdcPulseAmp[counterNew-1]); //Fill histo with the goodAdcPulseAmp branch without any cuts 	     	  
		 }// end singleFlagAmp check          
	     }// end looking at AdcPulseAmp
     
 

	   //looking at AdcPulseInt
	   if(ngcer_goodAdcPulseInt[counterNew-1]>0){
	     singleFlagInt=kTRUE;// redundent statment
	     for (Int_t counter3=1; counter3 <npmt+1;counter3++){
	       if (counterNew != counter3 && ngcer_goodAdcPulseInt[counter3-1] >0){// loop over the other PMTs to see if any had a signal
		 singleFlagInt = kFALSE; // event was not in 1 single pmt
	       }
	     }
	     if (singleFlagInt){// if event was in 1 single pmt
	       ng_goodAdcPulseInt_multcut_single[counterNew]->Fill(ngcer_goodAdcPulseInt[counterNew-1]); //Fill histo with the goodAdcPulseInt branch without shower cuts
	     } //end of single PMT check  
	   }// end of AdcPulseInt analysis 
 
	 }
       }//end of non-physical check

     }//end of multiplicity==1 cut for the pmt under calibration
   } // end of single PMT analysis
 }//end of event loop
 
 // Ending of analysis of events and applying all the cuts
 /***************************************************************************************************************************/

 
 //Graph the results!
 /*
 // graph the AdcPulseAmp histos 
TCanvas *c1=new TCanvas("c1","c1",1000,700);
 c1->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c1->cd(counter);
  gPad->SetLogy();

  ng_goodAdcPulseAmp[counter]->SetLineColor(1);//black
  ng_goodAdcPulseAmp[counter]->SetLineWidth(1);
  ng_goodAdcPulseAmp[counter]->Draw();   

  ng_goodAdcPulseAmp_multcut[counter]->SetLineColor(416);//green
  ng_goodAdcPulseAmp_multcut[counter]->SetLineWidth(1);
  ng_goodAdcPulseAmp_multcut[counter]->Draw("SAME"); 

  ng_goodAdcPulseAmp_multcut_nonphys1[counter]->SetLineColor(632);//Red
  ng_goodAdcPulseAmp_multcut_nonphys1[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_multcut_nonphys1[counter]->Draw("SAME");
  
  ng_goodAdcPulseAmp_multcut_nonphys2[counter]->SetLineColor(880);//violet
  ng_goodAdcPulseAmp_multcut_nonphys2[counter]->SetLineWidth(1);
  ng_goodAdcPulseAmp_multcut_nonphys2[counter]->Draw("SAME");

  ng_goodAdcPulseAmp_multcut_nonphys[counter]->SetLineColor(800);//orange
  ng_goodAdcPulseAmp_multcut_nonphys[counter]->SetLineWidth(1);
  ng_goodAdcPulseAmp_multcut_nonphys[counter]->Draw("SAME");
  
  ng_goodAdcPulseAmp_multcut_single[counter]->SetLineColor(7);//pink
  ng_goodAdcPulseAmp_multcut_single[counter]->SetLineWidth(1);
  ng_goodAdcPulseAmp_multcut_single[counter]->Draw("SAME");

  TLegend *legend1 = new TLegend(0.1,0.6,0.6,0.9);
 
   legend1->AddEntry(ng_goodAdcPulseAmp[counter],"ng_goodAdcPulseAmp(raw branch)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_multcut[counter],"ng_goodAdcPulseAmp (mul=1)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_multcut_nonphys2[counter],"ng_goodAdcPulseAmp (mul=1,ngcer_YatCer != 0 && ngcer_XatCer != 0  )","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_multcut_nonphys1[counter],"#splitline{ng_goodAdcPulseAmp (mul=1, cal_eprtracknorm>0 &&}{ cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm)}","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_multcut_nonphys[counter],"#splitline{ng_goodAdcPulseAmp (mul=1,ngcer_YatCer != 0 &&}{#splitline{ngcer_XatCer != 0,cal_eprtracknorm>0 &&}{ cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm)}}","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_multcut_single[counter],"ng_goodAdcPulseAmp (mul=1,all cuts,single pmt)","l");
   legend1->Draw();
 }
 c1->Update();

 
 // Graph the AdcPulseInt histos
TCanvas *c2=new TCanvas("c2","c2",1000,700);
 c2->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c2->cd(counter);
  gPad->SetLogy();
  
  ng_goodAdcPulseInt[counter]->SetLineColor(kBlack);//black
  ng_goodAdcPulseInt[counter]->SetLineWidth(1);
  ng_goodAdcPulseInt[counter]->Draw();   

  ng_goodAdcPulseInt_multcut[counter]->SetLineColor(416);//green
  ng_goodAdcPulseInt_multcut[counter]->SetLineWidth(1);
  ng_goodAdcPulseInt_multcut[counter]->Draw("SAME");

  ng_goodAdcPulseInt_multcut_nonphys1[counter]->SetLineColor(632);//blue
  ng_goodAdcPulseInt_multcut_nonphys1[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_multcut_nonphys1[counter]->Draw("SAME");

  ng_goodAdcPulseInt_multcut_nonphys2[counter]->SetLineColor(880);//blue
  ng_goodAdcPulseInt_multcut_nonphys2[counter]->SetLineWidth(1);
  ng_goodAdcPulseInt_multcut_nonphys2[counter]->Draw("SAME");

  ng_goodAdcPulseInt_multcut_nonphys[counter]->SetLineColor(800);//blue
  ng_goodAdcPulseInt_multcut_nonphys[counter]->SetLineWidth(1);
  ng_goodAdcPulseInt_multcut_nonphys[counter]->Draw("SAME");

  
  ng_goodAdcPulseInt_multcut_single[counter]->SetLineColor(7);//magenta
  ng_goodAdcPulseInt_multcut_single[counter]->SetLineWidth(1);
  ng_goodAdcPulseInt_multcut_single[counter]->Draw("SAME");
 TLegend *legend2 = new TLegend(0.1,0.6,0.6,0.9);
 
   legend2->AddEntry(ng_goodAdcPulseInt[counter],"ng_goodAdcPulseInt(raw branch)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_multcut[counter],"ng_goodAdcPulseInt (mul=1)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_multcut_nonphys2[counter],"ng_goodAdcPulseAInt (mul=1,ngcer_YatCer != 0 && ngcer_XatCer != 0  )","l");
   legend2->AddEntry(ng_goodAdcPulseInt_multcut_nonphys1[counter],"#splitline{ng_goodAdcPulseInt (mul=1, cal_eprtracknorm>0 &&}{ cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm)}","l");
   legend2->AddEntry(ng_goodAdcPulseInt_multcut_nonphys[counter],"#splitline{ng_goodAdcPulseInt (mul=1,ngcer_YatCer != 0 &&}{#splitline{ngcer_XatCer != 0,cal_eprtracknorm>0 &&}{ cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm)}}","l");
   legend2->AddEntry(ng_goodAdcPulseInt_multcut_single[counter],"ng_goodAdcPulseInt (mul=1,all cuts,single pmt)","l");
   legend2->Draw();
 }
 c2->Update();
 */

//declare fits for 1D histos. Change as desired. 
 Double_t FitAmpMinArray[3]={-1000,12.5,12.5}; // {flag, PMT1, PMT2} Where to start the Amp fits for each PMT
 Double_t FitAmpMaxArray[3]={-1000,24.5,24.5}; // {flag, PMT1, PMT2} Where to end the Amp fits

 Double_t FitIntMinArray[3]={-1000,6.2,6.2}; //{flag, PMT1, PMT2} Where to start the Int fits
 Double_t FitIntMaxArray[3]={-1000,10.5,10.5};//{flag, PMT1, PMT2} Where to end the Int fits

 Double_t FitnpeMinArray[3]={-1000,0.78,0.78}; //{flag, PMT1, PMT2} Where to start the Int fits
 Double_t FitnpeMaxArray[3]={-1000,1.22,1.22};//{flag, PMT1, PMT2} Where to end the Int fits
 
 
 gausFitAmp1 = new TF1("gausFitAmp1", "gaus",FitAmpMinArray[1],FitAmpMaxArray[1]); // PMT 1
 gausFitInt1 = new TF1("gausFitInt1", "gaus",FitIntMinArray[1],FitIntMaxArray[1]);
 gausFitInt_zoom1 = new TF1("gausFitInt_zoom1", "gaus",FitIntMinArray[1],FitIntMaxArray[1]);
 gausFitnpe1 = new TF1("gausFitnpe1", "gaus",FitnpeMinArray[1],FitnpeMaxArray[1]);

 gausFitAmp2 = new TF1("gausFitAmp2", "gaus",FitAmpMinArray[2],FitAmpMaxArray[2]);// PMT 2
 gausFitInt2 = new TF1("gausFitInt2", "gaus",FitIntMinArray[2],FitIntMaxArray[2]);
 gausFitInt_zoom2 = new TF1("gausFitInt_zoom2", "gaus",FitIntMinArray[2],FitIntMaxArray[2]);
 gausFitnpe2 = new TF1("gausFitnpe2", "gaus",FitnpeMinArray[2],FitnpeMaxArray[2]);


 /*
 
 // graph the results on AdcPulseAmp from all the cuts and fit gaussian
 TCanvas *c15=new TCanvas("c15","c15",1000,700);
 c15->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c15->cd(counter);
  gPad->SetLogy(); 
  ng_goodAdcPulseAmp_multcut[counter] -> Fit(Form("gausFitAmp%d",counter),"R"); // comment out if you don't want to fit the ngcer   
  ng_goodAdcPulseAmp_multcut[counter]->SetLineColor(4);
  ng_goodAdcPulseAmp_multcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_multcut[counter]->Draw("SAME"); 
 }
 c15->Update();
 c15->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/cherenkov_calibration/HMS_CER_Calib_Plots/HMS_%s_cer_goodAdcPulseAmp.pdf",run_num));
 */
 // graph the results on AdcPulseInt from the cuts
 TCanvas *c16=new TCanvas("c16","c16",1000,700);
 c16->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c16->cd(counter);
  gPad->SetLogy();
  ng_goodAdcPulseInt_multcut_fit[counter] -> Fit(Form("gausFitInt_zoom%d",counter),"R"); // comment out if you don't want to fit the ngcer
  ng_goodAdcPulseInt_multcut_fit[counter]->SetLineColor(4);
  ng_goodAdcPulseInt_multcut_fit[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_multcut_fit[counter]->Draw("SAME");
 }
 c16->Update();
 c16->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/cherenkov_calibration/HMS_CER_Calib_Plots/HMS_%s_cer_goodAdcPulseInt_fit.pdf",run_num));

 TCanvas *c17=new TCanvas("c17","c17",1000,700);
 c17->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c17->cd(counter);
  gPad->SetLogy();
  ng_goodAdcPulseInt_multcut[counter] -> Fit(Form("gausFitInt%d",counter),"R"); // comment out if you don't want to fit the ngcer
  ng_goodAdcPulseInt_multcut[counter]->SetLineColor(4);
  ng_goodAdcPulseInt_multcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_multcut[counter]->Draw("SAME");
 }
 c17->Update();
 c17->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/cherenkov_calibration/HMS_CER_Calib_Plots/HMS_%s_cer_goodAdcPulseInt_new_replay1.pdf",run_num));


 Double_t mean[5];
 Double_t sigma[5];
 Double_t npe[5]; // "number of photoelectrons"
 Double_t spe[5]; // "integral per single photo electron"
 Double_t calic_const[5];
 mean[1]=gausFitInt1-> GetParameter(1);
 sigma[1]=gausFitInt1->GetParameter(2);
 mean[2]=gausFitInt2-> GetParameter(1);
 sigma[2]=gausFitInt2->GetParameter(2);
 
 cout<<"***************************************************************************"<<endl;
 cout<<"PulseInt Fit for Run "<<run_num<<endl;
 for (int i=1;i<3;i++){
   spe[i] = mean[i];
   cout<<"PMT"<<i<<": mean = "<<mean[i]<<"; sigma ="<<sigma[i]<<endl;
   //cout<<"PMT"<<i<<":npe = (mean/sigma)^2 = "<< npe[i] << "; spe = mean/npe = "<<spe[i]<<endl;
   cout<<"PMT"<<i<<":The calibration constant in the parameter file is 1/spe = "<<1/spe[i]<<endl;
 }

  cout<<"***************************************************************************"<<endl;

 
  //graph the H.cer.npe 
TCanvas *c19=new TCanvas("c19","c19",1000,700);
 c19-> Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c19->cd(counter);
   gPad->SetLogy();
   ngcer_npe_histo[counter]-> Fit(Form("gausFitnpe%d",counter),"R");
   ngcer_npe_histo[counter]->SetLineColor(kBlack);
   ngcer_npe_histo[counter]->SetLineWidth(2);
   ngcer_npe_histo[counter]->Draw("SAME");
  
 }
 c19-> Update();
  c19->SaveAs(Form("/u/group/c-polhe3/Users/murchhana/A1N_2020/cherenkov_calibration/HMS_CER_Calib_Plots/HMS_%s_cer_npe_new_replay1.pdf",run_num));

 


 
return 0;
}
 























