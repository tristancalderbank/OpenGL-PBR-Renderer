#pragma once

#include "glad/glad.h"
#include "glfw3.h"

#include "fullscreenquad.h"
#include "skybox.h"
#include "shader.h"
#include "model.h"
#include "ibl/diffuseirradiancemap.h"
#include "ibl/equirectangularcubemap.h"
#include "ibl/specularmap.h"

const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

class RenderManager {
public:
    void startup();
    void shutdown();
    void render();
private:
    GLFWwindow* mWindow;

    // pre-computed IBL stuff
    std::unique_ptr<EquirectangularCubemap> mIblEquirectangularCubemap;
    std::unique_ptr<DiffuseIrradianceMap> mIblDiffuseIrradianceMap;
    std::unique_ptr<SpecularMap> mIblSpecularMap;

    // skybox
    std::unique_ptr<Skybox> mSkybox;

    // objects
    std::unique_ptr<Model> mHelmet;

    // lights
    std::vector<glm::vec3> mLightPositions = {
        glm::vec3(0.0f, 0.0f, 10.0f)
    };

    std::vector<glm::vec3> mLightColors = {
        glm::vec3(150.0f, 150.0f, 150.0f)
    };

    // post-processing
    bool mTonemappingEnabled = false;
    float mGammaCorrectionFactor = 2.2;
    std::unique_ptr<FullscreenQuad> mFullscreenQuad;

    // shaders
    std::unique_ptr<Shader> mPbrShader;
    std::unique_ptr<Shader> mPostShader;
    std::unique_ptr<Shader> mSkyboxShader;
};