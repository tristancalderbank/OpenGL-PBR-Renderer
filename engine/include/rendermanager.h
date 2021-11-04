#pragma once

#include "glad/glad.h"
#include "glfw3.h"

#include "cameramanager.h"
#include "engineconfig.h"
#include "framebuffer.h"
#include "fullscreenquad.h"
#include "scene.h"
#include "skybox.h"
#include "shader.h"
#include "model.h"
#include "ibl/diffuseirradiancemap.h"
#include "ibl/equirectangularcubemap.h"
#include "ibl/specularmap.h"
#include "windowmanager.h"

const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

class RenderManager {
public:
    RenderManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager, std::shared_ptr<CameraManager> cameraManager);
    void startup(std::shared_ptr<Scene> scene);
    void shutdown();
    void startGuiFrame();
    void render();
private:
    EngineConfig mEngineConfig;
    std::shared_ptr<WindowManager> mWindowManager;
    std::shared_ptr<CameraManager> mCameraManager;

    // framebuffers
    std::unique_ptr<Framebuffer> mFramebuffer;

    // pre-computed IBL stuff
    std::unique_ptr<EquirectangularCubemap> mIblEquirectangularCubemap;
    std::unique_ptr<DiffuseIrradianceMap> mIblDiffuseIrradianceMap;
    std::unique_ptr<SpecularMap> mIblSpecularMap;

    // skybox
    std::unique_ptr<Skybox> mSkybox;

    // scene
    std::shared_ptr<Scene> mScene;

    // post-processing
    bool mTonemappingEnabled = false;
    float mGammaCorrectionFactor = 2.2;
    std::unique_ptr<FullscreenQuad> mFullscreenQuad;

    // shaders
    std::unique_ptr<Shader> mPbrShader;
    std::unique_ptr<Shader> mPostShader;
    std::unique_ptr<Shader> mSkyboxShader;
};