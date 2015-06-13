#include "Light.h"
#include "GL.h"

namespace MyGL {
	Color Light::Calculate(const Vector3 &world_position, const Vector3& normal, const Material &material) {
		Vector3 light;
		float attenuation_factor = 1.0f;
		if (IsDirection()) {
			// direction light do not need attenuation.
			light = -_position.AsVector3();
		}
		else {
			light = _position.AsVector3() - world_position;
			attenuation_factor = GetAttenuationFactor(light.Length());
		}
		light.Normalize();

		// diffuse
		auto diffuse = Math::Max(0.0f, Dot(normal, light));
		if (Math::Greater(diffuse, 0)) {
			int tmp = 1;
		}
		auto diffuse_color = _diffuse * material.diffuse() * diffuse;

		// specular
		auto reflect = 2 * normal * Dot(normal, light) - light;
		auto vector = -world_position.Unit();
		// world_position is the vector from camera (0, 0, 0) to the vertex, so negative one is the vector from vertex to camerea.
		auto specular = diffuse <= 0 ? 0 : Math::Power(Math::Max(0.0f, Dot(reflect, vector)), material.shinness());
		auto specular_color = _specular * material.specular() * specular;

		// ambient
		auto ambient_color = _ambient * material.ambient();

		// emission
		auto emission_color = material.emission();

		auto final_color = diffuse_color + specular_color + ambient_color + emission_color;
		final_color.Normalize();
		return final_color;
	}

	void Light::set_position(const Vector4 &position) {
		if (Math::IsZero(position.w())) {
			_position = position;
		}
		else {
			// if it's a point light, calculate out its real world position.
			_position = Vector4::Of(GL::Instance().TransformToWorld(position.AsVector3()));
		}
	}
}