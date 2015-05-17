#ifndef INC_EVENT_H
#define INC_EVENT_H

#include "TObject.h"

class IncEvent : public TObject
{
    public:
        IncEvent();
        IncEvent(int hnmax);
        virtual ~IncEvent();

        // event information =====================================================
        Int_t    eRun;       // run number
        Int_t    eSpill;     // spill number
        Int_t    eEvInSpill; // event number in spill
        Long64_t eEvNr;      // unique event number
        Int_t    eTrigMask;  // trigger mask
        Int_t    eSpinConf;  // 0 - u, 1 - d, 2 - u', 3 - d'
        Float_t  ePt;        // Target polarisation
        Float_t  ePb;        // Beam polarisation

        // vertex ================================================================
        Float_t vX; // X coordinate of vertex
        Float_t vY; // Y coordinate of vertex
        Float_t vZ; // Z coordinate of vertex
        Int_t   vN; // Number of tracks in vertex
        Int_t   vCell; // Target Cell (0,1,2)

        // kinematics ============================================================
        Float_t kD;         // Depolarization factor
        Float_t kF;         // Dilution factor
        Double_t kQ2;        // mass of virtual photon
        Double_t kXbj;       // bjorken variable
        Double_t kY;         // mu relative energy loss  
        Float_t kRadWeightInc; // Weight of the event from Radiative Corrections (data: etha, MC: 1/etha, etha=sigma_born/sigma_meas)
        Float_t kRadWeightSem; // Weight of the event from Radiative Corrections (data: etha, MC: 1/etha, etha=sigma_born/sigma_meas)
        Float_t kPMiss;      // missing momentum
        Float_t kPtMiss;     // missing transverse momentum (wrt gamma)
        Float_t kPlMiss;     // missing longt momentum (wrt gamma)

        // mu,mup, virtual photon ================================================
        Float_t bP; // Lorentz vector - beam
        Float_t mP; // Lorentz vector - scattered mu
        Float_t mTheta;
        Float_t mPhi;

        // hadrons ===============================================================
        Int_t     hNmax; // Maximum number of hadrons to store
        Int_t     hN;    // Actual number of hadrons
        Float_t   hMinv01; // Invariant mass pseudo pi0 + leading hadron
        Float_t   hMinv02; // Invariant mass pseudo pi0 + subleading hadron
        Float_t   hMinv12; // Invariant mass hadron pair
        Int_t* hQ;             //[hNmax] Charge of hadrons
        Int_t* hNhits;         //[hNmax] Hits associated to hadron track
        Float_t* hP;           //[hNmax] Hadron momentum
        Float_t* hPl;          //[hNmax] Hadron momentum longitudinal component
        Float_t* hPt;          //[hNmax] Hadron momentum transverse component
        Float_t* hZ;           //[hNmax] Z variable of a hadron
        Float_t* hXf;          //[hNmax] Xf variable of a hadron
        Float_t* hZlast;       //[hNmax] Z position of the last hit
        Float_t* hXX0;         //[hNmax] Number of radiation lengths X/X0 passed

        // NeuralNetworks
        Float_t nnXg;
        Float_t nnXc;
        Float_t nnAllg;
        Float_t nnAllc;
        Float_t nnRpgf;
        Float_t nnRqcdc;
        Float_t nnRlp;

        void virtual Clear(Option_t* option="");
        void virtual ResetHadrons(Option_t* option="");

    protected:
        void virtual Init(Option_t* option="");
        void virtual ClearHadrons(Option_t* option="");

        ClassDef(IncEvent,12) //Inclusive event
};

#endif

