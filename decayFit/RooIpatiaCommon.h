#ifndef ROOIPATIACOMMON
#define ROOIPATIACOMMON

#include "TMath.h"

const Double_t sq2pi = TMath::Sqrt(2.*TMath::ACos(-1.));
const Double_t sq2pi_inv = 1./sq2pi;
const Double_t logsq2pi = TMath::Log(sq2pi);
const Double_t log_de_2 = TMath::Log(2.);

Double_t low_x_BK(Double_t nu,Double_t x);
Double_t low_x_LnBK(Double_t nu, Double_t x);
Double_t BK(Double_t ni, Double_t x);
Double_t LnBK(double ni, double x);
Double_t LogEval(Double_t d, Double_t l, Double_t alpha, Double_t beta, Double_t delta);
Double_t diff_eval(Double_t d, Double_t l, Double_t alpha, Double_t beta, Double_t delta);

#endif
