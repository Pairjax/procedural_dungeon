#include "material.h"

Material::Material(int _useSpecularMap = false, int _specularity = 0.1, 
		int _ambiance = 0.1, int _diffuse = 0.1) :
		useSpecularMap(_useSpecularMap), specularity(_specularity), 
		ambiance(_ambiance), diffuse(_diffuse) {}

void Material::get_uniforms() {
	// TODO: Add uniform type & get these uniforms from the material
}