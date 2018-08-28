#! /bin/env python

import os
import sys
import json
import time
import argparse

start_time = time.time()
# Parse parameters
parser = argparse.ArgumentParser(description='Generate RooHistPdfs and store them in a ROOT file.')
parser.add_argument('trees', metavar='TREE', nargs='?', action='append',
                    help='ROOT files with data tree.')
parser.add_argument('-s', '--setup', dest='setup', action='store', required=True,
                    help='Name of Json file with fit description.')
parser.add_argument('-n', '--tree-name', dest='tree_name', action='store', required=True,
                    help='Tree name with path inside the ROOT file.')
parser.add_argument('-o', '--output', dest='output', action='store',
                    default='output.root', help='Name of the output ROOT file.')

args = parser.parse_args()

setup = args.setup
data_files = args.trees
path = args.tree_name
tree = os.path.basename(path)
out_file = args.output


# Load fit setup
with open(setup) as f:
    params = json.load(f)

print "Setup: " + setup
print "Tree: " + tree
print "Path in ROOT file: " + path
print "Input files: %s" % data_files

# Setup ROOT
sys.argv.append('-b')
import ROOT
from ROOT import gSystem, gROOT, TChain, TFile

# Load data files
full_ch = TChain(tree)

i=0
for _f in data_files:
    name = "%s/%s" %(_f, path)
    print "Adding: " + name
    full_ch.Add(name)

print "Will load #files: " + str(full_ch.GetListOfFiles().GetEntries())
print "Will load #entries: " + str(full_ch.GetEntries())
gSystem.Load("libRooFit")
gSystem.Load("libRooStats")

# Compile and load the MassFit class
from ROOT import RooDataSet, RooHistPdf, RooFit, RooRealVar, RooArgSet

f_out = TFile(out_file, "RECREATE")

# Generate pdfs
for pars in params:
    if 'selection' in pars:
        ch = full_ch.CopyTree(pars['selection'])
    else:
        ch = full_ch
    x = RooRealVar(pars['var_name'], '', pars["min"], pars["max"])
    if 'bins' in pars:
        x.setBins(int(pars['bins']))
    else:
        x.setBins(100)
    data = RooDataSet('ds_' + pars['name'],'',RooArgSet(x),RooFit.Import(ch))
    hist = data.binnedClone()
    pdf = RooHistPdf(pars['name'],pars['title'],RooArgSet(x),hist,0)
    pdf.Write()

print "Done"
print("--- %s seconds ---" % (time.time() - start_time))
#raw_input("Press Enter to continue...")

