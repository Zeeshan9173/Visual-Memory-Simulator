const express = require('express');
const { exec } = require('child_process');
const cors = require('cors');
const fs = require('fs'); // Added for reading the image file
const path = require('path'); // Added for path handling

const app = express();

// Middleware
app.use(cors());
app.use(express.json());

app.post('/simulate', (req, res) => {
    const { block, l1_sets, l1_assoc, l2_sets, l2_assoc, l3_sets, l3_assoc } = req.body;
    
    const isWindows = process.platform === "win32";
    const executable = isWindows ? "cache_sim.exe" : "./cache_sim";
    const pythonCmd = isWindows ? "python" : "python3";

    // 1. Construct the C++ command
    const simCmd = `${executable} ${block} ${l1_sets} ${l1_assoc} ${l2_sets} ${l2_assoc} ${l3_sets} ${l3_assoc}`;
    
    console.log("Step 1: Running Simulation...");

    exec(simCmd, (error, stdout, stderr) => {
        if (error) {
            console.error("Simulation Error:", stderr);
            return res.status(500).send(stderr || "Simulation failed");
        }

        // 2. Run the Python Visualizer (lauda.py)
        console.log("Step 2: Generating Heatmap...");
        
        exec(`${pythonCmd} imgen.py`, (pyError, pyStdout, pyStderr) => {
            if (pyError) {
                console.error("Python Error:", pyStderr);
                return res.status(500).send("Visualization failed");
            }

            // 3. Read the generated PNG and convert to Base64
            const imagePath = path.join(__dirname, 'cache_simulator_heatmap.png');
            
            try {
                const imageBase64 = fs.readFileSync(imagePath, 'base64');
                
                // 4. Send both the console stats and the image back as JSON
                res.json({
                    success: true,
                    stats: stdout, // This is your C++ output
                    heatmap: `data:image/png;base64,${imageBase64}` // This is your image
                });
            } catch (fsError) {
                console.error("File Reading Error:", fsError);
                res.status(500).send("Could not find generated heatmap image.");
            }
        });
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Bridge server running at http://localhost:${PORT}`);
});