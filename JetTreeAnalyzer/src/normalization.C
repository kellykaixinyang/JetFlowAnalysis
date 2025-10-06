//#define MyClass_cxx

//#include "include/MyTrimPythia.h"
//#include "include/coordinateTools.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include "TH1D.h"
#include <TH2D.h>
#include "TMath.h"

#include "include/1d2d_constants.h"
//
#include <iostream>
#include <iomanip>
//
//#include <vector>
//#include "math.h"
//#include <numeric>
//#include <string>
//#include <fstream>
//#include <algorithm>
//#include <iterator>
//
//#include <TStyle.h>
//#include "TH1D.h"
//#include "TH2D.h"
//#include "TF1.h"
//#include "TCanvas.h"
//#include "TPad.h"
//#include "TProfile.h"
//#include "TGraph.h"
//#include "TMath.h"
//#include "TFile.h"
//#include "TTree.h"
//#include "TCutG.h"
//#include "TRandom3.h"

// Opening root file
TFile *file = TFile::Open("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/result_1_txt.root");

// Function that performs normalization and division
void yieldDistr() {
	TH1F *hjetpass = (TH1F*)file->Get("hJet_Pass");

	//Initialize 2D histograms
	TH2D *hSignalShiftedUnc[trackbin][ptbin][1]; 
	TH2D *hBckrndShiftedUnc[trackbin][ptbin][1];
	TH2D *hYield[trackbin][ptbin][1];

	//Boolean array that checks whether histogram is empty
	//std::vector<bool> histFlags(trackbin);
	bool histFlags[trackbin];

	for (int i = 0; i < trackbin; i++) { //loop over multiplicity bins
		double njet = hjetpass->GetBinContent(i+1); //number of jet in the multiplicity bin
		histFlags[i] = (njet != 0);
		if (njet == 0) continue;
		
		for( int j = 0; j < ptbin;    j++) { //loop over particle pt bini

			std::string signalName = Form("hSigS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1);
			std::string bckrndName = Form("hBckS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1);


			// Retrieve histograms from file
			hSignalShiftedUnc[i][j][0] = (TH2D*)(file->Get(signalName.c_str()));
			hBckrndShiftedUnc[i][j][0] = (TH2D*)(file->Get(bckrndName.c_str()));
			
			//**Normalizing singal**
			hSignalShiftedUnc[i][j][0]->Scale(1.0/njet);

			//**Dividing by background**
			int binX = hBckrndShiftedUnc[i][j][0]->GetXaxis()->FindBin(static_cast<Double_t>(0.0));
			int binY = hBckrndShiftedUnc[i][j][0]->GetYaxis()->FindBin(static_cast<Double_t>(0.0));

			double Bckrnd00 = hBckrndShiftedUnc[i][j][0]->GetBinContent(binX,binY); // does this include the (0, 0) value?
			hYield[i][j][0] = (TH2D*)hSignalShiftedUnc[i][j][0]->Clone(Form("hYield[%d][%d][0]", i, j));

			hYield[i][j][0]->Divide(hBckrndShiftedUnc[i][j][0]);
			hYield[i][j][0]->Scale(1.0*Bckrnd00);
		}
	}
	//std::cout<<hSignalShiftedUnc[0][0][0]->GetEntries()<<std::endl;
	TFile* fS_tempA = new TFile("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/result_norm_1.root", "recreate");
	for(int wtrk = 1; wtrk < trackbin+1; wtrk++){
		for(int wppt = 1; wppt < ptbin+1; wppt++){
	//hjetpass->Write();	
	//std::cout<<hSignalShiftedUnc[wtrk-1][wppt-1][0]->GetEntries()<<std::endl;
			if (!histFlags[wtrk-1]) continue;
 		
			hSignalShiftedUnc[wtrk-1][wppt-1][0]->Write(Form("hSigS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[wtrk-1],trackbinboundsUpper[wtrk-1] ,(int)(10*ptbinbounds_lo[wppt-1]),(int)(10*ptbinbounds_hi[wppt-1]),1 ));
			hYield[wtrk-1][wppt-1][0]->Write(Form("hYield_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[wtrk-1],trackbinboundsUpper[wtrk-1] ,(int)(10*ptbinbounds_lo[wppt-1]),(int)(10*ptbinbounds_hi[wppt-1]),1 ));
		}
	}
	fS_tempA->Close();
}

//Code execution starts here
int main() {

	// Opening root file
	//	TFile *file = TFile::Open("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/result_txt.root");
	 
	//Call the function to execute tasks
	yieldDistr();
	
	file->Close();
    
	// After performing the task, exit the program immediately
	return 0;  // This will end the program.
}
