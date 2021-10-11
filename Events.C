#define Events_cxx
#include "Events.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>





Int_t Events::TypeIndex()
{
    for (int i=0; i<=nLHEPart; i++) { //loop over the particles
        
        if(abs(LHEPart_pdgId[i])==11||abs(LHEPart_pdgId[i])==13||abs(LHEPart_pdgId[i])==15)  return i;
    }
    return -1;
}


    /*
    
Ora sul tau faccio un istogramam a tre colori, in cui faccio pedere lo spettro degli elettroni ricostruiti isolati, sommando separatamente elettrone, muone, tau
     il colore deve essere l'origine dell'evento
     il pt degli elettroni, muoni isolati
     
     Dopo guardiamo come affinare l'identificazione
     
    */
    
    
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
     
     
     TTree* top = new TTree("top", "top"); //initailize a Tree
    
    top->Branch("pT",&pt,"electron:muon:tau"); //divide the branch pT in three leaf, one for every particle
    top->Branch("phi",&phi,"electron:muon:tau"); //same as above
    top->Branch("eta",&eta,"electron:muon:tau"); //''
    //top->Branch("electron pT",&hept,"electronpT");
    */
    //TO FURTHER IMPLEMENT: IN THE SAME FILE CREATE A TREE FOR THE TBAR FILE

void Events::Filling(Int_t ind, Int_t ilept)
{
    ((h[ind]))->Fill(LHEPart_pt[ilept]); //fill histogram for pt
    if(nElectron>0){ //if there is at least one reconstructed Electron
        ((hR[2*ind]))->Fill(Electron_pt[0]); //store pT value of Electron
        if(Electron_pfRelIso03_all[0]<0.15){
            ((hRi[2*ind]))->Fill(Electron_pt[0]);
            RctIsoPt[2*ind]=Electron_pt[0];
            top->Fill();
        }
    }
    if (nMuon>0){ //Muon
        ((hR[2*ind+1]))->Fill(Muon_pt[0]);
        if(Muon_pfRelIso03_all[0]<0.15){
            ((hRi[2*ind+1]))->Fill(Muon_pt[0]);
            RctIsoPt[2*ind+1]=Muon_pt[0];
            top->Fill();
        }
    }
    
    return;
}


void Events::ReconStack()

//COLOR THE HISTO GIVEN THE PARTICLE
{
    //COLOR THE HISTOGRAM
    for (int i=0; i<5;  i+=2) { //Electron Blue
        ((hR[i]))->SetFillColor(38);
        ((hRi[i]))->SetFillColor(38);
    }
    ((h[0]))->SetFillColor(38);
    
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


void Events::IsoStack()
{
    TCanvas *d[2];
    
    d[0] = new TCanvas("c3","Electron pT stacked");
    d[1] = new TCanvas("c4","Muon pT stacked");
    
    
    //RECOLOR, TO FIX LAteR
    ((hRi[0]))->SetFillColor(38);
    ((hRi[1]))->SetFillColor(38);
    
    ((hRi[2]))->SetFillColor(46);
    ((hRi[3]))->SetFillColor(46);
    
    ((hRi[4]))->SetFillColor(30);
    ((hRi[5]))->SetFillColor(30);
    
    //REFILL
    
    for (int j=0; j<5; j+=2) { // RECONSTROCTUD STACKING HS[1,2,3]
        ((hs[7]))->Add((hRi[j]));
        ((hs[8]))->Add((hRi[j+1]));
    }
    
    for (int i=0, j=0; i<2; i++,j++) {
        (d[i])->Divide(2,2);
        (d[i])->cd(1);
        (hRi[j])->Draw();
        (d[i])->cd(2);
        (hRi[j+2])->Draw();
        (d[i])->cd(3);
        (hRi[j+4])->Draw();
        
        (d[i])->cd(4);
        (hs[j+7])->Draw();
    }
    
    //Plot Electron and Muon stacked isolated pt
    //da riscrivere ammodino
    
    
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
    top->Write();
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
    int provamu=0; //debug flag
    int nele=0; //number of electrons
    int ntau=0; // number of taus
    Long64_t nbytes = 0, nb = 0;
    
    //nentries=100;
    
    for (Long64_t jentry=0; jentry<nentries;jentry++) { //loop over entries
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;

        int ilept=TypeIndex();
        if(ilept<0) break;

        int lept_type=0;
        lept_type=abs(LHEPart_pdgId[ilept]); // lepton pdgID type
        
        
        switch (lept_type) {
            case 11: //if electron
                /*
                 
                h[0]->Fill(LHEPart_pt[ilept]); //fill histogram for pt
                h2->Fill(LHEPart_phi[ilept]); //same for phi
                h3->Fill(LHEPart_eta[ilept]); //same for eta
                
                pt.electron=LHEPart_pt[ilept]; //insert pt variable in the structure to further insert the tree
                phi.electron=LHEPart_phi[ilept];//phi for tree
                eta.electron=LHEPart_eta[ilept];//eta for tree
                
                top->Fill(); //fill top branch
                */
                
                nele++;
                Filling(0, ilept);

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
                
                nmu++;
                Filling(1, ilept);
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
                
                ntau++;
                Filling(2, ilept);
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

    ReconStack();


    IsoStack();

    //Write and close file

    WriteToFile();
    output->Write();
    output->Close();
    
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

