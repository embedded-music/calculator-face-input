# Publishing Pattern 256 with M5Burner

M5Burner accepts community firmware through its logged-in publishing screen.
The old `m5stack/M5Stack-Firmware` pull-request catalog is archived and is not
the current submission path.

## Build the image

Set the release version in `VERSION`, then run:

```text
just package-m5burner
```

The command builds the Calculator firmware and copies PlatformIO's merged
factory image to:

```text
dist/m5burner/pattern256-calculator-v0.1.0.bin
```

It also writes a SHA-256 checksum alongside the image. The versioned binary is
an ignored build artifact and should not be committed to the repository.

## Publish a GitHub release

After the hardware release gate passes, tag the validated revision with the
version from `VERSION` and push the tag:

```text
jj tag set pattern256-v0.1.0
jj git push --tag pattern256-v0.1.0
```

The `Release firmware` GitHub Actions workflow validates that the tag and
`VERSION` agree, runs the native tests, rebuilds the merged image, verifies its
checksum, and creates a GitHub release containing both files. Application
release tags use `pattern256-vX.Y.Z`, keeping them distinct from this
repository's `step-trigger-vX.Y.Z` package tags.

The merged image is flashed from address `0x0`. It contains:

| Address | Content |
| --- | --- |
| `0x1000` | ESP32 bootloader |
| `0x8000` | `huge_app.csv` partition table |
| `0xe000` | Arduino boot metadata |
| `0x10000` | Pattern 256 application |

Do not upload the smaller `.pio/build/drum-step-sequencer/firmware.bin` as a
standalone full-flash image. That file contains only the application and
expects to be written at `0x10000`.

## Validate before publishing

1. Flash the generated merged image at address `0x0` on an M5Stack Core Gray.
2. Confirm it boots to the `PATTERN 256` screen with the Faces Calculator
   connected.
3. Exercise pattern editing, all sixteen arrangement positions, Clone, Clear,
   tempo, swing, sound selection, and audio output.
4. Erase and flash once through M5Burner's local/custom firmware flow. This
   confirms that M5Burner interprets the merged image correctly before it is
   made public.

The command-line equivalent for the binary layout is:

```text
esptool --chip esp32 write-flash 0x0 \
  dist/m5burner/pattern256-calculator-v0.1.0.bin
```

## Upload in M5Burner

1. Sign in to M5Burner with an M5Stack Community account.
2. Open **USER CUSTOM**, then **Publish**.
3. Upload the generated `.bin` and a product cover image.
4. Use the proposed metadata below.
5. Upload it privately first and burn that uploaded copy on the target device.
6. Use **Publish** to make the validated entry public. **Share** can produce a
   share code for testing before broad publication.

Suggested initial fields:

| Field | Value |
| --- | --- |
| Name | `Pattern 256` |
| Version | value from `VERSION` |
| Device Type | M5Stack Core/Core Gray option offered by M5Burner |
| GitHub | `https://github.com/embedded-music/pattern256` |
| Firmware | generated `pattern256-calculator-vX.Y.Z.bin` |
| Description | `Four-track, sixteen-step drum sequencer for M5Stack Core Gray and the Faces Calculator. Build and chain patterns into arrangements up to 256 steps, with per-track sounds, accents, tempo, rate, and swing controls.` |

The cover image and final wording are product assets and remain outside this
build slice.

## Updating a release

Change `VERSION`, rebuild and re-run the hardware checks, then push the matching
`pattern256-vX.Y.Z` tag. In M5Burner, open the existing entry's **Detail**
action and upload the binary from the resulting GitHub release.

## Official references

- [M5Stack: Publish Firmware](https://docs.m5stack.com/en/uiflow/m5burner/publish)
- [M5Stack: Add custom firmware](https://docs.m5stack.com/en/related_documents/M5Burner)
- [Archived repository-based submission process](https://github.com/m5stack/M5Stack-Firmware)
