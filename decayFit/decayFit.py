#! /bin/env python

import os
import sys
import json

# Parse parameters
if len(sys.argv) < 4:
    print """decayFit.py [-h/--help] <setup> <tree> tree.root []

<setup> - name of setup file: e.g. Bs_SIG_2Gauss_BKG_Chebyshev.json
<tree> - tree name, can contain path: e.g. TupleBs/DecayTree
tree.root - input ROOT files
"""
    sys.exit(1)

setup = sys.argv[1]
path = sys.argv[2]
tree = os.path.basename(path)
jobs = sys.argv[3:]
script_dir = os.path.dirname(os.path.realpath(__file__))

print "Setup: " + setup
print "Tree: " + tree
print "Path in ROOT file: " + path
#print "Input files: %s" % jobs

# Load fit setup
with open(setup) as f:
    fit_params = json.load(f)

# Setup ROOT
sys.argv.append('-b')
import ROOT
from ROOT import gSystem, gROOT, TChain, TF1, TH1D, TPaveLabel, TList, kTRUE

# Load data files
ch = TChain(tree)

i=0
for job in jobs:
    name = "%s/%s" %(job, path)
    #print "Adding: " + name
    ch.Add(name)

print "Will load #files: " + str(ch.GetListOfFiles().GetEntries())
print "Will load #entries: " + str(ch.GetEntries())
gSystem.Load("libRooFit")
gSystem.Load("libRooStats")

# Compile and load the MassFit class
gROOT.ProcessLine(".L %s+" % os.path.join(script_dir, "massFit.C"))
from ROOT import MassFit, fit_list, RooRealVar, RooGaussian, RooArgList
from ROOT import RooAddPdf, RooPolynomial, RooChebychev, RooExponential
from ROOT import RooVoigtian, RooCBShape, RooArgusBG, RooDecay
from ROOT import RooGaussModel, RooFormulaVar, RooIpatia, RooBreitWigner
from ROOT import RooFFTConvPdf, RooNumConvPdf


def init_pdfs(fit, params, prefix, signal):
    '''
    Initialize RooFit PDFs from parameter dictionary

    :param MassFit fit: - MassFit instance
    :param dict params: - parameter dictionary
    :param str prefix: - pdf name prefix
    :param bool signal: - signal pdf
    :return: (pdf, components) with pdf - Composit pdf, components - list of component pdfs
    :rtype: tuple
    '''
    # Build recursive composit pdf:
    # M(x) = f_sig*S(x) + (1-f_sig){f_peak*P(x) + (1-f_peak)*B(x)}
    #
    # This avoids problems with sum(fractions) > 1
    # See chapter 3 of RooFit Users Manual
    #
    # Store all auto generated obejcts in TLists. Otherwise python will destroy
    # them as soon as this function ends

    _pdf = None  #: Composit PDF to return
    _components = TList()  #: List of component PDFs
    _fracs = TList()  #: List of fractions
    _funcs = TList()  #: List of formula variables
    _params = { prefix+"_"+fit.mass_name: fit.mass }
    _i = 1
    for _comp in params["components"]:
        # Create parameters
        _comp_params = TList()
        for _par in _comp["params"]:
            _name = prefix + "_" + _par["name"]
            if _name in _params:
                _comp_params.append(_params[_name])
            else:
                _var = RooRealVar(
                        _name, _par["title"], _par["value"],
                        _par["min"], _par["max"], _par["units"]
                        )
                fit.parameters.push_back(_var)
                _comp_params.Add(_var)
                _params[_name] = _var
        # Store mean and sigma of core part of the signal PDF
        if _i == 1 and signal == True:
            fit.sig_mass = _comp_params.At(0)
            fit.sig_sigma = _comp_params.At(1)
        _name = prefix+"_"+_comp["name"]
        # Initialize component
        if _comp["type"] == "RooGaussian":
            _components.Add(RooGaussian(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1))
                    )
        elif _comp["type"] == "RooPolynomial":
            if len(_comp_params):
                _components.Add(RooPolynomial(
                        _name,
                        _comp["title"],
                        fit.mass,
                        RooArgList(_comp_params))
                        )
            else:
                _components.Add(RooPolynomial(
                        _name,
                        _comp["title"],
                        fit.mass)
                        )
        elif _comp["type"] == "RooChebychev":
            _components.Add(RooChebychev(
                    _name,
                    _comp["title"],
                    fit.mass,
                    RooArgList(_comp_params))
                    )
        elif _comp["type"] == "RooExponential":
            _components.Add(RooExponential(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0))
                    )
        elif _comp["type"] == "RooVoigtian":
            _components.Add(RooVoigtian(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2))
                    )
        elif _comp["type"] == "RooCBShape":
            _components.Add(RooCBShape(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3))
                    )
        elif _comp["type"] == "RooArgusBG":
            _components.Add(RooArgusBG(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1))
                    )
        elif _comp["type"] == "RooRArgusBG":
            _v = RooFormulaVar(
                    _name+"_variable",
                    "",
                    "2*@0 - @1",
                    RooArgList(
                        _comp_params.At(0),
                        fit.mass)
                    )
            _funcs.Add(_v)
            _components.Add(RooArgusBG(
                    _name,
                    _comp["title"],
                    _v,
                    _comp_params.At(0),
                    _comp_params.At(1))
                    )
        elif _comp["type"] == "RooIpatia":
            _components.Add(RooIpatia(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3),
                    _comp_params.At(4),
                    _comp_params.At(5),
                    _comp_params.At(6))
                    )
        elif _comp["type"] == "RooDecay":
            _r = RooGaussModel(
                    _name+"_resolution",
                    _comp["title"]+"_resolution",
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(2)
                    )
            _components.Add(RooDecay(
                _name,
                _comp["title"],
                fit.mass,
                _comp_params.At(1),
                _r,
                RooDecay.DoubleSided)
                )
            _funcs.Add(_r)
        elif _comp["type"] == "RooDecayLeft":
            _r = RooGaussModel(
                    _name+"_resolution",
                    _comp["title"]+"_resolution",
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(2)
                    )
            _components.Add(RooDecay(
                _name,
                _comp["title"],
                fit.mass,
                _comp_params.At(1),
                _r,
                RooDecay.Flipped)
                )
            _funcs.Add(_r)
        elif _comp["type"] == "RooDecayRight":
            _r = RooGaussModel(
                    _name+"_resolution",
                    _comp["title"]+"_resolution",
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(2)
                    )
            _components.Add(RooDecay(
                _name,
                _comp["title"],
                fit.mass,
                _comp_params.At(1),
                _r,
                RooDecay.SingleSided)
                )
            _funcs.Add(_r)
        elif _comp["type"] == "RooBwDecay":
            # Define sampling frequency
            #fit.mass.setBins(10000,"fft") ;
            _r1 = RooGaussModel(
                    _name+"_resolution_1",
                    _comp["title"]+"_resolution_1",
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(2)
                    )
            _f1 = RooDecay(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(1),
                _r1,
                RooDecay.DoubleSided
                )
            _f2 = RooBreitWigner(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(3),
                _comp_params.At(4)
                )
            _funcs.Add(_r1)
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooNumConvPdf(
            #_pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            #_pdf.setBufferFraction(0.2)
            _components.Add(_pdf)
        else:
            raise Exception("Unknown PDF type: "+_comp["type"])

        if _i > 1:
            _fracs.Add(RooRealVar(
                    prefix+"_frac_"+str(_i),
                    prefix+" component "+str(_i)+" fraction",
                    0.1, 0.0, 1.0)
                    )

        _i += 1

    if _fracs.GetSize() == 0:
        _pdf = _components.At(0)
    else:
        # Make sure we are creating a recursive composit PDF
        _pdf = RooAddPdf(
            prefix+"_pdf",
            prefix+" PDF",
            RooArgList(_components),
            RooArgList(_fracs),
            kTRUE)

    return (_pdf, _components)

# Generate fit instances
for fpars in fit_params:
    fit = MassFit()

    fit.name = str(fpars["name"])
    fit.selection = str(fpars["selection"])
    fit.mass_name = str(fpars["mass"]["var_name"])
    fit.mass_err_name = str(fpars["mass"]["err_name"])
    fit.mass_title = str(fpars["mass"]["title"])
    fit.mass_range_min = fpars["mass"]["min"]
    fit.mass_range_max = fpars["mass"]["max"]
    if "fit_min" in fpars["mass"]:
        fit.mass_fit_min = fpars["mass"]["fit_min"]
    if "fit_max" in fpars["mass"]:
        fit.mass_fit_max = fpars["mass"]["fit_max"]
    fit.single_candidate = fpars["single_candidate"]
    fit.bins = fpars["bins"]
    fit.bins_pull = fpars["bins_pull"]
    if "run_sfit" in fpars:
        fit.run_sfit = fpars["run_sfit"];
    fit.ncpu = 4
    fit.init()

    sig_yield = fpars["signal"]["yield"]
    nsig = RooRealVar( str(sig_yield["name"]), str(sig_yield["title"]),
            sig_yield["value"], sig_yield["min"], sig_yield["max"])
    fit.sig_n = nsig
    prefix = "Sig"
    if len(fit_params) > 1:
        prefix = fit.name + "_" + prefix
    (sig_pdf, sig_components) = init_pdfs(fit, fpars["signal"], prefix, True)

    bkg_yield = fpars["background"]["yield"]
    nbkg = RooRealVar( bkg_yield["name"], bkg_yield["title"],
            bkg_yield["value"], bkg_yield["min"], bkg_yield["max"])
    fit.bkg_n = nbkg
    prefix = "Bg"
    if len(fit_params) > 1:
        prefix = fit.name + "_" + prefix
    (bkg_pdf, bkg_components) = init_pdfs(fit, fpars["background"], prefix, False)

    pdf = RooAddPdf(
            "MassPdf^{"+fit.name+"}",
            "Mass PDF",
            RooArgList(sig_pdf, bkg_pdf),
            RooArgList(nsig, nbkg) )
    fit.sig_pdf = sig_pdf
    fit.bkg_pdf = bkg_pdf
    for x in range(sig_components.GetSize()):
        fit.sig_components.push_back(sig_components.At(x))
    for x in range(bkg_components.GetSize()):
        fit.bkg_components.push_back(bkg_components.At(x))
    fit.fit_pdf = pdf

    fit_list.push_back(fit)

ch.MakeProxy("decayFit_proxy", os.path.join(script_dir, "decayFit.C"), "", "nohist")
ch.Process("decayFit_proxy.h++");

print "Done"

raw_input("Press Enter to continue...")

