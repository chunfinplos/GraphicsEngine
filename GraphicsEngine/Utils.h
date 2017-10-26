#pragma once


using namespace std;

class Utils {

public:
    static bool LoadOBJ(const char * path,
                        vector<unsigned short> & out_indices,
                        vector<glm::vec3> & out_vertices,
                        vector<glm::vec2> & out_uvs,
                        vector<glm::vec3> & out_normals);

    static GLuint LoadDDS(const char * imagepath);

    static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
};

