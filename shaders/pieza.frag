#version 330 core
in vec3 normalMundo;
in vec3 posicionLocal;
uniform vec3 colorPieza;
uniform bool resaltado;
uniform int textura;
out vec4 colorFinal;
void main() {
    vec3 base=colorPieza;
    // Texturas procedurales: no requieren imágenes ni archivos externos.
    if(textura == 1 && abs(normalMundo.z) > 0.9) {
        vec2 celda=fract((posicionLocal.xy+0.5)*vec2(12.0,14.0));
        float circuito=1.0-smoothstep(0.02,0.045,min(celda.x,celda.y));
        base=mix(base,vec3(0.42,0.82,0.58),circuito*0.42);
    }
    if(textura == 2 && abs(normalMundo.z) > 0.9) {
        float ranura=step(0.55,fract(posicionLocal.y*26.0));
        base*=mix(0.48,1.0,ranura);
    }
    // Lambert: una cara orientada hacia la luz recibe más iluminación.
    vec3 n=normalize(normalMundo);
    vec3 luz=normalize(vec3(-0.35,0.70,1.0));
    float difusa=max(dot(n,luz),0.0);
    vec3 iluminado=base*(0.38+0.62*difusa);
    // El resaltado también tiene aporte propio para distinguir caras en sombra.
    if(resaltado) iluminado=mix(iluminado,vec3(1.0,0.94,0.70),0.26);
    colorFinal=vec4(iluminado,1.0);
}
