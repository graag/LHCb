#ifndef HPT_EVENT_OLD_H
#define HPT_EVENT_OLD_H
//#define HPT_DEBUG
//#define HPT_OLD_TREE 

#include "TObject.h"
#include "TLorentzVector.h"

class HptEventOld : public TObject
{
	public:
		HptEventOld();
		HptEventOld(Int_t hnmax);
		~HptEventOld();

		void virtual Clear(Option_t* option="");

		// event information =====================================================
		Int_t       eRun;     // run number
		Int_t       eSpill;   // spill number
		Int_t       eEvinspill;   // event number in spill
		Long64_t eEvNr;	// Unique event number (for MC - event nr from lepto)
		Int_t       eTrig;    // trigger mask
		Int_t       eTrigAlt;    // corrected trigger mask
		Float_t  eImu;   // incident muon Int_tensity.
		Float_t  ePolu;   // polar upstream
		Float_t  ePold;   // polar downstream 
		Float_t  eD;      // depolarization factor
		Float_t  eNN;    //Neural Network output
		Float_t   eRadWeight;
		Float_t   eRadWeightInc;
		Float_t   eRadWeightSemInc;

		// vertex ================================================================
		Float_t vX;    // X coordinate of vertex
		Float_t vY;    // Y coordinate of vertex
		Float_t vZ;    // Z coordinate of vertex
		Int_t   vN;    // Number of tracks in vertex
		Float_t vChi2; // Chi2 of primary vertex

		// kinematics ============================================================
		Float_t kEgamma;
		Double_t kQ2;     // mass of virtual photon
		Double_t kXbj;    // bjorken variable
		Double_t kY;      // mu relative energy loss  
		Float_t kW2;     // invariant mass of photon nucleon system

		// mu,mup, virtual photon ================================================
		Float_t        bP;
		Float_t        bPx;
		Float_t        bPy;
		Float_t        bTheta;
		Float_t        bPhi;
		Float_t        bE;
		Float_t        mP;
		Float_t        mPx;
		Float_t        mPy;
		Float_t        mTheta;
		Float_t        mPhi;
		Float_t        mE;
		Float_t        mZlast;
#ifdef HPT_OLD_TREE
		Float_t        mNhits;
#else
		Int_t           mNhits;
#endif
		Float_t        mXX0;
		Int_t           mPid;

		// hadrons ===============================================================
		Int_t hN;          // number of hadrons
		Int_t hNmax;       // number of hadrons to store
		Float_t hMinv; // Invariant mass of two hadrons
		Float_t*        hChi2; //[hNmax]
		Float_t*        hQ; //[hNmax]
		Float_t*        hPhi; //[hNmax]
		Float_t*        hPt; //[hNmax]
		Float_t*        hPl; //[hNmax]
		Float_t*        hZ; //[hNmax]
		Float_t*        hP; //[hNmax]
		Float_t*        hPx; //[hNmax]
		Float_t*        hPy; //[hNmax]
		Float_t*        hPz; //[hNmax]
		Float_t*        hE; //[hNmax]
		Float_t*        hTheta; //[hNmax]
		Float_t*        hXf; //[hNmax]
		Float_t*        hZlast; //[hNmax]
		Float_t*        hNhits; //[hNmax]
		Float_t*        hXX0; //[hNmax]
		Float_t*        hEc; //[hNmax]
		Float_t*        hHc1; //[hNmax]
		Float_t*        hHc2; //[hNmax]
		Float_t*        hEc1; //[hNmax]
		Int_t*        hPid; //[hNmax]

#ifdef HPT_DEBUG
		Int_t eMW1reco;
		Float_t eSM1end;
		Float_t eSM2end;
		Int_t mYokeSM2;
		Float_t        mZfirst;
		Float_t        mTime;
		Float_t        mChi2;

		Int_t mNhitsMA01;
		Int_t mNhitsMA02;
		Int_t mNhitsPA;
		Int_t mNhitsPB;
		Int_t mNhitsMB;
		Int_t    mNhitsHO03Y1;   // mu' hits in HO03
		Int_t    mNhitsHO04Y1;   // mu' hits in HO04Y1
		Int_t    mNhitsHO04Y2;   // mu' hits in HO04Y2

		Int_t mHitMapSize;
#ifdef HPT_OLD_TREE
		Int_t* mHitMap;// [mHitMapSize]
#else
		UInt_t* mHitMap;// [mHitMapSize]
#endif

		Int_t    mHO03Y1;   // mu' poInt_ts to HO03
		Int_t    mHO04Y1;   // mu' poInt_ts to HO04Y1
		Int_t    mHO04Y2;   // mu' hits in HO04Y2

		Float_t mHO03Y1_X; // mu' X pos in HO03
		Float_t mHO03Y1_Y; // mu' Y pos in HO03
		Float_t mHO04Y1_X; // mu' X pos in HO04Y1
		Float_t mHO04Y1_Y; // mu' Y pos in HO04Y1
		Float_t mHO04Y2_X; // mu' X pos in HO04Y2
		Float_t mHO04Y2_Y; // mu' Y pos in HO04Y2
		Float_t mMA02Y1_X; // mu' X pos in MA02Y1
		Float_t mMA02Y1_Y; // mu' Y pos in MA02Y1
		Float_t mSM1_X;
		Float_t mSM1_Y;
		Float_t mSM1_dX;
		Float_t mSM1_dY;
		Float_t mSM2_X;
		Float_t mSM2_Y;
		Float_t mSM2_dX;
		Float_t mSM2_dY;
		Float_t mLast_X;
		Float_t mLast_Y;
		Float_t mLast_dX;
		Float_t mLast_dY;

		Float_t eCaloMax_E_HC1;  // maximum energy deposit in HCAL1
		Float_t eCaloMax_E_HC2;  // maximum energy deposit in HCAL2
		Float_t eCaloMax_E_EC1;  // maximum energy deposit in ECAL1
		Float_t eCaloMax_E_EC2;  // maximum energy deposit in ECAL2
		Float_t eCaloMax_P_HC1;  // maximum energy deposit in HCAL1
		Float_t eCaloMax_P_HC2;  // maximum energy deposit in HCAL2
		Float_t eCaloMax_P_EC1;  // maximum energy deposit in ECAL1
		Float_t eCaloMax_P_EC2;  // maximum energy deposit in ECAL2

		Float_t*        hTime; //[hNmax]
		Float_t*        hSM1_X; //[hNmax]
		Float_t*        hSM1_Y; //[hNmax]
		Float_t*        hSM1_dX; //[hNmax]
		Float_t*        hSM1_dY; //[hNmax]
		Float_t*        hSM2_X; //[hNmax]
		Float_t*        hSM2_Y; //[hNmax]
		Float_t*        hSM2_dX; //[hNmax]
		Float_t*        hSM2_dY; //[hNmax]
		Float_t*        hLast_X; //[hNmax]
		Float_t*        hLast_Y; //[hNmax]
		Float_t*        hLast_dX; //[hNmax]
		Float_t*        hLast_dY; //[hNmax]
		Int_t*           hYokeSM2; //[hNmax]
#endif

	private:
		void virtual Init();

#ifndef HPT_DEBUG
ClassDef(HptEventOld,21) //High pT event
#else
ClassDef(HptEventOld,104) //High pT event
#endif
};

#endif

