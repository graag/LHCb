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

/** \class RooCutCBShape
    \ingroup Roofit

P.d.f implementing the Crystal Ball line shape
**/

#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooCutCBShape.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooMath.h"
#include "TMath.h"

#include "TError.h"

using namespace std;

//ClassImp(RooCutCBShape);

////////////////////////////////////////////////////////////////////////////////

Double_t RooCutCBShape::ApproxErf(Double_t arg) const
{
  static const double erflim = 5.0;
  if( arg > erflim )
    return 1.0;
  if( arg < -erflim )
    return -1.0;

  return RooMath::erf(arg);
}

////////////////////////////////////////////////////////////////////////////////

RooCutCBShape::RooCutCBShape(const char *name, const char *title,
             RooAbsReal& _m, RooAbsReal& _m0, RooAbsReal& _sigma,
             RooAbsReal& _alpha, RooAbsReal& _beta, RooAbsReal& _n) :
  RooAbsPdf(name, title),
  m("m", "Dependent", this, _m),
  m0("m0", "M0", this, _m0),
  sigma("sigma", "Sigma", this, _sigma),
  alpha("alpha", "Alpha", this, _alpha),
  beta("beta", "Beta", this, _beta),
  n("n", "Order", this, _n)
{
}

////////////////////////////////////////////////////////////////////////////////

RooCutCBShape::RooCutCBShape(const RooCutCBShape& other, const char* name) :
  RooAbsPdf(other, name), m("m", this, other.m), m0("m0", this, other.m0),
  sigma("sigma", this, other.sigma), alpha("alpha", this, other.alpha),
  beta("beta", this, other.beta), n("n", this, other.n)
{
}

////////////////////////////////////////////////////////////////////////////////

Double_t RooCutCBShape::evaluate() const {
  Double_t t = (m-m0)/sigma;
  if (alpha < 0) t = -t;

  Double_t absAlpha = fabs((Double_t)alpha);
  Double_t absBeta = fabs((Double_t)beta)+absAlpha;

  if (t >= -absAlpha) {
    return exp(-0.5*t*t);
  }
  else if (t < -absBeta) {
    Double_t a =  TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    Double_t b = n/absAlpha - absAlpha;
    Double_t c = a/TMath::Power(b + absBeta, n);
    Double_t x = sqrt(-2.0*log(c));
    Double_t y = fabs(t) + x - absBeta;
    return exp(-0.5*y*y);
  }
  else {
    Double_t a =  TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    Double_t b = n/absAlpha - absAlpha;

    return a/TMath::Power(b - t, n);
  }
}
