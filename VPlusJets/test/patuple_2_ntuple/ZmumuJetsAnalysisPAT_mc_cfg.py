import FWCore.ParameterSet.Config as cms
import pprint
#isMC = False
isMC = True

process = cms.Process("demo")

##---------  Load standard Reco modules ------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')



##----- this config frament brings you the generator information ----
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("Configuration.StandardSequences.Generator_cff")


##----- Detector geometry : some of these needed for b-tag -------
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")


##----- B-tags --------------
process.load("RecoBTag.Configuration.RecoBTag_cff")


##----- Global tag: conditions database ------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

##----- Counter module ------------
process.load("ElectroWeakAnalysis.VPlusJets.AllPassFilter_cfi")

## import skeleton process
#from PhysicsTools.PatAlgos.patTemplate_cfg import *


############################################
if not isMC:
    process.GlobalTag.globaltag = 'GR_R_53_V10::All'
else:
    #process.GlobalTag.globaltag = 'START53_V7E::All'
    process.GlobalTag.globaltag = 'START53_V26::All'

OutputFileName = "zmumujetsanalysisntuple.root"
#numEventsToRun = 2000
#numEventsToRun =  20000
numEventsToRun = -1
############################################
########################################################################################
########################################################################################

##---------  Z-->mumu Collection ------------
process.load("ElectroWeakAnalysis.VPlusJets.ZmumuCollectionsPAT_cfi")

##---------  Jet Collection ----------------
#process.load("ElectroWeakAnalysis.VPlusJets.JetCollectionsPAT_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.B2GJetCollectionsPAT_cfi")

##---------  Vertex and track Collections -----------
process.load("ElectroWeakAnalysis.VPlusJets.TrackCollections_cfi")
#


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

#process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound')
#)
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
	   #'file:/uscms_data/d3/zixu/BoostJet/ZPlusJets_jetsubstructure/July29/B2G/CMSSW_5_3_8_patch1/src/TopQuarkAnalysis/TopPairBSM/test/tlbsm_53x_v3_mc.root'
       #'/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_118_1_C5o.root'
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_101_1_LmQ.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_102_1_ZUF.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_103_1_9Ws.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_104_1_zRb.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_105_1_PnP.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_106_1_Tp7.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_107_1_YWN.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_108_1_sC5.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_109_1_dp6.root',
       #'/store/user/bparida/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_100_1_8Xh.root'
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_100_1_zkU.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_101_1_HLF.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_102_1_1EG.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_103_1_ZKL.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_104_1_TJW.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_105_1_Sng.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_106_1_mUf.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_107_1_ogj.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_108_1_BX2.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_109_1_9IX.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_110_1_cjH.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_111_1_Ach.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_112_1_Vx7.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_113_1_vQl.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_114_1_bpx.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_115_1_MYk.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_116_1_jvw.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_117_1_iQz.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_118_1_ENJ.root',
       '/store/user/zixu/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_v0/d1dd4fe4091510a6bd0f3de124216781/tlbsm_53x_v3_mc_10_1_ZF8.root'
) )




##-------- Electron events of interest --------
process.HLTMu =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     #HLTPaths = cms.vstring('HLT_IsoMu24_*','HLT_IsoMu30_*'),
     HLTPaths = cms.vstring("HLT_Mu5*", "HLT_Mu7*", "HLT_Mu9*", "HLT_Mu11*", "HLT_Mu15*", "HLT_Mu24*", "HLT_Mu13_Mu8_*", "HLT_Mu*_Mu*", "HLT_DoubleMu*"),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
     throw = cms.bool(False) # throw exception on unknown path names
)

process.primaryVertex.src = cms.InputTag("goodOfflinePrimaryVertices");
process.primaryVertex.cut = cms.string(" ");

###########################################
### Filter to require at least two jets in the event
#process.RequireTwoJetsORboostedV = cms.EDFilter("JetsORboostedV",
#    minNumber = cms.untracked.int32(2),
#    maxNumber = cms.untracked.int32(100),
#    srcJets = cms.InputTag("ak5PFJetsLooseIdAll"),
#    srcVectorBoson = cms.InputTag("bestZmumu"),
#    minVpt = cms.untracked.double(100.),
#    minNumberPhotons = cms.untracked.int32(0)
#)
#process.RequireTwoJetsORboostedVStep = process.AllPassFilter.clone()


##-------- Save V+jets trees --------
process.VplusJets = cms.EDAnalyzer("VplusJetsAnalysis", 
    TreeName    = cms.string('ZJet'), # ZJet or Dijets
    jetType = cms.string("PF"),
  #  srcPFCor = cms.InputTag("selectedPatJetsPFlow"),
    srcPFCor = cms.InputTag("ak5PFJetsLooseId"),
    srcPhoton = cms.InputTag("photons"),
    IsoValPhoton = cms.VInputTag(cms.InputTag('phoPFIso:chIsoForGsfEle'), cms.InputTag('phoPFIso:phIsoForGsfEle'), cms.InputTag('phoPFIso:nhIsoForGsfEle'),),
    srcPFCorVBFTag = cms.InputTag("ak5PFJetsLooseIdVBFTag"), 
    srcVectorBoson = cms.InputTag("bestZmumu"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('muon'),                          
    srcPrimaryVertex = cms.InputTag("goodOfflinePrimaryVertices"),                               
    runningOverMC = cms.bool(isMC),			
    runningOverAOD = cms.bool(False),			
    #srcMet = cms.InputTag("patType1CorrectedPFMet"),
    srcMet = cms.InputTag("patMetShiftCorrected"), # type1 + shift corrections
    #srcMetMVA = cms.InputTag("pfMEtMVA"),
    srcGen  = cms.InputTag("ak5GenJets"),
    srcMuons  = cms.InputTag("selectedPatMuonsPFlow"),
    srcBeamSpot  = cms.InputTag("offlineBeamSpot"),
    srcCaloMet  = cms.InputTag("patMETs"),
    srcgenMet  = cms.InputTag("genMetTrue"),
    srcGenParticles  = cms.InputTag("genParticles"),
    srcTcMet    = cms.InputTag("patMETsAK5TC"),
    srcJetsforRho = cms.string("kt6PFJetsPFlow"),                               
    #srcJetsforRho_lepIso = cms.string("kt6PFJetsForIsolation"),       
    #srcJetsforRhoCHS = cms.string("kt6PFJetsChsPFlow"),
    #srcJetsforRho_lepIsoCHS = cms.string("kt6PFJetsChsForIsolationPFlow"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),
    bTagger=cms.string("simpleSecondaryVertexHighEffBJetTags"),

    applyJECToGroomedJets=cms.bool(True),
    doGroomedAK5 = cms.bool(True),
    doGroomedAK7 = cms.bool(False),
    doGroomedAK8 = cms.bool(True),
    doGroomedAK12= cms.bool(True),
    doGroomedCA8 = cms.bool(False),
    doGroomedCA12= cms.bool(False),
    GroomedJet_doQJets = cms.bool(False)#drop QJets process
)

if isMC:
    #process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("START53_V15")
    #process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("START53_V7G")
    process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("START53_V26")
else:
    process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("FT_53_V10_AN3")
    #process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("GR_R_53_V10")



process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string( OutputFileName ),
    closeFileFast = cms.untracked.bool(False)
)


##
## MET shift correction on the fly
##
#from PhysicsTools.PatAlgos.tools.metTools import addPfMET

process.load("JetMETCorrections/Type1MET/pfMETsysShiftCorrections_cfi")
process.pfMEtSysShiftCorr.src = cms.InputTag('patMETsPFlow')
process.pfMEtSysShiftCorr.srcMEt = cms.InputTag('patMETsPFlow')
process.pfMEtSysShiftCorr.srcJets = cms.InputTag('selectedPatJetsPFlow')

if isMC:
# pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_mc
  process.pfMEtSysShiftCorr.parameter = cms.PSet(
    numJetsMin = cms.int32(-1),
    numJetsMax = cms.int32(-1),
    px = cms.string("+2.22335e-02 - 6.59183e-02*Nvtx"),
    py = cms.string("+1.52720e-01 - 1.28052e-01*Nvtx")
  )
else:
# pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_data
  process.pfMEtSysShiftCorr.parameter = cms.PSet(
    numJetsMin = cms.int32(-1),
    numJetsMax = cms.int32(-1),
    px = cms.string("+1.68804e-01 + 3.37139e-01*Nvtx"),
    py = cms.string("-1.72555e-01 - 1.79594e-01*Nvtx")
  )

process.patMetShiftCorrected = cms.EDProducer("CorrectedPATMETProducer",
    src = cms.InputTag('patMETsPFlow'),
    applyType1Corrections = cms.bool(True),
    srcType1Corrections = cms.VInputTag(
        cms.InputTag('pfMEtSysShiftCorr')
    ),
    applyType2Corrections = cms.bool(False)
)   

process.myseq = cms.Sequence(
    process.pfMEtSysShiftCorrSequence *
    process.patMetShiftCorrected *
    process.TrackVtxPath *
    process.HLTMu *
    process.ZPath*
    #process.GenJetPath *
    #process.TagJetPath *
    process.B2GPFJetPath 
    #process.RequireTwoJetsORboostedV *
    #process.RequireTwoJetsORboostedVStep
    )

if isMC:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HBHENoiseFilter)
    process.myseq.remove ( process.HLTMu)
else:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HBHENoiseFilter)
    #process.myseq.remove ( process.GenJetPath)
    #process.myseq.remove ( process.TagJetPath)


##---- if do not want to require >= 2 jets then disable that filter ---
##process.myseq.remove ( process.RequireTwoJets)  

#process.outpath.remove(process.out)
process.p = cms.Path( process.myseq  * process.VplusJets)
