#ifndef CAMERA_H
#define CAMERA_H

class Camera{
    public:
    float position[3];
    float up[3], right[3], forward[3];
    float yaw, pitch;
    
    //x = horitontal
    //y = vertical
    //z = depth
    
    //Projection settings
    float fov, aspectRatio, nearPlane, farPlane;
    
    Camera(float px, float py, float pz, float yaw = -90.0f, float pitch = 0.0f);
    ~Camera();

    private:
    void updateVectors();
};

#endif