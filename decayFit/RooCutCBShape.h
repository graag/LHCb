/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooCutCBShape.h,v 1.11 2007/07/12 20:30:49 wouter Exp $
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
#ifndef ROO_CUT_CB_SHAPE
#define ROO_CUT_CB_SHAPE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooCutCBShape : public RooAbsPdf {
public:
  RooCutCBShape() {} ;
  RooCutCBShape(const char *name, const char *title, RooAbsReal& _m,
        RooAbsReal& _m0, RooAbsReal& _sigma,
        RooAbsReal& _alpha, RooAbsReal& _beta, RooAbsReal& _n);

  RooCutCBShape(const RooCutCBShape& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { return new RooCutCBShape(*this,newname); }

  inline virtual ~RooCutCBShape() { }

protected:

  Double_t ApproxErf(Double_t arg) const ;

  RooRealProxy m;
  RooRealProxy m0;
  RooRealProxy sigma;
  RooRealProxy alpha;
  RooRealProxy beta;
  RooRealProxy n;

  Double_t evaluate() const;

//private:

//  ClassDef(RooCutCBShape,1) // Crystal Ball lineshape PDF
};

#endif
