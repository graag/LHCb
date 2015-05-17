# decayFit

A pyROOT script for fitting mass spectra of resonance decays using RooFit. Performs sPlot fit and calculates FoM values.

## Usage

Create your own setup based on decayFitSetup.C. The setup is written in C++ and will be compiled using ACLiC on execution.

Execute using decayFit.py:

```
decayFit.py [-h/--help] <setup> <tree> tree.root []

<setup> - name of setup file: e.g. decayFitSetup.C
<tree> - tree name, can contain path: e.g. TupleBs/DecayTree
tree.root - input ROOT files
```

For each defined fit eps files will be generated for mass spectrum and all defined control_variables.
