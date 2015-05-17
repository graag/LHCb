#include "NNRunner.h"
#include <iostream>

using namespace std;

int main(int argc,char** argv) {
    if(argc != 4) {
        cerr<<"Usage:\nNNRunner options.json output.root \"input_files\""<<endl;
        exit(1);
    }

    cout<<"############"<<endl;
    cout<<"# NNRunner #"<<endl;
    cout<<"############"<<endl;

    NNRunner* nnr;
    try {
        nnr = new NNRunner(argv[1], argv[2]);
    } catch (const std::string& ex) {
        cerr<<ex<<endl;
        return -1;
    }

    try {
        nnr->Process(argv[3]);
    } catch (const std::string& ex) {
        cerr<<ex<<endl;
        return -1;
    }

    return 0;
}

