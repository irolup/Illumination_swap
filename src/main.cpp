#include "ofMain.h"
#include "application.h"

int main()
{
  ofGLFWWindowSettings window_settings;
  window_settings.setSize(1024, 1080);
  window_settings.setGLVersion(3, 3);
  window_settings.numSamples = 4;
  window_settings.doubleBuffering = true;
  ofCreateWindow(window_settings);
  ofRunApp(new Application());
}