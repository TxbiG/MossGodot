#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <memory>
#include <Moss/Resources/Timer.h>

class MossTimer : public godot::RefCounted {
    GDCLASS(MossTimer, godot::RefCounted)

    std::unique_ptr<::Timer> timer_;
    float duration_ = 1.0f;
    bool repeating_ = false;

protected:
    static void _bind_methods();

public:
    MossTimer() = default;
    ~MossTimer() override = default;

    void configure(double duration_seconds, bool repeating = false);
    void tick(double delta_seconds);
    void start();
    void stop();
    void pause();
    void resume();
    void reset();
    bool is_running() const;
    bool is_paused() const;
    double get_remaining() const;
    double get_duration() const { return duration_; }
    bool is_repeating() const { return repeating_; }
};
