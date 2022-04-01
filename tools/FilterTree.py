#!/usr/bin/env python3

import argparse
from ROOT import TFile, TChain

parser = argparse.ArgumentParser(
    description='Program that filters a ROOT tree with selected cut.')
parser.add_argument('-c', '--cut', dest='cut', type=str, required=True,
                    help='Cut used to filter events')
parser.add_argument('-t', '--tree', dest='tree', type=str, required=True,
                     help='Name of the tree')
parser.add_argument('-o', '--output', dest='output', type=str, required=True,
                    help='Output file')
parser.add_argument('input', metavar='F', type=str, nargs='+',
                    help='Input ROOT files')
args = parser.parse_args()

ch = TChain(args.tree)
for name in args.input:
    ch.Add(name)

out = TFile(args.output,"recreate")

filtered = ch.CopyTree(args.cut)
filtered.Write()
out.Close()
