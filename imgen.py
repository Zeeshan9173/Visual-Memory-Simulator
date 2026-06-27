import matplotlib.pyplot as plt
import numpy as np
import sys
from matplotlib.colors import LinearSegmentedColormap

# --- 1. Argument Handling ---
if len(sys.argv) > 7:
    b_size = int(sys.argv[1])
    l1_s, l1_a = int(sys.argv[2]), int(sys.argv[3])
    l2_s, l2_a = int(sys.argv[4]), int(sys.argv[5])
    l3_s, l3_a = int(sys.argv[6]), int(sys.argv[7])
else:
    b_size, l1_s, l1_a = 64, 128, 4
    l2_s, l2_a = 1024, 8
    l3_s, l3_a = 2048, 16

# --- 2. Simulation Stats (Placeholders for 3 levels) ---
activity = {
    'L1': {'hits': 800000, 'misses': 51000},
    'L2': {'hits': 30000, 'misses': 21000},
    'L3': {'hits': 10000, 'misses': 11000}
}

def get_miss_rate(lvl):
    total = activity[lvl]['hits'] + activity[lvl]['misses']
    return (activity[lvl]['misses'] / total) if total > 0 else 0

# --- 3. Visualization Setup ---
# We create a layout with 3 plots on top and 1 text area on bottom
fig = plt.figure(figsize=(15, 10), constrained_layout=True)
gs = fig.add_gridspec(2, 3, height_ratios=[6, 4])

# Define Heatmap Color Scheme
cmap = LinearSegmentedColormap.from_list("cache_map", ["#ff4444", "#aa22aa", "#22bb22", "#ffff44"])

# --- 4. Generate 3 Heatmaps (L1, L2, L3) ---
levels = [('L1', l1_a), ('L2', l2_a), ('L3', l3_a)]

for i, (name, assoc) in enumerate(levels):
    ax = fig.add_subplot(gs[0, i])
    
    # Create representative data (Scaling the 'visual' sets for display)
    display_sets = 64 
    data = np.zeros((display_sets, assoc))
    
    # Simulate locality differences visually
    # L1 usually has higher 'life' (yellow/green), L3 has more 'dead' (red)
    decay = 0.2 if name == 'L1' else (0.5 if name == 'L2' else 0.8)
    for s in range(display_sets):
        for a in range(assoc):
            data[s, a] = np.clip((1.0 - (a / assoc)) * (1.0 - (s / display_sets)**decay) + np.random.uniform(-0.1, 0.1), 0, 1)

    im = ax.imshow(data, cmap=cmap, aspect='auto', interpolation='nearest', vmin=0, vmax=1)
    ax.set_title(f"{name} Heatmap\n({assoc}-Way)", fontsize=14, fontweight='bold')
    ax.set_xlabel("Ways")
    if i == 0: ax.set_ylabel("Sets (Sample)")

# Add a single colorbar for all heatmaps
cbar = fig.colorbar(im, ax=fig.axes, location='right', shrink=0.5)
cbar.set_ticks([0.1, 0.9])
cbar.set_ticklabels(['Cold/Dead', 'Hot/Active'])

# --- 5. Analysis & Summary Text ---
ax_text = fig.add_subplot(gs[1, :]) # Spans all 3 columns
ax_text.axis('off')

summary = (f"Cache Hierarchy Performance Summary\n"
           f"{'='*80}\n"
           f"L1 Miss Rate: {get_miss_rate('L1'):.2%} | Capacity: {(b_size*l1_s*l1_a)/1024:.1f} KB\n"
           f"L2 Miss Rate: {get_miss_rate('L2'):.2%} | Capacity: {(b_size*l2_s*l2_a)/1024:.1f} KB\n"
           f"L3 Miss Rate: {get_miss_rate('L3'):.2%} | Capacity: {(b_size*l3_s*l3_a)/(1024*1024):.1f} MB\n"
           f"{'='*80}\n\n"
           f"Architectural Analysis:\n")

# Logic-based classification
if get_miss_rate('L1') > 0.15:
    summary += "Critical: High L1 Miss Rate detected. Workload lacks temporal locality.\n"
if get_miss_rate('L2') > 0.25:
    summary += "Observation: L2 Thrashing. Consider increasing L2 Associativity.\n"
else:
    summary += "System Health: Cache hierarchy is efficiently filtering requests to Main Memory.\n"

ax_text.text(0.01, 0.9, summary, transform=ax_text.transAxes, 
             fontsize=12, family='monospace', va='top', linespacing=1.5,
             bbox=dict(boxstyle='round', facecolor='blue', alpha=0.03))

# --- 6. Save ---
filename = "cache_simulator_heatmap.png"
plt.savefig(filename, dpi=100)
plt.close(fig)

print(f"Generated Triple-Cache Heatmap: {filename}")