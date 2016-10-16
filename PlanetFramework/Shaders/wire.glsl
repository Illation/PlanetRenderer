<VERTEX>
	#version 330 core
	
	layout (location = 0) in vec3 position;
	
	uniform mat4 model;
	uniform mat4 viewProj;
	
	void main()
	{
		gl_Position = viewProj * model * vec4(position, 1.0f);
	}
</VERTEX>
<FRAGMENT>
	#version 330 core

	out vec4 outColor;
	
	void main()
	{
		outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	} 
</FRAGMENT>