#! /bin/env python

import os
import sys
import timeit

import ROOT
from ROOT import RooRealVar, RooGaussian, RooArgList
from ROOT import RooVoigtian, RooCBShape, RooArgusBG, RooDecay
from ROOT import RooGaussModel, RooFormulaVar, TCanvas
from ROOT import RooFFTConvPdf, RooNumConvPdf, gPad, RooBreitWigner
from ROOT import RooGenericPdf, gROOT

gROOT.ProcessLine(".L RooAsymCassandra.cxx+")

from ROOT import RooAsymCassandra


# Measured variable
x     = RooRealVar(  'x',     'x',  -1000, 2000 )

# Parameters 1
mean  = RooRealVar(  'mean',  'mean', 1000.0 )
tau0 = RooRealVar(  'tau0',  'tau0', 15.0)
tau1 = RooRealVar(  'tau1',  'tau1', 50.0)
tau2 = RooRealVar(  'tau2',  'tau2', 500.0)

# Parameters 2
mean2  = RooRealVar(  'mean2',  'mean of gaussian', 0.0 )
sigma2 = RooRealVar(  'sigma2', 'width of gaussian', 15.0 )

a_cas = RooAsymCassandra( 'asym_cass', 'PDF', \
        x, mean, tau0, tau1, tau0, tau2)

gauss1 = RooGaussian( 'gauss1', 'gaussian PDF', \
        x, mean2, sigma2 )

p1 = a_cas
p2 = gauss1

pdf = RooNumConvPdf("pdf", 'convolution', x, p2, p1)
pdf.setStringAttribute("CACHEPARMINT","x,tau0,tau1,tau2");
start_time = timeit.default_timer()
pdf.getVal()
elapsed = timeit.default_timer() - start_time
print "Elapsed time: %s" % elapsed
start_time = timeit.default_timer()
pdf.getVal()
elapsed = timeit.default_timer() - start_time
print "Elapsed time: %s" % elapsed
#x.setBins(100000,"fft") ;
#pdf = RooFFTConvPdf("pdf", 'convolution', x, p2, p1)
#pdf.setBufferFraction(500.0)

# Plot PDF
canvas = TCanvas("c1","",1200,480);
canvas.Divide(3,1);

canvas.cd(1)
xframe = x.frame()
p1.plotOn( xframe )
xframe.Draw()
gPad.SetLogy()

canvas.cd(2)
xframe2 = x.frame()
p2.plotOn( xframe2 )
xframe2.Draw()
gPad.SetLogy()

start_time = timeit.default_timer()
canvas.cd(3)
xframe3 = x.frame()
pdf.plotOn( xframe3 )
xframe3.Draw()
gPad.SetLogy()
elapsed = timeit.default_timer() - start_time
print "Elapsed time: %s" % elapsed

print "Done"

raw_input("Press Enter to continue...")

