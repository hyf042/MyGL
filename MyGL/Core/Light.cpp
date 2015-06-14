#include "Light.h"
#include "GL.h"
#include "GLState.h"

namespace MyGL {
	Color Light::Calculate(const Vector3 &world_position, const Vector3& normal, const GLState &state) {
		const Material& material = state.material;

		Vector3 light;
		Color final_color(0.0f, 0.0f, 0.0f, 0.0f);
		float attenuation_factor = 1.0f;

		if (IsDirection()) {
			// direction light do not need attenuation.
			light = -_position.AsVector3();
			light.Normalize();
		}
		else {
			// in default, the point light is equal to a half-space spotlight.
			light = _position.AsVector3() - world_position;
			light.Normalize();

			// attenuation
			attenuation_factor = GetAttenuationFactor(light.Length());

			// spotlight factor
			auto direction = _spot_direction.Unit();
			float cos_cur_angle = Dot(-light, direction);
			float cos_inner_cone_angle = Math::Cos(Math::Deg2Rad(_spot_cutoff));
			float cos_outer_cone_angle = Math::Cos(Math::Deg2Rad(_spot_cutoff + _spot_outer_delta_angle));

			float spot = 0.0f;
			if (!Math::IsZero(cos_inner_cone_angle - cos_outer_cone_angle)) {
				spot = Math::Clamp(
					(cos_cur_angle - cos_outer_cone_angle) / (cos_inner_cone_angle - cos_outer_cone_angle),
					0.0f, 1.0f);
			}
			else {
				if (cos_cur_angle < cos_inner_cone_angle) {
					int tmp = 1;
				}
				spot = cos_cur_angle >= cos_inner_cone_angle ? 1.0f : 0.0f;
			}
			if (spot > 0) {
				spot = Math::Power(spot, _spot_exponent);
			}
			attenuation_factor *= spot;
		}
		
		// diffuse
		auto lambert_term = Math::Max(0.0f, Dot(normal, light));

		if (lambert_term > 0.0f) {
			auto diffuse_color = _diffuse * material.diffuse() * lambert_term;

			// specular
			auto reflect = Reflect(light, normal);
			auto view = -world_position.Unit();
			// world_position is the vector from camera (0, 0, 0) to the vertex, so negative one is the vector from vertex to camerea.
			auto specular = Math::Power(Math::Max(0.0f, Dot(reflect, view)), material.shinness());
			auto specular_color = _specular * material.specular() * specular;

			final_color += (diffuse_color + specular_color) * attenuation_factor;
		}

		// ambient
		auto ambient_color = (_ambient + state.ambient) * material.ambient();

		// emission
		auto emission_color = material.emission();

		final_color += ambient_color + emission_color;
		final_color.Normalize();
		final_color.set_a(1.0f);
		return final_color;
	}

	void Light::set_position(const Vector4 &position) {
		if (Math::IsZero(position.w())) {
			_position = Vector4::Of(GL::Instance().TransformNormalToWorld(position.AsVector3()), true);
		}
		else {
			// if it's a point light, calculate out its real world position.
			_position = Vector4::Of(GL::Instance().TransformPointToWorld(position.AsVector3()));
		}
	}
}