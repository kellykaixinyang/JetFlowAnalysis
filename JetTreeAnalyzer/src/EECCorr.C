#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TAttLine.h"
#include "THelix.h"
#include "TView.h"
#include "TRandom.h"
#include "TAttPad.h"
#include "TMath.h"
#include "TVector3.h"
#include "TView3D.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLegend.h"

#include <iostream>
#include <string>
#include <vector>

//#include "SURF_FUNCTIONS.h"
//#include "coordinateTools.h"
//R__LOAD_LIBRARY(SURF_FUNCTIONS_C);

// Global variables
std::string title = "#mu^{+}#mu^{#minus} (3000 GeV)";
Float_t energy = 3000;
std::string fullPathDir = "/Users/kellyyanggoingharvard/Documents/MCdata/data/mupmum_3000GeV.root";
Float_t etaCutOff = 100;
Int_t multCutOff = 194;
TFile *f = new TFile(fullPathDir.c_str(), "read"); // Opening file

// Creating TTreeReader object and linking branches
TTreeReader* reader = new TTreeReader("trackTree");

// Setup branches for other particles
TTreeReaderValue<std::vector<vector<int>>> chgBranch(*reader,"genDau_chg");
TTreeReaderValue<std::vector<vector<float>>> etaBranch(*reader,"genDau_eta");
TTreeReaderValue<std::vector<vector<float>>> phiBranch(*reader,"genDau_phi"); 
TTreeReaderValue<std::vector<vector<float>>> ptBranch(*reader,"genDau_pt");
TTreeReaderValue<std::vector<float>> jetptBranch(*reader,"genJetPt");
TTreeReaderValue<std::vector<float>> jetetaBranch(*reader,"genJetEta");
TTreeReaderValue<std::vector<float>> jetphiBranch(*reader,"genJetPhi");
TTreeReaderValue<std::vector<int>> jetmultiplicityBranch(*reader,"genJetChargedMultiplicity");


// Function that returns jet pt distribution
TH1F createJetPtDistr() {
		
	std::string jetptTitle = "Jet Pt Distribution for " + std::to_string(static_cast<int>(energy)) + " GeV";

	const float ptBW = 10;

	TH1F hJetpt("hJetpt", jetptTitle.c_str(),energy/ptBW,0,1500);
	
	// **** EVENT LOOP ****
	reader->Restart();
	while (reader->Next())  { //looping through the events
		// ***** JET LOOP FOR GIVEN EVENT *****
		for (Int_t i = 0; i < jetptBranch->size(); i++) {
			Float_t jetpt = (*jetptBranch)[i];
			hJetpt.Fill(jetpt);
		}
	}	
	// ***** HISTOGRAM CUSTOMIZATION *****
        hJetpt.GetXaxis()->SetTitle("Pt (GeV)");

        hJetpt.SetTitleFont(200, "T");
        hJetpt.SetTitleFont(200, "XYZ");
        hJetpt.SetLabelFont(132,"T");
        hJetpt.SetLabelFont(132,"XYZ");
	return hJetpt;
}	 	


// Function that returns jet multiplicity distribution
TH1F createNchDistr() {
	std::string jetmultiplicityTitle = "Jet Charge Multiplicity Distribution for " + std::to_string(static_cast<int>(energy)) + " GeV";

	const float NchBW = 1;
	
	TH1F hJetNch("hJetNch",jetmultiplicityTitle.c_str(),100/NchBW,0,100);
	
	// **** EVENT LOOP ****
        reader->Restart();
        while (reader->Next())  { //looping through the events
                // ***** JET LOOP FOR GIVEN EVENT *****
                for (Int_t i = 0; i < jetmultiplicityBranch->size(); i++) {
                        int Nch = (*jetmultiplicityBranch)[i];
			hJetNch.Fill(Nch);
                }
        }
        // ***** HISTOGRAM CUSTOMIZATION *****
        hJetNch.GetXaxis()->SetTitle("N_ch");

        hJetNch.SetTitleFont(200, "T");
        hJetNch.SetTitleFont(200, "XYZ");
        hJetNch.SetLabelFont(132,"T");
        hJetNch.SetLabelFont(132,"XYZ");
        return hJetNch;
}




// Function that returns 1D EEC distribution
TH1F createEnergyDistr(Float_t jetptLB, Float_t jetptUB,int jetmultLB,int jetmultUB) {
		
	std::string energyTitle = "Energy Energy Correlator for " + title;

	//Histogram for the EEC distribution 
	const float deltarBW = 0.001;
	TH1F hEnergy("hEnergy", energyTitle.c_str(),1/deltarBW,0,1);

	// **** EVENT LOOP ****
	reader->Restart(); // Restarting event loop
	//Int_t numSelectedEvents = 0;
	Int_t numPairs = 0;

	while (reader->Next())  { //looping through the events

		// Check to see if the event is in the multiplicity bin
		//Int_t eventIndex = reader->GetCurrentEntry();
		//if (multiplicityVector[eventIndex] < multCutOff) {continue;}
		//numSelectedEvents++;

		// ***** JET LOOP FOR GIVEN EVENT *****
		for (Int_t i = 0; i < etaBranch->size(); i++) { //etaBranch gives number of jets
			Float_t jetpt = (*jetptBranch)[i];
			int jetmult = (*jetmultiplicityBranch)[i];

			//Jet selection criteria

			// pt selection
			if (jetpt < jetptLB || jetpt > jetptUB) {continue;}

			// multiplicity selection
			if (jetmult < jetmultLB || jetmult > jetmultUB) {continue;}

			// eta selection
		//	if (fabs((*jetetaBranch)[i]) >= 1.6) {continue;}
			//if (-0.1 < ((*jetphiBranch)[i]) < 1.2) {continue;}	
		
			// ***** PARTICLE LOOP FOR JET i *****
			//Particle 1 Loop
			for (Int_t j = 0; j < (*etaBranch)[i].size()-1; j++) {

            			// Checking if particle 1 is charged
            			if ((*chgBranch)[i][j] == 0) {continue;}
				
				
			
		
				Float_t eta1 = (*etaBranch)[i][j]; 
            			if (fabs(eta1) > etaCutOff) {continue;} // Checking eta range
				Float_t phi1 = (*phiBranch)[i][j];
			
				Float_t pt1 = (*ptBranch)[i][j];
				// Checking if particle 1 pt reaches threshold
                                if (pt1 < 1) {continue;}

				// Particle 2 Loop	
            			for (Int_t k = j + 1; k < (*etaBranch)[i].size(); k++) {
				
                			// Checking if particle 2 is charged
                			if ((*chgBranch)[i][k] == 0) {continue;}
				
                               	

					Float_t eta2 = (*etaBranch)[i][k];
					if (fabs(eta2) > etaCutOff) {continue;} // Checking eta range
					
					
					Float_t phi2 = (*phiBranch)[i][k];
					
					Float_t pt2 = (*ptBranch)[i][k];
					// Checking if particle 2 pt reaches threshold
                                        if (pt2 < 1) {continue;}

					// Calculating delta eta, delta phi, and delta r
                			Float_t deltaEta = eta2 - eta1;
                			Float_t deltaPhi = TMath::ACos(TMath::Cos(phi2-phi1));	
					Float_t delta_r = sqrt(pow(deltaEta,2)+pow(deltaPhi,2)) / 0.8;

					//Filling 1D Histogram
					Int_t n = 1;
 		
				//	hEnergy.Fill(delta_r*jetpt,pow((pt1*pt2), n));
					hEnergy.Fill(delta_r,pow((pt1*pt2), n));
					numPairs = numPairs + (pt1 * pt2);
				}//ending particle 2 loop
			}//ending particle 1 loop
		}//ending jet loop
	}//ending event loop
	//std::cout << "Number of selected events for Energy Distribution: " << numSelectedEvents << std::end1;

	// ***** NORMALIZATION ******
    
    	hEnergy.Scale(1.0/numPairs);
	hEnergy.Scale(1.0/deltarBW);


	// ***** HISTOGRAM CUSTOMIZATION *****
	hEnergy.GetXaxis()->SetTitle("#deltar");
	
	hEnergy.SetYTitle("EEC");
	hEnergy.GetYaxis()->SetRangeUser(1e-5,1e2);
	
	//hEnergy.GetXaxis()->SetTitleSize(0.05);
	//hEnergy.GetXaxis()->SetTitleFont(64);
	//hEnergy.GetYaxis()->SetTitleSize(0.05);
       // hEnergy.GetYaxis()->SetTitleFont(64);

	hEnergy.SetTitleFont(200, "T");
        hEnergy.SetTitleFont(200, "XYZ");
	
	
	
	hEnergy.SetLabelFont(132,"T");
	hEnergy.SetLabelFont(132,"XYZ");

	return hEnergy;
}

void EECCorr() {
	// Finding the multiplicities of each event and storing it in a vector
    	//std::vector<Int_t> multVec;
	//reader->Restart(); // Ensuring event loop starts from beginning
    	//while (reader->Next()) {
        //	Int_t multiplicity = 0; // Counter for N_ch
        //	for (Int_t i = 0; i < pxBranch->size(); i++) {
          //  		if ((*chgBranch)[i] != 0) {multiplicity++;}
        //	}
        //	multVec.push_back(multiplicity);
        
    //	}
    
	TFile *fout = new TFile("simpleEEC.root", "recreate");

	// Creating canvas for jetpt distribution
	TCanvas *cJetpt = new TCanvas("cJetpt", "Canvas for jet pt distribution", 800, 600);
	TH1F jetptHist = createJetPtDistr();
	
	cJetpt->cd();
	jetptHist.Draw();
	cJetpt->Write();
	jetptHist.Write();

	delete cJetpt;

	// Creating canvas for jetnch distribution
	TCanvas *cJetNch = new TCanvas("cJetNch", "Canvas for jet multiplicity distribution", 800, 600);
	TH1F jetmultHist = createNchDistr();

	cJetNch->cd();
	jetmultHist.Draw();
	cJetNch->Write();

	// Creating canvas for energy distribution
	TCanvas *cEnergy = new TCanvas("cEnergy", "Canvas for delta r Distribution", 800, 600);

	TH1F EEChist1 = createEnergyDistr(300,1000,0,10);
	TH1F EEChist2 = createEnergyDistr(300,1000,25,50);
	TH1F EEChist3 = createEnergyDistr(300,1000,50,75);
	TH1F EEChist4 = createEnergyDistr(300,1000,10,25);
	




	
	EEChist1.SetLineColor(kGreen);
	EEChist2.SetLineColor(kRed);
	EEChist3.SetLineColor(kCyan);

	cEnergy->cd();
	cEnergy->SetLogx();	
	cEnergy->SetLogy();
	

	EEChist1.Draw();
	EEChist2.Draw("same");
	EEChist3.Draw("same");
	EEChist4.Draw("same");
	
	TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  // (x1, y1, x2, y2)

    // 	Add entries to the legend
   	legend->AddEntry(&EEChist1, "N_chg 0-10", "l");
	legend->AddEntry(&EEChist2, "N_chg 25-50", "l");
	legend->AddEntry(&EEChist3, "N_chg 50-75", "l");
	legend->AddEntry(&EEChist4, "N_chg 10-25", "l");

    


   	legend->Draw();

	cEnergy->Write();
	
	

	delete cEnergy;
	f->Close();
	fout->Close();
}
    

	
