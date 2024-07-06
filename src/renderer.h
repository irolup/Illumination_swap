#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

//struct for sdhader type
enum ShaderType
{
  COLOR_FILL,
  COLOR_FILL_NO_TEX,
  LAMBERT,
  LAMBERT_NO_TEX,
  PHONG,
  PHONG_NO_TEX,
  BLINN_PHONG,
  BLINN_PHONG_NO_TEX,
  GOURAUD,
  GOURAUD_NO_TEX,
  GOOSH,
  GOOSH_NO_TEX,
  CEL,
  CEL_NO_TEX,
  NORMAL,
  NORMAL_NO_TEX,
  PBR,
  PBR_NO_TEX
};

//enum tex no tex
enum TextureType
{
  NO_TEXTURE,
  TEXTURE
};

class Renderer
{
public: 

  ShaderType shader_active;

  ofShader shader_color_fill;
  ofShader shader_color_fill_no_tex;
  ofShader shader_lambert;
  ofShader shader_lambert_no_tex;
  ofShader shader_phong;
  ofShader shader_phong_no_tex;
  ofShader shader_blinn_phong;
  ofShader shader_blinn_phong_no_tex;
  ofShader shader_gouraud;
  ofShader shader_gouraud_no_tex;
  ofShader shader_goosh;
  ofShader shader_goosh_no_tex;
  ofShader shader_cel;
  ofShader shader_cel_no_tex;
  ofShader shader_normal;
  ofShader shader_normal_no_tex;
  ofShader shader_pbr;
  ofShader shader_pbr_no_tex;

  ofShader* shader;

  string shader_name;

  TextureType texture_active;
  
  //update shader type
  void update_shader_type();
  void update_shader_type_no_tex();
  void update_shader_type_texture();
  void switch_shader_texture();

  ofColor material_color_ambient;
  ofColor material_color_diffuse;
  ofColor material_color_specular;    
  float material_metallic;
  float material_roughness;
  float material_occlusion;
  float material_brightness;  
  glm::vec3 material_fresnel_ior; 
  ofImage texture_diffuse;
  ofImage texture_metallic;
  ofImage texture_roughness;
  ofImage texture_occlusion;
  ofImage texture_normal;
  ofImage texture_displacement;
  bool perturb_normal;
  bool parallax_active;  
  ofLight light;
  ofColor light_color;
  float light_intensity;
  bool light_motion;  
  float tone_mapping_exposure;
  float tone_mapping_gamma;
  bool tone_mapping_toggle;   
  ofxAssimpModelLoader teapot;    
  ofVec3f position_cube;
  ofVec3f position_sphere;
  ofVec3f position_teapot;    
  float scale_cube;
  float scale_sphere;
  float scale_teapot; 
  float speed_motion;
  float speed_rotation;   
  float offset_x;
  float offset_z; 
  float delta_x;
  float delta_y;
  float delta_z;  
  float rotation_y;   
  float initial_x;
  float initial_z;    
  float center_x;
  float center_y;


  //primitives
  ofBoxPrimitive box;
  ofSpherePrimitive sphere;

  void setup();
  void update();
  void draw();
  void reset();
};