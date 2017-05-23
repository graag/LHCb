#! /bin/env python

import os
import sys

import ROOT
from ROOT import RooRealVar, RooGaussian, RooArgList
from ROOT import RooVoigtian, RooCBShape, RooArgusBG, RooDecay
from ROOT import RooGaussModel, RooFormulaVar, TCanvas
from ROOT import RooFFTConvPdf, RooNumConvPdf, gPad, RooBreitWigner

# Measured variable
x     = RooRealVar(  'x',     'x',  -5000, 5000 )

# Parameters
mean  = RooRealVar(  'mean',  'mean of gaussian', 0 )
mean2  = RooRealVar(  'mean2',  'mean of gaussian', 0 )
tau = RooRealVar(  'tau',  'tau', 1000)
tau2 = RooRealVar(  'tau2',  'tau', 100 )
sigma1 = RooRealVar(  'sigma1', 'width of gaussian', 0.01
         )
sigma2 = RooRealVar(  'sigma2', 'width of gaussian', 100.0
         )

gauss1 = RooGaussian( 'gauss1', 'gaussian PDF', \
        x, mean, sigma1 )
gauss2 = RooGaussian( 'gauss2', 'gaussian PDF', \
        x, mean, sigma2 )

bw = RooBreitWigner( 'bw', 'gaussian PDF', \
        x, mean2, sigma2 )

r1 = RooGaussModel(
        "resolution",
        "resolution",
        x,
        mean,
        sigma1
        )
decay = RooDecay(
    "exp",
    "exp",
    x,
    tau,
    r1,
    RooDecay.DoubleSided)

r2 = RooGaussModel(
        "resolution2",
        "resolution",
        x,
        mean2,
        sigma2
        )
decay2 = RooDecay(
    "exp2",
    "exp",
    x,
    tau2,
    r2,
    RooDecay.DoubleSided)


p1 = decay
p2 = bw


pdf = RooNumConvPdf("pdf", 'convolution', x, p1, p2)
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

canvas.cd(3)
xframe3 = x.frame()
pdf.plotOn( xframe3 )
xframe3.Draw()
gPad.SetLogy()

print "Done"

raw_input("Press Enter to continue...")

