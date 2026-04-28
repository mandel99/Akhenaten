# UI Migration Plan

## Goal

Move menu and dialog UI to a consistent `JS-first` layout system so that:

- layout and styling live in `src/scripts/ui_*.js`
- C++ keeps data loading, actions, and game integration
- fullscreen backgrounds use one shared `cover` behavior
- centered menu windows use one shared `center_window:true` behavior
- resizing works consistently across screens

## Target Architecture

### JS owns

- root window layout
- fullscreen background layers
- panel composition
- button placement
- labels, spacing, alignment
- responsive positioning with `margin{...}`, `centerx`, `centery`, `bottom`, `right`

### C++ owns

- window registration and show/hide flow
- game state reads
- mission/scenario lists
- callbacks and actions
- dynamic data binding into JS elements
- special rendering only where the JS system cannot yet express it

## Rules For New UI

1. Every menu/dialog screen should have a `src/scripts/ui_<name>.js` file.
2. Fullscreen background screens should use:
   - `background_image : background({cover:true, fill_width:true, fill_height:true})`
3. Centered dialog windows should use:
   - `center_window : true`
4. Root windows should avoid hardcoded screen math for final placement.
5. Inner elements should avoid raw `sw()/sh()` positioning where possible.
6. Prefer relative layout inside the panel:
   - `margin{centerx...}`
   - `margin{centery...}`
   - `margin{right...}`
   - `margin{bottom...}`
7. C++ should not manually draw menu backgrounds unless the screen is a known engine-level exception.
8. If C++ must keep a custom screen flow, it should still route visual layout through `autoconfig_window`.

## Current State

The project already has a strong JS UI base in `src/scripts/ui_*.js`, but it is still mixed with older C++-drawn windows.

### Already aligned or partially aligned

- main menu
- mission briefing
- scenario selection / family history
- many option/menu dialogs
- many info/building windows

### Still mostly C++ driven or mixed

- `src/window/console.cpp`
- `src/window/difficulty_options.cpp`
- `src/window/donate_to_city.cpp`
- `src/window/hotkey_config.cpp`
- `src/window/hotkey_editor.cpp`
- `src/window/numeric_input.cpp`
- `src/window/plain_message_dialog.cpp`
- `src/window/select_list.cpp`
- `src/window/sound_options.cpp`
- `src/window/trade_opened.cpp`
- `src/window/victory_dialog.cpp`
- parts of advisor and info flows

## Migration Strategy

### Phase 1: Standardize menu/dialog windows

Focus on windows that are visually simple and mostly panel-based.

Best first candidates:

- difficulty options
- sound options
- hotkey editor
- plain message dialog
- victory dialog

These are good candidates because they are mostly classic:

- background
- one panel
- a few labels
- a few buttons

### Phase 2: Convert mixed windows to hybrid JS layout

Keep behavior in C++, move layout to JS.

Best candidates:

- donate to city
- hotkey config
- trade opened
- select list
- numeric input

### Phase 3: Normalize shared patterns

Create or refine reusable JS building blocks for:

- standard modal panel
- confirm/cancel button rows
- title/subtitle blocks
- list-with-scrollbar layouts
- fullscreen cover background screens

### Phase 4: Tackle special screens

Handle the more complex mixed-rendering windows after the common patterns are stable:

- advisor shells
- empire overlays
- building info exceptions
- remaining intermezzo / end-game / message hybrids

## Recommended Conventions

### Naming

- JS layout file: `ui_<window_name>.js`
- section name: `<window_name>`
- C++ wrapper: `<window_name>_window` or existing window class name

### Backgrounds

- fullscreen visual layer id: `background_image`
- main panel id: `background`

### Buttons

- use semantic ids like `ok`, `cancel`, `back`, `close`, `apply`, `start_mission`
- keep click handlers in C++

## Definition Of Done

A screen is considered migrated when:

- background is not manually positioned against fixed screen size
- panel is centered through `center_window:true` or intentional anchoring
- layout lives primarily in JS
- resize behavior is stable
- there is no duplicated positioning logic split between JS and C++

## Recommended Next Step

Start with a small batch of simple dialogs and make them the reference style for all later rewrites:

1. `difficulty_options.cpp`
2. `sound_options.cpp`
3. `plain_message_dialog.cpp`
4. `victory_dialog.cpp`
5. `hotkey_editor.cpp`

After that, use the same template for the rest of the old modal windows.
