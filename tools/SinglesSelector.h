//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar  1 17:09:32 2021 by ROOT version 6.22/06
// from TTree DecayTree/DecayTree
// found on file: /afs/cern.ch/user/k/kklimasz/workspace/data/Bs2JPsieePhi/RD_Fits/Bootstrap/Result_DmsPDG_TimeResConstr/FullBootstrap_CorrectFit_NewTimeAcc_GsDGsFreeNoConstr_DmsPDG/input/fitNTuple_Bs2JpsieePhiStrip21_TupleBsDetached_RDfull_MassSigmaStd_BDT1_NewSel_probnnp_2CB_BkgExp_PartBkgGood_ConstrFloat_MCS21_NewSSK_bkgcat0_MM_BremAll.root
//////////////////////////////////////////////////////////

#ifndef SinglesSelector_h
#define SinglesSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TTreeIndex.h>

// Headers needed by this particular selector


class SinglesSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
   TFile          *fOutFile = 0;
   TTree          *fClone = 0;
   Double_t       run = -1;
   Double_t       event = -1;
   TTreeIndex*    index = 0;

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Double_t> sigWeight_2KKBins = {fReader, "sigWeight_2KKBins"};
   TTreeReaderValue<Int_t> runPeriod = {fReader, "runPeriod"};
   TTreeReaderValue<Double_t> eventNumber = {fReader, "eventNumber"};
   TTreeReaderValue<Double_t> runNumber = {fReader, "runNumber"};
   TTreeReaderValue<Double_t> mass = {fReader, "mass"};
   TTreeReaderValue<Double_t> mass_std = {fReader, "mass_std"};
   TTreeReaderValue<Double_t> mdau2 = {fReader, "mdau2"};
   TTreeReaderValue<Double_t> mdau1 = {fReader, "mdau1"};
   TTreeReaderValue<Double_t> sigmam = {fReader, "sigmam"};
   TTreeReaderValue<Double_t> sigmam_std = {fReader, "sigmam_std"};
   TTreeReaderValue<Double_t> sigmamdau2 = {fReader, "sigmamdau2"};
   TTreeReaderValue<Double_t> sigmadau1 = {fReader, "sigmadau1"};
   TTreeReaderValue<Double_t> time = {fReader, "time"};
   TTreeReaderValue<Double_t> sigmat = {fReader, "sigmat"};
   TTreeReaderValue<Double_t> helcosthetaK = {fReader, "helcosthetaK"};
   TTreeReaderValue<Double_t> helcosthetaL = {fReader, "helcosthetaL"};
   TTreeReaderValue<Double_t> helphi = {fReader, "helphi"};
   TTreeReaderValue<Double_t> trcospsi = {fReader, "trcospsi"};
   TTreeReaderValue<Double_t> trcostheta = {fReader, "trcostheta"};
   TTreeReaderValue<Double_t> trphi = {fReader, "trphi"};
   TTreeReaderValue<Double_t> tagomega_os_cb = {fReader, "tagomega_os_cb"};
   TTreeReaderValue<Int_t> tagdecision_os_cb = {fReader, "tagdecision_os_cb"};
   TTreeReaderValue<Double_t> tagomega_ss_nn = {fReader, "tagomega_ss_nn"};
   TTreeReaderValue<Int_t> tagdecision_ss_nn = {fReader, "tagdecision_ss_nn"};
   TTreeReaderValue<Double_t> tagomega_os_cb_calib = {fReader, "tagomega_os_cb_calib"};
   TTreeReaderValue<Int_t> tagdecision_os_cb_calib = {fReader, "tagdecision_os_cb_calib"};
   TTreeReaderValue<Double_t> tagomega_ss_nn_calib = {fReader, "tagomega_ss_nn_calib"};
   TTreeReaderValue<Int_t> tagdecision_ss_nn_calib = {fReader, "tagdecision_ss_nn_calib"};
   TTreeReaderValue<Int_t> sel = {fReader, "sel"};
   TTreeReaderValue<Int_t> hlt1_biased = {fReader, "hlt1_biased"};
   TTreeReaderValue<Int_t> hlt2_biased = {fReader, "hlt2_biased"};
   TTreeReaderValue<Int_t> l0_unbiased = {fReader, "l0_unbiased"};
   TTreeReaderValue<Int_t> pid = {fReader, "pid"};
   TTreeReaderValue<Double_t> B_P = {fReader, "B_P"};
   TTreeReaderValue<Double_t> Kplus_P = {fReader, "Kplus_P"};
   TTreeReaderValue<Double_t> Kplus_PT = {fReader, "Kplus_PT"};
   TTreeReaderValue<Double_t> Kminus_P = {fReader, "Kminus_P"};
   TTreeReaderValue<Double_t> Kminus_PT = {fReader, "Kminus_PT"};
   TTreeReaderValue<Double_t> phi_p = {fReader, "phi_p"};
   TTreeReaderValue<Double_t> B_Pt = {fReader, "B_Pt"};
   TTreeReaderValue<Double_t> B_eta = {fReader, "B_eta"};
   TTreeReaderValue<Double_t> B_phi = {fReader, "B_phi"};
   TTreeReaderValue<Double_t> nPV = {fReader, "nPV"};
   TTreeReaderValue<Double_t> nTracks = {fReader, "nTracks"};
   TTreeReaderValue<Double_t> B_ip_ownpv = {fReader, "B_ip_ownpv"};
   TTreeReaderValue<Double_t> B_fdchi2_ownpv = {fReader, "B_fdchi2_ownpv"};
   TTreeReaderValue<Double_t> Bs_endvertex_chi2 = {fReader, "Bs_endvertex_chi2"};
   TTreeReaderValue<Double_t> B_dira_ownpv = {fReader, "B_dira_ownpv"};
   TTreeReaderValue<Double_t> phi_pt = {fReader, "phi_pt"};
   TTreeReaderValue<Double_t> jpsi_pt = {fReader, "jpsi_pt"};
   TTreeReaderValue<Double_t> mass_chi2 = {fReader, "mass_chi2"};
   TTreeReaderValue<Double_t> eplus_pide = {fReader, "eplus_pide"};
   TTreeReaderValue<Double_t> eminus_pide = {fReader, "eminus_pide"};
   TTreeReaderValue<Double_t> eplus_probnne = {fReader, "eplus_probnne"};
   TTreeReaderValue<Double_t> eminus_probnne = {fReader, "eminus_probnne"};
   TTreeReaderValue<Double_t> Kplus_pidK = {fReader, "Kplus_pidK"};
   TTreeReaderValue<Double_t> Kminus_pidK = {fReader, "Kminus_pidK"};
   TTreeReaderValue<Double_t> Kplus_pidp = {fReader, "Kplus_pidp"};
   TTreeReaderValue<Double_t> Kminus_pidp = {fReader, "Kminus_pidp"};
   TTreeReaderValue<Double_t> Kplus_probnnk = {fReader, "Kplus_probnnk"};
   TTreeReaderValue<Double_t> Kminus_probnnk = {fReader, "Kminus_probnnk"};
   TTreeReaderValue<Double_t> Kplus_probnnp = {fReader, "Kplus_probnnp"};
   TTreeReaderValue<Double_t> Kminus_probnnp = {fReader, "Kminus_probnnp"};
   TTreeReaderValue<Double_t> Kplus_probnnpi = {fReader, "Kplus_probnnpi"};
   TTreeReaderValue<Double_t> Kminus_probnnpi = {fReader, "Kminus_probnnpi"};
   TTreeReaderValue<Double_t> eplus_bremmult = {fReader, "eplus_bremmult"};
   TTreeReaderValue<Double_t> eminus_bremmult = {fReader, "eminus_bremmult"};
   TTreeReaderValue<Double_t> eplus_hasbremadd = {fReader, "eplus_hasbremadd"};
   TTreeReaderValue<Double_t> eminus_hasbremadd = {fReader, "eminus_hasbremadd"};
   TTreeReaderValue<Double_t> BDT_response_NewSel_MCS21corr = {fReader, "BDT_response_NewSel_MCS21corr"};
   TTreeReaderValue<Int_t> polarity = {fReader, "polarity"};
   TTreeReaderValue<Double_t> N_cbkgMass_sw = {fReader, "N_cbkgMass_sw"};
   TTreeReaderValue<Double_t> N_cbkg23Mass_sw = {fReader, "N_cbkg23Mass_sw"};


   SinglesSelector(TTree * /*tree*/ =0) { }
   virtual ~SinglesSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
       Int_t result =  fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
       cout<<"GetEntry: "<<entry<<", "<<fChain<<", "<<result<<endl;
        return result;

       Long64_t local = fChain->GetTree()->LoadTree( index->GetIndex()[entry] );
       cout<<"GetEntry: "<<entry<<", "<<local<<endl;
       fChain->GetTree()->GetEntry(local, getall);
   }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(SinglesSelector,0);

};

#endif

#ifdef SinglesSelector_cxx
void SinglesSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   tree->GetTree()->BuildIndex("runNumber","eventNumber");
   index = (TTreeIndex*)tree->GetTree()->GetTreeIndex();
   fReader.SetTree(tree);
}

Bool_t SinglesSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef SinglesSelector_cxx
