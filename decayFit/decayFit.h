//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 28 22:28:44 2010 by ROOT version 5.27/03
// from TTree DecayTree/DecayTree
// found on file: /data/he/rootfiles/Jpsi2ee/Tuple_MagDown_10240448.root
//////////////////////////////////////////////////////////

#ifndef decayFit_h
#define decayFit_h

#include <TROOT.h>
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTreeFormula.h>
#include <TDataMember.h>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooStats/SPlot.h"

using namespace RooFit;
//using namespace RooStats;
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
using namespace std;

#include "decayFitOpts.h"

class MassFit
{
    public:
        MassFit(); // c-tor
        ~MassFit(); // d-tor

        string name; // Decay name
        string selection; // Selection string
        string mass_name; // Name of NTUPLE variable with mass value
        string mass_err_name; // Name of NTUPLE variable with mass error
        string mass_title; // Title of mass axis
        string weight_name; // Name of a variable to use as a weight
        Double_t mass_range_min; // Range of mass values to use in the fit
        Double_t mass_range_max; //
        Double_t sigma_range_min; // Range of error values to use in the fit
        Double_t sigma_range_max; //
        bool single_candidate;

        // PDFS
        RooAbsPdf* sig_pdf; // Signal PDF
        RooAbsPdf* sig_pdf_plot; // PDF to plot as signal
        RooAbsPdf* bkg_pdf; // Background PDF
        RooAbsPdf* bkg_pdf_plot; // PDF to plot as background
        RooAbsPdf* fit_pdf; // Full PDF
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
        RooRealVar* sigma; // Here we feed mass error for the event
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

        // Plots
        TStyle* style;
        TCanvas* canvas;
        Int_t bins; // Number of bins to use on the plot

        UInt_t run;
        ULong64_t event;

        // Functions
        void init();
        void fit();
        void sfit();
        void plot();
};

MassFit::MassFit()
{
    name = "";
    selection = "";
    mass_name = "";
    mass_err_name = "";
    mass_title = "M [MeV/c^{2}]";
    weight_name = "";
    mass_range_min = 0;
    mass_range_max = 0;
    sigma_range_min = 0;
    sigma_range_max = 0;
    single_candidate = false;

    run = 0;
    event = 0;

    // PDFS
    sig_pdf = NULL;
    sig_pdf_plot = NULL;
    bkg_pdf = NULL;
    bkg_pdf_plot = NULL;
    fit_pdf = NULL;
    // Fit parameters
    sig_n = NULL;
    bkg_n = NULL;
    sig_mass = NULL;
    sig_sigma = NULL;

    // Variables used to feed RooDataSet
    formula = NULL;
    mass = NULL;
    sigma = NULL;
    observables = NULL;
    data = NULL;
    weight = NULL;

    // Results
    fit_result = NULL;
    sfit_results = NULL;
    fom = 0;

    // Plots
    style = NULL;
    canvas = NULL;
    bins = 40;
}

MassFit::~MassFit()
{
    // PDFS
    if(sig_pdf != NULL)
        delete sig_pdf;
    if(sig_pdf_plot != NULL && sig_pdf_plot != sig_pdf)
        delete sig_pdf_plot;
    if(bkg_pdf != NULL)
        delete bkg_pdf;
    if(bkg_pdf_plot != NULL && bkg_pdf_plot != bkg_pdf)
        delete bkg_pdf_plot;
    if(fit_pdf != NULL)
        delete fit_pdf;
    // Fit parameters
    if(sig_n != NULL)
        delete sig_n;
    if(bkg_n != NULL)
        delete bkg_n;
    if(sig_mass != NULL)
        delete sig_mass;
    if(sig_sigma != NULL)
        delete sig_sigma;

    // Variables used to feed RooDataSet
    if(formula != NULL)
        delete formula;
    if(mass != NULL)
        delete mass;
    if(sigma != NULL)
        delete sigma;
    if(observables != NULL)
        delete observables;
    if(data != NULL)
        delete data;

    // Results
    if(fit_result != NULL)
        delete fit_result;
    if(sfit_results != NULL)
        delete sfit_results;

    // Plots
    if(style != NULL)
        delete style;
    if(canvas != NULL)
        delete canvas;

    // PDFS
    sig_pdf = NULL;
    sig_pdf_plot = NULL;
    bkg_pdf = NULL;
    bkg_pdf_plot = NULL;
    fit_pdf = NULL;
    // Fit parameters
    sig_n = NULL;
    bkg_n = NULL;
    sig_mass = NULL;
    sig_sigma = NULL;

    // Variables used to feed RooDataSet
    formula = NULL;
    mass = NULL;
    sigma = NULL;
    observables = NULL;
    data = NULL;

    // Results
    fit_result = NULL;
    sfit_results = NULL;

    // Plots
    style = NULL;
    canvas = NULL;
}

void MassFit::init()
{
    if(name.empty() || mass_name.empty() || mass_err_name.empty() ||
            mass_range_min == mass_range_max ||
            sigma_range_min == sigma_range_max)
        throw string("Data not defined.");

    mass  = new RooRealVar(
            (string("m_")+mass_name).c_str(),
            (string("M_{")+name+"}").c_str(),
            mass_range_min,
            mass_range_max,"MeV/c^{2}");
    sigma = new RooRealVar(
            (string("sm_")+mass_name).c_str(),
            "Estimated mass resolution",
            sigma_range_min,
            sigma_range_max,
            "MeV/c^{2}");
    weight = new RooRealVar(
            (string("w_")+mass_name).c_str(),
            "Weight",
            0, 1);

    // Inject additional variable to store weight values
    // The sPlot will erease the actual values so we have to preserve them
    if(!weight_name.empty()) {
        RooRealVar *w_observable = new RooRealVar(
                (string("wo_")+mass_name).c_str(),
                "Weight", 0, 1);
        control_variables.push_back(w_observable);
        control_names.push_back(weight_name);
        control_titles.push_back(string("Weight"));
        control_functions.push_back(NULL);
    }

    observables = new RooArgSet(*mass, *sigma, *weight);
    for(unsigned i=0; i<control_variables.size(); i++) {
        observables->add(*(control_variables[i]));
    }

    data = new RooDataSet(
            (string("data_")+mass_name).c_str(),
            (name+" data").c_str(),
            RooArgSet( *observables ),
            (string("w_")+mass_name).c_str());

    style = new TStyle(*gROOT->GetStyle("Plain"));
    style->SetName("Simple");
    //style->SetPaperSize(20,20);
    style->SetPadBottomMargin(0.14);
    //style->SetPadLeftMargin(0.14);
    style->SetNdivisions(505);
    style->SetTitleSize(0.05,"x");
    style->SetTitleSize(0.05,"y");
    style->SetLabelSize(0.04,"x");
    style->SetLabelSize(0.04,"y");
    style->SetTitleOffset(1.1,"x");
    style->SetTitleOffset(1.02,"y");
    //style->SetMarkerSize(1.5);
    style->SetHistLineWidth(3);
    // the pretty color palette of old
    style->SetPalette(1,0);
    style->SetOptFit(1111);
    style->SetOptStat(0);
}

void MassFit::fit()
{
    fit_result = fit_pdf->fitTo(
            *data,
            Extended(true),
//            Minimizer("Minuit2", "migrad"),
            Hesse(true),
            Save(true)
            );
}

void MassFit::sfit(){
    Double_t sumw = 0;
    Double_t sumw2 = 0;
    Double_t w = 0;

    // Set constans
    for(unsigned i=0; i<parameters.size(); i++)
        parameters[i]->setConstant();

    sfit_results = new RooStats::SPlot(
            (string("sData^{")+name+"}").c_str(),
            "An SPlot",
            *data,
            fit_pdf,
            RooArgList(*sig_n, *bkg_n));

    for(int i=0; i<sfit_results->GetSDataSet()->numEntries(); i++) {
        w = sfit_results->GetSWeight(i, sig_n->GetName());
        sumw += w;
        sumw2 += w*w;
    }
    fom = (sumw*sumw)/sumw2;
}

void MassFit::plot()
{
    style->cd();

    canvas = new TCanvas((string("Canvas ")+name).c_str(),"",600,480);

    RooPlot *mframe = mass->frame(Bins(bins));
    mframe->SetTitle(name.c_str());
    mframe->GetXaxis()->SetTitle(mass_title.c_str());
    mframe->GetYaxis()->SetNdivisions(505);

    // sPlot removes the weights assigned during DataSet filling.
    // Create a subset of the original data set with proper weights stored as
    // control variable during filling.
    RooDataSet * data_plot = data;
    if(!weight_name.empty())
        data_plot = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,(string("wo_")+mass_name).c_str()) ;

    data_plot->plotOn(mframe, Name("myData"));
    fit_pdf->plotOn(mframe, Name("myTot") );
    fit_pdf->plotOn(mframe, Name("mySig"), LineStyle(3), Components(*sig_pdf_plot), LineColor(kRed) );
    fit_pdf->plotOn(mframe, Name("myBkg"), Components(*bkg_pdf_plot), LineColor(kGreen), LineStyle(kDashed) );
    fit_pdf->paramOn(mframe, Layout(0.10,0.45,0.99), ShowConstants(true) );

    /*
       TLegend *leg = new TLegend(0.6,0.67,0.89,0.89);  
       leg->SetBorderSize(0);
       leg->SetFillColor(0);
       leg->SetTextSize(0.04);
       RooHist *rohData = dynamic_cast<RooHist*>(mframe->findObject("myData"));
       RooCurve *rocTot = dynamic_cast<RooCurve*>(mframe->findObject("myTot"));
       RooCurve *rocSig = dynamic_cast<RooCurve*>(mframe->findObject("mySig"));
       RooCurve *rocBkg = dynamic_cast<RooCurve*>(mframe->findObject("myBkg"));  
       leg->AddEntry(rohData,"Data","LPE");
       leg->AddEntry(rocTot, "Total","L");
       leg->AddEntry(rocSig, "Signal","L");
       leg->AddEntry(rocBkg, "Backgrounds","L");
       mframe->addObject(leg);
       */

    mframe->Draw();
    canvas->Print((mass_name + ".eps").c_str());
    delete mframe;

    // Extract a data set with sWeights. The sWeights are multipled by the
    // original weights (at least they should be ...)
    string w_name = string(sig_n->GetName()) + "_sw";
    RooDataSet * data_weight = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,w_name.c_str()) ;

    for(unsigned i=0; i<control_names.size(); i++) {
        RooPlot *mframe = control_variables[i]->frame(Bins(bins));
        mframe->SetTitle(name.c_str());
        mframe->GetXaxis()->SetTitle(control_titles[i].c_str());
        mframe->GetYaxis()->SetNdivisions(505);

        data_weight->plotOn(mframe, Name("myData"));
        mframe->Draw();
        canvas->Print((mass_name + string("_") + control_names[i] + ".eps").c_str());
        delete mframe;
    }
}

// Globals

TTreeFormula* formula = NULL;
vector<MassFit*> fit_list;
TString default_selection = "";

void decayFitSetup();

#include DECAY_SETUP
#endif // __CINT__

#if defined(__MAKECINT__)
#pragma link C++ class MassFit;
#endif

#endif // #ifdef decayFit_h
