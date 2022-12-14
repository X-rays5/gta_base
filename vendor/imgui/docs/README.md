# Dear ImGui but made safe for GTA V

**This fork of ImGui alone is not enough to defeat the GTA V sigscanner.**

## Changes

I have removed the default font from ImGui as some bytes in the font were detected by GTA V's sigscanner.

As well as the demo file has been removed, if you really need it copy paste it into your base and us it only in single player.

## What YOU need to additionally

Depending on your development environment you'll have to make some changes to to become fully undetected.

Add the following defines to your project so certain code will be commencted from ImGui:
 - `IMGUI_DISABLE_DEMO_WINDOWS`
 - `IMGUI_DISABLE_METRICS_WINDOW`
 - `IM_ASSERT(_EXPR) ((void)(_EXPR))`
