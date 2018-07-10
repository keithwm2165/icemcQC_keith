#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "TTreeIndex.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TRandom.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLine.h"
#include "TROOT.h"
#include "TPostScript.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TText.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include <unistd.h>
#include "TVector3.h"
#include "TRotation.h"
#include "TSpline.h"
#include "Math/InterpolationTypes.h"
#include "Math/Interpolator.h"
#include "signal.h"

#include "TGaxis.h"
#include "TPaveStats.h"

#include "Constants.h"
#include "Settings.h"
#include "position.hh"
#include "earthmodel.hh"
#include "Tools.h"
#include "vector.hh"
#include "roughness.hh"
#include "anita.hh"
#include "balloon.hh"
#include "icemodel.hh"
#include "trigger.hh"
#include "Spectra.h"
#include "signal.hh"
#include "secondaries.hh"
#include "ray.hh"
#include "counting.hh"
#include "Primaries.h"
#include "Taumodel.hh"

//#include <vector>


//TStyle* RootStyle(); 
//TStyle *color=RootStyle();

using namespace std;




int NNU;
double RANDOMISEPOL;

int main(int argc, char *argv[])
{
	gStyle->SetOptStat(111111);

  // gStyle=color;

	string readfile;
	if(argc<2)
	  {
	    readfile="outputs_update/icefinal.root";
	  }
	if(argc==2)
	  {
	    readfile=argv[1];
	  }
	
	TFile *AnitaFile = new TFile(( readfile ).c_str());
	cout << "AnitaFile" << endl;
	
	
	////////////////////////////////////////////////////////////////////////
	// Only Passing Events
	////////////////////////////////////////////////////////////////////////
	
	//Define Variables for tree:
	int num_pass;//number of passing events 
 
	double nnu[3];//trajectory of passing events

	double weight;//weight of passing events
	
	double costheta_nutraject;
	double phi_nutraject;
	double r_in[3];
	double r_enterice[3];
	double nuexit;
	double nuexitice;
	double d1;
	double d2;
	int currentint;
	double altitude_int;
	double r_fromballoon;
	double r_exit2bn;
	double r_exit2bn_measured;

	double logchord;
	double weight_bestcase;
	double chord_kgm2_bestcase;

	int nuflavorint;

	double dtryingdirection;
	double posnu[3];

	//get "passing_events" tree from AnitaFile and set branch addresses
	TTree *passing_events= (TTree*)AnitaFile->Get("passing_events");

	/*PRIMARIES VARIABLES*/
	passing_events->SetBranchAddress("nnu", &nnu);
	passing_events->SetBranchAddress("costheta_nutraject", &costheta_nutraject);
	passing_events->SetBranchAddress("phi_nutraject", &phi_nutraject);
	passing_events->SetBranchAddress("weight1", &weight);
	passing_events->SetBranchAddress("r_in", &r_in);
	passing_events->SetBranchAddress("r_enterice", &r_enterice);
      	passing_events->SetBranchAddress("nuexitlength", &nuexit);
	passing_events->SetBranchAddress("d1", &d1);
	passing_events->SetBranchAddress("d2", &d2);
	passing_events->SetBranchAddress("current", &currentint);
	passing_events->SetBranchAddress("altitude_int", &altitude_int);
	passing_events->SetBranchAddress("r_fromballoon", &r_fromballoon);
	passing_events->SetBranchAddress("r_exit2bn", &r_exit2bn);
	passing_events->SetBranchAddress("r_exit2bn_measured", &r_exit2bn_measured);
	passing_events->SetBranchAddress("nuflavor", &nuflavorint);
	passing_events->SetBranchAddress("posnu", &posnu);
	passing_events->SetBranchAddress("logchord", &logchord);
	passing_events->SetBranchAddress("nuexitice", &nuexitice);
	passing_events->SetBranchAddress("weight_bestcase", &weight_bestcase);
	passing_events->SetBranchAddress("chord_kgm2_bestcase", &chord_kgm2_bestcase);
	passing_events->SetBranchAddress("dtryingdirection", &dtryingdirection);
	num_pass= passing_events->GetEntries();
	cout << "num_pass is " << num_pass << endl;//check how many passing events there are

	double weight_array[num_pass];

	/*
	TTree *tree5= (TTree*)AnitaFile->Get("h5000");
	tree5->SetBranchAddress("logchord", &logchord);
	tree5->SetBranchAddress("nuexitice", &nuexitice);
	int tree5pass = tree5->GetEntries();
	cout << "tree5 is "<< tree5pass << endl;

	TTree *tree6= (TTree*)AnitaFile->Get("h6000");
	tree6->SetBranchAddress("weight_bestcase", &weight_bestcase);
	tree6->SetBranchAddress("chord_kgm2_bestcase", &chord_kgm2_bestcase);
	int tree6pass = tree6->GetEntries();
	cout << "tree6 is "<< tree6pass << endl;
	
	TTree *tree7= (TTree*)AnitaFile->Get("h7000");
	tree7->SetBranchAddress("nuflavor", &nuflavorint);
	
	TTree *tree4= (TTree*)AnitaFile->Get("h4000");
	tree4->SetBranchAddress("dtryingdirection", &dtryingdirection);

*/

	////////////////////////////////////////////////////////////////////////
	// Histograms
	////////////////////////////////////////////////////////////////////////


	//NNU cos(theta) and cos(phi) Histograms

	TCanvas *c1 = new TCanvas("c1", "NNU Graphs", 8000, 4000);
	c1->Divide(2);

	c1->cd(1);
       	TH1F *nnucostheta_hist = new TH1F("nnucostheta_hist", "Cos(theta) of NNU (Unweighted)", 50, -1, 1);
	c1->cd(2);
       	TH1F *nnuphi_hist = new TH1F("nnuphi_hist", "phi of NNU (Unweighted)", 50, 0, 6.3);

	double nnucostheta;
	double nnuphi;
	for (int k=0; k<num_pass; k++)
	  {
	    weight_array[k]=weight;
	    passing_events->GetEvent(k);
	    nnucostheta=nnu[2]/(sqrt(nnu[0]*nnu[0]+nnu[1]*nnu[1]+nnu[2]*nnu[2]));
	    nnucostheta_hist->Fill(nnucostheta); //kaeli
	    nnuphi=atan2(nnu[1],nnu[0]);
	    if (nnuphi<0)
	      {
		nnuphi=nnuphi+2*PI;
	      }
	    nnuphi_hist->Fill(nnuphi); //kaeli
	  }
	c1->cd(1);
	nnucostheta_hist->Draw();
	nnucostheta_hist->GetXaxis()->SetTitle("Cos(theta) of NNU");
	nnucostheta_hist->GetYaxis()->SetTitle("Number of Events");
	nnucostheta_hist->GetYaxis()->SetTitleOffset(1.5);
	c1->cd(2);
	nnuphi_hist->Draw();
	nnuphi_hist->GetXaxis()->SetTitle("Phi of NNU");
	nnuphi_hist->GetYaxis()->SetTitle("Number of Events");
	nnuphi_hist->GetYaxis()->SetTitleOffset(1.5);

	c1->Print("primaryplots/nnu_costheta_cosphi_unweighted.jpg");
	c1->Print("primaryplots/nnu_costheta_cosphi_unweighted.root");


	//costheta_nutraject Histogram
	double weight2;
	TCanvas *c2 = new TCanvas("c2", "costheta_nutraject", 8000,4000);
	TH1F *costheta_nutraject_hist = new TH1F("costheta_nutraject_hist", "Cos(theta) of Trajectory from Balloon Coordinate System", 50, -1, 1);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    costheta_nutraject_hist->Fill(costheta_nutraject, weight);
	    weight2=weight2+weight;
	  }
	//cout << "weight 2 is " << weight2 << endl;
	costheta_nutraject_hist->Draw();
	costheta_nutraject_hist->GetXaxis()->SetTitle("Cos(theta) of Trajectory");
	costheta_nutraject_hist->GetYaxis()->SetTitle("Number of Event(Weighted)");
	costheta_nutraject_hist->SetTitleOffset(1.5);
	/*
	double mean1=costheta_nutraject_hist->GetMean();

	TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
	leg->SetHeader("Title");
	leg->AddEntry(mean, );
	leg->(Draw);*/

	c2->Print("primaryplots/costheta_nutraject1.jpg");
	c2->Print("primaryplots/costheta_nutraject1.root");
	/*
	TCanvas *c2a = new TCanvas("c2a", "costheta_nutraject", 8000,8000);
	TH1F *costheta_nutraject_zoom = new TH1F("costheta_nutraject_zoom", "Cos(theta) of Trajectory from Balloon Coordinate System", 50, 0, .3);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    costheta_nutraject_zoom->Fill(costheta_nutraject, weight);
	  }
	costheta_nutraject_zoom->Draw();
	costheta_nutraject_zoom->GetXaxis()->SetTitle("Cos(theta) of Trajectory");
	costheta_nutraject_zoom->GetYaxis()->SetTitle("Number of Event(Weighted)");
	costheta_nutraject_zoom->SetTitleOffset(1.5);


	c2a->Print("primaryplots/costheta_nutraject_zoom.jpg");
*/

	//r_in graphs

	TCanvas *c3 = new TCanvas("c3", "r_in magnitude", 8000,4000);
	TH1F *r_in_hist = new TH1F("r_in_hist", "r_in Magnitude Histogram (Weighted)", 50, 6355000, 6365000);
	
	double weight3;
	double r_in_mag;
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    r_in_mag=sqrt(r_in[0]*r_in[0]+r_in[1]*r_in[1]+r_in[2]*r_in[2]);
	    r_in_hist->Fill(r_in_mag, weight);
	    weight3=weight3+weight;
	  }
	cout << "weight3 is " << weight3 << endl;
	r_in_hist->Draw();
	r_in_hist->GetXaxis()->SetTitle("r_in Magnitude (m)");
	r_in_hist->GetYaxis()->SetTitle("Number of Events(Weighted)");
	r_in_hist->GetYaxis()->SetTitleOffset(1.5);
	r_in_hist->GetXaxis()->SetNdivisions(5);

	c3->Print("primaryplots/r_in_hist.jpg");
	c3->Print("primaryplots/r_in_hist.root");


	TCanvas *c4 = new TCanvas("c4", "r_in cos(theta) and Phi", 18000, 6000);
	c4->Divide(2);
	c4->cd(1);
       	TH1F *rincostheta_hist = new TH1F("rincostheta_hist", "Cos(theta) of r_in (Weighted)", 50, 0, 1.1);
	c4->cd(2);
       	TH1F *rinphi_hist = new TH1F("rinphi_hist", "Phi of r_in (Weighted)", 50, 0, 6.3);

	double rincostheta;
	double rinphi;
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    rincostheta=r_in[2]/(sqrt(r_in[0]*r_in[0]+r_in[1]*r_in[1]+r_in[2]*r_in[2]));
	    rincostheta_hist->Fill(rincostheta, weight);
	    rinphi=atan2(r_in[1],r_in[0]);
	    if (rinphi<0)
	      {
		rinphi=rinphi+2*PI;
	      }
	    rinphi_hist->Fill(rinphi, weight);
	  }
	c4->cd(1);
	rincostheta_hist->Draw();
	rincostheta_hist->GetXaxis()->SetTitle("Cos(theta) of r_in");
	rincostheta_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	rincostheta_hist->GetYaxis()->SetTitleOffset(1.5);
	c4->cd(2);
	rinphi_hist->Draw();
	rinphi_hist->GetXaxis()->SetTitle("Phi of rin");
	rinphi_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	rinphi_hist->GetYaxis()->SetTitleOffset(1.5);

	c4->Print("primaryplots/rin_costheta_cosphi.jpg");
	c4->Print("primaryplots/rin_costheta_cosphi.root");

	TCanvas *c5a = new TCanvas("c5a","nuexitlength (magnitude)", 8000,4000);
	TH1F *nuexit_hist = new TH1F("nuexit_hist", "nuexit histogram",100, 0,2000000);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    nuexit_hist->Fill(nuexit, weight);
	  }
	nuexit_hist->Draw();
	nuexit_hist->GetXaxis()->SetTitle("magnitude of nuexit (m)");
	nuexit_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexit_hist->GetYaxis()->SetTitleOffset(0.5);

	c5a->Print("primaryplots/nuexit.jpg");
	c5a->Print("primaryplots/nuexit.root");
	
	/*
	TCanvas *c5 = new TCanvas("c5", "nuexit cos(theta) and phi", 18000, 6000);
	c5->Divide(2);
	c5->cd(1);
       	TH1F *nuexitcostheta_hist = new TH1F("nuexitcostheta_hist", "Cos(theta) of nuexit (Weighted)", 50, -1, 1);
	c5->cd(2);
       	TH1F *nuexitphi_hist = new TH1F("nuexitcosphi_hist", "Phi of nuexit (Weighted)", 50, -1, 1.1);

	double nuexitcostheta;
	double nuexitphi;
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    nuexitcostheta=nuexit[2]/(sqrt(nuexit[0]*nuexit[0]+nuexit[1]*nuexit[1]+nuexit[2]*nuexit[2]));
	    nuexitcostheta_hist->Fill(nuexitcostheta, weight);
	    nuexitphi=atan2(nuexit[1],nuexit[0]);
	    if (nuexitphi<0)
	      {
		nuexitphi=nuexitphi+2*PI;
	      }
	    nuexitphi_hist->Fill(nuexitphi, weight);
	  }
	c5->cd(1);
	nuexitcostheta_hist->Draw();
	nuexitcostheta_hist->GetXaxis()->SetTitle("Cos(theta) of nuexit");
	nuexitcostheta_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexitcostheta_hist->GetYaxis()->SetTitleOffset(1.5);
	c5->cd(2);
	nuexitphi_hist->Draw();
	nuexitphi_hist->GetXaxis()->SetTitle("Phi of nuexit");
	nuexitphi_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexitphi_hist->GetYaxis()->SetTitleOffset(1.5);

	c5->Print("primaryplots/nuexit_costheta_cosphi.jpg");
	*/
	
	TCanvas *c6 = new TCanvas("c6", "nuexitice", 8000,4000);
	c6->cd(1);
	TH1F *nuexitice_hist = new TH1F("nuexitice_hist", "nuexitice (weighted)",50, 6356000, 6362000);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    nuexitice_hist->Fill(nuexitice, weight);
	  }
	nuexitice_hist->Draw();
	nuexitice_hist->GetXaxis()->SetTitle("nuexitice (m)");
	nuexitice_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexitice_hist->GetYaxis()->SetTitleOffset(0.5);
	
	c6->Print("primaryplots/nuexitice.jpg");
	c6->Print("primaryplots/nuexitice.root");


	/*
	TCanvas *c6 = new TCanvas("c6", "nuexitice cos theta cos phi", 18000, 6000);
	c6->Divide(2);
	c6->cd(1);
       	TH1F *nuexiticecostheta_hist = new TH1F("nuexiticecostheta_hist", "Cos(theta) of nuexitice (Weighted)", 50, -1, 1);
	c6->cd(2);
       	TH1F *nuexiticephi_hist = new TH1F("nuexiticecosphi_hist", "Phi of nuexitice (Weighted)", 50, -1, 1.1);

	double nuexiticecostheta;
	double nuexiticephi;
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    // cout << nuexitice[0] << endl;
	    //cout << nuexitice[1] << endl;
	    //cout << nuexitice[2] << endl;
	    nuexiticecostheta=nuexitice[2]/(sqrt(nuexitice[0]*nuexitice[0]+nuexitice[1]*nuexitice[1]+nuexitice[2]*nuexitice[2]));
	    nuexiticecostheta_hist->Fill(nuexiticecostheta, weight);
	    nuexiticephi=nuexitice[0]/(sqrt(nuexitice[0]*nuexitice[0]+nuexitice[1]*nuexitice[1]));
	    nuexiticephi=atan2(nuexitice[1],nuexitice[0]);
	    if (nuexiticephi<0)
	      {
		nuexiticephi=nuexiticephi+2*PI;
	      }
	    nuexiticephi_hist->Fill(nuexiticephi, weight);
	  }
	c6->cd(1);
	nuexiticecostheta_hist->Draw();
	nuexiticecostheta_hist->GetXaxis()->SetTitle("Cos(theta) of nuexit");
	nuexiticecostheta_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexiticecostheta_hist->GetYaxis()->SetTitleOffset(1.5);
	c6->cd(2);
	nuexiticephi_hist->Draw();
	nuexiticephi_hist->GetXaxis()->SetTitle("Phi of nuexit");
	nuexiticephi_hist->GetYaxis()->SetTitle("Number of Events (Weighted)");
	nuexiticephi_hist->GetYaxis()->SetTitleOffset(1.5);

	c6->Print("primaryplots/nuexitice_costheta_cosphi.jpg");
	c6->Print("primaryplots/nuexitice_costheta_cosphi.jpg");
	*/


	/*
	double nu_diff;
	double cos_theta;
	TCanvas *c8 = new TCanvas("c8", "nuexit difference vs. cos theta", 8000,8000);
       	TH2F *nu_diff_costheta = new TH2F("nu_diff_costheta", "Absolute Difference between nuexit and nuexitice vs. cos(theta) of nnu (Weighted)", 50, 0, 10000, 50, -1, 1);

	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    nu_diff=abs( sqrt(nuexit[0]*nuexit[0]+nuexit[1]*nuexit[1]+nuexit[2]*nuexit[2])-sqrt(nuexitice[0]*nuexitice[0]+nuexitice[1]*nuexitice[1]+nuexitice[2]*nuexitice[2]));
	    cos_theta=nnu[2]/(sqrt(nnu[0]*nnu[0]+nnu[1]*nnu[1]+nnu[2]*nnu[2]));
	    nu_diff_costheta->Fill(nu_diff,cos_theta,weight);
	  }
	nu_diff_costheta->GetXaxis()->SetTitle("Absolute Difference between nuexit and nuexitice");
	nu_diff_costheta->GetYaxis()->SetTitle("Cos(theta) of nnu");
	nu_diff_costheta->SetTitle("(nuexit-nuexitice) vs. cos(theta)");
	nu_diff_costheta->Draw("p");
	c8->Print("primaryplots/nu_diff_costheta.jpg");
*/
	TCanvas *c7 = new TCanvas("c7", "logchord", 8000,4000);
	TH1F *logchord_hist = new TH1F("logchord_hist", "logchord Histogram (Weighted)", 50, 0, 8);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    //cout << logchord << endl;
	    logchord_hist->Fill(logchord, weight);
	  }
	logchord_hist->Draw();
	logchord_hist->GetXaxis()->SetTitle("log base 10 of chord");
	logchord_hist->GetYaxis()->SetTitle("Number of Event(Weighted)");
	logchord_hist->GetYaxis()->SetTitleOffset(0.5);

	c7->Print("primaryplots/logchord.jpg");
	c7->Print("primaryplots/logchord.root");


	TCanvas *c9 = new TCanvas("c9", "weight_bestcase", 8000,4000);
	TH1F *weight_bestcase_hist = new TH1F("weight_bestcase_hist", "weight_bestcase Histogram (Unweighted)",50,0,1);
	for (int k=0; k<num_pass;k++)
	  {
	    passing_events->GetEvent(k);
	    weight_bestcase_hist->Fill(weight_bestcase);
	  }
	weight_bestcase_hist->Draw();
	weight_bestcase_hist->GetXaxis()->SetTitle("weight_bestcase");
	weight_bestcase_hist->GetYaxis()->SetTitle("Number of events");
	weight_bestcase_hist->SetTitleOffset(1.5);
	weight_bestcase_hist->GetYaxis()->SetTitleOffset(1.5);
	c9->Print("primaryplots/weight_bestcase.jpg");
	c9->Print("primaryplots/weight_bestcase.root");

	TCanvas *c10 = new TCanvas("c10", "chord_kgm2_bestcase", 8000,4000);
	TH1F *chord_kgm2_bestcase_hist = new TH1F("chord_kgm2_bestcase_hist","chord_kgm2_bestcase histogram (weighted)", 50,0,12e9);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    chord_kgm2_bestcase_hist->Fill(chord_kgm2_bestcase);
	  }
	chord_kgm2_bestcase_hist->Draw(); 
	chord_kgm2_bestcase_hist->GetXaxis()->SetTitle("chord_kgm2_bestcase");
	chord_kgm2_bestcase_hist->GetYaxis()->SetTitle("Number of events");
	chord_kgm2_bestcase_hist->GetYaxis()->SetTitleOffset(1.5);
	c10->Print("primaryplots/chord_kgm2_bestcase.jpg");
	c10->Print("primaryplots/chord_kgm2_bestcase.root");


	TCanvas *c12 = new TCanvas("c12", "d1", 8000,4000);
	TH1F *d1_hist = new TH1F("d1_hist","d1 histogram (unweighted)",50,0,1e6);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    d1_hist->Fill(d1);
	  }
	d1_hist->Draw();
	d1_hist->GetXaxis()->SetTitle("d1:chord from earth entrance to rock-ice boundary (m)");
	d1_hist->GetYaxis()->SetTitle("Number of Events");
	c12->Print("primaryplots/d1.jpg");
	c12->Print("primaryplots/d1.root");


	TCanvas *c12a = new TCanvas("c12a", "d1", 8000,4000);
	TH1F *d1_hist_zoom = new TH1F("d1_hist_zoom","d1 histogram (unweighted, zoomed)",50,0,1e5);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    d1_hist_zoom->Fill(d1);
	  }
	d1_hist_zoom->Draw();
	d1_hist_zoom->GetXaxis()->SetTitle("d1:chord from earth entrance to rock-ice boundary (m)");
	d1_hist_zoom->GetYaxis()->SetTitle("Number of Events");
	c12a->Print("primaryplots/d1_zoom.jpg");
	c12a->Print("primaryplots/d1_zoom.root");


	TCanvas *c13 = new TCanvas("c13", "d2", 8000,4000);
	TH1F *d2_hist = new TH1F("d2_hist","d2 histogram (unweighted)",50,0,1e6);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    //cout << d2 << endl;
	    d2_hist->Fill(d2);
	  }
	d2_hist->Draw();
	d2_hist->GetXaxis()->SetTitle("d2: ice-rock boundary to interaction point (m)");
	d2_hist->GetYaxis()->SetTitle("Number of Events");
	c13->Print("primaryplots/d2.jpg");
	c13->Print("primaryplots/d2.root");


	TCanvas *c14 = new TCanvas("c14", "nuflavorint", 8000,4000);
	TH1F *nuflavorint_hist = new TH1F("nuflavorint_hist", "nuflavorint histogram",3,.5,3.5);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    nuflavorint_hist->Fill(nuflavorint, weight);
	    //cout << nuflavorint << endl;
	  }
	nuflavorint_hist->Draw();
	nuflavorint_hist->GetXaxis()->SetTitle("Nuetrino Flavor");
	nuflavorint_hist->GetYaxis()->SetTitle("Number of Events(weighted)");
	nuflavorint_hist->GetYaxis()->SetTitleOffset(1.5);
	c14->Print("primaryplots/nuflavorint.jpg");
	c14->Print("primaryplots/nuflavorint.root");


	TCanvas *c15 = new TCanvas("c15", "currentint", 8000,4000);
	TH1F *currentint_hist = new TH1F("currentint_hist", "currentint histogram", 2, -0.5, 1.5);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    currentint_hist->Fill(currentint, weight);
	  }
	currentint_hist->Draw();
	currentint_hist->GetXaxis()->SetTitle("Current: 0 is neutral, 1 is charged");
	currentint_hist->GetYaxis()->SetTitle("Number of Events(weighted)");
	currentint_hist->GetYaxis()->SetTitleOffset(1.5);
	c15->Print("primaryplots/currentint.jpg");
	c15->Print("primaryplots/currentint.root");

	TCanvas *c16 = new TCanvas("c16", "dtryingdirection", 8000,4000);
	TH1F *dtrying_hist = new TH1F("dtrying_hist", "dtryingdirection histogram", 25, 0, 2);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    dtrying_hist->Fill(dtryingdirection);
	  }
	dtrying_hist->Draw();
	dtrying_hist->GetXaxis()->SetTitle("dtryingdirection");
	dtrying_hist->GetYaxis()->SetTitle("Number of Events");
	dtrying_hist->GetYaxis()->SetTitleOffset(1.5);
	c16->Print("primaryplots/dtryingdirection.jpg");
	c16->Print("primaryplots/dtryingdirection.root");

	TCanvas *c17 = new TCanvas("c17", "altitude_int", 8000,4000);
	TH1F *altitude_hist = new TH1F("altitude_hist", "altitude_int histogram", 50,-3500,0.1);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    altitude_hist->Fill(altitude_int, weight);
	  }
	altitude_hist->Draw();
	altitude_hist->GetXaxis()->SetTitle("altitude_int (m)");
	altitude_hist->GetYaxis()->SetTitle("Number of Events (weighted)");
	altitude_hist->GetYaxis()->SetTitleOffset(1.5);
	c17->Print("primaryplots/altitude_int.jpg");
	c17->Print("primaryplots/altitude_int.root");

	/*
	TCanvas *c18 = new TCanvas("c18", "altitude_int_mirror", 8000,8000);
	TH1F *altitude_mirror_hist = new TH1F("altitude_mirror_hist", "altitude_int_mirror histogram", 50,0,1);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    altitude_hist->Fill(altitude_int_mirror, weight);
	  }
	altitude_mirror_hist->Draw();
	altitude_mirror_hist->GetXaxis()->SetTitle("altitude_int_mirror");
	altitude_mirror_hist->GetYaxis()->SetTitle("Number of Events (weighted)");
	c18->Print("primaryplots/altitude_int_mirror.jpg");
	*/


	TCanvas *c19 = new TCanvas("c19","r_fromballoon", 8000,4000);
	TH1F *r_fromballoon_hist = new TH1F("r_fromballoon_hist", "r_fromballoon histogram", 50, 0, 800000);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    r_fromballoon_hist->Fill(r_fromballoon, weight);
	    //  cout << r_fromballoon << endl;
	  }
	r_fromballoon_hist->Draw();
	r_fromballoon_hist->GetXaxis()->SetTitle("r_fromballoon (m)");
	r_fromballoon_hist->GetYaxis()->SetTitle("Number of Events(weighted)");
	c19->Print("primaryplots/r_fromballoon.jpg");
	c19->Print("primaryplots/r_fromballoon.root");

	TCanvas *c20 = new TCanvas("c20","r_exit2bn", 8000,4000);
	TH1F *r_exit2bn_hist = new TH1F("r_exit2bn_hist","r_exit2bn histogram",50,0,700000);
	for (int k=0; k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    r_exit2bn_hist->Fill(r_exit2bn, weight);
	    //   cout << r_exit2bn << endl;
	  }
	r_exit2bn_hist->Draw();
	r_exit2bn_hist->GetXaxis()->SetTitle("r_exit2bn (m)");
	r_exit2bn_hist->GetYaxis()->SetTitle("Number of Events(weighted)");
	r_exit2bn_hist->GetYaxis()->SetTitleOffset(1.5);
	c20->Print("primaryplots/r_exit2bn.jpg");
	c20->Print("primaryplots/r_exit2bn.root");

	TCanvas *c21 = new TCanvas("c21", "r_exit2bn_measured", 8000,4000);
	TH1F *r_exit2bn_measured_hist = new TH1F("r_exit2bn_measured_hist", "r_exit2bn_measured histogram", 50, 0, 700000);
	for (int k=0;k<num_pass; k++)
	  {
	    passing_events->GetEvent(k);
	    r_exit2bn_measured_hist->Fill(r_exit2bn, weight);
	    // cout << r_exit2bn_measured << endl;
	  }
	r_exit2bn_measured_hist->Draw();
	r_exit2bn_measured_hist->GetXaxis()->SetTitle("r_exit2bn_measured (m)");
	r_exit2bn_measured_hist->GetYaxis()->SetTitle("Number of Events(weighted)");
	r_exit2bn_measured_hist->GetYaxis()->SetTitleOffset(1.5);
	c21->Print("primaryplots/r_exit2bn_measured.jpg");
	c21->Print("primaryplots/r_exit2bn_measured.root");

	double posnu_x[num_pass];
	double posnu_y[num_pass];
	double posnu_z[num_pass];

	for (int i=0; i<num_pass; i++)
	  {
	    passing_events->GetEvent(i);
	    posnu_x[i]=posnu[0];
	    posnu_y[i]=posnu[1];
	    posnu_z[i]=posnu[2];
	  }

	TCanvas *c22 = new TCanvas("c22", "Posnu Graphs", 18000, 6000);
	c22->Divide(3);

	TGraph *gr_posXY;
	gr_posXY = new TGraph(num_pass, posnu_x, posnu_y);

	c22->cd(1);
	gr_posXY->SetTitle("posnuXY");
	gr_posXY->GetHistogram()->SetXTitle("X (m)");
	gr_posXY->GetHistogram()->SetYTitle("Y (m)");
	gr_posXY->GetYaxis()->SetTitleOffset(1.2);
	gr_posXY->Draw("a*");
	

	c22->cd(2);
	TGraph *gr_posYZ;
	gr_posYZ = new TGraph(num_pass, posnu_y, posnu_z);
	gr_posYZ->SetTitle("posnuYZ");
	gr_posYZ->GetHistogram()->SetXTitle("Y (m)");
	gr_posYZ->GetHistogram()->SetYTitle("Z (m)");
	gr_posYZ->GetYaxis()->SetTitleOffset(1.2);
	gr_posYZ->Draw("a*");


	c22->cd(3);
	TGraph *gr_posXZ;
	gr_posXZ = new TGraph(num_pass, posnu_x, posnu_z);
	gr_posXZ->SetTitle("posnuXZ");
	gr_posXZ->GetHistogram()->SetXTitle("X (m)");
	gr_posXZ->GetHistogram()->SetYTitle("Z (m)");
	gr_posXZ->GetYaxis()->SetTitleOffset(1.2);
	gr_posXZ->Draw("a*");
	
	
	c22->Print("posnu_unweighted.jpg");
	c22->Print("posnu_unweighted.root");
	

	//TCanvas c5 is created for the weight histograms

	TCanvas *c5 = new TCanvas("c5", "Weight Histograms", 18000, 6000);
	c5->Divide(3);


	int limit = 2000000;


	c5->cd(1);
	TH2F *xy_weight = new TH2F("xy_weight", "NEUTRINO POSITION XY PLANE", 40, -limit, limit, 40, -limit, limit);

	for (int k=0; k<num_pass; k++)
	  {
	    xy_weight->Fill(posnu_x[k], posnu_y[k], weight_array[k]);

	  }
	xy_weight->Draw("colz");
	xy_weight->GetXaxis()->SetTitle("X (m)");
	xy_weight->GetYaxis()->SetTitle("Y (m)");
	xy_weight->GetYaxis()->SetTitleOffset(1.3);

	
	c5->cd(2);
	TH2F *yz_weight = new TH2F("yz_weight", "NEUTRINO POSITION YZ PLANE", 40, -limit, limit, 40, 6050000, 6400000);
      
	for (int m=0; m<num_pass; m++)
	  {
	    yz_weight->Fill(posnu_y[m], posnu_z[m], weight_array[m]);

	  }
	
        yz_weight->Draw("colz");
	yz_weight->GetXaxis()->SetTitle("Y (m)");
	yz_weight->GetYaxis()->SetTitle("Z (m)");
	yz_weight->GetYaxis()->SetTitleOffset(1.6);
	
	c5->cd(3);
	TH2F *xz_weight = new TH2F("xz_weight", "NEUTRINO POSITION XZ PLANE", 40, -limit, limit, 40, 6050000, 6400000);

	for (int k=0; k<num_pass; k++)
	  {
	    xz_weight->Fill(posnu_x[k], posnu_z[k], weight_array[k]);

	  }

	xz_weight->Draw("colz");
	xz_weight->GetXaxis()->SetTitle("X (m)");
	xz_weight->GetYaxis()->SetTitle("Z (m)");
	xz_weight->GetYaxis()->SetTitleOffset(1.6);

	c5->Print("primaryplots/posnu.jpg");
	c5->Print("primaryplots/posnu.root");


}

//Next step: go through icemc and mark all the other files that are created in the output directory (distance.txt, events.txt, etc) so that they change with the array id. Keep the output file the same and then use "hadd" to add up all the icefinal.root files after running multiple jobs.




/*

int main() {
  gStyle=color;//Sets the style of plots


  //I dont know if you actually need this stuff or not. You can not add it and check to see what your plots look like then add in the lines. 
 
  gStyle->SetPalette(1); //rainbow plot
  gStyle -> SetOptStat(0000000000000); //stat opts. (Mean, rms, overflow underflow etc. I dont know which 0 corresponds to what though)
  gStyle-> SetStatX(0.9);//moves the legend around
  gStyle-> SetStatY(0.9);
  gStyle-> SetStatH(.9);
   
  
}//main
&
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TStyle* RootStyle() {

  // const char* modified = "Borrowed and adapted from paus et al";

  TStyle *RootStyle = new TStyle("Root-Style","The Perfect Style for Plots ;-)");

#ifdef __CINT__
  TStyle *GloStyle;
  GloStyle = gStyle;                          // save the global style reference

  gStyle = RootStyle;
#endif
  // otherwise you need to call TROOT::SetStyle("Root-Style")

  // Paper size

  RootStyle->SetPaperSize(TStyle::kUSLetter);

  // Canvas

  RootStyle->SetCanvasColor     (0);
  RootStyle->SetCanvasBorderSize(10);
  RootStyle->SetCanvasBorderMode(0);
  RootStyle->SetCanvasDefH      (600);
  RootStyle->SetCanvasDefW      (600);
  RootStyle->SetCanvasDefX      (10);
  RootStyle->SetCanvasDefY      (10);

  // Pads

  RootStyle->SetPadColor       (0);
  RootStyle->SetPadBorderSize  (10);
  RootStyle->SetPadBorderMode  (0);
  //  RootStyle->SetPadBottomMargin(0.13);
  RootStyle->SetPadBottomMargin(0.16);
  RootStyle->SetPadTopMargin   (0.08);
  RootStyle->SetPadLeftMargin  (0.13);
  RootStyle->SetPadRightMargin (.13);
  RootStyle->SetPadGridX       (0);
  RootStyle->SetPadGridY       (0);
  RootStyle->SetPadTickX       (1);
  RootStyle->SetPadTickY       (1);

  // Frames

  RootStyle->SetFrameFillStyle ( 0);
  RootStyle->SetFrameFillColor ( 0);
  RootStyle->SetFrameLineColor ( 1);
  RootStyle->SetFrameLineStyle ( 0);
  RootStyle->SetFrameLineWidth ( 2);
  RootStyle->SetFrameBorderSize(10);
  RootStyle->SetFrameBorderMode( 0);


  // Histograms

  RootStyle->SetHistFillColor(0);
  RootStyle->SetHistFillStyle(1);
  RootStyle->SetHistLineColor(1);
  RootStyle->SetHistLineStyle(0);
  RootStyle->SetHistLineWidth(2);

  // Functions

  RootStyle->SetFuncColor(1);
  RootStyle->SetFuncStyle(0);
  RootStyle->SetFuncWidth(1);

  //Legends 

  RootStyle->SetStatBorderSize(0);
  RootStyle->SetStatFont      (42);
  RootStyle->SetOptStat       (111111);
  //RootStyle->SetOptStat       ("ne");
  RootStyle->SetStatColor     (0);
  RootStyle->SetStatX         (0);
  RootStyle->SetStatY         (0);
  RootStyle->SetStatFontSize  (0.06);
  RootStyle->SetStatW         (0.2);
  RootStyle->SetStatH         (0.15);
  
  // Labels, Ticks, and Titles

  RootStyle->SetTickLength ( 0.015,"X");
  RootStyle->SetTitleSize  ( 0.055,"X");
  RootStyle->SetTitleOffset( 1.000,"X");
  RootStyle->SetTitleBorderSize(0);
  //  RootStyle->SetTitleFontSize((float)3.);
  RootStyle->SetLabelOffset( 0.015,"X");
  RootStyle->SetLabelSize  ( 0.040,"X");
  RootStyle->SetLabelFont  ( 42   ,"X");

  RootStyle->SetTickLength ( 0.015,"Y");
  RootStyle->SetTitleSize  ( 0.055,"Y");
  RootStyle->SetTitleOffset( 1.00,"Y");
  RootStyle->SetLabelOffset( 0.015,"Y");
  RootStyle->SetLabelSize  ( 0.040,"Y");
  RootStyle->SetLabelSize  ( 0.040,"Z");
  RootStyle->SetLabelFont  ( 42   ,"Y");
  RootStyle->SetLabelFont  ( 42   ,"Z");
  RootStyle->SetTitleOffset(1.00  ,"Z");
  RootStyle->SetTitleSize  ( 0.055,"Z");
  RootStyle->SetTitleFont  (42,"XYZ");
  RootStyle->SetTitleColor  (1);

  // Options

  RootStyle->SetOptFit     (1);

  RootStyle->SetMarkerStyle(20);
  RootStyle->SetMarkerSize(.8);

  //  cout << ">> Style initialized with the Root Style!" << endl;
  //  cout << ">> " << modified << endl << endl;
  return RootStyle;
}
*/
