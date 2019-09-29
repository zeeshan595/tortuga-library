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

//key events
typedef void (*OnKeyEvent)(KeyCode::KeyCode, KeyAction::KeyAction);
void NotifyOnKeyEvent(OnKeyEvent callback);
void RemoveOnKeyEvent(OnKeyEvent callback);
void TriggerKeyEvent(KeyCode::KeyCode key, KeyAction::KeyAction action);

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