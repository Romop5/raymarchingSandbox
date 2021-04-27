#ifndef RAYMARCHER_IMGUI_ADAPTER_HPP
#define RAYMARCHER_IMGUI_ADAPTER_HPP

#include <cstddef>

namespace raymarcher
{

    enum ModifierBitmap
    {
        NORMAL  = 0 << 0,
        CTRL    = 1 << 0,
        SHIFT   = 1 << 1,
    };

    /**
     * @brief Adapts DearImgui into project
     */
    class ImguiAdapter
    {
        public:
            /// Prepare resources
            bool Initialize(size_t width, size_t height);

            /// Change internal screen resolution
            void Resize(size_t newWidth, size_t newHeight);

            /// Start accepting user's definitions
            void BeginFrame();
            /// End of user's definitions and prepare draw buffer
            void EndFrame();
            /// Render draw buffer
            void RenderCurrentFrame();
            /// Clean up
            void Destroy();

            void OnCharacter(size_t character);
            void OnKey(size_t key, bool isDown, ModifierBitmap mod);
            void OnMousePositionRelative(float deltaX, float deltaY);
            void OnMousePosition(float x, float y);
            void OnButton(size_t buttonID, bool isPressed);

            bool IsVisible();
            void SetVisibility(bool isVisible);
            void SetScaling(float scale);

            bool WantCaptureMouse() const;
            bool WantCaptureKeyboard() const;
        private:
            bool m_Visibility = false;
            float m_Scaling = 1.0;

            float posX = 0.0;
            float posY = 0.0;
    };
} // namespace ve
#endif
