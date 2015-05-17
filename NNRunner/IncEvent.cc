#include "IncEvent.h"
#include <iostream>

using namespace std;

ClassImp(IncEvent)

    IncEvent::IncEvent()
: hNmax(20), hN(20), hQ(NULL), hNhits(NULL), hP(NULL),
    hPl(NULL), hPt(NULL), hZ(NULL), hXf(NULL), hZlast(NULL),
    hXX0(NULL)
{
    // cout<<"IncEvent ctor()"<<endl; 
    Init();
    Clear();
}

    IncEvent::IncEvent(int hnmax)
: hNmax(hnmax), hN(hnmax), hQ(NULL), hNhits(NULL), hP(NULL),
    hPl(NULL), hPt(NULL), hZ(NULL), hXf(NULL), hZlast(NULL),
    hXX0(NULL)
{
    // cout<<"IncEvent ctor(n)"<<endl; 
    Init();
    Clear();
}

void IncEvent::Init(Option_t* option)
{
    // cout<<"IncEvent::Init"<<endl; 
    hQ = new Int_t[hNmax];
    hNhits = new Int_t[hNmax];
    hP = new Float_t[hNmax];
    hPl = new Float_t[hNmax];
    hPt = new Float_t[hNmax];
    hZ = new Float_t[hNmax];
    hXf = new Float_t[hNmax];
    hZlast = new Float_t[hNmax];
    hXX0 = new Float_t[hNmax];
}

void IncEvent::ResetHadrons(Option_t* option) {
    // cout<<"IncEvent::ResetHadrons"<<endl; 
    if(hQ != NULL) delete [] hQ;
    if(hNhits != NULL) delete [] hNhits;
    if(hP != NULL) delete [] hP;
    if(hPl != NULL) delete [] hPl;
    if(hPt != NULL) delete [] hPt;
    if(hZ != NULL) delete [] hZ;
    if(hXf != NULL) delete [] hXf;
    if(hZlast != NULL) delete [] hZlast;
    if(hXX0 != NULL) delete [] hXX0;
    hQ = hNhits = NULL;
    hP = hPl = hPt = hZ = hXf = hZlast = hXX0 = NULL;

    Init();
    ClearHadrons();
}

void IncEvent::Clear(Option_t* option) {
    // cout<<"IncEvent::Clear"<<endl; 
    eRun = eSpill = eEvInSpill = eEvNr = eTrigMask = eSpinConf = -10000;
    vX = vY = vZ = vN = vCell =  -10000;
    kD = kF = ePt = ePb = -10000;
    kXbj = kQ2 = kY = kRadWeightInc = kRadWeightSem = -10000;
    bP = mP = mPhi = mTheta = -10000;
    kPMiss = kPtMiss = kPlMiss = -10000;
    nnXg = nnXc = nnAllg = nnAllc = nnRpgf = nnRqcdc = nnRlp = -10000;

    ClearHadrons();
}

void IncEvent::ClearHadrons(Option_t* option) {
    // cout<<"IncEvent::ClearHadrons"<<endl; 
    hMinv01 = hMinv02 = hMinv12 = -10000;
    for(int i=0; i < hNmax; i++) {
        hZ[i] = hXf[i] = hZlast[i] = hXX0[i] = -10000;
        hQ[i] = hNhits[i] = -10000;
        hP[i] = hPl[i] = hPt[i] = -10000;
    }
}

IncEvent::~IncEvent()
{
    // cout<<"IncEvent dtor"<<endl; 
    Clear();

    if(hQ != NULL) delete [] hQ;
    if(hNhits != NULL) delete [] hNhits;
    if(hP != NULL) delete [] hP;
    if(hPl != NULL) delete [] hPl;
    if(hPt != NULL) delete [] hPt;
    if(hZ != NULL) delete [] hZ;
    if(hXf != NULL) delete [] hXf;
    if(hZlast != NULL) delete [] hZlast;
    if(hXX0 != NULL) delete [] hXX0;

    hQ = hNhits = NULL;
    hP = hPl = hPt = hZ = hXf = hZlast = hXX0 = NULL;
}

