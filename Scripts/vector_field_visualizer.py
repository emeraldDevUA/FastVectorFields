import json
import numpy as np
import matplotlib.pyplot as plt

import sys

# Load JSON
field_path = sys.argv[1]

# Load JSON file
with open(field_path, "r", encoding="utf-8") as f:
    data = json.load(f)

# Navigate structure
vf = data["vector_field"]
Nx = vf["x_size"]
Ny = vf["y_size"]
inner = vf["inner_data"]

# Extract components
U = np.array([v["x"] for v in inner]).reshape(Ny, Nx)
V = np.array([v["y"] for v in inner]).reshape(Ny, Nx)

# Grid coordinates
X, Y = np.meshgrid(np.arange(Nx), np.arange(Ny))

# Magnitude
mag = np.sqrt(U**2 + V**2)

# Mask: exclude zero (or near-zero) vectors
eps = 1e-12
mask = mag > eps

# Direction angle
angles = np.arctan2(V, U)

# Apply mask (flattened)
X_m = X[mask]
Y_m = Y[mask]
U_m = U[mask]
V_m = V[mask]
angles_m = angles[mask]

# Plot
plt.figure(figsize=(7.3, 6))
q = plt.quiver(X_m, Y_m, U_m, V_m, angles_m, cmap="hsv", pivot="middle")

plt.colorbar(q, label="Direction (radians)")
plt.title("Vector Field Colored by Direction (Zero Vectors Removed)")
# plt.gca().invert_yaxis()
plt.axis("equal")
plt.tight_layout()
plt.show()
