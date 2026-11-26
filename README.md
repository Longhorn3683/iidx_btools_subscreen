# [WIP] vefxio + eamio subscreen for iidx 12-24
<img width="1920" height="1080" alt="Capture" src="https://github.com/user-attachments/assets/ddc6eea5-0e5a-42a4-bc4e-d4a4b7b305ed" />

## Installation
- place `backend.dll`, `vefxio.dll`, `eamio.dll` in the same folder as your `gamestart-XX.bat`
- for styles that ship with `bm2dx.dll` edit `gamestart-XX.bat` as shown below
```
inject iidxhook2.dll backend.dll bm2dx.exe
```
- otherwise change it to
```
inject -K iidxhook5.dll -K backend.dll bm2dx.dll
```

### Spice2x SDK integration

Build the `iidx_btools_spice` target and load the resulting `iidx_btools_spice.dll` as a Spice2x SDK plugin. The existing injected `backend.dll` looks up the SDK plugin by this exact module name and uses its SDK v0 functions when available. It falls back to the existing Bemanitools EAMIO behavior if the Spice plugin is not loaded.

The SDK integration uses `insert_coin(1)` for the coin button, `set_keypad()` for keypad input, and `insert_card()` for card selection. Spice2x must provide the corresponding SDK functions (coin insertion requires SDK v0.3 or later). The SDK plugin and `backend.dll` must have matching process architecture.

## Features
- Ticker
- keypads
- effector sliders
- spice2x CardIO reader support
- spice2x card json support
- concentration mode

## todo
- replay recording and uploading using official XRPC apis
- scene-awareness
- add auto-releases
- improve readme

## Credits
[Radioo/aic_key_eamio](https://github.com/Radioo/aic_key_eamio) - for the initial aic CardIO implementation and general help with netcode and CI

[aixxe/omnifix](https://github.com/aixxe/omnifix) - for the memory / module management code and general help regarding hooking

[dmadison/LED-Segment-ASCII](https://github.com/dmadison/LED-Segment-ASCII) - for base subscreen font (modified by me to match beatmania)

## changelog
- v0.2.0 (27.07.2025)
	- fixed alpha in some textures as they got saved without it
	- rewrote resource+texture management
	- added attributions to their respective authors
	- Added auto-disable of windows touch accessibility visual feedback
	- Added concentration mode
- v0.1.0 (26.07.2025)
	- initial release
	- keypads, ticker, CardIO driver, effector
