#ifndef decayFitSetup_C
#define decayFitSetup_C
#include "decayFit.h"

void decayFitSetup() {

MassFit* fit = NULL;

default_selection =
     "eplus_PT > 700 && eminus_PT > 700 &&" \
     "eplus_PIDe > 1 && eminus_PIDe > 1 &&" \
     "eminus_TRACK_CHI2NDOF<4 && eplus_TRACK_CHI2NDOF<4 &&" \
     "eminus_IPCHI2_OWNPV>10 && eplus_IPCHI2_OWNPV>10 &&" \
     "2500 < Jpsi_M && Jpsi_M < 3300 &&" \
     "Jpsi_PT>2000 &&" \
     "Jpsi_ENDVERTEX_CHI2/Jpsi_ENDVERTEX_NDOF<16 &&" \
     "Kplus_PIDK > 0 && Kminus_PIDK > 0 &&" \
     "Kminus_TRACK_CHI2NDOF<4 && Kplus_TRACK_CHI2NDOF<4 &&" \
     "phi_ENDVERTEX_CHI2/phi_ENDVERTEX_NDOF<16 &&" \
     "phi_PT > 1000 &&" \
     "990 < phi_M && phi_M < 1050 &&" \
     "Bs_ENDVERTEX_CHI2/Bs_ENDVERTEX_NDOF<10 &&" \
     "Bs_IPCHI2_OWNPV < 30";

//     "eminus_IPCHI2_OWNPV>22 && eplus_IPCHI2_OWNPV>22 &&" \
//     "Bs_IPCHI2_OWNPV < 25";
/*
default_selection =
     "2500 < Jpsi_M && Jpsi_M < 3300 &&" \
     "Jpsi_PT>2000 &&" \
     "Bs_ENDVERTEX_CHI2/Bs_ENDVERTEX_NDOF<10 &&" \
     "Bs_IPCHI2_OWNPV < 25 &&" \
     "Bs_DIRA_OWNPV > 0.99";
*/

/*
default_selection =
     "eminus_IPCHI2_OWNPV>4 && eplus_IPCHI2_OWNPV>4 &&" \
     "Jpsi_PT>1500 &&" \
     "Bs_IPCHI2_OWNPV < 50 &&" \
     "Bs_MM > 4700";
*/

// J/Psi

// Init
MassFit* jpsi_fit = new MassFit();
fit_list.push_back(jpsi_fit);
fit = jpsi_fit;

// Main settings
fit->name = "J/#psi";
fit->mass_name = "Jpsi_MM";
fit->mass_err_name = "Jpsi_MMERR";
fit->mass_title = "M(e^{+}e^{-}) [MeV/c^{2}]";
fit->mass_range_min = 2500;
fit->mass_range_max = 3300;
fit->sigma_range_min = 1;
fit->sigma_range_max = 300;
fit->init();

// PDF params
// CB
RooRealVar *jpsi_M        = new RooRealVar(
        "M",
        "Signal mass",
        3050,
        fit->mass_range_min,
        fit->mass_range_max,
        "MeV/c^{2}" );
RooRealVar *jpsi_SM       = new RooRealVar(
        "#sigma_{M}",
        "Signal mass resolution",
        56,
        fit->sigma_range_min,
        fit->sigma_range_max,
        "MeV/c^{2}");
RooRealVar *jpsi_CB_alpha = new RooRealVar(
        "#alpha_{CB}",
        "CB alpha",
        0.6,
        0.1,
        10.0);
RooRealVar *jpsi_CB_n     = new RooRealVar(
        "n_{CB}",
        "CB n",
        5.0,
        0.1,
        100.0);
// Linear background
RooRealVar *jpsi_BG_c = new RooRealVar(
        "c0_{bg}",
        "Slope of mass bkg",
        5e-02,
        -1.0,
        1.0);
// Signal fraction
RooRealVar *jpsi_nsig = new RooRealVar(
        "N_{Sig}",
        "number of sig events",
        200,
        0,
        1.0e+04);
RooRealVar *jpsi_nbkg = new RooRealVar(
        "N_{Bkg}",
        "number of bkg events",
        100,
        0,
        1.0e+04);

fit->parameters.push_back(jpsi_M);
fit->parameters.push_back(jpsi_SM);
fit->parameters.push_back(jpsi_CB_alpha);
fit->parameters.push_back(jpsi_CB_n);
fit->parameters.push_back(jpsi_BG_c);
fit->sig_n = jpsi_nsig;
fit->bkg_n = jpsi_nbkg;
fit->sig_mass = jpsi_M;
fit->sig_sigma = jpsi_SM;

// PDFs
RooAbsPdf *jpsi_sig_pdf = new RooCBShape(
        (string("SigMPdf^{")+fit->name+"}").c_str(),
        "SigMPdf",
        *(fit->mass),
        *jpsi_M,
        *jpsi_SM,
        *jpsi_CB_alpha,
        *jpsi_CB_n);
RooAbsPdf *jpsi_bkg_pdf = new RooPolynomial(
        (string("BkgMPdf^{")+fit->name+"}").c_str(),
        "BkgMPdf",
        *(fit->mass),
        *jpsi_BG_c);
RooAbsPdf *jpsi_pdf = new RooAddPdf(
        (string("MassPdf^{")+fit->name+"}").c_str(),
        "Mass PDF",
        RooArgList(*jpsi_sig_pdf,*jpsi_bkg_pdf),
        RooArgList( *jpsi_nsig, *jpsi_nbkg ) );
fit->sig_pdf = jpsi_sig_pdf;
fit->sig_pdf_plot = jpsi_sig_pdf;
fit->bkg_pdf = jpsi_bkg_pdf;
fit->bkg_pdf_plot = jpsi_bkg_pdf;
fit->fit_pdf = jpsi_pdf;

// Phi

// Init
MassFit* phi_fit = new MassFit();
fit_list.push_back(phi_fit);
fit = phi_fit;

// Main settings
fit->name = "#phi";
fit->mass_name = "phi_MM";
fit->mass_err_name = "phi_MMERR";
fit->mass_title = "M(K^{+}K^{-}) [MeV/c^{2}]";
fit->mass_range_min = 990;
fit->mass_range_max = 1050;
fit->sigma_range_min = 1;
fit->sigma_range_max = 300;
fit->init();

// PDF params
// Gauss
RooRealVar *phi_M        = new RooRealVar(
        "M",
        "Signal mass",
        1020,
        fit->mass_range_min,
        fit->mass_range_max,
        "MeV/c^{2}" );
RooRealVar *phi_SM       = new RooRealVar(
        "#sigma_{M}",
        "Signal mass resolution",
        20,
        fit->sigma_range_min,
        fit->sigma_range_max,
        "MeV/c^{2}");
// Chebyshev background
RooRealVar *phi_BG_c0 = new RooRealVar(
        "c0_{bg}",
        "c0 parameter",
        0.4,
        -1.0,
        1.0);
RooRealVar *phi_BG_c1 = new RooRealVar(
        "c1_{bg}",
        "c1 parameter",
        7.0e-04,
        -0.001,
        0.001);
// Signal fraction
RooRealVar *phi_nsig = new RooRealVar(
        "N_{Sig}",
        "number of sig events",
        200,
        0,
        1.0e+04);
RooRealVar *phi_nbkg = new RooRealVar(
        "N_{Bkg}",
        "number of bkg events",
        100,
        0,
        1.0e+04);

fit->parameters.push_back(phi_M);
fit->parameters.push_back(phi_SM);
fit->parameters.push_back(phi_BG_c0);
fit->parameters.push_back(phi_BG_c1);
fit->sig_n = phi_nsig;
fit->bkg_n = phi_nbkg;
fit->sig_mass = phi_M;
fit->sig_sigma = phi_SM;

// PDFs
RooAbsPdf *phi_sig_pdf = new RooGaussian(
        (string("SigMPdf^{")+fit->name+"}").c_str(),
        "SigMPdf",
        *(fit->mass),
        *phi_M,
        *phi_SM);
RooAbsPdf *phi_bkg_pdf = new RooChebychev(
        (string("BkgMPdf^{")+fit->name+"}").c_str(),
        "BkgMPdf",
        *(fit->mass),
        RooArgList(*phi_BG_c0, *phi_BG_c1));
RooAbsPdf *phi_pdf = new RooAddPdf(
        (string("MassPdf^{")+fit->name+"}").c_str(),
        "Mass PDF",
        RooArgList(*phi_sig_pdf,*phi_bkg_pdf),
        RooArgList( *phi_nsig, *phi_nbkg ) );
fit->sig_pdf = phi_sig_pdf;
fit->sig_pdf_plot = phi_sig_pdf;
fit->bkg_pdf = phi_bkg_pdf;
fit->bkg_pdf_plot = phi_bkg_pdf;
fit->fit_pdf = phi_pdf;

// B0 raw

// Init
MassFit* Bs_fit = new MassFit();
fit_list.push_back(Bs_fit);
fit = Bs_fit;

// Main settings
fit->selection = "1010 < phi_M && phi_M < 1030 && 2700 < Jpsi_MM && Jpsi_MM < 3190 && Bs_MM>4600";
fit->name = "B^{0}_{s}";
fit->mass_name = "Bs_MM";
fit->mass_err_name = "Bs_MMERR";
fit->mass_title = "M(e^{+}e^{-}K^{+}K^{-}) [MeV/c^{2}]";
fit->mass_range_min = 4600;
fit->mass_range_max = 6000;
fit->sigma_range_min = 1;
fit->sigma_range_max = 60;
fit->single_candidate = false;
fit->init();

// PDF params
// Gauss
RooRealVar *Bs_M        = new RooRealVar(
        "M",
        "Signal mass",
        5370,
        5000,
        5700,
        "MeV/c^{2}" );
RooRealVar *Bs_SM       = new RooRealVar(
        "#sigma_{M}",
        "Signal mass resolution",
        50,
        fit->sigma_range_min,
        fit->sigma_range_max,
        "MeV/c^{2}");
RooRealVar *Bs_CB_alpha = new RooRealVar(
        "#alpha_{CB}",
        "CB alpha",
        0.6,
        0.1,
        10.0);
RooRealVar *Bs_CB_n     = new RooRealVar(
        "n_{CB}",
        "CB n",
        5.0,
        0.1,
        100.0);
// Exponent background
RooRealVar *Bs_BG_c0 = new RooRealVar(
        "c0_{bg}",
        "c0 parameter",
        -0.1,
        -5.0,
        0.0);
// Signal fraction
RooRealVar *Bs_nsig = new RooRealVar(
        "N_{Sig}",
        "number of sig events",
        200,
        0,
        1.0e+10);
RooRealVar *Bs_nbkg = new RooRealVar(
        "N_{Bkg}",
        "number of bkg events",
        100,
        0,
        1.0e+10);

fit->parameters.push_back(Bs_M);
fit->parameters.push_back(Bs_SM);
fit->parameters.push_back(Bs_BG_c0);
fit->parameters.push_back(Bs_CB_n);
fit->parameters.push_back(Bs_CB_alpha);
fit->sig_n = Bs_nsig;
fit->bkg_n = Bs_nbkg;
fit->sig_mass = Bs_M;
fit->sig_sigma = Bs_SM;

// PDFs
RooAbsPdf *Bs_sig_pdf = new RooCBShape(
        (string("SigMPdf^{")+fit->name+"}").c_str(),
        "SigMPdf",
        *(fit->mass),
        *Bs_M,
        *Bs_SM,
        *Bs_CB_alpha,
        *Bs_CB_n);
RooAbsPdf *Bs_bkg_pdf = new RooExponential(
        (string("BkgMPdf^{")+fit->name+"}").c_str(),
        "BkgMPdf",
        *(fit->mass),
        *Bs_BG_c0);
RooAbsPdf *Bs_pdf = new RooAddPdf(
        (string("MassPdf^{")+fit->name+"}").c_str(),
        "Mass PDF",
        RooArgList(*Bs_sig_pdf,*Bs_bkg_pdf),
        RooArgList( *Bs_nsig, *Bs_nbkg ) );
fit->sig_pdf = Bs_sig_pdf;
fit->sig_pdf_plot = Bs_sig_pdf;
fit->bkg_pdf = Bs_bkg_pdf;
fit->bkg_pdf_plot = Bs_bkg_pdf;
fit->fit_pdf = Bs_pdf;

// B0 DTF

// Init
MassFit* BsDTF_fit = new MassFit();
fit_list.push_back(BsDTF_fit);
fit = BsDTF_fit;

// Main settings
//fit->selection = "1010 < phi_M && phi_M > 1030 && 2700 < Jpsi_MM && Jpsi_MM < 3190 && Bs_PVFit_M[0]>4800";
fit->selection = "1010 < phi_M && phi_M < 1030 && 2700 < Jpsi_MM && Jpsi_MM < 3190 && best_M>4600 && best_M<6000 ";
fit->name = "B^{0}_{s_DTF}";
fit->mass_name = "best_M";
fit->mass_err_name = "best_MERR";
fit->mass_title = "M(J/#psi K^{+}K^{-}) [MeV/c^{2}]";
fit->mass_range_min = 4600;
fit->mass_range_max = 6000;
fit->sigma_range_min = 1;
fit->sigma_range_max = 300;
fit->single_candidate = false;
fit->bins = 150;

RooRealVar *BsDTF_ThetaK   = new RooRealVar(
        "cos#theta_{K}",
        "Cosine ThetaK",
        0,
        -1,
        1);
RooRealVar *BsDTF_ThetaL   = new RooRealVar(
        "cos#theta_{e}",
        "Cosine ThetaL",
        0,
        -1,
        1);
RooRealVar *BsDTF_PhiH   = new RooRealVar(
        "#phi_{h}",
        "Phi_h",
        0,
        -3.15,
        3.15,
        "rad");
RooRealVar *BsDTF_Tau   = new RooRealVar(
        "#tau",
        "Lifetime",
        0,
        0,
        15,
        "ps");
fit->control_variables.push_back(BsDTF_ThetaK);
fit->control_variables.push_back(BsDTF_ThetaL);
fit->control_variables.push_back(BsDTF_PhiH);
fit->control_variables.push_back(BsDTF_Tau);
fit->control_names.push_back(string("Bs_ThetaK"));
fit->control_names.push_back(string("Bs_ThetaL"));
fit->control_names.push_back(string("Bs_Phi"));
fit->control_names.push_back(string("Bs_PVFit_ctau"));
fit->control_titles.push_back(string("cos#theta_{K}"));
fit->control_titles.push_back(string("cos#theta_{e}"));
fit->control_titles.push_back(string("#phi_{h} [rad]"));
fit->control_titles.push_back(string("Decay time [ps]"));
TF1* _cos = new TF1("cos", "cos(x)", 0, 6.3);
float light_speed = 0.299792458; // mm/ps
TF1* _time = new TF1("time", "x/0.299792458",-100,100);
fit->control_functions.push_back(_cos);
fit->control_functions.push_back(_cos);
fit->control_functions.push_back(NULL);
fit->control_functions.push_back(_time);
fit->init();

// PDF params
// Gauss
RooRealVar *BsDTF_M        = new RooRealVar(
        "M",
        "Signal mass",
        5370,
        5000,
        5700,
        "MeV/c^{2}" );
RooRealVar *BsDTF_SM       = new RooRealVar(
        "#sigma_{M}",
        "Signal mass resolution",
        15,
        fit->sigma_range_min,
        fit->sigma_range_max,
        "MeV/c^{2}");
// Second Gauss
RooRealVar *BsDTF_SM2      = new RooRealVar(
        "#sigma_{M2}",
        "Signal mass resolution",
        50,
        fit->sigma_range_min,
        fit->sigma_range_max,
        "MeV/c^{2}");
// Exponent background
RooRealVar *BsDTF_BG_c0 = new RooRealVar(
        "c0_{bg}",
        "c0 parameter",
        -0.1,
        -5.0,
        0.0);
// Core fraction
RooRealVar *BsDTF_ncore = new RooRealVar(
        "F_{Core}",
        "Fraction of signal core events",
        0.9,
        0,
        1.0);
// Signal fraction
RooRealVar *BsDTF_nsig = new RooRealVar(
        "N_{Sig}",
        "number of sig events",
        200,
        0,
        1.0e+04);
RooRealVar *BsDTF_nbkg = new RooRealVar(
        "N_{Bkg}",
        "number of bkg events",
        100,
        0,
        1.0e+04);

fit->parameters.push_back(BsDTF_M);
fit->parameters.push_back(BsDTF_SM);
fit->parameters.push_back(BsDTF_SM2);
fit->parameters.push_back(BsDTF_BG_c0);
fit->parameters.push_back(BsDTF_ncore);
fit->sig_n = BsDTF_nsig;
fit->bkg_n = BsDTF_nbkg;
fit->sig_mass = BsDTF_M;
fit->sig_sigma = BsDTF_SM;

// PDFs
RooAbsPdf *BsDTF_core_pdf = new RooGaussian(
        (string("CoreMPdf^{")+fit->name+"}").c_str(),
        "SigMPdfCore",
        *(fit->mass),
        *BsDTF_M,
        *BsDTF_SM);
RooAbsPdf *BsDTF_wide_pdf = new RooGaussian(
        (string("WideMPdf^{")+fit->name+"}").c_str(),
        "SigMPdfTail",
        *(fit->mass),
        *BsDTF_M,
        *BsDTF_SM2);
RooAbsPdf *BsDTF_sig_pdf = new RooAddPdf(
        (string("SigMPdf^{")+fit->name+"}").c_str(),
        "SigMPdf",
        RooArgList(*BsDTF_core_pdf, *BsDTF_wide_pdf),
        *BsDTF_ncore);
RooAbsPdf *BsDTF_bkg_pdf = new RooExponential(
        (string("BkgMPdf^{")+fit->name+"}").c_str(),
        "BkgMPdf",
        *(fit->mass),
        *BsDTF_BG_c0);
RooAbsPdf *BsDTF_pdf = new RooAddPdf(
        (string("MassPdf^{")+fit->name+"}").c_str(),
        "Mass PDF",
        RooArgList(*BsDTF_sig_pdf,*BsDTF_bkg_pdf),
        RooArgList( *BsDTF_nsig, *BsDTF_nbkg ) );
fit->sig_pdf = BsDTF_sig_pdf;
fit->sig_pdf_plot = BsDTF_sig_pdf;
fit->bkg_pdf = BsDTF_bkg_pdf;
fit->bkg_pdf_plot = BsDTF_bkg_pdf;
fit->fit_pdf = BsDTF_pdf;
}
#endif // decayFitSetup_C
