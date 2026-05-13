#!/usr/bin/env python3
"""
loopify.py - Prepare an audio file for seamless looping.

Cuts the file in half (A, B), reorders as B+A with a crossfade
at the join point. When the player loops this file end-to-end,
the original midpoint becomes the seamless loop point.

Requires: ffmpeg

Usage:
  python3 tools/loopify.py input.mp3
  python3 tools/loopify.py input.wav -o output.mp3
  python3 tools/loopify.py input.flac --crossfade 4000
"""

import argparse
import json
import os
import subprocess
import sys
import tempfile


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        sys.exit(f"ffmpeg error: {r.stderr.strip()}")
    return r


def get_duration_ms(path):
    r = subprocess.run(
        ["ffprobe", "-v", "quiet", "-print_format", "json", "-show_format", path],
        capture_output=True, text=True
    )
    if r.returncode != 0:
        sys.exit(f"ffprobe error: {r.stderr.strip()}")
    info = json.loads(r.stdout)
    return int(float(info["format"]["duration"]) * 1000)


def main():
    parser = argparse.ArgumentParser(
        description="Reorder audio as B+A with crossfade for seamless looping"
    )
    parser.add_argument("input", help="Input audio file (any format ffmpeg supports)")
    parser.add_argument("-o", "--output", help="Output file (default: <input>_loop.<ext>)")
    parser.add_argument(
        "--crossfade", type=int, default=6000,
        help="Crossfade duration in ms at the B->A join (default: 6000)"
    )
    args = parser.parse_args()

    if not os.path.isfile(args.input):
        sys.exit(f"Error: {args.input} not found")

    in_ext = os.path.splitext(args.input)[1].lstrip(".").lower()
    out_ext = "mp3"  # always output MP3 for VS1053 compatibility
    if not args.output:
        stem = os.path.splitext(args.input)[0]
        args.output = f"{stem}_loop.{out_ext}"

    total_ms = get_duration_ms(args.input)
    print(f"Input: {args.input} ({total_ms / 1000:.1f}s)")

    if args.crossfade >= total_ms // 2:
        args.crossfade = total_ms // 4
        print(f"  Crossfade too long, falling back to {args.crossfade}ms")

    mid_s = total_ms / 2000.0
    xfade_s = args.crossfade / 1000.0

    print(f"  Split at {mid_s:.1f}s, crossfade {xfade_s:.1f}s")

    with tempfile.TemporaryDirectory() as tmp:
        part_a = os.path.join(tmp, f"a.{in_ext}")
        part_b = os.path.join(tmp, f"b.{in_ext}")

        # split: A = first half, B = second half
        run(["ffmpeg", "-y", "-i", args.input, "-t", str(mid_s), "-c", "copy", part_a])
        run(["ffmpeg", "-y", "-i", args.input, "-ss", str(mid_s), "-c", "copy", part_b])

        # concat B + A with crossfade at join
        run([
            "ffmpeg", "-y",
            "-i", part_b,
            "-i", part_a,
            "-filter_complex",
            f"[0:a][1:a]acrossfade=d={xfade_s}:c1=tri:c2=tri",
            args.output
        ])

    result_ms = get_duration_ms(args.output)
    print(f"Output: {args.output} ({result_ms / 1000:.1f}s)")
    print("Done.")


if __name__ == "__main__":
    main()