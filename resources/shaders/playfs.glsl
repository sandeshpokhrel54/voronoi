#version 460 core

//layout(pixel_center_integer) in vec4 gl_FragCoord;
out vec4 FragColor;

uniform float SCR_HEI;
uniform float SCR_WID;
uniform vec2 ball;
uniform vec2 player;

void main()
{

    vec3 color = vec3(0.0, 1.0, 0.0);
    vec2 points[5];
    vec2 minpoint = vec2(0.0, 0.0);
    float dist = 1.0;



    //hardcoded points they take fragment coordinates starting i.e 0,0 is bottom left
    points[0] = vec2(0.80, 0.182);
    points[1] = vec2(0.50, 0.7);
    points[2] = vec2(0.141, 0.64);
    points[3] = vec2(0.5 * (player.x + 1.0), 0.5 * (player.y + 1));
    points[4] = vec2(0.5 * (ball.x + 1.0), 0.5 * (ball.y + 1));


    float aspectRatio = SCR_WID / SCR_HEI;
    vec2 current = vec2(gl_FragCoord.x / SCR_WID, gl_FragCoord.y / SCR_HEI);
    //current.x *= aspectRatio;
    //current.y *= aspectRatio;
    float currentDis;

    for (int i = 0; i < 5; i++)
    {
        currentDis = distance(current, points[i]);
        //dist = min(dist,currentDis);
        if (dist > currentDis)
        {
            dist = currentDis;
            minpoint = points[i];
        }
    }

    color += dist;
    color.x = minpoint.x;
    color.y = minpoint.y;
    color += 1. - step(.002, dist);

    //gl_FragColor = vec4(color,1.0f);
    FragColor = vec4(color, 1.0f);

}