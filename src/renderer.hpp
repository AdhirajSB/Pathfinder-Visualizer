#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raymath.h>


class Renderer {
    private:
    int m_Width, m_Height;
    float m_Zoom = 1.0f;

    Image m_MapImage;
    Texture2D m_MapTexture;
    Vector2 m_Offset = {0, 0};

    public:
    Renderer(int width, int height, const std::string& name)
        :m_Width(width), m_Height(height) 
    {
        InitWindow(m_Width, m_Height, name.c_str());
        SetTargetFPS(60);
    }
    ~Renderer() {
        UnloadTexture(m_MapTexture);
        CloseWindow();
    }

    void LoadMap(const std::string& path) {
        m_MapImage = LoadImage(path.c_str());
        m_MapTexture = LoadTextureFromImage(m_MapImage);

        UnloadImage(m_MapImage);

        m_Offset.x = (m_Width - m_MapTexture.width) / 2.0f;
        m_Offset.y = (m_Height - m_MapTexture.height) / 2.0f;
    }

    void HandleInput() {
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f){
            Vector2 mouse = GetMousePosition();
            Vector2 worldPos = {
                (mouse.x - m_Offset.x) / m_Zoom,
                (mouse.y - m_Offset.y) / m_Zoom
            };

            const float zoomFactor = 1.1f;

            if (wheel > 0) m_Zoom *= zoomFactor;
            else if (wheel < 0) m_Zoom /= zoomFactor;

            m_Zoom = Clamp(m_Zoom, 0.1f, 10.0f);

            m_Offset.x = mouse.x - worldPos.x * m_Zoom;
            m_Offset.y = mouse.y - worldPos.y * m_Zoom;
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
            Vector2 delta = GetMouseDelta();
            m_Offset.x += delta.x;
            m_Offset.y += delta.y;
        }

        SetBounds();
    }

    void DrawBackground() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureEx(
            m_MapTexture,
            m_Offset,
            0.0f,
            m_Zoom,
            WHITE
        );
        EndDrawing();
    }

    inline bool Running() const { return !WindowShouldClose(); }

    private:
    void SetBounds() {
        float imageWidth = m_MapTexture.width * m_Zoom;
        float imageHeight = m_MapTexture.height * m_Zoom;
        
        // Calculate bounds
        float minX = m_Width - imageWidth;  
        float maxX = 0;                   
        float minY = m_Height - imageHeight; 
        float maxY = 0;                   
        
        // If image is smaller than screen, center it
        if (imageWidth < m_Width) {
            minX = maxX = (m_Width - imageWidth) / 2;
        }
        if (imageHeight < m_Height) {
            minY = maxY = (m_Height - imageHeight) / 2;
        }
        
        // Apply constraints
        m_Offset.x = Clamp(m_Offset.x, minX, maxX);
        m_Offset.y = Clamp(m_Offset.y, minY, maxY);
    }
};

#endif