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

gROOT.ProcessLine(".L RooDoubleCBShape.cxx+")

from ROOT import RooDoubleCBShape


# Measured variable
x     = RooRealVar(  'x',     'x',  800.0, 1200.0 )

# Parameters 1
mean  = RooRealVar(  'mean',  'mean', 1000.0 )
sigma = RooRealVar(  'sigma', 'width of gaussian', 15.0 )
alpha = RooRealVar(  'alpha',  'alpha', -2.5)
beta = RooRealVar(  'beta',  'beta', 0.5)
n1 = RooRealVar(  'n1',  'n1', 1.0)
n2 = RooRealVar(  'n2',  'n2', 1.0)

# Parameters 2
mean2  = RooRealVar(  'mean2',  'mean of gaussian', 1000.0 )
sigma2 = RooRealVar(  'sigma2', 'width of gaussian', 15.0 )

p1 = RooDoubleCBShape( 'double_crystal_ball', 'PDF', \
        x, mean, sigma, alpha, beta, n1, n2)

p2 = RooGaussian( 'gauss1', 'gaussian PDF', \
        x, mean2, sigma2 )

pdf = p1

#pdf = RooPdfAdd("pdf", 'convolution', x, p2, p1)

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

