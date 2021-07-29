#include <TFile.h>
#include <TMath.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void MakeNtuple(){

	// Initial setup.
	
	// Open file.
	string fileToOpen = "Data18.txt";
	ifstream datafile;
	datafile.open(fileToOpen.c_str(), ifstream::in);
	cout << "Reading filelist: " << fileToOpen.c_str() << endl;
	char datafilename[500];
	TChain *t1 = (TChain*)new TChain("outTree");
	while(true){
		datafile >> datafilename;
		if(strncmp(datafilename,"#",1) ==0)
		{
			continue;
		}
		if(datafile.eof()) break;
		t1->Add(datafilename);
		cout << "datafilename = " << datafilename << endl;
	}
	cout << "total number of events = " << t1->GetEntries() << endl;
	
	TFile *output = new TFile("dijet_Data18.root","RECREATE");

	int runNumber = 0;
	Long64_t eventNumber=0;
	float mjj=0;
	vector<float> *jet_pt=0, *jet_eta=0, *jet_DL1r_pb=0, *jet_DL1r_pc=0, *jet_DL1r_pu=0;
	float yStar=0;

	t1->SetBranchAddress("jet_pt",&jet_pt); 
	t1->SetBranchAddress("jet_eta",&jet_eta);
	t1->SetBranchAddress("yStar",&yStar);
	t1->SetBranchAddress("mjj",&mjj);
	t1->SetBranchAddress("jet_DL1r_pb",&jet_DL1r_pb);
	t1->SetBranchAddress("jet_DL1r_pc",&jet_DL1r_pc);
	t1->SetBranchAddress("jet_DL1r_pu",&jet_DL1r_pu);
	t1->SetBranchAddress("runNumber",&runNumber);
	t1->SetBranchAddress("eventNumber",&eventNumber);
	
	int Npt = 0;
	int Nystar = 0;
	int Nmjj = 0;

	// Create new tree.
	TTree *overlap = new TTree("overlap","overlap tree");

	// Create Run Number Branch
	int run_number = 0;
	TBranch *b_run_number = overlap->Branch("run_number", &run_number);

	// Create Event Number Branch
	unsigned long long int event_number = 0;
	TBranch *b_event_number = overlap->Branch("event_number", &event_number);

	// Create Region Branch
	vector<int> region;
	TBranch *b_region = overlap->Branch("region", &region);

	// Create Discrimination Variable Branch (or Branches if necessary).
	double m_jj = 0;
	TBranch *b_m_jj = overlap->Branch("m_jj", &m_jj);

	// Loop to emulate running over a number of events.
	for (int i = 0; i < t1->GetEntries(); i++){
		if(i%500000==0){
			cout << "processed " << i << "events." << endl;
		}
		t1->GetEntry(i);

	// Set Run Number.
//		run_number++;
		run_number = runNumber;
		// Set Event Number.
//		event_number++;
		event_number=eventNumber;
		m_jj = mjj;
		region.clear();
	
		// Clear and set as many regions as needed.

		if(jet_pt->at(0)<420)
			continue;
		Npt += 1;
		if(fabs(yStar)>=0.6)
			continue;
		Nystar +=1;
		if(mjj<=1100)
			continue;
		Nmjj +=1;
/*
// di-b channel selection
		if(jet_pt->at(0)<420)
			continue;
		if(fabs(jet_eta->at(0))>2 || fabs(jet_eta->at(1))>2)
			continue;
		if(fabs(yStar)>=0.8)
			continue;
		if(mjj<=1133)
			continue;
		double jet_DL1r_leading = log(jet_DL1r_pb->at(0)/(0.08*jet_DL1r_pc->at(0)+0.92*jet_DL1r_pu->at(0)));
		double jet_DL1r_subleading = log(jet_DL1r_pb->at(1)/(0.08*jet_DL1r_pc->at(1)+0.92*jet_DL1r_pu->at(1)));
		if(jet_DL1r_leading<0.8216141 && jet_DL1r_subleading<0.8216141)
			continue;
		region.push_back(1100);
		if(jet_DL1r_leading>0.8216141 && jet_DL1r_subleading>0.8216141)
			region.push_back(1101);
*/

		// Fill Tree for this event.
		overlap->Fill();

		jet_pt->clear();
//		jet_eta->clear();
//		jet_DL1r_pb->clear();
//		jet_DL1r_pc->clear();
//		jet_DL1r_pu->clear();

	}

//	overlap->Scan("*");

	cout << "pt = " << Npt << endl;
	cout << "ystar = " << Nystar << endl;
	cout << "mjj = " << Nmjj << endl;

	output->cd();
	overlap->Write();
	output->Close();

}

