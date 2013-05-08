//
//  vec2.h
//  CS315_XCode_4.5_template
//
//  Created by Matthew Sauder on 2013-01-30. vec.h from template project used as reference.
//  Copyright (c) 2013 CS 315. All rights reserved.
//

#ifndef CS315_XCode_4_5_template_vec2_h
#define CS315_XCode_4_5_template_vec2_h

class vec2 {
public:
    // Data members
    GLfloat x;
    GLfloat y;
    
    // Default constructor
    vec2() {
        x = 0; y = 0;
    }
    
    // Constructor with initial values
    vec2 (GLfloat a, GLfloat b) {
        x = a; y = b;
    }
    
    // Copy constructor
    vec2 (const vec2& v) {
        x = v.x; y = v.y;
    }
    
    // Unary minus operator
    vec2 operator - () const {
        return vec2(-x, -y);
    }
    
    // Vector addition
    vec2 operator + (const vec2& v) const	{
        return vec2(x + v.x, y + v.y);
    }
    
    // Vector subtraction
    vec2 operator - (const vec2& v) const	{
        return vec2(x - v.x, y - v.y);
    }
    
    // Scalar multiplication
    vec2 operator * (const GLfloat s) const {
        return vec2(s*x, s*y);
    }
    
    // Multiply vector components
    vec2 operator * (const vec2& v) const {
        return vec2(x*v.x, y*v.y);
    }
    
    // Scalar multiplication (scalar comes second)
    friend vec2 operator * (const GLfloat s, const vec2& v) {
        return v * s;
    }
    
    // Vector scalar division (repurposing of scalar multiplication)
    vec2 operator / (const GLfloat s) const {
        GLfloat r = GLfloat(1.0) / s;
        return *this * r;
    }
};

// Length of vector
inline GLfloat length(const vec2& v) {
    return std::sqrt(pow(v.x, 2) + pow(v.y, 2));
}

#endif
