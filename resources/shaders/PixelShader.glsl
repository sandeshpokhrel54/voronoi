#version 460 core

out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;
uniform vec2 mouse;
uniform vec2 time;

void main()
{

    vec3 color = vec3(0.0, 1.0, 0.0); 
    vec2 points[10];
    vec2 minpoint = vec2(0.0,0.0);
    float dist = 1.0;

    vec2 tp = 0.7*sin(time);

    //hardcoded points they take fragment coordinates starting i.e 0,0 is bottom left

    //making points dependent on points[4] i.e mouse motion
    //points[4] = points[4] = vec2(0.5*(mouse.x+1.0),0.5*(mouse.y+1));
    //points[0] = 0.5 + 0.314*sin(1.68*points[4]);
    //points[1] = vec2(0.20+points[4].x,0.2+points[4].y);
    //points[1] = 0.5 + 0.5*cos(2.7182*points[4]);
    //points[2] = 0.5 + 0.5*sin(3.141*points[4]);
    //points[3] = tp;
    //points[3] = 0.5 + 0.314*cos(6.18*points[4]);
    //points[4] = vec2(0.5*(mouse.x+1.0),0.5*(mouse.y+1));
    //points[4] = vec2(0.2,0.1);
    
    //for animated points with time
    points[0] = 0.2+ cos(time*0.122);
    points[1] =  0.2+ sin(time*0.31);
    points[2] = 0.5 + tp*0.5;
    points[3] = vec2(0.31,0.26)+cos(tp*0.618);
    points[4] = vec2(0.5, 0.5);
    points[5] = vec2(0.5*(mouse.x+1.0),0.5*(mouse.y+1));
    points[6] = points[5] * 0.01*tp;
    points[7] = points[5].x * points[6];
    points[8] = 0.5+0.5*cos(1.68*time);
    points[9] = 0.02*time;


    float aspectRatio = SCR_WID/SCR_HEI;
    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI);
    float currentDis;

    for(int i=0; i<10; i++)
    {
        currentDis = distance(current, points[i]);
        if(dist>currentDis)
        {
            dist = currentDis;
            minpoint = points[i];
        }
    }

    color += dist;
    color.x = minpoint.x;
    color.y = minpoint.y;
    color += 1.-step(.002, dist);

    //gl_FragColor = vec4(color,1.0f);
    FragColor = vec4(color, 1.0f);

}