# System sounds

Generated C headers for PROGMEM playback. Do not edit by hand.

## Generate a new sound

```bash
python tools/audio2header.py my_sound.wav
```

This creates `sounds/my_sound.h` containing a `my_sound[]` PROGMEM array.

## Use in sleep.ino

```cpp
#include "sounds/my_sound.h"

PLAY_SOUND(my_sound);
```

## Options

```
--bitrate 48k         Higher quality (default: 32k)
--samplerate 44100    Higher sample rate (default: 22050)
--name custom_name    Override C array name
-o path/out.h         Override output path
```
