# Partial Life [Web Build](https://jptrzy.github.io/partial-life)
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
python3 -m http.server -d build-web
```

## Helpfull
```
meson subprojects download

```

# License
All right reserved.

It is published purely for learning and showcase purposes.

You may not use it for AI (or any other software) learning purposes.

# TODO
- [ ] Incorporate [MicroUI](https://github.com/rxi/microui)
- [ ] Research Fractal techniques for optimisation reasons -> QuadTree [impl example](https://github.com/bones-ai/c-blob-survival/tree/main)
- [ ] Use shader for ball rendering -> merge them to make them more fluid like
- [ ] Shader for background grain
- [ ] Persistent error messages (with a timer for how they display)
- [ ] Load example snapshots
- [ ] Mix ball colors using some opengl functionality
