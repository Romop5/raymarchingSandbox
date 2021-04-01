#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <geGL/geGL.h>

#include "helpers.hpp"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}



std::string getFramgmentShaderCode()
{
    return R"(
    #version 330 core
    const float iTime            = 100.0;
    const vec2  iResolution      = vec2(1.0,1.0);

    const int g_maxIterations    = 64;
    const float g_eps            = 0.001;

    const int coloringMode       = 0;

    const float roughness        = 0.7;
    const float ambientRatio     = 0.0;
    const float shininess        = 20.0;
    const float specularity      = 0.3;

    const vec3  albedoColor      = vec3(1.0,1.0,1.0);
    const vec3  sunColor         = vec3(1.0,1.0,1.0);


    const float floorElevation   = -2.0;
    const float farPlane         = 100.0;
    const float fogStartPlane    = 50.0;
    const vec3  fogColor         = vec3(0.2,0.2,0.7);

    const vec3  floorAColor      = vec3(1.0);
    const vec3  floorBColor      = vec3(0.0);
    const float floorThickness   = 0.99;

    float sphere(vec3 pos, float radius)
    {
        return length(pos)-radius;
    }

    float sphereModulated(vec3 pos, float radius)
    {
        pos.x += 0.1*(sin(iTime*pos.y*pos.x*3.0));
        return length(pos)-radius;
    }

    float cube(vec3 pos, float radius)
    {
        return max(abs(pos.x),max(abs(pos.y),abs(pos.z)))-radius;
    }

    float unite(float a, float b)
    {
        return min(a, b);
    }

    float uniteSmooth(float a, float b)
    {
        return smoothstep(a,b, min(0.0,max(1.0,a/(a+b))));
    }

    float intersect(float a, float b)
    {
        return max(a, b);
    }

    float ground(vec3 position, float elevation)
    {
        return position.y - elevation;
    }

    float object(vec3 pos)
    {
        pos.y += sin(cos(tan(iTime*3.0)*pos.x)+pos.y*0.3*20.0)*0.3;
        pos.x += sin(pos.y*pos.x*2.0);
        pos.y += sin(iTime);
        float b = sphere(pos+vec3(0.0,0.0,0.0), 1.0);
        float c = sphere(pos+vec3(1.0,0.0,0.0), 1.0);
        return intersect(b,c);
    }

    float df(vec3 pos)
    {
        float gd = ground(pos, floorElevation);
        //float a = sphere(pos, 1.0);
        //float ab = sphere(pos+vec3(0.5+sin(iTime),0.0,sin(iTime)*-0.0), 1.0);
        
        //float d = unite(a,b);
        return unite(gd,object(pos));
        //return uniteSmooth(ab,a);
    }

    float threshold(float val, float thres)
    {
        return float(val > thres);
    }

    vec3 colorize(vec3 pos)
    {
        float distToFloor = max(0.0, min(1.0, 10.0*g_eps+pos.y-(floorElevation)));
        float checkerColor = clamp(0.0,1.0,threshold(sin(6.0*pos.x),floorThickness)+
                                           threshold(sin(6.0*pos.z),floorThickness));
        return mix(mix(floorAColor, floorBColor, checkerColor),
                   vec3(1.0,0.8,0.5),
                   distToFloor);
    }
    vec2 rayMarchWithIterations(vec3 o, vec3 d)
    {
        int currentIterations = 0;
        float t = 0.0;
        while((g_maxIterations-currentIterations) > 0)
        {
            float closestDistance = df(o+t*d);
            if(closestDistance < g_eps)
            {
                return vec2(t+closestDistance, currentIterations);
            }
            t += closestDistance;
            currentIterations = currentIterations + 1;
        }
        return vec2(t, currentIterations);
    }

    float rayMarch(vec3 o, vec3 d)
    {
        int maximumIterations = g_maxIterations;
        float t = 0.0;
        while(maximumIterations > 0)
        {
            float closestDistance = df(o+t*d);
            if(closestDistance < g_eps)
            {
                return t+closestDistance;
            }
            t += closestDistance*0.5;
            maximumIterations = maximumIterations - 1;
        }
        return t;
    }

    vec3 normalVector(vec3 o, vec3 d)
    {
        vec2 eps = vec2(g_eps, 0);
        float t = rayMarch(o, d);
        if(t < 0.0)
        {
            return vec3(0.0);
        }
        vec3 p = o+d*t;
        vec3 ds = vec3(df(p));
        vec3 dd = vec3(df(p-eps.xyy), df(p-eps.yxy), df(p-eps.yyx));
        return ds-dd;
    }

    vec3 phongShading(vec3 o, vec3 d, vec3 l)
    {;
     
        float dO = rayMarch(o, d);
        if(dO < 0.0 || dO > farPlane)
        {
           return fogColor;
        }
        vec3 p = o+d*dO;
        
      
        vec3  nlDir = normalize(l-p);
        float lightDistance = length(l-p);
        
        bool visibility = (rayMarch(p+nlDir*g_eps*5.0, nlDir) >= lightDistance-2.0*g_eps);
        
        vec3 nd = normalize(o-p);
     
        vec3 nv = normalize(normalVector(o, d));
        vec3 nnv = nv;
        
        vec3 halfDir = normalize(nlDir + nd);
        float specAngle = max(dot(halfDir, nnv), 0.0);
        float specular = pow(specAngle, shininess);
        
        float specularRatio = specular*specularity;
        float lambertianRatio = max(dot(nlDir, nnv),0.0);
        if(length(nv) < g_eps)
        {
          lambertianRatio = 0.0;
        }
        float lightAttenuation = 0.1*(pow(lightDistance,2.0)+1.0);
        float lightIntensity = 1.0/lightAttenuation;
        float fogRatio = max(0.0,(1.0+dO)/farPlane);
        
        
        vec3 albedoColor = colorize(p);
        
        vec3 ambientColorPart    = albedoColor*ambientRatio;
        vec3 shadingColor = ambientColorPart;
        if(visibility)
        {
            vec3 lambertianColorPart = albedoColor*lightIntensity*lambertianRatio*roughness;
            vec3 specularColorPart   = sunColor*specularRatio*lightIntensity;
        
        
            shadingColor += lambertianColorPart + specularColorPart;
        }
        
        return mix(shadingColor, fogColor, fogRatio);
    }


    vec3 depthMap(vec3 o, vec3 d)
    {
        
        float dO = rayMarch(o, d);
        if(dO < 0.0)
        {
            return vec3(0.0);
        }
        return vec3(1.0-(1.0/dO),0.0,0.0);
        //return vec3(1.0-(exp(dO)/10.0));
    }

    vec3 iterationsMap(vec3 o, vec3 d)
    {
        vec2 result = rayMarchWithIterations(o, d);
        float dO = result.x;
        if(dO < 0.0)
        {
            return vec3(0.0);
        }
        return vec3(result.y/float(g_maxIterations),0.0,0.0);
    }

    void mainImage( out vec4 fragColor, in vec2 fragCoord )
    {
        vec3 sunPos = vec3(sin(iTime)*0.0,2.0,sin(iTime)*10.0);

        // Normalized pixel coordinates (from 0 to 1)
        vec2 uv = fragCoord/iResolution.xy;
        uv = uv - vec2(0.5);
        uv = uv * 2.0;
        uv *= 0.5;
        
        float aspect = iResolution.x/iResolution.y;
        uv.x *= aspect;
        
        //vec3 cameraOrigin = vec3(0,0.0,-(sin(0.0)+5.0));
        vec3  orbiterPoint = vec3(0.0);
        float orbiterSpeed = 0.3;
        vec3  orbiterDir = vec3(sin(orbiterSpeed*iTime),0.0, cos(orbiterSpeed*iTime));
        float orbiterRadius = (3.0+2.0*sin(iTime*1.0))*3.0;
        vec3 cameraOrigin = orbiterPoint+orbiterDir*orbiterRadius;
        
        vec3 cameraLookUp = vec3(sin(iTime)*0.1,1.0,cos(iTime)*0.1);
        //vec3 cameraDir = normalize(vec3(sin(0.0), 0.0,abs(cos(0.0))));
        vec3 cameraDir = -normalize(orbiterDir);
        mat3 rotation = mat3( cross(cameraLookUp,cameraDir), cameraLookUp,cameraDir);
        vec3 screenLookVector = normalize(vec3(uv, 1.0));
        
        screenLookVector = rotation*screenLookVector;
        

        
        vec3 col = vec3(0.0);
        if(coloringMode == 0)
        {
            col = phongShading(cameraOrigin, screenLookVector, sunPos);
        }
        else if (coloringMode == 1)
        {
            col = depthMap(cameraOrigin, screenLookVector);
        }
        else if (coloringMode == 2)
        {
            col = abs(normalize(normalVector(cameraOrigin, screenLookVector)));
        }
        else if (coloringMode == 3)
        {
            col = iterationsMap(cameraOrigin, screenLookVector);
        }
        else
        {
            col = vec3(1.0,0.0,0.0);
        }
        
        // Output to screen
        fragColor = vec4(col,1.0);
    }

    out vec4 FragColor;
    in vec2 uv;

    void main()
    {
        mainImage(FragColor, uv);
    }
    )";
}


int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    ge::gl::init();

    auto p = std::make_shared<ge::gl::Program>();
    auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER);
    vs->compile(R"(
        #version 330 core
        layout(location = 0) in vec3 normal;
        layout(location = 1) in vec2 uvIn;

        out vec2 vs_uv;
        void main()
        {
            vs_uv = uvIn;
            gl_Position = vec4(normal.xy, 0.0, 1.0);
        }
        )");

    auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER);
    /*fs->compile(R"(
        #version 430 core
        out vec4 FragColor;
        in vec2 uv;

        void main()
        {
            vec2 color = uv;
            FragColor = vec4(color, 1.0,1.0);
        }
    )");
    */
    fs->compile(getFramgmentShaderCode());
    auto gs = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER);
    gs->compile(R"(#version 430 core
    
    in vec2 vs_uv[];
    out vec2 uv;

    layout (invocations = 9) in;
        layout (triangles) in;
        layout (triangle_strip,max_vertices=3) out;

        void main()
        {
            int i;
            for (i = 0; i < gl_in.length(); i++)
            {
                gl_Layer = gl_InvocationID;
                //gl_Layer = 0;
                gl_Position = gl_in[i].gl_Position;
                uv = vs_uv[i];
                EmitVertex();
            }
            EndPrimitive();
        }
    )");
 
    p->link({vs,fs,gs});

    //---------------------------------------------------------------
    // CREATE Full screen quad with UV
    //---------------------------------------------------------------
    auto fullscreenQuad = FullscreenQuad();

    //---------------------------------------------------------------
    // Create FBO
    //---------------------------------------------------------------
    auto fbo = std::make_shared<ge::gl::Framebuffer>();

    auto colorAttachment = std::make_shared<ge::gl::Texture>();
    colorAttachment->create(GL_TEXTURE_2D, GL_RGBA8, 1, 500,500);


    //auto textureArray = std::make_shared<ge::gl::Texture>();
    //textureArray->create(GL_TEXTURE_2D_ARRAY, GL_RGBA8, 0, 500,500,9);

    GLuint textureArray;
    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glTextureStorage3D(textureArray, 1, GL_RGBA8, 500,500,9);
    
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthTexture);
    glTextureStorage3D(depthTexture, 1, GL_DEPTH_COMPONENT32F, 500,500,9);

    //auto depthAttachment = std::make_shared<ge::gl::Texture>();
    //depthAttachment->create(GL_TEXTURE_2D_ARRAY, GL_DEPTH_COMPONENT32F, 0, 500,500,2);
   
    
    //fbo->attachTexture(GL_COLOR_ATTACHMENT0,  colorAttachment);
    fbo->bind();
    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,textureArray,0);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthTexture,0);
    fbo->unbind();
    //fbo->attachTexture(GL_DEPTH_ATTACHMENT,  depthAttachment);

    std::cout << "FBO status: " << fbo->check() << std::endl;
    //---------------------------------------------------------------
    // CREATE a procedural texture
    //---------------------------------------------------------------
    auto tex = std::make_shared<ge::gl::Texture>();
    tex->create(GL_TEXTURE_2D, GL_RGBA8, 1, 500,100);
    fillTexture(tex, 500,100);

    //---------------------------------------------------------------
    // Create texture view
    //---------------------------------------------------------------
    GLuint tex_view;
    glGenTextures(1, &tex_view);
    glTextureView(tex_view, GL_TEXTURE_2D, textureArray, GL_RGBA8,0,1,4,1);
    
    glDepthMask(GL_TRUE);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fullscreenQuad.vao->bind();
        p->use();
        glDrawArrays(GL_TRIANGLES,0,6);

        fullscreenQuad.vao->unbind();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
