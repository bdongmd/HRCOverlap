import ROOT
import sys
import numpy as np
from array import array
from datetime import datetime

import argparse
parser = argparse.ArgumentParser(description='output file')
parser.add_argument('-j', '--dijet', type=str)
parser.add_argument('-b', '--dibjet', type=str)
parser.add_argument('-o', '--output', type=str)
args = parser.parse_args()

f_dijet = ROOT.TFile.Open(args.dijet, 'READ')
f_dibjet = ROOT.TFile.Open(args.dibjet, 'READ')

dijetTree = f_dijet.Get('overlap')
dibjetTree = f_dibjet.Get('overlap')

f_output = ROOT.TFile.Open(args.output, 'RECREATE')
tree = ROOT.TTree('overlap','overlap')
f_output.cd()
run_number = array('i', [0]) 
event_number = array('q', [0])
m_jj = array('f', [0])
tree.Branch('run_number', run_number, 'run_number/I')
tree.Branch('event_number', event_number, 'event_number/L')
tree.Branch('m_jj', m_jj, 'm_jj/F')

diblist = np.empty([dibjetTree.GetEntries(), 2], dtype=np.int64)
for i in range(0,dibjetTree.GetEntries()):
	dibjetTree.GetEntry(i)
	tmplist =  [getattr(dibjetTree, 'run_number'), getattr(dibjetTree, 'event_number')]
	diblist[i] = tmplist
	if i % 100000 == 0:
		print('processed {} events'.format(i))
f_dibjet.Close()

diblist = diblist.tolist()
for i in range(0, dijetTree.GetEntries()):
	dijetTree.GetEntry(i)
	tmplist =  [getattr(dijetTree, 'run_number'), getattr(dijetTree, 'event_number')]
	if i % 100000 == 0:
		print('{}: processed {} events'.format(datetime.now().strftime('%H:%M:%S'),i))
	if tmplist in diblist:
		print("kicking out an event")
		continue
	run_number[0] = getattr(dijetTree, 'run_number')
	event_number[0] =  getattr(dijetTree, 'event_number')
	m_jj[0] = getattr(dijetTree, 'm_jj')
	tree.Fill()
f_dijet.Close()

tree.Write()
f_output.Close()
