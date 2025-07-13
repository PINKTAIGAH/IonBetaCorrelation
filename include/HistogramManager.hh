#ifndef HISTOGRAM_MANAGER_H
#define HISTOGRAM_MANAGER_H

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TObject.h"

#include "GlobalConstants.hh"
#include "ConfigReader.hh"

namespace HistogramConstants {
  const Long64_t wrExperimentStart = 1.7401830e+18;                                   // White rabbit start time of files
  const Long64_t wrExperimentEnd = 1.74022529e+18;                                    // White rabbit end time of files,
  const Double_t slicesEvery = 1;                                                     // Size of white rabbit histogram bins
  const Long64_t durationInSeconds = (wrExperimentEnd - wrExperimentStart) / 1e9;     // Duration of experiment
  const Long64_t numberOfSlices = durationInSeconds / slicesEvery;                    // Number of white rabbit histogram bins
  const Double_t implantDecayBinDt = 1e6;

}


class HistogramManager{
  private:

    TFile* outputFile; 

    void InitialiseHistograms();
  
  public:

    // Histograms for implant beta matches
    TH2D* h2_aida_implant_xy;
    TH2D* h2_aida_matched_xy;
    TH1D* h1_aida_wr_times;
    TH1D* h1_onspill_times;
    TH1D* h1_aida_implant_beta_dt;
    TH1D* h1_aida_implant_beta_dt_forward;
    TH1D* h1_aida_implant_beta_dt_backward;
    TH1D* h1_aida_implant_beta_secondmatch_dt;
    TH1D* h1_aida_implant_beta_thirdmatch_dt;
    TH1D* h1_aida_implant_beta_tenthmatch_dt;

    // Histograms for beta candidate events
    TH1D* h1_implantbeta_candidate_multiplicity_forwards;
    TH1D* h1_implantbeta_candidate_multiplicity_backwards;
    TH2D* h2_decayenergy_candidatemultiplicity_forwards;
    TH2D* h2_decayenergy_candidatemultiplicity_backwards;
    TH2D* h2_implantenergy_candidatemultiplicity_forwards;
    TH2D* h2_implantenergy_candidatemultiplicity_backwards;
    TH2D* h2_clustersize_candidatemultiplicity_forwards;
    TH2D* h2_clustersize_candidatemultiplicity_backwards;

    // Histograms for betaGamma correlations
    TH1D* h1_gamma_spectrum;
    TH1D* h1_implantbetagamma_spectrum_before_ionbeta;
    TH1D* h1_implantbetagamma_spectrum_before_ionbeta_dt;
    TH2D* h2_implantbetagamma_spectrum_before_ionbeta_dt_energy;
    TH2D* h2_betagamma_spectrum_before_ionbeta_decay_exy;
    TH2D* h2_beta_decay_exy;

    // Histograms for implant-beta-gamma backward correlated events
    TH1D* h1_gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch;
    TH2D* h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch;
    TH1D* h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch;
    // Histograms for implant-beta-gamma firward correlated events
    TH1D* h1_gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch;
    TH2D* h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch;
    TH1D* h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch;
    TH2D* h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch;

    // Histograms for implant-beta-gamma backward correlated events
    TH1D* h1_allimplantbetagamma_spectrum_after_ionbeta_backwardmatch;
    TH2D* h2_allimplantbetagamma_energy_vs_decaystrip_backwardmatch;
    TH1D* h1_allimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch;
    // Histograms for implant-beta-gamma firward correlated events
    TH1D* h1_allimplantbetagamma_spectrum_after_ionbeta_forwardmatch;
    TH2D* h2_allimplantbetagamma_energy_vs_decaystrip_forwardmatch;
    TH1D* h1_allimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch;
    TH2D* h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch;

    // Histograms for forward backwards matche
    TH2D* h2_gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt;
    TH2D* h2_gatedimplantbetagamma_gammaenergy_vs_implantbeta_dt;
    TH2D* h2_allimplantbetagamma_betagamma_dt_vs_implantbeta_dt;
    TH2D* h2_allimplantbetagamma_gammaenergy_vs_implantbeta_dt;

    // Correlate forward implant-decay dt with other observables
    TH2D* h2_implant_energy_dt_forward;
    TH2D* h2_decay_energy_dt_forward;
    TH2D* h2_strip_dt_forward;

    // Correlate backward implant-decay dt with other observables
    TH2D* h2_implant_energy_dt_backward;
    TH2D* h2_decay_energy_dt_backward;
    TH2D* h2_strip_dt_backward;

    // NTuple for plotting in python
    TNtuple* nt_aida_implant_beta_dt;
    TNtuple* nt_all_candidate_ionbeta_dt;

    // WR times of decays vetoed by implant deadtime
    TH1D* h1_deadtime_implant_vetoed_decay_candidates_time;
    TH1D* h1_interrupted_implant_vetoed_decay_candidates_time;

    // Define plots doen with a canddate beta plot
    TH1D* h1_aida_implant_beta_dt_candidate_cut;
    TH2D* h2_implant_energy_dt_candidate_cut;
    TH2D* h2_decay_energy_dt_candidate_cut;
    TH2D* h2_strip_dt_candidate_cut;
    TH1D* h1_clustersize_candidate_cut;

    // Define plots for all candidate implant decay dt
    TH1D* h1_all_candidate_ionbeta_dt;

    // Correlate low multiplicity implant-decay dt with other observables
    TH2D* h2_hitpattern_low_multiplicity;
    TH2D* h2_implant_energy_dt_low_multiplicity;
    TH2D* h2_decay_energy_dt_low_multiplicity;
    TH2D* h2_strip_dt_low_multiplicity;
    TH2D* h2_impstrip_dt_low_multiplicity;
    TH1D* h1_candidate_wr_time_low_multiplicity;
    TH1D* h1_candidate_ionbeta_dt_low_multiplicity;
    TH1D* h1_clustersize_low_multiplicity;

    // Correlate high multiplicity implant-decay dt with other observables
    TH2D* h2_hitpattern_high_multiplicity;
    TH2D* h2_implant_energy_dt_high_multiplicity;
    TH2D* h2_decay_energy_dt_high_multiplicity;
    TH2D* h2_strip_dt_high_multiplicity;
    TH2D* h2_impstrip_dt_high_multiplicity;
    TH1D* h1_candidate_wr_time_high_multiplicity;
    TH1D* h1_candidate_ionbeta_dt_high_multiplicity;
    TH1D* h1_clustersize_high_multiplicity;

    // Diagnostic histograms for ionbetagamma mathces to known gammas
    TH1D* h1_aida_implant_beta_dt_knowngamma_match;
    TH2D* h2_implant_energy_decay_energy_knowngamma_match;
    TH2D* h2_implant_energy_dt_knowngamma_match;
    TH2D* h2_decay_energy_dt_knowngamma_match;
    TH2D* h2_strip_dt_knowngamma_match;
    TH2D* h2_decayenergy_candidatemultiplicity_forwards_knowngamma_match;
    TH2D* h2_implantenergy_candidatemultiplicity_forwards_knowngamma_match;
    TH2D* h2_clustersize_candidatemultiplicity_forwards_knowngamma_match;
    TH1D* h1_clustersize_forwards_knowngamma_match;

    // Diagnostic histograms for ionbetagamma mathces to unknown gammas
    TH1D* h1_aida_implant_beta_dt_unknowngamma_match;
    TH2D* h2_implant_energy_decay_energy_unknowngamma_match;
    TH2D* h2_implant_energy_dt_unknowngamma_match;
    TH2D* h2_decay_energy_dt_unknowngamma_match;
    TH2D* h2_strip_dt_unknowngamma_match;
    TH2D* h2_decayenergy_candidatemultiplicity_forwards_unknowngamma_match;
    TH2D* h2_implantenergy_candidatemultiplicity_forwards_unknowngamma_match;
    TH2D* h2_clustersize_candidatemultiplicity_forwards_unknowngamma_match;
    TH1D* h1_clustersize_forwards_unknowngamma_match;

    HistogramManager(TFile* outputFile);
    ~HistogramManager();    

    void WriteHistograms();

};

#endif