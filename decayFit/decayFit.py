#! /bin/env python

import os
import sys

if len(sys.argv) < 4:
    print """decayFit.py [-h/--help] <setup> <tree> tree.root []

<setup> - name of setup file: e.g. decayFitSetup.C
<tree> - tree name, can contain path: e.g. TupleBs/DecayTree
tree.root - input ROOT files
"""
    sys.exit(1)

setup = sys.argv[1]
path = sys.argv[2]
tree = os.path.basename(path)
jobs = sys.argv[3:]

print "Setup: " + setup
print "Tree: " + tree
print "Path in ROOT file: " + path
#print "Input files: %s" % jobs

sys.argv.append('-b')
import ROOT
from ROOT import gSystem, TChain, TF1, TH1D, TPaveLabel

script_dir = os.path.dirname(os.path.realpath(__file__))
with open("decayFitOpts.h", "w") as _opts:
    _opts.write("#define DECAY_PLOTS 1\n")
    _opts.write("#define DECAY_SETUP \"%s\"\n" % setup)

ch = TChain(tree)

i=0
for job in jobs:
    name = "%s/%s" %(job, path)
    #print "Adding: " + name
    ch.Add(name)

print "Will load #files: " + str(ch.GetListOfFiles().GetEntries())
print "Will load #entries: " + str(ch.GetEntries())
ch.MakeProxy("decayFit_proxy", os.path.join(script_dir, "decayFit.C"), "", "nohist")
gSystem.Load("libRooFit")
gSystem.Load("libRooStats")
ch.Process("decayFit_proxy.h++");

print "Done"

raw_input("Press Enter to continue...")

