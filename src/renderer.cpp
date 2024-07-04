#include "renderer.h"

void Renderer::setup()
{
  ofSetFrameRate(60);
  ofSetBackgroundColor(0);
  ofDisableArbTex();
  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSetSphereResolution(32);

  // chargement d'un modèle 3D
  teapot.loadModel("geometry/teapot.obj");

  // désactiver le matériau par défaut du modèle
  teapot.disableMaterials();

  // charger, compiler et linker les sources des shaders
  shader_color_fill.load("shaders/color_fill_330_vs.glsl","shaders/color_fill_330_fs.glsl");
  shader_color_fill_no_tex.load("shaders_no_tex/color_fill_no_tex_330_vs.glsl","shaders_no_tex/color_fill_no_tex_330_fs.glsl");
  shader_lambert.load("shaders/lambert_330_vs.glsl","shaders/lambert_330_fs.glsl");
  shader_lambert_no_tex.load("shaders_no_tex/lambert_no_tex_330_vs.glsl","shaders_no_tex/lambert_no_tex_330_fs.glsl");  
  shader_phong.load("shaders/phong_330_vs.glsl", "shaders/phong_330_fs.glsl");
  shader_phong_no_tex.load("shaders_no_tex/phong_no_tex_330_vs.glsl", "shaders_no_tex/phong_no_tex_330_fs.glsl");
  shader_blinn_phong.load("shaders/blinn_phong_330_vs.glsl", "shaders/blinn_phong_330_fs.glsl");
  shader_blinn_phong_no_tex.load("shaders_no_tex/blinn_phong_no_tex_330_vs.glsl", "shaders_no_tex/blinn_phong_no_tex_330_fs.glsl");
  shader_gouraud.load("shaders/gouraud_330_vs.glsl", "shaders/gouraud_330_fs.glsl");
  shader_gouraud_no_tex.load("shaders_no_tex/gouraud_no_tex_330_vs.glsl", "shaders_no_tex/gouraud_no_tex_330_fs.glsl");
  shader_goosh.load("shaders/goosh_330_vs.glsl", "shaders/goosh_330_fs.glsl");
  shader_goosh_no_tex.load("shaders_no_tex/goosh_no_tex_330_vs.glsl", "shaders_no_tex/goosh_no_tex_330_fs.glsl");
  shader_cel.load("shaders/cel_330_vs.glsl", "shaders/cel_330_fs.glsl");
  shader_cel_no_tex.load("shaders_no_tex/cel_no_tex_330_vs.glsl", "shaders_no_tex/cel_no_tex_330_fs.glsl");
  shader_normal.load("shaders/normal_330_vs.glsl", "shaders/normal_330_fs.glsl");
  shader_normal_no_tex.load("shaders_no_tex/normal_no_tex_330_vs.glsl", "shaders_no_tex/normal_no_tex_330_fs.glsl");
  shader_pbr.load("shaders/pbr_330_vs.glsl","shaders/pbr_330_fs.glsl");
  shader_pbr_no_tex.load("shaders_no_tex/pbr_no_tex_330_vs.glsl", "shaders_no_tex/pbr_no_tex_330_fs.glsl");

  // charger les textures du matériau
  texture_diffuse.load("texture/diffuse.jpg");
  texture_metallic.load("texture/metal.jpg");
  texture_roughness.load("texture/rough.jpg");
  texture_occlusion.load("texture/ao.jpg");
  texture_normal.load("texture/normal.jpg");
  texture_displacement.load("texture/displacement.jpg");

  // paramètres des textures du matériau
  texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_normal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

  perturb_normal = true;
  parallax_active = false;

  // initialiser les paramètres
  shader_active = ShaderType::PHONG;
  texture_active = TextureType::TEXTURE;
}

void Renderer::reset()
{
  // paramètres de transformation
  speed_motion = 150.0f;
  speed_rotation = 50.0f;
  initial_x = 0.0f;
  initial_z = -100.0f;
  rotation_y = 0.0f;
  scale_cube = 110.0f;
  scale_sphere = 90.0f;
  scale_teapot = 0.618f;

  // positions initiales des maillages géométriques
  position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
  position_sphere.set(0.0f, 0.0f, 0.0f);
  position_teapot.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);

  // paramètres du matériau
  material_color_ambient = ofColor(0.1f * 255, 0.1f * 255, 0.1f * 255); // ofColor(26, 26, 26);
  material_color_diffuse = ofColor(0.8f * 255, 0.8f * 255, 0.8f * 255); // ofColor(204, 204, 204);
  material_color_specular = ofColor(0.5f * 255, 0.5f * 255, 0.0f * 255); // ofColor(127, 127, 127);

  material_metallic = 0.5f;
  material_roughness = 0.5f;
  material_occlusion = 1.0f;
  material_brightness = 1.0f;

  material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

  // paramètres de la lumière
  light_color = ofColor(255, 255, 255);
  light_intensity = 1.0f;
  light_motion = true;

  // paramètres de mappage tonal
  tone_mapping_exposure = 1.0f;
  tone_mapping_toggle = true;

  // initialisation des variables
  delta_x = speed_motion;
  delta_y = speed_rotation;
  delta_z = speed_motion;

  // position initiale de la caméra
  offset_x = initial_x;
  offset_z = initial_z;

  ofLog() << "<reset>";
}

void Renderer::update()
{
  // centre de la fenêtre d'affichage
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;

  if (light_motion)
  {
    // transformer la lumière
    light.setGlobalPosition(
      ofMap(ofGetMouseX() / (float) ofGetWidth(), 0.0f, 1.0f, -center_x, center_x),
      ofMap(ofGetMouseY() / (float) ofGetHeight(), 0.0f, 1.0f, -center_y, center_y),
      -offset_z * 1.0f);
  }
  switch_shader_texture();
  update_shader_type();
}

void Renderer::draw()
{
  // activer l'occlusion en profondeur
  ofEnableDepthTest();

  // activer l'éclairage dynamique
  ofEnableLighting();

  // activer la lumière dynamique
  light.enable();

  // activer le shader
  shader->begin();
  //apply_uniform_by_type();

  ofPushMatrix();

    // transformer l'origine de la scène au milieu de la fenêtre d'affichage
    ofTranslate(center_x + offset_x, center_y, offset_z);

    // rotation globale
    ofRotateDeg(rotation_y, 0.0f, 1.0f, 0.0f);

    ofPushMatrix();

      // positionnner le cube
      ofTranslate(
        position_cube.x,
        position_cube.y,
        position_cube.z);

      // rotation locale
      ofRotateDeg(45.0f, 1.0f, 0.0f, 0.0f);

      // dessiner un cube
      ofDrawBox(0.0f, 0.0f, 0.0f, scale_cube);

    ofPopMatrix();

    ofPushMatrix();

      // positionner la sphère
      ofTranslate(
        position_sphere.x,
        position_sphere.y,
        position_sphere.z);

      // dessiner une sphère
      ofDrawSphere(0.0f, 0.0f, 0.0f, scale_sphere);

    ofPopMatrix();

    ofPushMatrix();

      // positionner le teapot
      teapot.setPosition(
        position_teapot.x + 25.0f,
        position_teapot.y + 15.0f,
        position_teapot.z);

      // dimension du teapot
      teapot.setScale(
        scale_teapot,
        scale_teapot,
        scale_teapot);

      // dessiner un teapot
      teapot.draw(OF_MESH_FILL);

    ofPopMatrix();

  ofPopMatrix();

  // désactiver le shader
  shader->end();

  // désactiver la lumière
  light.disable();

  // désactiver l'éclairage dynamique
  ofDisableLighting();

  // désactiver l'occlusion en profondeur
  ofDisableDepthTest();
}



//fonction wui switch entre les shaders
void Renderer::update_shader_type()
{
    switch (shader_active)
    {
    case ShaderType::COLOR_FILL:
      shader_name = "Color Fill";
      shader = &shader_color_fill;
      shader->begin();
      shader->setUniform3f("color", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->end();
      break;

    case ShaderType::COLOR_FILL_NO_TEX:
      shader_name = "Color Fill (No Texture)";
      shader = &shader_color_fill_no_tex;
      shader->begin();
      shader->setUniform3f("color", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->end();
      break;

    case ShaderType::LAMBERT:
      shader_name = "Lambert";
      shader = &shader_lambert;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::LAMBERT_NO_TEX:
      shader_name = "Lambert (No Texture)";
      shader = &shader_lambert_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::PHONG:
      shader_name = "Phong";
      shader = &shader_phong;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->end();
      break;

    case ShaderType::PHONG_NO_TEX:
    shader_name = "Phong (No Texture)";
    shader = &shader_phong_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::BLINN_PHONG:
      shader_name = "Blinn-Phong";
      shader = &shader_blinn_phong;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->setUniform1i("parallax_active", parallax_active);
      
      shader->end();
      break;

    case ShaderType::BLINN_PHONG_NO_TEX:
    shader_name = "Blinn-Phong (No Texture)";
    shader = &shader_blinn_phong_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::GOURAUD:
      shader_name = "Gouraud";
      shader = &shader_gouraud;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::GOURAUD_NO_TEX:
      shader_name = "Gouraud (No Texture)";
      shader = &shader_gouraud_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::GOOSH:
      shader_name = "Goosh";
      shader = &shader_goosh;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::GOOSH_NO_TEX:
      shader_name = "Goosh (No Texture)";
      shader = &shader_goosh_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::CEL:
      shader_name = "Cel";
      shader = &shader_cel;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 3);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::CEL_NO_TEX:
      shader_name = "Cel (No Texture)";
      shader = &shader_cel_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->end();
      break;

    case ShaderType::NORMAL:
      shader_name = "Normal";
      shader = &shader_normal;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 2);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::NORMAL_NO_TEX:
      shader_name = "Normal (No Texture)";
      shader = &shader_normal_no_tex;
      shader->begin();
      shader->setUniform3f("color_ambient",  material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("color_diffuse",  material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("brightness", material_brightness);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 1);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::PBR:
      shader_name = "Physically-Based Rendering";
      shader = &shader_pbr;
      shader->begin();
      shader->setUniform3f("material_color_ambient", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("material_color_diffuse", material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("material_color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("material_metallic", material_metallic);
      shader->setUniform1f("material_roughness", material_roughness);
      shader->setUniform1f("material_occlusion", material_occlusion);
      shader->setUniform1f("material_brightness", material_brightness);
      shader->setUniform3f("material_fresnel_ior", material_fresnel_ior);
      shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
      shader->setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
      shader->setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
      shader->setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);
      shader->setUniformTexture("texture_normal", texture_normal.getTexture(), 5);
      shader->setUniformTexture("texture_displacement", texture_displacement.getTexture(), 6);
      shader->setUniform1f("light_intensity", light_intensity);
      shader->setUniform3f("light_color", light_color.r / 255.0f, light_color.g / 255.0f, light_color.b / 255.0f);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
      shader->setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
      shader->setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
      shader->setUniform1i("parallax_active", parallax_active);
      shader->setUniform1i("perturb_normal", perturb_normal);
      shader->end();
      break;

    case ShaderType::PBR_NO_TEX:
    shader_name = "Physically-Based Rendering (No Texture)";
    shader = &shader_pbr_no_tex;
    shader->begin();
      shader->setUniform3f("material_color_ambient", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
      shader->setUniform3f("material_color_diffuse", material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
      shader->setUniform3f("material_color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);
      shader->setUniform1f("material_metallic", material_metallic);
      shader->setUniform1f("material_roughness", material_roughness);
      shader->setUniform1f("material_occlusion", material_occlusion);
      shader->setUniform1f("material_brightness", material_brightness);
      shader->setUniform3f("material_fresnel_ior", material_fresnel_ior);
      shader->setUniform1f("light_intensity", light_intensity);
      shader->setUniform3f("light_color", light_color.r / 255.0f, light_color.g / 255.0f, light_color.b / 255.0f);
      shader->setUniform3f("light_position", light.getGlobalPosition());
      shader->setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
      shader->setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
      shader->setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
      shader->end();
      break;
    }
}

void Renderer::update_shader_type_no_tex()
{
    switch (shader_active)
    {
    case ShaderType::COLOR_FILL:
      shader_active = ShaderType::COLOR_FILL_NO_TEX;
      break;

    case ShaderType::COLOR_FILL_NO_TEX:
      shader_active = ShaderType::COLOR_FILL_NO_TEX;
      break;

    case ShaderType::LAMBERT:
      shader_active = ShaderType::LAMBERT_NO_TEX;
      break;

    case ShaderType::LAMBERT_NO_TEX:
      shader_active = ShaderType::LAMBERT_NO_TEX;
      break;

    case ShaderType::PHONG:
      shader_active = ShaderType::PHONG_NO_TEX;
      break;

    case ShaderType::PHONG_NO_TEX:
      shader_active = ShaderType::PHONG_NO_TEX;
      break;
    
    case ShaderType::BLINN_PHONG:
      shader_active = ShaderType::BLINN_PHONG_NO_TEX;
      break;

    case ShaderType::BLINN_PHONG_NO_TEX:
      shader_active = ShaderType::BLINN_PHONG_NO_TEX;
      break;  

    case ShaderType::GOURAUD:
      shader_active = ShaderType::GOURAUD_NO_TEX;
      break;

    case ShaderType::GOURAUD_NO_TEX:
      shader_active = ShaderType::GOURAUD_NO_TEX;
      break;

    case ShaderType::GOOSH:
      shader_active = ShaderType::GOOSH_NO_TEX;
      break;

    case ShaderType::GOOSH_NO_TEX:
      shader_active = ShaderType::GOOSH_NO_TEX;
      break;

    case ShaderType::CEL:
      shader_active = ShaderType::CEL_NO_TEX;
      break;

    case ShaderType::CEL_NO_TEX:
      shader_active = ShaderType::CEL_NO_TEX;
      break;

    case ShaderType::NORMAL:
      shader_active = ShaderType::NORMAL_NO_TEX;
      break;
    
    case ShaderType::NORMAL_NO_TEX:
      shader_active = ShaderType::NORMAL_NO_TEX;
      break;

    case ShaderType::PBR:
      shader_active = ShaderType::PBR_NO_TEX;
      break;

    case ShaderType::PBR_NO_TEX:
      shader_active = ShaderType::PBR_NO_TEX;
      break;
    }
}

void Renderer::update_shader_type_texture()
{
    switch (shader_active)
    {
    case ShaderType::COLOR_FILL:
      shader_active = ShaderType::COLOR_FILL;
      break;

    case ShaderType::COLOR_FILL_NO_TEX:
      shader_active = ShaderType::COLOR_FILL;
      break;

    case ShaderType::LAMBERT:
      shader_active = ShaderType::LAMBERT;
      break;

    case ShaderType::LAMBERT_NO_TEX:
      shader_active = ShaderType::LAMBERT;
      break;

    case ShaderType::PHONG:
      shader_active = ShaderType::PHONG;
      break;

    case ShaderType::PHONG_NO_TEX:
      shader_active = ShaderType::PHONG;
      break;

    case ShaderType::BLINN_PHONG:
      shader_active = ShaderType::BLINN_PHONG;
      break;

    case ShaderType::BLINN_PHONG_NO_TEX:
      shader_active = ShaderType::BLINN_PHONG;
      break;

    case ShaderType::GOURAUD:
      shader_active = ShaderType::GOURAUD;
      break;

    case ShaderType::GOURAUD_NO_TEX:
      shader_active = ShaderType::GOURAUD;
      break;

    case ShaderType::GOOSH:
      shader_active = ShaderType::GOOSH;
      break;

    case ShaderType::GOOSH_NO_TEX:
      shader_active = ShaderType::GOOSH;
      break;

    case ShaderType::CEL:
      shader_active = ShaderType::CEL;
      break;

    case ShaderType::CEL_NO_TEX:
      shader_active = ShaderType::CEL;
      break;

    case ShaderType::NORMAL:
      shader_active = ShaderType::NORMAL;
      break;
    
    case ShaderType::NORMAL_NO_TEX:
      shader_active = ShaderType::NORMAL;
      break;

    case ShaderType::PBR:
      shader_active = ShaderType::PBR;
      break;

    case ShaderType::PBR_NO_TEX:
      shader_active = ShaderType::PBR;
      break;
    }
}

//fonction switch entre les shaders pour texutre ou non avec break
void Renderer::switch_shader_texture()
{
    switch (texture_active)
    {
    case TextureType::NO_TEXTURE:
      update_shader_type_no_tex();
      break;

    case TextureType::TEXTURE:
      update_shader_type_texture();
      break;
    }
}