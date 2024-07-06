#pragma once
#include "ofMain.h"
#include "ofxGui.h"

enum class CameraType { Front, Back };

class Camera {
public:
    Camera();

    void setup_gui();
    void drawGui();

    void setup();
    void begin();
    void end();
    void update();
    void reset();
    void removeGui();
    void drawFrustum();

    void switchPerspective();    
    bool is_visible_frustum;
    bool is_key_press_w;
    bool is_key_press_s;
    bool is_key_press_a;
    bool is_key_press_d;
    bool is_camera_tilt_up;
    bool is_camera_tilt_down;
    bool is_camera_pan_left;
    bool is_camera_pan_right;
    bool is_camera_roll_left;
    bool is_camera_roll_right;
    bool is_camera_fov_narrow;
    bool is_camera_fov_wide;
    bool is_camera_perspective;
    bool is_camera_move_up;
    bool is_camera_move_down;
    bool is_camera_move_forward;
    bool is_camera_move_backward;
    bool is_camera_move_left;
    bool is_camera_move_right;
private:
    void init_camera();
    void cam_update();
    void cam_reset();
    void cam_draw_frustum();

    CameraType camera_active;
    ofCamera camera_front;
    ofCamera camera_back;
    ofCamera* camera;

    ofQuaternion camera_orientation;
    ofVec3f camera_position;
    ofVec3f camera_target;
    string camera_name;
    string camera_projection;
    float camera_near;
    float camera_far;
    float camera_fov;
    float camera_fov_delta;
    float offset_camera;
    //camera control
    float speed_delta;
    float speed_translation;
    float speed_rotation;

    float time_current;
    float time_last;
    float time_elapsed;



    ofxPanel gui;
    ofParameterGroup parameters;
    ofxButton perspective_button;
    void perspective_button_pressed();
    ofxButton  frustum_button;
    void frustum_button_pressed();
    ofParameter<float> renderFar;
    ofParameter<float> cameraSpeed;
    //parametre de choix de cameras
    ofxButton frontCameraButton;
    ofxButton backCameraButton;

    void button_came_front_pressed();
    void button_came_back_pressed();


    void onToggleFrustum(bool& show);
    void onTogglePerspective(bool& perspective);
    void onUpdateRenderFar(float& far);
    void onUpdateCameraSpeed(float& speed);


};