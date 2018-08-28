/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id$
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// BEGIN_HTML
// P.d.f implementing the Crystall Ball line shape
// END_HTML
//

#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooDoubleCBShape.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooMath.h"
#include "TMath.h"

#include "TError.h"

using namespace std;

//ClassImp(RooCBShape) ;

  

//_____________________________________________________________________________
Double_t RooDoubleCBShape::ApproxErf(Double_t arg) const 
{
  static const double erflim = 5.0;
  if( arg > erflim )
    return 1.0;
  if( arg < -erflim )
    return -1.0;
  
  return RooMath::erf(arg);
}



//_____________________________________________________________________________
RooDoubleCBShape::RooDoubleCBShape(const char *name, const char *title,
		       RooAbsReal& _m, RooAbsReal& _m0, RooAbsReal& _sigma,
		       RooAbsReal& _alpha, RooAbsReal& _beta, RooAbsReal& _n1,
		       RooAbsReal& _n2) :
  RooAbsPdf(name, title),
  m("m", "Dependent", this, _m),
  m0("m0", "M0", this, _m0),
  sigma("sigma", "Sigma", this, _sigma),
  alpha("alpha", "Alpha", this, _alpha),
  beta("beta", "Beta", this, _beta),
  n1("n1", "Order low", this, _n1),
  n2("n2", "Order high", this, _n2)
{
}


//_____________________________________________________________________________
RooDoubleCBShape::RooDoubleCBShape(const RooDoubleCBShape& other, const char* name) :
  RooAbsPdf(other, name), m("m", this, other.m), m0("m0", this, other.m0),
  sigma("sigma", this, other.sigma), alpha("alpha", this, other.alpha),
  beta("beta", this, other.beta),
  n1("n1", this, other.n1), n2("n2", this, other.n2)
{
}


//_____________________________________________________________________________
Double_t RooDoubleCBShape::evaluate() const {

  assert (beta >= alpha);
  Double_t t = (m-m0)/sigma;
  //if (alpha < 0) t = -t;

  Double_t absAlpha = fabs((Double_t)alpha);
  Double_t absBeta = fabs((Double_t)beta);

  if (t < alpha) {
    Double_t a =  TMath::Power(n1/absAlpha,n1)*exp(-0.5*absAlpha*absAlpha);
    Double_t b= n1/absAlpha - absAlpha; 

    return a/TMath::Power(b - t, n1);
  } else if (alpha<=t && t<beta) {
    return exp(-0.5*t*t);
  } else {
    t = -t;
    Double_t a =  TMath::Power(n2/absBeta,n2)*exp(-0.5*absBeta*absBeta);
    Double_t b= n2/absBeta - absBeta;

    return a/TMath::Power(b - t, n2);
  }
}


//_____________________________________________________________________________
//Int_t RooDoubleCBShape::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
/*
{
  if( matchArgs(allVars,analVars,m) )
    return 1 ;
  
  return 0;
}



//_____________________________________________________________________________
Double_t RooDoubleCBShape::analyticalIntegral(Int_t code, const char* rangeName) const
{
  static const double sqrtPiOver2 = 1.2533141373;
  static const double sqrt2 = 1.4142135624;

  R__ASSERT(code==1);
  double result = 0.0;
  bool useLog = false;
  
  if( fabs(n-1.0) < 1.0e-05 )
    useLog = true;
  
  double sig = fabs((Double_t)sigma);
  
  double tmin = (m.min(rangeName)-m0)/sig;
  double tmax = (m.max(rangeName)-m0)/sig;
  
  if(alpha < 0) {
    double tmp = tmin;
    tmin = -tmax;
    tmax = -tmp;
  }

  double absAlpha = fabs((Double_t)alpha);
  
  if( tmin >= -absAlpha ) {
    result += sig*sqrtPiOver2*(   ApproxErf(tmax/sqrt2)
                                - ApproxErf(tmin/sqrt2) );
  }
  else if( tmax <= -absAlpha ) {
    double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    double b = n/absAlpha - absAlpha;
    
    if(useLog) {
      result += a*sig*( log(b-tmin) - log(b-tmax) );
    }
    else {
      result += a*sig/(1.0-n)*(   1.0/(TMath::Power(b-tmin,n-1.0))
                                - 1.0/(TMath::Power(b-tmax,n-1.0)) );
    }
  }
  else {
    double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    double b = n/absAlpha - absAlpha;
    
    double term1 = 0.0;
    if(useLog) {
      term1 = a*sig*(  log(b-tmin) - log(n/absAlpha));
    }
    else {
      term1 = a*sig/(1.0-n)*(   1.0/(TMath::Power(b-tmin,n-1.0))
                              - 1.0/(TMath::Power(n/absAlpha,n-1.0)) );
    }
    
    double term2 = sig*sqrtPiOver2*(   ApproxErf(tmax/sqrt2)
                                     - ApproxErf(-absAlpha/sqrt2) );
    
    
    result += term1 + term2;
  }
  
  return result;
}
*/

/*
//_____________________________________________________________________________
Int_t RooCBShape::getMaxVal(const RooArgSet& vars) const 
{
  // Advertise that we know the maximum of self for given (m0,alpha,n,sigma)
  RooArgSet dummy ;

  if (matchArgs(vars,dummy,m)) {
    return 1 ;  
  }
  return 0 ;  
}



//_____________________________________________________________________________
Double_t RooCBShape::maxVal(Int_t code) const
{
  R__ASSERT(code==1) ;

  // The maximum value for given (m0,alpha,n,sigma)
  return 1.0 ;
}
*/

