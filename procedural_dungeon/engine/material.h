class Material {
public:
	Material(int _useSpecularMap, int _specularity, int _ambiance, int _diffuse);

	void get_uniforms();

private:
	// TODO: texture class
	bool useSpecularMap;
	int specularity;
	int ambiance;
	int diffuse;
};