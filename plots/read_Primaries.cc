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
    
    gStyle->SetHistFillColor(0);
    gStyle->SetHistFillStyle(1);
    gStyle->SetHistLineColor(1);
    gStyle->SetHistLineStyle(0);
    gStyle->SetHistLineWidth(2.5);       //Setup plot Style
    
    if(argc<2)
    {
        cout << "Not enough arguments! Stop run. " << endl;
    }
    
    TH2D *posnu_xy = new TH2D("posnu_xy","posnu xy", 100, -3000000, 4000000, 100, -2500000, 2500000);
    TH1F *altitude_hist = new TH1F("Depth", "", 50,-2500,100);
    TH1F *r_fromballoon_hist = new TH1F("r_fromballoon", "", 50, 0, 800000);
    TH1F *nuflavorint_hist = new TH1F("nuflavorint", "",3,.5,3.5);
    TH1D *r_fromballoon_sq_hist = new TH1D("r_fromballoon_sq", "; r_fromballoon_sq; counts", 100, 0, 5e11);
    TH2D *r_fromballoon_vs_depth_int = new TH2D("r_fromballoon_vs_depth_int", "; r_fromballoon; depth_int", 100, 0, 7e5, 100, -3500, 0);
    TH2D *r_fromballoon_sq_vs_depth_int = new TH2D("r_fromballoon_sq_vs_depth_int", "; r_fromballoon_sq; depth_int", 100, 0, 5e11, 100, -3500, 0);
    TH1D *fresnel_1 = new TH1D("fresnel1", "; fresnel1; counts", 75, 0.975, 1.005);
    TH1D *fresnel_2 = new TH1D("fresnel2", "; fresnel2; counts", 75, 0.2, 1.2);
    TH1F *currentint_hist = new TH1F("currentint", "", 2, -0.2, 1.4);
    TH1F *dtrying_hist = new TH1F("dtryingdirection", "", 50, 0.8, 1.2);
    TH1F *chord_kgm2_bestcase_hist = new TH1F("chord_kgm2_bestcase","", 70,-0.5e9,3e9);
    TH1F *weight_bestcase_hist = new TH1F("weight_bestcase", "",50,0,1);
    TH1F *logchord_hist = new TH1F("logchord", "", 50, 1, 8);
    TH1F *r_exit2bn_hist = new TH1F("r_exit2bn","",50,0,700000);
    TH1F *r_exit2bn_measured_hist = new TH1F("r_exit2bn_measured", "", 80, 0, 500000);
    TH1F *nuexit_hist = new TH1F("nuexit", "",75,-1e5,1250000);
    TH1F *nuexitice_hist = new TH1F("nuexitice", "",60, 6356000, 6365000);
    TH1F *costheta_nutraject_hist = new TH1F("cos(theta)", "", 50, -0.2, 0.2);
    TH1F *d1_hist = new TH1F("d1","",50,-2e5,6e5);
    TH1F *d2_hist = new TH1F("d2","",30,0,6e5);
    TH1F *d1plusd2_hist = new TH1F("d1+d2","",50,-2e5,1e6);
    TH2D *nnu_hist = new TH2D("nnu", "; nnu[0]; nnu[1]", 100, -2, 2, 100, -2, 2);
    TH1D *theta_hist = new TH1D("theta", "; theta; counts", 75, 80, 100);
    TH1D *r_in_hist = new TH1D("r_in","; r_in; counts",75,6355000,6365000);
    TH1D *r_enterice_hist = new TH1D("r_enterice","; r_enterice; counts",75,6355000,6365000);
    TH1D *chord_hist = new TH1D("chord","; chord; counts",50,-200,1.6e6);
    TH1D *dviewangle_deg_hist = new TH1D("dviewangle_deg", "; dviewangle_deg; counts", 75, -15, 10);
    TH1D *n_exit_phi_hist = new TH1D("n_exit_phi", "; n_exit_phi; counts", 75, 0, 360);
    TH1D *mytheta_hist = new TH1D("mytheta", "; mytheta; counts", 50, -1, 8);
    TH1D *mybeta_hist = new TH1D("mybeta", "; mybeta; counts", 50, -10, 10);
    TH1D *theta_rf_atbn_hist = new TH1D("theta_rf_atbn", "; theta_rf_atbn; counts", 75, 40, 90);
    TH1F *nnucostheta_hist = new TH1F("nnucostheta", "", 50, -1, 1);
    TH1F *nnuphi_hist = new TH1F("nnuphi", "", 50, -0.5, 7);
    TH1F *rincostheta_hist = new TH1F("rin_costheta", "", 50, 0.5, 1.1);
    TH1F *rinphi_hist = new TH1F("rin_phi", "", 50, 0, 6.3);
    TH2D *theta_vs_phi_hist = new TH2D("theta_vs_phi", "; theta; phi", 100, 0, 180, 100, 0, 0.000001);
    
    posnu_xy->SetTitle("Locations of Interactions in Antarctica");
    posnu_xy->GetXaxis()->SetTitle("X Coordinate (m)"); //set the x-axis title
    posnu_xy->GetXaxis()->SetTitleOffset(1.2);
    posnu_xy->GetXaxis()->CenterTitle();
    posnu_xy->GetYaxis()->SetTitle("Y Coordinate (m)"); //set the y-axis title
    posnu_xy->GetYaxis()->SetTitleOffset(1.5);
    posnu_xy->GetYaxis()->CenterTitle();
    
    altitude_hist->SetTitle("Depth of Interaction");
    altitude_hist->GetXaxis()->SetTitle("Depth of Interaction (m)");
    altitude_hist->GetXaxis()->SetTitleOffset(1.2);
    altitude_hist->GetXaxis()->CenterTitle();
    altitude_hist->GetYaxis()->SetTitle("Weighted Counts of Interactions");
    altitude_hist->GetYaxis()->SetTitleOffset(1.0);
    altitude_hist->GetYaxis()->CenterTitle();
    
    r_fromballoon_hist->SetTitle("Surface Distance of Interaction from Balloon");
    r_fromballoon_hist->GetXaxis()->SetTitle("Surface Distance of Interaction (m)");
    r_fromballoon_hist->GetXaxis()->CenterTitle();
    r_fromballoon_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    r_fromballoon_hist->GetYaxis()->CenterTitle();
    
    nuflavorint_hist->SetTitle("Neutrino Flavors");
    nuflavorint_hist->GetXaxis()->SetTitle("Neutrino Flavors (1=e, 2=muon, 3=tau)");
    nuflavorint_hist->GetYaxis()->SetTitle("Weigthed Fraction of Total Detected Events");
    nuflavorint_hist->GetXaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetYaxis()->SetTitleOffset(1.2);
    nuflavorint_hist->GetXaxis()->CenterTitle();
    nuflavorint_hist->GetYaxis()->CenterTitle();
    
    r_fromballoon_sq_hist->SetTitle("Surface Distance Squared of Interaction from Balloon");
    r_fromballoon_sq_hist->GetXaxis()->SetTitle("Surface Distance of Interaction Squared (m^2)");
    r_fromballoon_sq_hist->GetXaxis()->CenterTitle();
    r_fromballoon_sq_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    r_fromballoon_sq_hist->GetYaxis()->CenterTitle();
    
    r_fromballoon_vs_depth_int->SetTitle("Surface Distance vs Depth of Interaction");
    r_fromballoon_vs_depth_int->GetXaxis()->SetTitle("Surface Distance from Interaction (m)");
    r_fromballoon_vs_depth_int->GetXaxis()->SetTitleOffset(1.2);
    r_fromballoon_vs_depth_int->GetXaxis()->CenterTitle();
    r_fromballoon_vs_depth_int->GetYaxis()->SetTitle("Depth of Interaction(m)");
    r_fromballoon_vs_depth_int->GetYaxis()->SetTitleOffset(1.6);
    r_fromballoon_vs_depth_int->GetYaxis()->CenterTitle();
    
    r_fromballoon_sq_vs_depth_int->GetXaxis()->SetTitle("Surface Distance from Interaction (m^2)");
    r_fromballoon_sq_vs_depth_int->GetXaxis()->SetTitleOffset(1.2);
    r_fromballoon_sq_vs_depth_int->GetXaxis()->CenterTitle();
    r_fromballoon_sq_vs_depth_int->GetYaxis()->SetTitle("Depth of Interaction(m)");
    r_fromballoon_sq_vs_depth_int->GetYaxis()->SetTitleOffset(1.6);
    r_fromballoon_sq_vs_depth_int->GetYaxis()->CenterTitle();
    r_fromballoon_sq_vs_depth_int->SetTitle("Surface Distance Squared of Interaction from Balloon vs Depth");
    
    fresnel_1->SetTitle("Fresnel Coefficient at Air-Firn Interface");
    fresnel_1->GetXaxis()->SetTitle("Fresnel Coefficient");
    fresnel_1->GetXaxis()->SetTitleOffset(1.0);
    fresnel_1->GetXaxis()->CenterTitle();
    fresnel_1->GetYaxis()->SetTitle("Weigthed Number of Events");
    fresnel_1->GetYaxis()->SetTitleOffset(1.0);
    fresnel_1->GetYaxis()->CenterTitle();
    
    fresnel_2->SetTitle("Fresnel Coefficient at Air-Firn Interface");
    fresnel_2->GetXaxis()->SetTitle("Fresnel Coefficient");
    fresnel_2->GetXaxis()->SetTitleOffset(1.0);
    fresnel_2->GetXaxis()->CenterTitle();
    fresnel_2->GetYaxis()->SetTitle("Weigthed Number of Events");
    fresnel_2->GetYaxis()->SetTitleOffset(1.0);
    fresnel_2->GetYaxis()->CenterTitle();
    
    currentint_hist->SetTitle("The Charge of Neutrinos");
    currentint_hist->GetXaxis()->SetTitle("current: 0 is neutral, 1 is charged");
    currentint_hist->GetXaxis()->SetTitleOffset(1.2);
    currentint_hist->GetXaxis()->CenterTitle();
    currentint_hist->GetYaxis()->SetTitle("counts (weighted)");
    currentint_hist->GetYaxis()->SetTitleOffset(1.2);
    currentint_hist->GetYaxis()->CenterTitle();
    
    dtrying_hist->SetTitle("Dtrying Directions of Neutrinos");
    dtrying_hist->GetXaxis()->SetTitle("equivalent tries");
    dtrying_hist->GetXaxis()->SetTitleOffset(1.2);
    dtrying_hist->GetXaxis()->CenterTitle();
    dtrying_hist->GetYaxis()->SetTitle("counts (weighted)");
    dtrying_hist->GetYaxis()->SetTitleOffset(1.2);
    dtrying_hist->GetYaxis()->CenterTitle();
    
    chord_kgm2_bestcase_hist->SetTitle("Traverse Chord Length (if neutrino all was crust density)");
    chord_kgm2_bestcase_hist->GetXaxis()->SetTitle("chord length (from ice entrance to interaction point)(m)");
    chord_kgm2_bestcase_hist->GetXaxis()->SetTitleOffset(1.0);
    chord_kgm2_bestcase_hist->GetXaxis()->CenterTitle();
    chord_kgm2_bestcase_hist->GetYaxis()->SetTitle("counts (weighted)");
    chord_kgm2_bestcase_hist->GetYaxis()->SetTitleOffset(1.0);
    chord_kgm2_bestcase_hist->GetYaxis()->CenterTitle();
    
    weight_bestcase_hist->SetTitle("Best Case Weight");
    weight_bestcase_hist->GetXaxis()->SetTitle("weight if all earth had density of the crust");
    weight_bestcase_hist->GetXaxis()->SetTitleOffset(1.0);
    weight_bestcase_hist->GetXaxis()->CenterTitle();
    weight_bestcase_hist->GetYaxis()->SetTitle("counts (weighted)");
    weight_bestcase_hist->GetYaxis()->SetTitleOffset(1.0);
    weight_bestcase_hist->GetYaxis()->CenterTitle();
    
    logchord_hist->SetTitle("Log of Chord Length");
    logchord_hist->GetXaxis()->SetTitle("log_10(chord)");
    logchord_hist->GetXaxis()->SetTitleOffset(1.0);
    logchord_hist->GetXaxis()->CenterTitle();
    logchord_hist->GetYaxis()->SetTitle("counts (weighted)");
    logchord_hist->GetYaxis()->SetTitleOffset(1.0);
    logchord_hist->GetYaxis()->CenterTitle();
    
    r_exit2bn_hist->SetTitle("Surface Distance of Neutrino When Exiting Balloon");
    r_exit2bn_hist->GetXaxis()->SetTitle("Distance from Exit to Balloon (m)");
    r_exit2bn_hist->GetXaxis()->SetTitleOffset(1.0);
    r_exit2bn_hist->GetXaxis()->CenterTitle();
    r_exit2bn_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_exit2bn_hist->GetYaxis()->SetTitleOffset(1.0);
    r_exit2bn_hist->GetYaxis()->CenterTitle();
    
    r_exit2bn_measured_hist->SetTitle("Measured Surface Distance of Neutrino When Exiting Balloon");
    r_exit2bn_measured_hist->GetXaxis()->SetTitle("Measured Distance from Exit to Balloon (m)");
    r_exit2bn_measured_hist->GetXaxis()->SetTitleOffset(1.0);
    r_exit2bn_measured_hist->GetXaxis()->CenterTitle();
    r_exit2bn_measured_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_exit2bn_measured_hist->GetYaxis()->SetTitleOffset(1.0);
    r_exit2bn_measured_hist->GetYaxis()->CenterTitle();

    nuexit_hist->SetTitle("Neutrinos Exit the Earth");
    nuexit_hist->GetXaxis()->SetTitle("where neutrino would have left the Earth (m)");
    nuexit_hist->GetXaxis()->SetTitleOffset(1.0);
    nuexit_hist->GetXaxis()->CenterTitle();
    nuexit_hist->GetYaxis()->SetTitle("counts (weighted)");
    nuexit_hist->GetYaxis()->SetTitleOffset(1.0);
    nuexit_hist->GetYaxis()->CenterTitle();
    
    nuexitice_hist->SetTitle("Neutrinos Exit the Ice");
    nuexitice_hist->GetXaxis()->SetTitle("where neutrino would have left the ice (m)");
    nuexitice_hist->GetXaxis()->SetTitleOffset(1.0);
    nuexitice_hist->GetXaxis()->CenterTitle();
    nuexitice_hist->GetYaxis()->SetTitle("counts (weighted)");
    nuexitice_hist->GetYaxis()->SetTitleOffset(1.0);
    nuexitice_hist->GetYaxis()->CenterTitle();
    
    costheta_nutraject_hist->SetTitle("Cos(theta) of Neutrinos from Balloon Coordinate System");
    costheta_nutraject_hist->GetXaxis()->SetTitle("cos(theta)");
    costheta_nutraject_hist->GetXaxis()->SetTitleOffset(1.0);
    costheta_nutraject_hist->GetXaxis()->CenterTitle();
    costheta_nutraject_hist->GetYaxis()->SetTitle("counts (weighted)");
    costheta_nutraject_hist->GetYaxis()->SetTitleOffset(1.0);
    costheta_nutraject_hist->GetYaxis()->CenterTitle();
    
    d1_hist->SetTitle("d1: Earth Entrance to Rock-Ice Boundary");
    d1_hist->GetXaxis()->SetTitle("Distance(m)");
    d1_hist->GetXaxis()->SetTitleOffset(1.0);
    d1_hist->GetXaxis()->CenterTitle();
    d1_hist->GetYaxis()->SetTitle("counts (weighted)");
    d1_hist->GetYaxis()->SetTitleOffset(1.0);
    d1_hist->GetYaxis()->CenterTitle();
    
    d2_hist->SetTitle("d2: Ice-rock Boundary to Interaction Point");
    d2_hist->GetXaxis()->SetTitle("Distance(m)");
    d2_hist->GetXaxis()->SetTitleOffset(1.0);
    d2_hist->GetXaxis()->CenterTitle();
    d2_hist->GetYaxis()->SetTitle("counts (weighted)");
    d2_hist->GetYaxis()->SetTitleOffset(1.0);
    d2_hist->GetYaxis()->CenterTitle();
    
    d1plusd2_hist->SetTitle("Earth Entrance to Interaction Point");
    d1plusd2_hist->GetXaxis()->SetTitle("Distance(m)");
    d1plusd2_hist->GetXaxis()->SetTitleOffset(1.0);
    d1plusd2_hist->GetXaxis()->CenterTitle();
    d1plusd2_hist->GetYaxis()->SetTitle("counts (weighted)");
    d1plusd2_hist->GetYaxis()->SetTitleOffset(1.0);
    d1plusd2_hist->GetYaxis()->CenterTitle();
    
    nnu_hist->SetTitle("Direction of Neutrinos(+z in south pole direction)");
    nnu_hist->GetXaxis()->SetTitle("X Coordinate(m)");
    nnu_hist->GetXaxis()->SetTitleOffset(1.0);
    nnu_hist->GetXaxis()->CenterTitle();
    nnu_hist->GetYaxis()->SetTitle("Y Coordinate(m)");
    nnu_hist->GetYaxis()->SetTitleOffset(1.0);
    nnu_hist->GetYaxis()->CenterTitle();
    
    theta_hist->SetTitle("Interaction Angle (Theta)");
    theta_hist->GetXaxis()->SetTitle("Theta (Degree)");
    theta_hist->GetXaxis()->SetTitleOffset(1.0);
    theta_hist->GetXaxis()->CenterTitle();
    theta_hist->GetYaxis()->SetTitle("counts (weighted)");
    theta_hist->GetYaxis()->SetTitleOffset(1.0);
    theta_hist->GetYaxis()->CenterTitle();
    
    r_in_hist->SetTitle("Position(Magnitude) where Neutrino Enters the Earth");
    r_in_hist->GetXaxis()->SetTitle("Distance(m)");
    r_in_hist->GetXaxis()->SetTitleOffset(1.0);
    r_in_hist->GetXaxis()->CenterTitle();
    r_in_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_in_hist->GetYaxis()->SetTitleOffset(1.0);
    r_in_hist->GetYaxis()->CenterTitle();
    
    r_enterice_hist->SetTitle("Position where Neutrino Enters the Ice");
    r_enterice_hist->GetXaxis()->SetTitle("Distance(m)");
    r_enterice_hist->GetXaxis()->SetTitleOffset(1.0);
    r_enterice_hist->GetXaxis()->CenterTitle();
    r_enterice_hist->GetYaxis()->SetTitle("counts (weighted)");
    r_enterice_hist->GetYaxis()->SetTitleOffset(1.0);
    r_enterice_hist->GetYaxis()->CenterTitle();
    
    chord_hist->SetTitle("Chord from Earth Entrance to Rock-Ice Boundary");
    chord_hist->GetXaxis()->SetTitle("chord(m)");
    chord_hist->GetXaxis()->SetTitleOffset(1.0);
    chord_hist->GetXaxis()->CenterTitle();
    chord_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    chord_hist->GetYaxis()->SetTitleOffset(1.0);
    chord_hist->GetYaxis()->CenterTitle();
    
    dviewangle_deg_hist->SetTitle("Deviation from the Cerenkov Angle");
    dviewangle_deg_hist->GetXaxis()->SetTitle("Deviation");
    dviewangle_deg_hist->GetXaxis()->SetTitleOffset(1.0);
    dviewangle_deg_hist->GetXaxis()->CenterTitle();
    dviewangle_deg_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    dviewangle_deg_hist->GetYaxis()->SetTitleOffset(1.0);
    dviewangle_deg_hist->GetYaxis()->CenterTitle();
    
    n_exit_phi_hist->SetTitle("Phi Angle of the Ray from the Surface to the Balloon");
    n_exit_phi_hist->GetXaxis()->SetTitle("Phi");
    n_exit_phi_hist->GetXaxis()->SetTitleOffset(1.0);
    n_exit_phi_hist->GetXaxis()->CenterTitle();
    n_exit_phi_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    n_exit_phi_hist->GetYaxis()->SetTitleOffset(1.0);
    n_exit_phi_hist->GetYaxis()->CenterTitle();
    
    mytheta_hist->SetTitle("Alpha minus 90 Degrees");
    mytheta_hist->GetXaxis()->SetTitle("Degree");
    mytheta_hist->GetXaxis()->SetTitleOffset(1.0);
    mytheta_hist->GetXaxis()->CenterTitle();
    mytheta_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    mytheta_hist->GetYaxis()->SetTitleOffset(1.0);
    mytheta_hist->GetYaxis()->CenterTitle();
    
    mybeta_hist->SetTitle("Beta minus 90 degrees");
    mybeta_hist->GetXaxis()->SetTitle("Degree");
    mybeta_hist->GetXaxis()->SetTitleOffset(1.0);
    mybeta_hist->GetXaxis()->CenterTitle();
    mybeta_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    mybeta_hist->GetYaxis()->SetTitleOffset(1.0);
    mybeta_hist->GetYaxis()->CenterTitle();
    
    theta_rf_atbn_hist->SetTitle("Polar Angle of the Signal seen by Perfect Eyes at the Balloon");
    theta_rf_atbn_hist->GetXaxis()->SetTitle("Degree");
    theta_rf_atbn_hist->GetXaxis()->SetTitleOffset(1.0);
    theta_rf_atbn_hist->GetXaxis()->CenterTitle();
    theta_rf_atbn_hist->GetYaxis()->SetTitle("Triggered Events(Counts)");
    theta_rf_atbn_hist->GetYaxis()->SetTitleOffset(1.0);
    theta_rf_atbn_hist->GetYaxis()->CenterTitle();
    
    nnucostheta_hist->Draw("HIST B");
    nnucostheta_hist->SetTitle("NNU Cos(Theta)");
    nnucostheta_hist->GetXaxis()->SetTitle("cos(theta) of neutrino direction");
    nnucostheta_hist->GetXaxis()->SetTitleOffset(1.2);
    nnucostheta_hist->GetXaxis()->CenterTitle();
    nnucostheta_hist->GetYaxis()->SetTitle("counts (weighted)");
    nnucostheta_hist->GetYaxis()->SetTitleOffset(1.2);
    nnucostheta_hist->GetYaxis()->CenterTitle();
    
    nnuphi_hist->Draw("HIST B");
    nnuphi_hist->SetTitle("NNU Phi");
    nnuphi_hist->GetXaxis()->SetTitle("phi of neutrino direction");
    nnuphi_hist->GetXaxis()->SetTitleOffset(1.2);
    nnuphi_hist->GetXaxis()->CenterTitle();
    nnuphi_hist->GetYaxis()->SetTitle("counts (weighted)");
    nnuphi_hist->GetYaxis()->SetTitleOffset(1.2);
    nnuphi_hist->GetYaxis()->CenterTitle();
    
    rincostheta_hist->Draw("HIST B");
    rincostheta_hist->SetTitle("Cos(theta) of Neutrino Entrance Vector");
    rincostheta_hist->GetXaxis()->SetTitle("cos(theta)");
    rincostheta_hist->GetXaxis()->SetTitleOffset(1.0);
    rincostheta_hist->GetXaxis()->CenterTitle();
    rincostheta_hist->GetYaxis()->SetTitle("counts (weighted)");
    rincostheta_hist->GetYaxis()->SetTitleOffset(1.0);
    rincostheta_hist->GetYaxis()->CenterTitle();
    
    rinphi_hist->Draw("HIST B");
    rinphi_hist->SetTitle("Phi of Neutrino Entrance Vector");
    rinphi_hist->GetXaxis()->SetTitle("phi");
    rinphi_hist->GetXaxis()->SetTitleOffset(1.0);
    rinphi_hist->GetXaxis()->CenterTitle();
    rinphi_hist->GetYaxis()->SetTitle("counts (weighted)");
    rinphi_hist->GetYaxis()->SetTitleOffset(1.0);
    rinphi_hist->GetYaxis()->CenterTitle();
    
    theta_vs_phi_hist->SetTitle("Theta vs Phi");
    theta_vs_phi_hist->GetXaxis()->SetTitle("theta");
    theta_vs_phi_hist->GetXaxis()->SetTitleOffset(1.0);
    theta_vs_phi_hist->GetXaxis()->CenterTitle();
    theta_vs_phi_hist->GetYaxis()->SetTitle("Phi");
    theta_vs_phi_hist->GetYaxis()->SetTitleOffset(1.0);
    theta_vs_phi_hist->GetYaxis()->CenterTitle();
    
    
    for(int i=1; i < argc; i++)
        
    {  // loop over the input files
        
        string readfile = string(argv[i]);
        TFile *AnitaFile = new TFile(( readfile ).c_str());
        cout << "AnitaFile" << endl;
        TTree *passing_events = (TTree*)AnitaFile->Get("passing_events");
        cout << "Reading AnitaFile..." << endl;
        
        
        int num_pass;               // number of entries (ultra-neutrinos);
        double weight;              // weight of neutrino counts;
        double nnu[3];              // trajectory of neutrinos;
        double posnu[3];            // position of interaction;
        double depth_int;           // depth of interaction;
        
        double costheta_nutraject;  // the interaction angle theta;
        double phi_nutraject;       // the interaction angle phi;
        double theta;               // arccos of costheta_nutraject in degrees;
        double phi;                 // phi_nutraject in degrees;
        double mybeta;              // beta angle minus 90 degrees;
        double mytheta;             // alpha angle minus 90 degrees;
        double theta_rf_atbn;       // polar angle of the signal viewed at the Balloon;
        double nnucostheta;         // costheta of the neutrino trajectory;
        double nnuphi;              // phi of the neutrino trajectory;
        double rincostheta;         // costheta at the entrance;
        double rinphi;              // phi at the entrance;
        
        double r_in[3];             // position of neurtinos when entering the balloon;
        double rin;                 // the magnitude of r_in;
        double r_enterice[3];       // position of neurtinos when entering the ice;
        double renterice;           // the magnitude of renterice;
        double nuexit;              // position where neutrino would have exit;
        double nuexitice;           // position where neutrino would have left the ice;
        double n_exit_phi;          // phi when neutrino exits;
        double r_exit2bn;           // surfance distance when neutrino exits the balloon;
        double r_exit2bn_measured;  // measured surface distance when neutrino exits the balloon;
        double r_fromballoon;       // Surface distance of interaction from the balloon;
        double r_fromballoon_sq;    // r_fromballoon squared
        
        
        double d1;                   // Distance from Earth entrance to rock-ice boundary
        double d2;                   // Distance from Rock-ice boundary to interaction point
        double d1plusd2;             // Distance from Earth Entrance to Interaction Point
        int currentint;              // the charge of neurtinos;
        
        
        double chord;                 // chord length of neutrino;
        double logchord;              // log_10 of chord length;
        double weight_bestcase;       // best case weight (when all densities equal to Earth crust;
        double chord_kgm2_bestcase;   // chord at bestcase weight;
        double dtryingdirection;      // weight factor;
        
        int nuflavorint;              // neutrino flavors;
        double fresnel1;              // Fresnel coefficient;
        double fresnel2;              // Fresnel coefficient;
        double dviewangle_deg;        // deviation from the cerenkov angle;
        
        num_pass = passing_events->GetEntries();
        cout << "num_pass is " << num_pass << endl;
        
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
        passing_events ->SetBranchAddress("n_exit_phi",&n_exit_phi);
        
        cout << endl;
    
        for (int k=0; k <=num_pass; k++)
        {
            
            r_fromballoon_sq=r_fromballoon*r_fromballoon;
            d1plusd2=d1+d2;
            theta=acos(costheta_nutraject)*180.0/3.14;
            rin=sqrt(r_in[0]*r_in[0]+r_in[1]*r_in[1]+r_in[2]*r_in[2]);
            renterice=sqrt(r_enterice[0]*r_enterice[0]+r_enterice[1]*r_enterice[1]+r_enterice[2]*r_enterice[2]);
            nnucostheta=nnu[2]/(sqrt(nnu[0]*nnu[0]+nnu[1]*nnu[1]+nnu[2]*nnu[2]));
            nnuphi=atan2(nnu[1],nnu[0]);
            if (nnuphi<0)
            {
                nnuphi=nnuphi+2*PI;
            }
            rincostheta=r_in[2]/(sqrt(r_in[0]*r_in[0]+r_in[1]*r_in[1]+r_in[2]*r_in[2]));
            rinphi=atan2(r_in[1],r_in[0]);
            if (rinphi<0)
            {
                rinphi=rinphi+2*PI;
            }
            phi = phi_nutraject * 57.3;
            
            passing_events->GetEvent(k);
            
            posnu_xy -> Fill(posnu[0], posnu[1],weight);
            altitude_hist->Fill(depth_int, weight);
            r_fromballoon_hist->Fill(r_fromballoon, weight);
            nuflavorint_hist->Fill(nuflavorint, weight);
            r_fromballoon_hist->Fill(r_fromballoon_sq, weight);
            r_fromballoon_vs_depth_int->Fill(r_fromballoon,depth_int,weight);
            r_fromballoon_sq_vs_depth_int->Fill(r_fromballoon_sq,depth_int,weight);
            fresnel_1->Fill(fresnel1, weight);
            fresnel_2->Fill(fresnel2, weight);
            currentint_hist->Fill(currentint, weight);
            dtrying_hist->Fill(dtryingdirection, weight);
            chord_kgm2_bestcase_hist->Fill(chord_kgm2_bestcase, weight);
            weight_bestcase_hist->Fill(weight_bestcase, weight);
            logchord_hist->Fill(logchord, weight);
            r_exit2bn_hist->Fill(r_exit2bn, weight);
            r_exit2bn_measured_hist->Fill(r_exit2bn_measured, weight);
            nuexit_hist->Fill(nuexit, weight);
            nuexitice_hist->Fill(nuexitice, weight);
            costheta_nutraject_hist->Fill(costheta_nutraject, weight);
            d1_hist->Fill(d1, weight);
            d2_hist->Fill(d2, weight);
            d1plusd2_hist->Fill(d1plusd2, weight);
            nnu_hist->Fill(nnu[0],nnu[1],weight);
            theta_hist->Fill(theta,weight);
            r_in_hist->Fill(rin,weight);
            chord_hist->Fill(chord,weight);
            r_enterice_hist->Fill(renterice,weight);
            dviewangle_deg_hist->Fill(dviewangle_deg,weight);
            n_exit_phi_hist->Fill(n_exit_phi,weight);
            mytheta_hist->Fill(mytheta,weight);
            mybeta_hist->Fill(mybeta,weight);
            theta_rf_atbn_hist->Fill(theta_rf_atbn*57.295779513, weight);
            nnucostheta_hist->Fill(nnucostheta,weight);
            nnuphi_hist->Fill(nnuphi,weight);
            rincostheta_hist->Fill(rincostheta, weight);
            rinphi_hist->Fill(rinphi, weight);
            theta_vs_phi_hist->Fill(theta,phi,weight);
            
            
        }               // CLOSE FOR LOOP OVER NUMBER OF EVENTS
        
    } // CLOSE FOR LOOP OVER NUMBER OF INPUT FILES
            
    //  Make Histograms
    
    // c1.Positions of Interactions Histogram (posnu)
    TCanvas *c1 = new TCanvas("c1","Location",1100,850); //make a square canvas
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.90);
    gStyle->SetStatY(0.90);
    posnu_xy->Draw("colz");
    
    //c2.Depth of Interaction Histogram (depth_int) *altitude_hist is depth_int
    TCanvas *c2 = new TCanvas("c2", "Depth", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    altitude_hist->Draw("HIST B");
    
    //c3.Surface Distance of Interaction from Balloon Histogram (r_fromballoon)
    TCanvas *c3 = new TCanvas("c3","r_fromballoon", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    r_fromballoon_hist->Draw("HIST B");
    
    //c4.Interacted Neutrino Flavors Histogram (nuflavorint)
    TCanvas *c4 = new TCanvas("c4", "nuflavorint", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    nuflavorint_hist->Draw("HIST B");
            
    //c5.Surface Distance Squared Histogram (r_fromballoon_sq)
    TCanvas *c5 = new TCanvas("c5","r_fromballoon_sq", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    r_fromballoon_sq_hist->Draw("HIST B");
    
    //c6.Surface Distance vs. Depth of Interaction Histogram
    TCanvas *c6 = new TCanvas("c6", "Surface_Dist vs Depth_Int",1550,1000);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.86);
    gStyle->SetStatY(0.50);
    r_fromballoon_vs_depth_int->Draw("colz");
    
    //c7.Surface Distance Squared vs. Depth of Interaction Histogram
    TCanvas *c7 = new TCanvas("c7", "Surface_Dist_Sq vs Depth_Int",1550,1000);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.86);
    gStyle->SetStatY(0.50);
    r_fromballoon_sq_vs_depth_int->Draw("colz");
    
    //c8.Fresnel Coefficient at Air-Firn Interface Histogram (first result, fresnel1)
    TCanvas *c8 = new TCanvas("c8", "fresnel1",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    fresnel_1->Draw("HIST B");
    
    //c9.Fresnel Coefficient at Air-Firn Interface Histogram (second result, fresnel2)
    TCanvas *c9 = new TCanvas("c9", "fresnel2",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.87);
    fresnel_2->Draw("HIST B");
    
    //c10.The Charge of Neutrinos Histogram (currentint)
    TCanvas *c10 = new TCanvas("c10", "currentint", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.87);
    currentint_hist->Draw("HIST B");
    
    //c11.Dtrying Direction Histogram (dtryingdirection)
    TCanvas *c11 = new TCanvas("c11", "dtryingdirection", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.90);
    gStyle->SetStatY(0.87);
    dtrying_hist->Draw("HIST B");
    
    //c12.The Traverse Chords of Neutrinos Histogram (chord_kgm2_bestcase)
    TCanvas *c12 = new TCanvas("c12", "chord_kgm2_bestcase", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    chord_kgm2_bestcase_hist->Draw("HIST B");
    
    //c13.Best Case Weight Histogram (weight_bestcase)
    TCanvas *c13 = new TCanvas("c13", "weight_bestcase", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    weight_bestcase_hist->Draw("HIST B");
    
    //c14.Log of Chord Length (logchord)
    TCanvas *c14 = new TCanvas("c14", "logchord", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    logchord_hist->Draw("HIST B");
    
    //c15. Surface Distance of Neurtino When Exiting Balloon Histogram (r_exit2bn)
    TCanvas *c15 = new TCanvas("c15","r_exit2bn", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    r_exit2bn_hist->Draw("HIST B");
    
    //c16.Measured Surface Distance of Neurtino When Exiting Balloon Histogram (r_exit2bn_measured)
    TCanvas *c16 = new TCanvas("c16", "r_exit2bn_measured", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    r_exit2bn_measured_hist->Draw("HIST B");
    
    //c17.Exit Earth Histogram (nuexit)
    TCanvas *c17 = new TCanvas("c17","nuexitlength (magnitude)", 1500,1000);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    nuexit_hist->Draw("HIST B");
    
    //c18.Exit Ice Histogram (nuexitice)
    TCanvas *c18 = new TCanvas("c18", "nuexitice", 1500,1000);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    nuexitice_hist->Draw("HIST B");
    
    //c19.Interaction Angles->Costheta Histogram (costheta_nutraject)
    TCanvas *c19 = new TCanvas("c19", "costheta_nutraject", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    costheta_nutraject_hist->Draw("HIST B");
    
    //c20.Earth Entrance to rock-ice boundary Histogram (d1)
    TCanvas *c20 = new TCanvas("c20", "d1", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    d1_hist->Draw("HIST B");
    
    //c21.Ice-rock Boundary to Interaction Point Histogram (d2)
    TCanvas *c21 = new TCanvas("c21", "d2", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    d2_hist->Draw("HIST B");
    
    //c22.Earth Entrance to Interaction Point Histogram (d1+d2)
    TCanvas *c22 = new TCanvas("c22", "d1+d2", 1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    d1plusd2_hist->Draw("HIST B");
    
    //c23. Direction of Neutrinos (nnu) (+z in south pole direction)
    TCanvas *c23 = new TCanvas("c23","nnu",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.90);
    gStyle->SetStatY(0.90);
    nnu_hist->Draw("colz");
    
    //c26.Theta_Nutraject in Degrees Histogram (theta)
    TCanvas *c26 = new TCanvas("c26","theta",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    theta_hist->Draw("HIST B");
    
    //c27.Position where Neutrino Enters the Earth Histogram (r_in)
    TCanvas *c27 = new TCanvas("c27", "r_in", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    r_in_hist->Draw("HIST B");
    
    //c28.Position where Neutrino Enters the Ice Histogram (r_enterice)
    TCanvas *c28 = new TCanvas("c28", "r_enterice", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    r_enterice_hist->Draw("HIST B");
    
    //c29.Chord from Earth Entrance to Rock-Ice Boundary Histogram (chord)
    TCanvas *c29 = new TCanvas("c29", "chord", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    chord_hist->Draw("HIST B");
    
    //c30.Deviation from the Cerenkov Angle Histogram (dviewangle_deg)
    TCanvas *c30 = new TCanvas("c30", "dviewangle_deg", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    dviewangle_deg_hist->Draw("HIST B");
    
    //c31.Phi Angle of the Ray from the Surface to the Balloon Histogram (n_exit_phi)
    TCanvas *c31 = new TCanvas("c31", "n_exit_phi", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    n_exit_phi_hist->Draw("HIST B");
    
    //c32.Mytheta Histogram (mytheta)
    TCanvas *c32 = new TCanvas("c32", "mytheta",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    mytheta_hist->Draw("HIST B");
    
    //c33.Mybeta Histogram (mybeta)
    TCanvas *c33 = new TCanvas("c33", "mybeta",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    mybeta_hist->Draw("HIST B");
    
    //c34.Polar Angle of the Signal at the Balloon Histogram (theta_rf_atbn)
    TCanvas *c34 = new TCanvas("c34", "theta_rf_atbn", 1100, 850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    theta_rf_atbn_hist->Draw("HIST B");
    
    //c35.NNU Cos(Theta) and Phi Histograms (nnucostheta, nnuphi)
    TCanvas *c35 = new TCanvas("c35", "NNU Graphs", 1100, 850);
    c35->Divide(2);
    c35->cd(1);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    nnucostheta_hist->Draw("HIST B");
    c35->cd(2);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.94);
    gStyle->SetStatY(0.94);
    nnuphi_hist->Draw("HIST B");
    
    //c36.Cos(Theta) and Phi of Position where Neutrino Enters the Earth Histogram (rincostheta, rinph)
    TCanvas *c36 = new TCanvas("c36", "r_in cos(theta) and Phi", 2200, 850);
    c36->Divide(2);
    c36->cd(1);
    c36->cd(2);
    
    c36->cd(1);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.33);
    gStyle->SetStatY(0.88);
    rincostheta_hist->Draw("HIST B");
    
    c36->cd(2);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.50);
    gStyle->SetStatY(0.88);
    rinphi_hist->Draw("HIST B");
    
    //Theta vs Phi Histogram (theta,phi); The plot does not look good since phi is too small;
    TCanvas *c38 = new TCanvas("c38", "theta_vs_phi",1100,850);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    theta_vs_phi_hist->Draw("colz");
    
    //Save Plots
    
    gStyle->SetHistLineWidth(9);
    c1->SaveAs("Primaries/posnu_xy.png");
    gStyle->SetHistLineWidth(2);
    c1->SaveAs("Primaries/posnu_xy.pdf");
    c1->SaveAs("Primaries/posnu_xy.root");
    
    gStyle->SetHistLineWidth(9);
    c2->Print("Primaries/altitude_int.png");
    gStyle->SetHistLineWidth(2);
    c2->Print("Primaries/altitude_int.pdf");
    c2->Print("Primaries/altitude_int.root");
    
    gStyle->SetHistLineWidth(9);
    c3->Print("Primaries/r_fromballoon.png");
    gStyle->SetHistLineWidth(2);
    c3->Print("Primaries/r_fromballoon.pdf");
    c3->Print("Primaries/r_fromballoon.root");
    
    gStyle->SetHistLineWidth(9);
    c4->Print("Primaries/nuflavorint.png");
    gStyle->SetHistLineWidth(2);
    c4->Print("Primaries/nuflavorint.pdf");
    c4->Print("Primaries/nuflavorint.root");
    
    gStyle->SetHistLineWidth(9);
    c5->Print("Primaries/r_fromballoon_sq.png");
    gStyle->SetHistLineWidth(2);
    c5->Print("Primaries/r_fromballoon_sq.pdf");
    c5->Print("Primaries/r_fromballoon_sq.root");
    
    c6->SaveAs("Primaries/r_fromballoon_vs_altitude_int.png");
    c6->SaveAs("Primaries/r_fromballoon_vs_altitude_int.pdf");
    c6->SaveAs("Primaries/r_fromballoon_vs_altitude_int.root");
    
    gStyle->SetHistLineWidth(9);
    c7->SaveAs("Primaries/r_fromballoon_sq_vs_altitude_int.png");
    gStyle->SetHistLineWidth(2);
    c7->SaveAs("Primaries/r_fromballoon_sq_vs_altitude_int.pdf");
    c7->SaveAs("Primaries/r_fromballoon_sq_vs_altitude_int.root");
    
    gStyle->SetHistLineWidth(9);
    c8->Print("Primaries/fresnel1.png");
    gStyle->SetHistLineWidth(2);
    c8->Print("Primaries/fresnel1.pdf");
    c8->Print("Primaries/fresnel1.root");
    
    gStyle->SetHistLineWidth(9);
    c9->Print("Primaries/fresnel2.png");
    gStyle->SetHistLineWidth(2);
    c9->Print("Primaries/fresnel2.pdf");
    c9->Print("Primaries/fresnel2.root");
    
    gStyle->SetHistLineWidth(9);
    c10->Print("Primaries/currentint.png");
    gStyle->SetHistLineWidth(2);
    c10->Print("Primaries/currentint.pdf");
    c10->Print("Primaries/currentint.root");
    
    gStyle->SetHistLineWidth(9);
    c11->Print("Primaries/dtryingdirection.png");
    gStyle->SetHistLineWidth(2);
    c11->Print("Primaries/dtryingdirection.pdf");
    c11->Print("Primaries/dtryingdirection.root");
    
    gStyle->SetHistLineWidth(9);
    c12->Print("Primaries/chord_kgm2_bestcase.png");
    gStyle->SetHistLineWidth(2);
    c12->Print("Primaries/chord_kgm2_bestcase.pdf");
    c12->Print("Primaries/chord_kgm2_bestcase.root");
    
    gStyle->SetHistLineWidth(9);
    c13->Print("Primaries/weight_bestcase.png");
    gStyle->SetHistLineWidth(2);
    c13->Print("Primaries/weight_bestcase.pdf");
    c13->Print("Primaries/weight_bestcase.root");
    
    gStyle->SetHistLineWidth(9);
    c14->Print("Primaries/logchord.png");
    gStyle->SetHistLineWidth(2);
    c14->Print("Primaries/logchord.pdf");
    c14->Print("Primaries/logchord.root");
    
    gStyle->SetHistLineWidth(9);
    c15->Print("Primaries/r_exit2bn.png");
    gStyle->SetHistLineWidth(2);
    c15->Print("Primaries/r_exit2bn.pdf");
    c15->Print("Primaries/r_exit2bn.root");
    
    gStyle->SetHistLineWidth(9);
    c16->Print("Primaries/r_exit2bn_measured.png");
    gStyle->SetHistLineWidth(2);
    c16->Print("Primaries/r_exit2bn_measured.pdf");
    c16->Print("Primaries/r_exit2bn_measured.root");
    
    gStyle->SetHistLineWidth(9);
    c17->Print("Primaries/nuexit.png");
    gStyle->SetHistLineWidth(2);
    c17->Print("Primaries/nuexit.pdf");
    c17->Print("Primaries/nuexit.root");
    
    gStyle->SetHistLineWidth(9);
    c18->Print("Primaries/nuexitice.png");
    gStyle->SetHistLineWidth(2);
    c18->Print("Primaries/nuexitice.pdf");
    c18->Print("Primaries/nuexitice.root");
    
    gStyle->SetHistLineWidth(9);
    c19->Print("Primaries/costheta_nutraject.png");
    gStyle->SetHistLineWidth(2);
    c19->Print("Primaries/costheta_nutraject.pdf");
    c19->Print("Primaries/costheta_nutraject.root");
    
    gStyle->SetHistLineWidth(9);
    c20->Print("Primaries/d1.png");
    gStyle->SetHistLineWidth(2);
    c20->Print("Primaries/d1.pdf");
    c20->Print("Primaries/d1.root");
    
    gStyle->SetHistLineWidth(9);
    c21->Print("Primaries/d2.png");
    gStyle->SetHistLineWidth(2);
    c21->Print("Primaries/d2.pdf");
    c21->Print("Primaries/d2.root");
    
    gStyle->SetHistLineWidth(9);
    c22->Print("Primaries/d1+d2.png");
    gStyle->SetHistLineWidth(2);
    c22->Print("Primaries/d1+d2.pdf");
    c22->Print("Primaries/d1+d2.root");
    
    gStyle->SetHistLineWidth(9);
    c23->Print("Primaries/nnu.png");
    gStyle->SetHistLineWidth(2);
    c23->Print("Primaries/nnu.pdf");
    c23->Print("Primaries/nnu.root");
    
    gStyle->SetHistLineWidth(9);
    c26->Print("Primaries/theta.png");
    gStyle->SetHistLineWidth(2);
    c26->Print("Primaries/theta.pdf");
    c26->Print("Primaries/theta.root");
    
    gStyle->SetHistLineWidth(9);
    c27->Print("Primaries/r_in.png");
    gStyle->SetHistLineWidth(2);
    c27->Print("Primaries/r_in.pdf");
    c27->Print("Primaries/r_in.root");
    
    gStyle->SetHistLineWidth(9);
    c28->Print("Primaries/r_enterice.png");
    gStyle->SetHistLineWidth(2);
    c28->Print("Primaries/r_enterice.pdf");
    c28->Print("Primaries/r_enterice.root");
    
    gStyle->SetHistLineWidth(9);
    c29->Print("Primaries/chord.png");
    gStyle->SetHistLineWidth(2);
    c29->Print("Primaries/chord.pdf");
    c29->Print("Primaries/chord.root");
    
    gStyle->SetHistLineWidth(9);
    c30->Print("Primaries/deviewangle_deg.png");
    gStyle->SetHistLineWidth(2);
    c30->Print("Primaries/deviewangle_deg.pdf");
    c30->Print("Primaries/deviewangle_deg.root");
    
    gStyle->SetHistLineWidth(9);
    c31->Print("Primaries/n_exit_phi.png");
    gStyle->SetHistLineWidth(2);
    c31->Print("Primaries/n_exit_phi.pdf");
    c31->Print("Primaries/n_exit_phi.root");
    
    gStyle->SetHistLineWidth(9);
    c32->Print("Primaries/mytheta.png");
    gStyle->SetHistLineWidth(2);
    c32->Print("Primaries/mytheta.pdf");
    c32->Print("Primaries/mytheta.root");
    
    gStyle->SetHistLineWidth(9);
    c33->Print("Primaries/mybeta.png");
    gStyle->SetHistLineWidth(2);
    c33->Print("Primaries/mybeta.pdf");
    c33->Print("Primaries/mybeta.root");
    
    gStyle->SetHistLineWidth(9);
    c34->Print("Primaries/theta_rf_atbn.png");
    gStyle->SetHistLineWidth(2);
    c34->Print("Primaries/theta_rf_atbn.pdf");
    c34->Print("Primaries/theta_rf_atbn.root");
    
    gStyle->SetHistLineWidth(9);
    c35->Print("Primaries/nnu_costheta_cosphi.png");
    gStyle->SetHistLineWidth(2);
    c35->Print("Primaries/nnu_costheta_cosphi.pdf");
    c35->Print("Primaries/nnu_costheta_cosphi.root");
    
    gStyle->SetHistLineWidth(9);
    c36->Print("Primaries/rin_costheta_phi.png");
    gStyle->SetHistLineWidth(2);
    c36->Print("Primaries/rin_costheta_phi.pdf");
    c36->Print("Primaries/rin_costheta_phi.root");
    
    gStyle->SetHistLineWidth(9);
    c38->SaveAs("Primaries/theta_vs_phi.png");
    gStyle->SetHistLineWidth(1);
    c38->SaveAs("Primaries/theta_vs_phi.pdf");
    c38->SaveAs("Primaries/theta_vs_phi.root");
    
    return 0;
    
}
    
    
    
    
    
    
