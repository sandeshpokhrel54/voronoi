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
    float t = 5.0;


    //hardcoded points they take fragment coordinates starting i.e 0,0 is bottom left
    
    //a point having x and y coordinates,
    //consider each point has a vector with velocity and direction
    //now calculate the time needed to reach any pixel from that point with its velocity and direction  
    //time = distance/velocity


    //for now assuming x and y coordinates to be same as velocity in x-direction and y-direction
    points[0] = vec2(0.80,0.182);
    points[1] = vec2(0.50,0.7);
    points[2] = vec2(0.141,0.64);
    points[3] =  vec2(0.31,0.26);
    points[4] = vec2( 0.5*(mouse.x+1.0),0.5*(mouse.y+1));

    //associated color with each point
    vec3 ranga[5];
    ranga[0] = vec3(0.8, 0.2, 0.2);
    ranga[1] = vec3(0.2, 0.8, 0.2);
    ranga[2] = vec3(0.2, 0.2, 0.8);
    ranga[3] = vec3(0.8, 0.8, 0.1);
    ranga[4] = vec3(0.8, 0.1, 0.8);
    //define velocities of each of the point
    vec2 velocity[5];
    velocity[0] = vec2(0.11, 0.2); 
    velocity[1] = vec2(0.2, 0.2);
    velocity[2] = vec2(0.3, 0.3);
    velocity[3] = vec2(0.32, 0.2);
    velocity[4] = vec2(0.1, 0.1);


    
    float aspectRatio = SCR_WID/SCR_HEI;
    vec2 current = vec2(gl_FragCoord.x/ SCR_WID, gl_FragCoord.y/SCR_HEI);
    float time;
    int minpoint_index;

    for(int i=0; i<5; i++)
    {
        time = distance(current, points[i])/length(velocity[i]);
        if(t>time)
        {
            t = time;
            minpoint = points[i];
            minpoint_index = i;

        }
    }

    //time field
    //color += t;
    //this for respective points
    //color = ranga[minpoint_index];
    color.x = minpoint.x;
    color.y = minpoint.y;
    
    //for dot
    color += 1.0 - step(0.002, t); 

    //gl_FragColor = vec4(color,1.0f);
    FragColor = vec4(color, 1.0f);

}