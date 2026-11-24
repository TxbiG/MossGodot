#include "moss_json.h"
#include "../core/moss_native.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <algorithm>
#include <exception>
#include <string>

using namespace godot;

namespace {
std::string to_std(const String &value) {
    CharString utf8 = value.utf8();
    return std::string(utf8.get_data(), static_cast<size_t>(utf8.length()));
}
String to_godot_string(const std::string &value) { return String::utf8(value.c_str()); }

std::unique_ptr<::SIMDJson> from_variant(const Variant &value) {
    switch (value.get_type()) {
        case Variant::NIL: return ::SIMDJson::make_null();
        case Variant::BOOL: return ::SIMDJson::make_bool(static_cast<bool>(value));
        case Variant::INT: return ::SIMDJson::make_number(static_cast<double>(static_cast<int64_t>(value)));
        case Variant::FLOAT: return ::SIMDJson::make_number(static_cast<double>(value));
        case Variant::STRING: return ::SIMDJson::make_string(to_std(static_cast<String>(value)));
        case Variant::ARRAY: {
            auto result = ::SIMDJson::make_array();
            Array array = value;
            for (int64_t i = 0; i < array.size(); ++i) result->as_arr().push_back(from_variant(array[i]));
            return result;
        }
        case Variant::DICTIONARY: {
            auto result = ::SIMDJson::make_object();
            Dictionary dictionary = value;
            Array keys = dictionary.keys();
            for (int64_t i = 0; i < keys.size(); ++i) {
                String key = keys[i];
                result->set(to_std(key), from_variant(dictionary[key]));
            }
            return result;
        }
        default:
            // Keep the bridge intentionally JSON-native. Node/Object/Resource and
            // packed engine types must be converted explicitly by callers.
            return ::SIMDJson::make_string("[unsupported Godot Variant]");
    }
}

Variant to_variant(const ::SIMDJson &value) {
    switch (value.type()) {
        case Type::Null: return Variant();
        case Type::Bool: return Variant(value.as_bool());
        case Type::Number: return Variant(value.as_num());
        case Type::String: return Variant(to_godot_string(value.as_str()));
        case Type::Array: {
            Array array;
            for (const auto &entry : value.as_arr()) array.push_back(to_variant(*entry));
            return array;
        }
        case Type::Object: {
            Dictionary dictionary;
            for (const auto &entry : value.as_obj()) dictionary[to_godot_string(entry.key)] = to_variant(*entry.value);
            return dictionary;
        }
    }
    return Variant();
}
}

void MossJson::_bind_methods() {
    ClassDB::bind_method(D_METHOD("parse", "text"), &MossJson::parse);
    ClassDB::bind_method(D_METHOD("stringify", "pretty", "indent"), &MossJson::stringify, DEFVAL(false), DEFVAL(2));
    ClassDB::bind_method(D_METHOD("set_value", "key", "value"), &MossJson::set_value);
    ClassDB::bind_method(D_METHOD("get_value", "key", "fallback"), &MossJson::get_value, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("to_dictionary"), &MossJson::to_dictionary);
    ClassDB::bind_method(D_METHOD("from_dictionary", "value"), &MossJson::from_dictionary);
    ClassDB::bind_method(D_METHOD("is_valid"), &MossJson::is_valid);
    ClassDB::bind_method(D_METHOD("get_last_error"), &MossJson::get_last_error);
}

bool MossJson::parse(const String &text) {
    try {
        root_ = ::SIMDJson::parse(to_std(text));
        last_error_ = String();
        return root_ != nullptr;
    } catch (const std::exception &e) {
        root_.reset();
        last_error_ = String::utf8(e.what());
        return false;
    }
}

String MossJson::stringify(bool pretty, int indent) const {
    if (!root_) return String();
    try { return to_godot_string(root_->dump(pretty, std::max(0, indent))); }
    catch (const std::exception &e) { return String::utf8(e.what()); }
}

bool MossJson::set_value(const String &key, const Variant &value) {
    if (!root_) root_ = ::SIMDJson::make_object();
    if (root_->type() != Type::Object) {
        last_error_ = "Root JSON value must be an object before setting a key.";
        return false;
    }
    root_->set(to_std(key), from_variant(value));
    last_error_ = String();
    return true;
}

Variant MossJson::get_value(const String &key, const Variant &fallback) const {
    if (!root_ || root_->type() != Type::Object) return fallback;
    const ::SIMDJson *value = root_->find(to_std(key));
    return value ? to_variant(*value) : fallback;
}

Dictionary MossJson::to_dictionary() const {
    Dictionary dictionary;
    if (!root_ || root_->type() != Type::Object) return dictionary;
    for (const auto &entry : root_->as_obj()) {
        dictionary[to_godot_string(entry.key)] = to_variant(*entry.value);
    }
    return dictionary;
}

bool MossJson::from_dictionary(const Dictionary &value) {
    root_ = from_variant(Variant(value));
    last_error_ = String();
    return root_ && root_->type() == Type::Object;
}
