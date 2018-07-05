#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TTree.h"

using namespace std;

int main(int argc, char **argv){
	
	if(argc<2){
		cout<<"Ivalid usage!"<<endl;
		cout<<"Usage is "<<argv[0]<<" path/to/icefinal.root"<<endl;
		return -1;
	}
	
    
	string readfile = argv[1]; //get a string that contains the filename
	TFile *file = new TFile((readfile).c_str()); //open that file
	
	double posnu[3]; //3 element array to hold the x, y, and z of the neutrino interaction position

	TTree *passing_events = (TTree*) file->Get("passing_events"); //get this tree
	passing_events -> SetBranchAddress("posnu",&posnu); //this is a variable we want inside the tree
	int num_pass = passing_events->GetEntries(); //find out how many events passed
	cout<<"Num passing: "<<num_pass<<endl; //we can also print how many events passed the trigger
	
    gStyle->SetOptStat(11);
    
	TH2D *posnu_xy = new TH2D("posnu_xy","posnu xy", 100, -2050000, 3000000, 100, -2000000, 2000000); //make a holder histogram
	
	for (int i=0; i<num_pass; i++){ //loop over all of the events in the tree
		passing_events->GetEvent(i); //get the evente
		//now print out some info
	/*	cout<<"Event "<<i<<endl;
		cout<<"	X: "<<posnu[0]<<endl;
		cout<<"	Y: "<<posnu[1]<<endl;
		cout<<"	Z: "<<posnu[2]<<endl; */
		posnu_xy -> Fill(posnu[0], posnu[1]); //put the event into the histogram
	}
	
	TCanvas *c = new TCanvas("c","Location",1100,850); //make a square canvas
	posnu_xy->Draw("colz"); //draw the histogram
    posnu_xy->SetTitle("Locations of Interactions in Antarctica, E=10^20.00");
	posnu_xy->GetXaxis()->SetTitle("X Coordinate (m)"); //set the x-axis title
    posnu_xy->GetXaxis()->SetTitleOffset(1.2);
    posnu_xy->GetXaxis()->CenterTitle();
	posnu_xy->GetYaxis()->SetTitle("Y Coordinate (m)"); //set the y-axis title
    posnu_xy->GetYaxis()->SetTitleOffset(1.5);
    posnu_xy->GetYaxis()->CenterTitle();
	c->SaveAs("Primariesplots/posnu_xy.pdf"); //save it
	delete posnu_xy; //cleanup
	delete c;
	
	file->Close(); //close the file we loaded
	delete file; //more clean up
	return 0; //return successfully!
}
