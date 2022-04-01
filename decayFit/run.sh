#!/bin/bash
#PBS -N {{ name }}
#PBS -q i12h

module load root/6.11.02-x86_64-gcc71-python27

cd $PBS_O_WORKDIR

python $HOME/Soft/LHCb/decayFit/decayFit.py -s Fit.json -n DecayTree {{ data }}
rm decayFit_proxy*
