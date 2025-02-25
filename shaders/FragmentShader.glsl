#version 330

in float noise;
in float height_impact;
in float fog;

out vec4 out_color;

void main() {
    vec4 color1 = vec4(0.1, 0.6, 0.1, 1);    //  green
    vec4 color2 = vec4(1.0, 0.5, 0.1, 1);    //  orange 
    vec4 color3 = vec4(0.98, 0.98, 0.98, 1); //  white
    vec4 color4 = vec4(.9f, .9f, .9f, 1);    //  grey
    vec4 color5 = vec4(.5f, .5f, .5f, .2f);  //  dark Grey

    vec4 frag_color;
    if (noise < 0.4) {
        frag_color = mix(color1, color2, noise/0.4);
    } else {
        frag_color = mix(color2, color3, (noise - 0.4) / 0.6);
    }

    frag_color = mix(frag_color, color4, height_impact);   //  for shader amplitude
    frag_color = mix(frag_color, color5, fog);             //  for shader fog
    
    out_color = frag_color;
}