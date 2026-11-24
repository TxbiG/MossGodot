#include "moss_system.h"
#include <algorithm>
#include <utility>

MossSystemRegistry::~MossSystemRegistry() { shutdown_all(); }

bool MossSystemRegistry::add(std::unique_ptr<MossSystem> system, std::string &error) {
    if (!system) { error = "Cannot register a null MossSystem."; return false; }
    if (initialized_) { error = "Systems cannot be added after registry initialization."; return false; }
    const char *name = system->get_name();
    if (!name || *name == '\0') { error = "MossSystem must provide a non-empty name."; return false; }
    for (const Entry &entry : systems_) {
        if (entry.system && std::string(entry.system->get_name()) == name) {
            error = "A MossSystem with this name is already registered.";
            return false;
        }
    }
    systems_.push_back({std::move(system), false});
    std::stable_sort(systems_.begin(), systems_.end(), [](const Entry &a, const Entry &b) {
        return a.system->get_update_order() < b.system->get_update_order();
    });
    error.clear();
    return true;
}

bool MossSystemRegistry::initialize_all(std::string &error) {
    if (initialized_) { error.clear(); return true; }
    for (Entry &entry : systems_) {
        if (!entry.system->on_initialize(error)) {
            shutdown_all();
            if (error.empty()) error = std::string("MossSystem initialization failed: ") + entry.system->get_name();
            return false;
        }
        entry.initialized = true;
    }
    initialized_ = true;
    error.clear();
    return true;
}

void MossSystemRegistry::update_all(float delta_seconds) {
    if (!initialized_ || delta_seconds <= 0.0f) return;
    for (Entry &entry : systems_) if (entry.initialized) entry.system->on_update(delta_seconds);
}

void MossSystemRegistry::shutdown_all() {
    for (auto it = systems_.rbegin(); it != systems_.rend(); ++it) {
        if (it->initialized && it->system) {
            it->system->on_shutdown();
            it->initialized = false;
        }
    }
    initialized_ = false;
}

std::vector<std::string> MossSystemRegistry::get_system_names() const {
    std::vector<std::string> names;
    names.reserve(systems_.size());
    for (const Entry &entry : systems_) names.emplace_back(entry.system->get_name());
    return names;
}
