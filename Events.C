#define Events_cxx
#include "Events.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>



//FUNCTION THAT RETURNS THE INDEX OF THE PARTICLE; IF THAT PARTICLE IS A LEPTON

Int_t Events::TypeIndex()
{
    for (int i=0; i<=nLHEPart; i++) { //loop over the particles
        
        if(abs(LHEPart_pdgId[i])==11||abs(LHEPart_pdgId[i])==13||abs(LHEPart_pdgId[i])==15)  return i; //if a particle is a lepton, it returns the index
    }
    return -1; //else return -1
}


void Events::Filling(Int_t ind, Int_t ilept)
{
    
    ((h[ind]))->Fill(LHEPart_pt[ilept]); //fill histogram for pt
    
    if(nElectron>0){ //if there is at least one reconstructed Electron
        ((hR[2*ind]))->Fill(Electron_pt[0]); //store pT value of Electron
        
        
        if(Electron_pfRelIso03_all[0]<0.15){ //cuts on the cone, i want an isolated particle
            ((hRi[2*ind]))->Fill(Electron_pt[0]); //fill histogram
            
            RctIsoPt[2*ind]=Electron_pt[0]; //store value for tree
            top->Fill(); //fill tree
        }
    }
    if (nMuon>0){ //Muon, structure similar as above
        ((hR[2*ind+1]))->Fill(Muon_pt[0]);
        if(Muon_pfRelIso03_all[0]<0.15){
            ((hRi[2*ind+1]))->Fill(Muon_pt[0]);
            RctIsoPt[2*ind+1]=Muon_pt[0];
            top->Fill();
        }
    }
    
    return;
}

//generate three plots, one for lept_type (event type), containing the Reconstructed (and isolated) Electron and Muon pT, and the two stacked together


//generate one additional plot, pT of particle (divided by event type) without cuts stacked together

void Events::ReconStack()

//COLOR THE HISTO GIVEN THE PARTICLE
{
    //COLOR THE HISTOGRAM
    for (int i=0; i<5;  i+=2) { //Electron Blue
        ((hR[i]))->SetFillColor(38);
        ((hRi[i]))->SetFillColor(38);
    }
    ((h[0]))->SetFillColor(38); //without cuts
    
    for (int i=1; i<6;  i+=2) { //Muon Red
        ((hR[i]))->SetFillColor(46);
        ((hRi[i]))->SetFillColor(46);
    }
    (h[1])->SetFillColor(46);
    
    (h[2])->SetFillColor(30); //Tau Green


    
    
 //STACK ALL THE HISTO

    //STACK ALL THE HISTOGRAMS
    for (int m=0; m<3; m++) { //PT STACKING IN HS[0]
        (hs[0])->Add(h[m]);
    }
    
    for (int i=1,j=0; i<4; i++,j+=2) { // RECONSTROCTUD STACKING HS[1,2,3]
        ((hs[i]))->Add((hR[j]));
        ((hs[i]))->Add((hR[j+1]));
        ((hs[i+3]))->Add((hRi[j])); //AND ISO REC HS[4,5,6]
        ((hs[i+3]))->Add((hRi[j+1]));
    }
    
// DRAW THE RECON HISTOGRAM
    
    TCanvas *c[4];
    
    c[0] = new TCanvas("c0","pT stacked, w/o cuts");
    c[1] = new TCanvas("c1","Electron event pT");
    c[2] = new TCanvas("c2","Muon event pT");
    c[3] = new TCanvas("c3","Tau event pT");



    //DRAW JUST THE PT STACKED, WITHOUT ANY CUTS
        (c[0])->Divide(2,2);
    
    for (int i=0; i<3; i++) {
        c[0]->cd(i+1);
        h[i]->Draw();
    }
        
        (c[0])->cd(4);
        (hs[0])->Draw();

    
    
    //Draw stacked histograms iso and rec particles
    
    for (int i=1,j=0; i<4; i++,j+=2) {
        ((c[i]))->Divide(3,2);
        ((c[i]))->cd(1);
        ((hs[i]))->Draw();
        ((c[i]))->cd(2);
        ((hR[j]))->Draw();
        
        ((c[i]))->cd(3);
        ((hR[j+1]))->Draw();
        
        ((c[i]))->cd(4);
        ((hs[i+3]))->Draw();
        
        ((c[i]))->cd(5);
        ((hRi[j]))->Draw();
        
        ((c[i]))->cd(6);
        ((hRi[j+1]))->Draw();


    }

    return;
}

//Draw two histograms. One for reconstructed isolated Electron pT, stacked by event type (lept_type), one for reconstructed muon pT, stacked by event type (lept type)

void Events::IsoStack()
{
    
    //RECOLOR THE HISTOGRAM BY LEPT_TYPE
    ((hRi[0]))->SetFillColor(38); //electron blue
    ((hRi[1]))->SetFillColor(38);
    
    ((hRi[2]))->SetFillColor(46); //muon red
    ((hRi[3]))->SetFillColor(46);
    
    ((hRi[4]))->SetFillColor(30);  //tau red
    ((hRi[5]))->SetFillColor(30);
    
    //REFILL
    
    // RECONSTRUCTED ISOLATED STACKING OVER LEPT_TYPE, ONE FOR ELECTRON PT, THE OTHER FOR MUON PT
    for (int j=0; j<5; j+=2) {
        ((hs[7]))->Add((hRi[j]));
        ((hs[8]))->Add((hRi[j+1]));
    }
    
    
    //PLOT IN TWO CANVAS
    
    TCanvas *d[2];
    
    d[0] = new TCanvas("c3","Electron pT stacked");
    d[1] = new TCanvas("c4","Muon pT stacked");
    
    for (int i=0, j=0; i<2; i++,j++) {
        (d[i])->Divide(2,2);
        (d[i])->cd(1);
        (hRi[j])->Draw(); //up left ele event
        (d[i])->cd(2);
        (hRi[j+2])->Draw(); //up right muon event
        (d[i])->cd(3);
        (hRi[j+4])->Draw(); //down left tau event
        
        (d[i])->cd(4);
        (hs[j+7])->Draw(); //down right stacked
    }
    

    
}

void Events::WriteToFile() //Write to File function
{
    for (int i=0; i<3; i++) { //pt
        ((h[i]))->Write();
    }
    
    for (int i=0; i<6; i++) { //rec prtc
        ((hR[i]))->Write();
    }
    
    for (int i=0; i<6; i++) { //rec iso prtc
        ((hRi[i]))->Write();
    }
    
    for (int i=0; i<9; i++) { //stacked histo
        ((hs[i]))->Write();
    }
    top->Write(); //write tree
    
    output->Write(); //write file
    output->Close();
    return;
}




void Events::Loop() //main fucntion, actually
{

    
    
    if (fChain == 0) return;
    
    Long64_t nentries = fChain->GetEntriesFast();
    

    
    int ilept=-1; // lepton index in LHEPart collection
    
    //VarDef();

    int nev=0; // number of events
    int nmu=0; // number of muons
    int nele=0; //number of electrons
    int ntau=0; // number of taus
    Long64_t nbytes = 0, nb = 0;
    
    //nentries=100;
    
    for (Long64_t jentry=0; jentry<nentries;jentry++) { //loop over entries
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;

        int ilept=TypeIndex(); //Get index of particle, if lepton
        if(ilept<0) break; //break loop if not lepton

        int lept_type=0;
        lept_type=abs(LHEPart_pdgId[ilept]); // lepton pdgID type
        
        
        switch (lept_type) {
            case 11: //if electron

                nele++; //increment electron counter
                Filling(0, ilept); //fill histograms and tree
                break;
                
            case 13: //if muon
                
                nmu++;
                Filling(1, ilept);
                break;
                
            case 15: //if tau

                ntau++;
                Filling(2, ilept);
                break;
                
            default:
                
                break;
                
                
        }
        

        nev++; //increment lepton type events counter
        
    }

    ReconStack(); //do the first


    IsoStack();


    WriteToFile();

    
    cout <<" TOTALE EVENTI " <<nev<<endl;
    cout << " Numero muoni " <<nmu<< " | n_mu/n_tau="<< (double) nmu/ntau<<endl;
    cout << " Numero elettroni " <<nele<< " | n_ele/n_tau="<< (double) nele/ntau<<endl;
    cout << " Numero tau " <<ntau<< " | 2*n_tau/(n_ele+n_mu)="<< (double) 2*ntau/(nele+nmu)<<endl;
}




//TO DO:

// -Concatenare due file per leggere sia T che TBar
//

