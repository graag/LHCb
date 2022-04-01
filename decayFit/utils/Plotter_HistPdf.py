#! /bin/env python

import os
import sys

import ROOT
from ROOT import RooRealVar, RooGaussian, RooArgList
from ROOT import RooVoigtian, RooCBShape, RooArgusBG, RooDecay
from ROOT import RooGaussModel, RooFormulaVar, TCanvas
from ROOT import RooFFTConvPdf, RooNumConvPdf, gPad, RooBreitWigner
from ROOT import RooGenericPdf, TChain, RooDataSet, RooArgSet, RooFit, RooHistPdf


# Measured variable
x     = RooRealVar(  'mass', 'mass',  4000, 7000 )

# Parameters
mean  = RooRealVar(  'mean',  'mean of gaussian', 0.0 )
sigma = RooRealVar(  'sigma', 'width of gaussian', 5.0 )

# Load reference ROOT tree
full_ch = TChain("DecayTree")
full_ch.Add("/mnt/home/kklimaszewski/LHCb/Bs2JpsieePhi/M_Bs_Fits/mc/DVNtuples_Bs2JpsieePhi_13154001_MCfull_R14ac_TupleBsDetached_PIDCorr_S21_NewSel_tupleB.root")
# Run selection
ch = full_ch.CopyTree("sigmam>0. && sigmam<250. && time>0.3 && time<14. && sigmat<0.12 && BDT_response_NewSel>0.2 && eminus_bremmult==0 && eplus_bremmult==0")
#fit.mass.setBins(_comp['config']['bins'])
# Create hist PDF
data = RooDataSet(
        'data_set',
        '',
        RooArgSet(x),
        RooFit.Import(ch)
        )
hist = data.binnedClone()
p1 = RooHistPdf(
        'histpdf',
        '',
        RooArgSet(x),
        hist,
        2  # Order of interpolation function
        )
#p2 = RooGaussian(
p2 = RooGaussModel(
    "comp_2",
    "",
    x,
    mean,
    sigma
    )

#pdf = RooNumConvPdf("pdf", 'convolution', x, p1, p2)
x.setBins(10000,"fft") ;
pdf = RooFFTConvPdf("pdf", 'convolution', x, p2, p1)
pdf.setBufferFraction(5.0)
#pdf.setBufferFraction(50.0)

# Plot PDF
canvas = TCanvas("c1","",1200,480);
canvas.Divide(3,1);

canvas.cd(1)
xframe = x.frame()
p1.plotOn( xframe )
xframe.Draw()
gPad.SetLogy()

#canvas.cd(2)
#xframe2 = x.frame()
#p2.plotOn( xframe2 )
#xframe2.Draw()
#gPad.SetLogy()

canvas.cd(3)
xframe3 = x.frame()
pdf.plotOn( xframe3 )
xframe3.Draw()
gPad.SetLogy()

print "Done"

raw_input("Press Enter to continue...")

