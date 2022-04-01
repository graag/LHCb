/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCassandra3.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 


 RooCassandra3::RooCassandra3(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _mean,
                        RooAbsReal& _tau_1,
                        RooAbsReal& _tau_2,
                        RooAbsReal& _tau_3) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   mean("mean","mean",this,_mean),
   tau_1("tau_1","tau_1",this,_tau_1),
   tau_2("tau_2","tau_2",this,_tau_2),
   tau_3("tau_3","tau_3",this,_tau_3)
 { 
 } 


 RooCassandra3::RooCassandra3(const RooCassandra3& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   mean("mean",this,other.mean),
   tau_1("tau_1",this,other.tau_1),
   tau_2("tau_2",this,other.tau_2),
   tau_3("tau_3",this,other.tau_3)
 { 
 } 



 Double_t RooCassandra3::evaluate() const 
 {
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t y1 = exp(-abs(x-mean)/tau_1)/tau_1;
   Double_t y2 = exp(-abs(x-mean)/tau_2)/tau_2;
   Double_t y3 = exp(-abs(x-mean)/tau_3)/tau_3;
   if( y2 > y1 && y2 > y3)
       return y2;
   else if ( y3 > y1 )
       return y3;
   return y1;
 }
