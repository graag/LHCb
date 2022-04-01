#ifndef massFit_h
#define massFit_h

#include <TROOT.h>
#include <TH2.h>
#include <TF1.h>
#include <TLine.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGaxis.h>
#include <TTreeFormula.h>
#include <TDataMember.h>
#include <TLegend.h>
#include <TTree.h>
#include <TFile.h>

#ifndef __CINT__
#include "RooWorkspace.h"
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooBreitWigner.h"
#include "RooVoigtian.h"
#include "RooPolynomial.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooStats/SPlot.h"
#include "RooPrintable.h"

#include "RooIpatia.h"
#include "RooIpatia2.h"
#include "RooCassandra.h"
#include "RooCassandra3.h"
#include "RooAsymCassandra.h"
#include "RooAsymCassandra3.h"
#include "RooExpGaussExp.h"
#include "RooDoubleCBShape.h"
#include "RooCutCBShape.h"

using namespace RooFit;
//using namespace RooStats;
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
using namespace std;

class MassFit
{
    public:
        MassFit(); // c-tor
        ~MassFit(); // d-tor

        // Options
        bool plot_params; // Should we draw fit params box on the plot
        bool plot_legend; // Should we draw a legend box on the plot
        bool single_candidate; // Accept only single candidate per event

        // Base fit params
        string name; // Decay name
        string selection; // Selection string
        string mass_name; // Name of NTUPLE variable with mass value
        string mass_err_name; // Name of NTUPLE variable with mass error
        string mass_title; // Title of mass axis
        string weight_name; // Name of a variable to use as a weight
        Double_t mass_range_min; // Range of mass values -
        Double_t mass_range_max; // default for the plot and the fit
        Double_t mass_fit_min; // Override fit mass range
        Double_t mass_fit_max;
        Double_t mass_plot_min; // Override plot mass range
        Double_t mass_plot_max;
        Double_t mass_logy_max;
        Double_t mass_logy_min;

        // PDFS
        RooAbsPdf* sig_pdf; // Signal PDF
        RooAbsPdf* bkg_pdf; // Background PDF
        RooAbsPdf* fit_pdf; // Full PDF
        vector<RooAbsPdf*> sig_components; // Components of signal PDF
        vector<RooAbsPdf*> bkg_components; // Components of background PDF
        vector<RooAbsPdf*> constraints; // Components of background PDF
        // Fit parameters
        RooRealVar* sig_n; // # Signal event
        RooRealVar* bkg_n; // # Background events
        RooRealVar* sig_mass; // Mass central value
        RooRealVar* sig_sigma; // Mass sigma
        RooRealVar* weight; // Event weight
        vector<RooRealVar*> parameters; // Parameters to set as const for sPlot

        // Variables used to feed RooDataSet
        TTreeFormula* formula; // Selection formula
        RooRealVar* mass; // Here we feed mass value for the event
        vector<RooRealVar*> control_variables; // Control observables not used in the fit
        vector<string> control_names; // Tree names of control observables
        vector<string> control_titles; // Plot titles for control observables
        vector<TF1*> control_functions; // Postprocess functions for control observables
        RooArgSet* observables; // Aggregate of mass, error
        RooDataSet* data; // Data set used in the fit

        // Results
        RooFitResult *fit_result; // Fit results
        RooStats::SPlot* sfit_results; // sPlot results
        Double_t fom; // FOM from splot
        Double_t fom_err; // FOM error from splot

        // Plots
        TStyle* style;
        TCanvas* canvas;
        Int_t bins; // Number of bins to use on the plot
        Int_t bins_pull; // Number of bins to use on the pullplot
        Int_t ncpu; // Number of CPU cores to use in the fit
        Bool_t run_sfit; // Execute sFit and calculate sWeights
        Bool_t save_output; // Store output data as ROOT file

        UInt_t run;
        ULong64_t event;

        // Functions
        void init();
        void fit();
        void sfit();
        void plot();
        void save();
        void load();
};
// Globals

extern TTreeFormula* formula;
extern vector<MassFit*> fit_list;
extern TString default_selection;
#endif // __CINT__

#endif // #ifdef massFit_h
