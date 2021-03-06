/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

float max(float csv1, float  csv2)   
{
if(csv1 > csv2) return csv1;
else return csv2;
}  

float min(float csv1, float  csv2)     
{  
if(csv1 > csv2) return csv2;
else return csv1;
}


double deltaPhi(double phi1, double phi2) 
{ 
    double PI = 3.14159265;
    double result = phi1 - phi2;
    while (result > PI) result -= 2*PI;
    while (result <= -PI) result += 2*PI;
    return result;
}


typedef struct 
{
  float mass;  //MT in case of W
  float pt;
  float eta;
  float phi;
} _TrackInfo;

typedef struct 
{
  float mass; 
  float pt;
  float eta;
  float phi;
  float dR;
  float dPhi;
  float dEta;
} HiggsInfo;
  
typedef struct 
  {
    int run;
    int lumi;
    int event;
    int json;
  } EventInfo;
 
 typedef struct
  {
    float pt;
    float eta;
    float phi;
    float csv;
    float cosTheta;
    int nuTracksSV;
    float chf;
    float nhf;
    float cef;
    float nef;
    float nch;
    float nconstituents;
    float flavour;
    float genPt;
    float genEta;
    float genPhi;
    float JECUnc;
    
  } _JetInfo;

using namespace TMVA;

void ApplicationBOOST_ZH( TString myMethodList = "" ) 
{   

for(int channel=0;channel<18;channel++)
{

int cFlag,bFlag;
float backNum = channel;

#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

_JetInfo Jet1,Jet2;
_TrackInfo V;
HiggsInfo H;
float hJet_pt0,hJet_pt1,hJet_eta0,hJet_eta1,hJet_phi0,hJet_phi1,hJet_csv0,hJet_csv1;
float hJet_pt[2]; float hJet_eta[2]; float hJet_phi[2]; float hJet_csv[2];



//What Did You Train With?
bool trZJets = 0;
bool trZZ    = 0;
bool trTTbar = 0;
bool trALL = 1;

TString *filename;

if(trZJets)
{
if(channel == 0) filename = new TString("ZJetZH.root");
if(channel == 1) filename = new TString("ZJetTT.root");
if(channel == 2) filename = new TString("ZJetZZ.root");
if(channel == 3) filename = new TString("ZJetZJ.root");  
}

if(trZZ)
{
if(channel == 0) filename = new TString("ZZZH.root");
if(channel == 1) filename = new TString("ZZTT.root");
if(channel == 2) filename = new TString("ZZZZ.root");
if(channel == 3) filename = new TString("ZZZJ.root");  
}


if(trALL)
{

if(channel == 0) filename = new TString("TMVA-ZH.root");
if(channel == 1) filename = new TString("TMVA-TT.root");
if(channel == 2) filename = new TString("TMVA-ZZ.root");

if(channel == 3)   filename = new TString("TMVA-Ts.root");
if(channel == 4)   filename = new TString("TMVA-Tt.root");
if(channel == 5)   filename = new TString("TMVA-TtW.root");
if(channel == 6)   filename = new TString("TMVA-Tsb.root");
if(channel == 7)   filename = new TString("TMVA-Ttb.root");
if(channel == 8)   filename = new TString("TMVA-TtWb.root");

if(channel == 9)   filename = new TString("TMVA-WW.root");
if(channel == 10)   filename = new TString("TMVA-WZ.root");
if(channel == 11)   filename = new TString("TMVA-WJ.root");
if(channel == 12)   filename = new TString("TMVA-ZJL.root");
if(channel == 13)   filename = new TString("TMVA-ZJH.root");

if(channel == 14) filename = new TString("TMVA-DATAM.root");
if(channel == 15) filename = new TString("TMVA-DATAA.root");
if(channel == 16) filename = new TString("TMVA-DATA4.root");
if(channel == 17) filename = new TString("TMVA-DATA6.root");


}
   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 1;
   Use["CutsD"]           = 1;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 1;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-trans formed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 1;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 1;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 1; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 1; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 1;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 1;
   // ---------------------------------------------------------------
   Use["Plugin"]          = 0;
   Use["Category"]        = 0;
   Use["SVM_Gauss"]       = 0;
   Use["SVM_Poly"]        = 0;
   Use["SVM_Lin"]         = 0;

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassificationApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   float nHjj,jjdr,jjdPhi,metdPhi,jcvsmva,jbprob1,jbprob2,jTCHP1,jTCHP2,pfmet,jcvs,pfmetsig,jjdistSVtx,V.mass,V.pt,hjjzmmdPhi,zeeMass,zeePt,hjjzeedPhi,wmnMt,wmnPt,hjjwmndPhi,wenMt,wenPt,hjjwendPhi,csv1,csv2,hjjzmmdPhi,hjjwdPhi,zmmEta,zeeEta,wmnPhi,wmnEta,wenEta,wenPhi,zeePhi,zmmPhi,deltaPullAngle,deltaPullAngleAK7,weight,cosRest,deltaEta,BDT,weight,weightTrig,btag1TSF,btag2TSF,btag1T2CSF,btag2CSF,btagA0CSF;;

int eventFlav;
EventInfo _EventInfo;

   int numJets;
   char  isIso;
//bool lng =1;

 
   reader->AddVariable( "H.mass"    ,  &H.mass);
   reader->AddVariable( "H.pt"      ,  &H.pt); 
   reader->AddVariable( "mincsv := min(hJet_csv[0],hJet_csv[1])" ,&hJet_csv0 );
   reader->AddVariable( "maxcsv := max(hJet_csv[0],hJet_csv[1])" ,&hJet_csv1 );
   reader->AddVariable( "V.pt"        ,  &V.pt);
   reader->AddVariable( "hjjzmmdPhi := abs(deltaPhi(H.phi,V.phi))"   ,  &hjjzmmdPhi);
   reader->AddVariable( "deltaEta := abs(hJet_eta[1]-hJet_eta[0])",&deltaEta);
//if(lng)   reader->AddVariable( "cosRest := abs(Jet1.cosTheta) + abs(Jet2.cosTheta)", &cosRest);
//   reader->AddVariable( "jjdr"       ,  &jjdr);
//   reader->AddVariable( "deltaPullAngle",&deltaPullAngle);
//   reader->AddVariable( "V.mass"      ,  &V.mass);
// if(lng)  reader->AddVariable( "hJet_pt[0]"       ,  &hJet_pt[0]);
// if(lng)  reader->AddVariable( "hJet_phi[0]"      ,  &hJet_phi[0]);
//   reader->AddVariable( "hJet_pt[1]"      ,   &hJet_pt[1]);
//if(lng)  reader->AddVariable( "hJet_phi[1]"      ,  &hJet_phi[1]);



   Float_t Category_cat1, Category_cat2, Category_cat3;
   if (Use["Category"]){
      // Add artificial spectators for distinguishing categories
      reader->AddSpectator( "Category_cat1 := var3<=0",             &Category_cat1 );
      reader->AddSpectator( "Category_cat2 := (var3>0)&&(var4<0)",  &Category_cat2 );
      reader->AddSpectator( "Category_cat3 := (var3>0)&&(var4>=0)", &Category_cat3 );
   }

   // --- Book the MVA methods

   TString dir    = "weights/";
   TString prefix = "TMVAClassification";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA( methodName, weightfile ); 
      }
   }
  
   // Book output histograms
   UInt_t nbin = 100;
   TH1F   *histLk(0), *histLkD(0), *histLkPCA(0), *histLkKDE(0), *histLkMIX(0), *histPD(0), *histPDD(0);
   TH1F   *histPDPCA(0), *histPDEFoam(0), *histPDEFoamErr(0), *histPDEFoamSig(0), *histKNN(0), *histHm(0);
   TH1F   *histFi(0), *histFiG(0), *histFiB(0), *histLD(0), *histNn(0),*histNnbfgs(0),*histNnbnn(0);
   TH1F   *histNnC(0), *histNnT(0), *histBdt(0), *histBdtG(0), *histBdtD(0), *histRf(0), *histSVMG(0);
   TH1F   *histSVMP(0), *histSVML(0), *histFDAMT(0), *histFDAGA(0), *histCat(0), *histPBdt(0);

   if (Use["Likelihood"])    histLk      = new TH1F( "MVA_Likelihood",    "MVA_Likelihood",    nbin, -1, 1 );
   if (Use["LikelihoodD"])   histLkD     = new TH1F( "MVA_LikelihoodD",   "MVA_LikelihoodD",   nbin, -1, 0.9999 );
   if (Use["LikelihoodPCA"]) histLkPCA   = new TH1F( "MVA_LikelihoodPCA", "MVA_LikelihoodPCA", nbin, -1, 1 );
   if (Use["LikelihoodKDE"]) histLkKDE   = new TH1F( "MVA_LikelihoodKDE", "MVA_LikelihoodKDE", nbin,  -0.00001, 0.99999 );
   if (Use["LikelihoodMIX"]) histLkMIX   = new TH1F( "MVA_LikelihoodMIX", "MVA_LikelihoodMIX", nbin,  0, 1 );
   if (Use["PDERS"])         histPD      = new TH1F( "MVA_PDERS",         "MVA_PDERS",         nbin,  0, 1 );
   if (Use["PDERSD"])        histPDD     = new TH1F( "MVA_PDERSD",        "MVA_PDERSD",        nbin,  0, 1 );
   if (Use["PDERSPCA"])      histPDPCA   = new TH1F( "MVA_PDERSPCA",      "MVA_PDERSPCA",      nbin,  0, 1 );
   if (Use["KNN"])           histKNN     = new TH1F( "MVA_KNN",           "MVA_KNN",           nbin,  0, 1 );
   if (Use["HMatrix"])       histHm      = new TH1F( "MVA_HMatrix",       "MVA_HMatrix",       nbin, -0.95, 1.55 );
   if (Use["Fisher"])        histFi      = new TH1F( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
   if (Use["FisherG"])       histFiG     = new TH1F( "MVA_FisherG",       "MVA_FisherG",       nbin, -1, 1 );
   if (Use["BoostedFisher"]) histFiB     = new TH1F( "MVA_BoostedFisher", "MVA_BoostedFisher", nbin, -2, 2 );
   if (Use["LD"])            histLD      = new TH1F( "MVA_LD",            "MVA_LD",            nbin, -2, 2 );
   if (Use["MLP"])           histNn      = new TH1F( "MVA_MLP",           "MVA_MLP",           nbin, -1.25, 1.5 );
   if (Use["MLPBFGS"])       histNnbfgs  = new TH1F( "MVA_MLPBFGS",       "MVA_MLPBFGS",       nbin, -1.25, 1.5 );
   if (Use["MLPBNN"])        histNnbnn   = new TH1F( "MVA_MLPBNN",        "MVA_MLPBNN",        nbin, -1.25, 1.5 );
   if (Use["CFMlpANN"])      histNnC     = new TH1F( "MVA_CFMlpANN",      "MVA_CFMlpANN",      nbin,  0, 1 );
   if (Use["TMlpANN"])       histNnT     = new TH1F( "MVA_TMlpANN",       "MVA_TMlpANN",       nbin, -1.3, 1.3 );
   if (Use["BDT"])           histBdt     = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -0.8, 0.8 );
   if (Use["BDTD"])          histBdtD    = new TH1F( "MVA_BDTD",          "MVA_BDTD",          nbin, -0.8, 0.8 );
   if (Use["BDTG"])          histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );
   if (Use["RuleFit"])       histRf      = new TH1F( "MVA_RuleFit",       "MVA_RuleFit",       nbin, -2.0, 2.0 );
   if (Use["SVM_Gauss"])     histSVMG    = new TH1F( "MVA_SVM_Gauss",     "MVA_SVM_Gauss",     nbin,  0.0, 1.0 );
   if (Use["SVM_Poly"])      histSVMP    = new TH1F( "MVA_SVM_Poly",      "MVA_SVM_Poly",      nbin,  0.0, 1.0 );
   if (Use["SVM_Lin"])       histSVML    = new TH1F( "MVA_SVM_Lin",       "MVA_SVM_Lin",       nbin,  0.0, 1.0 );
   if (Use["FDA_MT"])        histFDAMT   = new TH1F( "MVA_FDA_MT",        "MVA_FDA_MT",        nbin, -2.0, 3.0 );
   if (Use["FDA_GA"])        histFDAGA   = new TH1F( "MVA_FDA_GA",        "MVA_FDA_GA",        nbin, -2.0, 3.0 );
   if (Use["Category"])      histCat     = new TH1F( "MVA_Category",      "MVA_Category",      nbin, -2., 2. );
   if (Use["Plugin"])        histPBdt    = new TH1F( "MVA_PBDT",          "MVA_BDT",           nbin, -0.8, 0.8 );

   // PDEFoam also returns per-event error, fill in histogram, and also fill significance
   if (Use["PDEFoam"]) {
      histPDEFoam    = new TH1F( "MVA_PDEFoam",       "MVA_PDEFoam",              nbin,  0, 1 );
      histPDEFoamErr = new TH1F( "MVA_PDEFoamErr",    "MVA_PDEFoam error",        nbin,  0, 1 );
      histPDEFoamSig = new TH1F( "MVA_PDEFoamSig",    "MVA_PDEFoam significance", nbin,  0, 10 );
   }

   // Book example histogram for probability (the other methods are done similarly)
   TH1F *probHistFi(0), *rarityHistFi(0);
   if (Use["Fisher"]) {
      probHistFi   = new TH1F( "MVA_Fisher_Proba",  "MVA_Fisher_Proba",  nbin, 0, 1 );
      rarityHistFi = new TH1F( "MVA_Fisher_Rarity", "MVA_Fisher_Rarity", nbin, 0, 1 );
   }

   // Prepare input tree ithis must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TFile *input(0);



if(channel == 0)   TString fname = "./B-ZH_ZToLL_HToBB_M-115_7TeV-powheg-herwigpp.root";   
if(channel == 1)   TString fname = "./B-TTJets_TuneZ2_7TeV-madgraph-tauola.root";   
if(channel == 2)   TString fname = "./B-ZZ_TuneZ2_7TeV_pythia6_tauola.root";   

if(channel ==  3)   TString fname = "./B-T_TuneZ2_s-channel_7TeV-powheg-tauola.root";
if(channel ==  4)   TString fname = "./B-T_TuneZ2_t-channel_7TeV-powheg-tauola.root";
if(channel ==  5)   TString fname = "./B-T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola.root";
if(channel ==  6)   TString fname = "./B-Tbar_TuneZ2_s-channel_7TeV-powheg-tauola.root";
if(channel ==  7)   TString fname = "./B-Tbar_TuneZ2_t-channel_7TeV-powheg-tauola.root";
if(channel ==  8)   TString fname = "./B-Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola.root";

if(channel ==  9)   TString fname = "./B-WW_TuneZ2_7TeV_pythia6_tauola.root";
if(channel ==  10)  TString fname = "./B-WZ_TuneZ2_7TeV_pythia6_tauola.root";
if(channel ==  11)  TString fname = "./B-WJetsToLNu_TuneZ2_7TeV-madgraph-tauola.root";
if(channel ==  12)  TString fname = "./B-DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola.root";
if(channel ==  13)  TString fname = "./B-DYJetsToLL_PtZ-100_TuneZ2_7TeV-madgraph-tauola.root";

if(channel == 14) TString fname = "./B-SingleMu_Run2010_May10Rereco.root";
if(channel == 15) TString fname = "./B-SingleMu_Run2011A_Aug05ReReco.root";
if(channel == 16) TString fname = "./B-SingleMu_Run2011A_PromptRecoV4.root";
if(channel == 17) TString fname = "./B-SingleMu_Run2011A_PromptRecoV6.root";



   if (!gSystem->AccessPathName( fname )) 
      input = TFile::Open( fname ); // check if file in local directory exists
   else    
      input = TFile::Open( "http://root.cern.ch/files/tmva_class_example.root" ); // if not: download from ROOT server
   
   if (!input) {
      
      exit(1);
   }
   std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;
   
   // --- Event loop

   // Prepare the event tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
   std::cout << "--- Select signal sample" << std::endl;

   float flav1,flav2,PUweight;
   TTree* theTree = (TTree*)input->Get("tree");
   theTree->SetBranchAddress( "V"      ,&V       );
   theTree->SetBranchAddress( "H"          ,&H       );
   theTree->SetBranchAddress("btag1TSF"		,  &btag1TSF	  );
   theTree->SetBranchAddress("btag2TSF"		,  &btag2TSF	  );
   theTree->SetBranchAddress("btag1T2CSF"	,  &btag1T2CSF	  );
   theTree->SetBranchAddress("btag2CSF"	,  &btag2CSF	          );
   theTree->SetBranchAddress("btagA0CSF"	,  &btagA0CSF	  );

   theTree->SetBranchAddress("eventFlav"    ,  &eventFlav );                
   theTree->SetBranchAddress( "hJet_pt"	,  &hJet_pt	);
   theTree->SetBranchAddress("hJet_eta"	,  &hJet_eta	);
   theTree->SetBranchAddress("hJet_phi"	,  &hJet_phi	);
   theTree->SetBranchAddress("hJet_csv"	,  &hJet_csv	);
   theTree->SetBranchAddress( "PUweight"   ,  &PUweight);
   theTree->SetBranchAddress("EVENT",      &_EventInfo);
   theTree->SetBranchAddress( "weightTrig"   ,  &weightTrig);
   // Efficiency calculator for cut method
   Int_t    nSelCutsGA = 0;
   Double_t effS       = 0.7;

   std::vector<Float_t> vecVar(4); // vector for EvaluateMVA tests

   TFile *target  = new TFile( filename->Data(),"RECREATE" );
   TTree *tree = new TTree("tree", "myTree");
   tree->Branch("BDT"  	        ,  &BDT            ,  "BDT/F"       );            	
   tree->Branch("backNum"  	,  &backNum        ,  "backNum/I"         );             	
   tree->Branch("flav1"  	,  &flav1         ,   "flav1/F"        );             	
   tree->Branch("flav2"  	,  &flav2        ,   "flav2/F"        );             	
   tree->Branch("PUweight"  	,  &PUweight        ,   "PUweight/F"        );             	
   tree->Branch("weightTrig"  	,  &weightTrig        ,   "weightTrig/F"        );             	
   tree->Branch("hJet_pt"	,  &hJet_pt	    ,  "hJet_pt[2]/F");
   tree->Branch("hJet_eta"	,  &hJet_eta	    ,  "hJet_eta[2]/F");
   tree->Branch("hJet_phi"	,  &hJet_phi	    ,  "hJet_phi[2]/F");
   tree->Branch("hJet_csv"	,  &hJet_csv	    ,  "hJet_csv[2]/F");
   tree->Branch("H"		,  &H	            ,  "mass/F:pt/F:eta:phi/F:dR/F:dPhi/F:dEta/F");
   tree->Branch("V"		,  &V	            ,  "mass/F:pt/F:eta:phi/F");
   tree->Branch("hjjzmmdPhi"    ,  &hjjzmmdPhi   ,  "hjjzmmdPhi/F" );                
   tree->Branch("eventFlav"    ,  &eventFlav   ,  "eventFlav/I" );                
   tree->Branch("EVENT",      &_EventInfo	    ,  "run/I:lumi/I:event/I:json/I");
   tree->Branch("btag1TSF"		,  &btag1TSF	         ,   "btag1TSF/F");
   tree->Branch("btag2TSF"		,  &btag2TSF	         ,   "btag2TSF/F");
   tree->Branch("btag1T2CSF"	,  &btag1T2CSF	         ,   "btag1T2CSF/F");
   tree->Branch("btag2CSF"	,  &btag2CSF	         ,   "btag2CSF/F");
   tree->Branch("btagA0CSF"	,  &btagA0CSF	         ,   "btagA0CSF/F");





   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
      theTree->GetEntry(ievt);
      // --- Return the MVA outputs and fill into histograms
/*
      deltaEta = fabs(hJet_eta[1]-hJet_eta[0]);     
      hjjzmmdPhi = fabs(deltaPhi(H.phi,V.phi));
      std::cout << "hm " << H.mass << std::endl;
      std::cout << "deta " << deltaEta << std::endl;
      std::cout << "hpt " << H.pt <<std::endl; 
      std::cout << "csv " << hJet_csv[0] <<std::endl;
      std::cout << "hjcsv " << hJet_csv[1] <<std::endl;
      std::cout << "vpt " << V.pt   << std::endl;
      std::cout << "hzdphi " << hjjzmmdPhi << std::endl;
*/
      deltaEta = fabs(hJet_eta[1]-hJet_eta[0]);     
      hjjzmmdPhi = fabs(deltaPhi(H.phi,V.phi));
      hJet_csv0 = min(hJet_csv[0],hJet_csv[1]);
      hJet_csv1 = max(hJet_csv[0],hJet_csv[1]);



  
 //     cosRest = fabs(Jet1.cosTheta) + fabs(Jet2.cosTheta);
      if (Use["CutsGA"]) {
         // Cuts is a special case: give the desired signal efficienciy
         Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
         if (passed) nSelCutsGA++;
      }

///    std::cout <<                 cosRest << std::endl; 
///    std::cout <<                 H.mass << std::endl;
///    std::cout <<                 H.pt   << std::endl;
///    std::cout <<                 deltaPullAngle                   << std::endl;
///    std::cout <<                 jjdr                << std::endl;
///    std::cout <<                 hJet_csv[0]              << std::endl;      
///    std::cout <<                 hJet_csv[1]                << std::endl;    
///    std::cout <<                 V.mass                   << std::endl;
///    std::cout <<                 V.pt                     << std::endl;
///    std::cout <<                 zmmdPhi                  << std::endl; 
///    std::cout <<                 hJet_pt[0]                    << std::endl;
///    std::cout <<                 hJet_phi[0]                    << std::endl;
///    std::cout <<                 hJet_pt[1]                    << std::endl;
///    std::cout <<                 hJet_phi[1]                    << std::endl;
///    std::cout <<                 deltaEta                      << std::endl;                 







      if (Use["Likelihood"   ])   histLk     ->Fill( reader->EvaluateMVA( "Likelihood method"    ) ,weight);
      if (Use["LikelihoodD"  ])   histLkD    ->Fill( reader->EvaluateMVA( "LikelihoodD method"   ) ,weight);
      if (Use["LikelihoodPCA"])   histLkPCA  ->Fill( reader->EvaluateMVA( "LikelihoodPCA method" ) ,weight);
      if (Use["LikelihoodKDE"])   histLkKDE  ->Fill( reader->EvaluateMVA( "LikelihoodKDE method" ) ,weight);
      if (Use["LikelihoodMIX"])   histLkMIX  ->Fill( reader->EvaluateMVA( "LikelihoodMIX method" ) ,weight);
      if (Use["PDERS"        ])   histPD     ->Fill( reader->EvaluateMVA( "PDERS method"         ) ,weight);
      if (Use["PDERSD"       ])   histPDD    ->Fill( reader->EvaluateMVA( "PDERSD method"        ) ,weight);
      if (Use["PDERSPCA"     ])   histPDPCA  ->Fill( reader->EvaluateMVA( "PDERSPCA method"      ) ,weight);
      if (Use["KNN"          ])   histKNN    ->Fill( reader->EvaluateMVA( "KNN method"           ) ,weight);
      if (Use["HMatrix"      ])   histHm     ->Fill( reader->EvaluateMVA( "HMatrix method"       ) ,weight);
      if (Use["Fisher"       ])   histFi     ->Fill( reader->EvaluateMVA( "Fisher method"        ) ,weight);
      if (Use["FisherG"      ])   histFiG    ->Fill( reader->EvaluateMVA( "FisherG method"       ) ,weight);
      if (Use["BoostedFisher"])   histFiB    ->Fill( reader->EvaluateMVA( "BoostedFisher method" ) ,weight);
      if (Use["LD"           ])   histLD     ->Fill( reader->EvaluateMVA( "LD method"            ) ,weight);
      if (Use["MLP"          ])   histNn     ->Fill( reader->EvaluateMVA( "MLP method"           ) ,weight);
      if (Use["MLPBFGS"      ])   histNnbfgs ->Fill( reader->EvaluateMVA( "MLPBFGS method"       ) ,weight);
      if (Use["MLPBNN"       ])   histNnbnn  ->Fill( reader->EvaluateMVA( "MLPBNN method"        ) ,weight);
      if (Use["CFMlpANN"     ])   histNnC    ->Fill( reader->EvaluateMVA( "CFMlpANN method"      ) ,weight);
      if (Use["TMlpANN"      ])   histNnT    ->Fill( reader->EvaluateMVA( "TMlpANN method"       ) ,weight);
      if (Use["BDT"          ])   histBdt    ->Fill( reader->EvaluateMVA( "BDT method"           ) ,weight);
      if (Use["BDTD"         ])   histBdtD   ->Fill( reader->EvaluateMVA( "BDTD method"          ) ,weight);
      if (Use["BDTG"         ])   histBdtG   ->Fill( reader->EvaluateMVA( "BDTG method"          ) ,weight);
      if (Use["RuleFit"      ])   histRf     ->Fill( reader->EvaluateMVA( "RuleFit method"       ) ,weight);
      if (Use["SVM_Gauss"    ])   histSVMG   ->Fill( reader->EvaluateMVA( "SVM_Gauss method"     ) ,weight);
      if (Use["SVM_Poly"     ])   histSVMP   ->Fill( reader->EvaluateMVA( "SVM_Poly method"      ) ,weight);
      if (Use["SVM_Lin"      ])   histSVML   ->Fill( reader->EvaluateMVA( "SVM_Lin method"       ) ,weight);
      if (Use["FDA_MT"       ])   histFDAMT  ->Fill( reader->EvaluateMVA( "FDA_MT method"        ) ,weight);
      if (Use["FDA_GA"       ])   histFDAGA  ->Fill( reader->EvaluateMVA( "FDA_GA method"        ) ,weight);
      if (Use["Category"     ])   histCat    ->Fill( reader->EvaluateMVA( "Category method"      ) ,weight);
      if (Use["Plugin"       ])   histPBdt   ->Fill( reader->EvaluateMVA( "P_BDT method"         ) ,weight);
     
      if (Use["BDT"          ])  {BDT = reader->EvaluateMVA("BDT method"); tree->Fill();}
      if (Use["BDTG"          ])  {BDT = reader->EvaluateMVA("BDTG method"); tree->Fill();}
      if (Use["BDTD"         ])   {BDT =  reader->EvaluateMVA( "BDTD method"); tree->Fill();}
      if (Use["Fisher"       ])   {BDT =  reader->EvaluateMVA( "Fisher method");tree->Fill();}



      // Retrieve also per-event error
      if (Use["PDEFoam"]) {
         Double_t val = reader->EvaluateMVA( "PDEFoam method" );
         Double_t err = reader->GetMVAError();
         histPDEFoam   ->Fill( val );
         histPDEFoamErr->Fill( err );         
         if (err>1.e-50) histPDEFoamSig->Fill( val/err );
      }         

      // Retrieve probability instead of MVA output
      if (Use["Fisher"])   {
         probHistFi  ->Fill( reader->GetProba ( "Fisher method" ) );
         rarityHistFi->Fill( reader->GetRarity( "Fisher method" ) );
      }
   }

   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Get efficiency for cuts classifier
   if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
                                << " (for a required signal efficiency of " << effS << ")" << std::endl;

   if (Use["CutsGA"]) {

      // test: retrieve cuts for particular signal efficiency
      // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
      TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

      if (mcuts) {      
         std::vector<Double_t> cutsMin;
         std::vector<Double_t> cutsMax;
         mcuts->GetCuts( 0.7, cutsMin, cutsMax );
         std::cout << "--- -------------------------------------------------------------" << std::endl;
         std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
         for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
            std::cout << "... Cut: " 
                      << cutsMin[ivar] 
                      << " < \"" 
                      << mcuts->GetInputVar(ivar)
                      << "\" <= " 
                      << cutsMax[ivar] << std::endl;
         }
         std::cout << "--- -------------------------------------------------------------" << std::endl;
      }
   }

   // --- Write histograms


   if (Use["Likelihood"   ])   histLk     ->Write();
   if (Use["LikelihoodD"  ])   histLkD    ->Write();
   if (Use["LikelihoodPCA"])   histLkPCA  ->Write();
   if (Use["LikelihoodKDE"])   histLkKDE  ->Write();
   if (Use["LikelihoodMIX"])   histLkMIX  ->Write();
   if (Use["PDERS"        ])   histPD     ->Write();
   if (Use["PDERSD"       ])   histPDD    ->Write();
   if (Use["PDERSPCA"     ])   histPDPCA  ->Write();
   if (Use["KNN"          ])   histKNN    ->Write();
   if (Use["HMatrix"      ])   histHm     ->Write();
   if (Use["Fisher"       ]) {  histFi     ->Write();tree->Write();}
   if (Use["FisherG"      ])   histFiG    ->Write();
   if (Use["BoostedFisher"])   histFiB    ->Write();
   if (Use["LD"           ])   histLD     ->Write();
   if (Use["MLP"          ])   histNn     ->Write();
   if (Use["MLPBFGS"      ])   histNnbfgs ->Write();
   if (Use["MLPBNN"       ])   histNnbnn  ->Write();
   if (Use["CFMlpANN"     ])   histNnC    ->Write();
   if (Use["TMlpANN"      ])   histNnT    ->Write();
   if (Use["BDT"          ])   {histBdt    ->Write(); tree->Write();}
   if (Use["BDTD"         ])   {histBdtD   ->Write(); tree->Write();}
   if (Use["BDTG"         ])   {histBdtG   ->Write();tree->Write();} 
   if (Use["RuleFit"      ])   histRf     ->Write();
   if (Use["SVM_Gauss"    ])   histSVMG   ->Write();
   if (Use["SVM_Poly"     ])   histSVMP   ->Write();
   if (Use["SVM_Lin"      ])   histSVML   ->Write();
   if (Use["FDA_MT"       ])   histFDAMT  ->Write();
   if (Use["FDA_GA"       ])   histFDAGA  ->Write();
   if (Use["Category"     ])   histCat    ->Write();
   if (Use["Plugin"       ])   histPBdt   ->Write();

   // Write also error and significance histos
   if (Use["PDEFoam"]) { histPDEFoam->Write(); histPDEFoamErr->Write(); histPDEFoamSig->Write(); }

   // Write also probability hists
   if (Use["Fisher"]) { if (probHistFi != 0) probHistFi->Write(); if (rarityHistFi != 0) rarityHistFi->Write(); }
   target->Close();

   std::cout << "--- Created root file: " << filename->Data() << " containing the MVA output histograms" << std::endl;
  
   delete reader;
   std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;
}
} 
