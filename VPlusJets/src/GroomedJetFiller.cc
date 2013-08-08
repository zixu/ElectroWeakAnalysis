/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VplusJetSubstructure
 *
 *
 * Authors:
 *   Nhan V Tran, Fermilab - kalanand@fnal.gov
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill groomed jet related quantities into a specified TTree
 *   Works with jets in PAT data format.
 * History:
 *   
 *
 * Copyright (C) 2012 FNAL 
 *****************************************************************************/


// user include files
#include "ElectroWeakAnalysis/VPlusJets/interface/GroomedJetFiller.h" 

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include <fastjet/ClusterSequence.hh>
//#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/GhostedAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/GhostedAreaSpec.hh"

#include "TVector3.h"
#include "TMath.h"

ewk::GroomedJetFiller::GroomedJetFiller(const char *name, 
			TTree* tree, 
			const std::string jetAlgorithmLabel,
			//const std::string srcGroomedJet,
			const edm::ParameterSet& iConfig,bool isGen)
{
	tree_     = tree;

	//Gen Jet
	isGenJ = isGen;
	if(isGen){ lableGen = "Gen"; } else{ lableGen = ""; }

	// get algo and radius
	jetAlgorithmLabel_ = jetAlgorithmLabel;
	unsigned int labelSize = jetAlgorithmLabel.size();
	mJetAlgo = "";
	mJetAlgo.push_back( jetAlgorithmLabel.at(0) ); mJetAlgo.push_back( jetAlgorithmLabel.at(1) );
	if (labelSize == 3 || labelSize == 4 ){
		const char* tmp1 = &jetAlgorithmLabel.at(2);
		mJetRadius = atof( tmp1 )/10.;
	} else{
		std::cout << "problem in defining jet type!" << std::endl;
	}
	std::cout << "jet algo: " << mJetAlgo << ", jet radius: " << mJetRadius << std::endl;
	BREAK();

	// Declare all the branches of the tree
	SetBranch( jetpt_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_uncorr");
	SetBranch( jetmass_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_uncorr");
	SetBranch( jetmass_tr_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_tr_uncorr");
	SetBranch( jetmass_ft_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_ft_uncorr");
	SetBranch( jetmass_pr_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_pr_uncorr");
	SetBranch( tau2tau1, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_tau2tau1");
	SetBranch( tau1, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_tau1");
	SetBranch( tau2, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_tau2");
	SetBranch( tau3, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_tau3");
	SetBranch( tau4, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_tau4");
	SetBranch( massdrop_pr_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_massdrop_pr_uncorr");

	SetBranch( jetpt, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt");
	SetBranch( jeteta, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_eta");
	SetBranch( jetphi, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_phi");
	SetBranch( jete, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_e");
	SetBranch( jetpt_tr_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_tr_uncorr");
	SetBranch( jetpt_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_tr");
	SetBranch( jeteta_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_eta_tr");
	SetBranch( jetphi_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_phi_tr");
	SetBranch( jete_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_e_tr");
	SetBranch( jetpt_ft_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_ft_uncorr");
	SetBranch( jetpt_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_ft");
	SetBranch( jeteta_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_eta_ft");
	SetBranch( jetphi_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_phi_ft");
	SetBranch( jete_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_e_ft");
	SetBranch( jetpt_pr_uncorr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_pr_uncorr");
	SetBranch( jetpt_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_pt_pr");
	SetBranch( jeteta_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_eta_pr");
	SetBranch( jetphi_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_phi_pr");
	SetBranch( jete_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_e_pr");

	SetBranch( prsubjet1_px, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet1_px");
	SetBranch( prsubjet1_py, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet1_py");
	SetBranch( prsubjet1_pz, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet1_pz");
	SetBranch( prsubjet1_e, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet1_e");
	SetBranch( prsubjet2_px, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet2_px");
	SetBranch( prsubjet2_py, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet2_py");
	SetBranch( prsubjet2_pz, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet2_pz");
	SetBranch( prsubjet2_e, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_prsubjet2_e");

	SetBranch( jetmass, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass");
	SetBranch( jetmass_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_tr");
	SetBranch( jetmass_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_ft");
	SetBranch( jetmass_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_mass_pr");
	SetBranch( massdrop_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_massdrop_pr");
	SetBranch( jetarea, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_area");
	SetBranch( jetarea_tr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_area_tr");
	SetBranch( jetarea_ft, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_area_ft");
	SetBranch( jetarea_pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_area_pr");
	SetBranch( jetconstituents, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_jetconstituents");
	SetBranch( jetcharge, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_jetcharge");

	// cores
	tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_rcores").c_str(), rcores, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_rcores"+"[11][6]/F").c_str() );
	bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_rcores").c_str() );
	tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_ptcores").c_str(), ptcores, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_ptcores"+"[11][6]/F").c_str() );
	bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_ptcores").c_str() );

	//planarflow
	tree_->Branch((lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_planarflow").c_str(),planarflow, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_planarflow"+"[11][6]/F").c_str());
	bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_planarflow").c_str() );

	// qjets
	tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmass").c_str(), qjetmass, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmass"+"[50]/F").c_str() );
	bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmass").c_str() );
	tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmassdrop").c_str(), qjetmassdrop, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmassdrop"+"[50]/F").c_str() );
	bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_qjetmassdrop").c_str() );

	if( iConfig.existsAs<bool>("GroomedJet_saveConstituents") ) 
	  mSaveConstituents=iConfig.getParameter< bool >("GroomedJet_saveConstituents");
	else mSaveConstituents = true;

	if (mSaveConstituents){
		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_eta").c_str(), constituents0_eta, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_eta"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_eta").c_str() );   
		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_phi").c_str(), constituents0_phi, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_phi"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_phi").c_str() );   
		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_e").c_str(), constituents0_e, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_e"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0_e").c_str() );   
		SetBranchSingle( &nconstituents0, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_nconstituents0" );

		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_eta").c_str(), constituents0pr_eta, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_eta"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_eta").c_str() );   
		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_phi").c_str(), constituents0pr_phi, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_phi"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_phi").c_str() );   
		tree_->Branch( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_e").c_str(), constituents0pr_e, (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_e"+"[100]/F").c_str() );
		bnames.push_back( (lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_constituents0pr_e").c_str() );   
		SetBranchSingle( &nconstituents0pr, lableGen + "GroomedJet_" + jetAlgorithmLabel_ + "_nconstituents0pr" );
	}

	////////////////////////////////////
	// CORRECTIONS ON THE FLY
	////////////////////////////////////     
	//// --- groomed jet label -------
	//mGroomedJet =  srcGroomedJet;

	// --- Are we running over Monte Carlo ? --- 
	if( iConfig.existsAs<bool>("runningOverMC") ) 
	  runningOverMC_=iConfig.getParameter< bool >("runningOverMC");
	else runningOverMC_= false;

	// --- Are we applying AK7 JEC to our groomed jet ? --- 
	if( iConfig.existsAs<bool>("applyJECToGroomedJets") ) 
	  applyJECToGroomedJets_=iConfig.getParameter< bool >("applyJECToGroomedJets");
	else applyJECToGroomedJets_ = false;

	//// --- fastjet rho label -------
	JetsFor_rho =  iConfig.getParameter<std::string>("srcJetsforRho") ; 
	if(applyJECToGroomedJets_)
	  JEC_GlobalTag_forGroomedJet = iConfig.getParameter<std::string>("JEC_GlobalTag_forGroomedJet") ; 

	//// --- primary vertex -------
	if(  iConfig.existsAs<edm::InputTag>("srcPrimaryVertex") )
	  mPrimaryVertex = iConfig.getParameter<edm::InputTag>("srcPrimaryVertex"); 
	else mPrimaryVertex =  edm::InputTag("offlinePrimaryVertices");

	// ---- setting up the jec on-the-fly from text files...    
	//    std::string fDir = "JEC/" + JEC_GlobalTag_forGroomedJet;   
	std::string fDir = JEC_GlobalTag_forGroomedJet;   
	std::vector< JetCorrectorParameters > jecPars;
	std::vector< std::string > jecStr;

	if(applyJECToGroomedJets_) {
		if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001) {
			jecStr.push_back( fDir + "_L1FastJet_AK5PFchs.txt" );
			jecStr.push_back( fDir + "_L2Relative_AK5PFchs.txt" );
			jecStr.push_back( fDir + "_L3Absolute_AK5PFchs.txt" );
			if (!runningOverMC_)
			  jecStr.push_back( fDir + "_L2L3Residual_AK5PFchs.txt" );
		}else{
			jecStr.push_back( fDir + "_L1FastJet_AK7PFchs.txt" );
			jecStr.push_back( fDir + "_L2Relative_AK7PFchs.txt" );
			jecStr.push_back( fDir + "_L3Absolute_AK7PFchs.txt" );
			if (!runningOverMC_)
			  jecStr.push_back( fDir + "_L2L3Residual_AK7PFchs.txt" );
		}

		for (unsigned int i = 0; i < jecStr.size(); ++i){
			JetCorrectorParameters* ijec = new JetCorrectorParameters( jecStr[i] );
			jecPars.push_back( *ijec );
		}

		jec_ = new FactorizedJetCorrector(jecPars);
		if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001) {
			jecUnc_ = new JetCorrectionUncertainty( fDir + "_Uncertainty_AK5PFchs.txt" );
		}else{
			jecUnc_ = new JetCorrectionUncertainty( fDir + "_Uncertainty_AK7PFchs.txt" );
		}
	}

	// specific configurations
	if( iConfig.existsAs<double>("GroomedJet_JetChargeKappa") ) 
	  mJetChargeKappa=iConfig.getParameter< double >("GroomedJet_JetChargeKappa");
	else mJetChargeKappa = 0.3;
	if( iConfig.existsAs<int>("GroomedJet_QJetsPreclustering") ) 
	  mQJetsPreclustering=iConfig.getParameter< int >("GroomedJet_QJetsPreclustering");
	else mQJetsPreclustering = 30;
	if( iConfig.existsAs<int>("GroomedJet_QJetsN") ) 
	  mQJetsN=iConfig.getParameter< int >("GroomedJet_QJetsN");
	else mQJetsN = 50;
	if( iConfig.existsAs<double>("GroomedJet_NsubjettinessKappa") ) 
	  mNsubjettinessKappa=iConfig.getParameter< double >("GroomedJet_NsubjettinessKappa");
	else mNsubjettinessKappa = 1.0;
	if( iConfig.existsAs<bool>("GroomedJet_doQJets") ) 
	  mDoQJets=iConfig.getParameter< bool >("GroomedJet_doQJets");
	else mDoQJets = true;

	// define charges of pdgIds
	neutrals.push_back( 22 ); neutrals.push_back( 130 ); neutrals.push_back( 310 ); neutrals.push_back( 311 ); neutrals.push_back( 111 ); 
	neutrals.push_back( 1 ); neutrals.push_back( 2 ); neutrals.push_back( 3 ); neutrals.push_back( 4 ); neutrals.push_back( 5 ); 
	neutrals.push_back( -1 ); neutrals.push_back( -2 ); neutrals.push_back( -3 ); neutrals.push_back( -4 ); neutrals.push_back( -5 ); 
	neutrals.push_back( 2112 );

	positives.push_back( 321 ); positives.push_back( 211 ); ; positives.push_back( -11 ); positives.push_back( -13); positives.push_back( 2212);
	negatives.push_back( -321 ); negatives.push_back( -211 ); negatives.push_back( 11 ); negatives.push_back( 13 );


}




//////////////////////////////////////////////////////////////////
/////// Helper for above function ////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ewk::GroomedJetFiller::SetBranchSingle( float* x, std::string name)
{
	tree_->Branch( name.c_str(), x, ( name+"/F").c_str() );
	bnames.push_back( name );
}

void ewk::GroomedJetFiller::SetBranchSingle( int* x, std::string name)
{
	tree_->Branch( name.c_str(), x, ( name+"/I").c_str() );
	bnames.push_back( name );
}

void ewk::GroomedJetFiller::SetBranch( float* x, std::string name)
{
	tree_->Branch( name.c_str(), x, ( name+"[6]/F").c_str() );
	bnames.push_back( name );
}


void ewk::GroomedJetFiller::SetBranch( int* x, std::string name)
{
	tree_->Branch( name.c_str(), x, ( name+"[6]/I").c_str() );
	bnames.push_back( name );
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////





// ------------ method called to produce the data  ------------
void ewk::GroomedJetFiller::fill(const edm::Event& iEvent, std::vector<fastjet::PseudoJet> FJparticles) {

	////----------
	// init
	for (int j =0; j< NUM_JET_MAX; ++j) {
		jetmass_uncorr[j] = -1.;
		jetmass_tr_uncorr[j] = -1.;
		jetmass_ft_uncorr[j] = -1.;
		jetmass_pr_uncorr[j] = -1.;
		tau2tau1[j] = -1.;
		tau1[j] = -1.;
		tau2[j] = -1.;
		tau3[j] = -1.;
		tau4[j] = -1.;
		massdrop_pr_uncorr[j] = -1.; 
		jetpt_uncorr[j] = -1.;
		jetpt[j] = -1.;
		jeteta[j] = -10.;
		jetphi[j] = -10.;
		jete[j] = -1.;
		jetmass[j] = -1.;
		jetmass_tr[j] = -1.;
		jetmass_ft[j] = -1.;
		jetmass_pr[j] = -1.;
		jetarea[j] = -1.;
		jetarea_tr[j] = -1.;
		jetarea_ft[j] = -1.;
		jetarea_pr[j] = -1.;    
		massdrop_pr[j] = -1.;
		jetpt_tr_uncorr[j] = -1.;
		jetpt_tr[j] = -1.;
		jeteta_tr[j] = -10.;
		jetphi_tr[j] = -10.;
		jete_tr[j] = -1.;
		jetpt_ft_uncorr[j] = -1.;
		jetpt_ft[j] = -1.;
		jeteta_ft[j] = -10.;
		jetphi_ft[j] = -10.;
		jete_ft[j] = -1.;
		jetpt_pr_uncorr[j] = -1.;
		jetpt_pr[j] = -1.;
		jeteta_pr[j] = -10.;
		jetphi_pr[j] = -10.;
		jete_pr[j] = -1.;
		jetconstituents[j] = 0;
		jetcharge[j] = 0;

		prsubjet1_px[j] = 0.;
		prsubjet1_py[j] = 0.;
		prsubjet1_pz[j] = 0.;
		prsubjet1_e[j] = 0.;        
		prsubjet2_px[j] = 0.;
		prsubjet2_py[j] = 0.;
		prsubjet2_pz[j] = 0.;
		prsubjet2_e[j] = 0.;        

		for (int k = 0; k < 11; ++k){
			rcores[k][j] = -1.; ptcores[k][j] = -1.;
			planarflow[k][j] = -1.;
		}

		for (int k = 0; k < mQJetsN; ++k){
			qjetmass[k] = 0; qjetmassdrop[k] = 0;
		}
	}


	// ------ get rho --------    
	rhoVal_ = -99.;
	edm::Handle<double> rho;
	const edm::InputTag eventrho(JetsFor_rho, "rho");
	iEvent.getByLabel(eventrho,rho);
	rhoVal_ = *rho;

	// ------ get nPV: primary/secondary vertices------ 
	nPV_ = 0.;
	double nPVval = 0;
	edm::Handle <edm::View<reco::Vertex> > recVtxs;
	iEvent.getByLabel( mPrimaryVertex, recVtxs);
	for(unsigned int ind=0;ind<recVtxs->size();ind++){
		if (!((*recVtxs)[ind].isFake()) && ((*recVtxs)[ind].ndof()>=4) 
					&& (fabs((*recVtxs)[ind].z())<=24.0) &&  
					((*recVtxs)[ind].position().Rho()<=2.0) ) {
			nPVval += 1;
		}
	}
	nPV_ = nPVval;

	// ----------------------------
	// ------ start processing ------    
	std::cout << "FJparticles.size() = " << FJparticles.size() << std::endl; 
	if (FJparticles.size() < 1) return;


	std::vector<float>  PF_id_handle;
	PF_id_handle.clear();
	charge_handle_Gen.clear();

	for(size_t i = 0; i < FJparticles.size(); ++ i) {
		fastjet::PseudoJet  P = FJparticles[i];
		PF_id_handle.push_back(P.user_info<PseudoJetUserInfo>().pdg_id());
		if(isGenJ)charge_handle_Gen.push_back(P.user_info<PseudoJetUserInfo>().charge());
	}

	// do re-clustering
	fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, mJetRadius);
	if (mJetAlgo == "AK") jetDef.set_jet_algorithm( fastjet::antikt_algorithm );
	else if (mJetAlgo == "CA") jetDef.set_jet_algorithm( fastjet::cambridge_algorithm );
	else throw cms::Exception("GroomedJetFiller") << " unknown jet algorithm " << std::endl;

	int activeAreaRepeats = 1;
	double ghostArea = 0.01;
	double ghostEtaMax = 5.0;
	// fastjet::ActiveAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
	fastjet::GhostedAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
	fjActiveArea.set_fj2_placement(true);
	fastjet::AreaDefinition fjAreaDefinition( fastjet::active_area_explicit_ghosts, fjActiveArea );
	fastjet::ClusterSequenceArea thisClustering(FJparticles, jetDef, fjAreaDefinition);

	std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(50.0));
	if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001)
	  out_jets = sorted_by_pt(thisClustering.inclusive_jets(20.0));
	fastjet::ClusterSequence thisClustering_basic(FJparticles, jetDef);
	std::vector<fastjet::PseudoJet> out_jets_basic = sorted_by_pt(thisClustering_basic.inclusive_jets(50.0));
	if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001)
	  out_jets_basic = sorted_by_pt(thisClustering_basic.inclusive_jets(20.0));    

	// define groomers
	fastjet::Filter trimmer( fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.03)));
	fastjet::Filter filter( fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.3), fastjet::SelectorNHardest(3)));
	fastjet::Pruner pruner(fastjet::cambridge_algorithm, 0.1, 0.5);

	std::vector<fastjet::Transformer const *> transformers;
	transformers.push_back(&trimmer);
	transformers.push_back(&filter);
	transformers.push_back(&pruner);

	// define n-subjettiness
	//    NsubParameters paraNsub = NsubParameters(mNsubjettinessKappa, mJetRadius);   
	//    Nsubjettiness routine(nsub_kt_axes, paraNsub);
	//    Nsubjettiness routine(nsub_1pass_from_kt_axes, paraNsub);

	// Defining Nsubjettiness parameters
	double beta = mNsubjettinessKappa; // power for angular dependence, e.g. beta = 1 --> linear k-means, beta = 2 --> quadratic/classic k-means
	double R0 = mJetRadius; // Characteristic jet radius for normalization	      
	double Rcut = mJetRadius; // maximum R particles can be from axis to be included in jet	      

	//    fastjet::Nsubjettiness nSub1KT(1, Njettiness::kt_axes, beta, R0, Rcut);

	// -----------------------------------------------
	// -----------------------------------------------
	// s t a r t   l o o p   o n   j e t s
	// -----------------------------------------------
	// -----------------------------------------------
	//      cout<<mJetAlgo<<"\t"<<mJetRadius<<endl;


	for (unsigned j = 0; j < out_jets.size()&&int(j)<NUM_JET_MAX; j++) {

		if (mSaveConstituents && j==0){
			if (out_jets_basic.at(j).constituents().size() >= 100) nconstituents0 = 100;
			else nconstituents0 = (int) out_jets_basic.at(j).constituents().size();
			std::vector<fastjet::PseudoJet> cur_constituents = sorted_by_pt(out_jets_basic.at(j).constituents());
			for (int aa = 0; aa < nconstituents0; aa++){        
				constituents0_eta[aa] = cur_constituents.at(aa).eta();
				constituents0_phi[aa] = cur_constituents.at(aa).phi();                
				constituents0_e[aa] = cur_constituents.at(aa).e();                                
			}
		}

		if( !(j< (unsigned int) NUM_JET_MAX) ) break;            
		jetmass_uncorr[j] = out_jets.at(j).m();
		jetpt_uncorr[j] = out_jets.at(j).pt();
		TLorentzVector jet_corr = getCorrectedJet(out_jets.at(j));
		jetmass[j] = jet_corr.M();
		jetpt[j] = jet_corr.Pt();
		jeteta[j] = jet_corr.Eta();
		jetphi[j] = jet_corr.Phi();
		jete[j]   = jet_corr.Energy();
		jetarea[j] = out_jets.at(j).area();
		jetconstituents[j] = out_jets_basic.at(j).constituents().size();

		// pruning, trimming, filtering  -------------
		int transctr = 0;
		for ( std::vector<fastjet::Transformer const *>::const_iterator 
					itransf = transformers.begin(), itransfEnd = transformers.end(); 
					itransf != itransfEnd; ++itransf ) {  

			fastjet::PseudoJet transformedJet = out_jets.at(j);
			transformedJet = (**itransf)(transformedJet);

			fastjet::PseudoJet transformedJet_basic = out_jets_basic.at(j);
			transformedJet_basic = (**itransf)(transformedJet_basic);


			if (transctr == 0){ // trimmed
				jetmass_tr_uncorr[j] = transformedJet.m();
				jetpt_tr_uncorr[j] = transformedJet.pt();
				TLorentzVector jet_tr_corr = getCorrectedJet(transformedJet);
				jetmass_tr[j] = jet_tr_corr.M();
				jetpt_tr[j] = jet_tr_corr.Pt();
				jeteta_tr[j] = jet_tr_corr.Eta();
				jetphi_tr[j] = jet_tr_corr.Phi();
				jete_tr[j]   = jet_tr_corr.Energy();
				jetarea_tr[j] = transformedJet.area();
			}
			else if (transctr == 1){ // filtered
				jetmass_ft_uncorr[j] = transformedJet.m();
				jetpt_ft_uncorr[j] = transformedJet.pt();
				TLorentzVector jet_ft_corr = getCorrectedJet(transformedJet);
				jetmass_ft[j] = jet_ft_corr.M();
				jetpt_ft[j] = jet_ft_corr.Pt();
				jeteta_ft[j] = jet_ft_corr.Eta();
				jetphi_ft[j] = jet_ft_corr.Phi();
				jete_ft[j]   = jet_ft_corr.Energy();
				jetarea_ft[j] = transformedJet.area();                    
			}
			else if (transctr == 2){ // pruned
				jetmass_pr_uncorr[j] = transformedJet.m();
				jetpt_pr_uncorr[j] = transformedJet.pt();
				TLorentzVector jet_pr_corr = getCorrectedJet(transformedJet);
				jetmass_pr[j] = jet_pr_corr.M();
				jetpt_pr[j] = jet_pr_corr.Pt();
				jeteta_pr[j] = jet_pr_corr.Eta();
				jetphi_pr[j] = jet_pr_corr.Phi();
				jete_pr[j]   = jet_pr_corr.Energy();
				jetarea_pr[j] = transformedJet.area();                    
				//decompose into requested number of subjets:
				if (transformedJet_basic.constituents().size() > 1){
					int nsubjetstokeep = 2;
					std::vector<fastjet::PseudoJet> subjets = transformedJet_basic.associated_cluster_sequence()->exclusive_subjets(transformedJet_basic,nsubjetstokeep);    

					//                    for (unsigned k = 0; k < subjets.size(); k++) {
					//                        std::cout << "subjet " << k << ": mass = " << subjets.at(k).m() << " and pt = " << subjets.at(k).pt() << std::endl;
					//                    }
					TLorentzVector sj1( subjets.at(0).px(),subjets.at(0).py(),subjets.at(0).pz(),subjets.at(0).e());
					TLorentzVector sj2( subjets.at(1).px(),subjets.at(1).py(),subjets.at(1).pz(),subjets.at(1).e());     

					prsubjet1_px[j] = subjets.at(0).px(); prsubjet1_py[j] = subjets.at(0).py(); prsubjet1_pz[j] = subjets.at(0).pz(); prsubjet1_e[j] = subjets.at(0).e();
					prsubjet2_px[j] = subjets.at(1).px(); prsubjet2_py[j] = subjets.at(1).py(); prsubjet2_pz[j] = subjets.at(1).pz(); prsubjet2_e[j] = subjets.at(1).e();                    

					TLorentzVector fullj = sj1 + sj2;

					if (subjets.at(0).m() >= subjets.at(1).m()){
						massdrop_pr_uncorr[j] = subjets.at(0).m()/transformedJet.m();
						massdrop_pr[j] = (subjets.at(0).m()/jetmass_pr[j]);                        
					}
					else{
						massdrop_pr_uncorr[j] = subjets.at(1).m()/transformedJet.m();
						massdrop_pr[j] = (subjets.at(1).m()/jetmass_pr[j]);                                    
					}
				}

				// pruned tests
				if (mSaveConstituents && j==0){
					if (transformedJet_basic.constituents().size() >= 100) nconstituents0pr = 100;
					else nconstituents0pr = (int) transformedJet_basic.constituents().size();
					std::vector<fastjet::PseudoJet> cur_constituentspr = sorted_by_pt(transformedJet_basic.constituents());
					for (int aa = 0; aa < nconstituents0pr; aa++){        
						constituents0pr_eta[aa] = cur_constituentspr.at(aa).eta();
						constituents0pr_phi[aa] = cur_constituentspr.at(aa).phi();                
						constituents0pr_e[aa] = cur_constituentspr.at(aa).e();                                
					}
				}
			}
			else{ std::cout << "error in number of transformers" << std::endl;}                    
			transctr++;
		}        

		//std::cout<< "Beging the n-subjettiness computation" << endl; 
		// n-subjettiness  -------------
		//        tau1[j] = routine.getTau(1, out_jets.at(j).constituents()); 
		//        tau2[j] = routine.getTau(2, out_jets.at(j).constituents());
		//        tau3[j] = routine.getTau(3, out_jets.at(j).constituents());
		//        tau4[j] = routine.getTau(4, out_jets.at(j).constituents());
		//        tau2tau1[j] = tau2[j]/tau1[j];
		//        fastjet::Nsubjettiness nSub1KT(1, Njettiness::kt_axes, beta, R0, Rcut);
		fastjet::Nsubjettiness nSub1KT(1, Njettiness::onepass_kt_axes, beta, R0, Rcut);
		tau1[j] = nSub1KT(out_jets.at(j));
		fastjet::Nsubjettiness nSub2KT(2, Njettiness::onepass_kt_axes, beta, R0, Rcut);
		tau2[j] = nSub2KT(out_jets.at(j));
		fastjet::Nsubjettiness nSub3KT(3, Njettiness::onepass_kt_axes, beta, R0, Rcut);
		tau3[j] = nSub3KT(out_jets.at(j));
		fastjet::Nsubjettiness nSub4KT(4, Njettiness::onepass_kt_axes, beta, R0, Rcut);
		tau4[j] = nSub4KT(out_jets.at(j));
		tau2tau1[j] = tau2[j]/tau1[j];


		//std::cout<< "End the n-subjettiness computation" << endl;
		// cores computation  -------------
		//std::cout<< "Beging the core computation" << endl;
		std::vector<fastjet::PseudoJet> constits = thisClustering.constituents(out_jets.at(j));
		for (int kk = 0; kk < 11; ++kk){
			double coreCtr = (double) kk;    
			if (coreCtr < mJetRadius*10.){
				float tmpm = 0, tmppt = 0;
				computeCore( constits, coreCtr/10., tmpm, tmppt );
				if (tmpm > 0) rcores[kk][j] = tmpm/out_jets.at(j).m();
				if (tmppt > 0) ptcores[kk][j] = tmppt/out_jets.at(j).pt();
			}
		}
		//std::cout<< "Ending the core computation" << endl;

		//std::cout<< "Beging the planarflow computation" << endl;

		//planarflow computation
		for (int kk = 0; kk < 11; ++kk){
			double coreCtr = (double) (kk + 1);
			if (coreCtr < mJetRadius*10.){
				float tmppflow = 0;
				computePlanarflow(constits,coreCtr/10.,out_jets.at(j),mJetAlgo,tmppflow);
				planarflow[kk][j] = tmppflow;
			}
		}

		//std::cout<< "Ending the planarflow computation" << endl;

		// qjets computation  -------------
		if ((mDoQJets)&&(j == 0)){ // do qjets only for the hardest jet in the event!
			double zcut(0.1), dcut_fctr(0.5), exp_min(0.), exp_max(0.), rigidity(0.1);                
			QjetsPlugin qjet_plugin(zcut, dcut_fctr, exp_min, exp_max, rigidity);
			fastjet::JetDefinition qjet_def(&qjet_plugin);
			vector<fastjet::PseudoJet> constits;
			unsigned int nqjetconstits = out_jets_basic.at(j).constituents().size();
			if (nqjetconstits < (unsigned int) mQJetsPreclustering) constits = out_jets_basic.at(j).constituents();
			else constits = out_jets_basic.at(j).associated_cluster_sequence()->exclusive_subjets_up_to(out_jets_basic.at(j),mQJetsPreclustering);
			for(unsigned int ii = 0 ; ii < (unsigned int) mQJetsN ; ii++){
				fastjet::ClusterSequence qjet_seq(constits, qjet_def);
				vector<fastjet::PseudoJet> inclusive_jets2 = sorted_by_pt(qjet_seq.inclusive_jets(50.0));
				if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001)
				  inclusive_jets2 = sorted_by_pt(qjet_seq.inclusive_jets(20.0));

				if (inclusive_jets2.size()>0) {
					qjetmass[ii] = inclusive_jets2[0].m();
					if (inclusive_jets2[0].constituents().size() > 1){
						vector<fastjet::PseudoJet> subjets_qjet = qjet_seq.exclusive_subjets(inclusive_jets2[0],2);
						if (subjets_qjet.at(0).m() >= subjets_qjet.at(1).m()){
							qjetmassdrop[ii] = (subjets_qjet.at(0).m()/inclusive_jets2[0].m());                        
						}
						else{
							qjetmassdrop[ii] = (subjets_qjet.at(1).m()/inclusive_jets2[0].m());                                    
						}
					}
					else{
						qjetmassdrop[ii] = 1.;
					}
				}else{
					qjetmassdrop[ii] = 1.;
				}

			}
		}
		// jet charge try (?) computation  -------------
		std::vector< float > pdgIds;
		for (unsigned ii = 0; ii < out_jets_basic.at(j).constituents().size(); ii++){
			for (unsigned jj = 0; jj < FJparticles.size(); jj++){
				//                std::cout << ii << ", " << jj << ": " << FJparticles.at(jj).pt() << ", " << out_jets_basic.at(j).constituents().at(ii).pt() << std::endl;
				if (FJparticles.at(jj).pt() == out_jets_basic.at(j).constituents().at(ii).pt()){
					pdgIds.push_back(PF_id_handle.at(jj));
					/*if(!isGenJ) {
						if(mJetAlgo == "AK" && fabs(mJetRadius-0.5)<0.001) {
							pdgIds.push_back(PF_id_handle_AK5.at(jj));
						}else{
							pdgIds.push_back(PF_id_handle->at(jj));
						}
					}else{
						pdgIds.push_back(PF_id_handle_Gen.at(jj));
					}*/
					break;
				}
			}
		}
		jetcharge[j] = computeJetCharge(out_jets_basic.at(j).constituents(),pdgIds,out_jets_basic.at(j).e());

	}

}  



double ewk::GroomedJetFiller::getJEC(double curJetEta, double curJetPt, double curJetE, double curJetArea){
	// Jet energy corrections, something like this...
	jec_->setJetEta( curJetEta );
	jec_->setJetPt ( curJetPt );
	jec_->setJetE  ( curJetE );
	jec_->setJetA  ( curJetArea );
	jec_->setRho   ( rhoVal_ );
	jec_->setNPV   ( nPV_ );
	double corr = jec_->getCorrection();
	return corr;
}

TLorentzVector ewk::GroomedJetFiller::getCorrectedJet(fastjet::PseudoJet& jet) {
	double jecVal = 1.0;

	if(applyJECToGroomedJets_ && !isGenJ) 
	  jecVal = getJEC( jet.eta(), jet.pt(), jet.e(), jet.area() );   

	TLorentzVector jet_corr(jet.px() * jecVal, 
				jet.py() * jecVal, 
				jet.pz() * jecVal, 
				jet.e() * jecVal);
	return jet_corr;
}

void ewk::GroomedJetFiller::computeCore( std::vector<fastjet::PseudoJet> constits, double Rval, float &m_core, float &pt_core ){

	fastjet::JetDefinition jetDef_rcore(fastjet::cambridge_algorithm, Rval);
	fastjet::ClusterSequence thisClustering(constits, jetDef_rcore);

	std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(0.0));
	m_core = out_jets.at(0).m();
	pt_core = out_jets.at(0).pt();

}

void ewk::GroomedJetFiller::computePlanarflow(std::vector<fastjet::PseudoJet> constits, double Rval, fastjet::PseudoJet jet,std::string mJetAlgo, float &planarflow){

	fastjet::JetDefinition jetDef_rplanarflow(fastjet::cambridge_algorithm,Rval);
	if (mJetAlgo == "AK") jetDef_rplanarflow.set_jet_algorithm( fastjet::antikt_algorithm );
	else if (mJetAlgo == "CA") jetDef_rplanarflow.set_jet_algorithm( fastjet::cambridge_algorithm );
	else throw cms::Exception("GroomedJetFiller") << " unknown jet algorithm " << std::endl;
	fastjet::ClusterSequence thisClustering(constits, jetDef_rplanarflow);

	//reclustering jets
	std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(0.0));

	//leading sub jet constits mass not equal Zero
	float mJ = jet.m();
	if(mJ != 0)
	{
		std::vector<fastjet::PseudoJet> subconstits = thisClustering.constituents(out_jets.at(0)); 

		TLorentzVector jetp4;
		//jetp4.SetPxPyPzE(out_jets.at(0).px(),out_jets.at(0).py(),out_jets.at(0).pz(),out_jets.at(0).e());
		jetp4.SetPxPyPzE(jet.px(),jet.py(),jet.pz(),jet.e());

		TVector3 zaxis = jetp4.Vect().Unit();
		TVector3 zbeam(0, 0, 1);

		//Transverse component (X, Y) relative to the jet(Z) axis
		TVector3 xaxis = (zaxis.Cross(zbeam)).Unit();
		TVector3 yaxis = (xaxis.Cross(zaxis)).Unit();

		double I[3][3];
		for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++) I[i][j] = 0;

		int matrixsize = subconstits.size();

		for(int k = 0; k < matrixsize; k++)
		{   
			TLorentzVector tmpjetk;
			tmpjetk.SetPxPyPzE(subconstits.at(k).px(),subconstits.at(k).py(),subconstits.at(k).pz(),subconstits.at(k).e());
			float tmp_px = tmpjetk.Vect().Dot(xaxis);
			float tmp_py = tmpjetk.Vect().Dot(yaxis);
			//Avoid Too Samll Energy
			if(subconstits.at(k).e() >= 0.001){

				I[1][1] += tmp_px * tmp_px / (mJ * subconstits.at(k).e());
				I[1][2] += tmp_px * tmp_py / (mJ * subconstits.at(k).e());
				I[2][1] += tmp_py * tmp_px / (mJ * subconstits.at(k).e());
				I[2][2] += tmp_py * tmp_py / (mJ * subconstits.at(k).e());

			}
		}

		//From arXiv 1012.2077
		planarflow = 4*(I[1][1]*I[2][2] - I[1][2]*I[2][1])/((I[1][1]+I[2][2])*(I[1][1]+I[2][2])); 
	}
}

float ewk::GroomedJetFiller::computeJetCharge( std::vector<fastjet::PseudoJet> constits, std::vector<float> pdgIds, float Ejet ){

	float val = 0.;
	for (unsigned int i = 0; i < pdgIds.size(); i++){
		float qq ;
		if(isGenJ) {
			qq = charge_handle_Gen.at(i);
		}else{
			qq = getPdgIdCharge( pdgIds.at(i) );
		}
		val += qq*pow(constits.at(i).e(),mJetChargeKappa);
	}
	val /= Ejet;
	return val;

}

float ewk::GroomedJetFiller::getPdgIdCharge( float fid ){

	float qq = -99.;
	int id = (int) fid;
	if (std::find(neutrals.begin(), neutrals.end(), id) != neutrals.end()){
		qq = 0.;
	}
	else if (std::find(positives.begin(), positives.end(), id) != positives.end()){
		qq = 1.;
	}
	else if (std::find(negatives.begin(), negatives.end(), id) != negatives.end()){
		qq = -1.;
	}
	else{
		BREAK("unknown PDG id");
		throw cms::Exception("GroomedJetFiller") << " unknown PDG id " << id << std::endl;
	}
	return qq;
}




