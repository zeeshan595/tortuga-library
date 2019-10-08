#include "./Input.hpp"

namespace Tortuga
{
namespace Core
{
namespace Input
{
//window close
std::vector<OnWindowClose> windowClose;
void NotifyOnWindowClose(OnWindowClose callback)
{
  windowClose.push_back(callback);
}
void RemoveOnWindowClose(OnWindowClose callback)
{
  for (auto i = windowClose.begin(); i != windowClose.end(); ++i)
  {
    if ((*i) == callback)
    {
      windowClose.erase(i);
      break;
    }
  }
}
void TriggerWindowClose()
{
  for (const auto cb : windowClose)
    cb();
}

//key events
std::vector<OnKeyEvent> keyEvents;
void NotifyOnKeyEvent(OnKeyEvent callback)
{
  keyEvents.push_back(callback);
}
void RemoveOnKeyEvent(OnKeyEvent callback)
{
  for (auto i = keyEvents.begin(); i != keyEvents.end(); ++i)
  {
    if ((*i) == callback)
    {
      keyEvents.erase(i);
      break;
    }
  }
}
void TriggerKeyEvent(KeyCode::KeyCode key, KeyAction::KeyAction action)
{
  for (const auto e : keyEvents)
    e(key, action);
}

//mouse events
std::vector<OnMouseEvent> mouseEvents;
void NotifyOnMouseEvent(OnMouseEvent callback)
{
  mouseEvents.push_back(callback);
}
void RemoveOnMouseEvent(OnMouseEvent callback)
{
  for (auto i = mouseEvents.begin(); i != mouseEvents.end(); ++i)
  {
    if ((*i) == callback)
    {
      mouseEvents.erase(i);
      break;
    }
  }
}
void TriggerMouseEvent(double x, double y)
{
  for (const auto e : mouseEvents)
    e(x, y);
}

//scroll events
std::vector<OnScrollEvent> scrollEvents;
void NotifyOnScrollEvent(OnScrollEvent callback)
{
  scrollEvents.push_back(callback);
}
void RemoveOnScrollEvent(OnScrollEvent callback)
{
  for (auto i = scrollEvents.begin(); i != scrollEvents.end(); ++i)
  {
    if ((*i) == callback)
    {
      scrollEvents.erase(i);
      break;
    }
  }
}
void TriggerScrollEvent(double x, double y)
{
  for (const auto e : scrollEvents)
    e(x, y);
}

//drop events
std::vector<OnFileDrop> fileDrops;
void NotifyOnFileDrop(OnFileDrop callback)
{
  fileDrops.push_back(callback);
}
void RemoveOnFileDrop(OnFileDrop callback)
{
  for (auto i = fileDrops.begin(); i != fileDrops.end(); ++i)
  {
    if ((*i) == callback)
    {
      fileDrops.erase(i);
      break;
    }
  }
}
void TriggerFileDrop(std::vector<std::string> files)
{
  for (const auto e : fileDrops)
    e(files);
}
} // namespace Input
} // namespace Core
} // namespace Tortuga