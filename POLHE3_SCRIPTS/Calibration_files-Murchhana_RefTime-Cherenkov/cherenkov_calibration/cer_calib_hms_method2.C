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

// Draft of a program to conduct ngcer calibration for SHMS by hand using "Method 2" from Simona. 
// https://logbooks.jlab.org/entry/3650663
// https://hallcweb.jlab.org/DocDB/0010/001053/001/wisewordsoncheranalysis.pdf
// murchhanaroy@uky.edu
// August 2020 


void cer_calib_hms_method2()
{

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.01);

//Declare root file
char run_num[20];
cout<<"Enter HMS Run number (Including leading 0):"<<endl;
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

//define the arrays we are going to put the branches in. 

 Double_t ngcer_goodAdcTdcDiffTime[4],ngcer_counter[30],ngcer_mult[30],ngcer_adcPulseTime[30],ngcer_goodAdcPulseAmp[4],ngcer_goodAdcPulseInt[4];
 Double_t cal_eprtracknorm, cal_etracknorm,ngcer_XatCer,ngcer_YatCer, delta, win; //good == array of 4. (This is what Brad said, he can probably explain more why). 
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

 ////Energy variables for the shower and preshower
 T->SetBranchAddress("H.cal.etottracknorm", &cal_etracknorm); // shower + preshower energy (I'm not exactly sure how these are "normalized" but these are the variables Brad and Xiaochao wanted me to use)
 T->SetBranchAddress("H.cal.eprtracknorm", &cal_eprtracknorm); // preshower energy
 T->SetBranchAddress("H.gtr.dp", &delta);
  T->SetBranchAddress("H.react.z", &win);

 //******************************************************************************************************************************************************************

 /*
The cuts that used on the ADC pulse integral (P.ngcer.goodAdcPulseInt) are the following:

-- H.cal.etottracknorm>0.8 to choose electrons.

-- H.gtr.dp > -9 && P.gtr.dp < 9.

-- timing window cuts (H.cer.goodAdcTdcDiffTime) (already set in the corresponding parameter file in hallc_replay )

--NGCER X and Y cuts for each PMT.

--Requiring good Adc multiplicity == 1 for the pmt undr calibration and multiplicity == 0 for the other pmts.
 */

//*******************************************************************************************************************************************************************

// Declare histos and fits. 

//Old code is counting from 1 so we need arrays of 5.

//histos of goodAdcPulseAmp and goodAdcPulseInt

 TH1F *ng_goodAdcPulseAmp[5]; //no cuts, only the branch itself
 TH1F *ng_goodAdcPulseInt[5];

 
 TH1F *ng_goodAdcPulseAmp_nocut[5]; //single pmt condition applied, non-physical cut applied,  no other cuts
 TH1F *ng_goodAdcPulseInt_nocut[5];

 TH1F *ng_goodAdcPulseAmp_Calcut[5]; // single pmt condition applied, only Calorimeter cut applied
 TH1F *ng_goodAdcPulseInt_Calcut[5];

 TH1F *ng_goodAdcPulseInt_PreShowercut[5];

 TH1F *ng_goodAdcPulseAmp_gtrcut[5]; // single pmt condition applied, only gtrcut applied
 TH1F *ng_goodAdcPulseInt_gtrcut[5];

 TH1F *ng_goodAdcPulseAmp_xycut[5]; //single pmt condition applied, only xy cuts applied
 TH1F *ng_goodAdcPulseInt_xycut[5];

 TH1F *ng_goodAdcPulseAmp_allcut[5]; // single pmt condition applied, all cuts applied
 TH1F *ng_goodAdcPulseInt_allcut[5];


 TH1F *ng_goodAdcTdcDiffTime[5]; // histo after cuts are applied 


 // histos of mirror

 TH2F *ng_xyCerRaw[5]; // 2D histo of whatever is in the branch

 TH1F *XatCerHistoRAW[5];// 1D histo of whatever in the branch
 TH1F *YatCerHistoRAW[5];

 TH2F *ng_xyCer_nocut[5]; // no shower cut and no single PMT cut
 TH2F *ng_xyCer_SinglePmt[5]; // no shower cut and single PMT cut
 TH2F *ng_xyCer_SinglePmt_Calcut[5]; // shower cut and "single PMT" cut

 TH1F *XatCerHisto_nocut[5]; // same as above, just in 1D so we can look at applying cuts to it more carefully
 TH1F *YatCerHisto_nocut[5];
 TH1F *XatCerHisto_SinglePmt[5];
 TH1F *YatCerHisto_SinglePmt[5];
 TH1F *XatCerHisto_SinglePmt_Calcut[5];
 TH1F *YatCerHisto_SinglePmt_Calcut[5];

 //histos of calorimeter energy

 TH2F *shwr_totalen; // Total Energy on Y, Shower energy on X
 TH2F *shwr_prshwr; ;// Shower Energy on Y, Preshower energy on X
 TH2F *prshwr_totalen; // Total Energy on Y, Preshower energy on X

 TH1F *TotalCalEnergyRAW; // before non-physical cuts, whatever in the branch
 TH1F *ShowerEnergyRAW;
 TH1F *PreshowerEnergyRAW;

 TH1F *TotalCalEnergy;// after non-physical cuts
 TH1F *ShowerEnergy;
 TH1F *PreshowerEnergy;

 //fitting histos

 TF1 *gausFitAmp1, *gausFitAmp2;
 TF1 *gausFitInt1, *gausFitInt2;
 TF1 *gausFitnpe1, *gausFitnpe2;

 TF1 *poisFitInt1, *poisFitInt2;
 //****************************************************************************************************************************************************************************

 //Define all histos

 //Make shower vs preshower energy 2D histo

 Double_t shwr_prshwr_yaxis_max =1.5; //variable to change the histogram axis (so we don't have to change it in the drawn cut lines)
 Double_t shwr_prshwr_xaxis_max =1.5;

 shwr_totalen = new TH2F(Form("total energy vs shower energy run #%s",run_num), Form("total energy vs shower energy run #%s",run_num), 300, 0, shwr_prshwr_xaxis_max, 300, 0, shwr_prshwr_yaxis_max);
 shwr_prshwr= new TH2F(Form("shower energy vs preshower energy run #%s",run_num), Form("shower energy vs preshower energy run #%s",run_num), 300, 0, shwr_prshwr_xaxis_max, 300, 0, shwr_prshwr_yaxis_max);
 prshwr_totalen= new TH2F(Form("total energy vs preshower energy run #%s" ,run_num), Form("total energy vs preshower energy run #%s",run_num), 300, 0, shwr_prshwr_xaxis_max, 300, 0, shwr_prshwr_yaxis_max);

 TotalCalEnergyRAW=new TH1F(Form("total energy RAW etracknorm run #%s",run_num),Form("total energy RAW etracknorm run #%s",run_num),200,0,2);
 ShowerEnergyRAW=new TH1F(Form("Shower Energy RAW (etracknorm - eprtracknorm) run #%s",run_num),Form("Shower Energy RAW (etracknorm - eprtracknorm) run #%s",run_num),200,0,2);
 PreshowerEnergyRAW=new TH1F(Form("Preshower Energy RAW eprtracknorm run #%s",run_num),Form("Preshower Energy RAW eprtracknorm run #%s",run_num),200,0,2);

 TotalCalEnergy=new TH1F(Form("total energy etracknorm run #%s",run_num),Form("total energy etracknorm run #%s",run_num),200,0,2);
 ShowerEnergy=new TH1F(Form("Shower Energy (etracknorm - eprtracknorm) run #%s",run_num),Form("Shower Energy (etracknorm - eprtracknorm) run #%s",run_num),200,0,2);
 PreshowerEnergy=new TH1F(Form("Preshower Energy eprtracknorm run #%s",run_num),Form("Preshower Energy eprtracknorm run #%s",run_num),200,0,2);

 

 //ng_xyCerRAW=new TH2F(Form("XatCer vs YatCer RAW run #%s",run_num), Form("XatCer vs YatCer RAW run # %s",run_num),600, -60, 40, 600, --60, 40);
 //ng_xyCer =new TH2F(Form("XatCer vs YatCer for all, run #%s",run_num), Form("XatCer vs YatCer for all, run #%s",run_num),400,-40,40,400,-40,40);
 

 // initializing histos that are for each PMT
 for(Int_t i=1; i<npmt+1; i++)
{
   TString namePulseAmp=Form("hNcerPulseAmp%d",i);  
   TString titlePulseAmp=Form("PMT%d: HMS CER goodAdcPulseAmp, Run %s",i,run_num);
   TString namePulseInt=Form("hNcerPulseInt%d",i);  
   TString titlePulseInt=Form("PMT%d: HMS CER goodAdcPulseInt, Run %s",i,run_num);

   TString namePulseAmp_nocut=Form("hNcerPulseAmp%d nocut",i);  
   TString titlePulseAmp_nocut=Form("PMT%d: HMS CER goodAdcPulseAmp, Run %s",i,run_num);
   TString namePulseInt_nocut=Form("hNcerPulseInt%d nocut",i);  
   TString titlePulseInt_nocut=Form("PMT%d: HMS CER goodAdcPulseInt nocut, Run %s",i,run_num);

   TString namePulseAmp_Calcut=Form("hNcerPulseAmp%d Calcut",i);  
   TString titlePulseAmp_Calcut=Form("PMT%d: HMS CER goodAdcPulseAmp Cal cut, Run %s",i,run_num);
   TString namePulseInt_Calcut=Form("hNcerPulseInt%d Calcut",i);  
   TString titlePulseInt_Calcut=Form("PMT%d: HMS CER goodAdcPulseInt Cal cut, Run %s",i,run_num);


   TString namePulseInt_PreShowercut=Form("hNcerPulseInt%d PreShowercut",i);  
   TString titlePulseInt_PreShowercut=Form("PMT%d: HMS CER goodAdcPulseInt PreShower cut, Run %s",i,run_num);

   TString namePulseAmp_gtrcut=Form("hNcerPulseAmp%d gtrcut",i);  
   TString titlePulseAmp_gtrcut=Form("PMT%d: HMS CER goodAdcPulseAmp gtr cut, Run %s",i,run_num);
   TString namePulseInt_gtrcut=Form("hNcerPulseInt%d gtrcut",i);  
   TString titlePulseInt_gtrcut=Form("PMT%d: HMS CER goodAdcPulseInt gtr cut, Run %s",i,run_num);

   TString namePulseAmp_xycut=Form("hNcerPulseAmp%d xycut",i);  
   TString titlePulseAmp_xycut=Form("PMT%d: HMS CER goodAdcPulseAmp xy cut Run %s",i,run_num);
   TString namePulseInt_xycut=Form("hNcerPulseInt%d xycut",i);  
   TString titlePulseInt_xycut=Form("PMT%d: HMS CER goodAdcPulseInt xy cut Run %s",i,run_num);

   TString namePulseAmp_allcut=Form("hNcerPulseAmp%d allcut",i);  
   TString titlePulseAmp_allcut=Form("PMT%d: HMS CER goodAdcPulseAmp Cal cut,gtr cut, xy cut Run %s",i,run_num);
   TString namePulseInt_allcut=Form("hNcerPulseInt%d allcut",i);  
   TString titlePulseInt_allcut=Form("PMT%d: HMS CER goodAdcPulseInt Cal cut, gtr cut, xy cut Run %s",i,run_num);
   
   
   
   ng_goodAdcPulseAmp[i]= new TH1F(namePulseAmp, titlePulseAmp,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt[i]= new TH1F(namePulseInt, titlePulseInt, 140, 0,140);// (200 for 9740) (50 for 9753)
   
   ng_goodAdcPulseAmp_nocut[i]= new TH1F(namePulseAmp_nocut, titlePulseAmp_nocut,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_nocut[i]= new TH1F(namePulseInt_nocut, titlePulseInt_nocut, 140,0,140);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_Calcut[i]= new TH1F(namePulseAmp_Calcut, titlePulseAmp_Calcut,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_Calcut[i]= new TH1F(namePulseInt_Calcut, titlePulseInt_Calcut, 140,0,140);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseInt_PreShowercut[i]= new TH1F(namePulseInt_PreShowercut, titlePulseInt_PreShowercut, 140,0,140);

   ng_goodAdcPulseAmp_gtrcut[i]= new TH1F(namePulseAmp_gtrcut, titlePulseAmp_gtrcut,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_gtrcut[i]= new TH1F(namePulseInt_gtrcut, titlePulseInt_gtrcut, 140,0,140);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_xycut[i]= new TH1F(namePulseAmp_xycut, titlePulseAmp_xycut,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_xycut[i]= new TH1F(namePulseInt_xycut, titlePulseAmp_xycut, 140,0,140);// (200 for 9740) (50 for 9753)

   ng_goodAdcPulseAmp_allcut[i]= new TH1F(namePulseAmp_allcut, titlePulseAmp_allcut,400,0,400); // (600 for 9740) (100 for 9753)
   ng_goodAdcPulseInt_allcut[i]= new TH1F(namePulseInt_allcut, titlePulseInt_allcut, 140,0,140);// (200 for 9740) (50 for 9753)

   ng_goodAdcTdcDiffTime[i]=new TH1F(Form("goodAdcTdcDiffTime PMT%d run %s",i,run_num),Form("goodAdcTdcDiffTime PMT%d run %s",i,run_num),150,-50,100);

   ng_xyCerRaw[i]=new TH2F(Form("YatCer vs XatCer Raw PMT%d %s",i,run_num), Form("YatCer vs XatCer Raw PMT%d %s",i,run_num),525, -65, 40, 400, -40, 40);
   ng_xyCer_nocut[i]=new TH2F(Form("YatCer vs XatCer PMT%d nocut %s",i,run_num), Form("YatCer vs XatCer PMT%d non-physical cuts applied, no other cuts %s",i,run_num),525, -65, 40, 400, -40, 40);
   ng_xyCer_SinglePmt[i]=new TH2F(Form("YatCer vs XatCer PMT%d SinglePmt %s",i,run_num), Form("YatCer vs XatCer PMT%d  SinglePmt %s",i,run_num),525, -65, 40, 400, -40, 40);
   ng_xyCer_SinglePmt_Calcut[i]=new TH2F(Form("YatCer vs XatCer PMT%d SinglePmt Cal cut %s",i,run_num), Form("YatCer vs XatCer PMT%d SinglePmt Calcut %s",i,run_num),525, -65, 40, 400, -40, 40);

   XatCerHisto_nocut[i] =new TH1F(Form("XatCerHisto_nocut PMT%d %s",i,run_num), Form("XatCerHisto PMT%d nocut %s",i,run_num),400,-65,40);
   YatCerHisto_nocut[i] =new TH1F(Form("YatCerHisto_nocut PMT%d %s",i,run_num), Form("YatCerHisto PMT%d nocut %s",i,run_num),400,-40,40);

   XatCerHisto_SinglePmt[i] =new TH1F(Form("XatCerHisto_SinglePmt PMT%d Single%s",i,run_num), Form("XatCerHisto PMT%d SinglePmt %s",i,run_num),400,-65,40);
   YatCerHisto_SinglePmt[i] =new TH1F(Form("YatCerHisto_SinglePmt PMT%d %s",i,run_num), Form("YatCerHisto PMT%d SinglePmt %s",i,run_num),400,-40,40);

   XatCerHisto_SinglePmt_Calcut[i] =new TH1F(Form("XatCerHisto_SinglePmt_Calcut PMT%d %s",i,run_num), Form("XatCerHisto SinglePmt Cal cut PMT%d %s",i,run_num),400,-65,40);
   YatCerHisto_SinglePmt_Calcut[i] =new TH1F(Form("YatCerHisto_SinglePmt_Calcut PMT%d %s",i,run_num), Form("YatCerHisto SinglePmt Cal cut PMT%d %s",i,run_num),400,-40,40);

   XatCerHistoRAW[i] =new TH1F(Form("XatCerHisto PMT%d RAW %s",i,run_num), Form("XatCerHisto PMT%d RAW %s",i,run_num),400,-65,40);
   YatCerHistoRAW[i] =new TH1F(Form("YatCerHisto PMT%d RAW %s",i,run_num), Form("YatCerHisto PMT%d RAW %s",i,run_num),400,-40,40);   
}

 //*************************************************************************************************************************************************************************************************

 //Assign shower cuts (should be based on results from the shower vs preshower 2D histo)
 Double_t cal_y_cut_lower, cal_y_cut_upper, cal_x_cut_lower, cal_x_cut_upper;
 // y-axis
 cal_y_cut_lower=0.9; //set to 0 if you don't want to cut the ngcer data. (0.95-1.15 for run 9740) (0-0.3 for 9573) (0.1-0.6 for 10337)
 cal_y_cut_upper=1.35; //set to a big number (ex. 2) if you don't want a cut
 // x- axis
 cal_x_cut_lower=0.45; //set to 0 if you don't want to cut the ngcer data. (0.45-0.75 for run 9740) (0-0.07 for 9573) (0.025-0.06 for 10337)
 cal_x_cut_upper=1.1; //set to a big number (ex. 2) if you don't want a cut   

 // XatCer, YatCer cuts
 Double_t XatCerCutLower[5]={-1000,-1,-27}; //Old code is counting from 1
 Double_t XatCerCutUpper[5]={-1000,20,-3};
 Double_t YatCerCutLower[5]={-1000,-12,-10};// {FLAG, PMT1, PMT2}
 Double_t YatCerCutUpper[5]={-1000,16,12};


//declare fits for 1D histos. Change as desired. 

Double_t FitAmpMinArray[3]={-1000,50,50}; // {flag, PMT1, PMT2} Where to start the Amp fits for each PMT
 Double_t FitAmpMaxArray[3]={-1000,130,130}; // {flag, PMT1, PMT2} Where to end the Amp fits

 Double_t FitIntMinArray[3]={-1000,20,20}; //{flag, PMT1, PMT2} Where to start the Int fits
 Double_t FitIntMaxArray[3]={-1000,58,65};//{flag, PMT1, PMT2} Where to end the Int fits

 Double_t FitnpeMinArray[3]={-1000,50,50}; //{flag, PMT1, PMT2} Where to start the Int fits
 Double_t FitnpeMaxArray[3]={-1000,120,120};//{flag, PMT1, PMT2} Where to end the Int fits
 
 gausFitAmp1 = new TF1("gausFitAmp1", "gaus",FitAmpMinArray[1],FitAmpMaxArray[1]); // PMT 1
 gausFitInt1 = new TF1("gausFitInt1", "gaus",FitIntMinArray[1],FitIntMaxArray[1]);
 gausFitnpe1 = new TF1("gausFitnpe1", "gaus",FitnpeMinArray[1],FitnpeMaxArray[1]);

 gausFitAmp2 = new TF1("gausFitAmp2", "gaus",FitAmpMinArray[2],FitAmpMaxArray[2]);// PMT 2
 gausFitInt2 = new TF1("gausFitInt2", "gaus",FitIntMinArray[2],FitIntMaxArray[2]);
 gausFitnpe2 = new TF1("gausFitnpe2", "gaus",FitnpeMinArray[2],FitnpeMaxArray[2]);

 poisFitInt1 = new TF1("poisFitInt1", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)",FitIntMinArray[1],FitIntMaxArray[1]);
 poisFitInt1->SetParameters(1000,44,7.9);
 poisFitInt2 = new TF1("poisFitInt2", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)",FitIntMinArray[2],FitIntMaxArray[2]);
 poisFitInt2->SetParameters(1000,44,7.9);
 //*******************************************************************************************************************************************************************************************************

 // Begining of analysis on all the events and applying cuts

// Loop of entries in tree
 cout << "nentries= "<<nentries<<endl;
 for(Long64_t ievent=0; ievent < nentries; ievent++){   //can be lowered for testing                       

   if (ievent % 10000 == 0){//printing progress updates to the terminal
    cout<< "ievent= "<<ievent <<endl;
   }

   T->GetEntry(ievent);
 
   //Filling up the RAW histos. Counting from 1. 
   for (int i=1; i<npmt+1;i++){ 
     XatCerHistoRAW[i] -> Fill(ngcer_XatCer);
     YatCerHistoRAW[i] -> Fill(ngcer_YatCer);
     ng_goodAdcPulseAmp[i]->Fill(ngcer_goodAdcPulseAmp[i-1]);
     ng_goodAdcPulseInt[i]->Fill(ngcer_goodAdcPulseInt[i-1]);
     ng_goodAdcTdcDiffTime[i]->Fill(ngcer_goodAdcTdcDiffTime[i-1]);
     ng_xyCerRaw[i]->Fill(ngcer_XatCer, ngcer_YatCer);
   }
  
   TotalCalEnergyRAW ->Fill(cal_etracknorm);
   ShowerEnergyRAW -> Fill(cal_etracknorm-cal_eprtracknorm);
   PreshowerEnergyRAW ->Fill(cal_eprtracknorm);
 
 
   ////Fill 2D histo for shower vs preshower energy.
   //// cut out any non-physical singnals. There is a line at x=y in the calorimeter that isn't physical. 
   if (ngcer_YatCer != 0 && ngcer_XatCer != 0){
     if(cal_eprtracknorm>0 && cal_etracknorm>0 && cal_eprtracknorm < cal_etracknorm){ 
       shwr_totalen -> Fill( cal_etracknorm-cal_eprtracknorm, cal_etracknorm); //Fill(x,y). Total Energy on Y, Shower energy on X
       shwr_prshwr -> Fill (cal_etracknorm-cal_eprtracknorm, cal_eprtracknorm);// Shower Energy on X, Preshower energy on Y
       prshwr_totalen -> Fill (cal_eprtracknorm , cal_etracknorm);// Total Energy on Y, Preshower energy on X
       TotalCalEnergy ->Fill(cal_etracknorm);
       ShowerEnergy -> Fill(cal_etracknorm-cal_eprtracknorm);
       PreshowerEnergy ->Fill(cal_eprtracknorm);
     //ng_xyCer -> Fill(ngcer_YatCer,ngcer_XatCer); //Fill(xaxis,yaxis)



       Bool_t singleFlagAmp=kTRUE; // flags to help identify if there was singnal in only one PMT at a time
       Bool_t singleFlagInt=kFALSE;

       // now we ARE CHECKING if only one PMT had a singnal 
       for (Int_t counterNew=1; counterNew< npmt+1; counterNew++){  // counting from 1

	 //apply mutiplicity cuts
	 if(ngcer_mult[counterNew-1]==1){

	   ng_xyCer_nocut[counterNew] -> Fill(ngcer_YatCer,ngcer_XatCer);

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
		   XatCerHisto_SinglePmt[counterNew] -> Fill(ngcer_XatCer);
		   YatCerHisto_SinglePmt[counterNew] -> Fill(ngcer_YatCer);
		   ng_xyCer_SinglePmt[counterNew] -> Fill(ngcer_XatCer,ngcer_YatCer);

		   ng_goodAdcPulseAmp_nocut[counterNew]->Fill(ngcer_goodAdcPulseAmp[counterNew-1]); //Fill histo with the goodAdcPulseAmp branch without any cuts 

	

		   // apply shower/calorimeter cut

		   //if(cal_etracknorm > 0.8){
		   if(cal_etracknorm > cal_y_cut_lower &&  cal_etracknorm < cal_y_cut_upper  && (cal_etracknorm-cal_eprtracknorm) > cal_x_cut_lower && (cal_etracknorm-cal_eprtracknorm) < cal_x_cut_upper){
		     XatCerHisto_SinglePmt_Calcut[counterNew] -> Fill(ngcer_XatCer);// fill xy histos
		     YatCerHisto_SinglePmt_Calcut[counterNew] -> Fill(ngcer_YatCer);
		     ng_xyCer_SinglePmt_Calcut[counterNew] -> Fill(ngcer_XatCer,ngcer_YatCer);

		     ng_goodAdcPulseAmp_Calcut[counterNew]-> Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
		   }// end shower cut 

		   //applying X Y cuts
		   if(ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew]){
		     ng_goodAdcPulseAmp_xycut[counterNew]-> Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
		   }//end of XY cuts

		   //applying gtr/delta cuts
		   if(delta > -9.0 && delta < 9.0){
		     ng_goodAdcPulseAmp_gtrcut[counterNew]-> Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
		   }//end of gtr cuts

		   //applying all cuts
		   //if(cal_etracknorm > 0.8 && ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew] && delta > -10.0 && delta < 22.0)
		   if(cal_etracknorm > cal_y_cut_lower &&  cal_etracknorm < cal_y_cut_upper  && (cal_etracknorm-cal_eprtracknorm) > cal_x_cut_lower && (cal_etracknorm-cal_eprtracknorm)< cal_x_cut_upper && ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew] && delta > -9.0 && delta < 9.0 && win > -14 && win <14)
		     {
		       ng_goodAdcPulseAmp_allcut[counterNew]-> Fill(ngcer_goodAdcPulseAmp[counterNew-1]);
		     }//end of all cuts   
		     	  
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
	       ng_goodAdcPulseInt_nocut[counterNew]->Fill(ngcer_goodAdcPulseInt[counterNew-1]); //Fill histo with the goodAdcPulseInt branch without shower cuts
	
	       //shower cut if
	       //if(cal_etracknorm > 0.8){
	     if(cal_etracknorm > cal_y_cut_lower &&  cal_etracknorm < cal_y_cut_upper  && (cal_etracknorm-cal_eprtracknorm) > cal_x_cut_lower && (cal_etracknorm-cal_eprtracknorm) < cal_x_cut_upper){
		 ng_goodAdcPulseInt_Calcut[counterNew]-> Fill(ngcer_goodAdcPulseInt[counterNew-1]);
	       }// end of shower calorimeter cuts
	     if((cal_eprtracknorm)>0.1 && (cal_eprtracknorm)<0.8){
	        ng_goodAdcPulseInt_PreShowercut[counterNew]-> Fill(ngcer_goodAdcPulseInt[counterNew-1]);
	     }
	       


	       //applying X Y cuts
	       if(ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew]){
		 ng_goodAdcPulseInt_xycut[counterNew]-> Fill(ngcer_goodAdcPulseInt[counterNew-1]);
		 // ng_goodAdcTdcDiffTime[counterNew]->Fill(ngcer_goodAdcTdcDiffTime[counterNew-1]);
	       }// end of XY cut
	
	       //applying gtr/delta cuts
	       if(delta > -9.0 && delta < 9.0){
		 ng_goodAdcPulseInt_gtrcut[counterNew]-> Fill(ngcer_goodAdcPulseInt[counterNew-1]);
	       }//end of gtr cuts

	       //applying all cuts
	       // if(cal_etracknorm > 0.8  && ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew] && delta > -8.0 && delta < 8.0)
	       if(cal_etracknorm > cal_y_cut_lower &&  cal_etracknorm < cal_y_cut_upper  && (cal_etracknorm-cal_eprtracknorm) > cal_x_cut_lower && (cal_etracknorm-cal_eprtracknorm) < cal_x_cut_upper && ngcer_XatCer >XatCerCutLower[counterNew] && ngcer_XatCer <XatCerCutUpper[counterNew] && ngcer_YatCer > YatCerCutLower[counterNew] && ngcer_YatCer <YatCerCutUpper[counterNew] && delta > -9.0 && delta < 9.0 && win < 14 && win > -14)
		 {
		   ng_goodAdcPulseInt_allcut[counterNew]-> Fill(ngcer_goodAdcPulseInt[counterNew-1]);
		 }//end of all cuts 
	
	     } //end of single PMT check  
	   }// end of AdcPulseInt analysis 

	 }//end of multiplicity==1 cut for the pmt under calibration
       } // end of single PMT analysis 
     }
   }//end of non-physical check
 }//end of event loop
 
 // Ending of analysis of events and applying all the cuts
 /***************************************************************************************************************************/

 
 //Graph the results!
 
 // graph the AdcPulseAmp histos
TCanvas *c1=new TCanvas("c1","c1",1000,700);
 c1->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c1->cd(counter);
  gPad->SetLogy();
  // ng_goodAdcPulseAmp[counter] -> Fit("gausFitAmp","R"); // comment out if you don't want to fit the ngcer 

  ng_goodAdcPulseAmp[counter]->SetLineColor(1);//black
  ng_goodAdcPulseAmp[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp[counter]->Draw();   

  ng_goodAdcPulseAmp_nocut[counter]->SetLineColor(3);//green
  ng_goodAdcPulseAmp_nocut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_nocut[counter]->Draw("SAME");

  ng_goodAdcPulseAmp_Calcut[counter]->SetLineColor(4);//blue
  ng_goodAdcPulseAmp_Calcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_Calcut[counter]->Draw("SAME");

  ng_goodAdcPulseAmp_gtrcut[counter]->SetLineColor(5);//yellow
  ng_goodAdcPulseAmp_gtrcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_gtrcut[counter]->Draw("SAME"); 

  ng_goodAdcPulseAmp_xycut[counter]->SetLineColor(6);//magenta
  ng_goodAdcPulseAmp_xycut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_xycut[counter]->Draw("SAME");  

  ng_goodAdcPulseAmp_allcut[counter]->SetLineColor(2);//red
  ng_goodAdcPulseAmp_allcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseAmp_allcut[counter]->Draw("SAME"); 

  TLegend *legend1 = new TLegend(0.1,0.6,0.5,0.9);
 
   legend1->AddEntry(ng_goodAdcPulseAmp[counter],"ng_goodAdcPulseAmp(raw branch)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_nocut[counter],"ng_goodAdcPulseAmp (mul=1, non-physical cut)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_Calcut[counter],"ng_goodAdcPulseAmp (mul=1, non-physical cut, cal cut)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_gtrcut[counter],"ng_goodAdcPulseAmp (mul=1, non-physical cut, gtr cut)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_xycut[counter],"ng_goodAdcPulseAmp (mul=1, non-physical cut, xy cut)","l");
   legend1->AddEntry(ng_goodAdcPulseAmp_allcut[counter],"ng_goodAdcPulseAmp (mul=1, all cuts)","l");
   legend1->Draw();
 }
 c1->Update();

 
 // Graph the AdcPulseInt histos
TCanvas *c2=new TCanvas("c2","c2",1000,700);
 c2->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c2->cd(counter);
  gPad->SetLogy();
  //ng_goodAdcPulseInt[counter] -> Fit("gausFitInt","R"); // comment out if you don't want to fit the ngcer
  
  ng_goodAdcPulseInt[counter]->SetLineColor(1);//black
  ng_goodAdcPulseInt[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt[counter]->Draw();   

  ng_goodAdcPulseInt_nocut[counter]->SetLineColor(3);//green
  ng_goodAdcPulseInt_nocut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_nocut[counter]->Draw("SAME");

  ng_goodAdcPulseInt_Calcut[counter]->SetLineColor(4);//blue
  ng_goodAdcPulseInt_Calcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_Calcut[counter]->Draw("SAME");


  ng_goodAdcPulseInt_PreShowercut[counter]->SetLineColor(28);//brown
  ng_goodAdcPulseInt_PreShowercut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_PreShowercut[counter]->Draw("SAME");

  ng_goodAdcPulseInt_gtrcut[counter]->SetLineColor(5);//yellow
  ng_goodAdcPulseInt_gtrcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_gtrcut[counter]->Draw("SAME"); 

  ng_goodAdcPulseInt_xycut[counter]->SetLineColor(6);//magenta
  ng_goodAdcPulseInt_xycut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_xycut[counter]->Draw("SAME");  

  ng_goodAdcPulseInt_allcut[counter]->SetLineColor(2);//red
  ng_goodAdcPulseInt_allcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_allcut[counter]->Draw("SAME");


 TLegend *legend2 = new TLegend(0.1,0.6,0.5,0.9);
 
   legend2->AddEntry(ng_goodAdcPulseInt[counter],"ng_goodAdcPulseInt(raw branch)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_nocut[counter],"ng_goodAdcPulseInt (mul=1, non-physical cut)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_PreShowercut[counter],"ng_goodAdcPulseInt (mul=1, non-physical cut,PreShower cut (0.1 < PrSh < 0.8))","l");
   legend2->AddEntry(ng_goodAdcPulseInt_Calcut[counter],"ng_goodAdcPulseInt (mul=1, non-physical cut, cal cut)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_gtrcut[counter],"ng_goodAdcPulseInt (mul=1, non-physical cut, gtr cut)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_xycut[counter],"ng_goodAdcPulseInt (mul=1, non-physical cut, xy cut)","l");
   legend2->AddEntry(ng_goodAdcPulseInt_allcut[counter],"ng_goodAdcPulseInt (mul=1, all cuts)","l");
   legend2->Draw();
 }
 c2->Update();



 // Create lines to show the shower calorimeter cuts. TLine(x1, y1, x2, y2)
 TLine *cal_y_lower_line=new TLine(0,cal_y_cut_lower,shwr_prshwr_xaxis_max,cal_y_cut_lower); // y axis
 TLine *cal_y_upper_line=new TLine(0,cal_y_cut_upper,shwr_prshwr_xaxis_max,cal_y_cut_upper); 
 TLine *cal_x_lower_line=new TLine(cal_x_cut_lower,0,cal_x_cut_lower,shwr_prshwr_yaxis_max); // x axis
  TLine *cal_x_upper_line=new TLine(cal_x_cut_upper,0,cal_x_cut_upper,shwr_prshwr_yaxis_max);
  cal_y_lower_line->SetLineStyle(9);
  cal_y_lower_line->SetLineColor(kRed);
  cal_y_lower_line->SetLineWidth(2);
  cal_y_upper_line->SetLineStyle(9);
  cal_y_upper_line->SetLineColor(kRed);
  cal_y_upper_line->SetLineWidth(2);
  cal_x_lower_line->SetLineStyle(9);
  cal_x_lower_line->SetLineColor(kRed);
  cal_x_lower_line->SetLineWidth(2);
  cal_x_upper_line->SetLineStyle(9);
  cal_x_upper_line->SetLineColor(kRed);
  cal_x_upper_line->SetLineWidth(2);

  // make lines for the XY cuts
  TLine *XatCerLowerLines[5]; // old code is counting from 1
  TLine *YatCerLowerLines[5];
  TLine *XatCerUpperLines[5];
  TLine *YatCerUpperLines[5];
  //// TLine(x1,y1,x2,y2)
  for (int i=1;i<5;i++){//counting from 1   
    XatCerLowerLines[i]= new TLine(XatCerCutLower[i],-40,XatCerCutLower[i],40);
    XatCerLowerLines[i]->SetLineStyle(9);
    XatCerLowerLines[i]->SetLineColor(kRed);
    XatCerLowerLines[i]->SetLineWidth(1);

    XatCerUpperLines[i]= new TLine(XatCerCutUpper[i],-40,XatCerCutUpper[i],40);
    XatCerUpperLines[i]->SetLineStyle(9);
    XatCerUpperLines[i]->SetLineColor(kRed);
    XatCerUpperLines[i]->SetLineWidth(1);

    YatCerLowerLines[i]= new TLine(-65,YatCerCutLower[i],40,YatCerCutLower[i]);
    YatCerLowerLines[i]->SetLineStyle(9);
    YatCerLowerLines[i]->SetLineColor(kRed);
    YatCerLowerLines[i]->SetLineWidth(1);

    YatCerUpperLines[i]= new TLine(-65,YatCerCutUpper[i],40,YatCerCutUpper[i]);
    YatCerUpperLines[i]->SetLineStyle(9);
    YatCerUpperLines[i]->SetLineColor(kRed);
    YatCerUpperLines[i]->SetLineWidth(1);
  }
 

  // graph the total energy on Y, shower energy on X
  TCanvas *c3=new TCanvas("c3","c3",1000,700);
  shwr_totalen -> Draw("colz");
  shwr_totalen->GetXaxis()->SetTitle("Shower energy");
  shwr_totalen->GetYaxis()->SetTitle("Total energy");
  cal_y_lower_line -> Draw();
  cal_y_upper_line -> Draw();
  cal_x_lower_line -> Draw();
  cal_x_upper_line -> Draw();
  c3->Update();

 // graph the XY histos for each PMT raw
 TCanvas *c11=new TCanvas("c11","c11",1000,700);
 c11-> Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c11->cd(counter);
   ng_xyCerRaw[counter]->Draw("colz");
    XatCerLowerLines[counter] -> Draw();
    XatCerUpperLines[counter] -> Draw();
    YatCerLowerLines[counter] -> Draw();
    YatCerUpperLines[counter] -> Draw();
   ng_xyCerRaw[counter]->GetXaxis()->SetTitle("YatCer");
   ng_xyCerRaw[counter]->GetYaxis()->SetTitle("XatCer");
 }
 c11-> Update();
 

// graph the XY histos for each PMT without the shower cuts and without single pmt cuts, only non-physical cuts 
 TCanvas *c12=new TCanvas("c12","c12",1000,700);
 c12-> Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c12->cd(counter);
   ng_xyCer_nocut[counter]->Draw("colz");
    XatCerLowerLines[counter] -> Draw();
    XatCerUpperLines[counter] -> Draw();
    YatCerLowerLines[counter] -> Draw();
    YatCerUpperLines[counter] -> Draw();
   ng_xyCer_nocut[counter]->GetXaxis()->SetTitle("XatCer");
   ng_xyCer_nocut[counter]->GetYaxis()->SetTitle("YatCer");
 }
 c12-> Update();
 

// graph the xy plots without the shower cut and with single pmt cut
 TCanvas *c17=new TCanvas("c17","c17",1000,700);
 c17-> Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c17->cd(counter);
   ng_xyCer_SinglePmt[counter]->Draw("colz");
   XatCerLowerLines[counter] -> Draw();
   XatCerUpperLines[counter] -> Draw();
   YatCerLowerLines[counter] -> Draw();
   YatCerUpperLines[counter] -> Draw();
   ng_xyCer_SinglePmt[counter]->GetXaxis()->SetTitle("XatCer");
   ng_xyCer_SinglePmt[counter]->GetYaxis()->SetTitle("YatCer");
 }
 c17-> Update();


 // graoh the xy plots. with shower cut, with single pmt cut
TCanvas *c18=new TCanvas("c18","c18",1000,700);
 c18-> Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c18->cd(counter);
   ng_xyCer_SinglePmt_Calcut[counter]->Draw("colz");
    XatCerLowerLines[counter] -> Draw();
    XatCerUpperLines[counter] -> Draw();
    YatCerLowerLines[counter] -> Draw();
    YatCerUpperLines[counter] -> Draw();
   ng_xyCer_SinglePmt_Calcut[counter]->GetXaxis()->SetTitle("XatCer");
   ng_xyCer_SinglePmt_Calcut[counter]->GetYaxis()->SetTitle("YatCer");
 }
 c18-> Update();



 // graphing the 1D X histos
TCanvas *c5=new TCanvas("c5","c5",1000,700);
 c5->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c5->cd(counter);
  gPad->SetLogy();
  
  XatCerHistoRAW[counter]->SetLineWidth(2);
  XatCerHistoRAW[counter]->SetLineColor(1);
  XatCerHistoRAW[counter]->Draw(); 

  XatCerHisto_SinglePmt[counter]->SetLineWidth(2);
  XatCerHisto_SinglePmt[counter]->SetLineColor(4);
  XatCerHisto_SinglePmt[counter]->Draw("SAME");

  XatCerHisto_SinglePmt_Calcut[counter]->SetLineWidth(2);
  XatCerHisto_SinglePmt_Calcut[counter]->SetLineColor(2);
  XatCerHisto_SinglePmt_Calcut[counter]->Draw("SAME");


  TLegend *legend3 = new TLegend(0.1,0.75,0.4,0.9);
 
   legend3->AddEntry(XatCerHistoRAW[counter],"XatCer(raw branch)","l");
   legend3->AddEntry(XatCerHisto_SinglePmt[counter],"XatCer (ngcer_mult=1, non-physical cut, single pmt)","l");
   legend3->AddEntry(XatCerHisto_SinglePmt_Calcut[counter],"XatCer (ngcer_mult=1, non-physical cut,single pmt, cal cut)","l");
   legend3->Draw();
 }
 c5->Update();


 // Graphing the 1D Y histos
TCanvas *c6=new TCanvas("c6","c6",1000,700);
 c6->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c6->cd(counter);
  gPad->SetLogy();
   
  YatCerHistoRAW[counter]->SetLineWidth(2);
  YatCerHistoRAW[counter]->SetLineColor(1);
  YatCerHistoRAW[counter]->Draw(); 

  YatCerHisto_SinglePmt[counter]->SetLineWidth(2);
  YatCerHisto_SinglePmt[counter]->SetLineColor(4);
  YatCerHisto_SinglePmt[counter]->Draw("SAME");

  YatCerHisto_SinglePmt_Calcut[counter]->SetLineWidth(2);
  YatCerHisto_SinglePmt_Calcut[counter]->SetLineColor(2);
  YatCerHisto_SinglePmt_Calcut[counter]->Draw("SAME"); 
  TLegend *legend4 = new TLegend(0.1,0.75,0.4,0.9);
 
   legend4->AddEntry(YatCerHistoRAW[counter],"YatCer(raw branch)","l");
   legend4->AddEntry(YatCerHisto_SinglePmt[counter],"YatCer (ngcer_mult=1, non-physical cut, single pmt)","l");
   legend4->AddEntry(YatCerHisto_SinglePmt_Calcut[counter],"YatCer (ngcer_mult=1, non-physical cut,single pmt, cal cut)","l");
   legend4->Draw();
 
 }
 c6->Update();



 TCanvas *c8=new TCanvas("c8","c8",1000,700);
 gPad->SetLogy();
 
 TLegend *EnergyLegend = new TLegend(0.7,0.2,0.9,0.6);

 TotalCalEnergy -> SetLineColor(kGreen);
 TotalCalEnergy -> SetLineWidth(2);
 EnergyLegend ->AddEntry(TotalCalEnergy,"Total Cal Energy (non-physical cut)","l");
 TotalCalEnergy -> Draw();

 ShowerEnergy -> SetLineColor(kBlue);
 EnergyLegend -> AddEntry(ShowerEnergy, "Total Shower Energy (non-physical cut)", "l");
 ShowerEnergy -> Draw("SAME");

 PreshowerEnergy -> SetLineColor(kRed);
 PreshowerEnergy -> SetLineWidth(2);
 EnergyLegend -> AddEntry(PreshowerEnergy, "Total PreShower Energy (non-physical cut)", "l");
 PreshowerEnergy -> Draw("SAME"); 

 EnergyLegend-> Draw("SAME");
 c8->Update();

 TCanvas *c13=new TCanvas("c13","c13",1000,700);
 gPad->SetLogy();
 
 TLegend *EnergyLegendRAW = new TLegend(0.7,0.2,0.9,0.6);

 TotalCalEnergyRAW -> SetLineColor(kGreen);
 TotalCalEnergyRAW-> SetLineWidth(2);
 EnergyLegendRAW->AddEntry(TotalCalEnergy,"Total Cal Energy (RAW)","l");
 TotalCalEnergyRAW-> Draw();

 ShowerEnergyRAW -> SetLineColor(kBlue);
 EnergyLegendRAW -> AddEntry(ShowerEnergy, "Total Shower Energy (RAW)", "l");
 ShowerEnergyRAW -> Draw("SAME");

 PreshowerEnergyRAW -> SetLineColor(kRed);
 PreshowerEnergyRAW -> SetLineWidth(2);
 EnergyLegendRAW -> AddEntry(PreshowerEnergy, "Total PreShower Energy (RAW)", "l");
 PreshowerEnergyRAW -> Draw("SAME"); 

 EnergyLegendRAW-> Draw("SAME");
 c13->Update();
 

 
 // graph the results on AdcPulseInt from all the cuts and fit gaussian
 TCanvas *c15=new TCanvas("c15","c15",1000,700);
 c15->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c15->cd(counter);
  gPad->SetLogy();
  ng_goodAdcPulseInt_allcut[counter] -> Fit(Form("gausFitInt%d",counter),"R"); // comment out if you don't want to fit the ngcer   
  ng_goodAdcPulseInt_allcut[counter]->SetLineColor(4);
  ng_goodAdcPulseInt_allcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_allcut[counter]->Draw("SAME");  
 }
 c15->Update();
 /*
 // graph the results on AdcPulseInt from the cuts and fir Poisson
 TCanvas *c16=new TCanvas("c16","c16",1000,700);
 c16->Divide(2,1);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
  c16->cd(counter);
  gPad->SetLogy();
  ng_goodAdcPulseInt_allcut[counter] -> Fit(Form("poisFitInt%d",counter),"R"); // comment out if you don't want to fit the ngcer
  ng_goodAdcPulseInt_allcut[counter]->SetLineColor(4);
  ng_goodAdcPulseInt_allcut[counter]->SetLineWidth(2);
  ng_goodAdcPulseInt_allcut[counter]->Draw("SAME");
 }
 c16->Update();
 */
 Double_t mean[3];
 Double_t sigma[3];
 Double_t npe[3]; // "number of photoelectrons"
 Double_t spe[3]; // "integral per single photo electron"
 Double_t calic_const[3];
 mean[1]=gausFitInt1-> GetParameter(1); 
 sigma[1]=gausFitInt1->GetParameter(2);
 mean[2]=gausFitInt2-> GetParameter(1);
 sigma[2]=gausFitInt2->GetParameter(2);

 
 cout<<"***************************************************************************"<<endl;
 cout<<"PulseInt Fit for Run "<<run_num<<endl;
 for (int i=1;i<3;i++){
   npe[i] = (mean[i] * mean[i])/(sigma[i] * sigma[i]);
   spe[i] = mean[i]/npe[i];
   cout<<"PMT"<<i<<": mean ="<<mean[i]<<" sigma ="<<sigma[i]<<endl;
   cout<<"PMT"<<i<<":npe =(mean/sigma)^2="<< npe[i] << ", spe = mean/npe="<<spe[i]<<endl;
   cout<<"PMT"<<i<<":The calibration constant in the parameter file is 1/spe ="<<1/spe[i]<<endl;
 }

  cout<<"***************************************************************************"<<endl;

  /* 
  //graph the effect of the cuts on the timing plot 
TCanvas *c19=new TCanvas("c19","c19",1000,700);
 c19-> Divide(2,2);
 for(Int_t counter = 1; counter < npmt+1; counter++){ 
   c19->cd(counter);
   gPad->SetLogy();
   ng_goodAdcTdcDiffTimeRAW[counter]->SetLineColor(kGreen);
   ng_goodAdcTdcDiffTimeRAW[counter]->SetLineWidth(2);
   ng_goodAdcTdcDiffTimeRAW[counter]->Draw();
   ng_goodAdcTdcDiffTime[counter]->SetLineColor(kMagenta);
   ng_goodAdcTdcDiffTime[counter]->Draw("SAME");
 }
 c19-> Update();
 */
 
return 0;
 
}

 

 





















