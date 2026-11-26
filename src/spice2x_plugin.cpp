#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <atomic>
#include <cstdint>
#include <cstring>

// Keep the SDK ABI layout in sync with Spice SDK v0. Unused slots are retained
// as opaque function pointers so later entries keep their documented offsets.
enum SpiceStatus : int
{
    SPICE_SDK_STATUS_SUCCESS = 0
};

enum SpiceLogLevel : int
{
    SPICE_SDK_LOG_LEVEL_INFO = 1
};

enum SpiceToastSeverity : int
{
    SPICE_SDK_TOAST_LEVEL_SUCCESS = 1
};

using spice_log_fn = SpiceStatus (__cdecl*)(int, const char*, const char*);
using spice_insert_card_fn = SpiceStatus (__cdecl*)(uint8_t, const char*);
using spice_set_keypad_fn = SpiceStatus (__cdecl*)(uint8_t, char);
using spice_insert_coin_fn = SpiceStatus (__cdecl*)(uint8_t);
using spice_set_button_fn = SpiceStatus (__cdecl*)(uint32_t, bool, float);
using spice_destroy_callback_fn = void (__cdecl*)(void);
using spice_init_fn = SpiceStatus (__cdecl*)(uint32_t, spice_destroy_callback_fn, void*);

struct SpiceSdkV0
{
    uint32_t size;
    spice_log_fn log;
    void* get_game_info;
    void* get_avs_info;
    void* get_button;
    spice_set_button_fn set_button;
    void* get_analog;
    void* set_analog;
    void* get_light;
    void* set_light;
    void* set_touch;
    void* clear_touch;
    spice_insert_card_fn insert_card;
    spice_set_keypad_fn set_keypad;
    void* add_toast;
    spice_insert_coin_fn insert_coin;
    void* get_module_info;
    void* get_plugin_directory;
    void* register_d3d9;
    void* hook_library;
    void* get_coin_blocker;
    void* set_coin_blocker;
};

namespace
{
constexpr uint32_t IIDX_BUTTON_SERVICE = 0;
constexpr uint32_t IIDX_BUTTON_TEST = 1;
constexpr uint32_t IIDX_BUTTON_COIN_MECH = 2;

SpiceSdkV0 g_spice{};
std::atomic_bool g_spice_ready{ false };

void __cdecl destroy_callback()
{
    g_spice_ready.store(false, std::memory_order_release);
}
}

extern "C" __declspec(dllexport) int __cdecl spice_sdk_entry_point(spice_init_fn* init)
{
    if (!init)
        return 0;

    std::memset(&g_spice, 0, sizeof(g_spice));
    g_spice.size = sizeof(g_spice);
    if (init(0, destroy_callback, &g_spice) != SPICE_SDK_STATUS_SUCCESS)
        return 0;

    g_spice_ready.store(true, std::memory_order_release);
    if (g_spice.log)
        g_spice.log(SPICE_SDK_LOG_LEVEL_INFO, "iidx_btools_subscreen", "Spice SDK bridge initialized");
    return 1;
}

extern "C" __declspec(dllexport) int __cdecl subscreen_spice_insert_coin(uint8_t amount)
{
    if (!g_spice_ready.load(std::memory_order_acquire) || !g_spice.insert_coin)
        return 2;
    return g_spice.insert_coin(amount);
}

extern "C" __declspec(dllexport) int __cdecl subscreen_spice_set_button(
    uint32_t button_id, bool pressed, float velocity)
{
    if (!g_spice_ready.load(std::memory_order_acquire) || !g_spice.set_button)
        return 2;
    return g_spice.set_button(button_id, pressed, velocity);
}

extern "C" __declspec(dllexport) int __cdecl subscreen_spice_set_keypad(uint8_t unit, char key)
{
    if (!g_spice_ready.load(std::memory_order_acquire) || !g_spice.set_keypad)
        return 2;
    return g_spice.set_keypad(unit, key);
}

extern "C" __declspec(dllexport) int __cdecl subscreen_spice_insert_card(uint8_t unit, const char* card_id)
{
    if (!g_spice_ready.load(std::memory_order_acquire) || !g_spice.insert_card)
        return 2;
    return g_spice.insert_card(unit, card_id);
}
