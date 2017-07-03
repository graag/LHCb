/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOASYMCASSANDRA
#define ROOASYMCASSANDRA

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooAsymCassandra : public RooAbsPdf {
public:
  RooAsymCassandra() {} ; 
  RooAsymCassandra(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _mean,
	      RooAbsReal& _tau_1,
	      RooAbsReal& _tau_2,
	      RooAbsReal& _tau_3,
	      RooAbsReal& _tau_4);
  RooAsymCassandra(const RooAsymCassandra& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooAsymCassandra(*this,newname); }
  inline virtual ~RooAsymCassandra() { }

protected:

  RooRealProxy x ;
  RooRealProxy mean ;
  RooRealProxy tau_1 ;
  RooRealProxy tau_2 ;
  RooRealProxy tau_3 ;
  RooRealProxy tau_4 ;
  
  Double_t evaluate() const ;

};
 
#endif
