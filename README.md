# SimuCache-DBI: Multi-Level Cache Architecture Simulator & Visualizer

An end-to-end, highly configurable 3-level (L1, L2, L3) cache architecture simulator built in C++ and integrated with a full-stack profiling toolchain[cite: 1, 2, 3, 5]. Developed as part of a computer architecture project, this tool utilizes Dynamic Binary Instrumentation (DBI) via Intel Pin to capture live memory access traces from executing binaries, processes them through a custom cache simulation engine, and visualizes the architectural bottlenecks via a Node.js/Python web dashboard[cite: 1, 2, 3, 5, 7].

## 🚀 Core Features

*   **Custom Cache Engine (C++):** Simulates L1, L2, and L3 caches with dynamic runtime parameters for block sizes, scaling capacities, and variable mapping structures (direct-mapped, set-associative, and fully-associative)[cite: 1, 5].
*   **Strict Memory Consistency:** Implements a Least Recently Used (LRU) replacement algorithm alongside a Write-Back policy, ensuring accurate recursive dirty-bit evictions across the entire memory hierarchy down to Main Memory (RAM)[cite: 5].
*   **Live Binary Tracing (Intel Pin):** Captures real-time memory read/write instructions (`trace.out`) directly from compiled target binaries during execution[cite: 7].
*   **Automated Analytics Pipeline:** A Node.js (`Express`) backend bridges the C++ simulator with a Python visualizer (`Matplotlib`, `Seaborn`), generating hit/miss spatial distribution heatmaps[cite: 2, 3].
*   **Interactive Web UI:** A frontend dashboard to configure cache parameters, trigger executions, and dynamically render execution states and memory bottlenecks without relying on heavy frontend processing[cite: 1, 6].

## 🛠️ Technology Stack

*   **Systems Programming:** `C++`, `Intel Pin (Pintool)`
*   **Backend / Bridge:** `Node.js`, `Express.js`, `Bash / Batch Scripting`
*   **Data Analytics & Visualization:** `Python 3`, `Matplotlib`, `NumPy`, `Seaborn`
*   **Frontend:** `HTML5`, `CSS3`, `JavaScript (ES6+)`

## ⚙️ Architecture Workflow

1.  **Instrumentation:** Intel Pin injects into a target binary (e.g., `test_app.exe`) and outputs a raw memory access log (`trace.out`)[cite: 7, 9].
2.  **Configuration:** The user defines L1/L2/L3 parameters (Sets, Associativity, Block Size) via the Web UI[cite: 1].
3.  **Simulation:** The Express server triggers the C++ engine with the defined parameters, parsing the millions of lines in `trace.out` to compute exact hits, misses, and eviction states[cite: 2, 5].
4.  **Visualization:** Python processes the simulation statistics, generating a spatial heatmap mapping the "Hot/Active" and "Cold/Dead" cache lines[cite: 3].
5.  **Rendering:** The backend encodes the resulting PNG into Base64 and serves it back to the frontend DOM alongside the C++ execution metrics[cite: 2].

## 📦 Prerequisites

To run this pipeline locally, you need the following installed and added to your system `PATH`:
*   **C++ Compiler:** `g++` (MinGW for Windows or GCC for Linux)
*   **Intel Pin:** Pre-compiled Windows MSVC `.dll` or Linux `.so` Pintool[cite: 9].
*   **Node.js:** v14+ (with `express` and `cors` packages)
*   **Python 3:** (with `matplotlib`, `numpy`, and `seaborn` packages)

## 🏁 Getting Started (Windows)

1. Clone the repository:
   ```cmd
   git clone https://github.com/YOUR_USERNAME/SimuCache-DBI.git
   cd SimuCache-DBI
   ```

2. Install backend dependencies:
   ```cmd
   npm install express cors
   python -m pip install matplotlib numpy seaborn
   ```

3. Ensure your `trace.out` file is generated and placed in the root directory[cite: 9].

4. Run the automated startup script:
   ```cmd
   start.bat
   ```
   *This script automatically kills lingering ports, compiles the C++ engine, starts the Node server, and launches the web interface*[cite: 9].

5. Configure your hardware parameters in the browser and click **Run Simulation (trace.out)**[cite: 1].
