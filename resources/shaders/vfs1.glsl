#version 460 core
//out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;


vec2 random2(vec2 p){
    return fract(sin(vec2(dot(p, vec2(127.1,311.7)), dot(p,vec2(269.5, 183.3)))) * 43758.5453);
}

void main()
{

    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI); 
    
    vec2 points[5];
    vec2 minpoint = vec2(0.0,0.0);
    float dist = 1.0;



    //hardcoded points
    points[0] = vec2(0.83,0.75);
    points[1] = vec2(0.50,0.7);
    points[2] = vec2(0.28,0.64);
    points[3] =  vec2(0.31,0.26);


    float aspectRatio = SCR_WID/SCR_HEI;
    
    float currentDis;

    for(int i=0; i<5; i++)
    {
        currentDis = distance(current, points[i]);
        if(dist>currentDis)
        {
            dist = currentDis;
            minpoint = points[i];
        }
    }

    color += dist*0.5;
    color.x = minpoint.x;
    color.y = minpoint.y;
    color += 1.-step(.01, dist);

    gl_FragColor = vec4(color,1.0f);
    //FragColor = vec4(st.x, st.y, 0.0f, 1.0f);

}