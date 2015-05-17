#include "NNRunner.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"
#include "boost/regex.hpp"

#include "TTree.h"
#include "TFile.h"
#include "TDataMember.h"

#include "IncEvent.h"
#include "HptEventOld.h"

using namespace std;
using namespace boost::property_tree;

NNRunner::NNRunner(const string& json_file, const string& output_file):
    event(NULL)
{
    // in_parser.EnableDebugDump(true, false); 
    // out_parser.EnableDebugDump(true, false); 
    // Parse ini file
    string buff;
    ptree pt;
    string in_expr;
    string out_expr;
    string copy_expr;
    Network* net;

    json_parser::read_json(json_file, pt);

    size_t i = 0;
    // Get names of tree variables from where we will read NN inputs and wher we will store outputs
    cout<<"Reading inputs ..."<<endl;
    out_file = output_file;
    tree_name = pt.get<string>("input_tree");
    buff = pt.get<string>("mode");
    if(buff == "copy")
        mode = copy;
    else if(buff == "clone")
        mode = clone;
    else
        throw(string("Unknown execution mode: ")+buff);
    buff = pt.get<string>("event_class");
    if(buff == "None")
        event_class = none;
    else if(buff == "HptEventOld")
        event_class = hpt_event_old;
    else if(buff == "IncEvent")
        event_class = inc_event;
    else
        throw(string("Unknown output class: ")+buff);
    if(mode == copy && event_class != none) {
        if(event_class == inc_event) {
            event = new IncEvent();
        } else if(event_class == hpt_event_old) {
            event = new HptEventOld();
        }
    }

    inputs.resize(pt.get_child("inputs").size());
    tree_inputs = new TreeType[pt.get_child("inputs").size()];
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("inputs")) {
        input_names.push_back(make_pair(v.first.data(), v.second.data()));
        try
        {
            in_parser.DefineVar(v.first.data(), &inputs[i]);
            out_parser.DefineVar(v.first.data(), &inputs[i]);
            copy_parser.DefineVar(v.first.data(), &inputs[i]);
        }
        catch(mu::Parser::exception_type &e)
        {
            cerr << "Define input vars" << endl;
            cerr << "Message:  " << e.GetMsg() << "\n";
            cerr << "Formula:  " << e.GetExpr() << "\n";
            cerr << "Token:    " << e.GetToken() << "\n";
            cerr << "Position: " << e.GetPos() << "\n";
            cerr << "Errc:     " << e.GetCode() << "\n";
            exit(1);
        }
        i++;
    }
    cout<<"Reading outputs ..."<<endl;
    outputs.resize(pt.get_child("outputs").size());
    tree_outputs = new float[pt.get_child("outputs").size()];
    i = 0;
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("outputs")) {
        output_names.push_back(v.first.data());
        try
        {
            out_parser.DefineVar(v.first.data(), &outputs[i]);
            copy_parser.DefineVar(v.first.data(), &outputs[i]);
        }
        catch(mu::Parser::exception_type &e)
        {
            cerr << "Define output vars" << endl;
            cerr << "Message:  " << e.GetMsg() << "\n";
            cerr << "Formula:  " << e.GetExpr() << "\n";
            cerr << "Token:    " << e.GetToken() << "\n";
            cerr << "Position: " << e.GetPos() << "\n";
            cerr << "Errc:     " << e.GetCode() << "\n";
            exit(1);
        }
        if(i!=0) out_expr += ", ";
        out_expr += v.first.data();
        out_expr += "=";
        out_expr += v.second.data();
        i++;
    }
    if(mode == copy && event != NULL) {
        cout<<"Reading copies ..."<<endl;
        copies.resize(pt.get_child("copy").size());
        i = 0;
        BOOST_FOREACH(ptree::value_type &v, pt.get_child("copy")) {
            cout<<v.first.data()<<endl;
            cout<<event->ClassName()<<endl;
            copies[i].name = v.first.data();
            TDataMember* _member;
            if(event_class == inc_event) {
                _member = 
                    ((IncEvent*)event)->Class()->GetDataMember(v.first.data());
            } else if(event_class == hpt_event_old) {
                _member = 
                    ((HptEventOld*)event)->Class()->GetDataMember(v.first.data());
            }
            if(_member == NULL)
                throw(
                        str(boost::format(
                                "Class '%1%' does not have a data member '%2%'"
                                ) % event_class % v.first.data())
                        );
            copies[i].type = _member->GetTypeName();
            boost::regex re("\\[([0-9]+)\\]");
            boost::match_results<std::string::const_iterator> what;
            if(boost::regex_search(copies[i].name, what, re)) {
                copies[i].pos = atoi(
                        string(what[1].first, what[1].second).c_str()
                        );
                copies[i].name = boost::regex_replace(copies[i].name, re, "__\\1__");
            } else
                copies[i].pos = -1;
            if(_member->IsaPointer())
                copies[i].pointer = 
                    *((void**)((Long64_t)event + _member->GetOffset()));
            else
                copies[i].pointer = 
                    (void*)((Long64_t)event + _member->GetOffset());
            try
            {
                copy_parser.DefineVar(copies[i].name, &(copies[i].value));
            }
            catch(mu::Parser::exception_type &e)
            {
                cerr << "Define copy vars" << endl;
                cerr << "Message:  " << e.GetMsg() << "\n";
                cerr << "Formula:  " << e.GetExpr() << "\n";
                cerr << "Token:    " << e.GetToken() << "\n";
                cerr << "Position: " << e.GetPos() << "\n";
                cerr << "Errc:     " << e.GetCode() << "\n";
                exit(1);
            }
            if(i!=0) copy_expr += ", ";
            copy_expr += copies[i].name;
            copy_expr += "=";
            copy_expr += v.second.data();
            i++;
        }
    }
    cout<<"Reading networks ..."<<endl;
    i = 0;
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("networks")) {
        net = new Network;
        net->name = v.first.data();
        net->file = v.second.get<string>("xml");
        try
        {
            cout<<"Load network "<<net->name<<" from: "<<net->file<<endl;
            net->nn = new NeuralNetReader(net->file.c_str());
        }
        catch (char const* e)
        {
            cout << e << endl;
            exit(1);
        }
        size_t j = 0;
        net->inputs.resize(v.second.get_child("inputs").size());
        BOOST_FOREACH(ptree::value_type &item,
                v.second.get_child("inputs")) {
            try
            {
                in_parser.DefineVar(item.first.data(), &(net->inputs[j]));
            }
            catch(mu::Parser::exception_type &e)
            {
                cerr << "Define input NN vars" << endl;
                cerr << "Message:  " << e.GetMsg() << "\n";
                cerr << "Formula:  " << e.GetExpr() << "\n";
                cerr << "Token:    " << e.GetToken() << "\n";
                cerr << "Position: " << e.GetPos() << "\n";
                cerr << "Errc:     " << e.GetCode() << "\n";
                exit(1);
            }
            if( i!=0 || j!=0 ) in_expr += ", ";
            in_expr += item.first.data();
            in_expr += "=";
            in_expr += item.second.data();
            j++;
        }
        cout<<"Network outputs ..."<<endl;
        net->outputs.resize(v.second.get_child("outputs").size());
        j = 0;
        BOOST_FOREACH(ptree::value_type &item,
                v.second.get_child("outputs")) {
            try
            {
                out_parser.DefineVar(item.first.data(), &(net->outputs[j]));
            }
            catch(mu::Parser::exception_type &e)
            {
                cerr << "Define NN output vars" << endl;
                cerr << "Message:  " << e.GetMsg() << "\n";
                cerr << "Formula:  " << e.GetExpr() << "\n";
                cerr << "Token:    " << e.GetToken() << "\n";
                cerr << "Position: " << e.GetPos() << "\n";
                cerr << "Errc:     " << e.GetCode() << "\n";
                exit(1);
            }
            j++;
        }
        networks.push_back(net);
        i++;
    }

    try
    {
        in_parser.SetExpr(in_expr);
        out_parser.SetExpr(out_expr);
        if(mode == copy && event != NULL)
            copy_parser.SetExpr(copy_expr);
    }
    catch(mu::Parser::exception_type &e)
    {
        cerr << "Set expressions" << endl;
        cerr << "Message:  " << e.GetMsg() << "\n";
        cerr << "Formula:  " << e.GetExpr() << "\n";
        cerr << "Token:    " << e.GetToken() << "\n";
        cerr << "Position: " << e.GetPos() << "\n";
        cerr << "Errc:     " << e.GetCode() << "\n";
        exit(1);
    }

    cout<<"Tree variables:"<<endl;
    cout<<"***************"<<endl;
    cout<<"Inputs: "<<endl;
    for(int i=0; i<input_names.size(); i++) {
        cout<<input_names[i].first<<endl;
    }
    cout<<"***************"<<endl;
    cout<<"Outputs: "<<endl;
    for(int i=0; i<output_names.size(); i++) {
        cout<<output_names[i]<<endl;
    }
    cout<<"***************"<<endl;
    cout<<"Copies: "<<endl;
    for(int i=0; i<copies.size(); i++) {
        cout<<copies[i].name<<endl;
    }
    cout<<"***************"<<endl;
    cout<<"Formulas:"<<endl;
    cout<<"IN : "<<in_expr<<endl;
    cout<<"OUT: "<<out_expr<<endl;
    cout<<"COPY: "<<copy_expr<<endl;
}

void NNRunner::Process(const string& input_files)
{
    cout<<"Start processing ..."<<endl;
    chain = new TChain(tree_name.c_str());
    chain->Add(input_files.c_str());
    Long64_t nentries = chain->GetEntries();
    TFile* f = new TFile(out_file.c_str(), "recreate");
    TTree* out_tree;
    if(mode == copy && event != NULL)
        out_tree = new TTree("Test", "Test");
    else
        out_tree = chain->CloneTree(0);

    cout<<"Prepare input branches ..."<<endl;
    for(size_t i=0; i < input_names.size(); i++) {
        if(input_names[i].second == "bool")
            chain->SetBranchAddress(input_names[i].first.c_str(),
                    &(tree_inputs[i].Bool));
        else if(input_names[i].second == "int")
            chain->SetBranchAddress(input_names[i].first.c_str(),
                    &(tree_inputs[i].Int));
        else if(input_names[i].second == "long")
            chain->SetBranchAddress(input_names[i].first.c_str(),
                    &(tree_inputs[i].Long));
        else if(input_names[i].second == "float")
            chain->SetBranchAddress(input_names[i].first.c_str(),
                    &(tree_inputs[i].Float));
        else if(input_names[i].second == "double")
            chain->SetBranchAddress(input_names[i].first.c_str(),
                    &(tree_inputs[i].Double));
        else
            throw(string("Unknown data type: ")+input_names[i].second);
    }
    
    cout<<"Prepare output branches ..."<<endl;
    if(mode == copy && event != NULL) {
        if(event_class == inc_event) {
            IncEvent* ev = (IncEvent*) event;
            out_tree->Branch("Event", &ev);
        } else if(event_class == hpt_event_old) {
            HptEventOld* ev = (HptEventOld*) event;
            out_tree->Branch("Event", &ev);
        }
    } else {
        for(size_t i=0; i < output_names.size(); i++)
            out_tree->Branch(output_names[i].c_str(), &(tree_outputs[i]),
                    str(boost::format("%1%/F") % output_names[i]).c_str());
    }

    cout<<"Bout: "<<out_tree->GetListOfBranches()->GetEntries()<<endl;

    cout<<"Run ..."<<endl;
    // for(Long64_t i=0;i<1000; i++) { 
    for(Long64_t i=0;i<nentries; i++) {
        chain->GetEntry(i);
        for(size_t j = 0; j<inputs.size(); j++) {
            if(input_names[j].second == "bool")
                inputs[j] = tree_inputs[j].Bool;
            else if(input_names[j].second == "int")
                inputs[j] = tree_inputs[j].Int;
            else if(input_names[j].second == "long")
                inputs[j] = tree_inputs[j].Long;
            else if(input_names[j].second == "float")
                inputs[j] = tree_inputs[j].Float;
            else if(input_names[j].second == "double")
                inputs[j] = tree_inputs[j].Double;
            else
                throw(string("Unknown data type: ")+input_names[j].second);
        }
        try
        {
            in_parser.Eval();
        }
        catch(mu::Parser::exception_type &e)
        {
            cerr << "Eval input" << endl;
            cerr << "Message:  " << e.GetMsg() << "\n";
            cerr << "Formula:  " << e.GetExpr() << "\n";
            cerr << "Token:    " << e.GetToken() << "\n";
            cerr << "Position: " << e.GetPos() << "\n";
            cerr << "Errc:     " << e.GetCode() << "\n";
            exit(1);
        }
        Network* net;
        for(size_t j = 0; j<networks.size(); j++) {
            net = networks[j];
            try
            {
                net->nn->Run(net->inputs);
                for(size_t k = 0; k<net->outputs.size(); k++) {
                    net->outputs[k] = net->nn->GetOneOutput(k);
                }
            }
            catch (char const* e)
            {
                cout << e << endl;
                exit(1);
            }
        }
        try
        {
            out_parser.Eval();
        }
        catch(mu::Parser::exception_type &e)
        {
            cerr << "Eval output" << endl;
            cerr << "Message:  " << e.GetMsg() << "\n";
            cerr << "Formula:  " << e.GetExpr() << "\n";
            cerr << "Token:    " << e.GetToken() << "\n";
            cerr << "Position: " << e.GetPos() << "\n";
            cerr << "Errc:     " << e.GetCode() << "\n";
            exit(1);
        }
        if(mode == copy && event != NULL) {
            try
            {
                copy_parser.Eval();
            }
            catch(mu::Parser::exception_type &e)
            {
                cerr << "Eval copy" << endl;
                cerr << "Message:  " << e.GetMsg() << "\n";
                cerr << "Formula:  " << e.GetExpr() << "\n";
                cerr << "Token:    " << e.GetToken() << "\n";
                cerr << "Position: " << e.GetPos() << "\n";
                cerr << "Errc:     " << e.GetCode() << "\n";
                exit(1);
            }
            for(size_t j = 0; j<copies.size(); j++) {
                if(copies[j].type == "Bool_t")
                    if(copies[j].pos == -1)
                        *((Bool_t*)copies[j].pointer) = copies[j].value;
                    else
                        ((Bool_t*)copies[j].pointer)[copies[j].pos] =
                            copies[j].value;
                else if(copies[j].type == "Int_t")
                    if(copies[j].pos == -1)
                        *((Int_t*)copies[j].pointer) = copies[j].value;
                    else
                        ((Int_t*)copies[j].pointer)[copies[j].pos] =
                            copies[j].value;
                else if(copies[j].type == "Long64_t") {
                    if(copies[j].pos == -1)
                        *((Long64_t*)copies[j].pointer) = copies[j].value;
                    else
                        ((Long64_t*)copies[j].pointer)[copies[j].pos] =
                            copies[j].value;
                }
                else if(copies[j].type == "Float_t")
                    if(copies[j].pos == -1)
                        *((Float_t*)copies[j].pointer) = copies[j].value;
                    else
                        ((Float_t*)copies[j].pointer)[copies[j].pos] =
                            copies[j].value;
                else if(copies[j].type == "Double_t")
                    if(copies[j].pos == -1)
                        *((Double_t*)copies[j].pointer) = copies[j].value;
                    else
                        ((Double_t*)copies[j].pointer)[copies[j].pos] =
                            copies[j].value;
            }
        } else {
            for(size_t j = 0; j<outputs.size(); j++) {
                tree_outputs[j] = (float)outputs[j];
            }
        }
        out_tree->Fill();
    }

    cout<<"Write ..."<<endl;
    f->cd();
    out_tree->Write();
    f->Close();

    // delete chain; 
    // delete out_tree; 
    // delete f; 
}

        // Long_t offset = ev->Class()->GetDataMember("eRun")->GetOffset(); 
        // Int_t* val = (Int_t*)((Long_t)ev + offset); 
