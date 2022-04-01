#include "massFit.h"

#ifndef __CINT__

MassFit::MassFit()
{
    plot_params = false;
    plot_legend = false;
    single_candidate = false;

    name = "";
    selection = "";
    mass_name = "";
    mass_err_name = "";
    mass_title = "M [MeV/c^{2}]";
    weight_name = "";
    mass_range_min = 0;
    mass_range_max = 0;
    mass_fit_min = 0;
    mass_fit_max = 0;
    mass_plot_min = 0;
    mass_plot_max = 0;
    mass_logy_max = 0;
    mass_logy_min = 0;

    run = 0;
    event = 0;

    // PDFS
    sig_pdf = NULL;
    bkg_pdf = NULL;
    fit_pdf = NULL;
    // Fit parameters
    sig_n = NULL;
    bkg_n = NULL;
    sig_mass = NULL;
    sig_sigma = NULL;

    // Variables used to feed RooDataSet
    formula = NULL;
    mass = NULL;
    observables = NULL;
    data = NULL;
    weight = NULL;

    // Results
    fit_result = NULL;
    sfit_results = NULL;
    fom = 0;
    fom_err = 0;

    // Plots
    style = NULL;
    canvas = NULL;
    bins = 40;
    bins_pull = 40;
    ncpu = 1;
    run_sfit = true;
    save_output = true;
}

MassFit::~MassFit()
{
    // PDFS
    if(sig_pdf != NULL)
        delete sig_pdf;
    if(bkg_pdf != NULL)
        delete bkg_pdf;
    if(fit_pdf != NULL)
        delete fit_pdf;
    /*
    vector<RooAbsPdf*>::iterator pdf_iter;
    for(pdf_iter = sig_components.begin(); pdf_iter != sig_components.end(); pdf_iter++) {
        if(*pdf_iter != NULL and *pdf_iter != sig_pdf)
            delete *pdf_iter;
    }
    sig_components.clear();
    for(pdf_iter = bkg_components.begin(); pdf_iter != bkg_components.end(); pdf_iter++) {
        if(*pdf_iter != NULL and *pdf_iter != sig_pdf)
            delete *pdf_iter;
    }
    bkg_components.clear();
    */

    // Fit parameters
    if(sig_n != NULL)
        delete sig_n;
    if(bkg_n != NULL)
        delete bkg_n;
    if(sig_mass != NULL)
        delete sig_mass;
    if(weight != NULL)
        delete weight;
    /*
    vector<RooRealVar*>::iterator var_iter;
    for(var_iter = parameters.begin(); var_iter != parameters.end(); var_iter++) {
        if(*var_iter != NULL)
            delete *var_iter;
    }
    parameters.clear();
    */

    // Variables used to feed RooDataSet
    if(formula != NULL)
        delete formula;
    if(mass != NULL)
        delete mass;
    if(observables != NULL)
        delete observables;
    if(data != NULL)
        delete data;
    /*
    for(var_iter = control_variables.begin(); var_iter != control_variables.end(); var_iter++) {
        if(*var_iter != NULL)
            delete *var_iter;
    }
    control_variables.clear();
    vector<TF1*>::iterator fun_iter;
    for(fun_iter = control_functions.begin(); fun_iter != control_functions.end(); fun_iter++) {
        if(*fun_iter != NULL)
            delete *fun_iter;
    }
    control_functions.clear();
    */

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
    bkg_pdf = NULL;
    fit_pdf = NULL;
    // Fit parameters
    sig_n = NULL;
    bkg_n = NULL;
    sig_mass = NULL;
    sig_sigma = NULL;

    // Variables used to feed RooDataSet
    formula = NULL;
    mass = NULL;
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
    if(name.empty() || mass_name.empty() || mass_range_min == mass_range_max)
        throw string("Data not defined.");

    mass  = new RooRealVar(
            mass_name.c_str(),
            (string("M_{")+name+"}").c_str(),
            mass_range_min,
            mass_range_max,"MeV/c^{2}");
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

    observables = new RooArgSet(*mass, *weight);
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
    //TODO Add option to disable title
    style->SetOptTitle(0);

}

void MassFit::fit()
{
    RooLinkedList param_list;
    RooCmdArg* arg;
    arg = new RooCmdArg(); *arg = Extended(true);
    param_list.Add(arg);
    arg = new RooCmdArg(); *arg = NumCPU(ncpu);
    param_list.Add(arg);
    arg = new RooCmdArg(); *arg = Timer(kTRUE);
    param_list.Add(arg);
    arg = new RooCmdArg(); *arg = Save(true);
    param_list.Add(arg);

    if(mass_fit_min != mass_fit_max) {
        cerr<<"Fit will be performed in range: ("<<mass_fit_min<<", "<<mass_fit_max<<")"<<endl;
        arg = new RooCmdArg(); *arg = Range(mass_fit_min,mass_fit_max);
        param_list.Add(arg);
    }
    if(constraints.size()) {
        cerr<<"Fit will be performed using external constraints:"<<endl;
        RooArgSet ext_constraints;
        for(unsigned i=0; i<constraints.size(); i++) {
            ext_constraints.add(*constraints[i]);
            cerr<<"- "<<constraints[i]->GetName()<<endl;
        }
        arg = new RooCmdArg(); *arg = ExternalConstraints(ext_constraints);
        param_list.Add(arg);
    }

    fit_result = fit_pdf->fitTo(
            *data,
            param_list
            );

    RooArgSet* params = fit_pdf->getParameters(*data);
    params->printLatex(OutputFile((mass_name + "_fit.tex").c_str()));
    ofstream param_outfile;
    param_outfile.open(mass_name + "_fit.csv");
    RooFIter it = params->fwdIterator();
    RooAbsArg *next = 0;
    bool first_par = 1;
    params->printValue(param_outfile);
    param_outfile<<endl;
    while((0 != (next= it.next()))) {
        if(!first_par) {
            param_outfile<<", ";
        } else {
            first_par = 0;
        }
        next->printStream(param_outfile,RooPrintable::kValue,RooPrintable::kInline,"");
    }
    param_outfile.close();
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
    fom_err = (2.*sumw)/sqrt(sumw2);
}

void MassFit::plot()
{
    style->cd();

    canvas = new TCanvas((string("Canvas ")+name).c_str());
    canvas->Divide(1,2,0.1,0.1);
    TPad* padHisto = (TPad*) canvas->cd(1);
    TPad* padPull = (TPad*) canvas->cd(2);
    double r = 0.25;
    double small = 0.1;
    padHisto->SetPad( 0., r , 1., 1. );
    padHisto->SetBottomMargin(0.0);
    padPull->SetPad( 0., 0., 1., r  );
    padPull->SetBottomMargin( 0.3  );
    padPull->SetTopMargin(0.02);
    padHisto->cd();

    double mass_lo = mass_range_min;
    double mass_hi = mass_range_max;
    if (mass_plot_min != mass_plot_max) {
        mass_lo = mass_plot_min;
        mass_hi = mass_plot_max;
    }
    //RooPlot *mframe = mass->frame(Bins(bins), Range(mass_lo, mass_hi));
    RooPlot *mframe = mass->frame(mass_lo, mass_hi, bins);
    mframe->SetTitle(name.c_str());
    mframe->GetXaxis()->SetTitle(mass_title.c_str());
    mframe->GetYaxis()->SetNdivisions(505);

    // sPlot removes the weights assigned during DataSet filling.
    // Create a subset of the original data set with proper weights stored as
    // control variable during filling.
    RooDataSet * data_plot = data;
    //if(!weight_name.empty())
    //    data_plot = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,(string("wo_")+mass_name).c_str()) ;

    data_plot->plotOn(mframe, Name("myData"), MarkerSize(0.1), Range(mass_lo, mass_hi));
    for(unsigned i=0; i<sig_components.size(); i++) {
        fit_pdf->plotOn(mframe, Name((string("mySig_")+std::to_string(i+1)).c_str()), LineStyle(7), LineWidth(3), Components(*(sig_components[i])), LineColor(kRed+i), Range(mass_lo, mass_hi) );
    }
    for(unsigned i=0; i<bkg_components.size(); i++) {
        fit_pdf->plotOn(mframe, Name((string("myBkg_")+std::to_string(i+1)).c_str()), Components(*(bkg_components[i])), LineColor(kGreen+i), LineStyle(kDashed), LineWidth(3), Range(mass_lo, mass_hi) );
    }
    fit_pdf->plotOn(mframe, Name("myTot"), Range(mass_lo, mass_hi) );
    if(this->plot_params) fit_pdf->paramOn(mframe, Layout(0.10,0.45,0.99), ShowConstants(true) );
    mframe->SetMinimum(0.1);

    if(this->plot_legend) {
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
    }

    mframe->Draw();

    canvas->cd(2);
    RooHist* hpull = mframe->pullHist();
    //TODO Add option to disable title
    RooPlot* mframe2 = mass->frame(Title("Pull"),Bins(bins_pull));
    //TODO Add option to select style??
    mframe2->addPlotable(hpull,"E3");
    mframe2->GetXaxis()->SetTitle(mass_title.c_str());
    mframe2->GetYaxis()->SetRangeUser(-4.5, 4.5);
    mframe2->SetTitleSize(0.1,"x");
    mframe2->SetTitleSize(0.1,"y");
    mframe2->SetLabelSize(0.1,"x");
    mframe2->SetLabelSize(0.1,"y");
    mframe2->Draw();
    TLine* l_centr = new TLine(mass_range_min+(mass_range_max-mass_range_min)/50.0,0,mass_range_max-(mass_range_max-mass_range_min)/50.0,0);
    l_centr->SetLineColor(2);
    l_centr->SetLineWidth(3);
    l_centr->Draw("same");
    //TLine* l_sigma1 = new TLine(mass_range_min,-3,mass_range_max,-3);
    //l_sigma1->SetLineColor(2);
    //TLine* l_sigma2 = new TLine(mass_range_min,3,mass_range_max,3);
    //l_sigma2->SetLineColor(2);
//    if ((hpull->Integral())<-5. || (hpull->Integral())>5.){
//    if ((hpull->GetMinimum())<-5. || (hpull->GetMaximum())>5.){
       //l_sigma1->Draw("same");
       //l_sigma2->Draw("same");
//    }

    canvas->Print((mass_name + ".pdf").c_str());

    // Draw log plot

    canvas->cd(1);
    gPad->SetLogy();
    mframe->Draw();
    canvas->Print((mass_name + "_log.pdf").c_str());
    delete mframe;
    delete mframe2;

    if(run_sfit) {
        TCanvas* canvas2 = new TCanvas((string("Canvas2 ")+name).c_str());
        // Extract a data set with sWeights. The sWeights are multipled by the
        // original weights (at least they should be ...)
        string w_name = string(sig_n->GetName()) + "_sw";
        string bgw_name = string(bkg_n->GetName()) + "_sw";
        // Test of Workspace. Import data to workspace to extract sweight variables
        RooWorkspace* wspace = new RooWorkspace("myWS");
        wspace->import(*data);
        // Plot signal weight
        RooRealVar* sig_sw = wspace->var(w_name.c_str());
        cout<<sig_sw<<endl;
        sig_sw->SetTitle("Signal sWeight");
        sig_sw->setRange(-2.0, 2.0);
        RooPlot* frame_sigsw = sig_sw->frame(Title("Signal sWeights"));
        data->plotOn(frame_sigsw, Range(mass_lo, mass_hi));
        frame_sigsw->Draw();
        canvas->Print((mass_name + string("_signal_weight.pdf")).c_str());
        // Plot bkg weight
        RooRealVar* bkg_sw = wspace->var(bgw_name.c_str());
        bkg_sw->SetTitle("Background sWeight");
        bkg_sw->setRange(-2.0, 2.0);
        RooPlot* frame_bkgsw = bkg_sw->frame(Title("Background sWeights"));
        data->plotOn(frame_sigsw, Range(mass_lo, mass_hi));
        frame_bkgsw->Draw();
        canvas->Print((mass_name + string("_background_weight.pdf")).c_str());

        RooDataSet * data_weight = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,w_name.c_str()) ;
        for(unsigned i=0; i<control_names.size(); i++) {
            RooPlot *mframe = control_variables[i]->frame(Bins(bins));
            mframe->SetTitle(name.c_str());
            mframe->GetXaxis()->SetTitle(control_titles[i].c_str());
            mframe->GetYaxis()->SetNdivisions(505);

            data_weight->plotOn(mframe, Name("myData"));
            mframe->Draw();
            canvas->Print((mass_name + string("_") + control_names[i] + ".pdf").c_str());
            delete mframe;
        }
    }
}

void MassFit::save()
{
    TFile* out_file = new TFile((mass_name+"_data.root").c_str(),"recreate");
    RooWorkspace w("decayFit") ;
    //w.import(*mass);
    w.import(*fit_pdf);
    w.import(*data);
    //w.import(*sig_pdf);
    //w.import(*bkg_pdf);
    //w.import(*fit_result);
    //w.import(*sfit_results);
    //w.import(*sig_n);
    //w.import(*bkg_n);
    //for(unsigned i=0; i<sig_components.size(); i++) {
    //    w.import(*(sig_components[i]));
    //}
    //for(unsigned i=0; i<bkg_components.size(); i++) {
    //    w.import(*(bkg_components[i]));
    //}
    //for(unsigned i=0; i<control_variables.size(); i++) {
    //    w.import(*(control_variables[i]));
    //}
    w.Write();
    TTree* t = data->GetClonedTree();
    t->Write();
    out_file->Save();
    out_file->Close();
    delete out_file;
}

void MassFit::load()
{
    cout<<"LOAD"<<endl;
    TFile* in_file = new TFile((mass_name+"_data.root").c_str(),"read");
    RooWorkspace* w = (RooWorkspace*) in_file->Get("decayFit");
    cout<<"LOAD wspace"<<endl;
    mass = w->var(mass->GetName());
    cout<<"LOAD mass"<<endl;
    data = (RooDataSet*) w->data(data->GetName());
    cout<<"LOAD data"<<endl;
    sig_pdf = w->pdf(sig_pdf->GetName());
    bkg_pdf = w->pdf(bkg_pdf->GetName());
    fit_pdf = w->pdf(fit_pdf->GetName());
    cout<<"LOAD pdfs"<<endl;
    //in_file->GetObject(fit_result->GetName(), fit_result);
    //in_file->GetObject(sfit_results->GetName(), sfit_results);
    sig_n = w->var(sig_n->GetName());
    bkg_n = w->var(bkg_n->GetName());
    cout<<"LOAD counts"<<endl;
    for(unsigned i=0; i<sig_components.size(); i++) {
        sig_components[i] = w->pdf(sig_components[i]->GetName());
    }
    for(unsigned i=0; i<bkg_components.size(); i++) {
        bkg_components[i] = w->pdf(bkg_components[i]->GetName());
    }
    cout<<"LOAD pdf components"<<endl;
    for(unsigned i=0; i<control_variables.size(); i++) {
        control_variables[i] = w->var(control_variables[i]->GetName());
    }
    cout<<"LOAD controls"<<endl;
    in_file->Close();
    delete in_file;
>>>>>>> 8588bf7bed65d3e292c2074c007b399715e6c1a0
}


// Globals

TTreeFormula* formula = NULL;
vector<MassFit*> fit_list;
TString default_selection = "1==1";

#include "RooIpatiaCommon.cxx"
#include "RooIpatia.cxx"
#include "RooIpatia2.cxx"
#include "RooCassandra.cxx"
#include "RooCassandra3.cxx"
#include "RooAsymCassandra.cxx"
#include "RooAsymCassandra3.cxx"
#include "RooExpGaussExp.cxx"
#include "RooDoubleCBShape.cxx"
#include "RooCutCBShape.cxx"
#endif // __CINT__
