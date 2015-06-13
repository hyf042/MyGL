#ifndef _MYGL_CORE_MATERIAL_H_
#define _MYGL_CORE_MATERIAL_H_

namespace MyGL {
	class Material {
	public:
		Material() {}

		Color diffuse() const {
			return _diffuse;
		}
		Color ambient() const {
			return _ambient;
		}
		Color specular() const {
			return _specular;
		}
		Color emission() const {
			return _emission;
		}
		float shinness() const {
			return _shininess;
		}
		void set_diffuse(const Color &val) {
			_diffuse = val;
		}
		void set_ambient(const Color &val) {
			_ambient = val;
		}
		void set_specular(const Color &val) {
			_specular = val;
		}
		void set_emission(const Color &val) {
			_emission = val;
		}
		void set_shininess(float val) {
			_shininess = val;
		}

	private:
		Color _diffuse = Color(0.8f, 0.8f, 0.8f, 1.0f);
		Color _ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
		Color _specular = Color(0.0f, 0.0f, 0.0f, 1.0f);
		Color _emission = Color(0.0f, 0.0f, 0.0f, 1.0f);
		float _shininess = 0;
	};
}

#endif
