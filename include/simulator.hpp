#pragma once

#include "network.hpp"

#include <fstream>
#include <string>
#include <vector>

struct TraceSample {
  double t = 0.0;
  double V_a = 0.0;
  double V_b = 0.0;
  double m_a = 0.0;
  double h_a = 0.0;
  double n_a = 0.0;
  double I_Na_a = 0.0;
  double I_K_a = 0.0;
  double I_L_a = 0.0;
  double I_syn_b = 0.0;
  double g_syn = 0.0;
};

class Simulator {
 public:
  Simulator(Network& network, double dt_ms = 0.01);

  void run(double duration_ms,
           std::size_t neuron_a = 0,
           std::size_t neuron_b = 1,
           std::size_t synapse_idx = 0);

  const std::vector<TraceSample>& trace() const { return trace_; }

  void write_csv(const std::string& path) const;

 private:
  Network& network_;
  double dt_ms_;
  std::vector<TraceSample> trace_;
};
