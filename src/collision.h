#ifndef COLLISION_H
#define COLLISION_H

#include "ModelLoader.h"
#include "VKRenderer.h"

struct axis_aligned_bounding_box {
    axis_aligned_bounding_box() {

    }

    axis_aligned_bounding_box(float _x_min, float _x_max, float _y_min, float _y_max, float _z_min, float _z_max) : x_min(_x_min), x_max(_x_max), y_min(_y_min), y_max(_y_max), z_min(_z_min), z_max(_z_max) {

    }

    axis_aligned_bounding_box(const std::vector<Vertex>& vertices) {
        for(int i = 0; i < vertices.size(); ++i) {
            if(i == 0) {
                x_min = vertices[0].position[0];
                x_max = vertices[0].position[0];

                y_min = vertices[0].position[1];
                y_max = vertices[0].position[1];

                z_min = vertices[0].position[2];
                z_max = vertices[0].position[2];
            }

            x_min = std::min(x_min, vertices[i].position[0]);
            x_max = std::min(x_max, vertices[i].position[0]);

            y_min = std::min(y_min, vertices[i].position[1]);
            y_max = std::min(y_max, vertices[i].position[1]);

            z_min = std::min(z_min, vertices[i].position[2]);
            z_max = std::min(z_max, vertices[i].position[2]);
        }
    }

    float x_min = 0;
    float x_max = 0;
    float y_min = 0;
    float y_max = 0;
    float z_min = 0;
    float z_max = 0;
};

typedef axis_aligned_bounding_box aabb;

class game_object_type {
    public:
        bool operator==(const game_object_type& rhs) const {
            return rhs.id == id;
        }

        bool operator<(const game_object_type& rhs) const {
            return id < rhs.id;
        }

        game_object_type() : id(""), object_collision(aabb()), texture_path(""), vertices({}) {
            
        }

        game_object_type(std::string resource_path, std::string file_name, std::string _id) : id(_id) {
            auto [_vertices, _texture_path] = import_model(resource_path, file_name);
            vertices = _vertices;
            texture_path = _texture_path;

            object_collision = aabb(vertices);
        }

        game_object_type(std::vector<Vertex>& vertices, std::string _id) : object_collision(aabb(vertices)), id(_id) {

        }

        const bool has_texture() const {
            return texture_path != "";
        }

        const std::string get_texture() const {
            return texture_path;
        }

        const std::vector<Vertex>& get_vertices() const {
            return vertices;
        }

        std::vector<Vertex>& get_mutable_vertices() {
            return vertices;
        }

        const aabb get_collision_box() const {
            return object_collision;
        }

        const std::string get_id() const {
            return id;
        }

    private:
        aabb object_collision;
        std::vector<Vertex> vertices;
        std::string texture_path;
        std::string id;
};

#endif