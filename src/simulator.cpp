#include "simulator.hpp"

#include <iomanip>
#include <stdexcept>

Simulator::Simulator(Network& network, double dt_ms)
    : network_(network), dt_ms_(dt_ms) {}

void Simulator::run(double duration_ms,
                    std::size_t neuron_a,
                    std::size_t neuron_b,
                    std::size_t synapse_idx) {
  if (neuron_a >= network_.neuron_count() ||
      neuron_b >= network_.neuron_count()) {
    throw std::out_of_range("trace neuron id out of range");
  }
  if (synapse_idx >= network_.synapse_count()) {
    throw std::out_of_range("trace synapse index out of range");
  }

  trace_.clear();
  const std::size_t steps =
      static_cast<std::size_t>(duration_ms / dt_ms_ + 0.5);
  trace_.reserve(steps + 1);

  for (std::size_t i = 0; i <= steps; ++i) {
    const double t = static_cast<double>(i) * dt_ms_;
    const NeuronState a = network_.neuron(neuron_a).snapshot();
    const NeuronState b = network_.neuron(neuron_b).snapshot();
    const Synapse& syn = network_.synapse(synapse_idx);

    TraceSample sample;
    sample.t = t;
    sample.V_a = a.V;
    sample.V_b = b.V;
    sample.m_a = a.m;
    sample.h_a = a.h;
    sample.n_a = a.n;
    sample.I_Na_a = a.I_Na;
    sample.I_K_a = a.I_K;
    sample.I_L_a = a.I_L;
    sample.I_syn_b = b.I_syn;
    sample.g_syn = syn.g();
    trace_.push_back(sample);

    if (i < steps) {
      network_.step(dt_ms_);
    }
  }
}

void Simulator::write_csv(const std::string& path) const {
  std::ofstream out(path);
  if (!out) {
    throw std::runtime_error("failed to open CSV: " + path);
  }

  out << "t,V_a,V_b,m,h,n,I_Na,I_K,I_L,I_syn_b,g_syn\n";
  out << std::setprecision(10);
  for (const auto& s : trace_) {
    out << s.t << ',' << s.V_a << ',' << s.V_b << ',' << s.m_a << ',' << s.h_a
        << ',' << s.n_a << ',' << s.I_Na_a << ',' << s.I_K_a << ',' << s.I_L_a
        << ',' << s.I_syn_b << ',' << s.g_syn << '\n';
  }
}
