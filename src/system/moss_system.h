#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Engine-independent system contract. Derive from this class when your
// gameplay/service code should be usable without creating a Godot Node.
class MossSystem {
public:
    virtual ~MossSystem() = default;
    virtual const char *get_name() const = 0;
    virtual int get_update_order() const { return 0; }
    virtual bool on_initialize(std::string &error) = 0;
    virtual void on_update(float delta_seconds) = 0;
    virtual void on_shutdown() = 0;
};

class MossSystemRegistry final {
public:
    MossSystemRegistry() = default;
    ~MossSystemRegistry();
    MossSystemRegistry(const MossSystemRegistry &) = delete;
    MossSystemRegistry &operator=(const MossSystemRegistry &) = delete;

    bool add(std::unique_ptr<MossSystem> system, std::string &error);
    bool initialize_all(std::string &error);
    void update_all(float delta_seconds);
    void shutdown_all();
    std::vector<std::string> get_system_names() const;
    bool is_initialized() const { return initialized_; }

private:
    struct Entry {
        std::unique_ptr<MossSystem> system;
        bool initialized = false;
    };
    std::vector<Entry> systems_;
    bool initialized_ = false;
};
