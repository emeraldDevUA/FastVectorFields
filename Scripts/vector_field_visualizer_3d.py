import json
import numpy as np
import plotly.graph_objects as go
import sys

# --------------------
# Load JSON file
# --------------------
field_path = sys.argv[1]

with open(field_path, "r", encoding="utf-8") as f:
    data = json.load(f)

vf = data["vector_field"]
Nx = vf["x_size"]
Ny = vf["y_size"]
Nz = vf["z_size"]
inner = vf["inner_data"]

# --------------------
# Extract components
# --------------------
# Assuming each inner element is a dict: {"x":..., "y":..., "z":...}
U = np.array([v["x"] for v in inner]).reshape(Nz, Ny, Nx)
V = np.array([v["y"] for v in inner]).reshape(Nz, Ny, Nx)
W = np.array([v["z"] for v in inner]).reshape(Nz, Ny, Nx)

# --------------------
# Create 3D grid coordinates
# --------------------
Z, Y, X = np.meshgrid(np.arange(Nz), np.arange(Ny), np.arange(Nx), indexing="ij")

# --------------------
# Mask near-zero vectors
# --------------------
eps = 1e-12
mag = np.sqrt(U**2 + V**2 + W**2)
mask = mag > eps

X_m = X[mask]
Y_m = Y[mask]
Z_m = Z[mask]
U_m = U[mask]
V_m = V[mask]
W_m = W[mask]
mag_m = mag[mask]

# --------------------
# Subsample for readability
# --------------------
step = max(1, int(len(X_m) ** 0.33))  # roughly cube root
# X_m = X_m[::step]
# Y_m = Y_m[::step]
# Z_m = Z_m[::step]
# U_m = U_m[::step]
# V_m = V_m[::step]
# W_m = W_m[::step]
# mag_m = mag_m[::step]

# --------------------
# Normalize vectors for arrow directions
# --------------------
U_n = U_m / mag_m
V_n = V_m / mag_m
W_n = W_m / mag_m
# --------------------
# Angle with Z-axis (polar angle)
# --------------------
angle = np.arccos(np.clip(U_n, -1.0, 1.0))  # radians
angle_n = angle / np.pi

U_c = U_n * angle_n
V_c = V_n * angle_n
W_c = W_n * angle_n
fig = go.Figure(
    data=go.Cone(
        x=X_m,
        y=Y_m,
        z=Z_m,
        u=U_c,
        v=V_c,
        w=W_c,
        colorscale="Inferno",
        cmin=0.0,
        cmax=1.0,  # angle_n range
        sizemode="scaled",
        sizeref=1,
        showscale=True,
    )
)

fig.update_layout(
    title="Vector Field Colored by Angle to Z-axis",
    scene=dict(
        xaxis_title="X",
        yaxis_title="Y",
        zaxis_title="Z",
        aspectmode="data",
    ),
    coloraxis_colorbar=dict(title="Angle to Z-axis (0 → π)"),
)

fig.show()
