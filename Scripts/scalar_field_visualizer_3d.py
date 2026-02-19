import sys
import json
import numpy as np
import plotly.graph_objects as go


# --------------------
# Load JSON file
# --------------------
field_path = sys.argv[1]

with open(field_path, "r", encoding="utf-8") as f:
    data = json.load(f)

sf = data["scalar_field"]

Nx = sf["x_size"]
Ny = sf["y_size"]
Nz = sf["z_size"]
inner = sf["inner_data"]


# --------------------
# Convert Cereal values -> float
# --------------------
def extract_scalar(v):
    if isinstance(v, dict):
        # works for cereal wrappers
        return float(next(iter(v.values())))
    return float(v)


values = np.array([extract_scalar(v) for v in inner], dtype=np.float64)

# reshape to 3D
S = values.reshape(Nz, Ny, Nx)

# --------------------
# Create coordinate grid
# --------------------
Z, Y, X = np.meshgrid(np.arange(Nz), np.arange(Ny), np.arange(Nx), indexing="ij")

# flatten (required by Plotly)
x = X.flatten()
y = Y.flatten()
z = Z.flatten()
v = S.flatten()

# --------------------
# Volume rendering
# --------------------
fig = go.Figure(
    data=go.Volume(
        x=x,
        y=y,
        z=z,
        value=v,
        # controls the visible scalar range
        isomin=np.percentile(v, 10),
        isomax=np.percentile(v, 90),
        # number of surfaces (higher = smoother)
        surface_count=20,
        opacity=0.12,
        colorscale="Viridis",
        colorbar=dict(title="Scalar value"),
    )
)

fig.update_layout(
    title="3D Scalar Field (Volume Rendering)",
    width=1000,
    height=800,
    scene=dict(
        xaxis_title="X",
        yaxis_title="Y",
        zaxis_title="Z",
        aspectmode="data",
    ),
)

fig.show()
