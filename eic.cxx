# ifndef __CINT__/*{{{*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <string>
#include <sstream>
#include <TStopwatch.h>
#include <TDatime.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TF1.h>

#include "eic.h"
#include "initilize.h"
// #include "setrootfile.h"
#include "tssa_sigL_Para.h"
#include "tssa_sigT_Para.h"

using std::setw;
using std::setprecision;
using std::cout;
using std::cin;
using std::endl;
using namespace std;/*}}}*/

//---------------------------------------------------------{{{
double pim::fermiMomentum() {

    double fMom;
    bool kFermi = true;
    while ( kFermi ) {
        double fProton_Rand_Mom_Col      = fRandom->Uniform( 0, 300.0);
        double fProton_Rand_Mom_Col_Prob = fRandom->Uniform( fProb[299], fProb[0] );
        int    fProton_Mom_Int           = std::ceil( fProton_Rand_Mom_Col );
        double f3He_Value                = fProb[ fProton_Mom_Int - 1 ];

        if ( fProton_Rand_Mom_Col_Prob <= f3He_Value ) {
            fMom = fProton_Rand_Mom_Col;	
            kFermi = false;
        }
    }

    return fMom;
}/*}}}*/

//---------------------------------------------------------
// g++ -o pim pimFermi.C `root-config --cflags --glibs`
//---------------------------------------------------------
int main()
{
    //double cos_t, sin_t;
    pim myPim;
    myPim.Initilize();

    TDatime dsTime;
    cout << "Start Time:   " << dsTime.GetHour() << ":" << dsTime.GetMinute() << endl;

    TStopwatch tTime;
    tTime.Start(); 

    // kCalcFermi     = true;

    Int_t target_direction, kinematics_type;
    TString targetname="";  

    cout << "Electron Beam Momentum(GeV/c): "; cin >> fEBeam; cout<< endl;
    cout << "Proton Beam Momentum(GeV/c): "; cin >> fPBeam; cout<<  endl;
    cout << "Target Direction (1->Up, 2->Down): "; cin >> target_direction; cout << endl;
    cout << "Kinematics type (1->FF, 2->TSSA): ";  cin >> kinematics_type; cout << endl;
    cout << "Enter the number of events: ";        cin >> fNEvents;         cout << endl;
    cout << "Enter the file number: ";             cin >> fNFile;           cout << endl;

    if( target_direction == 1 ) targetname = "up";
    if( target_direction == 2 ) targetname = "down";

    fLumi = 1.000e34; 
    if(fEBeam<4.0 && fPBeam<30){//Chinese EIC
        fLumi = 1.000e33; 
        fScatElec_Theta_I = 60.0 * fDEG2RAD;
        fScatElec_Theta_F = 175.0 * fDEG2RAD;
        fScatElec_E_Lo    = 0.5;  // % of beam energy
        fScatElec_E_Hi    = 2.5;  // % of beam energy
        fPion_Theta_I     = 0.0 * fDEG2RAD;
        fPion_Theta_F     = 120.0 * fDEG2RAD;
    }
    fPSF = ( fEBeam * ( fScatElec_E_Hi - fScatElec_E_Lo ) * 
            ( sin( fScatElec_Theta_F ) - sin( fScatElec_Theta_I ) ) * 2 * fPI * 
            ( sin( fPion_Theta_F     ) - sin( fPion_Theta_I     ) ) * 2 * fPI );

    string sTFile;
    sTFile = Form("./LundFiles/eic_demp_E%2.1fGeV_P%dGeV_%i.txt",fEBeam, (int)fPBeam,(int)fNFile);
    string sRFile;
    sRFile = Form("./RootFiles/eic_demp_E%2.1fGeV_P%dGeV_%i.root",fEBeam, (int)fPBeam, (int)fNFile);
    string sLFile;
    sLFile= Form("./LundFiles/input_E%2.1fGeV_P%dGeV_%i.dat",fEBeam, (int)fPBeam,(int)fNFile);

    ofstream ppiOut ( sLFile.c_str() );
    ofstream ppiDetails ( sTFile.c_str() );

    //  myPim.setrootfile( sRFile );
    /*Set Rootfiles{{{*/
    TFile *f = new TFile( sRFile.c_str(), "recreate");
    TTree* t1 = new TTree("t1", "t1");

    t1->Branch("ebeam_theta", &fElectron_Theta_Col               ,"data/D");  /*{{{*/
    t1->Branch("ebeam_phi",   &fElectron_Phi_Col          ,"data/D");  
    t1->Branch("ebeam_ene",   &fElectron_Energy_Col_GeV           ,"data/D");  
    t1->Branch("ebeam_mom",   &fElectron_Mom_Col_GeV              ,"data/D");  
    t1->Branch("ebeam_px",    &fElectron_MomX_Col_GeV             ,"data/D");  
    t1->Branch("ebeam_py",    &fElectron_MomY_Col_GeV             ,"data/D");  
    t1->Branch("ebeam_pz",    &fElectron_MomZ_Col_GeV             ,"data/D");  /*}}}*/

    t1->Branch("pbeam_theta", &fProton_Theta_Col               ,"data/D");  /*{{{*/
    t1->Branch("pbeam_phi",   &fProton_Phi_Col          ,"data/D");  
    t1->Branch("pbeam_ene",   &fProton_Energy_Col_GeV           ,"data/D");  
    t1->Branch("pbeam_mom",   &fProton_Mom_Col_GeV              ,"data/D");  
    t1->Branch("pbeam_px",    &fProton_MomX_Col_GeV             ,"data/D");  
    t1->Branch("pbeam_py",    &fProton_MomY_Col_GeV             ,"data/D");  
    t1->Branch("pbeam_pz",    &fProton_MomZ_Col_GeV             ,"data/D");  /*}}}*/

    t1->Branch("neut_theta", &fNeutron_Theta_Col               ,"data/D");  /*{{{*/
    t1->Branch("neut_phi",   &fNeutron_Phi_Col          ,"data/D");  
    t1->Branch("neut_ene",   &fNeutron_Energy_Col_GeV           ,"data/D");  
    t1->Branch("neut_mom",   &fNeutron_Mom_Col_GeV              ,"data/D");  
    t1->Branch("neut_px",    &fNeutron_MomX_Col_GeV             ,"data/D");  
    t1->Branch("neut_py",    &fNeutron_MomY_Col_GeV             ,"data/D");  
    t1->Branch("neut_pz",    &fNeutron_MomZ_Col_GeV             ,"data/D");  /*}}}*/

    t1->Branch("pion_theta", &fPion_Theta_Col               ,"data/D");  /*{{{*/
    t1->Branch("pion_phi",   &fPion_Phi_Col          ,"data/D");  
    t1->Branch("pion_ene",   &fPion_Energy_Col_GeV           ,"data/D");  
    t1->Branch("pion_mom",   &fPion_Mom_Col_GeV              ,"data/D");  
    t1->Branch("pion_px",    &fPion_MomX_Col_GeV             ,"data/D");  
    t1->Branch("pion_py",    &fPion_MomY_Col_GeV             ,"data/D");  
    t1->Branch("pion_pz",    &fPion_MomZ_Col_GeV             ,"data/D");  /*}}}*/

    t1->Branch("ele_theta", &fScatElec_Theta_Col               ,"data/D");  /*{{{*/
    t1->Branch("ele_phi",   &fScatElec_Phi_Col          ,"data/D");  
    t1->Branch("ele_ene",   &fScatElec_Energy_Col_GeV           ,"data/D");  
    t1->Branch("ele_mom",   &fScatElec_Mom_Col_GeV              ,"data/D");  
    t1->Branch("ele_px",    &fScatElec_MomX_Col_GeV             ,"data/D");  
    t1->Branch("ele_py",    &fScatElec_MomY_Col_GeV             ,"data/D");  
    t1->Branch("ele_pz",    &fScatElec_MomZ_Col_GeV             ,"data/D");  /*}}}*/
    
    t1->Branch("vx",    &fVertex_X             ,"data/D"); 
    t1->Branch("vy",    &fVertex_Y             ,"data/D"); 
    t1->Branch("vz",    &fVertex_Z             ,"data/D"); 
    
    t1->Branch("x",    &fx             ,"data/D"); 
    t1->Branch("y",    &fy             ,"data/D"); 
    t1->Branch("z",    &fz             ,"data/D"); 
    t1->Branch("t",    &fT_GeV             ,"data/D"); 
    t1->Branch("t_para",    &fT_Para_GeV             ,"data/D"); 
    t1->Branch("Epsilon",    &fEpsilon             ,"data/D"); 
    t1->Branch("PSF",    &fPSF            ,"data/D"); 
    t1->Branch("Lumi",    &fLumi           ,"data/D"); 
    t1->Branch("Q2",    &fQsq_GeV             ,"data/D"); 
    t1->Branch("W2",    &fWsq_GeV             ,"data/D"); 
    t1->Branch("Wp",    &fW_Prime_GeV             ,"data/D"); 
    t1->Branch("weight",    &fEventWeight   ,"data/D"); 
    t1->Branch("Jacobian",    &fJacobian_CM_Col, "data/D");
    t1->Branch("FluxFactor",    &fFlux_Factor_Col, "data/D");
    t1->Branch("A_Factor",    &fA             ,"data/D"); 
    t1->Branch("Phi",    &fPhi           ,"data/D"); 
    t1->Branch("PhiS",    &fPhiS           ,"data/D"); 
    
    //t1->Branch("SSAsym",    &fSSAsym             ,"data/D"); 
    //t1->Branch("AsymPhi_S",    &fAsymPhi_S             ,"data/D"); 
    //t1->Branch("AsymPhiMinusPhi_S",    &fAsymPhiMinusPhi_S             ,"data/D"); 
    //t1->Branch("Asym2PhiMinusPhi_S",    &fAsym2PhiMinusPhi_S             ,"data/D"); 
    //t1->Branch("Asym3PhiMinusPhi_S",    &fAsym3PhiMinusPhi_S             ,"data/D"); 
    //t1->Branch("AsymPhiPlusPhi_S",    &fAsymPhiPlusPhi_S             ,"data/D"); 
    //t1->Branch("Asym2PhiPlusPhi_S",    &fAsym2PhiPlusPhi_S             ,"data/D"); 
   
    t1->Branch("Sigma",    &fSigma_Col            ,"data/D"); 
    t1->Branch("Sig_T",    &fSig_T            ,"data/D"); 
    t1->Branch("Sig_L",    &fSig_L            ,"data/D"); 
    //t1->Branch("Sig_Phi_S",    &fTerm_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_mPhiMinusPhi_S",    &fTerm_Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_2PhiMinusPhi_S",    &fTerm_2Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_3PhiMinusPhi_S",    &fTerm_3Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_PhiPlusPhi_S",    &fTerm_Phi_Plus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_2PhiPlusPhi_S",    &fTerm_2Phi_Plus_Phi_S_Col             ,"data/D"); 

    //t1->Branch("Sig_Phi_S",    &fSig_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_mPhiMinusPhi_S",    &fSig_Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_2PhiMinusPhi_S",    &fSig_2Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_3PhiMinusPhi_S",    &fSig_3Phi_Minus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_PhiPlusPhi_S",    &fSig_Phi_Plus_Phi_S_Col             ,"data/D"); 
    //t1->Branch("Sig_2PhiPlusPhi_S",    &fSig_2Phi_Plus_Phi_S_Col             ,"data/D"); 

    /*}}}*/

    int qsq_ev = 0, t_ev = 0, w_neg_ev = 0, w_ev = 0;
    double lpar0 = 0., lpar1 = 0., lpar2 = 0., lpar3 = 0., lpar4 = 0., lpar5 = 0., lpar6 = 0.;
    double tpar0 = 0., tpar1 = 0., tpar2 = 0., tpar3 = 0., tpar4 = 0.;

    long long int i;
    for ( i = 0; i < fNEvents; i++ ) {
        TDatime dFractTime;  
        fNGenerated ++;    

        //if ( i % ( fNEvents / 10 ) == 0 ) {
        if ( i % ( 1000000 ) == 0 ) {
            cout << "Event: " << setw(8) << i 
                << "     % of events " << setw(4) << ((1.0*i)/(1.0*fNEvents))*100.0
                << "   Day: " <<  dFractTime.GetDay() 
                << "   Time:   " << dFractTime.GetHour() 
                << ":" << dFractTime.GetMinute() 
                << ":" << dFractTime.GetSecond() 
                << endl;	  
        }

        // ----------------------------------------------------
        // Proton in lab (collider) frame
        // ----------------------------------------------------/*{{{*/
        fProton_Theta_Col = 50.0e-3;
        fProton_Phi_Col   = fPi; 
        fProton_Mom_Col   = fPBeam * 1e3; 
        fVertex_X         = 0.; 
        fVertex_Y         = 0.; 
        fVertex_Z         = 0.; 

        if  ( kCalcFermi ) {
            fProton_Mom_Col   = fProton_Mom_Col + myPim.fermiMomentum();
            fProton_Theta_Col = acos( fRandom->Uniform( cos(0.0) , cos(fPi) ) );
            fProton_Phi_Col   = fRandom->Uniform( 0 , 360 );
        }

        TLorentzVector lproton( fProton_Mom_Col * sin(fProton_Theta_Col) * cos(fProton_Phi_Col),
                fProton_Mom_Col * sin(fProton_Theta_Col) * sin(fProton_Phi_Col),
                fProton_Mom_Col * cos(fProton_Theta_Col),
                sqrt( pow( fProton_Mom_Col , 2 ) + pow( fProton_Mass , 2 ) ) ); 

        fProton_Mom_Col  = (lproton.Vect()).Mag(); fProton_Mom_Col_GeV = fProton_Mom_Col/1000.0;
        fProton_MomX_Col = lproton.X(); fProton_MomX_Col_GeV = fProton_MomX_Col/1000.0;
        fProton_MomY_Col = lproton.Y(); fProton_MomY_Col_GeV = fProton_MomY_Col/1000.0;
        fProton_MomZ_Col = lproton.Z(); fProton_MomZ_Col_GeV = fProton_MomZ_Col/1000.0;
        fProton_Energy_Col = lproton.E(); fProton_Energy_Col_GeV = fProton_Energy_Col/1000.0;

        TLorentzVector  lprotong;
        lprotong = lproton * fm;/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        // Boost vector from lab to protons rest frame
        // ----------------------------------------------------{{{
        TVector3 beta_col_rf;
        beta_col_rf = lproton.BoostVector();        
        fGamma_Col_RF = 1.0/sqrt( 1 - pow( beta_col_rf.Mag() , 2 ) );/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        // Electron in lab (collider) frame
        // ----------------------------------------------------/*{{{*/
        fElectron_Energy_Col = fElectron_Kin_Col; 
        fElectron_Mom_Col    = sqrt( pow(fElectron_Energy_Col , 2) - pow(fElectron_Mass , 2) );
        fElectron_Theta_Col  = fPi;
        fElectron_Phi_Col    = 0.0;
        fElectron_MomZ_Col   = fElectron_Mom_Col * cos(fElectron_Theta_Col);  
        fElectron_MomX_Col   = fElectron_Mom_Col * sin(fElectron_Theta_Col) * cos(fElectron_Phi_Col);
        fElectron_MomY_Col   = fElectron_Mom_Col * sin(fElectron_Theta_Col) * sin(fElectron_Phi_Col);  

        fElectron_Mom_Col_GeV = fElectron_Mom_Col/1000.0;
        fElectron_MomX_Col_GeV = fElectron_MomX_Col/1000.0;
        fElectron_MomY_Col_GeV = fElectron_MomY_Col/1000.0;
        fElectron_MomZ_Col_GeV = fElectron_MomZ_Col/1000.0;
        fElectron_Energy_Col_GeV = fElectron_Energy_Col/1000.0;

        TLorentzVector  lelectron( fElectron_MomX_Col, fElectron_MomY_Col, fElectron_MomZ_Col, fElectron_Energy_Col);
        TLorentzVector  lelectrong;
        lelectrong = lelectron * fm;/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ---------------------------------------------------------------------
        // Specify the energy and solid angle of scatterd electron in lab frame
        // ---------------------------------------------------------------------/*{{{*/
        fScatElec_Theta_Col  = acos( fRandom->Uniform( cos( fScatElec_Theta_I ) , cos( fScatElec_Theta_F ) ) );
        fScatElec_Phi_Col    = fRandom->Uniform( 0 , 2.0 * fPi);
        fScatElec_Energy_Col = fRandom->Uniform( fScatElec_E_Lo * fElectron_Energy_Col , fScatElec_E_Hi * fElectron_Energy_Col );
        fPion_Theta_Col      = acos( fRandom->Uniform( cos(fPion_Theta_I ) , cos(fPion_Theta_F ) ) ); 
        fPion_Phi_Col        = fRandom->Uniform( 0 , 2.0 * fPi );

        // fScatElec_Theta_Col  = 146.356*fDEG2RAD;
        // fScatElec_Phi_Col    = 11.8325*fDEG2RAD;
        // fScatElec_Energy_Col = 5.25281*1000.0;   
        // fPion_Theta_Col      = 14.5869*fDEG2RAD;
        // fPion_Phi_Col        = -168.57*fDEG2RAD;

        fScatElec_Mom_Col  = sqrt( pow( fScatElec_Energy_Col,2) - pow( fElectron_Mass , 2) );
        fScatElec_MomZ_Col = ( fScatElec_Mom_Col * cos(fScatElec_Theta_Col) );  
        fScatElec_MomX_Col = ( fScatElec_Mom_Col * sin(fScatElec_Theta_Col) * cos(fScatElec_Phi_Col) );
        fScatElec_MomY_Col = ( fScatElec_Mom_Col * sin(fScatElec_Theta_Col) * sin(fScatElec_Phi_Col) );

        fScatElec_Mom_Col_GeV = fScatElec_Mom_Col/1000.0;
        fScatElec_MomX_Col_GeV = fScatElec_MomX_Col/1000.0;
        fScatElec_MomY_Col_GeV = fScatElec_MomY_Col/1000.0;
        fScatElec_MomZ_Col_GeV = fScatElec_MomZ_Col/1000.0;
        fScatElec_Energy_Col_GeV = fScatElec_Energy_Col/1000.0;


        TLorentzVector lscatelec( fScatElec_MomX_Col, fScatElec_MomY_Col, fScatElec_MomZ_Col, fScatElec_Energy_Col);
        TLorentzVector lscatelecg;
        lscatelecg = lscatelec * fm;/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        // Photon in lab frame and Qsq
        // ----------------------------------------------------{{{
        TLorentzVector lphoton;
        lphoton = lelectron - lscatelec;
        TLorentzVector lphotong;
        lphotong = lelectrong - lscatelecg;

        fQsq_GeV = -1.*lphotong.Mag2();

        if ( fQsq_GeV < 5.0 ) {
            qsq_ev++;
            continue;
        }/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        // W square, Invariant Mass (P_g + P_p)^2
        // ----------------------------------------------------{{{
        TLorentzVector lwg;
        lwg = lprotong + lphotong;
        fW_GeV    = lwg.Mag();
        fWsq_GeV  = lwg.Mag2();

        if ( fWsq_GeV < 0 ) { 
            w_neg_ev++;
            continue;
        }    /*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        // Pion in Col frame
        // ----------------------------------------------------    {{{
        // fPion_Theta_Col            = acos( fRandom->Uniform( cos(fPion_Theta_I ) , cos(fPion_Theta_F ) ) ); 
        // fPion_Phi_Col              = fRandom->Uniform( 0 , 360 );}}}

        // ---------------------------------------------------------
        // Pion momentum in collider frame, analytic solution starts
        // ---------------------------------------------------------{{{
        double fupx = sin( fPion_Theta_Col ) * cos( fPion_Phi_Col );
        double fupy = sin( fPion_Theta_Col ) * sin( fPion_Phi_Col );
        double fupz = cos( fPion_Theta_Col );

        double fuqx = sin( lphoton.Theta() ) * cos( lphoton.Phi() );
        double fuqy = sin( lphoton.Theta() ) * sin( lphoton.Phi() );
        double fuqz = cos( lphoton.Theta() );

        double fa = -(lphoton.Vect()).Mag() * ( fupx * fuqx +  fupy * fuqy +  fupz * fuqz );
        double fb = pow ( (lphoton.Vect()).Mag() , 2 );
        double fc = lphoton.E() + fProton_Mass;

        fa = ( fa - std::abs( (lproton.Vect()).Mag() ) * ( ( ( lproton.X() / (lproton.Vect()).Mag() ) * fupx ) + 
                    ( ( lproton.Y() / (lproton.Vect()).Mag() ) * fupy ) + 
                    ( ( lproton.Z() / (lproton.Vect()).Mag() ) * fupz ) ) );

        double factor = ( pow( (lproton.Vect()).Mag() , 2 ) + 2.0 * (lphoton.Vect()).Mag() * (lproton.Vect()).Mag() *  
                ( ( ( lproton.X() / (lproton.Vect()).Mag() ) * fuqx ) + 
                  ( ( lproton.Y() / (lproton.Vect()).Mag() ) * fuqy ) + 
                  ( ( lproton.Z() / (lproton.Vect()).Mag() ) * fuqz ) ) );

        fb =  fb + factor;  
        fc = lphoton.E() + lproton.E();

        double ft = fc * fc - fb + fPion_Mass * fPion_Mass - fProton_Mass * fProton_Mass;

        double fQA = 4.0 * ( fa * fa - fc * fc );
        double fQB = 4.0 * fc * ft;
        double fQC = -4.0 * fa * fa * fPion_Mass * fPion_Mass - ft * ft;    

        fradical = fQB * fQB - 4.0 * fQA * fQC;

        fepi1 = ( -fQB - sqrt( fradical ) ) / ( 2.0 * fQA );
        fepi2 = ( -fQB + sqrt( fradical ) ) / ( 2.0 * fQA );

        fPion_Mom_Same = 0;
        if (  std::abs(fepi1 - fepi2) < fDiff ){ fPion_Mom_Same = 1; }/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ---------------------------------------------------------
        // Pion momentum in collider frame, analytic solution ends
        // ---------------------------------------------------------{{{
        TLorentzVector lpion( ( sqrt( pow( fepi1 , 2) - pow(fPion_Mass , 2) ) ) * sin(fPion_Theta_Col) * cos(fPion_Phi_Col),
                ( sqrt( pow( fepi1 , 2) - pow(fPion_Mass , 2) ) ) * sin(fPion_Theta_Col) * sin(fPion_Phi_Col),
                ( sqrt( pow( fepi1 , 2) - pow(fPion_Mass , 2) ) ) * cos(fPion_Theta_Col),
                fepi1 );

        fPion_Mom_Col     = (lpion.Vect()).Mag(); fPion_Mom_Col_GeV = fPion_Mom_Col/1000.0;
        fPion_MomX_Col = lpion.X(); fPion_MomX_Col_GeV = fPion_MomX_Col/1000.0;
        fPion_MomY_Col = lpion.Y(); fPion_MomY_Col_GeV = fPion_MomY_Col/1000.0;
        fPion_MomZ_Col = lpion.Z(); fPion_MomZ_Col_GeV = fPion_MomZ_Col/1000.0;
        fPion_Energy_Col = lpion.E(); fPion_Energy_Col_GeV = fPion_Energy_Col/1000.0;

        TLorentzVector lpiong;
        lpiong = lpion * fm;

        TLorentzVector lneutron( ( lproton + lelectron - lscatelec - lpion ).X(),
                ( lproton + lelectron - lscatelec - lpion ).Y(),
                ( lproton + lelectron - lscatelec - lpion ).Z(),
                sqrt( pow( ( ( ( lproton + lelectron - lscatelec - lpion ).Vect() ).Mag()),2) +
                    pow( fNeutron_Mass ,2 ) ) );

        fNeutron_Mom_Col  = (lneutron.Vect()).Mag(); fNeutron_Mom_Col_GeV = fNeutron_Mom_Col/1000.0;
        fNeutron_MomX_Col = lneutron.X(); fNeutron_MomX_Col_GeV = fNeutron_MomX_Col/1000.0;
        fNeutron_MomY_Col = lneutron.Y(); fNeutron_MomY_Col_GeV = fNeutron_MomY_Col/1000.0;
        fNeutron_MomZ_Col = lneutron.Z(); fNeutron_MomZ_Col_GeV = fNeutron_MomZ_Col/1000.0;
        fNeutron_Energy_Col = lneutron.E(); fNeutron_Energy_Col_GeV = fNeutron_Energy_Col/1000.0;
        fNeutron_Theta_Col = lneutron.Theta(); 
        fNeutron_Phi_Col = lneutron.Phi(); 

        TLorentzVector lneutrong;
        lneutrong = lneutron * fm;

        //fNeutron_Mom_Col_GeV     = (lneutrong.Vect()).Mag();
        //fNeutron_MomZ_Col_GeV    = lneutrong.Z();
        //fNeutron_MomX_Col_GeV    = lneutrong.X();
        //fNeutron_MomY_Col_GeV    = lneutrong.Y();
        //fNeutron_Energy_Col_GeV  = lneutrong.E();
        /*}}}*/
        // -------------------------------------------------------------------------------------------

        // ------------------------------------------------------------------------------------------------
        // Calculate w = (proton + photon)^2
        // ------------------------------------------------------------------------------------------------{{{
        // cout << fW_GeV << endl;
        if ( fW_GeV < 3.0 || fW_GeV > 10.6 ) {
            w_ev++;
            continue;
        }

        TLorentzVector lw;
        lw = lproton + lphoton;
        fW = lw.Mag();/*}}}*/
        // -------------------------------------------------------------------------------------------

        // -----------------------------------------------------------------------------------------------
        // Calculate w prime w' = (proton + photon - pion)^2
        // -----------------------------------------------------------------------------------------------{{{

        TLorentzVector lwp = lprotong + lphotong - lpiong;
        fW_Prime_GeV = lwp.Mag();    

        TLorentzVector fsini;
        fsini = lelectron + lproton;
        TLorentzVector fsfin;
        fsfin = lscatelec + lpion + lneutron;

        TLorentzVector fsinig;
        fsinig = fsini * fm;
        TLorentzVector fsfing;
        fsfing = fsfin * fm; 

        fMandSConserve = std::abs( fsinig.Mag() - fsfing.Mag() );

        kSConserve = false;
        if( std::abs( fsinig.Mag() - fsfing.Mag() ) < fDiff ) {
            kSConserve = true;
        }

        if ( myPim.CheckLaws( lelectron, lproton, lscatelec, lpion, lneutron ) != 1 )
            continue;/*}}}*/
        // -------------------------------------------------------------------------------------------

        /////////////////////////////////////////////////////////////////////////////////////////////////
        //                        Start                                                                //
        //   ransformation of e', pi- and recoil proton to target's rest frmae without energy loss     //
        /////////////////////////////////////////////////////////////////////////////////////////////////*{{{*/
        TLorentzVector lproton_rf;
        lproton_rf = lproton;
        lproton_rf.Boost(-beta_col_rf);
        TLorentzVector lproton_rfg;
        lproton_rfg = lproton_rf * fm;

        TLorentzVector lelectron_rf;
        lelectron_rf = lelectron;
        lelectron_rf.Boost(-beta_col_rf);
        TLorentzVector lelectron_rfg;
        lelectron_rfg = lelectron_rf * fm;

        TLorentzVector lscatelec_rf;
        lscatelec_rf = lscatelec;
        lscatelec_rf.Boost(-beta_col_rf);
        TLorentzVector lscatelec_rfg;
        lscatelec_rfg = lscatelec_rf * fm;

        TLorentzVector lphoton_rf;
        lphoton_rf = lphoton;
        lphoton_rf.Boost(-beta_col_rf);
        TLorentzVector lphoton_rfg;
        lphoton_rfg = lphoton_rf * fm;

        TLorentzVector lpion_rf;
        lpion_rf = lpion;
        lpion_rf.Boost(-beta_col_rf);
        TLorentzVector lpion_rfg;
        lpion_rfg = lpion_rf * fm;

        TLorentzVector lneutron_rf;
        lneutron_rf = lneutron;
        lneutron_rf.Boost(-beta_col_rf);
        TLorentzVector lneutron_rfg;
        lneutron_rfg = lneutron_rf * fm;/*}}}*/
        // -------------------------------------------------------------------------------------------

        /////////////////////////////////////////////////////////////////////////////////////////////////
        //         End                                                                                 //
        //   Transformation of e', pi- and recoil proton to target's rest frmae without energy loss    //
        ////////////////////////////////////////////////////////////////////////////////////////////////{{{
        // fElectron_Energy_RF_GeV              = lelectron_rf.E() / 1000.0;
        // fElectron_Mom_RF_GeV                 = (lelectron_rf.Vect()).Mag() / 1000.0;
        // fElectron_MomX_RF_GeV                = lelectron_rf.X() / 1000.0;
        // fElectron_MomY_RF_GeV                = lelectron_rf.Y() / 1000.0;
        // fElectron_MomZ_RF_GeV                = lelectron_rf.Z() / 1000.0;

        fScatElec_Energy_RF_GeV              = lscatelec_rf.E() / 1000.0;
        fScatElec_Mom_RF_GeV                 = (lscatelec_rf.Vect()).Mag() / 1000.0;
        fScatElec_MomX_RF_GeV                = lscatelec_rf.X() / 1000.0;
        fScatElec_MomY_RF_GeV                = lscatelec_rf.Y() / 1000.0;
        fScatElec_MomZ_RF_GeV                = lscatelec_rf.Z() / 1000.0;

        fPion_Energy_RF_GeV                  = lpion_rf.E() / 1000.0;
        fPion_Mom_RF_GeV                     = (lpion_rf.Vect()).Mag() / 1000.0;
        fPion_MomX_RF_GeV                    = lpion_rf.X() / 1000.0;
        fPion_MomY_RF_GeV                    = lpion_rf.Y() / 1000.0;
        fPion_MomZ_RF_GeV                    = lpion_rf.Z() / 1000.0;

        fNeutron_Energy_RF_GeV               = lneutron_rf.E() / 1000.0;
        fNeutron_Mom_RF_GeV                  = (lneutron_rf.Vect()).Mag() / 1000.0;
        fNeutron_MomX_RF_GeV                 = lneutron_rf.X() / 1000.0;
        fNeutron_MomY_RF_GeV                 = lneutron_rf.Y() / 1000.0;
        fNeutron_MomZ_RF_GeV                 = lneutron_rf.Z() / 1000.0;

        // if ( myPim.CheckLaws( lelectron_rf, lproton_rf, lscatelec_rf, lpion_rf, lneutron_rf ) != 1 )
        //   continue;}}}
        // -------------------------------------------------------------------------------------------

        // -----------------------------------------------------------------------------------------------
        // Calculate -t
        // -------------------------------------------------------------------------------------------------{{{
        fBeta_CM_RF           = (lphoton_rf.Vect()).Mag() / ( lphoton_rf.E() + fProton_Mass );
        fGamma_CM_RF          = ( lphoton_rf.E() + fProton_Mass ) / fW;
        fPion_Energy_CM       = ( pow( fW , 2) + pow(fPion_Mass , 2) - pow(fNeutron_Mass , 2) ) / ( 2.0 * fW);    
        fPion_Mom_CM          = sqrt( pow(fPion_Energy_CM , 2) - pow(fPion_Mass , 2));    
        fPion_Energy_CM_GeV   = fPion_Energy_CM / 1000.0;
        fPion_Mom_CM_GeV      = fPion_Mom_CM / 1000.0;

        // this equation is valid for parallel kinematics only!
        fT_Para = ( pow(((lphoton.Vect()).Mag() - (lpion.Vect()).Mag()),2) - pow((lphoton.E() - lpion.E()),2));
        fT_Para_GeV = fT_Para/1000000.0;

        TLorentzVector lt;
        lt = lphoton - lpion;
        TLorentzVector ltg;
        ltg = lt * fm;

        fT = -1.*lt.Mag2();
        fT_GeV = -1.*ltg.Mag2();

        if ( kinematics_type == 1 && fT_GeV > 0.5 ) {
            t_ev++;
            continue;
        }

        if ( kinematics_type == 2 && fT_GeV > 1.3 ) {
            t_ev++;
            continue;
        }

        fx = fQsq_GeV / ( 2.0 * lprotong.Dot( lphotong ) );
        fy = lprotong.Dot( lphotong ) / lprotong.Dot( lelectrong );
        fz = lpion.E()/lphoton.E();      /*}}}*/
        // -------------------------------------------------------------------------------------------

        // -------------------------------------------------------------------------------------------------------
        // Calculation of Phi  ( azimuthal angle of pion momentum w.r.t lepton plane in target's rest frame)
        // Calculation of PhiS ( azimuthal angle of target polarization w.r.t lepton plane in target's rest frame)
        // -------------------------------------------------------------------------------------------------------    {{{

        TVector3 v3Photon;     v3Photon.SetX( lphoton_rfg.X() );     v3Photon.SetY( lphoton_rfg.Y() );     v3Photon.SetZ( lphoton_rfg.Z() );    
        TVector3 v3Electron; v3Electron.SetX( lelectron_rfg.X() ); v3Electron.SetY( lelectron_rfg.Y() ); v3Electron.SetZ( lelectron_rfg.Z() );
        TVector3 v3Pion;         v3Pion.SetX( lpion_rfg.X() ) ;        v3Pion.SetY( lpion_rfg.Y() ) ;        v3Pion.SetZ( lpion_rfg.Z() );
        TVector3 v3S;               v3S.SetX( -1 );                       v3S.SetY( 0 );                        v3S.SetZ( 0 );        
        TVector3 v3PhotonUnit = v3Photon.Unit();    
        TVector3 v3QxL        = v3Photon.Cross(v3Electron);
        TVector3 v3QxP        = v3Photon.Cross(v3Pion);
        TVector3 v3QxS        = v3Photon.Cross(v3S);
        TVector3 v3LxP        = v3Electron.Cross(v3Pion);
        TVector3 v3LxS        = v3Electron.Cross(v3S);
        TVector3 v3PxL        = v3Pion.Cross(v3Electron);
        TVector3 v3QUnitxL    = v3PhotonUnit.Cross(v3Electron);
        TVector3 v3QUnitxP    = v3PhotonUnit.Cross(v3Pion);
        TVector3 v3QUnitxS    = v3PhotonUnit.Cross(v3S);

        fCos_Phi_Pion_LeptonPlane_RF = ( ( v3QUnitxL.Dot( v3QUnitxP ) ) / ( v3QUnitxL.Mag() * v3QUnitxP.Mag() ) ); // hep-ph/0410050v2
        fSin_Phi_Pion_LeptonPlane_RF = ( ( v3LxP.Dot( v3PhotonUnit  ) ) / ( v3QUnitxL.Mag() * v3QUnitxP.Mag() ) ); // hep-ph/0410050v2    
        if ( fSin_Phi_Pion_LeptonPlane_RF >= 0 )
            fPhi_Pion_LeptonPlane_RF    = fRAD2DEG * acos( ( v3QUnitxL.Dot( v3QUnitxP ) ) / ( v3QUnitxL.Mag() * v3QUnitxP.Mag() ) );
        if ( fSin_Phi_Pion_LeptonPlane_RF < 0 )
            fPhi_Pion_LeptonPlane_RF    = 360.0 - std::abs( fRAD2DEG * acos( ( v3QUnitxL.Dot( v3QUnitxP ) ) / ( v3QUnitxL.Mag() * v3QUnitxP.Mag() ) ) );

        fCos_Phi_TargPol_LeptonPlane_RF = ( ( v3QUnitxL.Dot( v3QUnitxS ) ) / ( v3QUnitxL.Mag() * v3QUnitxS.Mag() ) ); // hep-ph/0410050v2
        fSin_Phi_TargPol_LeptonPlane_RF = ( ( v3LxS.Dot( v3PhotonUnit  ) ) / ( v3QUnitxL.Mag() * v3QUnitxS.Mag() ) ); // hep-ph/0410050v2
        if ( fSin_Phi_TargPol_LeptonPlane_RF >= 0 )
            fPhi_TargPol_LeptonPlane_RF = fRAD2DEG * acos( ( v3QUnitxL.Dot( v3QUnitxS ) ) / ( v3QUnitxL.Mag() * v3QUnitxS.Mag() ) );
        if ( fSin_Phi_TargPol_LeptonPlane_RF < 0 )
            fPhi_TargPol_LeptonPlane_RF = 360.0 - std::abs( fRAD2DEG * acos( ( v3QUnitxL.Dot( v3QUnitxS ) ) / ( v3QUnitxL.Mag() * v3QUnitxS.Mag() ) ) );

        fTheta_Pion_Photon_RF       = fRAD2DEG * acos( ( v3Photon.Dot( v3Pion     ) ) / ( v3Photon.Mag()  * v3Pion.Mag()    ) );
        if ( fTheta_Pion_Photon_RF < 0 ) { fTheta_Pion_Photon_RF = 180.0 + fTheta_Pion_Photon_RF; }

        fPhi   = fPhi_Pion_LeptonPlane_RF;
        fPhiS  = fPhi_TargPol_LeptonPlane_RF;
/*}}}*/
        // -------------------------------------------------------------------------------------------

        // -----------------------------------------------------------------------------------
        // If we have fermi momentum then epsilon should be in rest frame 
        // The theta angle of scattered angle used in expression of epsilon is the angle 
        // with respect to direction of incoming electron in the rest frame of target nucleon
        // epsilon=1./(1.+ 2.*(pgam_restg**2)/q2g * *(tand(thscat_rest/2.))**2)
        // -----------------------------------------------------------------------------------{{{
        double fTheta_EEp = (lelectron_rf.Vect()).Angle(lscatelec_rf.Vect());

        fEpsilon = 1.0 / ( 1.0 + 2.0 * ( pow( (lphoton_rfg.Vect()).Mag(),2)/fQsq_GeV ) * pow( tan( fTheta_EEp / 2 ) , 2 ) );

        // ----------------------------------------------------
        // Virtual Photon flux factor in units of 1/(GeV*Sr)
        // ----------------------------------------------------
        fFlux_Factor_Col = (fAlpha/(2.0*pow(fPi,2))) * (lscatelecg.E() / lelectrong.E()) * 
            ( pow(fW_GeV,2) - pow(fProton_Mass_GeV,2) ) / (2.0*fProton_Mass_GeV*fQsq_GeV*(1.0 - fEpsilon));

        fFlux_Factor_RF = ( fAlpha / ( 2.0 * pow( fPi , 2 ) ) ) * ( lscatelec_rfg.E() / lelectron_rfg.E() ) *
            ( pow( fW_GeV , 2 ) - pow( fProton_Mass_GeV , 2 ) ) /
            ( 2.0 * fProton_Mass_GeV * fQsq_GeV * ( 1.0 - fEpsilon ) );
/*}}}*/
        // -------------------------------------------------------------------------------------------

        // ----------------------------------------------------
        //  Jacobian  dt/dcos(theta*)dphi in units of GeV2/sr
        // ----------------------------------------------------/*{{{*/
        fJacobian_CM = ( (lphoton_rfg.Vect()).Mag() - fBeta_CM_RF * lphoton_rfg.E() ) / ( fGamma_CM_RF * ( 1.0 - pow(fBeta_CM_RF,2) ) );

        fA = fJacobian_CM * fPion_Mom_CM_GeV / fPi;

        // ----------------------------------------------------
        // Jacobian dOmega* / dOmega dimensionless
        // ----------------------------------------------------
        fJacobian_CM_RF  = ( pow((lpion_rf.Vect()).Mag(),2)*fW) / 
            ( fPion_Mom_CM * std::abs( ( fProton_Mass + lphoton_rf.E()) * (lpion_rf.Vect()).Mag() - 
                                       ( lpion_rf.E() * (lphoton_rf.Vect()).Mag() * cos( lpion_rf.Theta() ) ) ) );

        fJacobian_CM_Col = ( ( pow((lpion.Vect()).Mag(),2) * fW ) /
                ( fPion_Mom_CM * std::abs( ( fProton_Mass + lphoton.E() ) * (lpion.Vect()).Mag() -
                                           ( lpion.E() * (lphoton.Vect()).Mag() * cos( lpion.Theta() ) ) ) ) );/*}}}*/
        // -------------------------------------------------------------------------------------------

        // -----------------------------------------------------------------------------------------------------
        // CKY sigma L and T starts
        // ------------------------------------------------------------------------------------------------------/*{{{*/
        lpar0 = 0.;    lpar1 = 0.;    lpar2 = 0.;    lpar3 = 0.;    lpar4 = 0.;    lpar5 = 0.;    lpar6 = 0.;
        tpar0 = 0.;    tpar1 = 0.;    tpar2 = 0.;    tpar3 = 0.;    tpar4 = 0.;

        fSig_L = 0;
        fSig_T = 0;

        if ( ( fT_GeV > 0. ) && ( fT_GeV < 0.15 ) ) {
            eicSigmaL( fW_GeV,  fQsq_GeV, lpar0, lpar1, lpar2 , lpar3 , lpar4 , lpar5 , lpar6 );
            TF1 *fitCKYLonglandau = new TF1("sigmaL","landau", 0.0 , 0.15 );
            fitCKYLonglandau->FixParameter( 0 , lpar0 );
            fitCKYLonglandau->FixParameter( 1 , lpar1 );
            fitCKYLonglandau->FixParameter( 2 , lpar2 );
            fSig_L = fitCKYLonglandau->Eval(fT_GeV);
            if ( lpar0 == 0 || lpar1 == 0 || lpar2 == 0 )
                fSig_L = 0;
            fitCKYLonglandau = NULL;
            delete fitCKYLonglandau;
        }
        else if ( ( fT_GeV > 0.15 ) && ( fT_GeV < 0.5 ) ) {
            eicSigmaL( fW_GeV,  fQsq_GeV, lpar0, lpar1, lpar2 , lpar3 , lpar4 , lpar5 , lpar6 );
            TF1 *fitCKYLongexpo1 = new TF1("sigmaL","expo", 0.15 , 0.5 );
            fitCKYLongexpo1->FixParameter( 0 , lpar3 );
            fitCKYLongexpo1->FixParameter( 1 , lpar4 );
            fSig_L = fitCKYLongexpo1->Eval(fT_GeV);
            if ( lpar3 == 0 || lpar4 == 0 )
                fSig_L = 0;
            fitCKYLongexpo1 = NULL;
            delete fitCKYLongexpo1;
        }
        else if ( ( fT_GeV > 0.5 ) && ( fT_GeV < 1.3 ) ) {
            eicSigmaL( fW_GeV,  fQsq_GeV, lpar0, lpar1, lpar2 , lpar3 , lpar4 , lpar5 , lpar6 );
            TF1 *fitCKYLongexpo2 = new TF1("sigmaL","expo", 0.5 , 1.3 );
            fitCKYLongexpo2->FixParameter( 0 , lpar5 );
            fitCKYLongexpo2->FixParameter( 1 , lpar6 );
            fSig_L = fitCKYLongexpo2->Eval(fT_GeV);
            if ( lpar5 == 0 || lpar6 == 0 )
                fSig_L = 0;
            fitCKYLongexpo2 = NULL;
            delete fitCKYLongexpo2;
        }
        else {
            fSig_L = 0;
        }
        // -------------------------------------------------------------------------------------------

        if ( ( fT_GeV > 0.0 ) && ( fT_GeV < 0.15 ) ) {
            eicSigmaT( fW_GeV,  fQsq_GeV, tpar0, tpar1, tpar2 , tpar3 , tpar4 );
            TF1 *fitCKYTranspol2 = new TF1("sigmaL","pol2", 0.0 , 0.2 );
            fitCKYTranspol2->FixParameter( 0 , tpar0 );
            fitCKYTranspol2->FixParameter( 1 , tpar1 );
            fitCKYTranspol2->FixParameter( 2 , tpar2 );
            fSig_T = fitCKYTranspol2->Eval(fT_GeV);
            if ( tpar0 == 0 || tpar1 == 0 || tpar2 == 0 )
                fSig_T = 0;
            fitCKYTranspol2 = NULL;
            delete fitCKYTranspol2;
        }
        else if ( ( fT_GeV > 0.2 ) && ( fT_GeV < 1.3 ) ) {
            eicSigmaT( fW_GeV,  fQsq_GeV, tpar0, tpar1, tpar2 , tpar3 , tpar4 );
            TF1 *fitCKYTransexpo = new TF1("sigmaL","expo", 0.2 , 1.3 );
            fitCKYTransexpo->FixParameter( 0 , tpar3 );
            fitCKYTransexpo->FixParameter( 1 , tpar4 );
            fSig_T = fitCKYTransexpo->Eval(fT_GeV);
            if ( tpar3 == 0 || tpar4 == 0 )
                fSig_T = 0;
            fitCKYTransexpo = NULL;
            delete fitCKYTransexpo;
        }

        // -------------------------------------------------------------------------------------------

        fSig_VR = fSig_T + fEpsilon * fSig_L;
/*}}}*/

        // ------------------------------------------------------------------------------------------------------
        // CKY sigma L and T ends
        // ------------------------------------------------------------------------------------------------------
        fSigma_Col = fSig_VR * fFlux_Factor_Col * fA * fJacobian_CM_Col;/*{{{*/

        // cout << endl;
        // cout << setw(12) << "Qsq" << setw(12) << "-t" << setw(12) << "W"
        // 	 << setw(12) << "x" << setw(12) << "y" << setw(12) << "z"
        // 	 << setw(12) << "epsilon" << setw(12) << "phi" << setw(12) << "phis"
        // 	 << setw(12) << "sig"
        // 	 << endl;    
        // cout << setw(12) << fQsq_GeV << setw(12) << fT_GeV << setw(12) << fW_GeV
        // 	 << setw(12) << fx << setw(12) << fy << setw(12) << fz
        // 	 << setw(12) << fEpsilon << setw(12) << fPhi << setw(12) << fPhiS
        // 	 << setw(12) << fSigma_Col
        // 	 << endl; 

        // cout << endl;
        // cout << setw(12) << "Particle" << setw(12) << "Px" << setw(12) << "Py" << setw(12) << "Pz" << setw(12) << "Energy" << endl;
        // cout << setw(12) << "Pion"     
        // 	 << setw(12) <<  lpiong.X()
        // 	 << setw(12) <<  lpiong.Y()
        // 	 << setw(12) <<  lpiong.Z()
        // 	 << setw(12) <<  lpiong.E()
        // 	 << endl;
        // cout << setw(12) << "Elec"     
        // 	 << setw(12) <<  lscatelecg.X()
        // 	 << setw(12) <<  lscatelecg.Y()
        // 	 << setw(12) <<  lscatelecg.Z()
        // 	 << setw(12) <<  lscatelecg.E()
        // 	 << endl;
        // cout << setw(12) << "Neutron"     
        // 	 << setw(12) <<  lneutrong.X()
        // 	 << setw(12) <<  lneutrong.Y()
        // 	 << setw(12) <<  lneutrong.Z()
        // 	 << setw(12) <<  lneutrong.E()
        // 	 << endl;

        if ( ( fSigma_Col <= 0 ) || std::isnan( fSigma_Col ) ) { 
            fNSigmaNeg ++;
            continue;
        }
/*}}}*/

        // -----------------------------------------------------------------------------------------------------------------
        //             Lab cross section     Phase Space   Conversion     Luminosity                Total events tried
        // Hz        = ub / ( sr^2 * GeV ) * GeV * sr^2 * ( cm^2 / ub ) * ( # / ( cm^2 * sec ) ) / ( # )
        // ------------------------------------------------------------------------------------------------------------------
        fEventWeight = fSigma_Col * fPSF * fuBcm2 * fLumi / fNEvents;   // in Hz

        fNRecorded ++;
        fLundRecorded++;
        fRatio = fNRecorded / fNGenerated;
        t1->Fill();

        ppiOut << "3"/*{{{*/
            << " \t " << fPhi           // var 1
            << " \t " << fPhiS          // var 2
            << " \t " << fx             // var 3
            << " \t " << "1"	       
            << " \t " << fQsq_GeV       // var 4
            << " \t " << fT_GeV         // var 5
            << " \t " << fW_GeV 	       // var 6
            << " \t " << fEpsilon       // var 7
            << " \t " << fEventWeight   // var 8	   
            << endl;

        // Pion -
        ppiOut << setw(10) << "1" 
            << setw(10) << "1" 
            << setw(10) << "1" 
            << setw(10) << "211" 
            << setw(10) << "0" 
            << setw(10) << "0" 
            << setw(16) << lpiong.X()
            << setw(16) << lpiong.Y()   
            << setw(16) << lpiong.Z()  
            << setw(16) << lpiong.E()
            << setw(16) << fPion_Mass_GeV
            << setw(16) << fVertex_X
            << setw(16) << fVertex_Y
            << setw(16) << fVertex_Z
            << endl;

        // Electron
        ppiOut << setw(10) << "2" 
            << setw(10) << "-1" 
            << setw(10) << "1" 
            << setw(10) << "11" 
            << setw(10) << "0" 
            << setw(10) << "0" 
            << setw(16) << lscatelecg.X() 
            << setw(16) << lscatelecg.Y() 
            << setw(16) << lscatelecg.Z() 
            << setw(16) << lscatelecg.E()
            << setw(16) << fElectron_Mass_GeV
            << setw(16) << fVertex_X
            << setw(16) << fVertex_Y
            << setw(16) << fVertex_Z
            << endl;

        // Neutron
        ppiOut << setw(10) << "3" 
            << setw(10) << "1" 
            << setw(10) << "1" 
            << setw(10) << "2112" 
            << setw(10) << "0" 
            << setw(10) << "0" 
            << setw(16) << lneutrong.X() 
            << setw(16) << lneutrong.Y()
            << setw(16) << lneutrong.Z()
            << setw(16) << lneutrong.E()
            << setw(16) << fNeutron_Mass_GeV
            << setw(16) << fVertex_X
            << setw(16) << fVertex_Y
            << setw(16) << fVertex_Z
            << endl;
/*}}}*/
} // This is the loop over total events.

tTime.Stop();
tTime.Print();
TDatime deTime;
cout << "End Time:   " << deTime.GetHour() << ":" << deTime.GetMinute() << endl;

ppiDetails << "Total events tried                           " << setw(50) << fNGenerated   << endl;/*{{{*/
ppiDetails << "Total events recorded                        " << setw(50) << fNRecorded    << endl;

ppiDetails << "Number of events with w more than 10.6       " << setw(50) << w_ev       << endl;
ppiDetails << "Number of events with wsq negative           " << setw(50) << w_neg_ev   << endl;
ppiDetails << "Number of events with qsq less than 4        " << setw(50) << qsq_ev     << endl;
ppiDetails << "Number of events with -t more than threshold " << setw(50) << t_ev       << endl;

ppiDetails << "Number of events with w less than threshold  " << setw(50) << fWsqNeg       << endl;
ppiDetails << "Number of events with mom not conserve       " << setw(50) << fNMomConserve << endl;
ppiDetails << "Number of events with Sigma negative         " << setw(50) << fNSigmaNeg    << endl;
ppiDetails << "Number of lund events                        " << setw(50) << fLundRecorded << endl;/*}}}*/

cout << "Total events tried                           " << setw(50) << fNGenerated   << endl;/*{{{*/
cout << "Total events recorded                        " << setw(50) << fNRecorded    << endl;

cout << "Number of events with w more than 10.6       " << setw(50) << w_ev       << endl;
cout << "Number of events with wsq negative           " << setw(50) << w_neg_ev   << endl;
cout << "Number of events with qsq less than 4        " << setw(50) << qsq_ev     << endl;
cout << "Number of events with -t more than threshold " << setw(50) << t_ev       << endl;

cout << "Number of events with w less than threshold  " << setw(50) << fWsqNeg       << endl;
cout << "Number of events with mom not conserve       " << setw(50) << fNMomConserve << endl;
cout << "Number of events with Sigma negative         " << setw(50) << fNSigmaNeg    << endl;
cout << "Number of lund events                        " << setw(50) << fLundRecorded << endl;/*}}}*/

ppiOut.close();
ppiDetails.close();
f->cd();
t1->Write();
f->Close();


return 0;
}
# endif
//---------------------------------------------------------
