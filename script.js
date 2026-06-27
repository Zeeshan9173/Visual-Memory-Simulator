function calculateAll() {
    const block = parseInt(document.getElementById('block_size').value) || 0;

    const levels = ['l1', 'l2', 'l3'];
    levels.forEach(lvl => {
        const sets = parseInt(document.getElementById(`${lvl}_sets`).value) || 0;
        const assoc = parseInt(document.getElementById(`${lvl}_assoc`).value) || 0;
        
        // Formula: (Sets * Assoc * BlockSize) / 1024 = Size in KB
        const kb = (sets * assoc * block) / 1024;
        document.getElementById(`${lvl}_kb`).innerText = kb.toLocaleString();
    });
}

async function runCppSimulation() {
    const output = document.getElementById('consoleOutput');
    const heatmapImg = document.getElementById('heatmapImage');
    
    output.innerText = "Running simulation on 851k lines... generating heatmaps... please wait...";
    heatmapImg.style.display = 'none'; // Hide old image while loading

    const data = {
        block: document.getElementById('block_size').value,
        l1_sets: document.getElementById('l1_sets').value,
        l1_assoc: document.getElementById('l1_assoc').value,
        l2_sets: document.getElementById('l2_sets').value,
        l2_assoc: document.getElementById('l2_assoc').value,
        l3_sets: document.getElementById('l3_sets').value,
        l3_assoc: document.getElementById('l3_assoc').value,
    };

    try {
        const response = await fetch('http://localhost:3000/simulate', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        
        // 1. Wait for the JSON response instead of text
        const result = await response.json();
        
        // 2. Inject the C++ output into the console box
        output.innerText = result.stats || "Simulation finished with no console output.";
        
        // 3. Inject the Base64 image into the <img> tag and unhide it
        if (result.heatmap) {
            heatmapImg.src = result.heatmap;
            heatmapImg.style.display = 'block';
        }

    } catch (err) {
        output.innerText = "Error: Make sure the Node.js server is running! (node server.js)\n" + err.message;
    }
}

// Initial calculation
calculateAll();