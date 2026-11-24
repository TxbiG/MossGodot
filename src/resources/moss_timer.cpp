#include "moss_timer.h"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

using namespace godot;

void MossTimer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("configure", "duration_seconds", "repeating"), &MossTimer::configure, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("tick", "delta_seconds"), &MossTimer::tick);
    ClassDB::bind_method(D_METHOD("start"), &MossTimer::start);
    ClassDB::bind_method(D_METHOD("stop"), &MossTimer::stop);
    ClassDB::bind_method(D_METHOD("pause"), &MossTimer::pause);
    ClassDB::bind_method(D_METHOD("resume"), &MossTimer::resume);
    ClassDB::bind_method(D_METHOD("reset"), &MossTimer::reset);
    ClassDB::bind_method(D_METHOD("is_running"), &MossTimer::is_running);
    ClassDB::bind_method(D_METHOD("is_paused"), &MossTimer::is_paused);
    ClassDB::bind_method(D_METHOD("get_remaining"), &MossTimer::get_remaining);
    ClassDB::bind_method(D_METHOD("get_duration"), &MossTimer::get_duration);
    ClassDB::bind_method(D_METHOD("is_repeating"), &MossTimer::is_repeating);
    ADD_SIGNAL(MethodInfo("timeout"));
}

void MossTimer::configure(double duration_seconds, bool repeating) {
    duration_ = static_cast<float>(std::max(0.0, duration_seconds));
    repeating_ = repeating;
    timer_ = std::make_unique<::Timer>(duration_, repeating_);
    timer_->setCallback([this]() { emit_signal("timeout"); });
}

void MossTimer::tick(double delta_seconds) {
    if (timer_ && delta_seconds > 0.0) {
        timer_->update(static_cast<float>(std::min(delta_seconds, 10.0)));
    }
}

void MossTimer::start() { if (!timer_) configure(duration_, repeating_); else timer_->reset(); }
void MossTimer::stop() { if (timer_) timer_->stop(); }
void MossTimer::pause() { if (timer_) timer_->pause(); }
void MossTimer::resume() { if (timer_) timer_->resume(); }
void MossTimer::reset() { if (!timer_) configure(duration_, repeating_); else timer_->reset(); }
bool MossTimer::is_running() const { return timer_ && timer_->isRunning(); }
bool MossTimer::is_paused() const { return timer_ && timer_->isPaused(); }
double MossTimer::get_remaining() const { return timer_ ? timer_->getRemaining() : duration_; }
