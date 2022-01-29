#version 460 core

//layout(pixel_center_integer) in vec4 gl_FragCoord;
//out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;
uniform vec2 mouse;
void main()
{

    vec3 color = vec3(0.0, 1.0, 0.0); 
    vec2 points[5];
    vec2 minpoint = vec2(0.0,0.0);
    float dist = 1.0;



    //hardcoded points
    points[0] = vec2(0.83,0.75);
    points[1] = vec2(0.60,0.07);
    points[2] = vec2(0.28,0.64);
    points[3] =  vec2(0.31,0.26);
    points[4] = mouse;


    float aspectRatio = SCR_WID/SCR_HEI;
    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI);
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