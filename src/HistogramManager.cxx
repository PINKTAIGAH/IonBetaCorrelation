#include "Logger.hh"
#include "HistogramManager.hh"

// Constructor
HistogramManager::HistogramManager(TFile* outputFile){

  this->outputFile = outputFile;

  InitialiseHistograms();

}

HistogramManager::~HistogramManager(){

  delete h2_aida_implant_xy;
  delete h2_aida_matched_xy;
  delete h1_aida_wr_times;
  delete h1_onspill_times;
  delete h1_aida_implant_beta_dt;
  delete h1_aida_implant_beta_dt_forward;
  delete h1_aida_implant_beta_dt_backward;
  delete h1_aida_implant_beta_secondmatch_dt;
  delete h1_aida_implant_beta_thirdmatch_dt;
  delete h1_aida_implant_beta_tenthmatch_dt;
  delete h1_implantbeta_candidate_multiplicity_forwards;
  delete h1_implantbeta_candidate_multiplicity_backwards;
  delete h2_decayenergy_candidatemultiplicity_forwards;
  delete h2_decayenergy_candidatemultiplicity_backwards;
  delete h2_implantenergy_candidatemultiplicity_forwards;
  delete h2_implantenergy_candidatemultiplicity_backwards;
  delete h2_clustersize_candidatemultiplicity_forwards;
  delete h2_clustersize_candidatemultiplicity_backwards;
  delete h1_gamma_spectrum;
  delete h1_implantbetagamma_spectrum_before_ionbeta;
  delete h1_implantbetagamma_spectrum_before_ionbeta_dt;
  delete h2_implantbetagamma_spectrum_before_ionbeta_dt_energy;
  delete h1_gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch;
  delete h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch;
  delete h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch;
  delete h1_gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch;
  delete h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch;
  delete h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch;
  delete h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch;
  delete h1_allimplantbetagamma_spectrum_after_ionbeta_backwardmatch;
  delete h2_allimplantbetagamma_energy_vs_decaystrip_backwardmatch;
  delete h1_allimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch;
  delete h1_allimplantbetagamma_spectrum_after_ionbeta_forwardmatch;
  delete h2_allimplantbetagamma_energy_vs_decaystrip_forwardmatch;
  delete h1_allimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch;
  delete h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch;
  delete h2_gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt;
  delete h2_gatedimplantbetagamma_gammaenergy_vs_implantbeta_dt;
  delete h2_allimplantbetagamma_betagamma_dt_vs_implantbeta_dt;
  delete h2_allimplantbetagamma_gammaenergy_vs_implantbeta_dt;
  delete h2_implant_energy_dt_forward;
  delete h2_decay_energy_dt_forward;
  delete h2_strip_dt_forward;
  delete h2_implant_energy_dt_backward;
  delete h2_decay_energy_dt_backward;
  delete h2_strip_dt_backward;
  delete nt_aida_implant_beta_dt;
  delete nt_all_candidate_ionbeta_dt;
  delete h1_deadtime_implant_vetoed_decay_candidates_time;
  delete h1_interrupted_implant_vetoed_decay_candidates_time;
  delete h1_aida_implant_beta_dt_candidate_cut;
  delete h2_implant_energy_dt_candidate_cut;
  delete h2_decay_energy_dt_candidate_cut;
  delete h2_strip_dt_candidate_cut;
  delete h1_clustersize_candidate_cut;
  delete h1_all_candidate_ionbeta_dt;
  delete h2_hitpattern_low_multiplicity;
  delete h2_implant_energy_dt_low_multiplicity;
  delete h2_decay_energy_dt_low_multiplicity;
  delete h2_strip_dt_low_multiplicity;
  delete h2_impstrip_dt_low_multiplicity;
  delete h1_candidate_wr_time_low_multiplicity;
  delete h1_candidate_ionbeta_dt_low_multiplicity;
  delete h1_clustersize_low_multiplicity;
  delete h2_hitpattern_high_multiplicity;
  delete h2_implant_energy_dt_high_multiplicity;
  delete h2_decay_energy_dt_high_multiplicity;
  delete h2_strip_dt_high_multiplicity;
  delete h2_impstrip_dt_high_multiplicity;
  delete h1_candidate_wr_time_high_multiplicity;
  delete h1_candidate_ionbeta_dt_high_multiplicity;
  delete h1_clustersize_high_multiplicity;
  delete h1_aida_implant_beta_dt_knowngamma_match;
  delete h2_implant_energy_decay_energy_knowngamma_match;
  delete h2_implant_energy_dt_knowngamma_match;
  delete h2_decay_energy_dt_knowngamma_match;
  delete h2_strip_dt_knowngamma_match;
  delete h2_decayenergy_candidatemultiplicity_forwards_knowngamma_match;
  delete h2_implantenergy_candidatemultiplicity_forwards_knowngamma_match;
  delete h2_clustersize_candidatemultiplicity_forwards_knowngamma_match;
  delete h1_clustersize_forwards_knowngamma_match;
  delete h1_aida_implant_beta_dt_unknowngamma_match;
  delete h2_implant_energy_decay_energy_unknowngamma_match;
  delete h2_implant_energy_dt_unknowngamma_match;
  delete h2_decay_energy_dt_unknowngamma_match;
  delete h2_strip_dt_unknowngamma_match;
  delete h2_decayenergy_candidatemultiplicity_forwards_unknowngamma_match;
  delete h2_implantenergy_candidatemultiplicity_forwards_unknowngamma_match;
  delete h2_clustersize_candidatemultiplicity_forwards_unknowngamma_match;
  delete h1_clustersize_forwards_unknowngamma_match;
  delete h2_betagamma_spectrum_before_ionbeta_decay_exy;
  delete h2_beta_decay_exy;

}

// Private
void HistogramManager::InitialiseHistograms(){

  // Histograms for implant beta matches
  h2_aida_implant_xy = new TH2D("aida_implant_xy", "AIDA Implant XY", 384, 0, 384, 128, 0, 128);
  h2_aida_matched_xy = new TH2D("aida_matched_xy", "AIDA Matched XY", 384, 0, 384, 128, 0, 128);
  h1_aida_wr_times = new TH1D("aida_wr_times", "AIDA WR Times", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);
  h1_onspill_times = new TH1D("aida_onspill_times", "Onspill WR Times", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);
  h1_aida_implant_beta_dt = new TH1D("aida_implant_beta_firstmatch_dt", "Implant-Decay #Deltat (First Match);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h1_aida_implant_beta_dt_forward = new TH1D("aida_implant_beta_firstmatch_dt_forward", "Implant-Decay #Deltat (First Match-Forward);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins / 2, 0, Global::timeThreshold);
  h1_aida_implant_beta_dt_backward = new TH1D("aida_implant_beta_firstmatch_dt_backward", "Implant-Decay #Deltat (First Match-Backward);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins / 2, 0, Global::timeThreshold);
  h1_aida_implant_beta_secondmatch_dt = new TH1D("aida_implant_beta_secondmatch_dt", "Implant-Decay #Deltat (Second Match);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h1_aida_implant_beta_thirdmatch_dt = new TH1D("aida_implant_beta_thirdmatch_dt", "Implant-Decay #Deltat (Third Match);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h1_aida_implant_beta_tenthmatch_dt = new TH1D("aida_implant_beta_tenthmatch_dt", "Implant-Decay #Deltat (Tenth Match);Implant-Decay #Deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);

  // Histograms for beta candidate events
  h1_implantbeta_candidate_multiplicity_forwards = new TH1D("implantbeta_candidate_multiplicity_forwards", "Implant-Decay Forwards Candidate Multiplicity; Candidate Multiplicity; Counts", 100, 0, 100);
  h1_implantbeta_candidate_multiplicity_backwards = new TH1D("implantbeta_candidate_multiplicity_backwards", "Implant-Decay Backwards Candidate Multiplicity; Candidate Multiplicity; Counts", 100, 0, 100);
  h2_decayenergy_candidatemultiplicity_forwards = new TH2D("decayenergy_candidatemultiplicity_forwards", "Decay Energy vs Candidate Multiplicity (Forwards); Decay Energy (keV);Candidate Multiplicity", 1500 / 10, 0, 1500, 100, 0, 100);
  h2_decayenergy_candidatemultiplicity_backwards = new TH2D("decayenergy_candidatemultiplicity_backwards", "Decay Energy vs Candidate Multiplicity (Backwards); Decay Energy (keV);Candidate Multiplicity", 1500 / 10, 0, 1500, 100, 0, 100);
  h2_implantenergy_candidatemultiplicity_forwards = new TH2D("implantenergy_candidatemultiplicity_forwards", "Implant Energy vs Candidate Multiplicity (Forwards); Implant Energy (keV);Candidate Multiplicity", 7000 / 20, 0, 7000, 100, 0, 100);
  h2_implantenergy_candidatemultiplicity_backwards = new TH2D("implantenergy_candidatemultiplicity_backwards", "Implant Energy vs Candidate Multiplicity (Backwards); Implant Energy (keV);Candidate Multiplicity", 7000 / 20, 0, 7000, 100, 0, 100);
  h2_clustersize_candidatemultiplicity_forwards = new TH2D("clustersize_candidatemultiplicity_forwards", "Cluster Size vs Forwards Candidate Multiplicity; Cluster Size; Multiplicity", 50, 0, 10, 100, 0, 100);
  h2_clustersize_candidatemultiplicity_backwards = new TH2D("clustersize_candidatemultiplicity_backwards", "Cluster Size vs Backwards Candidate Multiplicity; Cluster Size; Multiplicity", 50, 0, 10, 100, 0, 100);

  // Histograms for betaGamma correlations
  h1_gamma_spectrum = new TH1D("gamma_spectrum", "Gamma Energy Spectrum ; Energy (keV); Counts/keV", 2000, 0, 2000);
  h1_implantbetagamma_spectrum_before_ionbeta = new TH1D("implantbetagamma_spectrum_before_ionbeta", "Beta-Gamma Energy Spectrum (all); Energy (keV); Counts/keV", 2000, 0, 2000);
  h1_implantbetagamma_spectrum_before_ionbeta_dt = new TH1D("implantbetagamma_spectrum_dt", "Decay Germanium dt; Time (ns); Counts", 2000, -30000, 2000);
  h2_implantbetagamma_spectrum_before_ionbeta_dt_energy = new TH2D("h2_implantbetagamma_spectrum_before_ionbeta_dt_energy", "Time between beta decay and gamma ray vs energy (keV); Time (ns); Energy (keV); Counts", 500, -20e3, 2e3, 2000, 0, 2000);
  h2_beta_decay_exy = new TH2D("h2_beta_decay_exy", "DecayGamma Energy xy", 1000/20, 0, 1000, 1000/20, 0, 1000);
  h2_betagamma_spectrum_before_ionbeta_decay_exy = new TH2D("h2_betagamma_spectrum_before_ionbeta_decay_exy", "DecayGamma Energy xy", 1000/20, 0, 1000, 1000/20, 0, 1000);

  // Histograms for implant-beta-gamma backward correlated events
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch = new TH1D("gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch", "Implant-Beta-Gamma Energy Spectrum (backward ionbeta matched)); Energy (keV); Counts/keV", 2000, 0, 2000);
  h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch = new TH2D("gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch", "Matched Gamma Energy vs Decay Strip (X then Y);Strip;Energy (keV)", 528, 0, 528, 2000, 0, 2000);
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch = new TH1D("gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch", "Time between beta decay and gamma ray backward match; Time (ns); Counts", 2000, -30000, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch", "Time between beta decay and gamma ray backward Match vs energy (keV); Time (ns); Energy (keV); Counts", 500, -20e3, 2e3, 2000, 0, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch", "gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch", 2000, 0, 2000, 2000, 0, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch", "gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch", 2000, 0, 2000, 200, -100, 100);
  // Histograms for implant-beta-gamma firward correlated events
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch = new TH1D("gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch", "Implant-Beta-Gamma Energy Spectrum (forward ionbeta matched)); Energy (keV); Counts/keV", 2000, 0, 2000);
  h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch = new TH2D("gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch", "Matched Gamma Energy vs Decay Strip (X then Y);Strip;Energy (keV)", 528, 0, 528, 2000, 0, 2000);
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch = new TH1D("gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch", "Time between beta decay and gamma ray forward match; Time (ns); Counts", 2000, -20000, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch", "Time between beta decay and gamma ray Forward Match vs energy (keV); Time (ns); Energy (keV); Counts", 500, -20e3, 2e3, 2000, 0, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch", "gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch", 2000, 0, 2000, 2000, 0, 2000);
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch = new TH2D("gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch", "gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch", 2000, 0, 2000, 200, -100, 100);

  // Histograms for implant-beta-gamma backward correlated events
  h1_allimplantbetagamma_spectrum_after_ionbeta_backwardmatch = new TH1D("allimplantbetagamma_spectrum_after_ionbeta_backwardmatch", "Implant-Beta-Gamma Energy Spectrum (backward ionbeta matched)); Energy (keV); Counts/keV", 2000, 0, 2000);
  h2_allimplantbetagamma_energy_vs_decaystrip_backwardmatch = new TH2D("allimplantbetagamma_energy_vs_decaystrip_backwardmatch", "Matched Gamma Energy vs Decay Strip (X then Y);Strip;Energy (keV)", 528, 0, 528, 2000, 0, 2000);
  h1_allimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch = new TH1D("allimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch", "Time between beta decay and gamma ray backward match; Time (ns); Counts", 2000, -20000, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch", "Time between beta decay and gamma ray backward Match vs energy (keV); Time (ns); Energy (keV); Counts", 500, -20e3, 2e3, 2000, 0, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch", "allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch", 2000, 0, 2000, 2000, 0, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch", "allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch", 2000, 0, 2000, 200, -100, 100);
  // Histograms for implant-beta-gamma firward correlated events
  h1_allimplantbetagamma_spectrum_after_ionbeta_forwardmatch = new TH1D("allimplantbetagamma_spectrum_after_ionbeta_forwardmatch", "Implant-Beta-Gamma Energy Spectrum (forward ionbeta matched)); Energy (keV); Counts/keV", 2000, 0, 2000);
  h2_allimplantbetagamma_energy_vs_decaystrip_forwardmatch = new TH2D("allimplantbetagamma_energy_vs_decaystrip_forwardmatch", "Matched Gamma Energy vs Decay Strip (X then Y);Strip;Energy (keV)", 528, 0, 528, 2000, 0, 2000);
  h1_allimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch = new TH1D("allimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch", "Time between beta decay and gamma ray forward match; Time (ns); Counts", 2000, -20000, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch", "Time between beta decay and gamma ray Forward Match vs energy (keV); Time (ns); Energy (keV); Counts", 500, -20e3, 2e3, 2000, 0, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch", "allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch", 2000, 0, 2000, 2000, 0, 2000);
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch = new TH2D("allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch", "allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch", 2000, 0, 2000, 200, -100, 100);

  // Histograms for forward backwards matche
  h2_gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt = new TH2D("gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt", "Implant-Beta-Gamma ImplantBeta dt vs BetaGamma dt;betagamma dt (ns); ionbeta dt(ns)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold, 1000, -20, 2e3);
  h2_gatedimplantbetagamma_gammaenergy_vs_implantbeta_dt = new TH2D("gatedimplantbetagamma_gammaenergy_dt_vs_implantbeta_dt", "Implant-Beta-Gamma ImplantBeta dt vs Gamma energy;betagamma dt (ns); Energy (keV)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold, 2000, 0, 2000);
  h2_allimplantbetagamma_betagamma_dt_vs_implantbeta_dt = new TH2D("allimplantbetagamma_betagamma_dt_vs_implantbeta_dt", "Implant-Beta-Gamma ImplantBeta dt vs BetaGamma dt;betagamma dt (ns); ionbeta dt(ns)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold, 1000, -20, 2e3);
  h2_allimplantbetagamma_gammaenergy_vs_implantbeta_dt = new TH2D("allimplantbetagamma_gammaenergy_dt_vs_implantbeta_dt", "Implant-Beta-Gamma ImplantBeta dt vs Gamma energy;betagamma dt (ns); Energy (keV)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold, 2000, 0, 2000);

  // Correlate forward implant-decay dt with other observables
  h2_implant_energy_dt_forward = new TH2D("implant_energy_dt_forward", "Implant Energy vs Implant-Decay dt", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_forward = new TH2D("decay_energy_dt_forward", "Decay Energy vs Implant-Decay dt (Forward)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_forward = new TH2D("strip_dt_forward", "Strip XY vs Implant-Decay dt (Forward)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);

  // Correlate backward implant-decay dt with other observables
  h2_implant_energy_dt_backward = new TH2D("implant_energy_dt_backward", "Implant Energy vs Implant-Decay dt (Backward)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, -Global::timeThreshold, 0);
  h2_decay_energy_dt_backward = new TH2D("decay_energy_dt_backward", "Decay Energy vs Implant-Decay dt (Backward)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, -Global::timeThreshold, 0);
  h2_strip_dt_backward = new TH2D("strip_dt_backward", "Strip XY vs Implant-Decay dt (Backward)", 528, 0, 528, HistogramConstants::implantDecayBins, -Global::timeThreshold, 0);

  // NTuple for plotting in python
  nt_aida_implant_beta_dt = new TNtuple("nt_aida_implant_beta_dt", "Implant Decay dt", "dt");
  nt_all_candidate_ionbeta_dt = new TNtuple("nt_all_candidate_ionbeta_dt", "All beta candidate implant decay dt", "dt");

  // WR times of decays vetoed by implant deadtime
  h1_deadtime_implant_vetoed_decay_candidates_time = new TH1D("deadtime_implant_vetoed_decay_candidates_time", "AIDA WR times of vetoed decay candidates (Implant deadtime)", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);
  h1_interrupted_implant_vetoed_decay_candidates_time = new TH1D("interrupted_implant_vetoed_decay_candidates_time", "AIDA WR times of vetoed decay candidates (Implant interruption)", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);

  // Define plots doen with a canddate beta plot
  h1_aida_implant_beta_dt_candidate_cut = new TH1D("aida_implant_beta_dt_candidate_cut", "Implant-Decay #Deltat (Beta Candidate Cut);Implant-Decay #deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h2_implant_energy_dt_candidate_cut = new TH2D("implant_energy_dt_candidate_cut", "Implant Energy vs Implant-Decay dt (Beta Candidate Cut)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_candidate_cut = new TH2D("decay_energy_dt_candidate_cut", "Decay Energy vs Implant-Decay dt (Beta Candidate Cut)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_candidate_cut = new TH2D("strip_dt_candidate_cut", "Strip XY vs Implant-Decay dt (Beta Candidate Cut)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h1_clustersize_candidate_cut = new TH1D("clustersize_candidate_cut", "Decay Cluster Size (Beta Candidate Cut)", 50, 0, 10);

  // Define plots for all candidate implant decay dt
  h1_all_candidate_ionbeta_dt = new TH1D("all_candidate_ionbeta_dt", "Implant-Decay dt (All candidates)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);

  // Correlate low multiplicity implant-decay dt with other observables
  h2_hitpattern_low_multiplicity = new TH2D("hitpattern_low_multiplicity", "AIDA Beta Candidate Hitpattern (Beta Multiplicity < 4)", 384, 0, 384, 128, 0, 128);
  h2_implant_energy_dt_low_multiplicity = new TH2D("implant_energy_dt_low_multiplicity", "Implant Energy vs Implant-Decay dt (Beta Multiplicity < 4)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_low_multiplicity = new TH2D("decay_energy_dt_low_multiplicity", "Decay Energy vs Implant-Decay dt (Beta Multiplicity < 4)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_low_multiplicity = new TH2D("strip_dt_low_multiplicity", "Strip XY vs Implant-Decay dt (Beta Multiplicity < 4)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_impstrip_dt_low_multiplicity = new TH2D("impstrip_dt_low_multiplicity", "HEC Strip XY vs Implant-Decay dt (Beta Multiplicity < 4)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h1_candidate_wr_time_low_multiplicity = new TH1D("candidate_wr_time_low_multiplicity", "AIDA Candidate WR Times (Beta Multiplicity < 4)", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);
  h1_candidate_ionbeta_dt_low_multiplicity = new TH1D("candidate_ionbeta_dt_low_multiplicity", "Candidate Implant-Decay dt (Beta Multiplicity < 4)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h1_clustersize_low_multiplicity = new TH1D("clustersize_low_multiplicity", "Decay Cluster Size (Beta Multiplicity < 4)", 50, 0, 10);

  // Correlate high multiplicity implant-decay dt with other observables
  h2_hitpattern_high_multiplicity = new TH2D("hitpattern_high_multiplicity", "AIDA Beta Candidate Hitpattern (Beta Multiplicity > 20)", 384, 0, 384, 128, 0, 128);
  h2_implant_energy_dt_high_multiplicity = new TH2D("implant_energy_dt_high_multiplicity", "Implant Energy vs Implant-Decay dt (Beta Multiplicity > 20)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_high_multiplicity = new TH2D("decay_energy_dt_high_multiplicity", "Decay Energy vs Implant-Decay dt (Beta Multiplicity > 20)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_high_multiplicity = new TH2D("strip_dt_high_multiplicity", "Strip XY vs Implant-Decay dt (Beta Multiplicity > 20)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_impstrip_dt_high_multiplicity = new TH2D("impstrip_dt_high_multiplicity", "HEC Strip XY vs Implant-Decay dt (Beta Multiplicity > 20)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h1_candidate_wr_time_high_multiplicity = new TH1D("candidate_wr_time_high_multiplicity", "AIDA Candidate WR Times (Beta Multiplicity > 20)", HistogramConstants::numberOfSlices, HistogramConstants::wrExperimentStart, HistogramConstants::wrExperimentEnd);
  h1_candidate_ionbeta_dt_high_multiplicity = new TH1D("candidate_ionbeta_dt_high_multiplicity", "Candidate Implant-Decay dt (Beta Multiplicity > 20)", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h1_clustersize_high_multiplicity = new TH1D("clustersize_high_multiplicity", "Decay Cluster Size (Beta Multiplicity > 20)", 50, 0, 10);

  // Diagnostic histograms for ionbetagamma mathces to known gammas
  h1_aida_implant_beta_dt_knowngamma_match = new TH1D("aida_implant_beta_dt_knowngamma_match", "Implant-Decay #Deltat (Known Gamma Match);Implant-Decay #deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h2_implant_energy_decay_energy_knowngamma_match = new TH2D("implant_energy_decay_energy_knowngamma_match", "Implant Energy vs Decay Energy (Known Gamma Match);Decay Energy (keV);Implant Energy (MeV)", 1500 / 10, 0, 1500, 7000 / 20, 0, 7000);
  h2_implant_energy_dt_knowngamma_match = new TH2D("implant_energy_dt_knowngamma_match", "Implant Energy vs Implant-Decay dt (Known Gamma Match)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_knowngamma_match = new TH2D("decay_energy_dt_knowngamma_match", "Decay Energy vs Implant-Decay dt (Known Gamma Match)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_knowngamma_match = new TH2D("strip_dt_knowngamma_match", "Strip XY vs Implant-Decay dt (Known Gamma Match)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decayenergy_candidatemultiplicity_forwards_knowngamma_match = new TH2D("decayenergy_candidatemultiplicity_forwards_knowngamma_match", "Decay Energy vs Candidate Multiplicity (Forwards knowngammamatch); Decay Energy (keV);Candidate Multiplicity", 1500 / 10, 0, 1500, 100, 0, 100);
  h2_implantenergy_candidatemultiplicity_forwards_knowngamma_match = new TH2D("implantenergy_candidatemultiplicity_forwards_knowngamma_match", "Implant Energy vs Candidate Multiplicity (Forwards knowngamma match); Implant Energy (keV);Candidate Multiplicity", 7000 / 20, 0, 7000, 100, 0, 100);
  h2_clustersize_candidatemultiplicity_forwards_knowngamma_match = new TH2D("clustersize_candidatemultiplicity_forwards_knowngamma_match", "Cluster Size vs Forwards knowngamma match Candidate Multiplicity; Cluster Size; Multiplicity", 50, 0, 10, 100, 0, 100);
  h1_clustersize_forwards_knowngamma_match = new TH1D("clustersize_forwards_knowngamma_match", "Forwards Decay Cluster Size (Known Gamma Match)", 50, 0, 10);

  // Diagnostic histograms for ionbetagamma mathces to unknown gammas
  h1_aida_implant_beta_dt_unknowngamma_match = new TH1D("aida_implant_beta_dt_unknowngamma_match", "Implant-Decay #Deltat (Unknown Gamma Match);Implant-Decay #deltat (); Counts/", HistogramConstants::implantDecayBins, -Global::timeThreshold, Global::timeThreshold);
  h2_implant_energy_decay_energy_unknowngamma_match = new TH2D("implant_energy_decay_energy_unknowngamma_match", "Implant Energy vs Decay Energy (Unknown Gamma Match);Decay Energy (keV);Implant Energy (MeV)", 1500 / 10, 0, 1500, 7000 / 20, 0, 7000);
  h2_implant_energy_dt_unknowngamma_match = new TH2D("implant_energy_dt_unknowngamma_match", "Implant Energy vs Implant-Decay dt (Unknown Gamma Match)", 7000 / 20, 0, 7000, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decay_energy_dt_unknowngamma_match = new TH2D("decay_energy_dt_unknowngamma_match", "Decay Energy vs Implant-Decay dt (Unknown Gamma Match)", 1500 / 10, 0, 1500, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_strip_dt_unknowngamma_match = new TH2D("strip_dt_unknowngamma_match", "Strip XY vs Implant-Decay dt (Unknown Gamma Match)", 528, 0, 528, HistogramConstants::implantDecayBins, 0, Global::timeThreshold);
  h2_decayenergy_candidatemultiplicity_forwards_unknowngamma_match = new TH2D("decayenergy_candidatemultiplicity_forwards_unknowngamma_match", "Decay Energy vs Candidate Multiplicity (Forwards unknowngammamatch); Decay Energy (keV);Candidate Multiplicity", 1500 / 10, 0, 1500, 100, 0, 100);
  h2_implantenergy_candidatemultiplicity_forwards_unknowngamma_match = new TH2D("implantenergy_candidatemultiplicity_forwards_unknowngamma_match", "Implant Energy vs Candidate Multiplicity (Forwards unknowngamma match); Implant Energy (keV);Candidate Multiplicity", 7000 / 20, 0, 7000, 100, 0, 100);
  h2_clustersize_candidatemultiplicity_forwards_unknowngamma_match = new TH2D("clustersize_candidatemultiplicity_forwards_unknowngamma_match", "Cluster Size vs Forwards unknowngamma match Candidate Multiplicity; Cluster Size; Multiplicity", 50, 0, 10, 100, 0, 100);
  h1_clustersize_forwards_unknowngamma_match = new TH1D("clustersize_forwards_unknowngamma_match", "Forwards Decay Cluster Size (Unknown Gamma Match)", 50, 0, 10);

}

// Public

void HistogramManager::WriteHistograms(){

  Logger::Log("Writitng histograms...");

  outputFile->cd();

  TDirectory *candidateDir = outputFile->mkdir("candidate_matches");
  candidateDir->cd();
  h1_all_candidate_ionbeta_dt->Write();
  h1_implantbeta_candidate_multiplicity_forwards->Write();
  h1_implantbeta_candidate_multiplicity_backwards->Write();
  h2_decayenergy_candidatemultiplicity_forwards->Write();
  h2_implantenergy_candidatemultiplicity_forwards->Write();
  h2_decayenergy_candidatemultiplicity_backwards->Write();
  h2_implantenergy_candidatemultiplicity_backwards->Write();
  h2_clustersize_candidatemultiplicity_forwards->Write();
  h2_clustersize_candidatemultiplicity_backwards->Write();
  gFile->cd();

  TDirectory *forwardDir = outputFile->mkdir("forward_matches");
  forwardDir->cd();
  h2_implant_energy_dt_forward->Write();
  h2_decay_energy_dt_forward->Write();
  h2_strip_dt_forward->Write();
  h2_strip_dt_forward->Write();
  gFile->cd();

  TDirectory *backwardDir = outputFile->mkdir("backward_matches");
  backwardDir->cd();
  h2_implant_energy_dt_backward->Write();
  h2_decay_energy_dt_backward->Write();
  h2_strip_dt_backward->Write();
  h2_strip_dt_backward->Write();
  gFile->cd();

  TDirectory *candidateCutDir = outputFile->mkdir("candidate_cut_matches");
  candidateCutDir->cd();
  h1_aida_implant_beta_dt_candidate_cut->Write();
  h2_implant_energy_dt_candidate_cut->Write();
  h2_decay_energy_dt_candidate_cut->Write();
  h2_strip_dt_candidate_cut->Write();
  h2_strip_dt_candidate_cut->Write();
  h1_clustersize_candidate_cut->Write();
  gFile->cd();

  TDirectory *lowCandidateMultiplicity = outputFile->mkdir("low_candidate_multiplicity");
  lowCandidateMultiplicity->cd();
  h1_candidate_wr_time_low_multiplicity->Write();
  h1_candidate_ionbeta_dt_low_multiplicity->Write();
  h2_hitpattern_low_multiplicity->Write();
  h2_implant_energy_dt_low_multiplicity->Write();
  h2_decay_energy_dt_low_multiplicity->Write();
  h2_strip_dt_low_multiplicity->Write();
  h2_impstrip_dt_low_multiplicity->Write();
  h1_clustersize_low_multiplicity->Write();
  gFile->cd();

  TDirectory *highCandidateMultiplicity = outputFile->mkdir("high_candidate_multiplicity");
  highCandidateMultiplicity->cd();
  h1_candidate_wr_time_high_multiplicity->Write();
  h1_candidate_ionbeta_dt_high_multiplicity->Write();
  h2_hitpattern_high_multiplicity->Write();
  h2_implant_energy_dt_high_multiplicity->Write();
  h2_decay_energy_dt_high_multiplicity->Write();
  h2_strip_dt_high_multiplicity->Write();
  h2_impstrip_dt_high_multiplicity->Write();
  h1_clustersize_high_multiplicity->Write();
  gFile->cd();

  TDirectory *vetoedDecays = outputFile->mkdir("vetoed_decays");
  vetoedDecays->cd();
  h1_deadtime_implant_vetoed_decay_candidates_time->Write();
  h1_interrupted_implant_vetoed_decay_candidates_time->Write();
  gFile->cd();

  TDirectory *ionbetaMatch = outputFile->mkdir("ionbeta_match");
  ionbetaMatch->cd();
  h2_aida_implant_xy->Write();
  h1_aida_wr_times->Write();
  h1_onspill_times->Write();
  h2_aida_matched_xy->Write();
  h1_aida_implant_beta_dt->Write();
  h1_aida_implant_beta_dt_forward->Write();
  h1_aida_implant_beta_dt_backward->Write();
  h1_aida_implant_beta_secondmatch_dt->Write();
  h1_aida_implant_beta_thirdmatch_dt->Write();
  h1_aida_implant_beta_tenthmatch_dt->Write();
  gFile->cd();

  TDirectory *ionbetagammaForwardCoincidences = outputFile->mkdir("ionbetagamma_forward_coincidences");
  ionbetagammaForwardCoincidences->cd();
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch->Write();
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch->Write();
  h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch->Write();
  h1_allimplantbetagamma_spectrum_after_ionbeta_forwardmatch->Write();
  h1_allimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch->Write();
  h2_allimplantbetagamma_energy_vs_decaystrip_forwardmatch->Write();
  gFile->cd();

  TDirectory *ionbetagammaBackwardCoincidences = outputFile->mkdir("ionbetagamma_backward_coincidences");
  ionbetagammaBackwardCoincidences->cd();
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch->Write();
  h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch->Write();
  h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch->Write();
  h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch->Write();
  h1_allimplantbetagamma_spectrum_after_ionbeta_backwardmatch->Write();
  h1_allimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch->Write();
  h2_allimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch->Write();
  h2_allimplantbetagamma_energy_vs_decaystrip_backwardmatch->Write();
  gFile->cd();

  TDirectory *ionbetagammaAllCoincidence = outputFile->mkdir("ionbetagamma_all_coincidence");
  ionbetagammaAllCoincidence->cd();
  h2_gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt->Write();
  h2_gatedimplantbetagamma_gammaenergy_vs_implantbeta_dt->Write();
  h2_allimplantbetagamma_betagamma_dt_vs_implantbeta_dt->Write();
  h2_allimplantbetagamma_gammaenergy_vs_implantbeta_dt->Write();
  gFile->cd();

  TDirectory *betagammaCoincidences = outputFile->mkdir("betagamma_coincidences");
  betagammaCoincidences->cd();
  h1_gamma_spectrum->Write();
  h1_implantbetagamma_spectrum_before_ionbeta->Write();
  h1_implantbetagamma_spectrum_before_ionbeta_dt->Write();
  h2_implantbetagamma_spectrum_before_ionbeta_dt_energy->Write();
  h2_betagamma_spectrum_before_ionbeta_decay_exy->Write();
  h2_beta_decay_exy->Write();
  gFile->cd();

  TDirectory *knownGammaCoincidences = outputFile->mkdir("known_gamma_matches");
  knownGammaCoincidences->cd();
  h1_aida_implant_beta_dt_knowngamma_match->Write();
  h2_implant_energy_decay_energy_knowngamma_match->Write();
  h2_implant_energy_dt_knowngamma_match->Write();
  h2_decay_energy_dt_knowngamma_match->Write();
  h2_strip_dt_knowngamma_match->Write();
  h2_strip_dt_knowngamma_match->Write();
  h2_decayenergy_candidatemultiplicity_forwards_knowngamma_match->Write();
  h2_implantenergy_candidatemultiplicity_forwards_knowngamma_match->Write();
  h2_clustersize_candidatemultiplicity_forwards_knowngamma_match->Write();
  h1_clustersize_forwards_knowngamma_match->Write();
  gFile->cd();

  TDirectory *unknownGammaCoincidences = outputFile->mkdir("unknown_gamma_matches");
  unknownGammaCoincidences->cd();
  h1_aida_implant_beta_dt_unknowngamma_match->Write();
  h2_implant_energy_decay_energy_unknowngamma_match->Write();
  h2_implant_energy_dt_unknowngamma_match->Write();
  h2_decay_energy_dt_unknowngamma_match->Write();
  h2_strip_dt_unknowngamma_match->Write();
  h2_strip_dt_unknowngamma_match->Write();
  h2_decayenergy_candidatemultiplicity_forwards_unknowngamma_match->Write();
  h2_implantenergy_candidatemultiplicity_forwards_unknowngamma_match->Write();
  h2_clustersize_candidatemultiplicity_forwards_unknowngamma_match->Write();
  h1_clustersize_forwards_unknowngamma_match->Write();
  gFile->cd();

  nt_aida_implant_beta_dt->Write();
  nt_all_candidate_ionbeta_dt->Write();

}