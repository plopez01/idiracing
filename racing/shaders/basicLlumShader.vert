#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 tint;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;


void main()
{	
    mat3 NormalMatrix = inverse(transpose(mat3(View * TG)));

    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff * tint;
    fmatspec = matspec;
    fmatshin = matshin;
    fvertex = vec3(View * TG * vec4(vertex, 1.0)); // CALCULEU CORRECTAMENT
    fnormal = normalize(NormalMatrix * normalize(normal)); // CALCULEU CORRECTAMENT

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
