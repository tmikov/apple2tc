/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

/// KBD =($C00X)[R] Keyboard Input Register:
/// Gets set to a Key's /// High-ASCII Value when a Key is pressed (e.g., KBD > 127
/// if any Key has been pressed since last cleared via KBDSTRB).
#define A2_KBD 0xC000
/// KBDSTRB =($C01X)[W] Clear Keyboard Strobe:
/// Resets Keyboard Input /// Register's high bit (from 1 to 0) so that the next
/// keypress can set KBD again.
#define A2_KBDSTRB 0xC010
/// TAPEOUT =($C02X)[R] Cassette Data Out:
/// Digital to Analog Audio.
/// Output Toggle; Read Only!--Do NOT Write to these
/// addresses [(49184~49199)= ($C020~$C02F)] (which are
/// decoded {hard-wired} as the same single bit location)
/// Toggles Audio Output
/// (creates a 'click' on Cassette-Tape recordings).
#define A2_TAPEOUT 0xC020
/// SPKR =($C03X)[R/W] Speaker Data Out:
/// Digital to Analog Audio
/// Output Toggle; Read Only!--Do NOT Write to these
/// addresses [(49200~49215)=($C030~ $C03F)] (which are
/// decoded {hard-wired} as the same single bitlocation)
/// Toggles Audio Output
/// (Your Apple's speaker' clicks' once).
#define A2_SPKR 0xC030
/// STROBE =($C04X); Outputs Strobe Pulse to Game I/O Connector
/// Any one of these 16 locations
/// [(49216~49231)=($C040~$C04F)] has the same effect
#define A2_STROBE 0xC040

/// [R/W] Sets Graphics Mode without Clearing Screen
#define A2_TXTCLR 0xC050
/// [R/W] Sets Text Mode without Resetting Scrolling Window
#define A2_TXTSET 0xC051
/// [R/W] Sets Full-Screen Graphics Mode
#define A2_MIXCLR 0xC052
/// [R/W] Sets Mixed Text & Graphics Mode  (with 4 lines of text at the bottom of the screen).
#define A2_MIXSET 0xC053
/// [R/W] Displays Page 1 without Clearing the Screen.
#define A2_LOWSCR 0xC054
/// [R/W] Displays Page 2 without Clearing the Screen.
#define A2_HISCR 0xC055
/// [R/W] Resets Page from Hi-Res to Lo-Res/Text Mode.
#define A2_LORES 0xC056
/// [R/W] Resets Page from Lo-Res/Text to Hi-Res Mode.
#define A2_HIRES 0xC057

/// [R/W] CLRAN0: Reset AN0: Toggle OFF (0VDC)
#define A2_AN0OFF 0xC058
/// [R/W] SETAN0: * Set AN0: Toggle ON (+5VDC)
#define A2_AN0ON 0xC059
/// [R/W] CLRAN1: Reset AN1: Toggle OFF (0VDC)
#define A2_AN1OFF 0xC05A
/// [R/W] SETAN1: * Set AN1: Toggle ON (+5VDC)
#define A2_AN1ON 0xC05B
/// [R/W] CLRAN2: Reset AN2: Toggle OFF (0VDC)
#define A2_AN2OFF 0xC05C
/// [R/W] SETAN2: * Set AN2: Toggle ON (+5VDC)
#define A2_AN2ON 0xC05D
/// [R/W] CLRAN3: Reset AN3: Toggle OFF (0VDC)
#define A2_AN3OFF 0xC05E
/// [R/W] SETAN3: * Set AN3: Toggle ON (+5VDC)
#define A2_AN3ON 0xC05F
