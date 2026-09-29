# 066 — Keep the package consumer aligned

## Goal

Repair the CI failure introduced when the trigger-emission boundary moved out
of `TriggerPatternPlayer`, and make the archive-consumer check runnable through
the repository's normal local commands.

## Cause

Slice 062 introduced `TriggerPatternEmitter::emitStep` and removed the old
static helper from `TriggerPatternPlayer`. Package tests and Pattern 256 had
already migrated, but `ci/consumers/step-trigger` still compiled against the
removed API. The fixture is built from a freshly packed archive, so it failed
after all native tests had passed.

## Change

The consumer now includes `TriggerPatternEmitter.h` and calls its `emitStep`
function. `tools/test-step-trigger-consumer.sh` packages the current library
into a temporary directory and builds that same consumer against the archive.
`just test-native` runs this check after the native suites, closing the gap
between routine local verification and CI.

The temporary archive directory is removed on exit. PlatformIO's consumer
build directory remains ignored and reusable.

## Validation

```text
just test-native
just build-sequencer
```

The first command must pass the Pattern 256 tests, package tests, package the
library, and compile the isolated archive consumer. The firmware build guards
the concrete embedded integration.
