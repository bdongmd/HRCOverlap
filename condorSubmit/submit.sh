#!/bin/bash
shopt -s expand_aliases

CURRDIR=/afs/cern.ch/work/b/bdong/private/HeavyResCombination/HRCOverlap
cd $CURRDIR

python3 OverlapCheck.py -o inputData/dijet_removedDib.root
