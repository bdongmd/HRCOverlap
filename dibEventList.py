import ROOT
import sys
import numpy as np
from array import array
from datetime import datetime

import argparse
parser = argparse.ArgumentParser(description='output file')
parser.add_argument('-i', '--input', type=str)
parser.add_argument('-o', '--output', type=str)
args = parser.parse_args()

f_dibjet = ROOT.TFile.Open(args.input, 'READ')
dibjetTree = f_dibjet.Get('overlap')

with open(args.output, 'w') as outputfile:
	for i in range(0,dibjetTree.GetEntries()):
		if i % 100000 == 0:
			print('processed {} events'.format(i))
		dibjetTree.GetEntry(i)
		if (len(getattr(dibjetTree, 'region')) == 1):
			continue
		tmplist =  [getattr(dibjetTree, 'run_number'), getattr(dibjetTree, 'event_number')]
		outputfile.write('{}, {}\n'.format(tmplist[0],tmplist[1]))
	f_dibjet.Close()
