#define Events_cxx
#include "Events.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Events::Loop()
{
    //   In a ROOT session, you can do:
    //      root> .L Events.C
    //      root> Events t
    //      root> t.GetEntry(12); // Fill t data members with entry number 12
    //      root> t.Show();       // Show values of entry 12
    //      root> t.Show(16);     // Read and show values of entry 16
    //      root> t.Loop();       // Loop on all entries
    //
    
    //     This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //  Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //       To read only selected branches, Insert statements like:
    // METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    // METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    //by  b_branchname->GetEntry(ientry); //read only this branch
    if (fChain == 0) return;
    
    
    
    // Define some simple structures
    typedef struct {Float_t electron,muon,tau;} VARIABLES; //define a struct, so i can see a variable for every particle
    
    VARIABLES pt;
    VARIABLES phi;
    VARIABLES eta;
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    THStack *hs1 = new THStack("hs1","pT stacked");
    THStack *hs2 = new THStack("hs2","phi stacked");
    THStack *hs3 = new THStack("hs3","eta stacked");
       //gStyle->SetPalette(kOcean);
    
    TH1F* h1 = new TH1F("h1", "electron pT", 100, 0.0, 250.0);
    TH1F* h2 = new TH1F("h2", "electron phi", 100, -4, 4);
    TH1F* h3 = new TH1F("h3", "electron eta", 100, -6, 6);
    TH1F* h4 = new TH1F("h4", "muon pT", 100, 0.0, 250.0);
    TH1F* h5 = new TH1F("h5", "muon phi", 100, -4, 4);
    TH1F* h6 = new TH1F("h6", "muon eta", 100, -6, 6);
    TH1F* h7 = new TH1F("h7", "tau pT", 100, 0.0, 250.0);
    TH1F* h8 = new TH1F("h8", "tau phi", 100, -4, 4);
    TH1F* h9 = new TH1F("h9", "tau eta", 100, -6, 6);


    int ilept=-1; // lepton index in LHEPart collection
    
    
    
    
    // PARTE RELATIVA AL FILE OK
    TFile* output = new TFile("output_T.root","RECREATE"); //create a new file root
    
    TTree* top = new TTree("top", "top"); //initailize a Tree
    
    top->Branch("pT",&pt,"electron:muon:tau"); //divide the branch pT in three leaf, one for every particle
    top->Branch("phi",&phi,"electron:muon:tau");
    top->Branch("eta",&eta,"electron:muon:tau");
    //top->Branch("h1","TH1F",&h1);
    
    
    
    
    
    
    int nev=0; // number of events
    int nmu=0; // number of muons
    int provamu=0; //debug flag
    int nele=0; //number of electrons
    int ntau=0; // number of taus
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) { //loop over entries
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        int lept_type=-1; // lepton pdgID type
        for (int i=0; i<=nLHEPart; i++) { //loop over the
            
            // if (Cut(ientry) < 0) continue;
            if(abs(LHEPart_pdgId[i])==11)  {
                nele++; //increment count number of particle
                ilept = i;
                lept_type=11;
            }
            if(abs(LHEPart_pdgId[i])==13){
                nmu++; //increment count number of particle
                ilept = i;
                lept_type=13;
            }
            if(abs(LHEPart_pdgId[i])==15){
                ntau++; //increment count number of particle
                ilept = i;
                lept_type=15;
            }
            switch (lept_type) {
                case 11:
                    h1->Fill(LHEPart_pt[ilept]);
                    h1->SetFillColor(kRed);
                    h2->Fill(LHEPart_phi[ilept]);
                    h2->SetFillColor(kRed);
                    h3->Fill(LHEPart_eta[ilept]);
                    h3->SetFillColor(kRed);
                    
                    pt.electron=LHEPart_pt[ilept];
                    phi.electron=LHEPart_phi[ilept];
                    eta.electron=LHEPart_eta[ilept];

                    top->Fill();
                    
                    
                    break;
                case 13:
                    
                    h4->Fill(LHEPart_pt[ilept]);
                    h4->SetFillColor(kBlue);
                    h5->Fill(LHEPart_phi[ilept]);
                    h5->SetFillColor(kBlue);
                    h6->Fill(LHEPart_eta[ilept]);
                    h6->SetFillColor(kBlue);
                    
                    pt.muon=LHEPart_pt[ilept];
                    phi.muon=LHEPart_phi[ilept];
                    eta.muon=LHEPart_eta[ilept];
                    
                    top->Fill();
                    
                    break;
                case 15:
                    
                    h7->Fill(LHEPart_pt[ilept]);
                    h7->SetFillColor(kGreen);
                    h8->Fill(LHEPart_phi[ilept]);
                    h8->SetFillColor(kGreen);
                    h9->Fill(LHEPart_eta[ilept]);
                    h9->SetFillColor(kGreen);
                    
                    
                    pt.tau=LHEPart_pt[ilept];
                    phi.tau=LHEPart_phi[ilept];
                    eta.tau=LHEPart_eta[ilept];
                    
                    top->Fill();
                    
                    break;
                default:
                    break;
            }
        }
        nev++;
        // Fill TH1F with
        // LHEPart_pt[ilept] LHEPart_eta[ilept] LHEPart_phi[ilept]
        
    }
    //Write and close file
    output->Write();
    output->Close();
    
    //Stacking all the histograms
    hs1->Add(h1);
    hs2->Add(h2);
    hs3->Add(h3);
    hs1->Add(h4);
    hs2->Add(h5);
    hs3->Add(h6);
    hs1->Add(h7);
    hs2->Add(h8);
    hs3->Add(h9);
    
    
    
    
    //Draw histograms in 3 canvases, one for each variable
   TCanvas *c1 = new TCanvas("c1","pT");
    c1->Divide(2,2);
    c1->cd(1);
    hs1->Draw();
    c1->cd(2);
    h1->Draw();

    c1->cd(3);
    h4->Draw();

    c1->cd(4);
    h7->Draw();
    
    
    TCanvas *c2 = new TCanvas("c2","phi");
    c2->Divide(2,2);
    c2->cd(1);
    hs2->Draw();
    c2->cd(2);
    h2->Draw();

    c2->cd(3);
    h5->Draw();

    c2->cd(4);
    h8->Draw();

    TCanvas *c3 = new TCanvas("c3","eta");
    c3->Divide(2,2);
    c3->cd(1);
    hs3->Draw();
    c3->cd(2);
    h3->Draw();

    c3->cd(3);
    h6->Draw();
    c3->cd(4);
    h9->Draw();
    

    
    cout <<" TOTALE EVENTI " <<nev<<endl;
    cout << " Numero muoni " <<nmu<< " | n_mu/n_tau="<< (double) nmu/ntau<<endl;
    cout << " Numero elettroni " <<nele<< " | n_ele/n_tau="<< (double) nele/ntau<<endl;
    cout << " Numero tau " <<ntau<< " | 2*n_tau/(n_ele+n_mu)="<< (double) 2*ntau/(nele+nmu)<<endl;
}
