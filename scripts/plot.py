#!/usr/bin/env python3
"""Plot Hodgkin–Huxley traces exported by brain_hh."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


def load_csv(path: Path) -> dict[str, list[float]]:
    with path.open(newline="") as f:
        reader = csv.DictReader(f)
        cols: dict[str, list[float]] = {name: [] for name in reader.fieldnames or []}
        for row in reader:
            for key, value in row.items():
                cols[key].append(float(value))
    return cols


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "csv_path",
        nargs="?",
        default="data/trace.csv",
        help="CSV from brain_hh (default: data/trace.csv)",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="data/trace.png",
        help="Output image path (default: data/trace.png)",
    )
    args = parser.parse_args()

    try:
        import matplotlib.pyplot as plt
    except ImportError as exc:
        raise SystemExit(
            "matplotlib is required: pip install matplotlib"
        ) from exc

    data = load_csv(Path(args.csv_path))
    t = data["t"]

    fig, axes = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

    axes[0].plot(t, data["V_a"], label="V_A (driven)", color="#1f77b4", lw=1.2)
    axes[0].plot(t, data["V_b"], label="V_B (postsynaptic)", color="#d62728", lw=1.2)
    axes[0].set_ylabel("V (mV)")
    axes[0].set_title("Membrane potential")
    axes[0].legend(loc="upper right", frameon=False)
    axes[0].axhline(0.0, color="0.7", lw=0.8, ls="--")

    axes[1].plot(t, data["m"], label="m (Na activation)", color="#2ca02c", lw=1.0)
    axes[1].plot(t, data["h"], label="h (Na inactivation)", color="#ff7f0e", lw=1.0)
    axes[1].plot(t, data["n"], label="n (K activation)", color="#9467bd", lw=1.0)
    axes[1].set_ylabel("gate")
    axes[1].set_title("Neuron A — channel gates")
    axes[1].legend(loc="upper right", frameon=False, ncol=3)
    axes[1].set_ylim(0.0, 1.05)

    axes[2].plot(t, data["I_Na"], label="I_Na", color="#1f77b4", lw=1.0)
    axes[2].plot(t, data["I_K"], label="I_K", color="#d62728", lw=1.0)
    axes[2].plot(t, data["I_syn_b"], label="I_syn on B", color="#2ca02c", lw=1.0)
    axes[2].set_ylabel("I (µA/cm²)")
    axes[2].set_xlabel("t (ms)")
    axes[2].set_title("Currents")
    axes[2].legend(loc="upper right", frameon=False, ncol=3)

    fig.tight_layout()
    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(out, dpi=150)
    print(f"Saved {out}")


if __name__ == "__main__":
    main()
