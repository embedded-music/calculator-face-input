# Per-track drum sound browser

## Goal

Use a Core B modifier layer to audition alternative sounds in musical context
and let each track keep its own sound assignment.

## Sound model

AMY still loads one legacy General MIDI drum kit, patch 258. The individual
drum instruments are selected by MIDI note, so this interaction browses drum
sounds rather than loading a separate AMY patch for every track.

The fixed catalog contains the 19 notes previously rated good or usable on the
Core Gray speaker, plus note 54 (Tambourine) from the "possibly useful" group
as a twentieth audition candidate. This gives every Calculator key one sound.
The chosen defaults are:

| Track | Initial note | Sound |
| --- | ---: | --- |
| T1 | 38 | Acoustic Snare |
| T2 | 42 | Closed Hi-Hat |
| T3 | 55 | Splash Cymbal |
| T4 | 45 | Low Tom |

These defaults form a deliberately minimal kit with four distinct roles:
backbeat, short pulse, sustained accent, and low/body anchor.

Each track stores a fixed catalog index. No dynamic allocation is introduced.

## Interaction

Select a track normally, then hold Core B and release one Calculator key. The
physical layout directly selects these sounds:

```text
AC  A.Snare     M   E.Snare     %   Hand Clap   /   Claves
7   Closed HH   8   Pedal HH    9   Open HH     *   Tambourine
4   Vibraslap   5   Open Cuica  6   Splash      -   Chinese
1   Ride 1      2   Ride 2      3   Ride Bell   +   Cowbell
.   High Agogo  0   Low Agogo   +/- Low Tom     =   Maracas
```

The rows cluster short dry backbeats, hi-hats and timekeeping, contrasting
effect accents, rides and metallic bells, then pitched hand percussion.

The footer shows the selected track, GM note, and sound name. Calculator long
values from `AC` and `=` remain ignored. Changing a sound does not add an extra
audition hit; existing active steps let it be heard immediately in the running
pattern and make comparisons possible in context.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray:

1. put a few steps on one track, hold B, and try all 20 Calculator keys;
2. confirm each change is audible on later hits without changing the pattern;
3. assign different sounds to all four tracks and confirm each assignment is
   retained when selecting other tracks;
4. confirm B + `AC` and B + `=` select their short-release sounds while their
   long values make no change;
5. release B and confirm every Calculator key resumes its normal editing role;
6. confirm the four defaults remain distinct in dense and layered patterns.

## Hardware result

An initial pass on the original Core Gray found that High Agogo (67) and Hi
Wood Block (76) sounded the same, while Low Agogo (68) and Low Wood Block (77)
also sounded the same. This confirms two aliases in the reduced AMY kit; the
earlier drum probe had already described note 77 as almost identical to 68.

The two redundant wood-block slots were replaced by Cabasa (69) and Maracas
(70), both from the probe's "possibly useful" group. Their usefulness and
distinctness remain pending hardware validation.

A later comparison found Chinese Cymbal (52) and Crash Cymbal 2 (57) almost
identical. Crash Cymbal 2 was redundant with both that sound and the
then-retained Crash Cymbal 1, so its slot was replaced by Open Triangle (81).
The triangle remains a hardware-test candidate from the probe's "possibly
useful" group.

The next comparison found Crash Cymbal 1 (49) and Splash Cymbal (55) very
similar. Splash remains because it is part of the default kit; Crash Cymbal 1
was replaced by Mute Triangle (80). Keys `4` and `5` now form an adjacent
mute/open triangle pair for hardware comparison.

Cabasa (69) then proved indistinguishable from Tambourine (54). Tambourine was
retained in the timekeeping row, and the Cabasa slot was replaced by Hi Bongo
(60) as a more contrasting hand-percussion candidate.

The two triangle articulations were then removed in favor of broader timbral
variety. Mute Triangle (80) was replaced by Claves (75), and Open Triangle (81)
was replaced by Open Cuica (79). Both replacements require validation in the
running sequencer.

Hi Bongo (60) and Vibraslap (58) were distinguishable but too similar to merit
two slots. Vibraslap was retained as the more distinctive effect, while Hi
Bongo was replaced by Low Tom (45). The earlier probe rated the tom family poor
and nearly identical internally; this test asks whether one representative tom
still adds a useful lower, rounder voice relative to the rest of this palette.

Low Tom proved useful in the running sequencer. It replaced Low Agogo as the T4
default because it provides a stronger low/body anchor. Claves moved to the
short dry-hit row, while Vibraslap moved to the effect-accent row; the sound set
itself did not otherwise change.
