#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class MossAudioStream : public godot::Node {
    GDCLASS(MossRuntimeNode, godot::Node);

protected:
    static void _bind_methods();

public:
    MossAudioStream() = default;
    ~MossAudioStream() override = default;
};