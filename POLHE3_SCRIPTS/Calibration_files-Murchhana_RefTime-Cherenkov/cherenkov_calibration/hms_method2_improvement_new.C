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

void hms_method2_improvement_new()
{

  gStyle->SetOptFit(1111); //setting stat bar options
  gStyle->SetStatFormat("6.6g");
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.01);
  
  const int nbinsx = 80;
  const int nbinsy = 80 ;
  int npmt=2;	
  //Declare root file
  char run_num[20];
  char nevents[20];
  cout<<"Enter HMS Run number (Including leading 0):"<<endl;
  cout<<"Enter HMS run number"<<endl;
  cin>> run_num;
  cout<<"Enter number of events"<<endl;  
  cin>> nevents;
  TString filename=Form("/group/c-polhe3/Users/murchhana/hallc_replay/ROOTfiles/hms_replay_production_all_%s_%s.root",run_num,nevents);
  TFile *fileIN = new TFile(filename);


 //Declare Tree
TTree *T =  (TTree*)fileIN->Get("T");

// Acquire the number of entries
Long64_t nentries;
nentries = T->GetEntries();

 Double_t ngcer_mult[3], ngcer_goodAdcPulseInt[3], ngcer_npeSum[3];
 Double_t ngcer_XatCer, ngcer_YatCer,cal_etracknorm,cal_eprtracknorm,delta;


  
T->SetBranchAddress("H.cer.goodAdcMult", &ngcer_mult);
//T->SetBranchAddress("H.cer.npeSum", &ngcer_npeSum);
T->SetBranchAddress("H.cer.npe", &ngcer_npeSum);
T->SetBranchAddress("H.gtr.dp", &delta);
T-> SetBranchAddress("H.cer.xAtCer",&ngcer_XatCer);
T-> SetBranchAddress("H.cer.yAtCer",&ngcer_YatCer);
T->SetBranchAddress("H.cal.etottracknorm", &cal_etracknorm); // shower + preshower energy (I'm not exactly sure how these are "normalized" but these are the variables Brad and Xiaochao wanted me to use)
T->SetBranchAddress("H.cal.eprtracknorm", &cal_eprtracknorm); // preshower energy


 TH1F *etracknorm;
 etracknorm=new TH1F("etracknorm","H.cal.etracknorm",100,0,2);
 TH1F *etracknorm_delta;
 etracknorm_delta=new TH1F("etracknorm_delta","H.cal.etracknorm{H.gtr.dp>-9 && h,gtr.dp<9}",100,0,2);
 TH1F *etracknorm_e;
 etracknorm_e=new TH1F("etracknorm_e","H.cal.etracknorm{H.gtr.dp>-9 && h,gtr.dp<9 && H.cer.npeSum>0.5 && H.cal.etracknorm>0.8}",100,0,2);
  TH1F *etracknorm_p;
 etracknorm_p=new TH1F("etracknorm_p","H.cal.etracknorm{H.gtr.dp>-9 && h,gtr.dp<9 && H.cer.npeSum<0.1 && H.cal.etracknorm<0.8}",100,0,2);


 TH2F *ngcer_xy;
 ngcer_xy=new TH2F("ngcer_xy","XY event Distribution",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_xyw;
 ngcer_xyw=new TH2F("ngcer_xyw","XY event Distribution with npe",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_npe;
 ngcer_npe=new TH2F("ngcer_npe","npe Distribution",nbinsx,-40,40,nbinsy,-40,40);

TH2F *ngcer_xy_e;
 ngcer_xy_e=new TH2F("ngcer_xy_e","XY electron event Distribution",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_xyw_e;
 ngcer_xyw_e=new TH2F("ngcer_xyw_e","XY electron event Distribution with npe",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_npe_e;
 ngcer_npe_e=new TH2F("ngcer_npe_e","electron npe Distribution",nbinsx,-40,40,nbinsy,-40,40);

TH2F *ngcer_xy_p;
 ngcer_xy_p=new TH2F("ngcer_xy_p","XY pion event Distribution",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_xyw_p;
 ngcer_xyw_p=new TH2F("ngcer_xyw_p","XY pion event Distribution with npe",nbinsx,-40,40,nbinsy,-40,40);
 TH2F *ngcer_npe_p;
 ngcer_npe_p=new TH2F("ngcer_npe_p","pion npe Distribution",nbinsx,-40,40,nbinsy,-40,40);

TH2F *npesum_etracknorm;
npesum_etracknorm=new TH2F("npesum_etracknorm","H.cer.npeSum : H.cal.etracknorm {H.gtr.dp>-0.9 && H.gtr.dp<9}",80,0,2,nbinsy,-40,40);



cout << "nentries= "<<nentries<<endl;
for(Long64_t ievent=0; ievent < nentries; ievent++){                                   //loop over no. of entries T->GetEntry(ievent);
T->GetEntry(ievent);

//if(delta > -0.9 && delta < 0.9){
// ngcer_xy->Fill(ngcer_XatCer,ngcer_YatCer);
   // ngcer_xyw->Fill(ngcer_XatCer,ngcer_YatCer,ngcer_npeSum);
   // npesum_etracknorm->Fill(cal_etracknorm,ngcer_npeSum);

   if(cal_etracknorm > 0.98 && cal_etracknorm < 1.35 && (cal_etracknorm-cal_eprtracknorm)>0.45 && (cal_etracknorm-cal_eprtracknorm) <1.1 && ngcer_mult[1]==1){
   ngcer_xy_e->Fill(ngcer_XatCer,ngcer_YatCer);
   ngcer_xyw_e->Fill(ngcer_XatCer,ngcer_YatCer,ngcer_npeSum[1]);
   }

   if(cal_etracknorm < 0.1){
   ngcer_xy_p->Fill(ngcer_XatCer,ngcer_YatCer);
   ngcer_xyw_p->Fill(ngcer_XatCer,ngcer_YatCer,ngcer_npeSum[0]);
   }
   //}
 }
//ngcer_npe=(TH2F*)ngcer_xyw->Clone();
//ngcer_npe->Divide(ngcer_xy);

ngcer_npe_e=(TH2F*)ngcer_xyw_e->Clone();
ngcer_npe_e->Divide(ngcer_xy_e);

ngcer_npe_p=(TH2F*)ngcer_xyw_p->Clone();
ngcer_npe_p->Divide(ngcer_xy_p);

/*
 TCanvas* c1 = new TCanvas("c1", "Mean NPE Distribution", 1800, 600);
 c1->Divide(3,1);

	c1->cd(1);
	ngcer_xy->Draw("colz");  
        ngcer_xy->GetXaxis()->SetTitle("X");
	ngcer_xy->GetYaxis()->SetTitle("Y");
	
	c1->cd(2);
	ngcer_xyw->Draw("colz");
	ngcer_xyw->GetXaxis()->SetTitle("X");
	ngcer_xyw->GetYaxis()->SetTitle("Y");
	
	c1->cd(3);
	ngcer_npe->Draw("colz");
	ngcer_xyw->GetXaxis()->SetTitle("X");
	ngcer_xyw->GetYaxis()->SetTitle("Y");
*/
TCanvas* c2 = new TCanvas("c2", "Mean NPE Distribution (Electrons)", 1800, 600);	

c2->Divide(3,1);

	c2->cd(1);
	ngcer_xy_e->Draw("colz");  
        ngcer_xy_e->GetXaxis()->SetTitle("X");
	ngcer_xy_e->GetYaxis()->SetTitle("Y");
	
	c2->cd(2);
	ngcer_xyw_e->Draw("colz");
	ngcer_xyw_e->GetXaxis()->SetTitle("X");
	ngcer_xyw_e->GetYaxis()->SetTitle("Y");
	
	c2->cd(3);
	ngcer_npe_e->Draw("colz");
	ngcer_xyw_e->GetXaxis()->SetTitle("X");
	ngcer_xyw_e->GetYaxis()->SetTitle("Y");


TCanvas* c3 = new TCanvas("c3", "Mean NPE Distribution (Pions)", 1800, 600);	
/*
c3->Divide(3,1);

	c3->cd(1);
	ngcer_xy_p->Draw("colz");  
        ngcer_xy_p->GetXaxis()->SetTitle("X");
	ngcer_xy_p->GetYaxis()->SetTitle("Y");
	
	c3->cd(2);
	ngcer_xyw_p->Draw("colz");
	ngcer_xyw_p->GetXaxis()->SetTitle("X");
	ngcer_xyw_p->GetYaxis()->SetTitle("Y");
	
	c3->cd(3);
	ngcer_npe_p->Draw("colz");
	ngcer_xyw_p->GetXaxis()->SetTitle("X");
	ngcer_xyw_p->GetYaxis()->SetTitle("Y");


TCanvas* c4 = new TCanvas("c4", "Cherenkov vs. calorimeter", 600, 600);

 npesum_etracknorm->Draw("colz");
 npesum_etracknorm->GetXaxis()->SetTitle("Total E/P");
 npesum_etracknorm->GetYaxis()->SetTitle("npeSum");
*/
return 0;
}
