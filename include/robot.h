

class DenavitHartenberg {
private:

public:
    float theta = 0.0f; // angle about previous z from old x to new x
    float d = 0.0f;     // offset along previous z to the common normal
    float r = 0.0f;     // length of the common normal 
    float alpha = 0.0f; // angle about common normal, from old z axis to new z axis

};
