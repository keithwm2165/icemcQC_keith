//
//
//Author: Victoria Niu
//Created Time: Summer 2018 
//
//
//REVISED FROM THE SCRIPTS WRITTEN BY OINDREE BANERJEE, BRIAN CLARK AND PREVIOUS QC GROUP
//
// C++&Canvas Heading
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

// Icemc Headings
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
#include "Spectra.h"
#include "signal.hh"
#include "secondaries.hh"
#include "ray.hh"
#include "counting.hh"
#include "Primaries.h"
#include "Taumodel.hh"

using namespace std;

int NNU;
double RANDOMISEPOL;

////////////////PLOTTING SCRIPT FOR ICEMCQC WRITTEN BY VICTORIA NIU DURING SUMMER OF 2018, REVISED FROM THE SCRIPTS WRITTEN BY OINDREE BANERJEE, BRIAN CLARK AND PREVIOUS QC GROUP ///////////////////////////////

int main(int argc, char *argv[])
{
    gStyle->SetOptStat(111111);     // Setup Statistics;
    
    //gStyle=color;

	string readfile;
  
    //read icefinal.root file;
	if(argc<2)
	  {
          cout << "The arguments are not enough! Abort!" << endl;
          cout<<"Usage is "<<argv[0]<<" path/to/icefinal.root"<<endl;
          return -1;
	  }
	if(argc==2)
	  {
	    readfile=argv[1];
	  }
	
    TFile *AnitaFile = new TFile(( readfile ).c_str());   //Read AnitaFile(icefinal.root);
	cout << "Reading AnitaFile..." << endl;
	
	
    //Define Variables for Tree. Here we only read the Tree of "passing events" in icefinal.root file;
    
    int num_pass;                // number of entries (ultra-neutrinos);
    double weight;              // weight of neutrino counts;
    double nnu[3];              // trajectory of neutrinos;
    double posnu[3];            // position of interaction;
    double depth_int;           // depth of interaction;
    
    double costheta_nutraject;  // the interaction angle theta;
    double phi_nutraject;       // the interaction angle phi;
    double theta;               // arccos of costheta_nutraject in degrees
    double phi;                 // phi_nutraject in degrees
    double mybeta;
    double mytheta;
    double theta_rf_atbn;
    
    double r_in[3];             // position of neurtinos when entering the balloon;
    double r_enterice[3];       // position of neurtinos when entering the ice;
    double nuexit;              // position where neutrino would have exit;
    double nuexitice;           // position where neutrino would have left the ice;
    double n_exit_phi;
    double r_exit2bn;
    double r_exit2bn_measured;
    double r_fromballoon;        // Surface distance of interaction from the balloon
    double r_fromballoon_sq;     // r_fromballoon squared
    
    
    double d1;                   // Distance from Earth entrance to rock-ice boundary
    double d2;                   // Distance from Rock-ice boundary to interaction point
    double d1plusd2;             // Distance from Earth Entrance to Interaction Point
    int currentint;              // the charge of neurtinos;
    
    
    double chord;
    double logchord;
    double weight_bestcase;
    double chord_kgm2_bestcase;
    double dtryingdirection;
    
	int nuflavorint;
    double fresnel1;
    double fresnel2;
    double dviewangle_deg;
    
   
	//get "passing_events" tree from AnitaFile and set branch addresses
	TTree *passing_events= (TTree*)AnitaFile->Get("passing_events");

	/*PRIMARIES VARIABLES*/
	passing_events->SetBranchAddress("nnu", &nnu);
	passing_events->SetBranchAddress("costheta_nutraject", &costheta_nutraject);
	passing_events->SetBranchAddress("phi_nutraject", &phi_nutraject);
	passing_events->SetBranchAddress("weight", &weight);
	passing_events->SetBranchAddress("r_in", &r_in);
	passing_events->SetBranchAddress("r_enterice", &r_enterice);
        passing_events->SetBranchAddress("nuexitlength", &nuexit);
	passing_events->SetBranchAddress("d1", &d1);
	passing_events->SetBranchAddress("d2", &d2);
	passing_events->SetBranchAddress("current", &currentint);
	passing_events->SetBranchAddress("altitude_int", &depth_int);
	passing_events->SetBranchAddress("r_fromballoon", &r_fromballoon);
	passing_events->SetBranchAddress("r_exit2bn", &r_exit2bn);
	passing_events->SetBranchAddress("r_exit2bn_measured", &r_exit2bn_measured);
	passing_events->SetBranchAddress("nuflavor", &nuflavorint);
	passing_events->SetBranchAddress("posnu", &posnu);
        passing_events ->SetBranchAddress("chord",&chord);
	passing_events->SetBranchAddress("logchord", &logchord);
	passing_events->SetBranchAddress("nuexitice", &nuexitice);
	passing_events->SetBranchAddress("weight_bestcase", &weight_bestcase);
	passing_events->SetBranchAddress("chord_kgm2_bestcase", &chord_kgm2_bestcase);
	passing_events->SetBranchAddress("dtryingdirection", &dtryingdirection);
        passing_events ->SetBranchAddress("dviewangle_deg",&dviewangle_deg);
        passing_events ->SetBranchAddress("fresnel1",&fresnel1);
        passing_events ->SetBranchAddress("fresnel2",&fresnel2);
        passing_events ->SetBranchAddress("mytheta",&mytheta);
        passing_events ->SetBranchAddress("mybeta",&mybeta);
        passing_events ->SetBranchAddress("theta_rf_atbn",&theta_rf_atbn);
    
	num_pass= passing_events->GetEntries();
    cout << endl;
    cout << "AnitaFile" << endl;
	cout << "num_pass is " << num_pass << endl;//check how many passing events there are

	double weight_array[num_pass];

    /*  For other branches to read:
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
	tree4->SetBranchAddress("dtryingdirection", &dtryingdirection); */


    // Histogram Style Settings
  gStyle->SetHistFillColor(0);
  gStyle->SetHistFillStyle(1);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(2.5);

    // Make Histograms

	// c1.Positions of Interactions Histogram (posnu)
    TCanvas *c1 = new TCanvas("c1","Location",1100,850); //make a square canvas
    TH2D *posnu_xy = new TH2D("posnu_xy","posnu xy", 100, -3000000, 4000000, 100, -2500000, 2500000); //make a holder histogram
    
    for (int i=0; i<num_pass; i++){ //loop over all of the events in the tree
        passing_events->GetEvent(i); //get the events
        // now print out some info
        // cout<<"Event "<<i<<endl;
        // cout<<"    X: "<<posnu[0]<<endl;
        // cout<<"    Y: "<<posnu[1]<<endl;
        // cout<<"    Z: "<<posnu[2]<<endl;
        posnu_xy -> Fill(posnu[0], posnu[1]); //put the event into the histogram
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.90);
    gStyle->SetStatY(0.90);
    
    posnu_xy->Draw("colz"); //draw the histogram
    posnu_xy->SetTitle("Locations of Interactions in Antarctica, E=10^20.00");
    posnu_xy->GetXaxis()->SetTitle("X Coordinate (m)"); //set the x-axis title
    posnu_xy->GetXaxis()->SetTitleOffset(1.2);
    posnu_xy->GetXaxis()->CenterTitle();
    posnu_xy->GetYaxis()->SetTitle("Y Coordinate (m)"); //set the y-axis title
    posnu_xy->GetYaxis()->SetTitleOffset(1.5);
    posnu_xy->GetYaxis()->CenterTitle();
    c1->SaveAs("Primaries/Positions of Interactions.pdf"); //save it
    delete posnu_xy; //cleanup
    delete c1;
    
    
    //c2.Depth of Interaction Histogram (depth_int) *altitude_hist is depth_int
    TCanvas *c2 = new TCanvas("c2", "Depth", 1100,850);
    TH1F *altitude_hist = new TH1F("Depth", "", 50,-2500,100);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        altitude_hist->Fill(depth_int, weight);
        //  cout << "Event " << k << endl;
        //  cout << "Depth_int: " << depth_int << endl;
    }
    
    // Set Styles only for this plot;
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    
    altitude_hist->Draw("HIST B");
    altitude_hist->SetTitle("Depth of Interaction, Energy=10^20");
    altitude_hist->GetXaxis()->SetTitle("Depth of Interaction (m)");
    altitude_hist->GetXaxis()->SetTitleOffset(1.2);
    altitude_hist->GetXaxis()->CenterTitle();
    altitude_hist->GetYaxis()->SetTitle("Weighted Counts of Interactions");
    altitude_hist->GetYaxis()->SetTitleOffset(1.0);
    altitude_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(9);
    gStyle->SetHistLineWidth(1);
    c2->Print("Primaries/Depth of Interaction.pdf");
    delete c2;
    
    
    //c3.Surface Distance of Interaction from Balloon Histogram (r_fromballoon)
    TCanvas *c3 = new TCanvas("c3","Interaction_Dist", 1100,850);
    TH1F *r_fromballoon_hist = new TH1F("Interaction_Dist", "", 50, 0, 800000);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        r_fromballoon_hist->Fill(r_fromballoon, weight);
        //  cout << r_fromballoon << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    r_fromballoon_hist->Draw("HIST B");
    r_fromballoon_hist->SetTitle("Surface Distance of Interaction from Balloon,E=10^20.00");
    r_fromballoon_hist->GetXaxis()->SetTitle("Surface Distance of Interaction (m)");
    r_fromballoon_hist->GetXaxis()->CenterTitle();
    r_fromballoon_hist->GetYaxis()->SetTitle("Weighted Counts of Interactions");
    r_fromballoon_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(9);
    gStyle->SetHistLineWidth(2);
    c3->Print("Primaries/Surface Distance from Balloon.pdf");
    delete c3;
    
    
    //c4.Interacted Neutrino Flavors Histogram (nuflavorint)
    TCanvas *c4 = new TCanvas("c4", "nuflavorint", 1100,850);
    TH1F *nuflavorint_hist = new TH1F("nuflavorint", "",3,.5,3.5);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        nuflavorint_hist->Fill(nuflavorint, weight);
        //   cout << "Event: " << k << endl;
        //   cout << "Nuflavorint: " << nuflavorint << endl;
    }
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    
    nuflavorint_hist->Draw("HIST B");
    nuflavorint_hist->SetTitle("Neutrino Flavors,E=10^20.00");
    nuflavorint_hist->GetXaxis()->SetTitle("Nuetrino Flavors (1=e, 2=muon, 3=tau)");
    nuflavorint_hist->GetYaxis()->SetTitle("Weigthed Fraction of Total Detected Events");
    nuflavorint_hist->GetXaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetYaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetXaxis()->CenterTitle();
    nuflavorint_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(9);
    gStyle->SetHistLineWidth(2);
    c4->Print("Primaries/Neutrino Flavors.pdf");
    delete c4;
    
    
    //c5.Surface Distance Squared Histogram (r_fromballoon_sq)
    TCanvas *c5 = new TCanvas("c5","Interaction Dist Squared", 1100,850);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        r_fromballoon_sq=r_fromballoon*r_fromballoon;
        r_fromballoon_hist->Fill(r_fromballoon_sq, weight);
     // cout << "Event:" << k << endl;
     // cout << r_fromballoon_squ << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    r_fromballoon_hist->Draw("HIST B");
    r_fromballoon_hist->SetTitle("Surface Distance Squared of Interaction from Balloon,E=10^20.00");
    r_fromballoon_hist->GetXaxis()->SetTitle("Surface Distance of Interaction Squared (m^2)");
    r_fromballoon_hist->GetXaxis()->CenterTitle();
    r_fromballoon_hist->GetYaxis()->SetTitle("Weighted Counts of Interactions");
    r_fromballoon_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c5->Print("Primaries/Surface Distance Squared.pdf");
    delete c5;
    
    
    //c6.Surface Distance vs. Depth of Interaction Histogram
    TCanvas *c6 = new TCanvas("c6", "Surface_Dist vs Depth_Int",1550,1000);
    TH2D *r_fromballoon_vs_depth_int = new TH2D("r_fromballoon_vs_depth_int", "; r_fromballoon; depth_int", 100, 0, 7e5, 100, -3500, 0);

    r_fromballoon_vs_depth_int->GetXaxis()->SetTitle("Surface Distance from Interaction (m)");
    r_fromballoon_vs_depth_int->GetXaxis()->SetTitleOffset(1.2);
    r_fromballoon_vs_depth_int->GetXaxis()->CenterTitle();
    r_fromballoon_vs_depth_int->GetYaxis()->SetTitle("Depth of Interaction(m)");
    r_fromballoon_vs_depth_int->GetYaxis()->SetTitleOffset(1.6);
    r_fromballoon_vs_depth_int->GetYaxis()->CenterTitle();
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.86);
    gStyle->SetStatY(0.50);

    for (int j=0; j <=num_pass; j++)
    {
        
        passing_events -> GetEvent(j);
        r_fromballoon_vs_depth_int->Fill(r_fromballoon,depth_int,weight);
    }
    
    r_fromballoon_vs_depth_int->Draw("colz");
    r_fromballoon_vs_depth_int->SetTitle("Surface Distance of Interaction from Balloon vs Depth, E=10^20.00");
    r_fromballoon_vs_depth_int->SetTitleOffset(1.2);
    c6->SaveAs("Primaries/SurfaceDist_vs_Depth.pdf");
    delete c6;
    
    
    //c7.Surface Distance Squared vs. Depth of Interaction Histogram
    TCanvas *c7 = new TCanvas("c7", "Surface_Dist_Sq vs Depth_Int",1550,1000);
    TH2D *r_fromballoon_sq_vs_depth_int = new TH2D("r_fromballoon_sq_vs_depth_int", "; r_fromballoon_sq; depth_int", 100, 0, 5e11, 100, -3500, 0);
    
    r_fromballoon_sq_vs_depth_int->GetXaxis()->SetTitle("Surface Distance from Interaction (m^2)");
    r_fromballoon_sq_vs_depth_int->GetXaxis()->SetTitleOffset(1.2);
    r_fromballoon_sq_vs_depth_int->GetXaxis()->CenterTitle();
    r_fromballoon_sq_vs_depth_int->GetYaxis()->SetTitle("Depth of Interaction(m)");
    r_fromballoon_sq_vs_depth_int->GetYaxis()->SetTitleOffset(1.6);
    r_fromballoon_sq_vs_depth_int->GetYaxis()->CenterTitle();
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.86);
    gStyle->SetStatY(0.50);
    
    for (int j=0; j <=num_pass; j++)
        
    {
        passing_events -> GetEvent(j);
        r_fromballoon_sq = r_fromballoon * r_fromballoon;
        r_fromballoon_sq_vs_depth_int->Fill(r_fromballoon_sq,depth_int,weight);
    }
    r_fromballoon_sq_vs_depth_int->Draw("colz");
    r_fromballoon_sq_vs_depth_int->SetTitle("Surface Distance Squared of Interaction from Balloon vs Depth, E=10^20.00");
    c7->SaveAs("Primaries/SurfaceDistSq_vs_Depth.pdf");
    delete c7;
    
    
    //c8.Fresnel Coefficient at Air-Firn Interface Histogram (first result, fresnel1)
    TCanvas *c8 = new TCanvas("c8", "fresnel1",1100,850);
    TH1D *fresnel_1 = new TH1D("fresnel1", "; fresnel1; counts", 100, 0.98, 1.01);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        fresnel_1->Fill(fresnel1, weight);
        //     cout << "Event " << k << endl;
        //     cout << "Depth_int: " << depth_int << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    
    fresnel_1->Draw("HIST B");
    fresnel_1->SetTitle("Fresnel Coefficient at Air-Firn Interface, E=10^20.00");
    fresnel_1->GetXaxis()->SetTitle("Fresnel Coefficient");
    fresnel_1->GetXaxis()->SetTitleOffset(1.0);
    fresnel_1->GetXaxis()->CenterTitle();
    fresnel_1->GetYaxis()->SetTitle("Weigthed Number of Events");
    fresnel_1->GetYaxis()->SetTitleOffset(1.0);
    fresnel_1->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(9);
    gStyle->SetHistLineWidth(2);
    c8->Print("Primaries/Fresnel1.pdf");
    delete c8;
    
    
    //c9.Fresnel Coefficient at Air-Firn Interface Histogram (second result, fresnel2)
    TCanvas *c9 = new TCanvas("c9", "fresnel2",1100,850);
    TH1D *fresnel_2 = new TH1D("fresnel2", "; fresnel2; counts", 100, 0.0, 1.2);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        fresnel_2->Fill(fresnel2, weight);
        //     cout << "Event " << k << endl;
        //     cout << "Depth_int: " << depth_int << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    
    fresnel_2->Draw("HIST B");
    fresnel_2->SetTitle("Fresnel Coefficient at Air-Firn Interface, E=10^20.00");
    fresnel_2->GetXaxis()->SetTitle("Fresnel Coefficient");
    fresnel_2->GetXaxis()->SetTitleOffset(1.0);
    fresnel_2->GetXaxis()->CenterTitle();
    fresnel_2->GetYaxis()->SetTitle("Weigthed Number of Events");
    fresnel_2->GetYaxis()->SetTitleOffset(1.0);
    fresnel_2->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c9->Print("Primaries/Fresnel2.pdf");
    delete c9;
    
    
    //c10.The Charge of Neutrinos Histogram (currentint)
    TCanvas *c10 = new TCanvas("c10", "currentint", 1100,850);
    TH1F *currentint_hist = new TH1F("currentint", "", 2, -0.2, 1.4);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        currentint_hist->Fill(currentint, weight);
     // cout << "Event " << k << endl;
     // cout << "Currentint: " << currentint << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.87);
    
    currentint_hist->Draw("HIST B");
    currentint_hist->SetTitle("The Charge of Neutrinos, E=10^20.00");
    currentint_hist->GetXaxis()->SetTitle("current: 0 is neutral, 1 is charged");
    currentint_hist->GetXaxis()->SetTitleOffset(1.2);
    currentint_hist->GetXaxis()->CenterTitle();
    currentint_hist->GetYaxis()->SetTitle("counts (weighted)");
    currentint_hist->GetYaxis()->SetTitleOffset(1.2);
    currentint_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c10->Print("Primaries/Current of Neutrinos.pdf");
    delete c10;
    
    
    //c11.Dtrying Direction Histogram (dtryingdirection)
    TCanvas *c11 = new TCanvas("c11", "dtryingdirection", 1100,850);
    TH1F *dtrying_hist = new TH1F("dtryingdirection", "", 50, 0.8, 1.2);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        dtrying_hist->Fill(dtryingdirection, weight);
     // cout << "Event " << k << endl;
     // cout << "Dtryingdirection: " << dtryingdirection << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.90);
    gStyle->SetStatY(0.87);
    
    dtrying_hist->Draw("HIST B");
    dtrying_hist->SetTitle("Dtrying Directions of Neutrinos, E=10^20.00");
    dtrying_hist->GetXaxis()->SetTitle("equivalent tries");
    dtrying_hist->GetXaxis()->SetTitleOffset(1.2);
    dtrying_hist->GetXaxis()->CenterTitle();
    dtrying_hist->GetYaxis()->SetTitle("counts (weighted)");
    dtrying_hist->GetYaxis()->SetTitleOffset(1.2);
    dtrying_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c11->Print("Primaries/Dtrying Direction.pdf");
    
    /* dtryingdirection = weighting factor: how many equivalent tries each neutrino counts for after having reduced angular phase space for possibly detectable events    */
    delete c11;
    
    
    //c12.The Traverse Chords of Neutrinos Histogram (chord_kgm2_bestcase)
    TCanvas *c12 = new TCanvas("c12", "chord_kgm2_bestcase", 1100,850);
    TH1F *chord_kgm2_bestcase_hist = new TH1F("chord_kgm2_bestcase","", 80,0,5e9);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        chord_kgm2_bestcase_hist->Fill(chord_kgm2_bestcase, weight);
     // cout << "Event " << k << endl;
     // cout << "Chord_kgm2_bestcase: " << chord_kgm2_bestcase << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    chord_kgm2_bestcase_hist->Draw("HIST B");
    chord_kgm2_bestcase_hist->SetTitle("Traverse Chord Length (if neutrino all was crust density), E=10^20.00");
    chord_kgm2_bestcase_hist->GetXaxis()->SetTitle("chord length (from ice entrance to interaction point)(m)");
    chord_kgm2_bestcase_hist->GetXaxis()->SetTitleOffset(1.0);
    chord_kgm2_bestcase_hist->GetXaxis()->CenterTitle();
    chord_kgm2_bestcase_hist->GetYaxis()->SetTitle("counts (weighted)");
    chord_kgm2_bestcase_hist->GetYaxis()->SetTitleOffset(1.0);
    chord_kgm2_bestcase_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c12->Print("Primaries/Traverse Chord Length.pdf");
    // the chord the neutrino would traverse if it all was crust density;
    delete c12;

    
    //c13.Best Case Weight Histogram (weight_bestcase)
    TCanvas *c13 = new TCanvas("c13", "weight_bestcase", 1100,850);
    TH1F *weight_bestcase_hist = new TH1F("weight_bestcase", "",50,0,1);
    for (int k=0; k<num_pass;k++)
    {
        passing_events->GetEvent(k);
        weight_bestcase_hist->Fill(weight_bestcase, weight);
     // cout << "Event " << k << endl;
     // cout << "Weight_bestcase: " << weight_bestcase << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    weight_bestcase_hist->Draw("HIST B");
    weight_bestcase_hist->SetTitle("Best Case Weight, E=10^20.00");
    weight_bestcase_hist->GetXaxis()->SetTitle("weight if all earth had density of the crust");
    weight_bestcase_hist->GetXaxis()->SetTitleOffset(1.0);
    weight_bestcase_hist->GetXaxis()->CenterTitle();
    weight_bestcase_hist->GetYaxis()->SetTitle("counts (weighted)");
    weight_bestcase_hist->GetYaxis()->SetTitleOffset(1.0);
    weight_bestcase_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c13->Print("Primaries/Best Case Weight.pdf");
    // what weight1 would be if whole earth had density of crust - for quick and dirty calculation of best case scenario
    delete c13;
    
    
    //c14.Log of Chord Length (logchord)
    TCanvas *c14 = new TCanvas("c14", "logchord", 1100,850);
    TH1F *logchord_hist = new TH1F("logchord", "", 50, 1, 8);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        logchord_hist->Fill(logchord, weight);
        // cout << "Event: " << k << endl;
        // cout << "Logchord: " << logchord << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    logchord_hist->Draw("HIST B");
    logchord_hist->SetTitle("Log of Chord Length, E=10^20.00");
    logchord_hist->GetXaxis()->SetTitle("log_10(chord)");
    logchord_hist->GetXaxis()->SetTitleOffset(1.0);
    logchord_hist->GetXaxis()->CenterTitle();
    logchord_hist->GetYaxis()->SetTitle("counts (weighted)");
    logchord_hist->GetYaxis()->SetTitleOffset(1.0);
    logchord_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c14->Print("Primaries/Log(chord).pdf");
    delete c14;
    
    
    //c15. Surface Distance of Neurtino When Exiting Balloon Histogram (r_exit2bn)
    TCanvas *c15 = new TCanvas("c15","r_exit2bn", 1100,850);
    TH1F *r_exit2bn_hist = new TH1F("r_exit2bn","",50,0,700000);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        r_exit2bn_hist->Fill(r_exit2bn, weight);
        //  cout << r_exit2bn << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    r_exit2bn_hist->Draw("HIST B");
    r_exit2bn_hist->SetTitle("Surface Distance of Neutrino When Exiting Balloon, E=10^20.00");
    r_exit2bn_hist->GetXaxis()->SetTitle("Distance from Exit to Balloon (m)");
    r_exit2bn_hist->GetXaxis()->SetTitleOffset(1.0);
    r_exit2bn_hist->GetXaxis()->CenterTitle();
    r_exit2bn_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_exit2bn_hist->GetYaxis()->SetTitleOffset(1.0);
    r_exit2bn_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c15->Print("Primaries/Surface Distance When Exiting.pdf");
    delete c15;
    
    
    //c16.Measured Surface Distance of Neurtino When Exiting Balloon Histogram (r_exit2bn_measured)
    TCanvas *c16 = new TCanvas("c16", "r_exit2bn_measured", 1100,850);
    TH1F *r_exit2bn_measured_hist = new TH1F("r_exit2bn_measured", "", 80, 0, 500000);
    for (int k=0;k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        r_exit2bn_measured_hist->Fill(r_exit2bn_measured, weight);
        // cout << r_exit2bn_measured << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    r_exit2bn_measured_hist->Draw("HIST B");
    r_exit2bn_measured_hist->SetTitle("Measured Surface Distance of Neutrino When Exiting Balloon, E=10^20.00");
    r_exit2bn_measured_hist->GetXaxis()->SetTitle("Measured Distance from Exit to Balloon (m)");
    r_exit2bn_measured_hist->GetXaxis()->SetTitleOffset(1.0);
    r_exit2bn_measured_hist->GetXaxis()->CenterTitle();
    r_exit2bn_measured_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_exit2bn_measured_hist->GetYaxis()->SetTitleOffset(1.0);
    r_exit2bn_measured_hist->GetYaxis()->CenterTitle();

    gStyle->SetHistLineWidth(2);
    c16->Print("Primaries/Measured Surface Distance When Exiting.pdf");
    delete c16;
    
    
    //c17.Exit Earth Histogram (nuexit)
    TCanvas *c17 = new TCanvas("c17","nuexitlength (magnitude)", 1500,1000);
    TH1F *nuexit_hist = new TH1F("nuexit", "",100,-200,1250000);
    cout << "Weight is " << weight << endl;
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        nuexit_hist->Fill(nuexit, weight);
     // cout << "Event: " << k << endl;
     // cout << "Nuexit: " << nuexit << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    nuexit_hist->Draw("HIST B");
    nuexit_hist->SetTitle("Neutrinos Exit the Earth, E=10^20.00");
    nuexit_hist->GetXaxis()->SetTitle("where neutrino would have left the Earth (m)");
    nuexit_hist->GetXaxis()->SetTitleOffset(1.0);
    nuexit_hist->GetXaxis()->CenterTitle();
    nuexit_hist->GetYaxis()->SetTitle("counts (weighted)");
    nuexit_hist->GetYaxis()->SetTitleOffset(1.0);
    nuexit_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c17->Print("Primaries/Neutrino Exit Earth.pdf");
    delete c17;
    
    
    //c18.Exit Ice Histogram (nuexitice)
    TCanvas *c18 = new TCanvas("c18", "nuexitice", 1500,1000);
    TH1F *nuexitice_hist = new TH1F("nuexitice", "",60, 6355000, 6364000);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        nuexitice_hist->Fill(nuexitice, weight);
     // cout << "Event: " << k << endl;
     // cout << "Nuexitice: " << nuexitice << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    nuexitice_hist->Draw("HIST B");
    nuexitice_hist->SetTitle("Neutrinos Exit the Ice, E=10^20.00");
    nuexitice_hist->GetXaxis()->SetTitle("where neutrino would have left the ice (m)");
    nuexitice_hist->GetXaxis()->SetTitleOffset(1.0);
    nuexitice_hist->GetXaxis()->CenterTitle();
    nuexitice_hist->GetYaxis()->SetTitle("counts (weighted)");
    nuexitice_hist->GetYaxis()->SetTitleOffset(1.0);
    nuexitice_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c18->Print("Primaries/Neutrino Exit Ice.pdf");
    delete c18;
    
    
    //c19.Interaction Angles->Costheta Histogram (costheta_nutraject)
    TCanvas *c19 = new TCanvas("c19", "costheta_nutraject", 1100,850);
    TH1F *costheta_nutraject_hist = new TH1F("cos(theta)", "", 50, -0.2, 0.25);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        costheta_nutraject_hist->Fill(costheta_nutraject, weight);
        //  cout << "Event: " << k << endl;
        //  cout << "Cos(theta): " << costheta_nutraject << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    
    costheta_nutraject_hist->Draw("HIST B");
    costheta_nutraject_hist->SetTitle("Interaction Angle (Cos(theta)), Ε=10^20.00");
    costheta_nutraject_hist->GetXaxis()->SetTitle("cos(theta) (theta is neutrino direction with earth center to balloon as z axis");
    costheta_nutraject_hist->GetXaxis()->SetTitleOffset(1.0);
    costheta_nutraject_hist->GetXaxis()->CenterTitle();
    costheta_nutraject_hist->GetYaxis()->SetTitle("counts (weighted)");
    costheta_nutraject_hist->GetYaxis()->SetTitleOffset(1.0);
    costheta_nutraject_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c19->Print("Primaries/Cos(θ).pdf");
    delete c19;
    
    
    //c20.Earth Entrance to rock-ice boundary Histogram (d1)
    TCanvas *c20 = new TCanvas("c20", "d1", 1100,850);
    TH1F *d1_hist = new TH1F("d1","",50,0,7e5);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        d1_hist->Fill(d1, weight);
       // cout << "Event: " << k << endl;
        // cout << "d1: " << d1 << endl;
        
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    d1_hist->Draw("HIST B");
    d1_hist->SetTitle("d1: Earth Entrance to Rock-Ice Boundary, E=10^20.00");
    d1_hist->GetXaxis()->SetTitle("Distance(m)");
    d1_hist->GetXaxis()->SetTitleOffset(1.0);
    d1_hist->GetXaxis()->CenterTitle();
    d1_hist->GetYaxis()->SetTitle("counts (weighted)");
    d1_hist->GetYaxis()->SetTitleOffset(1.0);
    d1_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c20->Print("Primaries/d1.pdf");
    delete c20;
    
    
    //c21.Ice-rock Boundary to Interaction Point Histogram (d2)
    TCanvas *c21 = new TCanvas("c21", "d2", 1100,850);
    TH1F *d2_hist = new TH1F("d2","",50,0,6e5);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        d2_hist->Fill(d2, weight);
        // cout << "Event: " << k << endl;
        // cout << "d2: " << d2 << endl;
        
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    d2_hist->Draw("HIST B");
    d2_hist->SetTitle("d2: Ice-rock Boundary to Interaction Point, E=10^20.00");
    d2_hist->GetXaxis()->SetTitle("Distance(m)");
    d2_hist->GetXaxis()->SetTitleOffset(1.0);
    d2_hist->GetXaxis()->CenterTitle();
    d2_hist->GetYaxis()->SetTitle("counts (weighted)");
    d2_hist->GetYaxis()->SetTitleOffset(1.0);
    d2_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c21->Print("Primaries/d2.pdf");
    delete c21;
    
    
    //c22.Earth Entrance to Interaction Point Histogram (d1+d2)
    TCanvas *c22 = new TCanvas("c22", "d1+d2", 1100,850);
    TH1F *d1plusd2_hist = new TH1F("d1+d2","",50,0,1.2e6);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        d1plusd2=d1+d2;
        d1plusd2_hist->Fill(d1plusd2, weight);
        // cout << "Event: " << k << endl;
        // cout << "d1+d2: " << d1plusd2 << endl;
        
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    d1plusd2_hist->Draw("HIST B");
    d1plusd2_hist->SetTitle("Earth Entrance to Interaction Point, E=10^20.00");
    d1plusd2_hist->GetXaxis()->SetTitle("Distance(m)");
    d1plusd2_hist->GetXaxis()->SetTitleOffset(1.0);
    d1plusd2_hist->GetXaxis()->CenterTitle();
    d1plusd2_hist->GetYaxis()->SetTitle("counts (weighted)");
    d1plusd2_hist->GetYaxis()->SetTitleOffset(1.0);
    d1plusd2_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(2);
    c22->Print("Primaries/d1+d2.pdf");
    delete c22;

    
    AnitaFile->Close(); //close the file we loaded
    delete AnitaFile; //more clean up
    return 0; //return successfully!
    
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    

