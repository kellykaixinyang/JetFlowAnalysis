
#include "include/1d2d_constants.h"
#include <TROOT.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include "TH1D.h"
#include <TH2D.h>
#include "TMath.h"
#include <TApplication.h> 

//
//#include <iostream>
//#include <iomanip>
void DrawFlow(){
    TH2D* hSignal[10][3];//Ntrk,pt
    TH2D* hSignaluw[10][3];
    TH2D* hBkg[10][3];
    TH1D* hEEC[10][3]; // this is EEC with v2 = 0
    TH1D* hEEC_1[10][3]; // EEC with v2 = 0.1
    TH1D* hEEC_3[10][3];// EEC with v2 = 0.3
    TH1D* hEEC_ratio1[10][3];
    TH1D* hEEC_ratio2[10][3];
    TH2D* hSignal_ratio[10][3];

    //TH2D* h2DCorr[5][2];
    TH1D* h1DFlow[10][3];
    //TFile* f= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/ana_run3.root","READ");
    //TFile* f_bkg= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/Bkg_highMult_jets_run3.root","READ");
    //TFile* f= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/new_default_complete_vn.root","READ");
    //TFile* f= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/ana_run3_allNch.root","READ");
    TFile* f= new TFile("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/result_1_txt.root","READ");
    TFile* f2= new TFile("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/result_unweighted_txt.root","READ");
    //TFile* f= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/output_parkersbin.root","READ");
    //TFile* f= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/output_parkersbin_newBkg_for_high_Nch.root","READ");
    
    //int   trackbinbounds[5]= {76,78,80,81,82};
    //int ptbinbounds[2]={3,5};
    //const int   trackbinbounds[10]         = { 0,20,30,40,50,59,66,76,83,78};
    //const int   trackbinboundsUpper[10]    = {20,30,40,50,59,66,76,83,1000,1000};
    
    float ptname[3]={0.3,0.5,1.0};
    int YPlo=28;
    int YPhi=34;
    TH1D* hJetPass = (TH1D*)f->Get("hJet_Pass550");
    //TH1D* hJetPass = (TH1D*)f->Get("hJet_Pass550_hltCor");
    
    //for(int i=0;i<5;i++){
    for(int i=0;i<10;i++){
        for(int j=0;j<3;j++){
            //hSignal[i][j]=(TH2D*)f->Get(Form("hSigS_Cor_%d_to_1000_and_%d_to_30_w_PU_1",trackbinbounds[i],ptbinbounds[j]));
            //hBkg[i][j]=(TH2D*)f_bkg->Get(Form("hBckS_Cor_%d_to_1000_and_%d_to_30_w_PU_1",trackbinbounds[i],ptbinbounds[j]));
            hSignal[i][j]=(TH2D*)f->Get(Form("hSigS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1));
            hSignaluw[i][j]=(TH2D*)f2->Get(Form("hSigS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1));
            hBkg[i][j]=(TH2D*)f->Get(Form("hBckS_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1)); 
            hEEC[i][j]=(TH1D*)f->Get(Form("hEnergyEnergyCor_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1));
	    hEEC_1[i][j]=(TH1D*)f->Get(Form("hEnergyEnergyCor_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d_1",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1));	
	    hEEC_3[i][j]=(TH1D*)f->Get(Form("hEnergyEnergyCor_Unc_%d_to_%d_and_%d_to_%d_w_PU_%d_3",trackbinbounds[i],trackbinboundsUpper[i] ,(int)(10*ptbinbounds_lo[j]),(int)(10*ptbinbounds_hi[j]),1));
            //std::cout << "Histogram"<<i<<","<<j<<"successfully retrieved"<< std::endl;
            //h2DCorr[i][j]=(TH2D*)hSignal[i][j]->Clone(Form("h2DCorr_Cor_%d_to_1000_and_%d_to_30_w_PU_1",trackbinbounds[i],ptbinbounds[j]));
            //h2DCorr[i][j]->Reset();
            //double eta_bw=hSignal[i][j]->GetXaxis()->GetBinWidth(1);//x:delta eta; y:delta phi
            double phi_bw=hSignal[i][j]->GetYaxis()->GetBinWidth(1);

            hSignal[i][j]->Scale(1.0/(hJetPass->GetBinContent(i+1,j+1)));
            hSignal[i][j]->Scale(1./(phi_bw));
            hSignaluw[i][j]->Scale(1.0/(hJetPass->GetBinContent(i+1,j+1)));
            hSignaluw[i][j]->Scale(1./(phi_bw));


            TH2D* hSignalClone = (TH2D*) hSignal[i][j]->Clone();
            TH2D* hSignaluwClone = (TH2D*)  hSignaluw[i][j]->Clone();

            hSignalClone->Divide(hSignaluwClone);
            hSignal_ratio[i][j]=(TH2D*)hSignalClone->Clone();

            TH1D *histfit1 = (TH1D*) hSignal[i][j]->ProjectionY("",YPlo,YPhi)->Clone();
            TH1D *histfit2 = (TH1D*) hBkg[i][j]->ProjectionY("",YPlo,YPhi)->Clone();

            TH1D *histratio1 = (TH1D*) hEEC_3[i][j]->Clone();
            histratio1->Divide(hEEC[i][j]);
	    TH1D *histratio2 = (TH1D*) hEEC_1[i][j]->Clone();
	    histratio2->Divide(hEEC[i][j]);

            histfit1->Divide(histfit2);
            histfit1->Scale(hBkg[i][j]->GetMaximum());

            hEEC_ratio1[i][j]=(TH1D*)histratio1->Clone();
            hEEC_ratio2[i][j]=(TH1D*)histratio2->Clone();
           // std::cout << "Histogram"<<i<<","<<j<<"successfully added to hEEC_ratio" << std::endl;
            h1DFlow[i][j]=(TH1D*)histfit1->Clone(); 
            std::string function = "[0]/(TMath::Pi()*2)*(1+2*([1]*TMath::Cos(x)+[2]*TMath::Cos(2*x)+[3]*TMath::Cos(3*x)+[4]*TMath::Cos(4*x)+[5]*TMath::Cos(5*x)))";
            TF1 func1("deltaPhi1", function.c_str(), -0.5*TMath::Pi(), 1.5*TMath::Pi());
            func1.SetParameter(0, histfit1->GetMaximum());
            func1.SetParameter(1, 0.1);
            func1.SetParameter(2, 0.1);
            func1.SetParameter(3, 0.1);
            func1.SetParameter(4, 0.1);
            func1.SetParameter(5, 0.1);
            h1DFlow[i][j]->Fit(&func1, "m E q");
            

            // Set the histogram title
            //h1DFlow[i][j]->SetTitle(Form("Nch>%d, %.2f<jT<3 GeV/c",trackbinbounds[i],ptname[j]));
            h1DFlow[i][j]->SetTitle(Form("%d<Nch<%d, %.2f<jT<3 GeV/c",trackbinbounds[i],trackbinboundsUpper[i],ptname[j]));
            hSignal_ratio[i][j]->SetTitle(Form("Signal Ratio with weighted by jT/unweighted,v2=0.3,%d<Nch<%d, %.2f<jT<3 GeV/c",trackbinbounds[i],trackbinboundsUpper[i],ptname[j]));
            hEEC_ratio1[i][j]->SetTitle(Form("Energy-Energy Correlator v2=0.3/v2=0, %d<Nch<%d, %.2f<jT<3 GeV/c",trackbinbounds[i],trackbinboundsUpper[i],ptname[j]));
            hEEC_ratio2[i][j]->SetTitle(Form("Energy-Energy Correlator v2=0.1/v2=0,%d<Nch<%d, %.2f<jT<3 GeV/c",trackbinbounds[i],trackbinboundsUpper[i],ptname[j]));

        // std::cout << "Histogram"<<i<<","<<j<<"successfully named" << std::endl; 
        }
    }
    std::cout << "Histograms all created" << std::endl;
 
    TFile* fS_tempB = new TFile(Form("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/2PCSignalRatio.root"), "recreate");
    TCanvas *c1 = new TCanvas("canvas1", "Fourier Series Fits", 800, 1200);
    TCanvas *c3 = new TCanvas("canvas3", "EEC with Different v2", 800, 1200);
    TCanvas *c5 = new TCanvas("canvas5", "EEC Ratio v2=0.3/v2=0",800, 1200);
    TCanvas *c7 = new TCanvas("canvas7", "EEC Ratio v2=0.1/v2=0", 800, 1200);

    c1->Divide(1, 5); // Divide canvas into 2 columns and 5 rows
    c3->Divide(1,5);
    c5->Divide(1,5);
    c7->Divide(1,5);

    int colors[3] = {kRed, kBlue, kGreen};
    std::cout << "canvas and colors declared" << std::endl;


    for(int i=0;i<5;i++){
        for(int j=0;j<1;j++){
            hSignal_ratio[i][j]->Write(Form("hSigS_Ratio_%d_to_%d",trackbinbounds[i],trackbinboundsUpper[i]));

            c1->cd(i*2+j+1);
            h1DFlow[i][j]->Draw();
            std::cout << "h1dFlow["<<i<<"],["<<"j] entry added" << std::endl;

	    
	    c3->cd(i*2+j+1);
            hEEC[i][j]->SetLineColor(colors[0]);
            hEEC_1[i][j]->SetLineColor(colors[1]);
            hEEC_3[i][j]->SetLineColor(colors[2]);

            hEEC[i][j]->Draw("HIST");
            hEEC_1[i][j]->Draw("HIST SAME");
            hEEC_3[i][j]->Draw("HIST SAME");

            gPad->SetLogy();
            gPad->SetLogx();

            std::cout << "hEEC["<<i<<"],["<<"j] entry added" << std::endl;
            
	    c5->cd(i*2+j+1);
            hEEC_ratio1[i][j]->Draw();
            hEEC_ratio1[i][j]->GetYaxis()->SetRangeUser(0, 2);
           // gPad->SetLogy();
            gPad->SetLogx();


            std::cout << "hEECratio1["<<i<<"],["<<"j] entry added" << std::endl;
	    c7->cd(i*2+j+1);
            hEEC_ratio2[i][j]->Draw();
            hEEC_ratio2[i][j]->GetYaxis()->SetRangeUser(0, 2);
           // gPad->SetLogy();
            gPad->SetLogx();


            std::cout << "hEECratio2["<<i<<"],["<<"j] entry added" << std::endl;

        }
    }
// Adding legend
  c3->cd(0); // Go to the main canvas (not a pad)
  TLegend *legend = new TLegend(0.7, 0.1, 0.9, 0.3);

  TLegendEntry *entry1 = legend->AddEntry((TObject*)0, "v2=0", "l");
  entry1->SetTextColor(colors[0]); 
  TLegendEntry *entry2 = legend->AddEntry((TObject*)0, "v2=0.1", "l");
  entry2->SetTextColor(colors[1]);
  TLegendEntry *entry3 = legend->AddEntry((TObject*)0, "v2=0.3", "l");
  entry3->SetTextColor(colors[2]);



  legend->Draw();
  c3->Update();

  std::cout << "legends added" << std::endl;

    c1->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/pp_projection.pdf(");
    //c1->SaveAs("/Users/xl155/Documents/JetFlow_Run3_data/Flow_run2_unc_new.pdf(");
    //
    //
    c3->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EEC.pdf(");
    c5->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EECratio1.pdf(");
    c7->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EECratio2.pdf(");
    std::cout << "c1,c3, c5,c7 saved" << std::endl;

    TCanvas *c2 = new TCanvas("canvas2", "Fourier Series Fits", 800, 1200);
    TCanvas *c4 = new TCanvas("canvas4", "EEC with Different v2", 800, 1200);
    TCanvas *c6 = new TCanvas("canvas6", "EEC Ratio v2=0.3/v2=0",800, 1200);
    TCanvas *c8 = new TCanvas("canvas8", "EEC Ratio v2=0.1/v2=0", 800, 1200);

    c2->Divide(1, 5); // Divide canvas into 2 columns and 5 rows
    c4->Divide(1,5); 
    c6->Divide(1, 5);
    c8->Divide(1,5);
   for(int i=0;i<5;i++){
        for(int j=0;j<1;j++){

            hSignal_ratio[i+5][j]->Write(Form("hSigS_Ratio_%d_to_%d",trackbinbounds[i+5],trackbinboundsUpper[i+5]));

            c2->cd(i*2+j+1);
            h1DFlow[i+5][j]->Draw();

            c4->cd(i*2+j+1);
            hEEC[i+5][j]->SetLineColor(colors[0]);
            hEEC_1[i+5][j]->SetLineColor(colors[1]);
            hEEC_3[i+5][j]->SetLineColor(colors[2]);

            hEEC[i+5][j]->Draw("HIST");
            hEEC_1[i+5][j]->Draw("HIST SAME");
            hEEC_3[i+5][j]->Draw("HIST SAME");

            gPad->SetLogy();
            gPad->SetLogx();
            
            c6->cd(i*2+j+1);
            hEEC_ratio1[i+5][j]->Draw();
            hEEC_ratio1[i+5][j]->GetYaxis()->SetRangeUser(0, 2);
            //gPad->SetLogy();
            gPad->SetLogx();


            std::cout << "hEECratio1["<<i<<"],["<<"j] entry added" << std::endl;
            c8->cd(i*2+j+1);
            hEEC_ratio2[i+5][j]->Draw();
            hEEC_ratio2[i+5][j]->GetYaxis()->SetRangeUser(0, 2);
            //gPad->SetLogy();
            gPad->SetLogx();
        }
}

// Adding legend
   c4->cd(0); // Go to the main canvas (not a pad)
   TLegend *legend1 = new TLegend(0.7, 0.1, 0.9, 0.3);
//
   TLegendEntry *entry4 = legend1->AddEntry((TObject*)0, "v2=0", "l");
   entry4->SetTextColor(colors[0]);
   TLegendEntry *entry5 = legend1->AddEntry((TObject*)0, "v2=0.1", "l");
   entry5->SetTextColor(colors[1]);
   TLegendEntry *entry6 = legend1->AddEntry((TObject*)0, "v2=0.3", "l");
   entry6->SetTextColor(colors[2]);
//
//
//
  legend1->Draw();
 c4->Update();
    c2->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/pp_projection.pdf)");
    c4->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EEC.pdf)");
    c6->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EECratio1.pdf)");
    c8->SaveAs("/home/ky35/CMSSW_13_3_0/src/JetFlowAnalysis/Output/EECratio2.pdf)");  
    /* 
    //TFile* fout= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/2DCorr_run2_PG_ana_eta_phi_bw.root","RECREATE");
    //TFile* fout= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/2DCorr_run3.root","RECREATE");
    TFile* fout= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/1DFlow_run3_allNch_2023_2024.root","RECREATE");
    //TFile* fout= new TFile("/Users/xl155/Documents/JetFlow_Run3_data/1DFlow_run2_unc_new.root","RECREATE");
    
    //for(int i=0;i<5;i++){
    for(int i=0;i<10;i++){
        for(int j=0;j<2;j++){
            //h2DCorr[i][j]->Write();
            h1DFlow[i][j]->Write();
        }
    }
    */

    f->Close();
    delete f;
    f2->Close();
    delete f2;
    fS_tempB->Close();
    delete fS_tempB;
    //fout->Close();
    //delete fout;
    
}

//Code execution starts here
int main() {

//Call the function to execute tasks
	    DrawFlow();
// After performing the task, exit the program immediately
return 0;  // This will end the program.
}
