# 060 — Make Calculator builds independent of the sibling clock checkout

## Goal

Repair the first `step-trigger-v0.1.0` publication attempt, which correctly
stopped when Calculator integration tests could not resolve the local
`../musical-clock/packages/musical-clock` path on a clean GitHub runner.

## Change

The already hardware-validated alternating clock snapshot was published as
`fcz2/musical-clock@0.2.0`. Calculator native tests and firmware now consume
that registry version rather than depending on a particular sibling workspace
layout.

This decreases repository plumbing in both local configuration and CI: a clean
Calculator checkout now owns every dependency declaration needed to test and
build it.

## Validation

```text
pio test -e native
pio run -e drum-step-sequencer
```

After this commit reaches `main`, move the unpublished
`step-trigger-v0.1.0` tag to it. The tag workflow will repeat package tests,
Calculator tests, archive-consumer build, firmware build, and registry publish.
