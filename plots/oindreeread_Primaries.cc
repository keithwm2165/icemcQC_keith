#include <iostream>
#include <fstream>
#include <array>
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
// #include "trigger.hh"  (Not exist in Icemc heading anymore)
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


/////////////////////////////////PLOTTING SCRIPT WRITTEN BY OINDREE BANERJEE DURING SUMMER OF 2015////////////////////////////////////////////////REVISED BY VICTORIA NIU DURING SUMMER OF 2018

/* Variable posnu2, theta_fromtir, weight_nu, weight_nu_prob, nuexit can not be written out in the lastest Icemc (June 2018), they are commented out below. Ask icemc team if you want to write them. Also, they can be directly read out without writing them into a variable address */

int main(int argc, char *argv[])

{
  gStyle->SetOptStat(111111);

  if(argc<2)
    {
      cout << "Not enough arguments! Stop run. " << endl;
    }

  TH2D *hposnuXY = new TH2D("posnuXY", "; posnu[0]; posnu[1]", 100, -5000000, 5000000, 100, -4000000, 4000000);
  TH2D *hposnuYZ = new TH2D("posnuYZ", "; posnu[1]; posnu[2]", 100, -4000000, 4000000, 100, 5e6, 7e6); 
  TH2D *hposnuXZ = new TH2D("posnuXZ", "; posnu[0]; posnu[2]", 100, -5000000, 5000000, 100, 5e6, 7e6); 
 // TH2D *hposnu2 = new TH2D("posnu2", "; posnu2[0]; posnu2[1]", 100, -500000, 500000, 100, -400000, 400000);
  TH1D *hcostheta_nutraject = new TH1D("costheta_nutraject", "; costheta_nutraject; counts", 100, 0, 0.2); 
  TH2D *hnnu = new TH2D("nnu", "; nnu[0]; nnu[1]", 100, -2, 2, 100, -2, 2); 
  TH1D *hn_exit_phi = new TH1D("n_exit_phi", "; n_exit_phi; counts", 100, 0, 400);  
  TH1D *hpnu = new TH1D("pnu", "; pnu; counts", 100, 0, 1e19); 
  TH1D *hphi_nutraject = new TH1D("phi_nutraject", "; phi_nutraject; counts", 100, 0, 1e-4); //-4
  TH1D *haltitude_int = new TH1D("altitude_int", "; altitude_int; counts", 100, -3500, 0); 
  TH1D *hr_fromballoon = new TH1D("r_fromballoon", "; r_fromballoon; counts", 100, 0, 700000); 
  TH1D *hdtryingdirection = new TH1D("dtryingdirection", "; dtryingdirection; counts", 100, 0, 2); 
  TH1D *hdviewangle_deg = new TH1D("dviewangle_deg", "; dviewangle_deg; counts", 100, -30, 20); 
  TH1D *hd1 = new TH1D("d1", "; d1; counts", 100, 0, 600000); 
  TH1D *hd2 = new TH1D("d2", "; d2; counts", 100, 0, 500000); 
  TH1D *hd1plusd2 = new TH1D("d1plusd2", "; d1plusd2; counts", 100, 0, 1100000); 
  TH1D *hr_fromballoon_sq = new TH1D("r_fromballoon_sq", "; r_fromballoon_sq; counts", 100, 0, 5e11);   
  TH2D *hr_fromballoon_vs_altitude_int = new TH2D("r_fromballoon_vs_altitude_int", "; r_fromballoon; altitude_int", 100, 0, 7e5, 100, -3500, 0); 
  TH2D *hr_fromballoon_sq_vs_altitude_int = new TH2D("r_fromballoon_sq_vs_altitude_int", "; r_fromballoon_sq; altitude_int", 100, 0, 5e11, 100, -3500, 0); 
  TH1D *hmytheta = new TH1D("mytheta", "; mytheta; counts", 100, 0, 20); 
  TH1D *hmybeta = new TH1D("mybeta", "; mybeta; counts", 100, -20, 20); 
  TH1D *hfresnel1 = new TH1D("fresnel1", "; fresnel1; counts", 100, .8, 1.2); 
  TH1D *hfresnel2 = new TH1D("fresnel2", "; fresnel2; counts", 100, .8, 1.2); 
  TH2D *htheta_vs_phi = new TH2D("theta_vs_phi", "; theta; phi", 100, 0, 180, 100, 0, 360);  
  TH1D *htheta = new TH1D("theta", "; theta; counts", 100, 0, 180); 
  TH1D *hnuflavor = new TH1D("nuflavor", "; nuflavor; counts", 100, 0, 4); 
  TH1D *htheta_rf_atbn = new TH1D("theta_rf_atbn", "; theta_rf_atbn; counts", 100, 20, 110); 
  TH2D *htheta_vs_r_fromballoon_sq = new TH2D("theta_vs_r_fromballoon_sq", "; theta; r_fromballoon_sq", 100, 0, 180, 100, 0, 5e11);
 // TH1D *hweight_nu = new TH1D("weight_nu","; weight_nu; counts",100,-.2,.2);
 // TH1D *hweight_nu_prob = new TH1D("weight_nu_prob","; weight_nu_prob; counts",100,-.0001,.0001);
  TH1D *hr_in = new TH1D("r_in","; r_in; counts",100,-7000000,7000000);
  TH1D *hr_enterice = new TH1D("r_enterice","; r_enterice; counts",100,-3000000,7000000);
 // TH1D *hnuexit = new TH1D("nuexit","; nuexit; counts", 100,-1000000,1000000);
  TH1D *hnuexitice = new TH1D("nuexitice","; nuexitice; counts", 100,-1000000,1000000);
  TH1D *hchord = new TH1D("chord","; chord; counts",100,1000,11000000);
  TH1D *hlogchord=new TH1D("logchord","; logchord; counts",100,0,10);
  TH1D *hweight_bestcase= new TH1D("weight_bestcase","; weight_bestcase; counts",100,-1,1);
  TH1D *hchord_kgm2_bestcase = new TH1D("chord_kgm2_bestcase","; chord_kgm2_bestcase; counts",100,1e9,4e10);
  TH1D *hr_exit2bn = new TH1D("r_exit2bn","; r_exit2bn; counts",100,0e3,300e3);
  TH1D *hr_exit2bn_measured = new TH1D("r_exit2bn_measured","; r_exit2bn_measured; counts",100,20e3,240e3);
  TH1D *hr_fromballoon_db = new TH1D("r_fromballoon_db","; r_fromballoon_db; counts", 100,0e-72,2e-72);
  

 //create list of histograms 
  TH2D *hist2D[9]= {hposnuXY,hposnuYZ,hposnuXZ,hposnu2,hnnu,hr_fromballoon_vs_altitude_int,hr_fromballoon_sq_vs_altitude_int,htheta_vs_phi,htheta_vs_r_fromballoon_sq};
  TH1D *hist1D[19]={hcostheta_nutraject,hn_exit_phi,hpnu,hphi_nutraject,haltitude_int,hr_fromballoon,hdtryingdirection,hdviewangle_deg,hd1,hd2,hd1plusd2,hr_fromballoon_sq,hmytheta,hmybeta,hfresnel1,hfresnel2,htheta,hnuflavor,htheta_rf_atbn};

  
  //loop goes through lists and turn off stat boxes for all 
  /* for(int i=0;i<9;i++){//2D histograms
    hist2D[i] ->SetStats(false);
  }
  for(int i=0;i<19;i++){//1D histograms
    hist1D[i] ->SetStats(false);
    }*/

  hd1 ->GetXaxis()->SetTitle("Length of chord from earth entrance to rock-ice boundary(meters)");
  hd1 ->GetYaxis()->SetTitle("number of triggered events");

  hd2 ->GetXaxis()->SetTitle("Distance from ice-rock boundary to interaction point (meters)");
  hd2 ->GetYaxis()->SetTitle("number of triggered events");

  hcostheta_nutraject ->GetXaxis()->SetTitle("theta of nnu with earth center to balloon as z-axis");
  hcostheta_nutraject ->GetYaxis()->SetTitle("number of triggered events");

  //haltitude_int->GetXaxis()->SetTitle("Depth of the interaction");
  //haltitude_int->GetYaxis()->SetTitle("number of triggered events");

  hr_fromballoon ->GetXaxis()->SetTitle("Distance from interaction to balloon for each ray(meters)");
  hr_fromballoon ->GetYaxis()->SetTitle("number of triggered events");

  hposnuXY->GetXaxis()->SetTitle("Position of neutrino in X plane(meters)");
  hposnuXY->GetYaxis()->SetTitle("Position of neutrino in Y plane(meters)");
  hposnuYZ->GetXaxis()->SetTitle("Position of neutrino in Y plane(meters)");
  hposnuYZ->GetYaxis()->SetTitle("Position of neutrino in Z plane(meters)");
  hposnuXZ->GetXaxis()->SetTitle("Position of neutrino in X plane(meters)");
  hposnuXZ->GetYaxis()->SetTitle("Position of neutrino in Z plane(meters)");

  //hnnu->GetXaxis()->SetTitle("");//fix
  //hnnu->GetYaxis()->SetTitle("?");

  hn_exit_phi->GetXaxis()->SetTitle("phi angle of the ray from the surface to the balloon");
  hn_exit_phi->GetYaxis()->SetTitle("number of triggered events");

  hpnu->GetXaxis()->SetTitle("Energy of the Neutrinos");
  hpnu->GetYaxis()->SetTitle("number of triggered events");

  hphi_nutraject->GetXaxis()->SetTitle("phi angle of nnu with earth center to the balloon as z-axis");
  hphi_nutraject->GetYaxis()->SetTitle("number of triggered events");

  haltitude_int->GetXaxis()->SetTitle("depth of interaction(meters)");
  haltitude_int->GetYaxis()->SetTitle("number of triggered events");

  hdviewangle_deg->GetXaxis()->SetTitle("deviation from the cerenkov angle");
  hdviewangle_deg->GetYaxis()->SetTitle("number of triggered events");
  
  hmytheta->GetXaxis()->SetTitle("alpha minus 90 degrees");
  hmytheta->GetYaxis()->SetTitle("number of triggered events");

  hmybeta->GetXaxis()->SetTitle("beta minus 90 degrees");
  hmybeta->GetYaxis()->SetTitle("number of triggered events");

  hfresnel1 ->GetXaxis()->SetTitle("net fresnel factor on field at ice-firn interface");
  hfresnel1->GetYaxis()->SetTitle("number of triggered events");

  hfresnel2 ->GetXaxis()->SetTitle("net fresnel factor on field at firn air interface");
  hfresnel2->GetYaxis()->SetTitle("number of triggered events");

  htheta_rf_atbn ->GetXaxis()->SetTitle("polar angle of the signal as seen by perfect eyes at the balloon(degrees)");
  htheta_rf_atbn->GetYaxis()->SetTitle("number of triggered events");

  hweight_nu ->GetXaxis()->SetTitle("weight for neutrino that survies to posnu");
  hweight_nu->GetYaxis()->SetTitle("number of triggered events");

  hweight_nu_prob ->GetXaxis()->SetTitle("weight for neutrino that survives and interacts with ice");
  hweight_nu_prob->GetYaxis()->SetTitle("number of triggered events");

  hr_enterice ->GetXaxis()->SetTitle("postition where neutrino enters the ice");
  hr_enterice->GetYaxis()->SetTitle("number of triggered events");

  hnuexit->GetXaxis()->SetTitle("position where neutrino would have left the Earth");
  hnuexit->GetYaxis()->SetTitle("number of triggered events");

  hnuexitice->GetXaxis()->SetTitle("position where neutrino would have left the ice");
  hnuexitice->GetYaxis()->SetTitle("number of triggered events");

  hchord->GetXaxis()->SetTitle("chord in m from earth entrance to rock-ice boundary");
  hchord->GetYaxis()->SetTitle("number of triggered events");

  hlogchord->GetXaxis()->SetTitle("log_10 of chord length earth entrance to where it enters ice");
  hlogchord->GetYaxis()->SetTitle("number of triggered events");

  hweight_bestcase->GetXaxis()->SetTitle("what weight1 would be if whole earth had density of crust");
  hweight_bestcase->GetYaxis()->SetTitle("number of triggered events");

  for(int i=1; i < argc; i++)

    {// loop over the input files 

    string readfile = string(argv[i]); 
    TFile *AnitaFile = new TFile(( readfile ).c_str());
    cout << "AnitaFile" << endl;  
    TTree *passing_events = (TTree*)AnitaFile->Get("passing_events"); 
    
    // DECLARE VARIABLE TYPES 

    int num_pass = passing_events->GetEntries();  // not a branch, defining this for this program
    cout << "number of passing events is " << num_pass << endl; 
    int inu;
    double vmmhz_min; 
    double vmmhz_max;
    double horizcoord;
    double vertcoord; 
    double horizcoord_bn;
    double vertcoord_bn;
    double r_bn[3];
    double n_bn[3];
    double lomgitude_bn; 
    double weight; 
    double logweight; 
    double posnu[3];
    double posnu2[3];
    double costheta_nutraject;
    double chord_kgm2_ice;
    double phi_nutraject; 
    double altitude_int;     //depth of interaction 
    double nnu[3];
    double n_exit_phi;
    double pnu; 
    double elast_y;
    double emfrac;
    double hadfrac; 
    double sumfrac;
    int nuflavor;
    double r_fromballoon;     //surface distance from balloon of interaction
    double viewangle; 
    double offaxis; 
    double e_component;
    double h_component;
    double cosalpha;
    double mytheta;
    double mybeta; 
    double fresnel1;
    double fresnel2;
    double taulength;
    double icethickness;
    double dviewangle_deg; 
    double d1;
    double d2; 
    double theta_threshold_deg; 
    // double theta_fromtir;
    double d1plusd2; 
    double r_fromballoon_sq;     //r_fromballoon squared 
    double theta;               // arccos of costheta_nutraject in degrees 
    double phi;                // phi_nutraject in degrees 
    double theta_rf_atbn; 
    double weight_nu; 
    double weight_nu_prob;
    double r_in;
    double r_enterice;
    double nuexit;
    double nuexitice;
    double chord;
    double logchord;
    double weight_bestcase;
    double chord_kgm2_bestcase;
    double dtryingdirection;
    double r_exit2bn;
    double r_exit2bn_measured;
    double r_fromballoon_db;
    
  // SET BRANCH ADDRESS FOR ALL THE VARIABLES 
  
    passing_events ->SetBranchAddress("inu",&inu);
    passing_events ->SetBranchAddress("vmmhz_min",&vmmhz_min);
    passing_events ->SetBranchAddress("vmmhz_max",&vmmhz_max);
    passing_events ->SetBranchAddress("horizcoord",&horizcoord);
    passing_events ->SetBranchAddress("vertcoord",&vertcoord);
    passing_events ->SetBranchAddress("horizcoord_bn",&horizcoord_bn);
    passing_events ->SetBranchAddress("vertcoord_bn",&vertcoord_bn);
    passing_events ->SetBranchAddress("r_bn",&r_bn);
    passing_events ->SetBranchAddress("n_bn",&n_bn);
    passing_events ->SetBranchAddress("lomgitude_bn",&lomgitude_bn);
    passing_events ->SetBranchAddress("weight",&weight);
    passing_events ->SetBranchAddress("logweight",&logweight);
    passing_events ->SetBranchAddress("posnu",&posnu); 
  //  passing_events ->SetBranchAddress("posnu2",&posnu2);
    passing_events ->SetBranchAddress("costheta_nutraject",&costheta_nutraject);
    passing_events ->SetBranchAddress("chord",&chord); 
    passing_events ->SetBranchAddress("chord_kgm2_ice",&chord_kgm2_ice);
    passing_events ->SetBranchAddress("phi_nutraject",&phi_nutraject);
    passing_events ->SetBranchAddress("altitude_int",&altitude_int);
    passing_events ->SetBranchAddress("nnu",&nnu);
    passing_events ->SetBranchAddress("n_exit_phi",&n_exit_phi);
    passing_events ->SetBranchAddress("pnu",&pnu); 
    passing_events ->SetBranchAddress("elast_y",&elast_y);
    passing_events ->SetBranchAddress("emfrac",&emfrac);
    passing_events ->SetBranchAddress("hadfrac",&hadfrac);
    passing_events ->SetBranchAddress("sumfrac",&sumfrac);
    passing_events ->SetBranchAddress("nuflavor",&nuflavor);
    passing_events ->SetBranchAddress("logchord",&logchord);
    passing_events ->SetBranchAddress("r_fromballoon",&r_fromballoon); 
    passing_events ->SetBranchAddress("nuexitice",&nuexitice);
    passing_events ->SetBranchAddress("weight_bestcase",&weight_bestcase);
    passing_events ->SetBranchAddress("chord_kgm2_bestcase",&chord_kgm2_bestcase);
    passing_events ->SetBranchAddress("dtryingdirection",&dtryingdirection);
    passing_events ->SetBranchAddress("viewangle",&viewangle);
    passing_events ->SetBranchAddress("offaxis",&offaxis);
    passing_events ->SetBranchAddress("e_component",&e_component);
    passing_events ->SetBranchAddress("h_component",&h_component);
    passing_events ->SetBranchAddress("cosalpha",&cosalpha);
    passing_events ->SetBranchAddress("mytheta",&mytheta);
    passing_events ->SetBranchAddress("mybeta",&mybeta);
    passing_events ->SetBranchAddress("fresnel1",&fresnel1);
    passing_events ->SetBranchAddress("fresnel2",&fresnel2);
    passing_events ->SetBranchAddress("taulength",&taulength);
    passing_events ->SetBranchAddress("icethickness",&icethickness);
    passing_events ->SetBranchAddress("dviewangle_deg",&dviewangle_deg); 
    passing_events ->SetBranchAddress("d1",&d1); 
    passing_events ->SetBranchAddress("d2",&d2); 
    passing_events ->SetBranchAddress("theta_threshold_deg",&theta_threshold_deg); 
  // passing_events ->SetBranchAddress("theta_fromtir",&theta_fromtir);
    passing_events ->SetBranchAddress("theta_rf_atbn",&theta_rf_atbn); 
  // passing_events ->SetBranchAddress("weight_nu",&weight_nu);
  //  passing_events ->SetBranchAddress("weight_nu_prob",&weight_nu_prob);
    passing_events ->SetBranchAddress("r_in",&r_in);
    passing_events ->SetBranchAddress("r_enterice",&r_enterice);
  //  passing_events ->SetBranchAddress("nuexit",&nuexit);
    passing_events ->SetBranchAddress("nuexitice",&nuexitice);
    passing_events ->SetBranchAddress("chord",&chord);
    passing_events ->SetBranchAddress("logchord",&logchord);
    passing_events ->SetBranchAddress("weight_bestcase",&weight_bestcase);
    passing_events ->SetBranchAddress("chord_kgm2_bestcase",&chord_kgm2_bestcase);
    passing_events ->SetBranchAddress("r_exit2bn",&r_exit2bn);
    passing_events ->SetBranchAddress("r_exit2bn_measured",&r_exit2bn_measured);
    passing_events ->SetBranchAddress("r_fromballoon_db",&r_fromballoon_db);

    // PRINT VALUES OF VARIABLE TO TERMINAL AND ALSO FILL THE CORRESPONDING HISTOGRAM 

    for (int j=0; j <=num_pass; j++) 

      {

	passing_events -> GetEvent(j);

	d1plusd2 = d1 + d2; 
	r_fromballoon_sq = r_fromballoon * r_fromballoon; 
	theta = (acos(costheta_nutraject)) * 57.3;     // change from radians to degrees 
	phi = phi_nutraject * 57.3;                    // change from radians to degrees 
    

	//cout << "posnu[0] is " << posnu[0] << endl; 
	//cout << "posnu[1] is " << posnu[1] << endl; 
	//cout << "posnu2[0] is " << posnu2[0] << endl;  
	//cout << "posnu2[1] is " << posnu2[1] << endl; 
	//cout << "nnu[0] is " << nnu[0] << endl; 
	//cout << "nnu[1] is " << nnu[1] << endl; 
	//cout << "pnu is " << pnu << endl; 
	//cout << "phi_nutraject is " << phi_nutraject << endl; 
	//cout << "dtryingdirection is " << dtryingdirection << endl; 
	//cout << "n_exit_phi is " << n_exit_phi << endl; 
	//cout << "altitude_int is " << altitude_int << endl; 
	//cout << "costheta_nutraject is " << costheta_nutraject << endl; 
	//cout << "dviewangle_deg is " << dviewangle_deg << endl; 
	//cout << "d1 is " << d1 << endl; 
	//cout << "d2 is " << d2 << endl; 
	//cout << "d1plusd2 is " << d1 + d2 << endl; 
	//cout << "r_fromballoon is " << r_fromballoon << endl; 
	//cout << "r_fromballoon_sq is " << r_fromballoon_sq << endl; 
	//cout << "posnu[2] is " << posnu[2] << endl; 
	//cout << "mytheta is " << mytheta << endl; 
	//cout << "mybeta is " << mybeta << endl; 
	//cout << "fresnel1 is " << fresnel1 << endl; 
	// cout << "fresnel2 is " << fresnel2 << endl; 
	//cout << "theta is " << theta << endl;
	//cout << "phi is " << phi << endl; 
	//cout << "nuflavor is " << nuflavor << endl; 

	hposnuXY->Fill(posnu[0],posnu[1],weight);
	hposnuYZ->Fill(posnu[1],posnu[2],weight); 
	hposnuXZ->Fill(posnu[0],posnu[2],weight); 
	hposnu2->Fill(posnu2[0],posnu2[1],weight); 
	hcostheta_nutraject->Fill(costheta_nutraject, weight);
	hnnu->Fill(nnu[0],nnu[1],weight);
	hn_exit_phi->Fill(n_exit_phi,weight); 
	hpnu->Fill(pnu,weight); 
	hphi_nutraject->Fill(phi_nutraject,weight); 
	haltitude_int->Fill(altitude_int,weight);
	hr_fromballoon->Fill(r_fromballoon,weight); 
	hdtryingdirection->Fill(dtryingdirection,weight); 
	hdviewangle_deg->Fill(dviewangle_deg,weight); 
	hd1->Fill(d1,weight); 
	hd2->Fill(d2,weight); 
	hd1plusd2->Fill(d1plusd2,weight); 
	hr_fromballoon_sq->Fill(r_fromballoon_sq,weight); 
	hr_fromballoon_vs_altitude_int->Fill(r_fromballoon,altitude_int,weight); 
	hr_fromballoon_sq_vs_altitude_int->Fill(r_fromballoon_sq,altitude_int,weight); 
	hmytheta->Fill(mytheta,weight); 
	hmybeta->Fill(mybeta,weight); 
	hfresnel1->Fill(fresnel1,weight);
	hfresnel2->Fill(fresnel2,weight); 
	htheta_vs_phi->Fill(theta,phi,weight); 
	htheta->Fill(theta,weight); 
	hnuflavor->Fill(nuflavor,weight); 
	htheta_vs_r_fromballoon_sq->Fill(theta,r_fromballoon_sq,weight); 
	htheta_rf_atbn->Fill(theta_rf_atbn*57.295779513,weight); /////////////////////changes
	hweight_nu->Fill(weight_nu,weight);
	hweight_nu_prob ->Fill(weight_nu_prob,weight);
        hr_in->Fill(r_in,weight);
	hr_enterice->Fill(r_enterice,weight);
	hnuexit->Fill(nuexit,weight);
	hnuexitice->Fill(nuexitice,weight);
	hchord->Fill(chord,weight);
	hlogchord ->Fill(logchord,weight);
	hweight_bestcase->Fill(weight_bestcase,weight);
	hchord_kgm2_bestcase ->Fill(chord_kgm2_bestcase,weight);
	hr_exit2bn ->Fill(r_exit2bn,weight);
	hr_exit2bn_measured ->Fill(r_exit2bn_measured,weight);
       	hr_fromballoon_db ->Fill(r_fromballoon_db,weight);
      }  // CLOSE FOR LOOP OVER NUMBER OF EVENTS 

    } // CLOSE FOR LOOP OVER NUMBER OF INPUT FILES 

  // ONCE DONE FILLING HISTOGRAMS WITH DATA FROM ALL INPUT FILES, DRAW AND SAVE HISTOGRAMS 


  TCanvas *c1 = new TCanvas("c1","posnu",3500,850);

  c1->Divide(4,1); 
  c1->cd(1);
  hposnuXY->Draw("colz");

  c1->cd(2);
  hposnuYZ->Draw("colz");
  
  c1->cd(3);
  hposnuXZ->Draw("colz"); 

  TCanvas *c2 = new TCanvas("c2","posnu2",1100,850);
  hposnu2->Draw("colz");

  TCanvas *c3 = new TCanvas("c3","nnu",1100,850);
  hnnu->Draw("colz"); 

  TCanvas *c4 = new TCanvas("c4","pnu",1100,850); 
  hpnu->Draw();

  TCanvas *c5 = new TCanvas("c5", "phi_nutraject",1100,850); 
  hphi_nutraject->Draw();

  TCanvas *c6 = new TCanvas("c6", "dtryingdirection",1100,850); 
  hdtryingdirection->Draw();

  TCanvas *c7 = new TCanvas("c7", "n_exit_phi",1100,850);
  hn_exit_phi->Draw();

  TCanvas *c8 = new TCanvas("c8", "altitude_int",1100,850); 
  haltitude_int->Draw();

  TCanvas *c9 = new TCanvas("c9", "costheta_nutraject",1100,850); 
  hcostheta_nutraject->Draw();

  TCanvas *c10 = new TCanvas("c10", "dviewangle_deg",1100,850);
  hdviewangle_deg->Draw();

  TCanvas *c11 = new TCanvas("c11", "d1",1100,850); 
  hd1->Draw();

  TCanvas *c12 = new TCanvas("c12", "d2",1100,850); 
  hd2->Draw();

  TCanvas *c13 = new TCanvas("c13", "d1plusd2",1100,850); 
  hd1plusd2->Draw();

  TCanvas *c14 = new TCanvas("c14", "r_fromballoon",1100,850);
  hr_fromballoon->Draw();

  TCanvas *c15 = new TCanvas("c15", "r_fromballoon_sq",1100,850); 
  hr_fromballoon_sq->Draw();

  TCanvas *c16 = new TCanvas("c16", "r_fromballoon_vs_altitude_int",1100,850);
  hr_fromballoon_vs_altitude_int->Draw("colz");
  
  TCanvas *c17 = new TCanvas("c17", "r_fromballoon_sq_vs_altitude_int",1100,850);
  hr_fromballoon_sq_vs_altitude_int->Draw("colz");

  TCanvas *c18 = new TCanvas("c18", "mytheta",1100,850);
  hmytheta->Draw();

  TCanvas *c19 = new TCanvas("c19", "mybeta",1100,850); 
  hmybeta->Draw();

  TCanvas *c20 = new TCanvas("c20", "fresnel1",1100,850);
  hfresnel1->Draw();

  TCanvas *c21 = new TCanvas("c21", "fresnel2",1100,850); 
  hfresnel2->Draw();

  TCanvas *c22 = new TCanvas("c22", "theta_vs_phi",1100,850); 
  htheta_vs_phi->Draw("colz");

  TCanvas *c23 = new TCanvas("c23", "theta",1100,850);
  htheta->Draw();

  TCanvas *c24 = new TCanvas("c24", "nuflavor",1100,850);
  hnuflavor->Draw();

  TCanvas *c25 = new TCanvas("c25", "theta_vs_r_fromballoon_sq",1100,850);
  htheta_vs_r_fromballoon_sq->Draw("colz");  

  TCanvas *c26 = new TCanvas("c26", "theta_rf_atbn", 1100, 850); 
  htheta_rf_atbn->Draw(); 

  TCanvas *c27 = new TCanvas("c27","weight_nu",1100, 850);
  hweight_nu -> Draw();

  TCanvas *c28 = new TCanvas("c28","weight_nu_prob",1100,850);
  hweight_nu_prob -> Draw();
  
  TCanvas *c29 = new TCanvas("c29","r_i",1100,850);
  hr_in -> Draw();

  TCanvas *c30 = new TCanvas("c30","r_enterice",1100,850);
  hr_enterice ->Draw();

  TCanvas *c31 = new TCanvas("c31","nuexit",1100,850);
  hnuexit -> Draw();

  TCanvas *c32 = new TCanvas("c32","nuexitice",1100,850);
  hnuexitice -> Draw();

  TCanvas *c33= new TCanvas("c33","chord",1100,850);
  hchord -> Draw();

  TCanvas *c34 = new TCanvas("c34","logchord",1100,850);
  hlogchord -> Draw();

  TCanvas *c35 = new TCanvas("c35","weight_bestcase",1100,850);
  hweight_bestcase -> Draw();

  TCanvas *c36 = new TCanvas("c36","chord_kgm2_bestcase",1100,850);
  hchord_kgm2_bestcase ->Draw();

  TCanvas *c37 = new TCanvas("c37","r_exit2bn",1100,850);
  hr_exit2bn -> Draw();

  TCanvas *c38 = new TCanvas("c38","r_exit2bn_measured",1100,850);
  hr_exit2bn_measured -> Draw();

  TCanvas *c39 = new TCanvas("c39","r_fromballoon_db",1100,850);
  hr_fromballoon_db -> Draw();

  c1->SaveAs("plots/Primaries_posnu.png");
  c1->SaveAs("plots/Primaries_posnu.pdf");
  c1->SaveAs("plots/Primaries_posnu.root");

  c2->SaveAs("plots/Primaries_posnu2.png");
  c2->SaveAs("plots/Primaries_posnu2.pdf");
  c2->SaveAs("plots/Primaries_posnu2.root");

   
  c3->SaveAs("plots/Primaries_nnu.png");
  c3->SaveAs("plots/Primaries_nnu.pdf");
  c3->SaveAs("plots/Primaries_nnu.root");

   
  c4->SaveAs("plots/Primaries_pnu.png");
  c4->SaveAs("plots/Primaries_pnu.pdf");
  c4->SaveAs("plots/Primaries_pnu.root");


  c5->SaveAs("plots/Primaries_phinutraject.png");
  c5->SaveAs("plots/Primaries_phinutraject.pdf");
  c5->SaveAs("plots/Primaries_phinutraject.root");


  c6->SaveAs("plots/Primaries_dtryingdirection.png");
  c6->SaveAs("plots/Primaries_dtryingdirection.pdf");
  c6->SaveAs("plots/Primaries_dtryingdirection.root");


  c7->SaveAs("plots/Primaries_nexitphi.png");
  c7->SaveAs("plots/Primaries_nexitphi.pdf");
  c7->SaveAs("plots/Primaries_nexitphi.root");
   

  c8->SaveAs("plots/Primaries_altitudeint.png"); 
  c8->SaveAs("plots/Primaries_altitudeint.pdf"); 
  c8->SaveAs("plots/Primaries_altitudeint.root"); 


  c9->SaveAs("plots/Primaries_costheta_nutraject.png");
  c9->SaveAs("plots/Primaries_costheta_nutraject.pdf");
  c9->SaveAs("plots/Primaries_costheta_nutraject.root");
   

  c10->SaveAs("plots/Primaries_dviewangle_deg.png");
  c10->SaveAs("plots/Primaries_dviewangle_deg.pdf");
  c10->SaveAs("plots/Primaries_dviewangle_deg.root");


  c11->SaveAs("plots/Primaries_d1.png");
  c11->SaveAs("plots/Primaries_d1.pdf");
  c11->SaveAs("plots/Primaries_d1.root");


  c12->SaveAs("plots/Primaries_d2.png");
  c12->SaveAs("plots/Primaries_d2.pdf");
  c12->SaveAs("plots/Primaries_d2.root");


  c13->SaveAs("plots/Primaries_d1plusd2.png");
  c13->SaveAs("plots/Primaries_d1plusd2.pdf");
  c13->SaveAs("plots/Primaries_d1plusd2.root");


  c14->SaveAs("plots/Primaries_rfromballoon.png");
  c14->SaveAs("plots/Primaries_rfromballoon.pdf");
  c14->SaveAs("plots/Primaries_rfromballoon.root");

 
  c15->SaveAs("plots/Primaries_rfromballoonsq.png");
  c15->SaveAs("plots/Primaries_rfromballoonsq.pdf");
  c15->SaveAs("plots/Primaries_rfromballoonsq.root");


  c16->SaveAs("plots/Primaries_rfromballoonvsaltitudeint.png");
  c16->SaveAs("plots/Primaries_rfromballoonvsaltitudeint.pdf");
  c16->SaveAs("plots/Primaries_rfromballoonvsaltitudeint.root");


  c17->SaveAs("plots/Primaries_rfromballoonsqvsaltitudeint.png"); 
  c17->SaveAs("plots/Primaries_rfromballoonsqvsaltitudeint.pdf"); 
  c17->SaveAs("plots/Primaries_rfromballoonsqvsaltitudeint.root"); 


  c18->SaveAs("plots/Primaries_mytheta.png"); 
  c18->SaveAs("plots/Primaries_mytheta.pdf"); 
  c18->SaveAs("plots/Primaries_mytheta.root"); 


  c19->SaveAs("plots/Primaries_mybeta.png");
  c19->SaveAs("plots/Primaries_mybeta.pdf");
  c19->SaveAs("plots/Primaries_mybeta.root");


  c20->SaveAs("plots/Primaries_fresnel1.png");
  c20->SaveAs("plots/Primaries_fresnel1.pdf");
  c20->SaveAs("plots/Primaries_fresnel1.root");
   

  c21->SaveAs("plots/Primaries_fresnel2.png");
  c21->SaveAs("plots/Primaries_fresnel2.pdf");
  c21->SaveAs("plots/Primaries_fresnel2.root");
   

  c22->SaveAs("plots/Primaries_thetavsphi.png");
  c22->SaveAs("plots/Primaries_thetavsphi.pdf");
  c22->SaveAs("plots/Primaries_thetavsphi.root");


  c23->SaveAs("plots/Primaries_theta.png"); 
  c23->SaveAs("plots/Primaries_theta.pdf"); 
  c23->SaveAs("plots/Primaries_theta.root"); 


  c24->SaveAs("plots/Primaries_nuflavor.png");
  c24->SaveAs("plots/Primaries_nuflavor.pdf");
  c24->SaveAs("plots/Primaries_nuflavor.root");


  c25->SaveAs("plots/Primaries_thetavsrfromballoonsq.png");
  c25->SaveAs("plots/Primaries_thetavsrfromballoonsq.pdf");
  c25->SaveAs("plots/Primaries_thetavsrfromballoonsq.root");


  c26->SaveAs("plots/Primaries_thetarfatbn.png");
  c26->SaveAs("plots/Primaries_thetarfatbn.pdf");
  c26->SaveAs("plots/Primaries_thetarfatbn.root");

  c27->SaveAs("plots/Primaries_weight_nu.png");
  c27->SaveAs("plots/Primaries_weight_nu.pdf");
  c27->SaveAs("plots/Primaries_weight_nu.root");
  
  c28->SaveAs("plots/Primaries_weight_nu_prob.png");
  c28->SaveAs("plots/Primaries_weight_nu_prob.pdf");
  c28->SaveAs("plots/Primaries_weight_nu_prob.root");

  c29->SaveAs("plots/Primaries_r_in.png");
  c29->SaveAs("plots/Primaries_r_in.pdf");
  c29->SaveAs("plots/Primaries_r_in.root");

  c30->SaveAs("plots/Primaries_r_enterice.png");
  c30->SaveAs("plots/Primaries_r_enterice.pdf");
  c30->SaveAs("plots/Primaries_r_enterice.root");
 
  c31->SaveAs("plots/Primaries_nuexit.png");
  c31->SaveAs("plots/Primaries_nuexit.pdf");
  c31->SaveAs("plots/Primaries_nuexit.root");

  c32->SaveAs("plots/Primaries_nuexitice.png");
  c32->SaveAs("plots/Primaries_nuexitice.pdf");
  c32->SaveAs("plots/Primaries_nuexitice.root");

  c33->SaveAs("plots/Primaries_chord.png");
  c33->SaveAs("plots/Primaries_chord.pdf");
  c33->SaveAs("plots/Primaries_chord.root");

  c34->SaveAs("plots/Primaries_logchord.png");
  c34->SaveAs("plots/Primaries_logchord.pdf");
  c34->SaveAs("plots/Primaries_logchord.root");

  c35->SaveAs("plots/Primaries_weight_bestcase.png");
  c35->SaveAs("plots/Primaries_weight_bestcase.pdf");
  c35->SaveAs("plots/Primaries_weight_bestcase.root");

  c36->SaveAs("plots/Primaries_chord_kgm2_bestcase.png");
  c36->SaveAs("plots/Primaries_weight_chord_kgm2_bestcase.pdf");
  c36->SaveAs("plots/Primaries_chord_kgm2_bestcase.root");

  c37->SaveAs("plots/Primaries_r_exit2bn.png");
  c37->SaveAs("plots/Primaries_r_exit2bn.pdf");
  c37->SaveAs("plots/Primaries_r_exit2bn.root");
 
  c38->SaveAs("plots/Primaries_r_exit2bn _measured.png");
  c38->SaveAs("plots/Primaries_r_exit2bn _measured.pdf");
  c38->SaveAs("plots/Primaries_r_exit2bn _measured.root");

   c39->SaveAs("plots/Primaries_r_fromballoon_db.png");
   c39->SaveAs("plots/Primaries_r_fromballoon_db.pdf");
   c39->SaveAs("plots/Primaries_r_fromballoon_db.root");
  
  cout <<"changes have been made"<<endl;

}                                                                   //main ends 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
