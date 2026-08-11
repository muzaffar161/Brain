#pragma once

#include "ion_channel.hpp"

#include <cstddef>
#include <string>

struct NeuronState {
  double V = -65.0;
  double m = 0.0;
  double h = 0.0;
  double n = 0.0;
  double I_Na = 0.0;
  double I_K = 0.0;
  double I_L = 0.0;
  double I_syn = 0.0;
  double I_ext = 0.0;
  bool spiked = false;
};

class Neuron {
 public:
  explicit Neuron(std::string name, double C_m = 1.0, double V0 = -65.0);

  const std::string& name() const { return name_; }
  std::size_t id() const { return id_; }
  void set_id(std::size_t id) { id_ = id; }

  double V() const { return V_; }
  double I_ext() const { return I_ext_; }
  void set_I_ext(double I) { I_ext_ = I; }

  // Synaptic current injected this step (µA/cm²), reset by caller each step.
  void add_I_syn(double I) { I_syn_ += I; }
  void clear_I_syn() { I_syn_ = 0.0; }
  double I_syn() const { return I_syn_; }

  bool spiked() const { return spiked_; }

  const SodiumChannel& sodium() const { return na_; }
  const PotassiumChannel& potassium() const { return k_; }
  const LeakChannel& leak() const { return leak_; }

  NeuronState snapshot() const;

  // Integrate one time step (ms). Spike = upward threshold crossing.
  void step(double dt);

 private:
  std::string name_;
  std::size_t id_ = 0;
  double C_m_;
  double V_;
  double I_ext_ = 0.0;
  double I_syn_ = 0.0;
  bool spiked_ = false;
  bool above_threshold_ = false;

  SodiumChannel na_;
  PotassiumChannel k_;
  LeakChannel leak_;

  static constexpr double kSpikeThreshold = 0.0;  // mV
};
