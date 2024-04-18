attribute vec4 vertexPosition;
attribute vec3 vertexColour;
varying vec3 fragColour;
uniform mat4 projection;
uniform mat4 modelView;
void main()
{
    gl_Position = projection * modelView * vertexPosition;
    fragColour = vertexColour;
}