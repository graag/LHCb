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

- RooGaussian
- RooParamGaussian                       (gauss width with a scale factor)
- RooPolynomial
- RooChebychev
- RooExponential
- RooVoigtian
- RooCBShape
- RooArgusBG
- RooRArgusBG                            (reversed Argus shape)
- RooIpatia
- RooCassandra                           (exponential decay with two decay constants)
- RooDecay
- RooDecayLeft
- RooDecayRight
- FFT_RooDecay_RooBreitWigner            (FFT convolution of RooDecay and BreitWigner)
- Conv_RooDecay_RooBreitWigner           (Numerical convolution of RooDecay and BreitWigner)
- FFT_RooDecay_RooCBShape
- FFT_RooCassandra_RooGaussian
- FFT_RooAsymCassandra_RooGaussian       (FFT convolution of exponential decay with two asymmetric decay constants and a gauss)
- FFT_RooAsymCassandra_RooDoubleGaussian
- Conv_RooAsymCassandra_RooGaussian
- FFT_RooAsymCassandra3_RooGaussian
- FFT_RooCassandra_RooCBShape
- FFT_RooCassandra3_RooCBShape

Requires ROOT 6 and recent python 2.7.

Execute using decayFit.py:

```
usage: decayFit.py [-h] [--setup SETUP] [--list] TREE [TREE ...]

Fit decay distribution using predefined PDF.

positional arguments:
  TREE           ROOT files with data tree.

optional arguments:
  -h, --help     show this help message and exit
  --setup SETUP  Name of Json file with fit description.
  --list         List supported PDFs.
```

For each defined fit pdf files will be generated for mass spectrum and all defined control_variables.
