#!/usr/bin/env bash
set -euo pipefail

BUILD="../build"

SUN_SCALE="${1:-1.0}"
PLANET_SCALE="${2:-1.0}"
DIST_SCALE="${3:-1.0}"
ANGLE_VARIATION="${4:-0}"
TIME_SCALE="${5:-1.0}"
ADD_COMET="${6:-false}"

"$BUILD/generator" sphere 0.1 20 20 "$BUILD/sphere.3d"
"$BUILD/generator" torus 0.1 0.025 16 16 "$BUILD/torus.3d"
"$BUILD/generator" patch ../patches/teapot.patch 10 "$BUILD/bezier_10.3d"

python3 "generate_solar.py" "$SUN_SCALE" "$PLANET_SCALE" "$DIST_SCALE" "$ANGLE_VARIATION" "$TIME_SCALE" "$ADD_COMET"
mv "solar_system.xml" "$BUILD/solar_system.xml"
echo "Recursos guardados em $BUILD"