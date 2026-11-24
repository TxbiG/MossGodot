#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <memory>
#include <Moss/Resources/Json.h>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

class MossJson : public godot::RefCounted {
    GDCLASS(MossJson, godot::RefCounted)

    std::unique_ptr<::SIMDJson> root_;
    godot::String last_error_;

protected:
    static void _bind_methods();

public:
    bool parse(const godot::String &text);
    godot::String stringify(bool pretty = false, int indent = 2) const;
    bool set_value(const godot::String &key, const godot::Variant &value);
    godot::Variant get_value(const godot::String &key, const godot::Variant &fallback = godot::Variant()) const;
    godot::Dictionary to_dictionary() const;
    bool from_dictionary(const godot::Dictionary &value);
    bool is_valid() const { return root_ != nullptr; }
    godot::String get_last_error() const { return last_error_; }
};
