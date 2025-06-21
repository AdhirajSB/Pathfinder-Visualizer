import osmnx as ox
import matplotlib.pyplot as plt
import contextily as ctx
import os
import shutil
import json
from PIL import Image

# Get the road network around a point
G = ox.graph.graph_from_point((23.2540, 77.4160), dist = 1000, network_type = "drive")

# Convert graph to GeoDataFrames
gdf_nodes, gdf_edges = ox.graph_to_gdfs(G, nodes = True, edges = True)

# Calculate exact bounds
lon_min, lat_min = float(gdf_nodes['x'].min()), float(gdf_nodes['y'].min())
lon_max, lat_max = float(gdf_nodes['x'].max()), float(gdf_nodes['y'].max())

print(f"Data bounds: lon({lon_min:.6f}, {lon_max:.6f}), lat({lat_min:.6f}, {lat_max:.6f})")

# Create figure with exact aspect ratio
lon_range = lon_max - lon_min
lat_range = lat_max - lat_min
aspect_ratio = lon_range / lat_range

# Set a fixed width and calculate height to maintain aspect ratio
fig_width = 20
fig_height = fig_width / aspect_ratio

print(f"Figure size: {fig_width}x{fig_height:.2f}, aspect ratio: {aspect_ratio:.4f}")

fig, ax = plt.subplots(figsize=(fig_width, fig_height))

# Plot edges and nodes
gdf_edges.plot(ax=ax, linewidth=0.7, edgecolor="black")
gdf_nodes.plot(ax=ax, color="red", markersize=12)

# Set EXACT limits - this is crucial
ax.set_xlim(lon_min, lon_max)
ax.set_ylim(lat_min, lat_max)

# Remove all margins and padding
ax.set_aspect('equal')  # This ensures 1:1 aspect ratio
plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
ax.set_axis_off()

# Save with specific DPI and NO bbox_inches adjustment
dpi = 100  # Use a round number for easier calculation
plt.savefig("../maps/map.png", dpi=dpi, bbox_inches=None, pad_inches=0)

# Get actual figure size in pixels
fig_width_px = int(fig_width * dpi)
fig_height_px = int(fig_height * dpi)

print(f"Saved image size: {fig_width_px}x{fig_height_px} pixels")

# Verify the saved image size
img = Image.open("../maps/map.png")
actual_width, actual_height = img.size
print(f"Actual saved image size: {actual_width}x{actual_height} pixels")

if actual_width != fig_width_px or actual_height != fig_height_px:
    print("WARNING: Image size mismatch! This will cause scaling issues.")

plt.close()

# Save bounds that match the EXACT image coordinate system
bounds = {
    "lat_min": lat_min,
    "lat_max": lat_max,
    "lon_min": lon_min,
    "lon_max": lon_max,
    "image_width": actual_width,
    "image_height": actual_height,
    "dpi": dpi
}

print(f"Bounds saved: {bounds}")

with open("../maps/map_bounds.json", "w") as f:
    json.dump(bounds, f, indent=2)

# Save node and edge data
nodes = {}
for n, data in G.nodes(data=True):
    nodes[str(n)] = {
        "lat": float(data['y']),
        "lon": float(data['x'])
    }

edges = []
for u, v, data in G.edges(data=True):
    weight = data.get("length", 1.0)
    edges.append({
        "from": str(u),
        "to": str(v),
        "weight": weight
    })

with open("../maps/map_graph.json", "w") as f:
    json.dump({"nodes": nodes, "edges": edges}, f, indent=2)

# Verify coordinate transformation with a test node
if nodes:
    test_node = next(iter(nodes.values()))
    test_lat, test_lon = test_node['lat'], test_node['lon']
    
    # Calculate where this node should appear in pixels
    x_pixel = ((test_lon - lon_min) / (lon_max - lon_min)) * actual_width
    y_pixel = ((lat_max - test_lat) / (lat_max - lat_min)) * actual_height
    
    print(f"Test node: lat={test_lat:.6f}, lon={test_lon:.6f}")
    print(f"Should be at pixel: ({x_pixel:.2f}, {y_pixel:.2f})")

# Delete the cache
cache_path = "./cache"
if os.path.exists(cache_path):
    shutil.rmtree("./cache")

print("Script completed successfully!")