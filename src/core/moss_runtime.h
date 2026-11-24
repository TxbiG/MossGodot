#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/ref.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "moss_math.h"
#include "../resources/moss_json.h"
#include "../resources/moss_timer.h"
#include "../subsystems/moss_subsystem.h"

class MossRuntime final : public godot::Object {
    GDCLASS(MossRuntime, godot::Object)

protected:
    static void _bind_methods();

public:
    godot::String get_version() const;
    godot::String get_native_backend() const;
    godot::Ref<MossMath> create_math() const;
    godot::Ref<MossTimer> create_timer(double duration_seconds, bool repeating = false) const;
    godot::Ref<MossJson> create_json() const;
    godot::Dictionary get_subsystem_status() const;
    godot::Dictionary get_diagnostics() const;
};
