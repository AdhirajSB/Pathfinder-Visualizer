#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raymath.h>

class Renderer {
private:
    int m_Width, m_Height;
    Camera2D m_Camera;
    Image m_MapImage;
    Texture2D m_MapTexture;

public:
    Renderer(int width, int height, const std::string& name)
        : m_Width(width), m_Height(height)
    {
        InitWindow(m_Width, m_Height, name.c_str());
        SetTargetFPS(60);

        // Initialize camera
        m_Camera.offset = { m_Width / 2.0f, m_Height / 2.0f };
        m_Camera.target = { 0, 0 };
        m_Camera.zoom = 1.0f;
        m_Camera.rotation = 0.0f;
    }

    ~Renderer() {
        UnloadTexture(m_MapTexture);
        CloseWindow();
    }

    inline float MapWidth() const { return m_MapTexture.width; }
    inline float MapHeight() const { return m_MapTexture.height; }

    void LoadMap(const std::string& path) {
        m_MapImage = LoadImage(path.c_str());
        m_MapTexture = LoadTextureFromImage(m_MapImage);
        UnloadImage(m_MapImage);

        // Center camera on map
        m_Camera.target = {
            m_MapTexture.width / 2.0f,
            m_MapTexture.height / 2.0f
        };
    }

    void HandleInput() {
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            const float zoomFactor = 1.1f;
            Vector2 mouseScreen = GetMousePosition();
            Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, m_Camera);

            if (wheel > 0) m_Camera.zoom *= zoomFactor;
            else m_Camera.zoom /= zoomFactor;

            m_Camera.zoom = Clamp(m_Camera.zoom, 0.1f, 10.0f);

            Vector2 newMouseWorld = GetScreenToWorld2D(mouseScreen, m_Camera);
            Vector2 delta = Vector2Subtract(mouseWorld, newMouseWorld);
            m_Camera.target = Vector2Add(m_Camera.target, delta);
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);
            m_Camera.target = Vector2Add(m_Camera.target, delta);
        }
    }

    void DrawMap() {
        DrawTexture(m_MapTexture, 0, 0, WHITE);
    }

    inline bool Running() const { return !WindowShouldClose(); }

    Vector2 ScreenToWorld(Vector2 screenPos) const {
        return GetScreenToWorld2D(screenPos, m_Camera);
    }

    Vector2 WorldToScreen(Vector2 worldPos) const {
        return GetWorldToScreen2D(worldPos, m_Camera);
    }
    
    const Camera2D& GetCamera() const { return m_Camera; }
    
    // Utility method for precise coordinate conversion
    Vector2 LatLonToPixel(double lat, double lon, 
                         double lat_min, double lat_max, 
                         double lon_min, double lon_max) const {
        double x = ((lon - lon_min) / (lon_max - lon_min)) * static_cast<double>(m_MapTexture.width);
        double y = ((lat_max - lat) / (lat_max - lat_min)) * static_cast<double>(m_MapTexture.height);
        
        return {static_cast<float>(x), static_cast<float>(y)};
    }
};

#endif