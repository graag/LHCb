# decayFit

A pyROOT script for fitting mass spectra of resonance decays using RooFit. Performs sPlot fit and calculates FoM values.

## Usage

Create your own fit setup based on decayFitSetup.C. The setup is written in json. Most up to date list of supported PDFs can be found in the code of decayFit.py.
- RooGaussian
- RooPolynomial
- RooChebychev
- RooExponential
- RooVoigtian
- RooCBShape
- RooArgusBG
- RooRArgusBG
- RooIpatia
- RooDecay       (double sided exponential decay function)
- RooDecayLeft
- RooDecayRight
- RooBwDecay     (convolution of exponential decay function and Breit-Wigner)

Execute using decayFit.py:

```
decayFit.py [-h/--help] <setup> <tree> tree.root []

<setup> - name of setup file: e.g. Bs_SIG_2Gauss_BKG_Chebyshev.json
<tree> - tree name, can contain path: e.g. TupleBs/DecayTree
tree.root - input ROOT files
```

For each defined fit pdf files will be generated for mass spectrum and all defined control_variables.
