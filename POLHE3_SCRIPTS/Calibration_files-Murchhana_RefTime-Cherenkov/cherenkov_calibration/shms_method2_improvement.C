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

void shms_method2_improvement()
{

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.01);
  
  const int nbinsx = 240;
  const int nbinsy = 40 ;
  int npmt=2;	
  //Declare root file
  char run_num[20];
  cout<<"Enter SHMS Run number:"<<endl;
  cin>> run_num;

  TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/shms_replay_production_all_%s_200000.root",run_num);
  TFile *fileIN = new TFile(filename);


 //Declare Tree
TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries
Long64_t nentries;
nentries = T->GetEntries();

 Double_t ngcer_mult[3], ngcer_goodAdcPulseInt[3], ngcer_npe[3];
  Double_t ngcer_XatCer, ngcer_YatCer,cal_etracknorm,cal_eprtracknorm;


T->SetBranchAddress("P.ngcer.goodAdcPulseInt", &ngcer_goodAdcPulseInt);

T->SetBranchAddress("P.ngcer.npe", &ngcer_npe);
 T->SetBranchAddress("P.ngcer.goodAdcMult", &ngcer_mult);
 T-> SetBranchAddress("P.ngcer.xAtCer",&ngcer_XatCer);
 T-> SetBranchAddress("P.ngcer.yAtCer",&ngcer_YatCer);
 T->SetBranchAddress("P.cal.etottracknorm", &cal_etracknorm); // shower + preshower energy (I'm not exactly sure how these are "normalized" but these are the variables Brad and Xiaochao wanted me to use)
 T->SetBranchAddress("P.cal.eprtracknorm", &cal_eprtracknorm); // preshower energy



 std::vector< TCut > cuts = {"P.ngcer.goodAdcMult[0]==1 && P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.4 && (P.cal.etottracknorm-P.cal.eprtracknorm) > 0.25 && (P.cal.etottracknorm-P.cal.eprtracknorm) < 1.1"," P.ngcer.goodAdcMult[1]==1 && P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.4 && (P.cal.etottracknorm-P.cal.eprtracknorm) > 0.25 && (P.cal.etottracknorm-P.cal.eprtracknorm) < 1.1","P.ngcer.goodAdcMult[2]==1 && P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.4 && (P.cal.etottracknorm-P.cal.eprtracknorm) > 0.25 && (P.cal.etottracknorm-P.cal.eprtracknorm) < 1.1"," P.ngcer.goodAdcMult[3]==1 && P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.4 && (P.cal.etottracknorm-P.cal.eprtracknorm) > 0.25 && (P.cal.etottracknorm-P.cal.eprtracknorm) < 1.1"};//d2n cuts

 // std::vector< TCut > cuts = {"H.cer.goodAdcMult[0]==1 && H.cal.eprtracknorm > 0.1 && H.cal.eprtracknorm < 0.8"," H.cer.goodAdcMult[1]==1 && H.cal.eprtracknorm > 0.1 && H.cal.eprtracknorm < 0.8"};

// XatCer, YatCer cuts
 Double_t XatCerCutLower[5]={-1000,0,0,-29,-29}; //Old code is counting from 1
 Double_t XatCerCutUpper[5]={-1000,5.5,3.8,-0.2,-1.5};
 Double_t YatCerCutLower[5]={-1000,0,-13.7,1.5,-16};// {FLAG, PMT1, PMT2}
 Double_t YatCerCutUpper[5]={-1000,12,-2.2,16,0};

 // make lines for the XY cuts
  TLine *XatCerLowerLines[5]; // old code is counting from 1
  TLine *YatCerLowerLines[5];
  TLine *XatCerUpperLines[5];
  TLine *YatCerUpperLines[5];
  //// TLine(x1,y1,x2,y2)
  for (int i=1;i<5;i++){//counting from 1   
    XatCerLowerLines[i]= new TLine(-40,XatCerCutLower[i],40,XatCerCutLower[i]);
    XatCerLowerLines[i]->SetLineStyle(9);
    XatCerLowerLines[i]->SetLineColor(kRed);
    XatCerLowerLines[i]->SetLineWidth(1);

    XatCerUpperLines[i]= new TLine(-40,XatCerCutUpper[i],40,XatCerCutUpper[i]);
    XatCerUpperLines[i]->SetLineStyle(9);
    XatCerUpperLines[i]->SetLineColor(kRed);
    XatCerUpperLines[i]->SetLineWidth(1);

    YatCerLowerLines[i]= new TLine(YatCerCutLower[i],-60,YatCerCutLower[i],20);
    YatCerLowerLines[i]->SetLineStyle(9);
    YatCerLowerLines[i]->SetLineColor(kRed);
    YatCerLowerLines[i]->SetLineWidth(1);

    YatCerUpperLines[i]= new TLine(YatCerCutUpper[i],-60,YatCerCutUpper[i],20);
    YatCerUpperLines[i]->SetLineStyle(9);
    YatCerUpperLines[i]->SetLineColor(kRed);
    YatCerUpperLines[i]->SetLineWidth(1);
  }
 
 TCanvas* c1 = new TCanvas("c1", "Mean NPE Distribution (With Multiplicity Cut) PMT 1", 1800, 600);
	c1->Divide(3,1);

	c1->cd(1);
        T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventDistrMult1(%i,-40,40,%i,-40,40)",nbinsy,nbinsx),cuts.at(0),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult1 = (TH2F*)gDirectory->Get("cleanEventDistrMult1");
	cleanEventDistrMult1->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c1->cd(2);
	T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanTotNpeDistrMult1(%i,-40,40,%i,-40,40)",nbinsy,nbinsx),cuts.at(0)*"P.ngcer.npe[0]","colz"); //clean electron x/y npe distribution
	TH2F *cleanTotNpeDistrMult1 = (TH2F*)gDirectory->Get("cleanTotNpeDistrMult1");
	cleanTotNpeDistrMult1->SetTitle("Clean Electron Total NPE Distribution;Y Position;X Position");
	
	c1->cd(3);
	TH2F* cleanMeanNpeDistrMult1= (TH2F*)cleanTotNpeDistrMult1->Clone();
	cleanMeanNpeDistrMult1->Divide(cleanEventDistrMult1); // clean electron x/y mean npe per event distribution (cleanTotNpeDistr/cleanEventDistr)
	cleanMeanNpeDistrMult1->Draw("colz");
	cleanMeanNpeDistrMult1->SetTitle("Clean Electron Mean NPE/Event Distribution;Y Position;X Position");
	XatCerLowerLines[1] -> Draw();
	XatCerUpperLines[1] -> Draw();
	YatCerLowerLines[1] -> Draw();
	YatCerUpperLines[1] -> Draw();
	
 TCanvas* c2 = new TCanvas("c2", "Mean NPE Distribution (With Multiplicity Cut) PMT 2", 1800, 600);
	c2->Divide(3,1);

	c2->cd(1);
        T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventDistrMult2(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(1),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult2 = (TH2F*)gDirectory->Get("cleanEventDistrMult2");
	cleanEventDistrMult2->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c2->cd(2);
	T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanTotNpeDistrMult2(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(1)*"P.ngcer.npe[1]","colz"); //clean electron x/y npe distribution
	TH2F *cleanTotNpeDistrMult2 = (TH2F*)gDirectory->Get("cleanTotNpeDistrMult2");
	cleanTotNpeDistrMult2->SetTitle("Clean Electron Total NPE Distribution;Y Position;X Position");
	
	c2->cd(3);
	TH2F* cleanMeanNpeDistrMult2= (TH2F*)cleanTotNpeDistrMult2->Clone();
	cleanMeanNpeDistrMult2->Divide(cleanEventDistrMult2); // clean electron x/y mean npe per event distribution (cleanTotNpeDistr/cleanEventDistr)
	cleanMeanNpeDistrMult2->Draw("colz");
	cleanMeanNpeDistrMult2->SetTitle("Clean Electron Mean NPE/Event Distribution;Y Position;X Position");
	XatCerLowerLines[2] -> Draw();
	XatCerUpperLines[2] -> Draw();
	YatCerLowerLines[2] -> Draw();
	YatCerUpperLines[2] -> Draw();	

TCanvas* c3 = new TCanvas("c3", "Mean NPE Distribution (With Multiplicity Cut) PMT 3", 1800, 600);
	c3->Divide(3,1);

	c3->cd(1);
        T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventDistrMult3(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(2),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult3 = (TH2F*)gDirectory->Get("cleanEventDistrMult3");
	cleanEventDistrMult3->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c3->cd(2);
	T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanTotNpeDistrMult3(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(2)*"P.ngcer.npe[2]","colz"); //clean electron x/y npe distribution
	TH2F *cleanTotNpeDistrMult3 = (TH2F*)gDirectory->Get("cleanTotNpeDistrMult3");
	cleanTotNpeDistrMult3->SetTitle("Clean Electron Total NPE Distribution;Y Position;X Position");
	
	c3->cd(3);
	TH2F* cleanMeanNpeDistrMult3= (TH2F*)cleanTotNpeDistrMult3->Clone();
	cleanMeanNpeDistrMult3->Divide(cleanEventDistrMult3); // clean electron x/y mean npe per event distribution (cleanTotNpeDistr/cleanEventDistr)
	cleanMeanNpeDistrMult3->Draw("colz");
	cleanMeanNpeDistrMult3->SetTitle("Clean Electron Mean NPE/Event Distribution;Y Position;X Position");
	XatCerLowerLines[3] -> Draw();
	XatCerUpperLines[3] -> Draw();
	YatCerLowerLines[3] -> Draw();
	YatCerUpperLines[3] -> Draw();	


TCanvas* c4 = new TCanvas("c4", "Mean NPE Distribution (With Multiplicity Cut) PMT 4", 1800, 600);
	c4->Divide(3,1);

	c4->cd(1);
        T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventDistrMult4(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(3),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult4 = (TH2F*)gDirectory->Get("cleanEventDistrMult4");
	cleanEventDistrMult4->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c4->cd(2);
	T->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanTotNpeDistrMult4(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(3)*"P.ngcer.npe[3]","colz"); //clean electron x/y npe distribution
	TH2F *cleanTotNpeDistrMult4 = (TH2F*)gDirectory->Get("cleanTotNpeDistrMult4");
	cleanTotNpeDistrMult4->SetTitle("Clean Electron Total NPE Distribution;Y Position;X Position");
	
	c4->cd(3);
	TH2F* cleanMeanNpeDistrMult4= (TH2F*)cleanTotNpeDistrMult4->Clone();
	cleanMeanNpeDistrMult4->Divide(cleanEventDistrMult4); // clean electron x/y mean npe per event distribution (cleanTotNpeDistr/cleanEventDistr)
	cleanMeanNpeDistrMult4->Draw("colz");
	cleanMeanNpeDistrMult4->SetTitle("Clean Electron Mean NPE/Event Distribution;Y Position;X Position");
	XatCerLowerLines[4] -> Draw();
	XatCerUpperLines[4] -> Draw();
	YatCerLowerLines[4] -> Draw();
	YatCerUpperLines[4] -> Draw();	

  return 0;
}

