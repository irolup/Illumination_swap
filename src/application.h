#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "renderer.h"

class Application : public ofBaseApp
{
  Renderer renderer;

  ofxPanel gui;

  ofxGuiGroup group_material_color;
  ofxGuiGroup group_material_factor;
  ofxGuiGroup group_light;
  ofxGuiGroup group_tone_mapping;
  ofxGuiGroup group_shader_type;

  ofParameter<ofColor> color_picker_ambient;
  ofParameter<ofColor> color_picker_diffuse;
  ofParameter<ofColor> color_picker_specular;

  ofParameter<float> slider_metallic;
  ofParameter<float> slider_roughness;
  ofParameter<float> slider_occlusion;
  ofParameter<float> slider_brightness;

  ofParameter<glm::vec3> slider_fresnel_ior;

  ofParameter<ofColor> color_picker_light_color;

  ofParameter<float> slider_light_intensity;

  ofParameter<float> slider_exposure;
  ofParameter<float> slider_gamma;
  ofParameter<bool> toggle_tone_mapping;

  ofParameter<bool> toggle_ui;
  ofParameter<bool> toggle_light_motion;

  ofxButton button_reset;

  ofxButton button_color_fill;
  ofxButton button_lambert;
  ofxButton button_phong;
  ofxButton button_blinn_phong;
  ofxButton button_gouraud;
  ofxButton button_goosh;
  ofxButton button_cel;
  ofxButton button_pbr;
  ofxButton button_texture;
  ofxButton button_no_texture;
  void button_color_fill_pressed();
  void button_lambert_pressed();
  void button_phong_pressed();
  void button_blinn_phong_pressed();
  void button_gouraud_pressed();
  void button_goosh_pressed();
  void button_cel_pressed();
  void button_pbr_pressed();
  void button_texture_pressed();
  void button_no_texture_pressed();


  float time_current;
  float time_last;
  float time_elapsed;

  bool is_key_press_up;
  bool is_key_press_down;
  bool is_key_press_left;
  bool is_key_press_right;

  bool is_key_press_w;
  bool is_key_press_a;
  bool is_key_press_s;
  bool is_key_press_d;
  bool is_key_press_q;
  bool is_key_press_e;

  void setup();
  void reset();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);

  void button_reset_pressed();

  void exit();
};