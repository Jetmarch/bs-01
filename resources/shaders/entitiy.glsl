#version 430

struct Entity
{
    vec2 position;
    vec2 velocity;
};

layout(std430, binding = 0) buffer Entities
{
    Entity entities[];
};

layout(location = 0) uniform vec2 target;
layout(location = 1) uniform float dt;

layout(local_size_x = 256) in;

void main()
{
    uint id = gl_GlobalInvocationID.x;

    vec2 position = entities[id].position;
    vec2 velocity = entities[id].velocity;

    vec2 direction = target - position;

    float distance = length(direction);

    if (distance > 0.001)
    {
        direction /= distance;

        velocity += direction * 100.0 * dt;
    }

    float speed = length(velocity);

    if (speed > 100.0)
    {
        velocity = velocity / speed * 100.0;
    }

    position += velocity * dt;

    entities[id].position = position;
    entities[id].velocity = velocity;
}
