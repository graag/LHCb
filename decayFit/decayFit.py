#! /bin/env python

import os
import sys
import json
import time
import argparse
import random

supported_pdfs = [
        "RooAddPdf",
        "RooGaussian",
        "RooParamGaussian",
        "RooParamGaussianOffset",
        "RooPolynomial",
        "RooChebychev",
        "RooExponential",
        "RooVoigtian",
        "RooCBShape",
        "RooParamCBShape",
        "RooDoubleCBShape",
        "RooCutCBShape",
        "RooArgusBG",
        "RooRArgusBG",
        "RooIpatia",
        "RooIpatia2",
        "RooCassandra",
        "RooAsymCassandra",
        "RooDecay",
        "RooDecayLeft",
        "RooDecayRight",
        "RooExpGaussExp",
        "RooHistPdf",
        "FFT_RooDecay_RooBreitWigner",
        "Conv_RooDecay_RooBreitWigner",
        "FFT_RooDecay_RooCBShape",
        "FFT_RooCassandra_RooGaussian",
        "FFT_RooAsymCassandra_RooGaussian",
        "FFT_RooAsymCassandra_RooDoubleGaussian",
        "Conv_RooAsymCassandra_RooGaussian",
        "FFT_RooAsymCassandra3_RooGaussian",
        "FFT_RooCassandra_RooCBShape",
        "FFT_RooCassandra3_RooCBShape",
        "FFT_RooHistPdf_RooGaussian",
        "Conv_RooHistPdf_RooGaussian",
        "FFT_RooDoubleCBShape_RooGaussian",
        "FFT_2xRooCBShape_RooGaussian"
        ]

start_time = time.time()
# Parse parameters
parser = argparse.ArgumentParser(description='Fit decay distribution using predefined PDF.')
parser.add_argument('trees', metavar='TREE', nargs='?', action='append',
                    help='ROOT files with data tree.')
parser.add_argument('-s', '--setup', dest='setup', action='store',
                    help='Name of Json file with fit description.')
parser.add_argument('-n', '--tree-name', dest='tree_name', action='store',
                    help='Tree name with path inside the ROOT file.')
parser.add_argument('-l', '--list', dest='list', action='store_true',
                    help='List supported PDFs.')
parser.add_argument('-L', '--load', dest='load', action='store_true',
                    help='Load fit results from a ROOT file. Skip loading trees.')
parser.add_argument('-p', '--plot', dest='plot', action='store_true',
                    help='Plot PDFs with starting parameters.')

args = parser.parse_args()

if args.list:
    for _pdf in supported_pdfs:
        print "- %s" % _pdf
    sys.exit(0)

if args.load and len(args.trees) > 1:
    print "Specify single input file to load fit results."
    sys.exit(1)

setup = args.setup
jobs = args.trees
script_dir = os.path.dirname(os.path.realpath(__file__))
path = args.tree_name
tree = os.path.basename(path)


# Load fit setup
with open(setup) as f:
    fit_params = json.load(f)

print "Setup: " + setup
print "Tree: " + tree
print "Path in ROOT file: " + path
print "Input files: %s" % jobs

# Setup ROOT
#sys.argv.append('-b')
import ROOT
from ROOT import gSystem, gROOT, TChain, TF1, TH1D, TPaveLabel, TList, kTRUE, kRed
from ROOT import TFile

if not args.load:
    # Load data files
    ch = TChain(tree)

    i=0
    for job in jobs:
        name = "%s/%s" %(job, path)
        print "Adding: " + name
        ch.Add(name)

    print "Will load #files: " + str(ch.GetListOfFiles().GetEntries())
    print "Will load #entries: " + str(ch.GetEntries())

gSystem.Load("libRooFit")
gSystem.Load("libRooStats")

# Compile and load the MassFit class
gROOT.ProcessLine(".L %s+" % os.path.join(script_dir, "massFit.C"))
from ROOT import RooFit, MassFit, fit_list, RooRealVar, RooGaussian, RooArgList
from ROOT import RooAddPdf, RooPolynomial, RooChebychev, RooExponential
from ROOT import RooVoigtian, RooCBShape, RooArgusBG, RooDecay, RooHistPdf
from ROOT import RooGaussModel, RooFormulaVar, RooIpatia, RooBreitWigner
from ROOT import RooFFTConvPdf, RooNumConvPdf, RooCassandra, RooCassandra3
from ROOT import RooAsymCassandra, RooAsymCassandra3, RooDataSet, RooArgSet
from ROOT import RooExpGaussExp, RooDoubleCBShape, RooIpatia2, RooCutCBShape


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
    _components_all = TList()  #: List of component PDFs
    _fracs = TList()  #: List of fractions
    _funcs = TList()  #: List of formula variables
    _params = { prefix+"_"+fit.mass_name: fit.mass }
    _i = 1
    _n_comps = len(params["components"])
    for _comp in params["components"]:
        # Create parameters
        _comp_params = TList()
        _comp_constrs = TList()
        for _par in _comp["params"]:
            _name = prefix + "_" + _par["name"]
            if _name in _params:
                _comp_params.append(_params[_name])
            else:
                if "random" in _par and _par["random"] == True:
                    # Generate starting value and validity range
                    _min = random.uniform(_par["min"], _par["value"])
                    _max = random.uniform(_par["value"], _par["max"])
                    _value = random.uniform(_min, _max)
                    _par["random"] = False
                    # Store obtained values
                    _par["min"] = _min
                    _par["max"] = _max
                    _par["value"] = _value
                    _var = RooRealVar(
                            _name, _par["title"], _value,
                            _min, _max, _par["units"]
                            )
                else:
                    _var = RooRealVar(
                            _name, _par["title"], _par["value"],
                            _par["min"], _par["max"], _par["units"]
                            )
                fit.parameters.push_back(_var)
                _comp_params.Add(_var)
                _params[_name] = _var
                if "constraint" in _par:
                    _constr = RooGaussian(
                            _name+"_constr",
                            _par["title"]+"_constr",
                            _var,
                            RooFit.RooConst(_par["constraint"]["mean"]),
                            RooFit.RooConst(_par["constraint"]["sigma"])
                            )
                    fit.constraints.push_back(_constr)
                    _comp_constrs.Add(_constr)
        # Store mean and sigma of core part of the signal PDF
        if _i == 1 and signal == True:
            fit.sig_mass = _comp_params.At(0)
            fit.sig_sigma = _comp_params.At(1)
        _name = prefix+"_"+_comp["name"]
        # Initialize component
        if _comp["type"] not in supported_pdfs:
            raise Exception("Unknown PDF type: %s" % _comp["type"])
        if _comp["type"] == "RooAddPdf":
            (_sum_pdf, _sum_components, _sum_params) = init_pdfs(fit, _comp, _comp['name'], signal)
            _components_all.AddAll(_sum_components)
            _components.Add(_sum_pdf)
            _params.update(_sum_params)
        elif _comp["type"] == "RooGaussian":
            _components.Add(RooGaussian(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1))
                    )
        elif _comp["type"] == "RooParamGaussian":
            _v = RooFormulaVar(
                    _name+"_variable",
                    "",
                    "@0*@1",
                    RooArgList(
                        _comp_params.At(1),
                        _comp_params.At(2))
                    )
            _funcs.Add(_v)
            _components.Add(RooGaussian(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _v)
                    )
        elif _comp["type"] == "RooParamGaussianOffset":
            _m = RooFormulaVar(
                    _name+"_var_mean",
                    "",
                    "@0+@1",
                    RooArgList(
                        _comp_params.At(0),
                        _comp_params.At(1))
                    )
            _s = RooFormulaVar(
                    _name+"_var_sigma",
                    "",
                    "@0*@1",
                    RooArgList(
                        _comp_params.At(2),
                        _comp_params.At(3))
                    )
            _funcs.Add(_m)
            _funcs.Add(_s)
            _components.Add(RooGaussian(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _m,
                    _s)
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
        elif _comp["type"] == "RooParamCBShape":
            _v = RooFormulaVar(
                    _name+"_variable",
                    "",
                    "@0*@1",
                    RooArgList(
                        _comp_params.At(1),
                        _comp_params.At(2))
                    )
            _funcs.Add(_v)
            _components.Add(RooCBShape(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _v,
                    _comp_params.At(3),
                    _comp_params.At(4))
                    )
        elif _comp["type"] == "RooDoubleCBShape":
            _components.Add(RooDoubleCBShape(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3),
                    _comp_params.At(4),
                    _comp_params.At(5))
                    )
        elif _comp["type"] == "RooCutCBShape":
            _components.Add(RooCutCBShape(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3),
                    _comp_params.At(4))
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
        elif _comp["type"] == "RooIpatia2":
            _components.Add(RooIpatia2(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3),
                    _comp_params.At(4),
                    _comp_params.At(5),
                    _comp_params.At(6),
                    _comp_params.At(7),
                    _comp_params.At(8))
                    )
        elif _comp["type"] == "RooCassandra":
            _components.Add(RooCassandra(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3))
                    )
        elif _comp["type"] == "RooAsymCassandra":
            _components.Add(RooAsymCassandra(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3),
                    _comp_params.At(4))
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
        elif _comp["type"] == "RooExpGaussExp":
            _components.Add(RooExpGaussExp(
                    _name,
                    _comp["title"],
                    fit.mass,
                    _comp_params.At(0),
                    _comp_params.At(1),
                    _comp_params.At(2),
                    _comp_params.At(3))
                    )
        elif _comp["type"] == "RooHistPdf":
            if 'config' not in _comp:
                raise Exception("Missing 'config' section for RooHistPdf.")
            if 'file' not in _comp['config']:
                raise Exception("Missing 'file' key in the 'config' section for RooHistPdf.")
            if 'name' not in _comp['config']:
                raise Exception("Missing 'name' key in the 'config' section for RooHistPdf.")
            _f = TFile(_comp['config']['file'])
            _pdf = _f.Get(_comp['config']['name'])
            _components.Add(_pdf)
        elif _comp["type"] == "Conv_RooDecay_RooBreitWigner":
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
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooDecay_RooBreitWigner":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
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
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooDecay_RooCBShape":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
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
            _f2 = RooCBShape(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(3),
                _comp_params.At(4),
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _funcs.Add(_r1)
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooCassandra_RooGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _f1 = RooCassandra(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(3),
                _comp_params.At(4)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooAsymCassandra_RooGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _f1 = RooAsymCassandra(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3),
                _comp_params.At(4)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            #_pdf = RooFFTConvPdf(
            #    _name,
            #    _comp["title"],
            #    fit.mass,
            #    _f1,
            #    _f2
            #    )
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f2,
                _f1
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooAsymCassandra_RooDoubleGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _v = RooFormulaVar(
                    _name+"_variable",
                    "",
                    "@0*@1",
                    RooArgList(
                        _comp_params.At(6),
                        _comp_params.At(7))
                    )
            _funcs.Add(_v)
            _f1 = RooAsymCassandra(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3),
                _comp_params.At(4)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _f3 = RooGaussian(
                _name+"_comp_3",
                _comp["title"]+"_comp_3",
                fit.mass,
                _comp_params.At(5),
                _v
                )
            _f4 = RooAddPdf(
               _name,
               _comp["title"],
               RooArgList(_f2, _f3),
               RooArgList(_comp_params.At(8))
               )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _funcs.Add(_f3)
            _funcs.Add(_f4)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f4,
                _f1
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "Conv_RooAsymCassandra_RooGaussian":
            _f1 = RooAsymCassandra(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3),
                _comp_params.At(4)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooNumConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f2,
                _f1
                )
            # Does not work
            #_pdf.setConvolutionWindow(_comp_params.At(5), _comp_params.At(6), 6.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooAsymCassandra3_RooGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _f1 = RooAsymCassandra3(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3),
                _comp_params.At(4),
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(7),
                _comp_params.At(8)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f2,
                _f1
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooCassandra_RooCBShape":
            # Define sampling frequency
            fit.mass.setBins(50000,"fft") ;
            _f1 = RooCassandra(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2)
                )
            _f2 = RooCBShape(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(3),
                _comp_params.At(4),
                _comp_params.At(5),
                _comp_params.At(6)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(100.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooCassandra3_RooCBShape":
            # Define sampling frequency
            fit.mass.setBins(50000,"fft") ;
            _f1 = RooCassandra3(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3)
                )
            _f2 = RooCBShape(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(4),
                _comp_params.At(5),
                _comp_params.At(6),
                _comp_params.At(7)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(100.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooHistPdf_RooGaussian":
            # Load reference ROOT tree
            _ref_full_ch = TChain(_comp['config']['tree'])
            _ref_full_ch.Add(_comp['config']['files'])
            # Run selection
            _ref_ch = _ref_full_ch.CopyTree(_comp['config']['selection'])
            _bins = fit.mass.getBins()
            fit.mass.setBins(_comp['config']['bins'])
            # Create hist PDF
            _data = RooDataSet(
                    'ds_' + _name,
                    '',
                    RooArgSet(fit.mass),
                    RooFit.Import(_ref_ch)
                    )
            _hist = _data.binnedClone()
            _f1 = RooHistPdf(
                    name+'_histpdf',
                    _comp['title']+'_histpdf',
                    RooArgSet(fit.mass),
                    _hist,
                    2  # Order of interpolation function
                    )
            fit.mass.setBins(_bins)

            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _f2 = RooGaussModel(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1)
                )
            _funcs.Add(_hist)
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f2,
                _f1
                )
            _pdf.setBufferFraction(5.0)
            _components.Add(_pdf)
        elif _comp["type"] == "Conv_RooHistPdf_RooGaussian":
            if 'config' not in _comp:
                raise Exception("Missing 'config' section for RooHistPdf.")
            if 'file' not in _comp['config']:
                raise Exception("Missing 'file' key in the 'config' section for RooHistPdf.")
            if 'name' not in _comp['config']:
                raise Exception("Missing 'name' key in the 'config' section for RooHistPdf.")
            _file = TFile(_comp['config']['file'])
            _f1 = _file.Get(_comp['config']['name'])
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooNumConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f2,
                _f1
                )
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_RooDoubleCBShape_RooGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _f1 = RooDoubleCBShape(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3),
                _comp_params.At(4),
                _comp_params.At(5)
                )
            _f2 = RooGaussian(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(6),
                _comp_params.At(7)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f1,
                _f2
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        elif _comp["type"] == "FFT_2xCBShape_RooGaussian":
            # Define sampling frequency
            fit.mass.setBins(10000,"fft") ;
            _cb_frac = RooRealVar(
                    _name+"_cb_frac",
                    _name+"_cb_frac",
                    0.1, 0.0, 1.0)
            _f1 = RooCBShape(
                _name+"_comp_1",
                _comp["title"]+"_comp_1",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(2),
                _comp_params.At(3)
                )
            _f2 = RooCBShape(
                _name+"_comp_2",
                _comp["title"]+"_comp_2",
                fit.mass,
                _comp_params.At(0),
                _comp_params.At(1),
                _comp_params.At(4),
                _comp_params.At(5)
                )
            _f3 = RooAddPdf(
                _name+"_comp_3",
                _comp["title"]+"_comp_3",
                RooArgList(_f1, _f2),
                RooArgList(_cb_frac),
                kTRUE)
            _f4 = RooGaussian(
                _name+"_comp_4",
                _comp["title"]+"_comp_4",
                fit.mass,
                _comp_params.At(6),
                _comp_params.At(7)
                )
            _funcs.Add(_f1)
            _funcs.Add(_f2)
            _funcs.Add(_f3)
            _funcs.Add(_f4)
            _funcs.Add(_cb_frac)
            _pdf = RooFFTConvPdf(
                _name,
                _comp["title"],
                fit.mass,
                _f3,
                _f4
                )
            _pdf.setBufferFraction(50.0)
            _components.Add(_pdf)
        else:
            raise Exception("Unknown PDF type: "+_comp["type"])

        if _i < _n_comps:
            _f_v = 0.1
            _f_min = 0.0
            _f_max = 1.0
            if "fraction" in _comp:
                _f_v = _comp["fraction"]["value"]
                _f_min = _comp["fraction"]["min"]
                _f_max = _comp["fraction"]["max"]
            _f = RooRealVar(
                    prefix+"_frac_"+_comp["name"],
                    prefix+" component "+_comp["name"]+" fraction",
                    _f_v, _f_min, _f_max)
            if "fraction" in _comp and "constraint" in _comp["fraction"]:
                _constr = RooGaussian(
                        _name+"_frac_constr",
                        _name+"_frac_constr",
                        _f,
                        RooFit.RooConst(_comp["fraction"]["constraint"]["mean"]),
                        RooFit.RooConst(_comp["fraction"]["constraint"]["sigma"])
                        )
                fit.constraints.push_back(_constr)
                _comp_constrs.Add(_constr)
            _fracs.Add(_f)

        _i += 1

    if _n_comps == 1:
        _pdf = _components.At(0)
    else:
        # Make sure we are creating a recursive composit PDF
        _pdf = RooAddPdf(
            prefix+"_pdf",
            prefix+" PDF",
            RooArgList(_components),
            RooArgList(_fracs),
            kTRUE)

    _components_all.AddAll(_components)
    return (_pdf, _components_all, _params)

# Generate fit instances
for fpars in fit_params:
    fit = MassFit()

    fit.name = str(fpars["name"])
    fit.title = str(fpars["title"])
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
    for obs in fpars["observables"]:
        obs_var = RooRealVar(
            obs["name"],
            obs["title"],
            obs["min"],
            obs["max"],
            obs["unit"],
        )
        fit.control_variables.push_back(obs_var)
        fit.control_names.push_back(str(obs["var_name"]))
        fit.control_titles.push_back(str(obs["title"]))
        obs_func = None
        if "function" in obs:
            obs_func = TF1(obs["funcion"]["name"], obs["function"]["value"],
                           obs["function"]["min"], obs["function"]["max"])
        fit.control_functions.push_back(obs_func)
    if "plot_min" in fpars["mass"]:
        fit.mass_plot_min = fpars["mass"]["plot_min"]
    if "plot_max" in fpars["mass"]:
        fit.mass_plot_max = fpars["mass"]["plot_max"]
    fit.single_candidate = fpars["single_candidate"]
    fit.bins = fpars["bins"]
    fit.bins_pull = fpars["bins_pull"]
    if "run_sfit" in fpars:
        fit.run_sfit = fpars["run_sfit"]
    if "save_output" in fpars:
        fit.save_output = fpars["save_output"]
    if "ncpu" in fpars:
        fit.ncpu = fpars["ncpu"]
    if "control" in fpars:
        cfunctions = {
                "identity": TF1(
                    "identity",
                    "x",
                    -999.0,
                    999.0
                    )
                }
        for func_pars in fpars["control"]["functions"]:
            cfunc = TF1(
                    func_pars["name"],
                    func_pars["function"],
                    func_pars["min"],
                    func_pars["max"]
                    )
            cfunctions[func_pars["name"]] = cfunc
        for control in fpars["control"]["variables"]:
            cvar = RooRealVar(
                    control["name"],
                    control["title"],
                    control["value"],
                    control["min"],
                    control["max"] )
            fit.control_variables.push_back(cvar)
            fit.control_names.push_back(str(control["var_name"]))
            fit.control_titles.push_back(str(control["title"]))
            if "function" in control and control["function"] is not None:
                fit.control_functions.push_back(cfunctions[control["function"]])
            else:
                fit.control_functions.push_back(cfunctions["identity"])
    fit.init()
    all_params = {}

    sig_yield = fpars["signal"]["yield"]
    nsig = RooRealVar( str(sig_yield["name"]), str(sig_yield["title"]),
            sig_yield["value"], sig_yield["min"], sig_yield["max"])
    fit.sig_n = nsig
    all_params[sig_yield["name"]] = nsig
    prefix = "Sig"
    if len(fit_params) > 1:
        prefix = fit.name + "_" + prefix
    (sig_pdf, sig_components, sig_params) = init_pdfs(fit, fpars["signal"], prefix, True)
    all_params.update(sig_params)

    bkg_yield = fpars["background"]["yield"]
    nbkg = RooRealVar( bkg_yield["name"], bkg_yield["title"],
            bkg_yield["value"], bkg_yield["min"], bkg_yield["max"])
    all_params[bkg_yield["name"]] = nbkg
    fit.bkg_n = nbkg
    prefix = "Bg"
    if len(fit_params) > 1:
        prefix = fit.name + "_" + prefix
    (bkg_pdf, bkg_components, bkg_params) = init_pdfs(fit, fpars["background"], prefix, False)
    all_params.update(bkg_params)

    nextra = None
    if 'extra' in fpars:
        prefix = "Ext"
        if len(fit_params) > 1:
            prefix = fit.name + "_" + prefix
        (extra_pdf, extra_components, extra_params) = init_pdfs(fit, fpars["extra"], prefix, False)
        extra_yield = fpars["extra"]["yield"]
        if 'params' in extra_yield:
            _extra_params = TList()
            _extra_constrs = TList()
            for _par in extra_yield["params"]:
                _name = _par["name"]
                if _name in all_params:
                    _extra_params.append(all_params[_name])
                else:
                    _var = RooRealVar(
                            _name, _par["title"], _par["value"],
                            _par["min"], _par["max"], _par["units"]
                            )
                    fit.parameters.push_back(_var)
                    _extra_params.Add(_var)
                    all_params[_name] = _var
                    if "constraint" in _par:
                        _constr = RooGaussian(
                                _name+"_constr",
                                _par["title"]+"_constr",
                                _var,
                                RooFit.RooConst(_par["constraint"]["mean"]),
                                RooFit.RooConst(_par["constraint"]["sigma"])
                                )
                        fit.constraints.push_back(_constr)
                        _extra_constrs.Add(_constr)

            nextra = RooFormulaVar(
                    extra_yield["name"],
                    extra_yield["title"],
                    extra_yield["formula"],
                    RooArgList(_extra_params))
        else:
            nextra = RooRealVar( extra_yield["name"], extra_yield["title"],
                    extra_yield["value"], extra_yield["min"], extra_yield["max"])

    if nextra is None:
        pdf = RooAddPdf(
                "MassPdf^{"+fit.name+"}",
                "Mass PDF",
                RooArgList(sig_pdf, bkg_pdf),
                RooArgList(nsig, nbkg) )
    else:
        pdf = RooAddPdf(
                "MassPdf^{"+fit.name+"}",
                "Mass PDF",
                RooArgList(sig_pdf, bkg_pdf, extra_pdf),
                RooArgList(nsig, nbkg, nextra) )
    fit.sig_pdf = sig_pdf
    fit.bkg_pdf = bkg_pdf
    for x in range(sig_components.GetSize()):
        fit.sig_components.push_back(sig_components.At(x))
    for x in range(bkg_components.GetSize()):
        fit.bkg_components.push_back(bkg_components.At(x))
    if nextra is not None:
        for x in range(extra_components.GetSize()):
            fit.bkg_components.push_back(extra_components.At(x))
    fit.fit_pdf = pdf

    fit_list.push_back(fit)
    if args.plot:
        frame = fit.mass.frame(RooFit.Title(fit.title))
        fit.sig_pdf.plotOn(frame)
        frame.Draw()
        fit.bkg_pdf.plotOn(frame, RooFit.LineColor(kRed))
        frame.Draw("same")
    if args.load:
        fit.load()
        fit.plot()

if args.plot:
    raw_input("Press Enter to continue...")
    sys.exit()

# Store actual fit params
# Store actual fit setup
with open("FitRandomized.json", "w") as f:
    json.dump(fit_params, f)


if not args.load:
    ch.MakeProxy("decayFit_proxy", os.path.join(script_dir, "decayFit.C"), "", "nohist")
    ch.Process("decayFit_proxy.h+");

print "Done"
print("--- %s seconds ---" % (time.time() - start_time))
raw_input("Press Enter to continue...")

