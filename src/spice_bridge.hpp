#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

namespace spice_bridge
{
using insert_coin_fn = int (__cdecl*)(uint8_t);
using set_button_fn = int (__cdecl*)(uint32_t, bool, float);
using set_keypad_fn = int (__cdecl*)(uint8_t, char);
using insert_card_fn = int (__cdecl*)(uint8_t, const char*);

inline HMODULE module()
{
    return ::GetModuleHandleW(L"iidx_btools_spice.dll");
}

inline bool insert_coin(uint8_t amount = 1)
{
    const auto dll = module();
    const auto fn = dll ? reinterpret_cast<insert_coin_fn>(
        ::GetProcAddress(dll, "subscreen_spice_insert_coin")) : nullptr;
    return fn && fn(amount) == 0;
}

inline bool set_button(uint32_t button_id, bool pressed)
{
    const auto dll = module();
    const auto fn = dll ? reinterpret_cast<set_button_fn>(
        ::GetProcAddress(dll, "subscreen_spice_set_button")) : nullptr;
    return fn && fn(button_id, pressed, pressed ? 1.f : 0.f) == 0;
}

inline bool set_keypad(uint8_t unit, char key)
{
    const auto dll = module();
    const auto fn = dll ? reinterpret_cast<set_keypad_fn>(
        ::GetProcAddress(dll, "subscreen_spice_set_keypad")) : nullptr;
    return fn && fn(unit, key) == 0;
}

inline bool insert_card(uint8_t unit, const char* card_id)
{
    const auto dll = module();
    const auto fn = dll ? reinterpret_cast<insert_card_fn>(
        ::GetProcAddress(dll, "subscreen_spice_insert_card")) : nullptr;
    return fn && fn(unit, card_id) == 0;
}
}
