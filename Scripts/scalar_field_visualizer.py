import json
import numpy as np
import matplotlib.pyplot as plt
import sys

# Load JSON
field_path = sys.argv[1]

with open(field_path, "r", encoding="utf-8") as f:
    data = json.load(f)

sf = data["scalar_field"]
Nx = sf["x_size"]
Ny = sf["y_size"]
inner = sf["inner_data"]

# Convert to 2D array
S = np.array(inner).reshape(Ny, Nx)

# Plot
plt.figure(figsize=(8, 6))
im = plt.imshow(S, origin="upper", cmap="viridis")

plt.colorbar(im, label="Scalar value")
plt.title("Scalar Field Heatmap")
plt.xlabel("x")
plt.ylabel("y")
plt.axis("equal")
plt.tight_layout()
plt.show()
