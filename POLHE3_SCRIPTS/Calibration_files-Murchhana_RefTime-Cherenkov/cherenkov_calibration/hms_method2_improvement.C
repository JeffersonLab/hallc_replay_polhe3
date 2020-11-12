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

void hms_method2_improvement()
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
  cout<<"Enter HMS Run number (Including leading 0):"<<endl;
  cin>> run_num;

  TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_all_%s_200000.root",run_num);
  TFile *fileIN = new TFile(filename);


 //Declare Tree
TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries
Long64_t nentries;
nentries = T->GetEntries();

 Double_t ngcer_mult[3], ngcer_goodAdcPulseInt[3], ngcer_npe[3];
  Double_t ngcer_XatCer, ngcer_YatCer,cal_etracknorm,cal_eprtracknorm;


T->SetBranchAddress("H.cer.goodAdcPulseInt", &ngcer_goodAdcPulseInt);

T->SetBranchAddress("H.cer.npe", &ngcer_npe);
 T->SetBranchAddress("H.cer.goodAdcMult", &ngcer_mult);
 T-> SetBranchAddress("H.cer.xAtCer",&ngcer_XatCer);
 T-> SetBranchAddress("H.cer.yAtCer",&ngcer_YatCer);
 T->SetBranchAddress("H.cal.etottracknorm", &cal_etracknorm); // shower + preshower energy (I'm not exactly sure how these are "normalized" but these are the variables Brad and Xiaochao wanted me to use)
 T->SetBranchAddress("H.cal.eprtracknorm", &cal_eprtracknorm); // preshower energy



 std::vector< TCut > cuts = {"H.cer.goodAdcMult[0]==1 && H.cal.etottracknorm > 0.9 && H.cal.etottracknorm < 1.35 && (H.cal.etottracknorm-H.cal.eprtracknorm) > 0.45 && (H.cal.etottracknorm-H.cal.eprtracknorm) < 1.1"," H.cer.goodAdcMult[1]==1 && H.cal.etottracknorm > 0.9 && H.cal.etottracknorm < 1.35 && (H.cal.etottracknorm-H.cal.eprtracknorm) > 0.45 && (H.cal.etottracknorm-H.cal.eprtracknorm) < 1.1"};//d2n cuts

 // std::vector< TCut > cuts = {"H.cer.goodAdcMult[0]==1 && H.cal.eprtracknorm > 0.1 && H.cal.eprtracknorm < 0.8"," H.cer.goodAdcMult[1]==1 && H.cal.eprtracknorm > 0.1 && H.cal.eprtracknorm < 0.8"};

// XatCer, YatCer cuts
 Double_t XatCerCutLower[3]={-1000,4,-28}; //Old code is counting from 1
 Double_t XatCerCutUpper[3]={-1000,12,-3.5};
 Double_t YatCerCutLower[3]={-1000,4.4,-14};// {FLAG, PMT1, PMT2}
 Double_t YatCerCutUpper[3]={-1000,15,8};

 // make lines for the XY cuts
  TLine *XatCerLowerLines[3]; // old code is counting from 1
  TLine *YatCerLowerLines[3];
  TLine *XatCerUpperLines[3];
  TLine *YatCerUpperLines[3];
  //// TLine(x1,y1,x2,y2)
  for (int i=1;i<3;i++){//counting from 1   
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
        T->Draw(Form("H.cer.xAtCer:H.cer.yAtCer>>cleanEventDistrMult1(%i,-40,40,%i,-40,40)",nbinsy,nbinsx),cuts.at(0),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult1 = (TH2F*)gDirectory->Get("cleanEventDistrMult1");
	cleanEventDistrMult1->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c1->cd(2);
	T->Draw(Form("H.cer.xAtCer:H.cer.yAtCer>>cleanTotNpeDistrMult1(%i,-40,40,%i,-40,40)",nbinsy,nbinsx),cuts.at(0)*"H.cer.npe[0]","colz"); //clean electron x/y npe distribution
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
        T->Draw(Form("H.cer.xAtCer:H.cer.yAtCer>>cleanEventDistrMult2(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(1),"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistrMult2 = (TH2F*)gDirectory->Get("cleanEventDistrMult2");
	cleanEventDistrMult2->SetTitle("Clean Electron Event Distribution;Y Position;X Position");
	
	c2->cd(2);
	T->Draw(Form("H.cer.xAtCer:H.cer.yAtCer>>cleanTotNpeDistrMult2(%i,-40,40,%i,-60,20)",nbinsy,nbinsx),cuts.at(1)*"H.cer.npe[1]","colz"); //clean electron x/y npe distribution
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


  return 0;
}


















































  //TCut clean_electrons_w_mult1 = "H.cer.goodAdcMult[0]==1 && H.cal.etottracknorm > 0.9 && H.cal.etottracknorm < 1.25 && H.cal.eprtracknorm > 0.35 && H.cal.eprtracknorm < 1"; // Clean electrons with multiplicity cut
  //TCut clean_electrons_w_mult2 = " H.cer.goodAdcMult[1]==1 && H.cal.etottracknorm > 0.9 && H.cal.etottracknorm < 1.25 && H.cal.eprtracknorm > 0.35 && H.cal.eprtracknorm < 1"; // Clean electrons with multiplicity cut












   /*
  TH2F *cleanEventDistrMult[2];
  TH2F *cleanTotgoodAdcPulseIntDistrMult[2];
  TH2F *cleanMeangoodAdcPulseIntDistrMult[2];

for(Int_t counter = 1; counter < npmt+1; counter++){ 
  cleanEventDistrMult[counter-1]=new TH2F(Form("cleanEventDistrMult PMT%d %s",counter,run_num), Form("cleanEventDistrMult PMT%d %s",counter,run_num),nbins,-65,40,nbins,-65,40);
  cleanTotgoodAdcPulseIntDistrMult[counter-1]=new TH2F(Form("cleanTotgoodAdcPulseIntDistrMult PMT%d %s",counter,run_num), Form("cleanTotgoodAdcPulseIntDistrMult PMT%d %s",counter,run_num),nbins,-65,40,nbins,-65,40);
  cleanMeangoodAdcPulseIntDistrMult[counter-1]=new TH2F(Form("cleanMeangoodAdcPulseIntDistrMult PMT%d %s",counter,run_num), Form("cleanMeangoodAdcPulseIntDistrMult PMT%d %s",counter,run_num),nbins,-65,40,nbins,-65,40);
 }

cout << "nentries= "<<nentries<<endl;
 for(Long64_t ievent=0; ievent < nentries; ievent++){   //can be lowered for testing                       

   if (ievent % 10000 == 0){//printing progress updates to the terminal
    cout<< "ievent= "<<ievent <<endl;
   }

   T->GetEntry(ievent);
   for(Int_t counter = 1; counter < npmt+1; counter++){ 
  if(ngcer_mult[counter-1]==1){
   if(cal_etracknorm > 0.9 && cal_etracknorm < 1.25 && cal_eprtracknorm > 0.35 && cal_eprtracknorm < 1){
     cleanEventDistrMult[counter-1]->Fill(ngcer_XatCer,ngcer_YatCer);
     cleanTotgoodAdcPulseIntDistrMult[counter-1]->Fill(ngcer_XatCer*ngcer_npe[counter-1],ngcer_YatCer*ngcer_npe[counter-1]);
     //cleanMeangoodAdcPulseIntDistrMult[counter-1]->Fill(ngcer_XatCer,ngcer_YatCer);
    }
  } 
 }}


  // This canvas will display Mean NPE Distribution (NPE map)
  TCanvas* c1 = new TCanvas("c1", "Mean goodAdcPulseInt Distribution (With Multiplicity Cut)", 1800, 1800);
  c1->Divide(3,2);
  gPad->SetLogy();
  for(Int_t counter = 1; counter < npmt+1; counter++){ 
  int p=(3*(counter-1))+1;
	
  cout<<p<<endl;
  c1->cd(p);

  //tr->Draw("H.cer.xAtCer:H.cer.yAtCer>>cleanEventDistrMult",cuts.at((counter-1)),"goff"); //clean electron x/y event distrubtion
  cleanEventDistrMult[counter-1]->Draw("colz");
  // cleanEventDistrMult[counter-1] = (TH2F*)gDirectory->Get("cleanEventDistrMult");
  // cleanEventDistrMult[counter-1]->SetTitle("Clean Electron Event Distribution;X Position;Y Position");
	
  c1->cd(p+1);
  //tr->Draw("H.cer.xAtCer:H.cer.yAtCer>>cleanTotgoodAdcPulseIntDistrMult",cuts.at((counter-1))*"H.cer.goodAdcPulseInt","goff"); //clean electron x/y npe distribution
  cleanTotgoodAdcPulseIntDistrMult[counter-1]->Draw("colz");
  // cleanTotNpeDistrMult[counter-1]->SetTitle("Clean Electron Total NPE Distribution;X Position;Y Position");
	
  c1->cd(p+2);
  cleanMeangoodAdcPulseIntDistrMult[counter-1]= (TH2F*)cleanTotgoodAdcPulseIntDistrMult[counter-1]->Clone();
  cleanMeangoodAdcPulseIntDistrMult[counter-1]->Divide(cleanEventDistrMult[counter-1]); // clean electron x/y mean npe per event distribution (cleanTotNpeDistr/cleanEventDistr)
  
  cleanMeangoodAdcPulseIntDistrMult[counter-1]->Draw("colz");
  // cleanMeanNpeDistrMult[counter-1]->SetTitle("Clean Electron Mean NPE/Event Distribution;X Position;Y Position");
  
  }
  	
	// This canvas will display Intrinsic Efficiency Map
	TCanvas* c2 = new TCanvas("c2", "Intrinsic Efficiency Map", 1800, 600);
	c2->Divide(3,1);
	
	c2->cd(1);
	tr->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventDistr(%i,-40,40,%i,-40,40)",nbins,nbins),clean_electrons,"colz"); //clean electron x/y event distrubtion
	TH2F *cleanEventDistr = (TH2F*)gDirectory->Get("cleanEventDistr");
	cleanEventDistr->SetTitle("Clean Electron Event Distribution;X Position;Y Position");
	
	c2->cd(2);
	tr->Draw(Form("P.ngcer.xAtCer:P.ngcer.yAtCer>>cleanEventWithLightDistr(%i,-40,40,%i,-40,40)",nbins,nbins),clean_electrons_w_light,"colz"); //clean electron x/y events with light distribution
	TH2F *cleanEventWithLightDistr = (TH2F*)gDirectory->Get("cleanEventWithLightDistr");
	cleanEventWithLightDistr->SetTitle("Clean Electron Event With Light Distribution;X Position;Y Position");
	
	c2->cd(3);
	TH2F* effMap = (TH2F*)cleanEventWithLightDistr->Clone();
	effMap->Divide(cleanEventDistr);
	effMap->Draw("colz");
	effMap->GetZaxis()->SetRangeUser(0.9, 1.0);
	effMap->SetTitle("Intrinsic Efficiency Map;X Position;Y Position");
	
	
	//writing intrinsic efficiency map and mean npe map to 2d arrays and plotting bins against each other
	TCanvas* c3 = new TCanvas("c3","Scatter",1000,1000);
	TGraph* g = new TGraph();
	double intrEffMap[nbins][nbins];
	double meanNpeMap[nbins][nbins];
	for (int i = 1; i <= nbins; i++){
		for (int j = 1; j <= nbins; j++){
			intrEffMap[i-1][j-1] = effMap->GetBinContent(i,j);
			meanNpeMap[i-1][j-1] = cleanMeanNpeDistrMult->GetBinContent(i,j);
			g->SetPoint(g->GetN(), meanNpeMap[i-1][j-1], intrEffMap[i-1][j-1]);
		}
	}
	g->SetTitle("NGCER Efficiency Study;Mean NPE;Intrinsic Efficiency(%)");
	g->SetMarkerStyle(22);
	g->Draw("ap");
  */
