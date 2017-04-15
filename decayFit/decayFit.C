#define decayFit_cxx
// The class definition in massFit.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("massFit.C")
// Root > T->Process("massFit.C","some options")
// Root > T->Process("massFit.C+")
//

float decayFit(void){return 0;}

void decayFit_Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
}

void decayFit_SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

    cerr<<"# Reading data"<<endl;

#ifndef __CINT__
    decayFitSetup();

    TString option = GetOption();
    if(option != "" and option != "nohist")
        formula = new TTreeFormula("Selection", option, fChain);
    else
        formula = new TTreeFormula("Selection", default_selection, fChain);
    //formula->Print();

    for(unsigned i=0; i<fit_list.size(); i++)
    {
        if(!(fit_list[i]->selection.empty())) {
            fit_list[i]->formula = new TTreeFormula(
                    (fit_list[i]->name + "Selection").c_str(),
                    fit_list[i]->selection.c_str(),
                    fChain);

            //fit_list[i]->formula->Print();
        }
    }
#endif // __CINT__
}

Bool_t decayFit_Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either massFit::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   //

#ifndef __CINT__
    TDataMember* _member;
    string _member_type;
    MassFit* _fit;
    void* _pt;
    bool _select;
    double _weight;

    if(formula->EvalInstance()) {
        for(unsigned i=0; i<fit_list.size(); i++)
        {
            _weight = 1.0;
            _fit = fit_list[i];
            if(_fit->formula == NULL) {
                _select = true;
            } else {
                _select = _fit->formula->EvalInstance();
            }

            if(_select) {
                if(_fit->single_candidate) {
                    if(_fit->run == runNumber && _fit->event == eventNumber) {
                        continue;
                    } else {
                        _fit->run = runNumber;
                        _fit->event = eventNumber;
                    }
                }

               _member = fClass->GetDataMember(_fit->mass_name.c_str());
               _member_type = string(_member->GetTypeName());
               if(_member->IsaPointer())
                   _pt = *((void**)((Long64_t)this + _member->GetOffset()));
               else
                   _pt = (void*)((Long64_t)this + _member->GetOffset());
               if(_member_type == "ROOT::TImpProxy<double>"
                       || _member_type == "ROOT::Internal::TImpProxy<double>")
                   *(_fit->mass) = *((TDoubleProxy*)_pt);
               else if(_member_type == "ROOT::TImpProxy<float>"
                       || _member_type == "ROOT::Internal::TImpProxy<float>")
                   *(_fit->mass) = *((TFloatProxy*)_pt);
               else if(_member_type == "ROOT::TImpProxy<int>"
                       || _member_type == "ROOT::Internal::TImpProxy<int>")
                   *(_fit->mass) = *((TIntProxy*)_pt);
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<double,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<double,0> >")
                   *(_fit->mass) = (*((TArrayDoubleProxy*)_pt))[0];
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<float,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<float,0> >")
                   *(_fit->mass) = (*((TArrayFloatProxy*)_pt))[0];
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<int,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<int,0> >")
                   *(_fit->mass) = (*((TArrayIntProxy*)_pt))[0];
               else {
                   std::cerr<<"decayFit ERROR: Urecognized data member type - "<<_member_type<<std::endl;
                   this->Abort("Urecognized data member type");
               }

               _member = fClass->GetDataMember(_fit->mass_err_name.c_str());
               _member_type = string(_member->GetTypeName());
               if(_member->IsaPointer())
                   _pt = *((void**)((Long64_t)this + _member->GetOffset()));
               else
                   _pt = (void*)((Long64_t)this + _member->GetOffset());
               if(_member_type == "ROOT::TImpProxy<double>"
                       || _member_type == "ROOT::Internal::TImpProxy<double>")
                   *(_fit->sigma) = *((TDoubleProxy*)_pt);
               else if(_member_type == "ROOT::TImpProxy<float>"
                       || _member_type == "ROOT::Internal::TImpProxy<float>")
                   *(_fit->sigma) = *((TFloatProxy*)_pt);
               else if(_member_type == "ROOT::TImpProxy<int>"
                       || _member_type == "ROOT::Internal::TImpProxy<int>")
                   *(_fit->sigma) = *((TIntProxy*)_pt);
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<double,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<double,0> >")
                   *(_fit->sigma) = (*((TArrayDoubleProxy*)_pt))[0];
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<float,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<float,0> >")
                   *(_fit->sigma) = (*((TArrayFloatProxy*)_pt))[0];
               else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<int,0> >"
                       || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<int,0> >")
                   *(_fit->sigma) = (*((TArrayIntProxy*)_pt))[0];
               else {
                   std::cerr<<"decayFit ERROR: Urecognized data member type - "<<_member_type<<std::endl;
                   this->Abort("Urecognized data member type");
               }

               for(unsigned i=0; i<_fit->control_names.size(); i++) {
                   double v;
                   _member = fClass->GetDataMember(_fit->control_names[i].c_str());
                   _member_type = string(_member->GetTypeName());
                   if(_member->IsaPointer())
                       _pt = *((void**)((Long64_t)this + _member->GetOffset()));
                   else
                       _pt = (void*)((Long64_t)this + _member->GetOffset());
                   if(_member_type == "ROOT::TImpProxy<double>"
                           || _member_type == "ROOT::Internal::TImpProxy<double>")
                       v = *((TDoubleProxy*)_pt);
                   else if(_member_type == "ROOT::TImpProxy<float>"
                           || _member_type == "ROOT::Internal::TImpProxy<float>")
                       v = *((TFloatProxy*)_pt);
                   else if(_member_type == "ROOT::TImpProxy<int>"
                           || _member_type == "ROOT::Internal::TImpProxy<int>")
                       v = *((TIntProxy*)_pt);
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<double,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<double,0> >")
                       v = (*((TArrayDoubleProxy*)_pt))[0];
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<float,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<float,0> >")
                       v = (*((TArrayFloatProxy*)_pt))[0];
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<int,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<int,0> >")
                       v = (*((TArrayIntProxy*)_pt))[0];
                   else {
                       std::cerr<<"decayFit ERROR: Urecognized data member type - "<<_member_type<<std::endl;
                       this->Abort("Urecognized data member type");
                   }

                   if(_fit->control_functions[i] != NULL) {
                       v = _fit->control_functions[i]->Eval(v);
                   }

                   *(_fit->control_variables[i]) = v;
               }

               // Calculate weight if it was requested
               if(! _fit->weight_name.empty()) {
                   _member = fClass->GetDataMember(_fit->weight_name.c_str());
                   _member_type = string(_member->GetTypeName());
                   if(_member->IsaPointer())
                       _pt = *((void**)((Long64_t)this + _member->GetOffset()));
                   else
                       _pt = (void*)((Long64_t)this + _member->GetOffset());
                   if(_member_type == "ROOT::TImpProxy<double>"
                           || _member_type == "ROOT::Internal::TImpProxy<double>")
                       _weight = *((TDoubleProxy*)_pt);
                   else if(_member_type == "ROOT::TImpProxy<float>"
                           || _member_type == "ROOT::Internal::TImpProxy<float>")
                       _weight = *((TFloatProxy*)_pt);
                   else if(_member_type == "ROOT::TImpProxy<int>"
                           || _member_type == "ROOT::Internal::TImpProxy<int>")
                       _weight = *((TIntProxy*)_pt);
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<double,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<double,0> >")
                       _weight = (*((TArrayDoubleProxy*)_pt))[0];
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<float,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<float,0> >")
                       _weight = (*((TArrayFloatProxy*)_pt))[0];
                   else if(_member_type == "ROOT::TArrayProxy<ROOT::TArrayType<int,0> >"
                           || _member_type == "ROOT::Internal::TArrayProxy<ROOT::Internal::TArrayType<int,0> >")
                       _weight = (*((TArrayIntProxy*)_pt))[0];
                   else {
                       std::cerr<<"decayFit ERROR: Urecognized data member type - "<<_member_type<<std::endl;
                       this->Abort("Urecognized data member type");
                   }
               }

               _fit->data->add( RooArgSet( *(_fit->observables) ), _weight );
            }
        }
    }
#endif // __CINT__

    return kTRUE;
}

Bool_t decayFit_Notify()
{
    if(formula != NULL)
        formula->Notify();

    for(unsigned i=0; i<fit_list.size(); i++)
    {
        if(fit_list[i]->formula != NULL)
            fit_list[i]->formula->Notify();
    }

    return kTRUE;
}

void decayFit_SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
}

void decayFit_Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

#ifndef __CINT__
    cerr<<"# Start fitting"<<endl;
    MassFit* _fit;

    ofstream f;
    f.open ("decayFit.csv", ios::out | ios::app);

    for(unsigned i=0; i<fit_list.size(); i++)
    {
        _fit = fit_list[i];
        cout<<"Name: "<<_fit->name<<", Entries: "<<_fit->data->numEntries()<<endl;
        _fit->fit();
        _fit->sfit();
#ifdef DECAY_PLOTS
        _fit->plot();
#endif

        f<<_fit->fom<<","<<_fit->fom_err<<",";
        f<<_fit->sig_n->getVal()<<","<<_fit->sig_n->getError()<<",";
        f<<_fit->bkg_n->getVal()<<","<<_fit->bkg_n->getError()<<",";
        f<<_fit->sig_mass->getVal()<<","<<_fit->sig_mass->getError()<<",";
        f<<_fit->sig_sigma->getVal()<<","<<_fit->sig_sigma->getError()<<"; ";
    }

    f<<endl;
    f.close();

    if(gROOT->IsBatch()) {
        for(unsigned i=0; i<fit_list.size(); i++) {
            delete fit_list[i];
        }
        fit_list.clear();
        delete formula;
        formula = NULL;
    }

    cerr<<"# End fitting"<<endl;
#endif // __CINT__
}
