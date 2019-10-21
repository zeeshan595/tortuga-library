#ifndef _CORE_INPUT
#define _CORE_INPUT

#include "./KeyCode.hpp"
#include <vector>
#include <string>

namespace Tortuga
{
namespace Core
{
namespace Input
{
//window close
typedef void (*OnWindowClose)();
void NotifyOnWindowClose(OnWindowClose callback);
void RemoveOnWindowClose(OnWindowClose callback);
void TriggerWindowClose();

//window minimize
typedef void (*OnWindowMinimize)(bool isMinimized);
void NotifyOnWindowMinimize(OnWindowMinimize callback);
void RemoveOnWindowMinimize(OnWindowMinimize callback);
void TriggerWindowMinimize(bool isMinimized);

//window maximized
typedef void (*OnWindowMaximized)(bool isMaximized);
void NotifyOnWindowMaximized(OnWindowMaximized callback);
void RemoveOnWindowMaximized(OnWindowMaximized callback);
void TriggerWindowMaximized(bool isMaximized);

//window focus
typedef void (*OnWindowFocus)(bool isFocused);
void NotifyOnWindowFocus(OnWindowFocus callback);
void RemoveOnWindowFocus(OnWindowFocus callback);
void TriggerWindowFocus(bool isFocused);

//window resized
typedef void (*OnWindowResize)(uint32_t width, uint32_t height);
void NotifyOnWindowResize(OnWindowResize callback);
void RemoveOnWindowResize(OnWindowResize callback);
void TriggerWindowResize(uint32_t width, uint32_t height);

//key events
typedef void (*OnKeyEvent)(KeyCode, KeyAction);
void NotifyOnKeyEvent(OnKeyEvent callback);
void RemoveOnKeyEvent(OnKeyEvent callback);
void TriggerKeyEvent(KeyCode key, KeyAction action);

//mouse events
typedef void (*OnMouseEvent)(double x, double y);
void NotifyOnMouseEvent(OnMouseEvent callback);
void RemoveOnMouseEvent(OnMouseEvent callback);
void TriggerMouseEvent(double x, double y);

//scroll events
typedef void (*OnScrollEvent)(double x, double y);
void NotifyOnScrollEvent(OnScrollEvent callback);
void RemoveOnScrollEvent(OnScrollEvent callback);
void TriggerScrollEvent(double x, double y);

//drop events
typedef void (*OnFileDrop)(std::vector<std::string>);
void NotifyOnFileDrop(OnFileDrop callback);
void RemoveOnFileDrop(OnFileDrop callback);
void TriggerFileDrop(std::vector<std::string> files);
} // namespace Input
} // namespace Core
} // namespace Tortuga

#endif