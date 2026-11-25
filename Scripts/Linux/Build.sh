#!/usr/bin/env bash
set -euo pipefail

configuration="${1:-Release}"
case "${configuration}" in
    Debug | Release) ;;
    *)
        echo "Usage: Build.sh [Debug|Release]" >&2
        exit 2
        ;;
esac

cd "$(dirname "${BASH_SOURCE[0]}")/../.."
exec cmake --workflow --preset "Linux-${configuration}"