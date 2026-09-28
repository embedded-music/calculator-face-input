#!/usr/bin/env bash

set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
version="$(tr -d '[:space:]' < "${project_dir}/VERSION")"
build_dir="${project_dir}/.pio/build/drum-step-sequencer"
source_image="${build_dir}/firmware.factory.bin"
output_dir="${project_dir}/dist/m5burner"
output_name="pattern256-calculator-v${version}.bin"

if [[ ! "${version}" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "VERSION must contain a semantic version such as 0.1.0" >&2
  exit 2
fi

pio run --project-dir "${project_dir}" -e drum-step-sequencer

if [[ ! -f "${source_image}" ]]; then
  echo "PlatformIO did not produce ${source_image}" >&2
  exit 1
fi

mkdir -p "${output_dir}"
cp "${source_image}" "${output_dir}/${output_name}"
(
  cd "${output_dir}"
  sha256sum "${output_name}" > "${output_name}.sha256"
)

echo "M5Burner image: ${output_dir}/${output_name}"
echo "Flash address: 0x0"
echo "Version: ${version}"
