#include "HptEventOld.h"
// #include <iostream> 

using namespace std;

ClassImp(HptEventOld)

HptEventOld::HptEventOld()
        : hNmax(2)
{
	Init();
    Clear();
}

HptEventOld::HptEventOld(Int_t hnmax)
        : hNmax(hnmax)
{
	Init();
    Clear();
}

void HptEventOld::Init()
{
	// cout<<"HptEventOld::Init"<<endl; 
	hChi2 = new Float_t[hNmax];
	hQ = new Float_t[hNmax];
	hPhi = new Float_t[hNmax];
	hPt = new Float_t[hNmax];
	hPl = new Float_t[hNmax];
	hZ = new Float_t[hNmax];
	hP = new Float_t[hNmax];
	hPx = new Float_t[hNmax];
	hPy = new Float_t[hNmax];
	hPz = new Float_t[hNmax];
	hE = new Float_t[hNmax];
	hTheta = new Float_t[hNmax];
	hXf = new Float_t[hNmax];
	hZlast = new Float_t[hNmax];
	hNhits = new Float_t[hNmax];
	hXX0 = new Float_t[hNmax];
	hEc = new Float_t[hNmax];
	hHc1 = new Float_t[hNmax];
	hHc2 = new Float_t[hNmax];
	hEc1 = new Float_t[hNmax];
	hPid = new Int_t[hNmax];
#ifdef HPT_DEBUG
	hTime = new Float_t[hNmax];
	mHitMapSize = HIT_MAP_SIZE;
#ifdef HPT_OLD_TREE
	mHitMap = new Int_t[mHitMapSize];
#else
	mHitMap = new UInt_t[mHitMapSize];
#endif
	hSM1_X = new Float_t[hNmax];
	hSM1_Y = new Float_t[hNmax];
	hSM1_dX = new Float_t[hNmax];
	hSM1_dY = new Float_t[hNmax];
	hSM2_X = new Float_t[hNmax];
	hSM2_Y = new Float_t[hNmax];
	hSM2_dX = new Float_t[hNmax];
	hSM2_dY = new Float_t[hNmax];
	hLast_X = new Float_t[hNmax];
	hLast_Y = new Float_t[hNmax];
	hLast_dX = new Float_t[hNmax];
	hLast_dY = new Float_t[hNmax];
	hYokeSM2 = new Int_t[hNmax];
#endif
}

void HptEventOld::Clear(Option_t* option)
{
    eRun = eSpill = eEvinspill = eTrig = eTrigAlt = vN = mPid = -1000; //Int_t
	eEvNr = 0; //ulong64
	mNhits = -1000;
    hMinv = vX = vY = vZ = vChi2 = -1000; //Float_t
	eRadWeight = eRadWeightInc = eRadWeightSemInc = -1000; //Float_t
    kQ2 = kXbj = kY = -1000 ; //Double_t
    kW2 = kEgamma = eImu = ePolu = ePold = eD = eNN = -1000 ; //Float_t
	bE = bP = bPx = bPy = mPx = mPy = mE = mP = mZlast = mXX0 = -1000; //Float_t
	bTheta = bPhi = mTheta = mPhi = -1000; //Float_t

	for(Int_t i=0; i < hNmax; i++) {
		hChi2[i] = hQ[i] = -1000;
		hPhi[i] = hPt[i] = hPl[i] = hZ[i] = hP[i] = hE[i] = hTheta[i] = -1000;
		hPx[i] = hPy[i] = hPz[i] = -1000; //Float_t
		hXf[i] = hZlast[i] = hNhits[i] = hXX0[i] = hEc[i] = hHc1[i] = hHc2[i] = hEc1[i] = -1000;
		hPid[i] = -1000;
	}

#ifdef HPT_DEBUG
	eMW1reco = mYokeSM2 = -1000;
	eSM1end = eSM2end = mZfirst = mTime = mChi2 = -1000;

	mNhitsMA01 = mNhitsMA02 = mNhitsPA = mNhitsPB = mNhitsMB = -1000;
	mNhitsHO03Y1 = mNhitsHO04Y1 = mNhitsHO04Y2 = -1000;
	for(Int_t i=0; i < mHitMapSize; i++) {
		mHitMap[i] = 0;
	}

	mHO03Y1 = mHO04Y1 = mHO04Y2 = -1000;

	mHO03Y1_X = mHO03Y1_Y = mHO04Y1_X = mHO04Y1_Y = mHO04Y2_X = mHO04Y2_Y = -1000;
	mMA02Y1_X = mMA02Y1_Y = -1000;
	mSM1_X = mSM1_Y = mSM1_dX = mSM1_dY = mSM2_X = mSM2_Y = mSM2_dX = mSM2_dY = -1000; // Float_t
	mLast_X = mLast_Y = mLast_dX = mLast_dY = -1000; // Float_t

	eCaloMax_E_HC1 = eCaloMax_E_HC2 = eCaloMax_E_EC1 = eCaloMax_E_EC2 = -1000;
	eCaloMax_P_HC1 = eCaloMax_P_HC2 = eCaloMax_P_EC1 = eCaloMax_P_EC2 = -1000;

	for(Int_t i=0; i < hNmax; i++) {
		hTime[i] = -1000; //Float_t
		hSM1_X[i] = hSM1_Y[i] = hSM1_dX[i] = hSM1_dY[i] = -1000; //Float_t
		hSM2_X[i] = hSM2_Y[i] = hSM2_dX[i] = hSM2_dY[i] = -1000; //Float_t
		hLast_X[i] = hLast_Y[i] = hLast_dX[i] = hLast_dY[i] = -1000; // Float_t
		hYokeSM2[i] = -1000;
	}
#endif //HPT_DEBUG
}

HptEventOld::~HptEventOld()
{
    Clear();

	if(hChi2) delete [] hChi2;
	if(hQ) delete [] hQ;
	if(hPhi) delete [] hPhi;
	if(hPt) delete [] hPt;
	if(hPl) delete [] hPl;
	if(hZ) delete [] hZ;
	if(hP) delete [] hP;
	if(hE) delete [] hE;
	if(hTheta) delete [] hTheta;
	if(hXf) delete [] hXf;
	if(hZlast) delete [] hZlast;
	if(hNhits) delete [] hNhits;
	if(hXX0) delete [] hXX0;
	if(hEc) delete [] hEc;
	if(hHc1) delete [] hHc1;
	if(hHc2) delete [] hHc2;
	if(hEc1) delete [] hEc1;
	if(hPid) delete [] hPid;
#ifdef HPT_DEBUG
	if(hPx) delete [] hPx;
	if(hPy) delete [] hPy;
	if(hPz) delete [] hPz;
	if(hTime) delete [] hTime;
	if(mHitMap) delete [] mHitMap;
#endif
}
