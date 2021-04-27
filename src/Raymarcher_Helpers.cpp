#include "Raymarcher_Helpers.hpp"

#include <sstream>

namespace 
{
    std::string GetFragmentShaderHead()
    {
        auto literal = R"(
        #version 330 core
        uniform float iTime            = 100.0;
        uniform vec3 camera_origin   = vec3(0.0);
        uniform mat4 camera_rotation = mat4(1.0);
        const vec2  iResolution      = vec2(1.0,1.0);

        uniform int g_maxIterations;
        uniform float g_eps;
        uniform float g_stepRatio      = 0.99;

        uniform int coloringMode       = 0;

        const float roughness        = 0.7;
        uniform float ambientRatio;
        const float shininess        = 20.0;
        uniform float specularity =  1.0;

        const vec3  albedoColor      = vec3(1.0,1.0,1.0);
        uniform vec3  sunColor         = vec3(1.0,1.0,1.0);


        const float floorElevation   = -2.0;
        uniform float farPlane         = 200.0;
        //const vec3  fogColor         = vec3(0.1,0.1,0.3);
        //const vec3  fogColor         = vec3(0.2,0.2,0.7);
        uniform vec3  fogColor         = vec3(0.871,0.871,1.0);

        const vec3  floorAColor      = vec3(1.0);
        const vec3  floorBColor      = vec3(0.0);
        const float floorThickness   = 0.99;

        uniform bool renderShadows   = true;
        uniform bool renderFog       = true;

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

        vec4 unite(vec4 a, vec4 b)
        {
            if(a.x > b.x)
                return b;
            return a;
        }

        float uniteSmooth(float a, float b)
        {
            return smoothstep(a,b, min(0.0,max(1.0,a/(a+b))));
        }

        float intersect(float a, float b)
        {
            return max(a, b);
        }

        float threshold(float val, float thres)
        {
            return float(val > thres);
        }


        vec4 ground(vec3 position, float elevation)
        {
            float checkerColor = clamp(0.0,1.0,threshold(sin(6.0*position.x),floorThickness)+
                                               threshold(sin(6.0*position.z),floorThickness));
            vec3 color = mix(floorAColor, floorBColor, checkerColor);
            return vec4(position.y - elevation, color);
        }

        vec4 object(vec3 pos)
        {
            pos.y += sin(cos(tan(iTime*3.0)*pos.x)+pos.y*0.3*20.0)*0.3;
            pos.x += sin(pos.y*pos.x*2.0);
            pos.y += sin(iTime);
            float b = sphere(pos+vec3(0.0,0.0,0.0), 1.0);
            float c = sphere(pos+vec3(1.0,0.0,0.0), 1.0);
            return vec4(intersect(b,c), vec3(1.0,0.0,0.0));
        }
        )";
        return literal;
    }


    std::string GetFragmentShaderTail()
    {
        auto literal = std::string(R"(

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
                float closestDistance = df(o+t*d).x;
                if(closestDistance < g_eps)
                {
                    return vec2(t+closestDistance, currentIterations);
                }
                t += closestDistance*g_stepRatio;
                currentIterations = currentIterations + 1;
            }
            return vec2(t, currentIterations);
        }



        vec4 rayMarch(vec3 o, vec3 d)
        {
            int maximumIterations = g_maxIterations;
            float t = 0.0;
            vec3 color;
            while(maximumIterations > 0)
            {
                vec4 result = df(o+t*d);
                float closestDistance = result.x;
                if(closestDistance > farPlane)
                {
                    return vec4(farPlane*2.0, fogColor);
                }
                color = result.yzw;
                if(closestDistance < g_eps)
                {
                    return vec4(t+closestDistance, color);
                }
                t += closestDistance*g_stepRatio;
                maximumIterations = maximumIterations - 1;
            }
            return vec4(t, color);
        }

        vec3 normalVector(vec3 o, vec3 d)
        {
            vec2 eps = vec2(g_eps, 0);
            float t = rayMarch(o, d).x;
            if(t < 0.0)
            {
                return vec3(0.0);
            }
            vec3 p = o+d*t;
            vec3 ds = vec3(df(p).x);
            vec3 dd = vec3(df(p-eps.xyy).x, df(p-eps.yxy).x, df(p-eps.yyx).x);
            return ds-dd;
        }

        vec3 phongShading(vec3 o, vec3 d, vec3 l)
        {;
         
            vec4 result = rayMarch(o, d);
            float dO = result.x;
            vec3 albedo = result.yzw;
            if(dO < 0.0 || dO > farPlane)
            {
               return fogColor;
            }
            //return result.yzw;
            vec3 p = o+d*dO;

          
            vec3  nlDir = normalize(l-p);
            float lightDistance = length(l-p);

            bool visibility = false;

            if(renderShadows)
            {
                if(lightDistance < 30.0)
                {
                    visibility = (rayMarch(p+nlDir*g_eps*5.0, nlDir).x >= lightDistance-2.0*g_eps);
                }
            } else {
                visibility = true;
            }

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
            if(!renderFog)
            {
                fogRatio = 0.0;
            }
            
            
            vec3 albedoColor = albedo;
            
            vec3 ambientColorPart    = albedoColor*ambientRatio;
            vec3 shadingColor = ambientColorPart;
            if(visibility)
            {
                vec3 lambertianColorPart = sunColor*albedoColor*lightIntensity*lambertianRatio*roughness;
                vec3 specularColorPart   = sunColor*specularRatio*lightIntensity;
            
            
                shadingColor += lambertianColorPart + specularColorPart;
            }
            
            return mix(shadingColor, fogColor, fogRatio);
        }


        vec3 depthMap(vec3 o, vec3 d)
        {
            
            float dO = rayMarch(o, d).x;
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
            //vec3 cameraOrigin = orbiterPoint+orbiterDir*orbiterRadius;
            vec3 cameraOrigin = camera_origin;
            
            vec3 cameraLookUp = vec3(sin(iTime)*0.1,1.0,cos(iTime)*0.1);
            //vec3 cameraDir = normalize(vec3(sin(0.0), 0.0,abs(cos(0.0))));
            vec3 cameraDir = -normalize(orbiterDir);
            //mat3 rotation = mat3( cross(cameraLookUp,cameraDir), cameraLookUp,cameraDir);
            
            vec3 screenLookVector = normalize(vec3(uv, 1.0));
            
            //screenLookVector = rotation*screenLookVector;
            screenLookVector = (camera_rotation*vec4(screenLookVector,0.0)).xyz;
            
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

        )");
        return literal;
    }
}

std::string raymarcher::ConstructRenderedFragmentShader(std::string sdfGLSL)
{
    std::stringstream ss;
    ss  << GetFragmentShaderHead() << std::endl
        << sdfGLSL << std::endl
        << GetFragmentShaderTail();
    return ss.str();
}

std::string raymarcher::ConstructRenderedVertexShader()
{
    return R"(
    #version 330 core
    layout(location = 0) in vec3 normal;
    layout(location = 1) in vec2 uvIn;

    out vec2 uv;
    void main()
    {
        uv = uvIn;
        gl_Position = vec4(normal.xy, 0.0, 1.0);
    }
    )";
}
