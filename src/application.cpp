#include "application.h"

void Application::setup()
{
    ofLog() << "<app::setup>";  
    ofSetWindowTitle("Illumination models (↑↓←→ q e m u r)"); 
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;
    is_key_press_q = false;
    is_key_press_e = false; 
    toggle_ui = true;   
    renderer.setup();   
    reset();    
    gui.setup("Shaders & Textures");  
    // interface pour les couleurs du matériau
    group_material_color.setup("color");    
    group_material_color.add(color_picker_ambient);
    group_material_color.add(color_picker_diffuse);
    group_material_color.add(color_picker_specular);    
    gui.add(&group_material_color); 
    // interface pour les facteurs numériques du matériau
    group_material_factor.setup("factor");  
    group_material_factor.add(slider_metallic);
    group_material_factor.add(slider_roughness);
    group_material_factor.add(slider_occlusion);
    group_material_factor.add(slider_brightness);   
    group_material_factor.add(slider_fresnel_ior);  
    gui.add(&group_material_factor);    
    // interface pour les paramètres de la lumière
    group_light.setup("light"); 
    group_light.add(color_picker_light_color);
    group_light.add(slider_light_intensity);
    group_light.add(toggle_light_motion);   
    gui.add(&group_light);  
    // interface pour les paramètres de mappage tonal
    group_tone_mapping.setup("tone mapping");   
    group_tone_mapping.add(slider_exposure);
    group_tone_mapping.add(slider_gamma);
    group_tone_mapping.add(toggle_tone_mapping);    
    gui.add(&group_tone_mapping);   
    // interface pour les options de l'application
    toggle_ui.setName("hide ui");
    gui.add(toggle_ui); 
    button_reset.setup("reset");
    button_reset.addListener(this, &Application::button_reset_pressed);
    gui.add(&button_reset); 
    //shader switching buttons
    group_shader_type.setup("Shader type");
    group_shader_type.add(button_color_fill.setup("color fill"));
    button_color_fill.addListener(this, &Application::button_color_fill_pressed);
    group_shader_type.add(button_lambert.setup("lambert"));
    button_lambert.addListener(this, &Application::button_lambert_pressed);
    group_shader_type.add(button_phong.setup("phong"));
    button_phong.addListener(this, &Application::button_phong_pressed);
    group_shader_type.add(button_blinn_phong.setup("blinn-phong"));
    button_blinn_phong.addListener(this, &Application::button_blinn_phong_pressed);
    group_shader_type.add(button_gouraud.setup("gouraud"));
    button_gouraud.addListener(this, &Application::button_gouraud_pressed);
    group_shader_type.add(button_goosh.setup("goosh"));
    button_goosh.addListener(this, &Application::button_goosh_pressed);
    group_shader_type.add(button_cel.setup("cel"));
    button_cel.addListener(this, &Application::button_cel_pressed);
    group_shader_type.add(button_normal.setup("normal"));
    button_normal.addListener(this, &Application::button_normal_pressed);
    group_shader_type.add(button_pbr.setup("pbr"));
    button_pbr.addListener(this, &Application::button_pbr_pressed);
    group_shader_type.add(button_texture.setup("texture"));
    button_texture.addListener(this, &Application::button_texture_pressed);
    group_shader_type.add(button_no_texture.setup("no texture"));
    button_no_texture.addListener(this, &Application::button_no_texture_pressed);
    group_shader_type.add(button_perturb_normal.setup("perturb normal"));
    button_perturb_normal.addListener(this, &Application::button_perturb_normal_pressed);
    group_shader_type.add(button_parallax.setup("Parallax"));
    button_parallax.addListener(this, &Application::button_parallax_pressed);
    gui.add(&group_shader_type);
    
    camera.setup();

}

void Application::reset()
{
    camera.reset();
    renderer.reset();   
    color_picker_ambient.set("ambient", renderer.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
    color_picker_diffuse.set("diffuse", renderer.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
    color_picker_specular.set("specular", renderer.material_color_specular, ofColor(0, 0), ofColor(255, 255));  
    slider_metallic.set("metallic", renderer.material_metallic, 0.0f, 1.0f);
    slider_roughness.set("roughness", renderer.material_roughness, 0.0f, 1.0f);
    slider_occlusion.set("occlusion", renderer.material_occlusion, 0.0f, 5.0f);
    slider_brightness.set("brightness", renderer.material_brightness, 0.0f, 5.0f);  
    slider_fresnel_ior.set("fresnel ior", renderer.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f)); 
    color_picker_light_color.set("color", renderer.light_color, ofColor(0, 0), ofColor(255, 255));
    slider_light_intensity.set("intensity", renderer.light_intensity, 0.0f, 10.0f); 
    toggle_light_motion.set("motion", renderer.light_motion);   
    slider_exposure.set("exposure", 1.0f, 0.0f, 5.0f);
    slider_gamma.set("gamma", 2.2f, 0.0f, 5.0f);    
    if (renderer.tone_mapping_toggle)
      toggle_tone_mapping.set("aces filmic", true);
    else
      toggle_tone_mapping.set("reinhard", false);
}

void Application::update_key_press()
{
    if (is_key_press_up)
    {
      renderer.offset_z += renderer.delta_z * time_elapsed;
    }
    if (is_key_press_down)
    {
      renderer.offset_z -= renderer.delta_z * time_elapsed;
    }
    if (is_key_press_left)
    {
      renderer.offset_x += renderer.delta_x * time_elapsed;
    }
    if (is_key_press_right)
    {
      renderer.offset_x -= renderer.delta_x * time_elapsed;
    }
    if (is_key_press_q)
    {
      renderer.rotation_y += renderer.delta_y * time_elapsed;
    }
    if (is_key_press_e)
    {
      renderer.rotation_y -= renderer.delta_y * time_elapsed;
    }
}

void Application::update()
{
    camera.update();
    ofSetWindowTitle("modèle d'illumination : " + renderer.shader_name + " (1-5 ↑↓←→ r)");
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    //update_key_press();
   
    renderer.material_color_ambient = color_picker_ambient;
    renderer.material_color_diffuse = color_picker_diffuse;
    renderer.material_color_specular = color_picker_specular;   
    renderer.material_metallic = slider_metallic;
    renderer.material_roughness = slider_roughness;
    renderer.material_occlusion = slider_occlusion;
    renderer.material_brightness = slider_brightness;   
    renderer.material_fresnel_ior = slider_fresnel_ior; 
    renderer.light_color = color_picker_light_color;
    renderer.light_intensity = slider_light_intensity;
    renderer.light_motion = toggle_light_motion;    
    renderer.tone_mapping_exposure = slider_exposure;
    renderer.tone_mapping_gamma = slider_gamma;
    renderer.tone_mapping_toggle = toggle_tone_mapping; 
    if (renderer.tone_mapping_toggle)
      toggle_tone_mapping.set("aces filmic", true);
    else
      toggle_tone_mapping.set("reinhard", false);   
    renderer.update();
}

void Application::draw()
{
  camera.begin();
  renderer.draw();
  camera.end();

  if (toggle_ui){
    gui.draw();
    camera.drawGui();
    }
}

void Application::keyPressed(int key)
{
  switch (key)
  {
    case 97:  // touche a
      is_key_press_left = true;
      camera.is_key_press_a = true;
      break;

    case 100: // touche d
      is_key_press_right = true;
      camera.is_key_press_d = true;
      break;

    case 101: // touche q
      is_key_press_q = true;
      break;

    case 113: // touche e
      is_key_press_e = true;
      break;

    case 115: // touche s
      is_key_press_down = true;
      camera.is_key_press_s = true;
      break;

    case 119: // touche w
      is_key_press_up = true;
      camera.is_key_press_w = true;
      break;

    case OF_KEY_LEFT:  // touche ←
      is_key_press_left = true;
      break;

    case OF_KEY_UP:    // touche ↑
      is_key_press_up = true;
      break;

    case OF_KEY_RIGHT: // touche →
      is_key_press_right = true;
      break;

    case OF_KEY_DOWN:  // touche ↓
      is_key_press_down = true;
      break;

    default:
      break;
  }
}

void Application::keyReleased(int key)
{
  switch (key)
  {
    case 97:  // touche a
      is_key_press_left = false;
      camera.is_key_press_a = false;
      break;

    case 100: // touche d
      is_key_press_right = false;
      camera.is_key_press_d = false;
      break;

    case 101: // touche q
      is_key_press_q = false;
      break;

    case 109: // touche m
      toggle_light_motion = !toggle_light_motion;
      ofLog() << "<toggle light motion: " << toggle_light_motion << ">";
      break;

    case 113: // touche e
      is_key_press_e = false;
      break;

    case 114: // touche r
      reset();
      ofLog() << "<reset renderer>";
      break;

    case 115: // touche s
      is_key_press_down = false;
      camera.is_key_press_s = false;
      break;

    case 117: // touche u
      toggle_ui = !toggle_ui;
      ofLog() << "<toggle ui: " << toggle_ui << ">";
      break;

    case 119: // touche w
      is_key_press_up = false;
      camera.is_key_press_w = false;
      break;

    case OF_KEY_LEFT:  // touche ←
      is_key_press_left = false;
      break;

    case OF_KEY_UP:    // touche ↑
      is_key_press_up = false;
      break;

    case OF_KEY_RIGHT: // touche →
      is_key_press_right = false;
      break;

    case OF_KEY_DOWN:  // touche ↓
      is_key_press_down = false;
      break;

    default:
      break;
  }
}

void Application::mouseReleased(int x, int y, int button){}

void Application::windowResized(int w, int h)
{
  ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
}

void Application::button_reset_pressed()
{
  reset();
}

void Application::button_color_fill_pressed()
{
    renderer.shader_active = ShaderType::COLOR_FILL;
    ofLog() << "<button color fill pressed>";
}

void Application::button_lambert_pressed()
{
    renderer.shader_active = ShaderType::LAMBERT;
    ofLog() << "<button lambert pressed>";
}


void Application::button_phong_pressed()
{
    renderer.shader_active = ShaderType::PHONG;
    ofLog() << "<button phong pressed>";
}

void Application::button_blinn_phong_pressed()
{
    renderer.shader_active = ShaderType::BLINN_PHONG;
    ofLog() << "<button blinn-phong pressed>";
}

void Application::button_gouraud_pressed()
{
    renderer.shader_active = ShaderType::GOURAUD;
    ofLog() << "<button gouraud pressed>";
}

void Application::button_goosh_pressed()
{
    renderer.shader_active = ShaderType::GOOSH;
    ofLog() << "<button goosh pressed>";
}

void Application::button_cel_pressed()
{
    renderer.shader_active = ShaderType::CEL;
    ofLog() << "<button cel pressed>";
}

void Application::button_normal_pressed()
{
    renderer.shader_active = ShaderType::NORMAL;
    ofLog() << "<button normal pressed>";
}

void Application::button_pbr_pressed()
{
    renderer.shader_active = ShaderType::PBR;
    ofLog() << "<button pbr pressed>";
}

void Application::button_texture_pressed()
{
    renderer.texture_active = TextureType::TEXTURE;
    ofLog() << "<button texture pressed>";
}

void Application::button_no_texture_pressed()
{
    renderer.texture_active = TextureType::NO_TEXTURE;
    ofLog() << "<button no texture pressed>";
}

void Application::button_perturb_normal_pressed()
{
    renderer.perturb_normal = !renderer.perturb_normal;
    ofLog() << "<button perturb normal is " << renderer.perturb_normal << ">";
}

void Application::button_parallax_pressed()
{
  renderer.parallax_active = !renderer.parallax_active;
  ofLog() << "<button perturb normal is " << renderer.parallax_active << ">";
}

void Application::exit()
{
    button_reset.removeListener(this, &Application::button_reset_pressed);
    button_color_fill.removeListener(this, &Application::button_color_fill_pressed);
    button_lambert.removeListener(this, &Application::button_lambert_pressed);
    button_phong.removeListener(this, &Application::button_phong_pressed);
    button_blinn_phong.removeListener(this, &Application::button_blinn_phong_pressed);
    button_gouraud.removeListener(this, &Application::button_gouraud_pressed);
    button_goosh.removeListener(this, &Application::button_goosh_pressed);
    button_cel.removeListener(this, &Application::button_cel_pressed);
    button_normal.removeListener(this, &Application::button_normal_pressed);
    button_pbr.removeListener(this, &Application::button_pbr_pressed);
    button_texture.removeListener(this, &Application::button_texture_pressed);
    button_no_texture.removeListener(this, &Application::button_no_texture_pressed);
    ofLog() << "<app::exit>";
}