import osmnx as ox
import matplotlib.pyplot as plt
import contextily as ctx
import os
import shutil
import json

# Get the road network around a point
G = ox.graph.graph_from_point((23.2540, 77.4160), dist = 500, network_type = "drive")

# Convert graph to GeoDataFrames
gdf_nodes, gdf_edges = ox.graph_to_gdfs(G, nodes = True, edges = True)

# Plot edges and nodes
fig, ax = plt.subplots(figsize=(20, 20))
gdf_edges.plot(ax = ax, linewidth = 0.7, edgecolor = "black")
gdf_nodes.plot(ax = ax, color = "red", markersize = 12)

# Add basemap
ctx.add_basemap(ax, source = ctx.providers.OpenStreetMap.Mapnik, zoom = 16, attribution_size = 6)

# Save the figure
ax.set_axis_off()
plt.tight_layout()
plt.savefig("../maps/map.png", dpi = 500, bbox_inches = 'tight')


nodes = {}
for n, data in G.nodes(data = True):
    nodes[str(n)] = {
        "lat": float(data['y']),
        "lon": float(data['x'])
    }

edges = []
for u, v, data in G.edges(data = True):
    weight = data.get("length", 1.0) # Meters
    edges.append({
        "from": str(u),
        "to": str(v),
        "weight": weight
    })

# Save the map as a graph
with open("../maps/map_graph.json", "w") as f:
    json.dump({"nodes": nodes, "edges": edges}, f, indent = 2)

# Delete the cache
cache_path = "./cache"
if (os.path.exists(cache_path)):
    shutil.rmtree("./cache")