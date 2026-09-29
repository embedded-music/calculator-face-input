#!/usr/bin/env bash

set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
package_dir="${project_dir}/packages/step-trigger"
consumer_dir="${project_dir}/ci/consumers/step-trigger"
temporary_dir="$(mktemp -d)"

cleanup() {
  rm -rf "${temporary_dir}"
}
trap cleanup EXIT

version="$(python3 -c \
  'import json, sys; print(json.load(open(sys.argv[1], encoding="utf-8"))["version"])' \
  "${package_dir}/library.json")"
archive="${temporary_dir}/step-trigger-${version}.tar.gz"

pio pkg pack "${package_dir}" --output "${temporary_dir}"
test -f "${archive}"
STEP_TRIGGER_PACKAGE="file://${archive}" pio run --project-dir "${consumer_dir}"
