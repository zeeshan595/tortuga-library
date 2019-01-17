#ifndef _EVENT
#define _EVENT

namespace Tortuga
{
enum class EventType
{
    None = 0,
    //Window based events
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    //Key based events
    KeyPressed,
    KeyReleased,
    //Mouse based events
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3)
};
}; // namespace Tortuga

#endif