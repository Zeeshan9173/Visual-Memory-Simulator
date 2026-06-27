# Visual Memory Simulator (VMS)

SimuCache-DBI/VMS is a highly configurable 3-level (L1, L2, L3) cache architecture simulator built in C++ that integrates with a full-stack profiling toolchain. It utilizes Dynamic Binary Instrumentation (DBI) via Intel Pin to capture live memory access traces from executing binaries, processes them through a custom cache simulation engine, and visualizes architectural bottlenecks—like hits, misses, and spatial distribution—via an interactive Node.js and Python-powered web dashboard.

##  How to Run

**1. Install Dependencies**
You will need Node.js, Python 3, and a C++ compiler (`g++`). Install the required backend and visualization libraries:

```bash
npm install express cors
python3 -m pip install matplotlib numpy seaborn
```

**2. Generate the Memory Trace (Linux / WSL)**
Before running the simulator, you need to generate a raw memory trace from a target application using Intel Pin. Ensure the generated `trace.out` is placed in the root directory of this project.

```bash
# Compile your target application
g++ -g test.cpp -o test_app

# Run Intel Pin to generate the trace.out file
/path/to/pin_kit/pin -t /path/to/cache_tracer.so -- ./test_app
```

**3.1. Launch the Simulator**
Compile the C++ cache engine and boot up the local bridge server:

```bash
# Compile the simulation engine
g++ main.cpp -o cache_sim

# Start the local backend
node server.js
```

**3.2. Alternate**
You can just execute the `run.bat` file, it will automatically compile the main.cpp file and run the server in the background and land you at the visual interface. 


**4. View the Dashboard**
Once the server is running, simply open `index.html` in your web browser. Adjust your L1/L2/L3 hardware parameters in the UI and click **Run Simulation (trace.out)** to generate your metrics and spatial heatmaps.
