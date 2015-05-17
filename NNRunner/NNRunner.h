#ifndef NNRUNNER_H
#define NNRUNNER_H
#include "TChain.h"
#include "NeuralNetReader.h"
#include <muParser.h>
#include <string>
#include <vector>
#include <utility>

typedef struct Network {
    std::string name;
    std::string file;
    NeuralNetReader* nn;
    std::vector<double> outputs;
    std::vector<double> inputs;
} Network;

typedef union TreeType {
    Bool_t Bool;
    Int_t Int;
    Long64_t Long;
    Float_t Float;
    Double_t Double;
} TreeType;

typedef struct CopyType {
    std::string name;
    std::string type;
    int pos;
    double value;
    void* pointer;
} CopyType;

class NNRunner {
    public:
        NNRunner(const std::string& json_file, const std::string& output_file);

        void Process(const std::string& input_files);

    private:
        typedef enum {
            copy,
            clone
        } Modes;
        typedef enum {
            none,
            hpt_event_old,
            inc_event
        } EventClasses;
        Modes mode;
        EventClasses event_class;
        std::string out_file;
        std::string tree_name;
        std::vector<std::pair<std::string, std::string> > input_names;
        std::vector<std::string> output_names;
        TreeType* tree_inputs;
        float* tree_outputs;
        std::vector<double> inputs;
        std::vector<double> outputs;
        std::vector<Network*> networks;
        std::vector<CopyType> copies;
        TChain* chain;
        TObject* event;
        NeuralNetReader* nn;
        mu::Parser in_parser; // Relation expresions parser
        mu::Parser out_parser; // Relation expresions parser
        mu::Parser copy_parser; // Relation expresions parser
};

#endif
