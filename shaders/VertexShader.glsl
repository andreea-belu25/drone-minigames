#version 330

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 drone_position;

out float noise;
out float height_impact;
out float fog;

float random(vec2 st) {
   return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float generate_noise(vec2 st) {
   vec2 i = floor(st);
   vec2 f = fract(st);
   
   float a = random(i);
   float b = random(i + vec2(1.0, 0.0));
   float c = random(i + vec2(0.0, 1.0));
   float d = random(i + vec2(1.0, 1.0));
   
   vec2 u = f * f * (3.0 - 2.0 * f);
   
   return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
   float frequency = 0.15;
   noise = generate_noise(v_position.xz * frequency);
   float height = noise * 0.5;
   
   vec3 position = v_position;
   position.y = height;

   height_impact = 2 * drone_position.y / 50.0f;
   if (height_impact > 0.9f) {
      height_impact = 0.9f;
   }
   if (height_impact < 0.0f) {
      height_impact = 0.0f;
   }
   
   fog = distance(position, drone_position) / 50.0f;
   if (fog > .99f) {
      fog = .99f;
   }
   if (fog < 0.0f) {
      fog = 0.0f;
   }

   gl_Position = Projection * View * Model * vec4(position, 1.0);
}