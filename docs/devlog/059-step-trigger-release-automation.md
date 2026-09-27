# Automate step-trigger releases

## Goal

Make `step-trigger` independently verifiable and publishable so an umbrella
showcase can consume a registry version rather than reaching into the
Calculator repository through a relative path.

Publishing must occur only from an explicit package tag whose version matches
the package manifest.

## Package-local validation

The four `step-trigger` tests moved from the Calculator root suite into the
package:

```text
packages/step-trigger/
├── include/
├── src/
├── test/
├── platformio.ini
├── library.json
└── README.md
```

The package can now be tested and packed without building the application:

```text
pio test -d packages/step-trigger -e native
pio pkg pack packages/step-trigger --output /tmp
```

The Calculator root suite remains an integration consumer. Its sixteen tests
exercise pattern editing, routing, playback, clock policy, and held-button
behavior through the local package dependency.

## Archive consumer

`ci/consumers/step-trigger` is a minimal native program that depends on a
package location supplied through `STEP_TRIGGER_PACKAGE`. CI points it at the
archive produced by `pio pkg pack`, not at the source directory.

The fixture constructs a twelve-position pattern, emits a strong lane-two
trigger from its final position, and checks the received event. This catches
missing public headers, source files, manifest declarations, and archive
dependency problems that an in-tree build could hide.

## Continuous integration

The normal push and pull-request workflow performs, in order:

1. package-native tests;
2. Calculator-native integration tests;
3. package archive creation;
4. isolated consumer build from that archive;
5. complete Core Gray Calculator firmware build.

The workflows use a repository-local `.platformio-home` cache in CI and pin
PlatformIO 6.2.0, matching the current package-release workflows elsewhere in
the workspace. Local project configurations point to the workspace's shared
`.platformio-home` cache.

## Tagged publication

`.github/workflows/publish-platformio.yml` responds only to tags matching:

```text
step-trigger-v*
```

Before publication it:

- extracts the version from the tag;
- requires an exact match with `library.json`;
- repeats package and Calculator native tests;
- packs the package;
- builds the consumer from the generated archive;
- builds the Calculator firmware;
- requires the `PLATFORMIO_AUTH_TOKEN` repository secret;
- publishes non-interactively to the PlatformIO Registry.

The first intended release tag is:

```text
step-trigger-v0.1.0
```

The tag should be created only after this workflow is present on the remote
repository. A failed validation or build prevents registry publication.

## Hardware evidence

The preceding extraction passed its Core Gray hardware regression before this
release path was added. The release workflow cannot reproduce that listening
and interaction test, so the result remains recorded in the extraction
chapter while CI protects the command-line behavior and firmware build.

## Verification

```text
pio test -d packages/step-trigger -e native
pio test -e native
pio pkg pack packages/step-trigger --output /tmp
STEP_TRIGGER_PACKAGE=file:///tmp/step-trigger-0.1.0.tar.gz \
  pio run -d ci/consumers/step-trigger
pio run -e drum-step-sequencer
```

The four package-native tests and sixteen Calculator integration tests passed.
The package packed as `step-trigger-0.1.0.tar.gz`, the isolated native consumer
built from that archive, and the complete Core Gray firmware build passed.

The release tag and registry publication are intentionally not part of this
slice: they require the committed workflow to be available remotely first.
