#version 150

in vec2 vPosition; //receives incoming vertex positions
in vec4 vColour;
uniform vec4 uColour; //copy this to your colour output
out vec4 colour;  //passes the colour of this vertex to the fragment shader

void main() 
{
  //Add default x and w coordinates to the incoming position and pass it on.
  gl_Position = vec4(vPosition, 0.0, 1.0);

  //Colour every vertex red
  colour = uColour; //colour changes are red, green, blue and alpha
}