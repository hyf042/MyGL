#ifndef _MYGL_CORE_LIGHT_H_
#define _MYGL_CORE_LIGHT_H_

#include "Structures/Vector.h"
#include "Structures/Color.h"
#include "Material.h"

namespace MyGL {
	class Light {
	public:
		Light() {
			int tmp = 1;
		}
		virtual ~Light() {}

		virtual Color Calculate(const Vector3 &world_position, const Vector3& normal, const Material &material);

		bool IsPoint() const {
			return !IsDirection();
		}
		bool IsDirection() const {
			return Math::IsZero(_position.w());
		}
		float GetAttenuationFactor(float distance) const {
			return 1 / (_attenuation.x() + _attenuation.y() * distance + _attenuation.z() * distance * distance);
		}
		void SetDirectionLight(Vector3 direction) {
			set_position(Vector4(direction.x(), direction.y(), direction.z(), 0.0f));
		}
		void SetPointLight(Vector3 point) {
			set_position(Vector4(point.x(), point.y(), point.z(), 1.0f));
		}

		bool enabled() const {
			return _isEnabled;
		}
		void set_enabled(bool enabled) {
			_isEnabled = enabled;
		}
		Vector4 position() const {
			return _position;
		}
		Color diffuse() const {
			return _diffuse;
		}
		Color specular() const {
			return _specular;
		}
		Color ambient() const {
			return _ambient;
		}
		void set_position(const Vector4 &position);
		void set_diffuse(const Color &diffuse) {
			_diffuse = diffuse;
		}
		void set_specular(const Color &specular) {
			_specular = specular;
		}
		void set_ambient(const Color &ambient) {
			_ambient = ambient;
		}
		void set_constant_attenuation(float val) {
			_attenuation.set_x(val);
		}
		void set_linear_attenuation(float val) {
			_attenuation.set_y(val);
		}
		void set_quadratic_attenuation(float val) {
			_attenuation.set_z(val);
		}

		static shared_ptr<Light> CreateLight0() {
			auto light = make_shared<Light>();
			light->set_diffuse(Colors::White);
			light->set_specular(Colors::White);
			return light;
		}
	private:
		bool _isEnabled = false;
		Vector4 _position = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
		Color _diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
		Color _specular = Color(0.0f, 0.0f, 0.0f, 1.0f);
		Color _ambient = Color(0.0f, 0.0f, 0.0f, 1.0f);
		// (constant attenuation, linear attenuation, quadratic attenuation)
		Vector3 _attenuation = Vector3(1.0f, 0.0f, 0.0f);
	};
}

#endif