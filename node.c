#include "node.h"

Node* create_node() {
    Node* node = malloc(sizeof(Node));

    node->transform = (Transform){.translation = Vector3Zero(),
                                  .rotation = QuaternionIdentity(),
                                  .scale = Vector3One()};

    node->parent = NULL;
    node->next = NULL;
    node->child = NULL;

    return node;
}

void free_node(Node* node) {
    if (node->child) free_node(node->child);
    if (node->next) free_node(node->next);
    node->child = node->next = node->parent = NULL;
    free(node);
}

Node* create_node_from_mesh(Mesh mesh) {
    Node* node = create_node();
    node->model = LoadModelFromMesh(mesh);
    return node;
}

Node* create_node_from_mesh_with_transform(Mesh mesh, Transform transform) {
    Node* node = create_node();
    node->model = LoadModelFromMesh(mesh);
    node->transform = transform;
    return node;
}

void add_child_node(Node* parent, Node* child) {
    parent->child = child;
    child->parent = parent;
}

Transform get_transform(Node* self) {
    Transform self_t = self->transform;
    if (self->parent == NULL) return self_t;
    Transform parent_t = get_transform(self->parent);
    return add_transforms(self_t, parent_t);
}