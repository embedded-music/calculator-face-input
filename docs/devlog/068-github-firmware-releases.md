# 068: GitHub firmware releases

## Goal

Make each validated Pattern 256 firmware version downloadable from the GitHub
project page as the same merged image used by M5Burner.

## Design

The release workflow is triggered by a project-qualified
`pattern256-vX.Y.Z` tag, distinct from the repository's `step-trigger-vX.Y.Z`
package tags. It rejects malformed tags and versions that differ from
`VERSION`, runs both native test suites, and calls the existing M5Burner
packaging script. GitHub's built-in token and CLI create a release with the
merged binary and its SHA-256 checksum; no additional repository secret or
third-party release action is needed.

Tagging remains after the hardware gate. This keeps the physical-device and
M5Burner checks explicit while automating the reproducible build and public
download step.

## Verification

```text
just test-native
just package-m5burner
cd dist/m5burner
sha256sum --check pattern256-calculator-v0.1.0.bin.sha256
```

The first end-to-end GitHub validation target is pushing a matching
`pattern256-vX.Y.Z` tag after the next hardware-approved version.

## Hardware observations

No new hardware behavior is introduced. Existing Calculator Face, editing,
arrangement, playback, audio, and M5Burner burn checks remain the release gate.
