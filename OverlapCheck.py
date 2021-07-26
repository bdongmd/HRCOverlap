import ROOT
import sys
import numpy as np
from array import array
from datetime import datetime

import argparse
parser = argparse.ArgumentParser(description='output file')
parser.add_argument('-o', '--output', type=str)
args = parser.parse_args()

f_output = ROOT.TFile.Open(args.output, 'RECREATE')
tree = ROOT.TTree('overlap','overlap')
f_output.cd()
run_number = array('i', [0]) 
event_number = array('q', [0])
m_jj = array('f', [0])
tree.Branch('run_number', run_number, 'run_number/I')
tree.Branch('event_number', event_number, 'event_number/L')
tree.Branch('m_jj', m_jj, 'm_jj/F')

for j in range(4):
	f_dijet = ROOT.TFile.Open('inputData/dijetData1{}.root'.format(j+5), 'READ')
	f_dibjet = ROOT.TFile.Open('inputData/dibjetData1{}.root'.format(j+5), 'READ')

	dijetTree = f_dijet.Get('overlap')
	dibjetTree = f_dibjet.Get('overlap')

	diblist = np.array([])
	for i in range(0,dibjetTree.GetEntries()):
		dibjetTree.GetEntry(i)
		tmplist =  [getattr(dibjetTree, 'run_number'), getattr(dibjetTree, 'event_number')]
		diblist = np.append(diblist, tmplist)
		if i % 20000 == 0:
			print('processed {} events'.format(i))
	diblist = diblist.reshape(-1,2)

	for i in range(0, dijetTree.GetEntries()):
		dijetTree.GetEntry(i)
		tmplist =  [getattr(dijetTree, 'run_number'), getattr(dijetTree, 'event_number')]
		if tmplist in diblist:
			continue
		run_number[0] = getattr(dijetTree, 'run_number')
		event_number[0] =  getattr(dijetTree, 'event_number')
		m_jj[0] = getattr(dijetTree, 'm_jj')
		tree.Fill()
		if i % 50000 == 0:
			print('{}: processed {} events'.format(datetime.now().strftime('%H:%M:%S'),i))

tree.Write("", ROOT.TObject.kOverwrite);
f_output.Close()
