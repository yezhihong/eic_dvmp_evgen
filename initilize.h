#include <iostream>
#include "TRandom.h"
#include "TRandom2.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;

void pim::Initilize() { /*{{{*/
  fRandom = new TRandom2(0);
  fRandom->GetSeed();

  allset                                      = false;/*{{{*/
  print                                       = false;
  kCalcFermi                                  = false;
  kCalcBremss                                 = false;
  kCalcIon                                    = false;
  kCalcBremssEle                              = false;
  kCalcIonEle                                 = false;
  kFSI                                        = false;
  kMSele                                      = false;
  kMS                                         = false;/*}}}*/
  

  //////////////////////////////////////
  //
  // Change your kinematic setting here!
  //
  //////////////////////////////////////*{{{*/
  fLumi                                       = 1.000e33; // https://eic.jlab.org/wiki/index.php/EIC_luminosity, changed into EicC
  fuBcm2                                      = 1.0e-30;
  fPI                                         = 3.1415926;
  fDEG2RAD                                    = fPI/180.0;
  fRAD2DEG                                    = 180.0/fPI;
  fEBeam                                      = 3.5;  // GeV
  fPBeam                                      = 20.0; // GeV
  fScatElec_Theta_I                           = 0.0 * fDEG2RAD;
  fScatElec_Theta_F                           = 180.0 * fDEG2RAD;
  fScatElec_E_Lo                              = 0.5;  // % of beam energy
  fScatElec_E_Hi                              = 2.5;  // % of beam energy
  fPion_Theta_I                               = 0.0 * fDEG2RAD;
  fPion_Theta_F                               = 180.0 * fDEG2RAD;
  fPSF                                        = ( fEBeam * ( fScatElec_E_Hi - fScatElec_E_Lo ) * 
						  ( sin( fScatElec_Theta_F ) - sin( fScatElec_Theta_I ) ) * 2 * fPI * 
						  ( sin( fPion_Theta_F     ) - sin( fPion_Theta_I     ) ) * 2 * fPI );
/*}}}*/

  //constants{{{
  fK                                          = 1000.0;
  fm                                          = 1.0/1000.0;
  fElectron_Mass                              = 0.511;
  fElectron_Mass_GeV                          = fElectron_Mass/1000.0;
  fProton_Mass                                = 938.27; // Its is the mass of Proton which in SoLID DVMP is outgoing reocil Proton
  fProton_Mass_GeV                            = fProton_Mass/1000.0;
  fNeutron_Mass                               = 939.57; // It is the mass of Neutron. The target is Neutron in SoLID DVMP.
  fNeutron_Mass_GeV                           = fNeutron_Mass/1000.0;
  fRecoilProton_Mass                          = 938.27;
  fRecoilProton_Mass_GeV                      = fRecoilProton_Mass/1000.0;
  fPion_Mass                                  = 139.57;
  fPion_Mass_GeV                              = fPion_Mass/1000.0;
  fDiff                                       = 0.5;
  fElectron_Kin_Col_GeV                       = fEBeam;
  fElectron_Kin_Col                           = fElectron_Kin_Col_GeV * 1000.0;
  fAlpha                                      = 1./137.036;
  fMom_Ratio                                  = 0.460029;
  fMom_Dif                                    = 0.01;
  fPi                                         = TMath::Pi(); /*}}}*/

  fMandSConserve                              = 0;/*{{{*/
  fEnergyConserve                             = 0;
  fXMomConserve                               = 0;
  fYMomConserve                               = 0;
  fZMomConserve                               = 0;
  fXMomConserve_RF                            = 0;
  fYMomConserve_RF                            = 0;
  fZMomConserve_RF                            = 0;
  fEnergyConserve_RF                          = 0;
  fPion_Mom_Same                              = 0;
  fTop_Pion_Mom                               = 0;
  fBot_Pion_Mom                               = 0;
  fS_I_RF                                     = 0;
  fS_F_RF                                     = 0;
  fS_I_Col                                    = 0;
  fS_F_Col                                    = 0;
  fS_I_RF_GeV                                 = 0;
  fS_F_RF_GeV                                 = 0;
  fS_I_Col_GeV                                = 0;
  fS_F_Col_GeV                                = 0;
  fPion_Alpha                                 = 0;
  fPion_Beta                                  = 0;
  fNRecorded                                  = 0;
  fLundRecorded                               = 0;
  fNGenerated                                 = 0;
  fRatio                                      = 0;
  fWLessShell                                 = 0;
  fWLess1P9                                   = 0;
  fWSqNeg                                     = 0;
  fNSigmaNeg                                  = 0;
  fNMomConserve                               = 0;
  fSDiff                                      = 0;
  fScatElecEnergyLess                         = 0;
  fScatElecThetaLess                          = 0;
  fPionEnergyCMLess                           = 0;
  fSNotEqual                                  = 0;
  fVertex_X                                   = 0;
  fVertex_Y                                   = 0;
  fVertex_Z                                   = 0;/*}}}*/

  fProton_Energy_Col                          = 0;/*{{{*/
  fProton_Mom_Col                             = 0;
    fProton_Theta_Col                           = 0;
  fProton_Phi_Col                             = 0;
  fProton_MomZ_Col                            = 0;
  fProton_MomX_Col                            = 0;
  fProton_MomY_Col                            = 0;
  fProton_Energy_Col_GeV                      = 0;
  fProton_Mom_Col_GeV                         = 0;
  fProton_MomX_Col_GeV                        = 0;
  fProton_MomY_Col_GeV                        = 0;
  fProton_MomZ_Col_GeV                        = 0;/*}}}*/

  fTarget_Energy_Col                          = 0;/*{{{*/
  fTarget_Mom_Col                             = 0;
  fTarget_Theta_Col                           = 0;
  fTarget_Phi_Col                             = 0;
  fTarget_MomZ_Col                            = 0;
  fTarget_MomX_Col                            = 0;
  fTarget_MomY_Col                            = 0;
  fTarget_Energy_Col_GeV                      = 0;
  fTarget_Mom_Col_GeV                         = 0;
  fTarget_MomX_Col_GeV                        = 0;
  fTarget_MomY_Col_GeV                        = 0;
  fTarget_MomZ_Col_GeV                        = 0;
  fTarget_Pol0_Col                            = 0;
  fTarget_PolX_Col                            = 0;
  fTarget_PolY_Col                            = 0;
  fTarget_PolZ_Col                            = 0;
  fTarget_Pol0_RF                             = 0;
  fTarget_PolX_RF                             = 0;
  fTarget_PolY_RF                             = 0;
  fTarget_PolZ_RF                             = 0;/*}}}*/
  
  fBetaX_Col_RF                               = 0;
  fBetaY_Col_RF                               = 0;
  fBetaZ_Col_RF                               = 0;
  fBeta_Col_RF                                = 0;
  fGamma_Col_RF                               = 0;
  
  fProton_MomX_RF                             = 0;/*{{{*/
  fProton_MomY_RF                             = 0;
  fProton_MomZ_RF                             = 0;
  fProton_Mom_RF                              = 0;
  fProton_Energy_RF                           = 0;
  fProton_Energy_RF_GeV                       = 0;
  fProton_MomX_RF_GeV                         = 0;
  fProton_MomY_RF_GeV                         = 0;
  fProton_MomZ_RF_GeV                         = 0;
  fProton_Mom_RF_GeV                          = 0;
  fProton_Kin_Col_GeV                         = 0;/*}}}*/
  fScatElec_Angle                             = 0;
  fScatElec_Alpha_RF                          = 0;
  fScatElec_Beta_RF                           = 0;

  fRadiation_Lenght_Air                       = 0;/*{{{*/
  fElectron_Targ_Thickness                    = 0;
  fElectron_Targ_Thickness_RadLen             = 0;
  fElectron_Targ_BT                           = 0;
  fElectron_Targ_Bremss_Loss                  = 0;
  fElectron_Targ_Ion_Loss                     = 0;
  fElectron_TargWindow_Bremss_Loss            = 0;
  fElectron_TargWindow_Ion_Loss               = 0;
  fElectron_Air_Thickness                     = 0;
  fElectron_Air_Thickness_RadLen              = 0;
  fElectron_Air_BT                            = 0;
  fElectron_Air_Bremss_Loss                   = 0;
  fElectron_Air_Ion_Loss                      = 0;
  fElectron_Corrected_Theta_Col               = 0;
  fElectron_Corrected_Phi_Col                 = 0;
  fElectron_Corrected_Energy_Col              = 0;
  fElectron_Corrected_Mom_Col                 = 0;
  fElectron_Corrected_MomX_Col                = 0;
  fElectron_Corrected_MomY_Col                = 0;
  fElectron_Corrected_MomZ_Col                = 0;
  fElectron_Delta_Mom_Col                     = 0;
  fElectron_Corrected_Energy_Col_GeV          = 0;
  fElectron_Corrected_Mom_Col_GeV             = 0;
  fElectron_Corrected_MomX_Col_GeV            = 0;
  fElectron_Corrected_MomY_Col_GeV            = 0;
  fElectron_Corrected_MomZ_Col_GeV            = 0;
  fElectron_Delta_Mom_Col_GeV                 = 0;

  fElectron_Energy_Col                        = 0;
  fElectron_MomZ_Col                          = 0;
  fElectron_MomX_Col                          = 0;
  fElectron_MomY_Col                          = 0;
  fElectron_Theta_Col                         = 0;
  fElectron_Phi_Col                           = 0;
  fElectron_Mom_Col                           = 0;
  
  fElectron_MS_Energy_Col                     = 0;
  fElectron_MS_MomZ_Col                       = 0;
  fElectron_MS_MomX_Col                       = 0;
  fElectron_MS_MomY_Col                       = 0;
  fElectron_MS_Theta_Col                      = 0;
  fElectron_MS_Phi_Col                        = 0;
  fElectron_MS_Mom_Col                        = 0;/*}}}*/

  fElectron_Energy_Col_GeV                    = 0;/*{{{*/
  fElectron_Mom_Col_GeV                       = 0;
  fElectron_MomX_Col_GeV                      = 0;
  fElectron_MomY_Col_GeV                      = 0;
  fElectron_MomZ_Col_GeV                      = 0;
  fScatElec_Targ_Thickness                    = 0;
  fScatElec_Targ_Thickness_RadLen             = 0;
  fScatElec_Targ_BT                           = 0;
  fScatElec_Targ_Bremss_Loss                  = 0;
  fScatElec_Targ_Ion_Loss                     = 0;
  fScatElec_Air_Thickness                     = 0;
  fScatElec_Air_Thickness_RadLen              = 0;
  fScatElec_Air_BT                            = 0;
  fScatElec_Air_Bremss_Loss                   = 0;
  fScatElec_Air_Ion_Loss                      = 0;
  fScatElec_Corrected_Theta_Col               = 0;
  fScatElec_Corrected_Phi_Col                 = 0;
  fScatElec_Corrected_Energy_Col              = 0;
  fScatElec_Corrected_Mom_Col                 = 0;
  fScatElec_Corrected_MomX_Col                = 0;
  fScatElec_Corrected_MomY_Col                = 0;
  fScatElec_Corrected_MomZ_Col                = 0;
  fScatElec_Delta_Mom_Col                     = 0;
  fScatElec_Corrected_Energy_Col_GeV          = 0;
  fScatElec_Corrected_Mom_Col_GeV             = 0;
  fScatElec_Corrected_MomX_Col_GeV            = 0;
  fScatElec_Corrected_MomY_Col_GeV            = 0;
  fScatElec_Corrected_MomZ_Col_GeV            = 0;
  fScatElec_Delta_Mom_Col_GeV                 = 0;
  fScatElec_Energy_Col                        = 0;
  fScatElec_MomZ_Col                          = 0;
  fScatElec_MomX_Col                          = 0;
  fScatElec_MomY_Col                          = 0;
  fScatElec_Theta_Col                         = 0;
  fScatElec_Phi_Col                           = 0;
  fScatElec_Mom_Col                           = 0;
  fScatElec_Energy_Col_GeV                    = 0;
  fScatElec_Mom_Col_GeV                       = 0;
  fScatElec_MomX_Col_GeV                      = 0;
  fScatElec_MomY_Col_GeV                      = 0;
  fScatElec_MomZ_Col_GeV                      = 0;

  fScatElec_MS_Energy_Col                     = 0;
  fScatElec_MS_MomZ_Col                       = 0;
  fScatElec_MS_MomX_Col                       = 0;
  fScatElec_MS_MomY_Col                       = 0;
  fScatElec_MS_Theta_Col                      = 0;
  fScatElec_MS_Phi_Col                        = 0;
  fScatElec_MS_Mom_Col                        = 0;
  
  fScatElec_TargWindow_Bremss_Loss            = 0;
  fScatElec_TargWindow_Ion_Loss               = 0;/*}}}*/

  fTargWindow_Thickness                       = 0;
  fTargWindow_Thickness_RadLen                = 0;
  fTargWindow_BT                              = 0; 
  fPion_TargWindow_Ion_Loss                   = 0;
  fNeutron_TargWindow_Ion_Loss                = 0;
  
  fPion_MS_Energy_Col                         = 0;/*{{{*/
  fPion_MS_MomZ_Col                           = 0;
  fPion_MS_MomX_Col                           = 0;
  fPion_MS_MomY_Col                           = 0;
  fPion_MS_Theta_Col                          = 0;
  fPion_MS_Phi_Col                            = 0;
  fPion_MS_Mom_Col                            = 0;

  fPion_Targ_Thickness                        = 0;
  fPion_Targ_Thickness_RadLen                 = 0;
  fPion_Targ_BT                               = 0;
  fPion_Targ_Bremss_Loss                      = 0;
  fPion_Targ_Ion_Loss                         = 0;
  fPion_Air_Thickness                         = 0;
  fPion_Air_Thickness_RadLen                  = 0;
  fPion_Air_BT                                = 0;
  fPion_Air_Bremss_Loss                       = 0;
  fPion_Air_Ion_Loss                          = 0;
  fPion_Corrected_Theta_Col                   = 0;
  fPion_Corrected_Phi_Col                     = 0;
  fPion_Corrected_Energy_Col                  = 0;
  fPion_Corrected_Mom_Col                     = 0;
  fPion_Corrected_MomX_Col                    = 0;
  fPion_Corrected_MomY_Col                    = 0;
  fPion_Corrected_MomZ_Col                    = 0;
  fPion_Delta_Mom_Col                         = 0;
  fPion_Corrected_Energy_Col_GeV              = 0;
  fPion_Corrected_Mom_Col_GeV                 = 0;
  fPion_Corrected_MomX_Col_GeV                = 0;
  fPion_Corrected_MomY_Col_GeV                = 0;
  fPion_Corrected_MomZ_Col_GeV                = 0;
  fPion_Delta_Mom_Col_GeV                     = 0;
  
  fPion_Energy_Col                            = 0;
  fPion_MomZ_Col                              = 0;
  fPion_MomX_Col                              = 0;
  fPion_MomY_Col                              = 0;
  fPion_Theta_Col                             = 0;
  fPion_Phi_Col                               = 0;
  fPion_Mom_Col                               = 0;
  fPion_Energy_Col_GeV                        = 0;
  fPion_Mom_Col_GeV                           = 0;
  fPion_MomX_Col_GeV                          = 0;
  fPion_MomY_Col_GeV                          = 0;
  fPion_MomZ_Col_GeV                          = 0;
  
  fPion_FSI_Energy_Col                        = 0;
  fPion_FSI_MomZ_Col                          = 0;
  fPion_FSI_MomX_Col                          = 0;
  fPion_FSI_MomY_Col                          = 0;
  fPion_FSI_Theta_Col                         = 0;
  fPion_FSI_Phi_Col                           = 0;
  fPion_FSI_Mom_Col                           = 0;
  fPion_FSI_Energy_Col_GeV                    = 0;
  fPion_FSI_Mom_Col_GeV                       = 0;
  fPion_FSI_MomX_Col_GeV                      = 0;
  fPion_FSI_MomY_Col_GeV                      = 0;
  fPion_FSI_MomZ_Col_GeV                      = 0;
/*}}}*/

  fNeutron_MS_Energy_Col                      = 0;/*{{{*/
  fNeutron_MS_MomZ_Col                        = 0;
  fNeutron_MS_MomX_Col                        = 0;
  fNeutron_MS_MomY_Col                        = 0;
  fNeutron_MS_Theta_Col                       = 0;
  fNeutron_MS_Phi_Col                         = 0;
  fNeutron_MS_Mom_Col                         = 0;

  fNeutron_Targ_Thickness                     = 0;
  fNeutron_Targ_Thickness_RadLen              = 0;
  fNeutron_Targ_BT                            = 0;
  fNeutron_Targ_Bremss_Loss                   = 0;
  fNeutron_Targ_Ion_Loss                      = 0;
  fNeutron_Air_Thickness                      = 0;
  fNeutron_Air_Thickness_RadLen               = 0;
  fNeutron_Air_BT                             = 0;
  fNeutron_Air_Bremss_Loss                    = 0;
  fNeutron_Air_Ion_Loss                       = 0;
  fNeutron_Corrected_Theta_Col                = 0;
  fNeutron_Corrected_Phi_Col                  = 0;
  fNeutron_Corrected_Energy_Col               = 0;
  fNeutron_Corrected_Mom_Col                  = 0;
  fNeutron_Corrected_MomX_Col                 = 0;
  fNeutron_Corrected_MomY_Col                 = 0;
  fNeutron_Corrected_MomZ_Col                 = 0;
  fNeutron_Delta_Mom_Col                      = 0;
  fNeutron_Corrected_Energy_Col_GeV           = 0;
  fNeutron_Corrected_Mom_Col_GeV              = 0;
  fNeutron_Corrected_MomX_Col_GeV             = 0;
  fNeutron_Corrected_MomY_Col_GeV             = 0;
  fNeutron_Corrected_MomZ_Col_GeV             = 0;
  fNeutron_Delta_Mom_Col_GeV                  = 0;
  fNeutron_Energy_Col                         = 0;
  fNeutron_MomZ_Col                           = 0;
  fNeutron_MomX_Col                           = 0;
  fNeutron_MomY_Col                           = 0;
  fNeutron_Theta_Col                          = 0;
  fNeutron_Phi_Col                            = 0;
  fNeutron_Mom_Col                            = 0;
  fNeutron_Energy_Col_GeV                     = 0;
  fNeutron_Mom_Col_GeV                        = 0;
  fNeutron_MomX_Col_GeV                       = 0;
  fNeutron_MomY_Col_GeV                       = 0;
  fNeutron_MomZ_Col_GeV                       = 0;/*}}}*/

  fRecoilProton_Targ_Thickness                = 0;/*{{{*/
  fRecoilProton_Targ_Thickness_RadLen         = 0;
  fRecoilProton_Targ_BT                       = 0;
  fRecoilProton_Targ_Bremss_Loss              = 0;
  fRecoilProton_Targ_Ion_Loss                 = 0;
  fRecoilProton_Air_Thickness                 = 0;
  fRecoilProton_Air_Thickness_RadLen          = 0;
  fRecoilProton_Air_BT                        = 0;
  fRecoilProton_Air_Bremss_Loss               = 0;
  fRecoilProton_Air_Ion_Loss                  = 0;
  fRecoilProton_Theta_Col                     = 0;
  fRecoilProton_Phi_Col                       = 0;
  fRecoilProton_Energy_Col                    = 0;
  fRecoilProton_Mom_Col                       = 0;
  fRecoilProton_MomX_Col                      = 0;
  fRecoilProton_MomY_Col                      = 0;
  fRecoilProton_MomZ_Col                      = 0;
  fRecoilProton_Corrected_Energy_Col          = 0;
  fRecoilProton_Corrected_Mom_Col             = 0;
  fRecoilProton_Corrected_MomX_Col            = 0;
  fRecoilProton_Corrected_MomY_Col            = 0;
  fRecoilProton_Corrected_MomZ_Col            = 0;
  fRecoilProton_Corrected_Theta_Col           = 0;
  fRecoilProton_Corrected_Phi_Col             = 0;
  fRecoilProton_Delta_Mom_Col                 = 0;
  fRecoilProton_Energy_Col_GeV                = 0;
  fRecoilProton_Mom_Col_GeV                   = 0;
  fRecoilProton_MomX_Col_GeV                  = 0;
  fRecoilProton_MomY_Col_GeV                  = 0;
  fRecoilProton_MomZ_Col_GeV                  = 0;
  fRecoilProton_Corrected_Energy_Col_GeV      = 0;
  fRecoilProton_Corrected_Mom_Col_GeV         = 0;
  fRecoilProton_Corrected_MomX_Col_GeV        = 0;
  fRecoilProton_Corrected_MomY_Col_GeV        = 0;
  fRecoilProton_Corrected_MomZ_Col_GeV        = 0;
  fRecoilProton_Delta_Mom_Col_GeV             = 0;/*}}}*/

  fSSAsym                                     = 0;/*{{{*/
  fSineAsym                                   = 0;
  fT                                          = 0;
  fT_GeV                                      = 0;
  fProton_Kin_Col                             = 0;
  fQsq_Value                                  = 0;
  fQsq_Dif                                    = 0;
  fQsq_GeV                                    = 0;
  fQsq                                        = 0;
  fW_GeV_Col                                  = 0;
  fW_Col                                      = 0;
  fW                                          = 0;   
  fW_GeV                                      = 0;
  fW                                          = 0;   
  fW_GeV                                      = 0;
  fW_Prime_GeV                                = 0;
  fW_Corrected_Prime_GeV                      = 0;
  fWSq                                        = 0;   
  fWSq_GeV                                    = 0;
  fWSq_PiN                                    = 0;   
  fWSq_PiN_GeV                                = 0;
  fWSq_Top_PiN_GeV                            = 0;
  fWSq_Bot_PiN_GeV                            = 0;/*}}}*/
  
  fScatElec_Mom_RF                            = 0;/*{{{*/
  fScatElec_Mom_RF_GeV                        = 0;
  fScatElec_Energy_RF                         = 0;
  fScatElec_Energy_RF_GeV                     = 0;
  fElec_ScatElec_Theta_RF                     = 0;
  fScatElec_Cone_Phi_RF                       = 0;
  fScatElec_Theta_RF                          = 0;
  fScatElec_Phi_RF                            = 0;
  fScatElec_MomX_RF                           = 0;
  fScatElec_MomZ_RF                           = 0;
  fScatElec_MomY_RF                           = 0;
  fElectron_Energy_RF                         = 0;
  fElectron_Mom_RF                            = 0;
  fElectron_Theta_RF                          = 0;
  fElectron_Phi_RF                            = 0;
  fElectron_MomX_RF                           = 0;
  fElectron_MomZ_RF                           = 0;
  fElectron_MomY_RF                           = 0;
  fPhoton_Energy_RF                           = 0;
  fPhoton_Mom_RF                              = 0;
  fPhoton_Energy_RF_GeV                       = 0;
  fPhoton_Mom_RF_GeV                          = 0;
  fProton_Energy_CM                           = 0;
  fProton_Energy_CM_GeV                       = 0;
  fProton_Mom_CM                              = 0;
  fProton_Mom_CM_GeV                          = 0;
  fPhoton_Energy_CM                           = 0;
  fPhoton_Mom_CM                              = 0;
  fPhoton_Energy_CM_GeV                       = 0;
  fPhoton_Mom_CM_GeV                          = 0;
  fPion_Theta_CM                              = 0;
  fPion_Phi_CM                                = 0;
  fPion_Energy_CM                             = 0;
  fPion_Mom_CM                                = 0;
  fPion_Energy_CM_GeV                         = 0;
  fPion_Mom_CM_GeV                            = 0;
  fBeta_CM_RF                                 = 0;
  fGamma_CM_RF                                = 0;
  fNeutron_Theta_CM                           = 0;
  fNeutron_Phi_CM                             = 0;
  fNeutron_Energy_CM                          = 0;
  fNeutron_Energy_CM_GeV                      = 0;
  fNeutron_Mom_CM                             = 0;
  fNeutron_Mom_CM_GeV                         = 0;
  fPhoton_MomZ_RF                             = 0;
  fPhoton_MomX_RF                             = 0;
  fPhoton_MomY_RF                             = 0;
  fPhoton_Theta_RF                            = 0;
  fPhoton_Phi_RF                              = 0;
  fPion_Energy_RF                             = 0;
  fPion_Mom_RF                                = 0;
  fPion_Energy_RF_GeV                         = 0;
  fPion_Mom_RF_GeV                            = 0;
  fPiqVec_Theta_RF                            = 0;
  fPion_Mom_RF                                = 0;
  fPion_Mom_RF_GeV                            = 0;
  fPion_MomX_RF                               = 0;
  fPion_MomY_RF                               = 0;
  fPion_MomZ_RF                               = 0;
  fPion_Theta_RF                              = 0;
  fPion_Phi_RF                                = 0;
  fPion_MomX_RF_GeV                           = 0;
  fPion_MomY_RF_GeV                           = 0;
  fPion_MomZ_RF_GeV                           = 0;/*}}}*/

  fT_Para                                     = 0;
  fT_Para_GeV                                 = 0;
  fEpsilon                                    = 0;
  fx                                          = 0;
  fy                                          = 0;
  fz                                          = 0;
 
  fNeutron_Energy_RF                          = 0;/*{{{*/
  fNeutron_Energy_RF_GeV                      = 0;
  fNeutron_Mom_RF                             = 0;
  fNeutron_Mom_RF_GeV                         = 0;
  fNeutron_qVec_Theta_RF                      = 0;
  fNeutron_MomX_RF                            = 0;
  fNeutron_MomY_RF                            = 0;
  fNeutron_MomZ_RF                            = 0;
  fNeutron_Theta_RF                           = 0;
  fNeutron_Phi_RF                             = 0;
  fRecoilProton_Energy_RF                     = 0;
  fRecoilProton_Mom_RF                        = 0;
  fRecoilProton_MomX_RF                       = 0;
  fRecoilProton_MomY_RF                       = 0;
  fRecoilProton_MomZ_RF                       = 0;
  fRecoilProton_Energy_RF_GeV                 = 0;
  fRecoilProton_Mom_RF_GeV                    = 0;
  fRecoilProton_MomX_RF_GeV                   = 0;
  fRecoilProton_MomY_RF_GeV                   = 0;
  fRecoilProton_MomZ_RF_GeV                   = 0;
  fRecoilProton_Theta_RF                      = 0;
  fRecoilProton_Phi_RF                        = 0;
  fElectron_MomX_RF_GeV                       = 0;
  fElectron_MomY_RF_GeV                       = 0;
  fElectron_MomZ_RF_GeV                       = 0;
  fPhoton_MomX_RF_GeV                         = 0;
  fPhoton_MomY_RF_GeV                         = 0;
  fPhoton_MomZ_RF_GeV                         = 0;
  fScatElec_MomX_RF_GeV                       = 0;
  fScatElec_MomY_RF_GeV                       = 0;
  fScatElec_MomZ_RF_GeV                       = 0;
  fNeutron_MomX_RF_GeV                        = 0;
  fNeutron_MomY_RF_GeV                        = 0;
  fNeutron_MomZ_RF_GeV                        = 0;
  fPhoton_MomX_Col_GeV                        = 0;
  fPhoton_MomY_Col_GeV                        = 0;
  fPhoton_MomZ_Col_GeV                        = 0;
  fPion_MomX_Col_GeV                          = 0;
  fPion_MomY_Col_GeV                          = 0;
  fPion_MomZ_Col_GeV                          = 0; 
  fPhoton_Theta_Col                           = 0;
  fPhoton_Phi_Col                             = 0;
  fPhoton_Energy_Col                          = 0;
  fPhoton_Mom_Col                             = 0;
  fPhoton_MomX_Col                            = 0;
  fPhoton_MomZ_Col                            = 0;
  fPhoton_MomY_Col                            = 0;
  fPhoton_Energy_Col_GeV                      = 0;
  fPhoton_Mom_Col_GeV                         = 0;
  fPhoton_MomX_Col_GeV                        = 0;
  fPhoton_MomZ_Col_GeV                        = 0;
  fPhoton_MomY_Col_GeV                        = 0;/*}}}*/

  fWFactor                                    = 0;
  fA                                          = 0;

  fZASigma_UU                                 = 0;/*{{{*/
  fRorySigma_UT                               = 0;
  fSigma_Col                                  = 0;
  fSigma_UUPara                               = 0;
  fSig_VR                                     = 0;
  fSig_L                                      = 0;
  fSig_T                                      = 0;
  fSigmaPhiS                                  = 0;
  fSigmaPhi_Minus_PhiS                        = 0;
  fSigma2Phi_Minus_PhiS                       = 0;
  fSigma3Phi_Minus_PhiS                       = 0;
  fSigmaPhi_Plus_PhiS                         = 0;
  fSigma2Phi_Plus_PhiS                        = 0;
  fSig_Phi_Minus_PhiS                         = 0;
  fSig_PhiS                                   = 0;
  fSig_2Phi_Minus_PhiS                        = 0;
  fSig_Phi_Plus_PhiS                          = 0;
  fSig_3Phi_Minus_PhiS                        = 0;
  fSig_2Phi_Plus_PhiS                         = 0;
  fEventWeight                                = 0;
  fEventWeightMax                             = 0;
  fWilliamsWeight                             = 0;
  fDedrickWeight                              = 0;
  fCatchenWeight                              = 0;
  fFlux_Factor_Col                            = 0;
  fFlux_Factor_RF                             = 0;
  fJacobian_CM                                = 0;
  fJacobian_CM_RF                             = 0;
  fJacobian_CM_Col                            = 0;
  fZASig_T                                    = 0;
  fZASig_L                                    = 0;
  fZASig_L2                                   = 0;
  fZASig_LT                                   = 0;
  fZASig_TT                                   = 0;
  fPhi                                        = 0;
  fPhiS                                       = 0;
  fPhi_Corrected                              = 0;
  fPhiS_Corrected                             = 0;      

  fQsq_Corrected_GeV                          = 0;      
  fQsq_Corrected                              = 0;      
  fW_Corrected                                = 0;      
  fW_Corrected_GeV                            = 0;      
  fT_Corrected                                = 0;      
  fT_Corrected_GeV                            = 0;      
  fx_Corrected                                = 0;      
  fy_Corrected                                = 0;      
  fz_Corrected                                = 0;      

  fAsymPhiMinusPhi_S                          = 0;
  fAsymPhi_S                                  = 0;
  fAsym2PhiMinusPhi_S                         = 0;
  fAsymPhiPlusPhi_S                           = 0;
  fAsym3PhiMinusPhi_S                         = 0;
  fAsym2PhiPlusPhi_S                          = 0;
  
  fAsymPhiMinusPhi_S_Col                      = 0;
  fAsymPhi_S_Col                              = 0;
  fAsym2PhiMinusPhi_S_Col                     = 0;
  fAsymPhiPlusPhi_S_Col                       = 0;
  fAsym3PhiMinusPhi_S_Col                     = 0;
  fAsym2PhiPlusPhi_S_Col                      = 0;
  fTerm_PhiMinusPhi_S                         = 0;
  fTerm_Phi_S                                 = 0;
  fTerm_2PhiMinusPhi_S                        = 0;
  fTerm_PhiPlusPhi_S                          = 0;
  fTerm_3PhiMinusPhi_S                        = 0;
  fTerm_2PhiPlusPhi_S                         = 0;
  
  fTerm_PhiMinusPhi_S_Col                     = 0;
  fTerm_Phi_S_Col                             = 0;
  fTerm_2PhiMinusPhi_S_Col                    = 0;
  fTerm_PhiPlusPhi_S_Col                      = 0;
  fTerm_3PhiMinusPhi_S_Col                    = 0;
  fTerm_2PhiPlusPhi_S_Col                     = 0;
/*}}}*/

  fPhoton_Corrected_Theta_Col                 = 0;/*{{{*/
  fPhoton_Corrected_Phi_Col                   = 0;
  fPhoton_Corrected_Energy_Col                = 0;
  fPhoton_Corrected_Mom_Col                   = 0;
  fPhoton_Corrected_MomX_Col                  = 0;
  fPhoton_Corrected_MomZ_Col                  = 0;
  fPhoton_Corrected_MomY_Col                  = 0;
  fPhoton_Corrected_Energy_Col_GeV            = 0;
  fPhoton_Corrected_Mom_Col_GeV               = 0;
  fPhoton_Corrected_MomX_Col_GeV              = 0;
  fPhoton_Corrected_MomZ_Col_GeV              = 0;
  fPhoton_Corrected_MomY_Col_GeV              = 0;
  fPhi_Pion_LeptonPlane_RF                    = 0;
  fCos_Phi_Pion_LeptonPlane_RF                = 0;
  fSin_Phi_Pion_LeptonPlane_RF                = 0;
  fPhi_TargPol_LeptonPlane_RF                 = 0;
  fCos_Phi_TargPol_LeptonPlane_RF             = 0;
  fSin_Phi_TargPol_LeptonPlane_RF             = 0;
  fTheta_Pion_Photon_RF                       = 0;
  fPhi_Pion_LeptonPlane_Col                   = 0;
  fCos_Phi_Pion_LeptonPlane_Col               = 0;
  fSin_Phi_Pion_LeptonPlane_Col               = 0;
  fPhi_TargPol_LeptonPlane_Col                = 0;
  fCos_Phi_TargPol_LeptonPlane_Col            = 0;
  fSin_Phi_TargPol_LeptonPlane_Col            = 0;
  fTheta_Pion_Photon_Col                      = 0;
  fZASigma_UU_Col                             = 0;
  fRorySigma_UT_Col                           = 0;
  fSig_Phi_Minus_PhiS_Col                     = 0;
  fSig_PhiS_Col                               = 0;
  fSig_2Phi_Minus_PhiS_Col                    = 0;
  fSig_Phi_Plus_PhiS_Col                      = 0;
  fSig_3Phi_Minus_PhiS_Col                    = 0;
  fSig_2Phi_Plus_PhiS_Col                     = 0;/*}}}*/

}/*}}}*/

int pim::CheckLaws(TLorentzVector P_E0, TLorentzVector P_t, TLorentzVector P_e, TLorentzVector P_pim, TLorentzVector P_pro) {/*{{{*/
  double energy_check = (P_t.E() + P_E0.E()) - (P_e.E()+P_pim.E()+P_pro.E());
  double px_check =(P_t.Px() + P_E0.Px()) - (P_e.Px()+P_pim.Px()+P_pro.Px()); 
  double py_check =(P_t.Py() + P_E0.Py()) - (P_e.Py()+P_pim.Py()+P_pro.Py()); 
  double pz_check =(P_t.Pz() + P_E0.Pz()) - (P_e.Pz()+P_pim.Pz()+P_pro.Pz()); 
  
  Int_t err = -1;
  if( fabs( energy_check ) < fDiff &&
      fabs( px_check )     < fDiff &&
      fabs( py_check )     < fDiff &&
      fabs( pz_check )     < fDiff )
    err = 1;
  /* else{  */
  /*   cerr<<Form("*** dE = %f,  dPx = %f, dPy = %f, dPz = %f", energy_check, px_check, py_check, pz_check)<<endl;  */
  /*   err = -1;  */
  /* } */
  
  return err;
  
}/*}}}*/

//---------------------------------------------------------
