
AlidNdPtUnifiedAnalysisTask* AddTask_Unified_pPb(Int_t cutModeLow=123, Int_t cutModeHigh=131, TString controll = "")
// cut variation for studdy of cut systematics (cutMode == 100 - 119) and Matching Eff (cutMode == 2100 - 2199)
// controll = "vz10" for vz<10 cut
{
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

    if (!mgr){
        Error("AddTask_Unified_pPb", "No analysis manager found.");
        return 0;
    }

    // Switch off all AliInfo (too much output!!!)
    AliLog::SetGlobalLogLevel(AliLog::kError);
    mgr->SetDebugLevel(0);

    TString type = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"
    Bool_t hasMC=(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler()!=0x0);

    AlidNdPtUnifiedAnalysisTask* task125 = NULL; 
    
    for(Int_t cutMode=125; cutMode <= 125; cutMode++)
    {
    AlidNdPtUnifiedAnalysisTask *task = new AlidNdPtUnifiedAnalysisTask(Form("AlidNdPtUnifiedAnalysisTask_%d", cutMode));
    if(cutMode==125) {task125 = task;}

    task->SetUseMC(hasMC);
    if(type.Contains("ESD")) task->SetUseESD();
    else task->SetUseAOD();
    task->SetUseMultiplicity(kFALSE);
    task->SetTriggerMask(AliVEvent::kINT7);
    task->SelectCollisionCandidates(AliVEvent::kINT7); // kINT7 or kMB
    //task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kV0A);
    //   if(hasMC) task->SetMCParticleType(AlidNdPtUnifiedAnalysisTask::ParticleType::kPrimary); //only if MC, particle dependent MC analysis

    /*const Int_t multNbins = 100;
    Double_t binsMult = new Double_t[multNbins+1];
    for (int i=0; i<=multNbins; i++) { binsMult[i] = -0.5 + i; }
    binsMult[multNbins+1] = multNbins; //binsMult hat 100 eintraege, hier wird versucht auf das 101. zuzugreifen
    task->SetBinsMultCent(multNbins,binsMult);*/

  Int_t multNbins = 100;  
  Double_t binsMult[101];
  for (int i=0; i<=multNbins; i++) { binsMult[i] = -0.5 + i; }
  binsMult[100] = 100.;  
  task->SetBinsMultCent(multNbins,binsMult);
 
    // change pt binning
    const Int_t ptNbins = 81;
    Double_t bins[82] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 18.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0, 32.0, 34.0, 36.0, 40.0, 45.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0, 180.0, 200.0};
    Double_t* binsPt = new Double_t[82];
    for (int i=0; i<82; i++) {binsPt[i] = bins[i];}
    task->SetBinsPt(ptNbins-1, binsPt);
    task->SetBinsPtReso(ptNbins-1, binsPt);


    // y shift -0.465409 only for pPb!!!!
    const Int_t EtaNbins = 30;
    //  Double_t binse[31] = {-1.465409,-1.365409,-1.265409,-1.165409,-1.065409,-0.965409,-0.865409,-0.765409,-0.665409,-0.565409,-0.465409,-0.365409,-0.265409,-0.165409,-0.065409,0.034591,0.134591,0.234591,0.334591,0.434591,0.534591,0.634591,0.734591,0.834591,0.934591,1.034591,1.134591,1.234591,1.334591,1.434591,1.534591};
    Double_t* binsEta = new Double_t[EtaNbins+1];
    for (int i=0; i<31; i++) {binsEta[i] = -0.8 + 1.6 * i / 30.;}
    task->SetBinsEta(EtaNbins,binsEta);

    task->SetMinEta(-0.8);//cms -0.3
    task->SetMaxEta(+0.8);//cms +1.3


    task->SetMinPt(0.15);
    task->SetMaxPt(1.e10);

    task->SetMeanXYZv(0.0,0.0,0.0);
    task->SetSigmaMeanXYZv(1.0,1.0,10.0);
    task->SetZvtx(10.);

    task->Set2015data(kTRUE);    //only p-Pb 2013!!!


    //According to 223 for study of systematic uncertanties. Change to fit the default cuts for 5TeV analysis
    //Just like the 4000 but now with intiger increasing numbers.
    //Easier to use.
    if ((cutMode >= 100) && (cutMode <= 199)){

        // TPC
        task->SetTPCRefit(kTRUE);
        task->SetRatioCrossedRowsOverFindableClustersTPC(0.8);
        task->SetMaxchi2perTPCclu(4);
        task->SetFractionSharedClustersTPC(0.4);

        // ITS
        task->SetITSRefit(kTRUE);
        task->SetClusterReqITS(kTRUE);
        task->SetMaxchi2perITSclu(36.);

        // primary selection
        task->SetSigmaToVertex(kFALSE);
        task->SetDCAtoVertexZ(2.0);
        task->SetDCAtoVertexXYPtDep("0.0182+0.0350/pt^1.01");
        task->SetKinkDaughters(kFALSE);
        task->SetMaxChi2TPCConstrained(36.);
        task->SetGeometricalCut(kTRUE , 3, 130 , 1.5 , 0.85 , 0.7 );

        // Swich Low/High for study of systematics
        if(cutMode==101){task->SetMaxchi2perITSclu(25.);}						//	Low		1
        if(cutMode==102){task->SetMaxchi2perITSclu(49.);}						//	High		2
        if(cutMode==103){task->SetMaxchi2perTPCclu(3); }						//	Low		3
        if(cutMode==104){task->SetMaxchi2perTPCclu(5); }						//	High		4
        if(cutMode==105){task->SetRatioCrossedRowsOverFindableClustersTPC(0.7);}			//	Low 		7
        if(cutMode==106){task->SetRatioCrossedRowsOverFindableClustersTPC(0.9);}			//	High		8
        if(cutMode==107){task->SetFractionSharedClustersTPC(0.2);}					//	Low		9
        if(cutMode==108){task->SetFractionSharedClustersTPC(1.0);}					//	High		10
        if(cutMode==109){task->SetMaxChi2TPCConstrained(25.);}					//	LoW		11
        if(cutMode==110){task->SetMaxChi2TPCConstrained(49.);}

        if(cutMode==111){task->SetDCAtoVertexXYPtDep("0.0104+0.0200/pt^1.01");}			//	Low		13
        if(cutMode==112){task->SetDCAtoVertexXYPtDep("0.0260+0.0500/pt^1.01");}			//	High		14
        if(cutMode==113){task->SetDCAtoVertexZ(1.0); }						//	Low		15
        if(cutMode==114){task->SetDCAtoVertexZ(5.0); }						//	High		16
        if(cutMode==115){task->SetClusterReqITS(kFALSE); }	                                         //			17

        if(cutMode==116){task->SetGeometricalCut(kTRUE,3,120,1.5,0.85,0.7);}
        if(cutMode==117){task->SetGeometricalCut(kTRUE,3,140,1.5,0.85,0.7);}

        if(cutMode==118){task->SetGeometricalCut(kTRUE,4,130,1.5,0.85,0.7);}	// Make a varaition of cut on the width of the dead zone
        if(cutMode==119){task->SetGeometricalCut(kTRUE,2,130,1.5,0.85,0.7);}	// Make a varaition of cut on the width of the dead zone

        if(cutMode==120){task->SetGeometricalCut(kTRUE,3,130,1.5,0.80,0.65);}   // Make a variation of cut Nc,Ncl  THE EFFECT IS NEGLIGIBLE
        if(cutMode==121){task->SetGeometricalCut(kTRUE,3,130,1.5,0.9,0.75);}    // Make a variation of cut Nc,Ncl  THE EFFECT IS NEGLIGIBLE

        if(cutMode==122){task->SetGeometricalCut(kTRUE,0,130,1.5,0.85,0.7);}	// Make a variation of cut on the width of the dead zone
        
        // Centrality estimator selection
        if(cutMode==123){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kV0A);}   // 
        if(cutMode==124){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kV0C);}   // 
        if(cutMode==125){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kV0M);}   // V0M is equal to V0A and V0C combined, equal to cutmode 100
        if(cutMode==126){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kCL0);}   // 
        if(cutMode==127){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kCL1);}   // 
        if(cutMode==128){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kSPDt);}  // 
        if(cutMode==129){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kSPDc);}  // 
        if(cutMode==130){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kZNA);}   // 
        if(cutMode==131){task->SetCentralityEstimator(AlidNdPtUnifiedAnalysisTask::WhichCentralityEstimator::kZNC);}   // 
    }


    //Matching Systematic Uncertainty
    if ((cutMode >= 2100) && (cutMode <= 2199)){

        // Calculate matching efficiency: TPC only with Crossed Rows
        task->SetKinkDaughters(kFALSE);
        task->SetTPCRefit(kTRUE);
        task->SetRatioCrossedRowsOverFindableClustersTPC(0.8);
        task->SetMaxchi2perTPCclu(4.0);

        task->SetGeometricalCut(kTRUE , 3, 130 , 1.5 , 0.85 , 0.7 );

        task->SetFractionSharedClustersTPC(0.4);
        task->SetDCAtoVertexXY(2.4);
        task->SetDCAtoVertexZ(3.2);

        task->SetITSRefit(kFALSE);
        task->SetClusterReqITS(kFALSE);

        // Calculate matching efficiency: TPC + ITS with Crossed Rows
        if (cutMode==2101){
            task->SetITSRefit(kTRUE);
            task->SetClusterReqITS(kTRUE);
        }

        // Calculate matching efficiency: TPC + ITS without SPC hit with Crossed Rows
        if (cutMode==2102){
            task->SetITSRefit(kTRUE);
            task->SetClusterReqITS(kFALSE);
        }
    }

    mgr->AddTask(task);

    // Create containers for input
    AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

    TString stContainerName;
    stContainerName = Form("dNdPt_cutMode_%d",cutMode);

    AliAnalysisDataContainer *coutput = mgr->CreateContainer(stContainerName,
            TList::Class(),
            AliAnalysisManager::kOutputContainer,
            mgr->GetCommonFileName());

    mgr->ConnectInput(task, 0, cinput);
    mgr->ConnectOutput(task, 1, coutput);
    }


    return task125;

}





