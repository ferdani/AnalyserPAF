//////////////////////////////////////////////////////////////////////////////////////
//
//  EventBuilder: compute all important variables related with the event
//  Triggers, PU reweighting, global SFs and systematics...
//
//  To do: Add trigger SFs when become available
//         Add LHE weights when become available
//         Update PU weights
//
//////////////////////////////////////////////////////////////////////////////////////


#include "EventBuilder.h"

ClassImp(EventBuilder);
EventBuilder::EventBuilder() : PAFChainItemSelector() {}
void EventBuilder::Summary(){}


// Select your triggers
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Bool_t EventBuilder::PassesDoubleElecTrigger(){
	Bool_t pass = false;
	if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec)
		pass = (
				Get<Int_t>("HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v") ||
				Get<Int_t>("HLT_BIT_HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v") );
	return pass;
}

Bool_t EventBuilder::PassesDoubleMuonTrigger(){
	Bool_t pass = false;
	if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec)
		pass = (
				Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v") || 
				Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") );
	return pass;
}

Bool_t EventBuilder::PassesElMuTrigger(){
	Bool_t pass = false;
	if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec)
		pass = (
				Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
				Get<Int_t>("HLT_BIT_HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v") );
  return pass;
}

Bool_t EventBuilder::PassesSingleElecTrigger(){
  Bool_t pass = false;
	if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec)
		pass =  Get<Int_t>("HLT_BIT_HLT_Ele27_WPTight_Gsf_v");
	return pass;
}

Bool_t EventBuilder::PassesSingleMuonTrigger(){
	Bool_t pass = false;
	if(gSelection == iStopSelec || gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec)
		pass = (
				Get<Int_t>("HLT_BIT_HLT_IsoTkMu24_v") ||
				Get<Int_t>("HLT_BIT_HLT_IsoMu24_v") );
	return pass;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void EventBuilder::Initialise(){
	gIsData = GetParam<Bool_t>("IsData");
	gSelection = GetParam<Int_t>("iSelection");
  gSampleName  = GetParam<TString>("sampleName");
  gIsMCatNLO   = GetParam<Bool_t>("IsMCatNLO");
  gChannel = -1;

  selLeptons = std::vector<Lepton>();
  
  gIsDoubleElec = false; gIsDoubleMuon = false; gIsSingleElec = false;
  gIsSingleMuon = false; gIsMuonEG = false;
       if(gSampleName.Contains("SingleMuon")) gIsDoubleElec = true;
  else if(gSampleName.Contains("DoubleMuon")) gIsDoubleMuon = true;
  else if(gSampleName.Contains("SingleElec")) gIsSingleElec = true;
  else if(gSampleName.Contains("SingleMuon")) gIsSingleMuon = true;
  else if(gSampleName.Contains("MuonEG"))     gIsMuonEG     = true;
  
	fPUWeight     = new PUWeight(19664.225, Moriond17MC_PoissonOOTPU, "2016_Moriond17");
	fPUWeightUp   = new PUWeight(18494.9,   Moriond17MC_PoissonOOTPU, "2016_Moriond17"); //  18494.9 
	fPUWeightDown = new PUWeight(20441.7,   Moriond17MC_PoissonOOTPU, "2016_Moriond17"); //  20441.7 

  Weight = GetParam<Float_t>("weight");

	TriggSF       = new LeptonSF();
  // >>>>>>>>>>>>>>> Load histograms for trigger used in analysis  <<<<<<<<<<<<<<<<<<<<<<< Needs to be updated!!!
	// if(gSelection == iStopSelec){
  // Load histograms for trigger SFs here!!!! 
	// }
	passTrigger = 1;
  isSS = 0;
	
	nTrueInt = 0;

	TriggerSF      = 1;
	TriggerSF_err  = 1;
	TriggerSF_Up   = 1;
	TriggerSF_Down = 1;
	PUSF = 1;
	PUSF_Up = 1;
	PUSF_Down = 1;
}

void EventBuilder::InsideLoop(){
  // >>>>>>>>>>>>>> Get selected leptons:
  selLeptons = GetParam<std::vector<Lepton>>("selLeptons");
  // Set channel
  if(selLeptons.size()>=2){
         if(selLeptons.at(0).isElec && selLeptons.at(1).isMuon) gChannel = 1;
    else if(selLeptons.at(0).isMuon && selLeptons.at(1).isElec) gChannel = 1;
    else if(selLeptons.at(0).isMuon && selLeptons.at(1).isMuon) gChannel = 2;
		else if(selLeptons.at(0).isElec && selLeptons.at(1).isElec) gChannel = 3;
		isSS = (selLeptons[0].charge*selLeptons[1].charge) > 0;
	}
	else{
		gChannel = -1;
		isSS = false;
	}
  passTrigger = false;
       if(gChannel == 1 && TrigElMu()) passTrigger = true;
  else if(gChannel == 2 && TrigMuMu()) passTrigger = true;
  else if(gChannel == 3 && TrigElEl()) passTrigger = true;

	METfilters = PassesMETfilters();

	// >>>>>>>>> Calculate norm weight
	if(gIsMCatNLO) genWeight = Get<Float_t>("genWeight");
	else           genWeight = 1;
	NormWeight = Weight*genWeight;

	// >>>>>>>>> Calculate PU weight and variations  
	if(!gIsData){
		nTrueInt = Get<Float_t>("nTrueInt");  // <<<<<<<<<<<<<<<<<<<<<<<< Needs to be updated
		//PUSF      = fPUWeight    ->GetWeight(nTrueInt);
		//PUSF_Up   = fPUWeightUp  ->GetWeight(nTrueInt);
		//PUSF_Down = fPUWeightDown->GetWeight(nTrueInt);
		PUSF      = 1;
		PUSF_Up   = 1;
		PUSF_Down = 1;
	} else{
		PUSF      = 1;
		PUSF_Up   = 1;
		PUSF_Down = 1;
  }

  // >>>>>>>>> Calculate Trigger SF and variations <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Trigger SFs... needs to be updated!!!!
	// ### 2 LEPTONS
	TriggerSF = 1; TriggerSF_Up = 1; TriggerSF_Down = 1;
  /*if(selLeptons.size() < 2) continue; // At least 2 selected leptons
  
  if     (selLeptons[0].IsMuon && selLepton[1].IsMuon) gChannel = iMuon
  else if(selLeptons[0].IsElec && selLepton[1].IsElec) gChannel = iElec;
  else                                                 gChannel = iElMu;

  if     (gChannel == iMuon){  // µµ channel
    passTrigger = PassDoubleMuonTrigger();
		TriggerSF      = TrigSF->GetTrigDoubleMuSF(    SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
		TriggerSF_err  = TrigSF->GetTrigDoubleMuSF_err(SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
  }
  else if(gChannel == iElec){  // ee channel 
    passTrigger    = PassDoubleElecTrigger();
		TriggerSF      = TrigSF->GetTrigDoubleElSF(    SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
		TriggerSF_err  = TrigSF->GetTrigDoubleElSF_err(SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
  }
  else{  // eµ channel
    passTrigger = PassElMuTrigger();
		TriggerSF      = TrigSF->GetTrigElMuSF(    SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
		TriggerSF_err  = TrigSF->GetTrigElMuSF_err(SelLeptons[0].p.Eta(), SelLeptons[1].p.Eta());  
  }
  TriggerSF_Down = TriggerSF-TriggerSF_err;
  TriggerSF_Up   = TriggerSF+TriggerSF_err;
  */

  // Set Params to pass all the info...
  SetParam("TriggerSF",      TriggerSF);
  SetParam("TriggerSF_Up",   TriggerSF_Up);
  SetParam("TriggerSF_Down", TriggerSF_Down);
  SetParam("PUSF",      PUSF);
  SetParam("PUSF_Up",   PUSF_Up);
  SetParam("PUSF_Down", PUSF_Down);

  SetParam("gChannel",        gChannel);
  SetParam("NormWeight",     NormWeight);
  SetParam("passTrigger",     passTrigger);
  SetParam("isSS",            isSS);
  SetParam("METfilters",      METfilters);
}

//


Bool_t EventBuilder::TrigElEl(){
  Bool_t pass = false;
  if(gIsData){
    if     (gIsDoubleElec) pass =  PassesDoubleElecTrigger();
    else if(gIsSingleElec) pass = !PassesDoubleElecTrigger() && PassesSingleElecTrigger();
  }
  else pass = PassesDoubleElecTrigger() || PassesSingleElecTrigger(); 
  return pass;
}

Bool_t EventBuilder::TrigMuMu(){
  Bool_t pass = false;
  if(gIsData){
    if     (gIsDoubleMuon) pass =  PassesDoubleMuonTrigger();
    else if(gIsSingleMuon) pass = !PassesDoubleMuonTrigger() && PassesSingleMuonTrigger();
  }
  else pass = PassesDoubleMuonTrigger() || PassesSingleMuonTrigger();
  return pass;
}

Bool_t EventBuilder::TrigElMu(){
  Bool_t pass = false;
  if(gIsData){
         if(gIsMuonEG    ) pass =  PassesElMuTrigger();
    else if(gIsSingleMuon) pass = !PassesElMuTrigger() &&  PassesSingleMuonTrigger(); 
    else if(gIsSingleElec) pass = !PassesElMuTrigger() && !PassesSingleMuonTrigger() && PassesSingleElecTrigger();
  }
  else pass = PassesElMuTrigger() || PassesSingleMuonTrigger() || PassesSingleElecTrigger(); 
  return pass;
}



Bool_t EventBuilder::PassesMETfilters(){
  if(!gIsData) return true; // only for data
  if (Get<Int_t>("Flag_HBHENoiseFilter") && 
      Get<Int_t>("Flag_HBHENoiseIsoFilter") && 
      Get<Int_t>("Flag_EcalDeadCellTriggerPrimitiveFilter") && 
      Get<Int_t>("Flag_goodVertices") && 
			Get<Int_t>("Flag_eeBadScFilter") &&
			Get<Int_t>("Flag_badMuonFilter") && 
			Get<Int_t>("Flag_badChargedHadronFilter") &&  
			Get<Int_t>("Flag_globalTightHalo2016Filter")
		 ){
    return true;
  }
  else return false;
}
