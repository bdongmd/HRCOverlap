#!/bin/bash
shopt -s expand_aliases

CURRDIR=/afs/cern.ch/work/b/bdong/private/HeavyResCombination/HRCOverlap
cd $CURRDIR

python3 OverlapCheck.py -j inputData/dijet_Data18.root -b inputData/dibjet_Data18.root -o inputData/dijet_Data18_removeddib.root
