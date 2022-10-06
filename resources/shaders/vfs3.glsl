#version 460 core

//layout(pixel_center_integer) in vec4 gl_FragCoord;
out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;
uniform vec2 mouse;

void main()
{

    vec3 color = vec3(0.0, 0.0, 0.0);
    vec2 points[5];
    vec2 minpoint = vec2(0.0,0.0);
    float dist = 1.0;


    //hardcoded points they take fragment coordinates starting i.e 0,0 is bottom left
    points[0] = vec2(0.80,0.182);
    points[1] = vec2(0.50,0.7);
    points[2] = vec2(0.141,0.64);
    points[3] =  vec2(0.31,0.26);
    points[4] = vec2( 0.5*(mouse.x+1.0),0.5*(mouse.y+1));


    float aspectRatio = SCR_WID/SCR_HEI;
    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI);
    //current.x *= aspectRatio;
    //current.y *= aspectRatio;
    float currentDis;

    vec2 pluspoints = vec2(0.0);
    for(int i=0; i<5; i++)
    {
        currentDis = distance(current, points[i]);
        //dist = min(dist,currentDis);
        if(dist>currentDis)
        {
            dist = currentDis;
            minpoint = points[i];
            //pluspoints.x = points[i].x * 0.2;
            //pluspoints.y = points[i].y * 0.2;

        }
    }

    color += dist;
    //this for respective points
    color.x += minpoint.x; //+ pluspoints.x;
    color.y += minpoint.y; //+ pluspoints.y;

    //for dot
    color += 1.0- smoothstep(0.001,.005, dist); 

    //gl_FragColor = vec4(color,1.0f);
    FragColor = vec4(color, 1.0f);

}