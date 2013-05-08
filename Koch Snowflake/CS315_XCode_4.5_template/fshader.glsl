#version 150

in  vec4 colour; //The blended fragment colour from the vertex shader.
                 //Name must match the an output in the vertex shader.
out vec4 fragColour; //Define a name for the colour output

void main() 
{ 
    fragColour = colour;
} 