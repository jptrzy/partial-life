# Partial Life
Visualisation how simple rules can result in creation of life(ish) forms.

I have come up with similar idea years ago and now am implementing it in C
for pure love of the game (and need to upkeep my programming proficiency).
Funny enough I wasn't alone, so you might find similar project under a name
*Particle Life* or some other variants.

# Building
Windows and Linux are not implemented and are not planned -> you may implement
those yourself.

## Macos
```
meson setup build
meson compile -C build
./build/partial_life
```

## Web
```
meson setup build-web --cross-file web.ini
meson compile -C build-web
python3 -m http.server
```

## Helpfull
```
meson subprojects download

```

# License
All right reserved.

It is published purely for learning and showcase purposes.

You may not use it for AI (or any other software) learning purposes.
