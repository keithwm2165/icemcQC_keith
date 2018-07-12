//
//  S.Hoover_Plots.cpp
//  Here are plot codes reproducing the 25 catergories of plots in Steven Hoovers' essay
//
//
//  Author: Victoria Niu
//  Summer May-July 2018
//
//  Revised from Brian's, Oindreer's and previous QC group's code
//  It's a testing script. Several catergories are not finished.
//
//  C++&Canvas Heading
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
#include "TPaveLabel.h"

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

int main(int argc, char * argv[]) {
    
    gStyle->SetOptStat(111111);
    gStyle->SetOptTitle(0);
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
    cout << "Reading Icemc output file......" << endl;
    
    TFile *AnitaFile = new TFile(( readfile ).c_str());
    
    //Define Variables for Tree Branches;
    int num_pass;               // number of entries (ultra-neutrinos);
    double weight;              // weight of neutrino counts;
    double posnu[3];            // position of interaction;
    double r_in[3];             // surface distance where neutrino enters
    double depth_int;           // depth of interaction;
    double r_fromballoon;       // Surface distance of interaction from the balloon;
    double r_fromballoon_sq;    // r_fromballoon squared;
    double costheta_nutraject;  // Angle Between RF at top of Firn and Surf.Norm (Degrees);
    double theta;               // arccos of costheta_nutraject in degrees;
    double fresnel2;            // Fresnel coefficient;
    int nuflavorint;            // neutrino flavors;
    double rincostheta;         // costheta at the entrance;
    double angle;               // the magnitude of theta;
    
    //get "passing_events" tree from AnitaFile and set branch addresses
    TTree *passing_events= (TTree*)AnitaFile->Get("passing_events");
    
    passing_events->SetBranchAddress("nuflavor", &nuflavorint);
    passing_events->SetBranchAddress("posnu", &posnu);
    passing_events->SetBranchAddress("weight", &weight);
    passing_events ->SetBranchAddress("fresnel2",&fresnel2);
    passing_events->SetBranchAddress("altitude_int", &depth_int);
    passing_events->SetBranchAddress("r_fromballoon", &r_fromballoon);
    passing_events->SetBranchAddress("r_in", &r_in);
    passing_events->SetBranchAddress("costheta_nutraject", &costheta_nutraject);
    
    num_pass= passing_events->GetEntries();
    cout << endl;
    cout << "AnitaFile" << endl;
    cout << "num_pass is " << num_pass << endl;
    
    // Histogram Style Settings
    gStyle->SetHistFillColor(0);
    gStyle->SetHistFillStyle(1);
    gStyle->SetHistLineColor(1);
    gStyle->SetHistLineStyle(0);
    gStyle->SetHistLineWidth(2.5);
    
    // Make Histograms
    
    // c1.Positions of Interactions Histogram (posnu)
    TCanvas *c1 = new TCanvas("c1","Location",1100,850); //make a square canvas
    TH2D *posnu_xy = new TH2D("posnu_xy","posnu xy", 200, -3000000, 4000000, 200, -2500000, 2500000); //make a holder histogram
    
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
    c1->SaveAs("S.Hoover_Primaries/Positions of Interactions.pdf"); //save it
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
    
    
    gStyle->SetHistLineWidth(1);
    c2->Print("S.Hoover_Primaries/Depth of Interaction.pdf");
    delete c2;
    
    
    //c3.Surface Distance of Interaction from Balloon Histogram (r_fromballoon)
    TCanvas *c3 = new TCanvas("c3","r_fromballoon", 1100,850);
    TH1F *r_fromballoon_hist = new TH1F("r_fromballoon", "", 50, 0, 800000);
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
    r_fromballoon_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    r_fromballoon_hist->GetYaxis()->CenterTitle();
    gStyle->SetHistLineWidth(9);
    gStyle->SetHistLineWidth(2);
    c3->Print("S.Hoover_Primaries/Surface Distance from Balloon.pdf");
    delete c3;
    
    //c4.Surface Distance Squared Histogram (r_fromballoon_sq)
    TCanvas *c4 = new TCanvas("c4","r_fromballoon_sq", 1100,850);
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
    r_fromballoon_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    r_fromballoon_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c4->Print("S.Hoover_Primaries/Surface Distance Squared.pdf");
    delete c4;
    
    
    //c5.Surface Distance vs. Depth of Interaction Histogram
    TCanvas *c5 = new TCanvas("c5", "Surface_Dist vs Depth_Int",1550,1000);
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
    c5->SaveAs("S.Hoover_Primaries/SurfaceDist_vs_Depth.pdf");
    delete c5;
    
    
    //c6.Surface Distance Squared vs. Depth of Interaction Histogram
    TCanvas *c6 = new TCanvas("c6", "Surface_Dist_Sq vs Depth_Int",1550,1000);
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
    c6->SaveAs("S.Hoover_Primaries/SurfaceDistSq_vs_Depth.pdf");
    delete c6;
    
    
    //c7.Angle of Neutrino Direction Relative to Surface Histogram (rincostheta, r_in)
    TCanvas *c7 = new TCanvas("c7", "r_in_angle", 1100, 850);
    TH1F *rinangle_hist = new TH1F("Angle to Surface", "", 100, 0, 40);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        rincostheta=r_in[2]/(sqrt(r_in[0]*r_in[0]+r_in[1]*r_in[1]+r_in[2]*r_in[2]));
        angle=acos(rincostheta)*57.325;
        rinangle_hist->Fill(angle, weight);
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    rinangle_hist->Draw("HIST B");
    rinangle_hist->SetTitle("Angle of Neutrino Direction Relative to Surface, E=10^20.00");
    rinangle_hist->GetXaxis()->SetTitle("Degrees");
    rinangle_hist->GetXaxis()->SetTitleOffset(1.0);
    rinangle_hist->GetXaxis()->CenterTitle();
    rinangle_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    rinangle_hist->GetYaxis()->SetTitleOffset(1.0);
    rinangle_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c7->Print("S.Hoover_Primaries/Angle of Incoming Neutrino.pdf");
    delete c7;
    
    
    //c8.Fresnel Coefficient at Air-Firn Interface Histogram (Theta vs Fresnel2)
    TCanvas *c8 = new TCanvas("c8","theta_vs_fresnel", 1100, 850);
    TH2D *theta_vs_fresnel = new TH2D("Theta_vs_Fresnel2", "; theta; fresnel2", 200, 50, 120, 200, 0, 1.1);
    for (int k=0; k<=num_pass; k++)
    {
        passing_events->GetEvent(k);
        theta = acos(costheta_nutraject)*57.32;
        theta_vs_fresnel->Fill(theta,fresnel2,weight);
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    theta_vs_fresnel->Draw("colz");
    theta_vs_fresnel->SetTitle("Fresnel Coefficient at Air-Firn Interface, E=10^20.00");
    theta_vs_fresnel->GetXaxis()->SetTitle("Angle Between RF at top of Firn and Surf.Norm(Degrees)");
    theta_vs_fresnel->GetXaxis()->SetTitleOffset(1.2);
    theta_vs_fresnel->GetXaxis()->CenterTitle();
    theta_vs_fresnel->GetYaxis()->SetTitle("Fresnel Coefficient");
    theta_vs_fresnel->GetYaxis()->SetTitleOffset(1.2);
    theta_vs_fresnel->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c8->SaveAs("S.Hoover_Primaries/Fresnel Coefficient at Air-Firn Interface.pdf"); //save it
    delete c8;
    
    
    //c9.Fresnel Coefficient at Air-Firn Interface Histogram (second result, fresnel2)
    TCanvas *c9 = new TCanvas("c9", "fresnel2",1100,850);
    TH1D *fresnel_2 = new TH1D("fresnel2", "; fresnel2; counts", 75, 0.2, 1.2);
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
    c9->Print("S.Hoover_Primaries/Fresnel2.pdf");
    delete c9;
    
    
    //c10.RF2 Angles
    
    //c11.RF1 Angles
    
    //c12.Angle Between RF Propagation Direction in Air and Neutrino Momentum Histogram(theta_Angle)
    TCanvas *c12 = new TCanvas("c12","theta",1100,850);
    TH1D *angle_hist = new TH1D("Angle", "; angle; counts", 100, 80, 100);
    for (int k=0; k<num_pass; k++)
    {
        passing_events->GetEvent(k);
        angle=acos(costheta_nutraject)*180.0/3.14;
        angle_hist->Fill(angle,weight);
        //  cout << "Event: " << k << endl;
        //  cout << "Angle: " << angle << endl;
    }
    
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    
    angle_hist->Draw("HIST B");
    angle_hist->SetTitle("Angle Between RF Propagation Direction in Air and Neutrino Momentum, E=10^20.00");
    angle_hist->GetXaxis()->SetTitle("Angle (Degree)");
    angle_hist->GetXaxis()->SetTitleOffset(1.0);
    angle_hist->GetXaxis()->CenterTitle();
    angle_hist->GetYaxis()->SetTitle("counts (weighted)");
    angle_hist->GetYaxis()->SetTitleOffset(1.0);
    angle_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c12->Print("S.Hoover_Primaries/Theta(Degree).pdf");
    delete c12;
    
    
    //c13.Dots: Weighted Events, Axes: Angles Between RF prop. dir. in Air & p_nu (phi is ’down’)
    //c14.Antenna Reading Maximum Signal (In read_Trigger.cc)
    //c15.Channels Passing by Antenna (In read_Trigger.cc)
    //c16.Channels Passing by Antenna for Events #3762 (In read_Trigger.cc)
    //c17.Channels Passing by Antenna for Events #20481 (In read_Trigger.cc)
    //c18.Theta Angle of incoming signal with face of antenna with max reading, layer 1
    //c19.Theta Angle of incoming signal with face of antenna with mas reading, layer 3
    //c20.Angles of Incoming RF with E and H planes, layer 1
    //c21.Angles of Incoming RF with E and H planes, layer 3
    //c22.Angle of incoming signal with face of ant.(dots are weighted events), layer 1
    //c23.Angle of incoming signal with face of ant.(dots are weighted events), layer 3
    
    
    //c24.Interacted Neutrino Flavors Histogram (nuflavorint)
    TCanvas *c24 = new TCanvas("c24", "nuflavorint", 1100,850);
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
    nuflavorint_hist->GetXaxis()->SetTitle("Neutrino Flavors (1=e, 2=muon, 3=tau)");
    nuflavorint_hist->GetYaxis()->SetTitle("Weigthed Fraction of Total Detected Events");
    nuflavorint_hist->GetXaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetYaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetXaxis()->CenterTitle();
    nuflavorint_hist->GetYaxis()->CenterTitle();
    
    gStyle->SetHistLineWidth(2);
    c24->Print("S.Hoover_Primaries/Neutrino Flavors.pdf");
    delete c24;
    
    
    //c25.Volts per Meter integrated from 200 to 1200 MHz As Seen on the Sky (Banana!)
    //Entries of banana_tree is 0 in icefinal.root
    
    AnitaFile->Close();
    delete AnitaFile;
    return 0;
    
    
}

