
from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

barrier = 120
ROOT = Path(__file__).resolve().parent.parent
paths = np.loadtxt(ROOT / "data" /"paths.csv", delimiter=",")

plt.figure(figsize=(10, 6))
for path in paths:
    plt.plot(path, color='green' if path.max() >= barrier else 'red', alpha=0.7)
plt.axhline(barrier, color='blue', linestyle='--', label='Barrier')
plt.title("Sample GBM Paths for Up-and-In Call")
plt.xlabel("Steps")
plt.ylabel("Price")
plt.legend()
plt.savefig(ROOT / "images" / "paths.png", dpi=150)
print("Saved chart to", ROOT / "images" / "paths.png")
plt.show()
