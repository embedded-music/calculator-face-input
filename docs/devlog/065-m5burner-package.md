# 065 — Package the Calculator edition for M5Burner

## Goal

Turn the current Pattern 256 Calculator build into one reproducible binary and
identify the current M5Burner community-publishing workflow.

## Image choice

The pioarduino PlatformIO build produces both `firmware.bin` and
`firmware.factory.bin`. The first is only the application payload at address
`0x10000`. The factory image merges the ESP32 bootloader, partition table,
Arduino boot metadata, and application into one file flashed at address `0x0`.

`tools/package-m5burner.sh` selects the merged image, gives it a product and
semantic-version filename, and creates a SHA-256 checksum. `VERSION` is the
single release-version input. Generated files live under the ignored
`dist/m5burner/` directory.

## Publishing discovery

M5Stack's old repository-based firmware catalog was archived in 2023. The
current official documentation directs authors to sign in to M5Burner and use
`USER CUSTOM` → `Publish`. The form takes the name, version, description,
device type, GitHub URL, firmware binary, and cover image. Uploaded entries can
be burned privately and shared by code before their public status is enabled.

The repository now contains a publishing guide with proposed Pattern 256
metadata. Actual upload and publication remain manual because they use the
owner's M5Stack Community account and require a cover image plus a hardware
burn of the uploaded artifact.

## Validation

```text
just package-m5burner
cd dist/m5burner
sha256sum --check pattern256-calculator-v0.1.0.bin.sha256
```

The packaging command must rebuild successfully, create the versioned merged
binary, and produce a valid checksum. Hardware and M5Burner import checks are
explicit release gates documented in the publishing guide.
