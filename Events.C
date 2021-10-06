#define Events_cxx
#include "Events.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Events::Loop()
{

    if (fChain == 0) return;
    
    /*
    
    // Define a simple structure, need it for the tree later
    typedef struct {
        Float_t electron;
        Float_t muon;
        Float_t tau;
    } VARIABLES; //define a struct VARIABLE, so i can see a variable for every particle
    
    //define the variables of interest
    VARIABLES pt;
    VARIABLES phi;
    VARIABLES eta;
    */
    Long64_t nentries = fChain->GetEntriesFast();
    
    //definite histogram and Stack variables
    THStack *hs1 = new THStack("hs1","pT stacked");
    THStack *hs2 = new THStack("hs2","phi stacked");
    THStack *hs3 = new THStack("hs3","eta stacked");
    THStack *hs4 = new THStack("hs4","Electron pT, from rct Particles");
    THStack *hs5 = new THStack("hs5","Muon pT, from rct Particles");
    THStack *hs6 = new THStack("hs4","Tau pT, from rct Particles");
    
    THStack *hs7 = new THStack("hs4","Electron pT, from iso rct Particles");
    THStack *hs8 = new THStack("hs5","Muon pT, from iso rct Particles");
    THStack *hs9 = new THStack("hs4","Tau pT, from iso rct Particles");
    
    /*
    TH1F* h1 = new TH1F("h1", "electron pT", 80, 0.0, 250.0);
    TH1F* h2 = new TH1F("h2", "electron phi", 80, -4, 4);
    TH1F* h3 = new TH1F("h3", "electron eta", 80, -6.5, 6.5);
    TH1F* h4 = new TH1F("h4", "muon pT", 80, 0.0, 250.0);
    TH1F* h5 = new TH1F("h5", "muon phi", 80, -4, 4);
    TH1F* h6 = new TH1F("h6", "muon eta", 80, -6.5, 6.5);
    TH1F* h7 = new TH1F("h7", "tau pT", 80, 0.0, 250.0);
    TH1F* h8 = new TH1F("h8", "tau phi", 80, -4, 4);
    TH1F* h9 = new TH1F("h9", "tau eta", 80, -6.5, 6.5);
    */
    
    TH1F* hRee = new TH1F("hRee", "Electron pT, from rct electron event", 80, 0.0, 250.0);
    TH1F* hRem = new TH1F("hRem", "Electron pT, from rct muon event", 80, 0.0, 250.0);

    TH1F* hRme = new TH1F("hRme", "Muon pT, from rct electron event", 80, 0.0, 250.0);
    TH1F* hRmm = new TH1F("hRmm", "Muon pT, from rct muon event", 80, 0.0, 250.0);
    
    TH1F* hRte = new TH1F("hRte", "Tau pT, from rct electron event", 80, 0.0, 250.0);
    TH1F* hRtm = new TH1F("hRtm", "Tau pT, from rct muon event", 80, 0.0, 250.0);
    
    
    
    TH1F* hRiee = new TH1F("hRiee", "Electron pT, from rct iso electron event", 80, 0.0, 250.0);
    TH1F* hRiem = new TH1F("hRiem", "Electron pT, from rct iso muon event", 80, 0.0, 250.0);
    
    TH1F* hRime = new TH1F("hRime", "Muon pT, from rct iso electron event", 80, 0.0, 250.0);
    TH1F* hRimm = new TH1F("hRimm", "Muon pT, from rct iso muon event", 80, 0.0, 250.0);
    
    TH1F* hRite = new TH1F("hRite", "Tau pT, from rct iso electron event", 80, 0.0, 250.0);
    TH1F* hRitm = new TH1F("hRitm", "Tau pT, from rct iso muon event", 80, 0.0, 250.0);
    
    int ilept=-1; // lepton index in LHEPart collection
    
    
    
    
    // PARTE RELATIVA AL FILE OK
    //TFile* output = new TFile("./output/output_T.root","RECREATE"); //create a new file root
    
    
    
    /*
    TTree* top = new TTree("top", "top"); //initailize a Tree
    
    top->Branch("pT",&pt,"electron:muon:tau"); //divide the branch pT in three leaf, one for every particle
    top->Branch("phi",&phi,"electron:muon:tau"); //same as above
    top->Branch("eta",&eta,"electron:muon:tau"); //''
    //top->Branch("electron pT",&hept,"electronpT");
    */
//TO FURTHER IMPLEMENT: IN THE SAME FILE CREATE A TREE FOR THE TBAR FILE
    
    
    
    
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
        for (int i=0; i<=nLHEPart; i++) { //loop over the particles
            
            // if (Cut(ientry) < 0) continue;
            if(abs(LHEPart_pdgId[i])==11)  { //if electron
                nele++; //increment count number of particle
                ilept = i; //flag for remembering the index
                lept_type=11; //store particle type
                break;
            }
            if(abs(LHEPart_pdgId[i])==13){//see above
                nmu++; //increment count number of particle
                ilept = i;
                lept_type=13;
                break;
            }
            if(abs(LHEPart_pdgId[i])==15){//see above
                ntau++; //increment count number of particle
                ilept = i;
                lept_type=15;
                break;
            }
        }
        switch (lept_type) {
            case 11: //if electron
                /*
                h1->Fill(LHEPart_pt[ilept]); //fill histogram for pt
                h2->Fill(LHEPart_phi[ilept]); //same for phi
                h3->Fill(LHEPart_eta[ilept]); //same for eta
                
                pt.electron=LHEPart_pt[ilept]; //insert pt variable in the structure to further insert the tree
                phi.electron=LHEPart_phi[ilept];//phi for tree
                eta.electron=LHEPart_eta[ilept];//eta for tree
                
                top->Fill(); //fill top branch
                */
                
                

                
                if(nElectron>0){
                    //if(Electron_pfRelIso03_all[0]<0.15)//if there is at least one reconstructed Electron
                    hRee->Fill(Electron_pt[0]); //store pT value of Electron
                    if(Electron_pfRelIso03_all[0]<0.15) hRiee->Fill(Electron_pt[0]);
                }
                if (nMuon>0){ //Muon
                    hRem->Fill(Muon_pt[0]);
                    if(Muon_pfRelIso03_all[0]<0.15) hRiem->Fill(Muon_pt[0]);
                }
                
                //Electron_pfIsoall compreso fra 0 e 1. 0 è completamente isolato. Devo chiedere che sia minore di 0.15
                //if(Electron_pfRelIso03_all[0]<0.15)
                break;
            case 13: //if muon
                //see above, the structure is the same
                /*
                h4->Fill(LHEPart_pt[ilept]);
                h5->Fill(LHEPart_phi[ilept]);
                h6->Fill(LHEPart_eta[ilept]);
                
                pt.muon=LHEPart_pt[ilept];
                phi.muon=LHEPart_phi[ilept];
                eta.muon=LHEPart_eta[ilept];
                
                top->Fill();
                */
                if(nElectron>0){
                    //if(Electron_pfRelIso03_all[0]<0.15)//if there is at least one reconstructed Electron
                    hRme->Fill(Electron_pt[0]); //store pT value of Electron
                    if(Electron_pfRelIso03_all[0]<0.15) hRime->Fill(Electron_pt[0]);
                }
                if (nMuon>0){ //Muon
                    hRmm->Fill(Muon_pt[0]);
                    if(Muon_pfRelIso03_all[0]<0.15) hRimm->Fill(Muon_pt[0]);
                }
                
                break;
            case 15: //if tau
                //see above, the structure is the same
                
                /*
                h7->Fill(LHEPart_pt[ilept]);
                h8->Fill(LHEPart_phi[ilept]);
                h9->Fill(LHEPart_eta[ilept]);
                
                
                pt.tau=LHEPart_pt[ilept];
                phi.tau=LHEPart_phi[ilept];
                eta.tau=LHEPart_eta[ilept];
                
                top->Fill();
                */
                if(nElectron>0){
                    //if(Electron_pfRelIso03_all[0]<0.15)//if there is at least one reconstructed Electron
                    hRte->Fill(Electron_pt[0]); //store pT value of Electron
                    if(Electron_pfRelIso03_all[0]<0.15) hRite->Fill(Electron_pt[0]);
                }
                if (nMuon>0){ //Muon
                    hRtm->Fill(Muon_pt[0]);
                    if(Muon_pfRelIso03_all[0]<0.15) hRitm->Fill(Muon_pt[0]);
                }
                break;
            default:
                break;
                
                
        }
        
        
        //se nElectron (elettrone ricostruito)>0 salvo Electron_xy[0] pT, eta, phi solo per il primo
        //salvo il pt diviso per lept_type
        //tre istogrammi, electron_pT se c'è elettrone, e_pt nel decadimento muone, e_pt caso tau
        //e le stacko
        //se l'elettrone è ricostruito corettamente, ha un pt elevato perché decaduto dal W (che viene dal top), gli e da tau hanno energia più bassa
        //
        //isolamento e identificazione dopo
        //
        //uguale per nMuon
        nev++;

        
    }

    
    
    //TO DO: Add histogram to file OK
    
    

    
    
    //Writing and Stacking all the histograms

    /*
    h1->Write();
    h2->Write();
    h3->Write();
    h4->Write();
    h5->Write();
    h6->Write();
    h7->Write();
    h8->Write();
    h9->Write();
    
    hRee->Write();
    hRem->Write();
    hRme->Write();
    hRmm->Write();
    hRte->Write();
    hRtm->Write();
    
    hRiee->Write();
    hRiem->Write();
    hRime->Write();
    hRimm->Write();
    hRite->Write();
    hRitm->Write();
    */
    
    
    //h1->SetFillColor(38); //paint it black
    //h2->SetFillColor(38); //ELECTRON BLUE
    //h3->SetFillColor(38);
    
    hRee->SetFillColor(38);
    hRme->SetFillColor(38);
    hRte->SetFillColor(38);
    hRiee->SetFillColor(38);
    hRime->SetFillColor(38);
    hRite->SetFillColor(38);
    
    
    //h4->SetFillColor(46); //MUON RED
    //h5->SetFillColor(46);
    //h6->SetFillColor(46);
    
    
    hRem->SetFillColor(46);
    hRmm->SetFillColor(46);
    hRiem->SetFillColor(46);
    hRimm->SetFillColor(46);
    hRtm->SetFillColor(46);
    hRitm->SetFillColor(46);
    
    
    //h7->SetFillColor(30); //TAU GREEN
    //h8->SetFillColor(30);
    //h9->SetFillColor(30);
    

    
    /*
    hs1->Add(h1);
    hs2->Add(h2);
    hs3->Add(h3);
    hs1->Add(h4);
    hs2->Add(h5);
    hs3->Add(h6);
    hs1->Add(h7);
    hs2->Add(h8);
    hs3->Add(h9);
    */
    
    hs4->Add(hRee);
    hs4->Add(hRem);
    
    hs5->Add(hRmm);
    hs5->Add(hRme);
    
    hs6->Add(hRte);
    hs6->Add(hRtm);
    
    hs7->Add(hRiee);
    hs7->Add(hRiem);
    
    hs8->Add(hRimm);
    hs8->Add(hRime);
    
    hs9->Add(hRite);
    hs9->Add(hRitm);
    
    /*
    hs1->Write();
    hs2->Write();
    hs3->Write();
    hs4->Write();
    hs5->Write();
    */
    
    TCanvas *c1 = new TCanvas("c1","Electron pT");
    c1->Divide(3,2);
    c1->cd(1);
    hs4->Draw();
    c1->cd(2);
    hRee->Draw();
    
    c1->cd(3);
    hRem->Draw();
    
    c1->cd(4);
    hs7->Draw();
    c1->cd(5);
    hRiee->Draw();
    
    c1->cd(6);
    hRiem->Draw();
    

    
    
    TCanvas *c2 = new TCanvas("c2","Muon pT");
    c2->Divide(3,2);
    c2->cd(1);
    hs5->Draw();
    c2->cd(2);
    hRme->Draw();
    
    c2->cd(3);
    hRmm->Draw();

    c2->cd(4);
    hs8->Draw();
    c2->cd(5);
    hRime->Draw();
    
    c2->cd(6);
    hRimm->Draw();
    
    TCanvas *c3 = new TCanvas("c3","Tau pT");
    c3->Divide(3,2);
    c3->cd(1);
    hs6->Draw();
    c3->cd(2);
    hRte->Draw();
    c3->cd(3);
    hRtm->Draw();
    
    c3->cd(4);
    hs9->Draw();
    c3->cd(5);
    hRite->Draw();
    
    c3->cd(6);
    hRitm->Draw();
    
    
    
    

    
    /*
     
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
    */
    
    //Write and close file
    //output->Write();
    //output->Close();
    
    cout <<" TOTALE EVENTI " <<nev<<endl;
    cout << " Numero muoni " <<nmu<< " | n_mu/n_tau="<< (double) nmu/ntau<<endl;
    cout << " Numero elettroni " <<nele<< " | n_ele/n_tau="<< (double) nele/ntau<<endl;
    cout << " Numero tau " <<ntau<< " | 2*n_tau/(n_ele+n_mu)="<< (double) 2*ntau/(nele+nmu)<<endl;
}




//TO DO:
// -Sistemare e ordinare il codice con funzioni
// -Creare la tree con i dati che mi servono
// -Concatenare due file per leggere sia T che TBar
//

