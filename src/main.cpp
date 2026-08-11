#include "network.hpp"
#include "simulator.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
  const std::string out_path =
      (argc > 1) ? argv[1] : std::string("data/trace.csv");

  Network net;

  // Neuron A: driven by external current → regular spikes.
  Neuron& a = net.add_neuron("A");
  a.set_I_ext(10.0);  // µA/cm²

  // Neuron B: silent until synaptic input from A.
  Neuron& b = net.add_neuron("B");
  b.set_I_ext(0.0);

  // Excitatory conductance-based synapse A → B.
  // weight chosen so each spike in A produces a clear EPSP / can elicit spikes in B.
  net.add_synapse(/*pre=*/0, /*post=*/1, /*weight=*/1.5, /*E_syn=*/0.0,
                  /*tau_ms=*/2.0);

  Simulator sim(net, /*dt_ms=*/0.01);
  sim.run(/*duration_ms=*/100.0);
  sim.write_csv(out_path);

  std::size_t spikes_a = 0;
  std::size_t spikes_b = 0;
  bool above_a = false;
  bool above_b = false;
  for (const auto& s : sim.trace()) {
    const bool now_a = s.V_a >= 0.0;
    const bool now_b = s.V_b >= 0.0;
    if (now_a && !above_a) {
      ++spikes_a;
    }
    if (now_b && !above_b) {
      ++spikes_b;
    }
    above_a = now_a;
    above_b = now_b;
  }

  std::cout << "Wrote " << out_path << " (" << sim.trace().size()
            << " samples)\n"
            << "Neuron A spikes: " << spikes_a << "\n"
            << "Neuron B spikes: " << spikes_b << "\n";
  return 0;
}
