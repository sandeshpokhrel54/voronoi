#version 460 core
out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;
uniform float time;


vec2 random2(vec2 p){
    return fract(sin(vec2(dot(p, vec2(127.1,311.7)), dot(p,vec2(269.5, 183.3)))) * 43758.5453);
}

void main()
{

    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI); 
    
    //scaling- number of squares in the tiles
    current*=4.0;

    vec3 color = vec3(.0);
    float dist = 1.0;

    float aspectRatio = SCR_WID/SCR_HEI;

    //tiling
    vec2 i_current = floor(current);
    vec2 f_current = fract(current);


    for(int y=-1; y<=1; y++)
    {
        for(int x=-1; x<=1; x++)
        {
        
            vec2 neighbor = vec2(float(x),float(y));
    

            //random position from current + neighbor
            vec2 point = random2(i_current+neighbor);

            point = 0.5 + 0.5*sin(time+6.2831*point);

            //vector between pixel and point
            vec2 diff = neighbor + point - f_current;

            float currentDis = length(diff);

            dist = min(dist, currentDis);

        }
   }

   //distance field
   color += dist;

   //draw center
   color += 1.-step(0.01,dist);

   //draw grid
   color.r += step(.98, f_current.x) + step(.98, f_current.y);

    //gl_FragColor = vec4(color,1.0f);
    FragColor = vec4(color, 1.0f);

}