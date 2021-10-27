#include "ibl/specularmap.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "constants.h"
#include "timer.h"
#include "ibl/mipmapcubemapframebuffer.h"

SpecularMap::SpecularMap(const std::string &engineRoot, const unsigned int environmentCubemapId) : environmentCubemapId(environmentCubemapId) {
    std::string prefilteredEnvMapVertexShaderPath = engineRoot + "/src/ibl/shaders/specularenv.vert";
    std::string prefilteredEnvMapFragmentShaderPath = engineRoot + "/src/ibl/shaders/specularenv.frag";

    prefilteredEnvMapShader = std::make_unique<Shader>(prefilteredEnvMapVertexShaderPath.c_str(), prefilteredEnvMapFragmentShaderPath.c_str());
    prefilteredEnvMapFramebuffer = std::make_unique<MipmapCubemapFramebuffer>(prefilteredEnvMapWidth, prefilteredEnvMapHeight);
}

void SpecularMap::computePrefilteredEnvMap()
{
    Timer timer;

    glm::mat4 model = constants::mIndentity4;
    glm::mat4 cameraAngles[] =
        {
            glm::lookAt(constants::origin, constants::unitX, -constants::unitY),
            glm::lookAt(constants::origin, -constants::unitX, -constants::unitY),
            glm::lookAt(constants::origin, constants::unitY, constants::unitZ),
            glm::lookAt(constants::origin, -constants::unitY, -constants::unitZ),
            glm::lookAt(constants::origin, constants::unitZ, -constants::unitY),
            glm::lookAt(constants::origin, -constants::unitZ, -constants::unitY)
        };
    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f), // 90 degrees to cover one face
        1.0f, // its a square
        0.1f,
        2.0f);

    auto cube = Cube();
    prefilteredEnvMapFramebuffer->bind();
    prefilteredEnvMapShader->use();

    for (auto mipLevel = 0; mipLevel < prefilteredEnvMapMipLevels; mipLevel++) {
        prefilteredEnvMapFramebuffer->setMipLevel(mipLevel);

        glViewport(0, 0, prefilteredEnvMapFramebuffer->getWidth(), prefilteredEnvMapFramebuffer->getHeight());

        // each mip level has increasing roughness
        float roughness = (float) mipLevel / (float) (prefilteredEnvMapMipLevels - 1);
        prefilteredEnvMapShader->setFloat("roughness", roughness);

        // render to each side of the cubemap
        for (auto i = 0; i < 6; i++) {
            prefilteredEnvMapShader->setModelViewProjectionMatrices(model, cameraAngles[i], projection);
            prefilteredEnvMapFramebuffer->setCubeFace(i);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            prefilteredEnvMapShader->setInt("environmentCubemap", 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubemapId);
            cube.Draw();
        }
    }

    timer.logDifference("Rendered specular pre-filtered environment map");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int SpecularMap::getPrefilteredEnvMapId()
{
   return prefilteredEnvMapFramebuffer->getCubemapTextureId();
}