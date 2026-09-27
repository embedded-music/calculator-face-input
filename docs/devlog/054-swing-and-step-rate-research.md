# Research percentage swing and triplet step rates

## Goal

Define a defensible swing model before changing the sequencer clock, and
determine how that model should interact with the existing straight and
triplet step rates.

This chapter distinguishes three concepts that hardware interfaces sometimes
place beside one another:

- **tempo**: the duration of a quarter-note beat in BPM;
- **step rate**, **scale**, or **time division**: the musical duration of one
  sequencer step;
- **swing** or **shuffle**: an alternating displacement applied to a regular
  subdivision.

## Percentage swing convention

A widely used hardware convention expresses swing from 50% through 75%. For
two steps whose straight duration is `T`, the percentage `p` divides their
unchanged combined duration `2T`:

```text
long interval  = 2T * p
short interval = 2T * (1 - p)
```

The equivalent delay applied to the second step is:

```text
offbeat delay = T * (2p - 1)
```

The first onset remains on the straight grid, the second is delayed, and the
following pair begins at its original deadline. Swing therefore changes the
internal spacing without changing the pair, beat, or pattern duration.

| Swing | Pair division | Long:short ratio | Interpretation |
| ---: | ---: | ---: | --- |
| 50% | 50% + 50% | 1:1 | straight |
| 55% | 55% + 45% | 11:9 | light swing |
| 60% | 60% + 40% | 3:2 | moderate swing |
| 66.7% | 66.7% + 33.3% | 2:1 | triplet-like swing |
| 70% | 70% + 30% | 7:3 | hard swing |
| 75% | 75% + 25% | 3:1 | maximum shuffle in this convention |

At 120 BPM with a straight sixteenth-note step, `T` is 125 ms:

| Swing | First interval | Second interval | Second-step delay |
| ---: | ---: | ---: | ---: |
| 50% | 125.0 ms | 125.0 ms | 0.0 ms |
| 60% | 150.0 ms | 100.0 ms | 25.0 ms |
| 66.7% | 166.7 ms | 83.3 ms | 41.7 ms |
| 75% | 187.5 ms | 62.5 ms | 62.5 ms |

The 75% ceiling is practical rather than mathematical. It already produces a
strong 3:1 rhythm. Continuing toward 100% collapses the short interval toward
zero and makes neighboring steps nearly simultaneous.

Arturia documents 50% as equal timing and 75% as maximum shuffle in the
[DrumBrute Impact manual][arturia-drumbrute]. Behringer documents equal timing
at 50%, triplet placement near 66%, and a 50--75 range in the
[DeepMind 12D manual][behringer-deepmind]. The M-VAVE FM1 manual exposes the
same 50--75% range but does not document its timing equation. The agreement
between better-documented implementations is sufficient for this experiment
to adopt percentage swing as a de facto convention without claiming exact FM1
compatibility before measurement.

## How musicians communicate swing

Conventional notation rarely uses percentages. A chart usually writes
`Swing`, `Swing 8ths`, `Swing 16ths`, `Shuffle`, or a style such as `Medium
swing` or `Half-time shuffle`. It may show an equivalence indicating that two
written eighth notes are performed approximately as a long and short triplet
pair.

The familiar triplet interpretation is approximately 2:1, or 66.7% in the
hardware convention. Human swing is not necessarily a fixed ratio: tempo,
articulation, dynamics, genre, and interaction between musicians shape the
feel. Drummers use the same swing and shuffle terminology, while drum notation
also communicates cymbal pattern, backbeat, accents, ghost notes, and sticking.

Percentages are principally a deterministic sequencer and DAW control. Useful
display annotations could connect the machine parameter to musician language:

```text
50%  STRAIGHT
67%  TRIPLET
75%  HARD
```

These names are guides, not replacements for the numeric value.

## Meaning of `T` rates

`T` denotes a triplet note value. Three triplet steps occupy the duration that
two straight steps of the same written denominator would occupy.

| Rate | Steps per quarter-note beat | Duration at 120 BPM |
| --- | ---: | ---: |
| `1/2` | 0.5 | 1000.00 ms |
| `1/2T` | 0.75 | 666.67 ms |
| `1/4` | 1 | 500.00 ms |
| `1/4T` | 1.5 | 333.33 ms |
| `1/8` | 2 | 250.00 ms |
| `1/8T` | 3 | 166.67 ms |
| `1/16` | 4 | 125.00 ms |
| `1/16T` | 6 | 83.33 ms |
| `1/32` | 8 | 62.50 ms |
| `1/32T` | 12 | 41.67 ms |

A true triplet rate and triplet-like swing are related but not identical:

```text
1/8T:            three equal onsets per beat
1/8 at 66.7%:    two unequal onsets per beat
```

Arturia explicitly describes this distinction in the
[MiniLab 3 manual][arturia-minilab].

Changing the step rate also changes the musical duration of this sequencer's
fixed sixteen-step pattern. At `1/16`, the pattern is one four-beat bar. At
`1/16T`, it is `16/6`, or 2 2/3 quarter-note beats. This is normal for a
rate-based step sequencer, but a conventional triplet-grid 4/4 bar would need
24 steps, or a shorter 12-step half-bar.

## Manufacturer rate ranges

Manufacturers variously call this parameter rate, scale, sync rate, or time
division. `T` is a common compact label, although some interfaces use a
separate Triplet switch.

| Product | Documented sequencer/arp divisions | Swing or shuffle |
| --- | --- | --- |
| M-VAVE FM1 | `1/1` through `1/32T`, including straight and triplet choices used as the model for this experiment | 50--75%; combination rule undocumented |
| Arturia MicroFreak | whole, `1/2`, `1/2T`, then straight and triplet values through `1/32T` | swing available; combination rule undocumented |
| Arturia KeyStep 37 | `1/4`, `1/8`, `1/16`, `1/32` and their `T` variants | 50--75% swing |
| Arturia BeatStep Pro | `1/4`, `1/8`, `1/16`, `1/32`, with a separate Triplet selection | 50--75% swing per sequencer |
| Arturia MiniFreak | straight and dotted values from half notes, with triplets from `1/4T` through `1/32T` | 50--75%; combination rule undocumented |
| Roland SE-02 | `1/4`, `1/8`, `1/16` and their `T` variants | shuffle moves even steps |
| Roland SH-4d | `1/8`, `1/16`, `1/32`, `1/4T`, `1/8T`, `1/16T` | shuffle moves even steps |
| Roland GAIA 2 | `1/1` through `1/32`, plus `1/1T` through `1/16T` | shuffle moves even steps; no documented triplet exception |
| Novation Launchpad Pro MK3 | `1/4` through `1/32`, each with a `T` variant | pattern swing available |

The slower rates are established but not universal. In particular:

- Roland GAIA 2 includes `1/1`, `1/2`, `1/1T`, and `1/2T` in its
  [sequencer scale table][roland-gaia];
- Arturia MicroFreak includes a whole note, `1/2`, and `1/2T` in its
  [sequencer/arpeggiator divisions][arturia-microfreak];
- compact pad and keyboard sequencers often begin at `1/4` because their
  performance focus favors shorter patterns.

The current calculator sequencer offers:

```text
1/1, 1/2, 1/4, 1/4T, 1/8, 1/8T,
1/16, 1/16T, 1/32, 1/32T
```

Adding `1/2T` later would make its slow-rate progression more internally
consistent. `1/1T` is valid but has limited practical value for a fixed
sixteen-step drum pattern.

## Combining swing with a triplet rate

There is no universal documented rule. Three product-design families appear.

### Disable swing on triplet divisions

Akai explicitly states that swing is not applied when the time division is
triplet-based in the [MPD232 manual][akai-mpd232]. Under this policy, a `T`
rate always means an equal triplet grid. A stored swing amount becomes active
again after returning to a straight rate.

### Swing even steps at every division

Roland GAIA 2 and SH-4d expose triplet scales and separately document shuffle
as changing the timing of even-numbered steps. Their manuals do not state a
triplet exception. The natural interpretation is that the selected division
first establishes `T`, and shuffle then displaces alternate steps. However,
the exact combined formula is not documented, so this remains an inference
rather than a verified compatibility contract.

Pairwise swing on a triplet grid crosses the grid's three-step beat grouping:

```text
triplet beat groups:  1 2 3 | 1 2 3
swing pair groups:    1 2 | 3 1 | 2 3
```

This can be a useful 2-against-3 effect, but it is not the ordinary meaning of
triplet swing.

### Give swing an independent grid

Novation SL MkIII separates the pattern sync rate from a **Swing Sync Rate**.
Swing moves alternate positions of its own selected rate, whose triplet values
also use `T`. This is flexible and explicitly documented in the
[SL MkIII guide][novation-sl], but it adds another setting and is beyond the
current calculator UI's needs.

Arturia products commonly expose swing and triplet time divisions together,
and document both concepts clearly, but the manuals reviewed here do not state
what happens when both are selected. Availability of both controls therefore
does not by itself establish a combination policy.

## Decision for a future implementation

Use the common 50--75% pair-division formula on straight rates. Keep pair
duration constant and preserve absolute beat and pattern phase.

For the initial swing slice, follow the conservative Akai-style rule:

- straight rate: apply percentage swing to alternate steps;
- triplet rate: bypass swing;
- retain the selected swing percentage while bypassed;
- show that swing is inactive on the settings screen when a `T` rate is
  selected;
- restore the stored percentage automatically when returning to a straight
  rate.

This gives each control a clear musical role and avoids silently creating a
2-against-3 timing transformation. Applying swing to triplet rates or adding a
separate swing grid can remain later, intentional experiments.

## FM1 hardware question

The FM1 documentation confirms its range but not its `T + swing` rule. A small
measurement can resolve it without blocking our first implementation:

1. program consecutive identical notes;
2. select a triplet rate;
3. compare 50% and 75% swing;
4. record MIDI or audio transients;
5. check whether intervals remain equal or alternate long--short.

Equal intervals imply triplet bypass; alternating intervals imply pairwise
swing on the triplet grid. Exact transient measurements can also reveal its
percentage rounding.

## Sources

- [Arturia DrumBrute Impact manual][arturia-drumbrute]
- [M-VAVE FM1 manual mirror][mvave-fm1]
- [Arturia KeyStep 37 manual][arturia-keystep]
- [Arturia BeatStep Pro manual][arturia-beatstep]
- [Arturia MicroFreak manual][arturia-microfreak]
- [Arturia MiniFreak manual][arturia-minifreak]
- [Arturia MiniLab 3 manual][arturia-minilab]
- [Akai MPD232 manual][akai-mpd232]
- [Behringer DeepMind 12D manual][behringer-deepmind]
- [Roland SE-02 manual][roland-se02]
- [Roland SH-4d pattern settings][roland-sh4d]
- [Roland GAIA 2 reference manual][roland-gaia]
- [Novation Launchpad Pro MK3 sequencer guide][novation-launchpad]
- [Novation SL MkIII swing guide][novation-sl]

[akai-mpd232]: https://cdn.inmusicbrands.com/akai/attachments/MPD232/MPD232-User_Guide-v1.1.pdf
[arturia-beatstep]: https://downloads.arturia.net/products/beatstep-pro/manual/BeatStepPro_Manual_1_3_0_EN.pdf
[arturia-drumbrute]: https://downloads.arturia.net/products/drumbrute-impact/manual/drumbrute-impact_Manual_1_0_EN.pdf
[arturia-keystep]: https://downloads.arturia.net/products/keystep-37/manual/keystep-37_Manual_1_0_EN.pdf
[arturia-microfreak]: https://downloads.arturia.net/products/microfreak/manual/MicroFreak_Manual_1_3_3_EN.pdf
[arturia-minifreak]: https://downloads.arturia.net/products/minifreak/manual/minifreak_Manual_1_0_0_EN.pdf
[arturia-minilab]: https://downloads.arturia.net/products/minilab-3/manual/minilab-3_Manual_1_0_5_EN.pdf
[behringer-deepmind]: https://mediadl.musictribe.com/media/sys_master/h5a/hbd/8849803575326.pdf
[mvave-fm1]: https://device.report/manual/20413651
[novation-launchpad]: https://userguides.novationmusic.com/hc/en-gb/articles/25494505907346-Using-Launchpad-Pro-MK3-s-Sequencer
[novation-sl]: https://userguides.novationmusic.com/hc/en-gb/articles/25626797155986-SL-MkIII-Tempo-swing-view
[roland-gaia]: https://static.roland.com/assets/media/pdf/GAIA-2_reference_eng01_W.pdf
[roland-se02]: https://static.roland.com/assets/media/pdf/SE-02_eng01_W.pdf
[roland-sh4d]: https://static.roland.com/manuals/sh-4d/eng/50790868.html
