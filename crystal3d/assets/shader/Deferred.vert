varying out vec2 fragCoord;

void main( void )
{
	//TODO: Use In's
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	fragCoord = gl_MultiTexCoord0.xy;
    gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
}