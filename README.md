# 🗺️ Pathfinder Visualizer

A real-world Dijkstra pathfinding visualizer built with **Raylib** and **C++**, using street network data from **OpenStreetMap** via `osmnx`.

It allows interactive visualization of road networks, selection of custom start and end points, and renders the shortest path using Dijkstra’s algorithm.

---
## 🖼️ Preview

![Pathfinder Demo](screenshots/demo.png)


## ✨ Features

- 🛰️ **Real Map Data**: Automatically fetched via `osmnx` API from OpenStreetMap  
- 🎮 **Raylib UI**: Smooth pan, zoom, and mouse interaction  
- 🖱️ **Custom Start & End Nodes**: User can select source and destination interactively  
- 🔴 **Node Visualization**: All graph nodes are rendered as red dots  
- 📍 **Shortest Path Rendering**: Highlights the optimal route using Dijkstra  
- 📦 **Modular C++ Design**: Easy to extend with new features or algorithms  

---

## 🛠️ Setup

### 1. Python Dependencies

To generate the map and graph files, install the following Python packages:

```bash
pip install osmnx matplotlib pillow
```

### 2. Generate the Map Data

To alter the location of the map, open `map_loader.py` and change the latitude and longitude values.

Then, run the following command inside the `src` folder:

```bash
python3 map_loader.py
```

This will generate the following files inside a `maps/` folder:

```
maps/
├── map.png
├── map_bounds.json
└── map_graph.json
```

These are required by the C++ visualizer.

---

### 3. Compile the C++ Files

Make sure you have the following installed:

#### ✅ Requirements:
- A C++17-compatible compiler (e.g. `g++`)
- [`raylib`](https://www.raylib.com/) (graphics & input library)
- [`nlohmann/json`](https://github.com/nlohmann/json) (for JSON parsing)

#### 🧱 Compile with `g++` (Linux/macOS example):

```bash
g++ main.cpp -o pathfinder -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

> 💡 On macOS, replace `-lGL` and `-lX11` with:  
> `-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo`

---

### 4. Run the Visualizer

After compiling and generating the map data:

```bash
./pathfinder
```

---

## 🎮 Controls

| Action                   | Input                    |
|--------------------------|--------------------------|
| Zoom in/out              | Mouse Wheel              |
| Pan map                  | Right Mouse Drag         |
| Print cursor coordinates | Left Click (console)     |
| Select nodes (planned)   | Click start/end locations |

---

## 📦 Project Structure

| File               | Description                                |
|--------------------|--------------------------------------------|
| `main.cpp`         | App entry point + main loop                |
| `graph.hpp/.cpp`   | Loads graph structure from JSON            |
| `projector.hpp`    | Maps lat/lon to screen coordinates         |
| `renderer.hpp`     | Handles texture rendering, pan, zoom       |
| `pathfinder.hpp`   | Dijkstra implementation                    |
| `map_loader.py`    | Python script to fetch and export map data |

---

## 🧠 To Do

- ✅ Path drawing on map  
- 🖱️ Click-to-select start and end nodes  
- 📍 Animated path traversal  
- 🔁 Add support for alternative pathfinding algorithms  

---

## 📜 License

MIT — use freely with credit.

---

## 🙏 Thanks To

- [OpenStreetMap](https://www.openstreetmap.org/)
- [osmnx](https://github.com/gboeing/osmnx)
- [Raylib](https://www.raylib.com/)
- [matplotlib](https://matplotlib.org/)
- [Pillow (PIL)](https://pillow.readthedocs.io/)
