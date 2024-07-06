#include "camera.h"

Camera::Camera() : camera_active(CameraType::Front) {}

void Camera::setup_gui() {
    gui.setup();  // Initialise le panneau GUI
    parameters.setName("Camera Parameters");
    perspective_button.setup("Perspective");
    perspective_button.addListener(this, &Camera::perspective_button_pressed);
    gui.add(&perspective_button);
    frustum_button.setup("Frustum");
    frustum_button.addListener(this, &Camera::frustum_button_pressed);
    gui.add(&frustum_button);
    gui.add(renderFar.set("Render Distance", 2, 2, 500));
    gui.add(cameraSpeed.set("Camera Speed", 250, 0, 500));

    //boutons pour changer de camera entre choix "Front", "Back"
    frontCameraButton.setup("Front Camera");
    backCameraButton.setup("Back Camera");
    frontCameraButton.addListener(this, &Camera::button_came_front_pressed);
    backCameraButton.addListener(this, &Camera::button_came_back_pressed);
    gui.add(&frontCameraButton);
    gui.add(&backCameraButton);
    camera_active = CameraType::Front;

    // Définir les callbacks pour les paramètres
    perspective_button.addListener(this, &Camera::perspective_button_pressed);
    frustum_button.addListener(this, &Camera::frustum_button_pressed);
    renderFar.addListener(this, &Camera::onUpdateRenderFar);
    cameraSpeed.addListener(this, &Camera::onUpdateCameraSpeed);
}


void Camera::drawGui() {
    gui.draw();
}

void Camera::setup() {
    setup_gui();
    init_camera();
}

void Camera::begin() {
    camera->begin();
}

void Camera::end() {
    camera->end();
}

void Camera::update() {
    cam_update();
}

void Camera::reset() 
{
    cam_reset();
}

void Camera::cam_reset() {
    // initialisation des variables
    offset_camera = -20.0f;

    // position initiale de chaque caméra
    camera_front.setPosition(0.0f, 3.0f, -offset_camera);
    camera_back.setPosition(0.0f, 3.0f, -offset_camera);

    // orientation de chaque caméra
    camera_front.lookAt(ofVec3f(0.0f, 3.0f, 0.0f));
    camera_back.lookAt(ofVec3f(0.0f, 3.0f, 0.0f));


    // caméra par défaut
    camera_active = CameraType::Front;

    ofLog() << "<cam reset>";
}

void Camera::cam_draw_frustum()
{
    if (camera_active != CameraType::Front)
        camera_front.drawFrustum();
    if (camera_active != CameraType::Back)
        camera_back.drawFrustum();
}

void Camera::removeGui()
{
    perspective_button.removeListener(this, &Camera::perspective_button_pressed);
    frustum_button.removeListener(this, &Camera::frustum_button_pressed);
    renderFar.removeListener(this, &Camera::onUpdateRenderFar);
    cameraSpeed.removeListener(this, &Camera::onUpdateCameraSpeed);
}

void Camera::drawFrustum() {
    // Draw camera frustum for debugging or visualization
}

void Camera::switchPerspective() {
    // Switch between perspective modes (if applicable)
}

void Camera::init_camera() 
{
    // paramètres
    camera_position = ofVec3f{0.0f, 60.0f, 0.0f};
    camera_target = ofVec3f(0.0f, 60.0f, 0.0f); 
    camera_near = 1.0f;
    camera_far = 150.0f;    
    float verticalFieldOfView = PI / 3.0f;
    const float side = -2.0f * tan(verticalFieldOfView / 2.0f);
    ofLog() << "side: " << side;    
    camera_fov = 60.0f;
    camera_fov_delta = 16.0f;   
    speed_delta = 50.0f;   
    is_visible_frustum = false; 
    is_camera_move_left = false;
    is_camera_move_right = false;
    is_camera_move_up = false;
    is_camera_move_down = false;
    is_camera_move_forward = false;
    is_camera_move_backward = false;    
    is_camera_tilt_up = false;
    is_camera_tilt_down = false;
    is_camera_pan_left = false;
    is_camera_pan_right = false;
    is_camera_roll_left = false;
    is_camera_roll_right = false;   
    is_camera_fov_narrow = false;
    is_camera_fov_wide = false; 
    is_camera_perspective = true;
    switch (camera_active)
  {
    case CameraType::Front:
      camera = &camera_front;
      camera_name = "avant";
      break;

    case CameraType::Back:
      camera = &camera_back;
      camera_name = "arrière";
      //mettre position de la camera back 'a 1 et direction vers 0,0,0
      //simule le pinholecam de laddons ratracer si on la bouge pas au debut
      camera_back.setPosition(0.0f, 0.0f, 1.0f);
      camera_back.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
      break;

    //ajouter cam player with locked mouse

    default:
      break;
  }

  camera_position = camera->getPosition();
  camera_orientation = camera->getOrientationQuat();

  // mode de projection de la caméra
  if (is_camera_perspective)
  {
    camera->disableOrtho();
    camera->setupPerspective(false, camera_fov, camera_near, camera_far, ofVec2f(0, 0));
    camera_projection = "perspective";
  }
  else
  {
    camera->enableOrtho();
    camera_projection = "orthogonale";
  }

  camera->setPosition(camera_position);
  camera->setOrientation(camera_orientation);
}

void Camera::cam_update()
{
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;   
    speed_translation = speed_delta * time_elapsed * 0.06f;
    speed_rotation = speed_translation; 

    if (is_key_press_w)
        camera->dolly(-speed_translation);
    if (is_key_press_s)
        camera->dolly(speed_translation); 
    if (is_key_press_a)
        camera->truck(-speed_translation);
    if (is_key_press_d)
        camera->truck(speed_translation);   
    if (is_camera_move_up)
        camera->boom(speed_translation);
    if (is_camera_move_down)
        camera->boom(-speed_translation);   
    if (is_camera_tilt_up)
        camera->tiltDeg(-speed_rotation);
    if (is_camera_tilt_down)
        camera->tiltDeg(speed_rotation);
    if (is_camera_pan_left)
        camera->panDeg(speed_rotation);
    if (is_camera_pan_right)
        camera->panDeg(-speed_rotation);    
    if (is_camera_roll_left)
      camera->rollDeg(-speed_rotation);
    if (is_camera_roll_right)
      camera->rollDeg(speed_rotation);  
    if (is_camera_perspective)
    {
      if (is_camera_fov_narrow)
      {
        camera_fov = std::max(camera_fov -= camera_fov_delta * time_elapsed, 0.0f);
        camera->setFov(camera_fov);
      } 
      if (is_camera_fov_wide)
      {
        camera_fov = std::min(camera_fov += camera_fov_delta * time_elapsed, 180.0f);
        camera->setFov(camera_fov);
      }
    }
}

void Camera::perspective_button_pressed(){
    is_camera_perspective = !is_camera_perspective;

}

void Camera::frustum_button_pressed()
{
    is_visible_frustum = !is_visible_frustum;
}

void Camera::button_came_front_pressed() {
    camera_active = CameraType::Front;
}

void Camera::button_came_back_pressed() {
    camera_active = CameraType::Back;
}

void Camera::onUpdateRenderFar(float& far) {
    camera_far = far;
    camera->setFarClip(camera_far);
}

void Camera::onUpdateCameraSpeed(float& speed) {
    camera_fov_delta = speed;
}