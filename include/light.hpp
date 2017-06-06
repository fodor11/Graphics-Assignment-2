#include "../src/tdogl/Program.h"
#include <gl/glm/glm.hpp>

class Light {
public:
	/// creates light, sets uniforms in shaderprogram
	Light(tdogl::Program* shaderProgram, glm::vec3 position, glm::vec3 intensities);
	/// updates position, sets light.position uniform in shaderprogram
	void updatePosition(glm::vec3 newPosition);
	/// updates intensities, sets light.intensities uniform in shaderprogram
	void updateIntensities(glm::vec3 newIntensities);
	/// updates intensities and position, sets uniforms in shaderprogram
	void updateLight(glm::vec3 newPosition, glm::vec3 newIntensities);
	/// returns the position of the lightsource
	glm::vec3 getPosition();
	/// returns the intensities of the lightsource
	glm::vec3 getIntensities();

private:
	glm::vec3 m_vPosition;
	glm::vec3 m_vIntensities;
	tdogl::Program* m_pProgram;
	void setShaderUniforms();
};