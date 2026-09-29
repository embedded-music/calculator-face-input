# 067 — Build without the sibling AMY checkout

## Goal

Make the Calculator firmware build from a standalone Pattern 256 checkout in
CI and in a release workspace.

## Cause

After the packed `step-trigger` consumer was repaired, Actions reached the
firmware step and exposed another local-only dependency. `platformio.ini`
selected `amy-synth-m5` from `../amy-synth-m5`, which exists in the development
workspace but not in a GitHub Actions checkout.

The local sibling currently contains unfinished 0.3.3 work. Pattern 256 does
not need that work: `AmyTriggerOutput` and the other APIs it consumes are
already available in the published 0.3.2 package.

## Change

The firmware now depends on `fcz2/amy-synth-m5@0.3.2` from the PlatformIO
Registry. The application continues to select its local `step-trigger@0.2.0`
package explicitly, so AMY's older transitive trigger dependency does not
replace the application package selected in the dependency graph.

## Validation

A clean dependency directory, containing no sibling packages, resolves this
graph and builds the firmware:

```text
M5Unified 0.2.17
amy-synth-m5 0.3.2
musical-clock 0.2.0
step-trigger 0.2.0
```

The clean firmware build and the GitHub Actions workflow are the acceptance
targets. Hardware behavior is unchanged because the consumed AMY API and
implementation are the same published adapter used before the local 0.3.3
dependency experiment.
